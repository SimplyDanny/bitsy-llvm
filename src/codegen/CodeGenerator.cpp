#include "codegen/CodeGenerator.hpp"

#include "llvm/IR/Function.h"
#include "llvm/Support/Host.h"

#include <memory>
#include <unordered_map>

CodeGenerator::CodeGenerator(llvm::Module &module)
  : module(module)
  , builder(module.getContext())
  , had_break(false)
  , read_template(builder.CreateGlobalStringPtr("%i", "read_template", 0, &module))
  , print_template(builder.CreateGlobalStringPtr("%i\n", "print_template", 0, &module)) {
    module.setTargetTriple(llvm::sys::getDefaultTargetTriple());

    llvm::FunctionType *return_type = llvm::FunctionType::get(builder.getInt32Ty(), false);
    main_function = llvm::Function::Create(return_type, llvm::Function::ExternalLinkage, "main", &module);
    main_block = llvm::BasicBlock::Create(module.getContext(), "main_block", main_function);
}

void CodeGenerator::visit(const Program *program) {
    builder.SetInsertPoint(main_block);
    visit(program->block.get());
    builder.CreateRet(llvm::ConstantInt::get(builder.getInt32Ty(), 0));
}

void CodeGenerator::visit(const Block *block) {
    for (const auto &statement : block->statements) {
        if (had_break) {
            return;
        }
        visit(statement.get());
    }
}

void CodeGenerator::visit(const IfStatement *if_statement) {
    auto then_block = llvm::BasicBlock::Create(module.getContext(), "then_block", main_function);
    auto continuation_block = llvm::BasicBlock::Create(module.getContext(), "continuation_block", main_function);

    auto condition = create_if_condition(if_statement);
    llvm::BasicBlock *else_block;
    if (if_statement->else_block) {
        else_block = llvm::BasicBlock::Create(module.getContext(), "else_block", main_function);
        builder.CreateCondBr(condition, then_block, else_block);
    } else {
        builder.CreateCondBr(condition, then_block, continuation_block);
    }

    builder.SetInsertPoint(then_block);
    visit(if_statement->then_block.get());
    if (!had_break) {
        builder.CreateBr(continuation_block);
    }
    had_break = false;

    if (if_statement->else_block) {
        builder.SetInsertPoint(else_block);
        visit(if_statement->else_block.get());
        if (!had_break) {
            builder.CreateBr(continuation_block);
        }
        had_break = false;
    }

    builder.SetInsertPoint(continuation_block);
}

void CodeGenerator::visit(const LoopStatement *loop_statement) {
    auto loop_block = llvm::BasicBlock::Create(module.getContext(), "loop_block", main_function);
    auto after_loop_block = llvm::BasicBlock::Create(module.getContext(), "after_loop_block", main_function);
    loop_continuation_hierarchy.push(after_loop_block);
    builder.CreateBr(loop_block);

    builder.SetInsertPoint(loop_block);
    visit(loop_statement->block.get());
    if (!had_break || loop_continuation_hierarchy.size() != 1) {
        builder.CreateBr(loop_block);
    }
    loop_continuation_hierarchy.pop();
    had_break = false;

    builder.SetInsertPoint(after_loop_block);
}

void CodeGenerator::visit(const PrintStatement *print_statement) {
    auto print_function = module.getOrInsertFunction(
        "printf",
        llvm::FunctionType::get(builder.getInt32Ty(), builder.getInt8PtrTy(), true));
    std::vector<llvm::Value *> arguments{print_template, visit(print_statement->expression.get())};
    builder.CreateCall(print_function, arguments, "print");
}

void CodeGenerator::visit(const ReadStatement *read_statement) {
    auto read_function = module.getOrInsertFunction(
        "scanf",
        llvm::FunctionType::get(builder.getInt32Ty(), builder.getInt8PtrTy(), true));
    auto allocated_variable = builder.CreateAlloca(builder.getInt32Ty());
    known_variables[read_statement->variable_expression->name] = allocated_variable;
    std::vector<llvm::Value *> arguments{read_template, allocated_variable};
    builder.CreateCall(read_function, arguments, "read");
}

void CodeGenerator::visit(const AssignmentStatement *assignment_statement) {
    auto value = visit(assignment_statement->expression.get());
    auto variable_name = assignment_statement->variable->name;
    if (auto known_variable = known_variables[variable_name]) {
        builder.CreateStore(value, known_variable);
        return;
    }
    builder.CreateStore(value, allocate_variable(variable_name));
}

void CodeGenerator::visit(const BreakStatement *break_statement) {
    (void)break_statement;
    builder.CreateBr(loop_continuation_hierarchy.top());
    had_break = true;
}

llvm::Value *CodeGenerator::visit(const NumberExpression *number_expression) {
    return llvm::ConstantInt::getSigned(builder.getInt32Ty(), number_expression->value);
}

llvm::Value *CodeGenerator::visit(const VariableExpression *variable_expression) {
    if (auto known_variable = known_variables[variable_expression->name]) {
        return builder.CreateLoad(builder.getInt32Ty(), known_variable);
    }
    return builder.CreateLoad(builder.getInt32Ty(), allocate_variable(variable_expression->name));
}

llvm::Value *CodeGenerator::visit(const BinaryOperationExpression *binary_operation_expression) {
    auto lhs = visit(binary_operation_expression->left_expression.get());
    auto rhs = visit(binary_operation_expression->right_expression.get());
    switch (binary_operation_expression->operator_symbol) {
        case '+':
            return builder.CreateAdd(lhs, rhs);
        case '-':
            return builder.CreateSub(lhs, rhs);
        case '*':
            return builder.CreateMul(lhs, rhs);
        case '/': {
            auto div = builder.CreateSDiv(lhs, rhs);
            return builder.CreateIntCast(div, builder.getInt32Ty(), true);
        }
        case '%':
            return builder.CreateSRem(lhs, rhs);
        default:
            llvm_unreachable("Unknown binary operator.");
    }
}

llvm::Value *CodeGenerator::allocate_variable(const std::string &name) {
    auto current_insert_point = builder.GetInsertBlock();
    bool not_in_main_block = main_block != current_insert_point;
    if (not_in_main_block) {
        builder.SetInsertPoint(&(*main_block->getFirstInsertionPt()));
    }
    auto new_variable = builder.CreateAlloca(builder.getInt32Ty(), nullptr, name);
    builder.CreateStore(llvm::ConstantInt::get(builder.getInt32Ty(), 0), new_variable);
    known_variables[name] = new_variable;
    if (not_in_main_block) {
        builder.SetInsertPoint(current_insert_point);
    }
    return new_variable;
}

llvm::Value *CodeGenerator::create_if_condition(const IfStatement *if_statement) {
    using enum IfStatementType;
    using enum llvm::CmpInst::Predicate;
    const std::unordered_map type_predicate_mapping = {std::pair{positive, ICMP_SLT},
                                                       std::pair{zero, ICMP_EQ},
                                                       std::pair{negative, ICMP_SGT}};
    auto condition = visit(if_statement->expression.get());
    auto null = llvm::ConstantInt::get(builder.getInt32Ty(), 0);
    auto predicate = type_predicate_mapping.at(if_statement->type);
    return builder.CreateICmp(predicate, null, condition);
}
