#ifndef POSTPROCESSOR_HPP
#define POSTPROCESSOR_HPP

#include "llvm/IR/Module.h"

struct PostProcessor {
    virtual ~PostProcessor() {}
    virtual void process(const llvm::Module* module) const = 0;
};

struct ConsoleOutput : public PostProcessor {
    void process(const llvm::Module* module) const;
};

struct ClangCompiler : public PostProcessor {
    void process(const llvm::Module* module) const;
};

struct Executer : public PostProcessor {
    void process(const llvm::Module* module) const;
};

#endif
