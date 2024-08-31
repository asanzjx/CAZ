/*******
* file_name:CodeGeneration.h
* date:
* author:asanzjx
* description: CodeGeneration head file
*/

#pragma once

#include "Parser.h"

#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Verifier.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/GlobalVariable.h>
#include <llvm/IR/BasicBlock.h>

#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/FileSystem.h>

#include <llvm/Support/TargetSelect.h>
#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/ExecutionEngine/GenericValue.h>


class CodeGeneration {
private:

	// 1 llvm context and module
	llvm::LLVMContext _caz_llvm_context;
    llvm::Module * _caz_llvm_module;
	llvm::IRBuilder<> *_caz_builder;

public:
	PAST *_p_past = nullptr;

	CodeGeneration();
	~CodeGeneration();

	bool gen_var(bool is_global=false);
	bool gen_func();

	bool gen_llvm_ll();

	bool run_func(llvm::Function* function);

	llvm::Type *get_type_from_builder(){
		return _caz_builder->getInt32Ty();
	}
};

