/*******
* file_name:CodeGeneration.cpp
* date:
* author:asanzjx
* description: CodeGeneration
* $ clang++ -c -o ./CodeGeneration.o `llvm-config --cxxflags --ldflags --system-libs --libs core` ./CodeGeneration.cpp
*/

#include "CodeGeneration.h"


CodeGeneration::CodeGeneration() {
	_caz_llvm_module = new llvm::Module("caz", _caz_llvm_context);
	_caz_builder = new llvm::IRBuilder<>(_caz_llvm_context);

}

CodeGeneration::~CodeGeneration() {
	delete _caz_builder;
	delete _caz_llvm_module;

	_p_past = nullptr;

}

bool CodeGeneration::gen_var(bool is_global) {
	
	// C++11
	llvm::Type *var_type = nullptr;
	for(auto it : _p_past->global_var_table){
		switch (it.second.var_type){
		case Token_INT:
		{
			var_type = llvm::Type::getInt32Ty(_caz_llvm_context);
			break;
		}
		case Token_CHAR:
		{
			// to update
			var_type = llvm::Type::getInt1Ty(_caz_llvm_context);
			break;
		}
		case Token_STRING:
		{
			// to update
			var_type = llvm::Type::getInt32Ty(_caz_llvm_context);
			break;
		}
		default:
			std::cout<<"[-] Unsupport var type" << std::endl;
			break;
		}

		// global var
		if (is_global) {
			_caz_llvm_module->getOrInsertGlobal(it.first, var_type);
		}else{

		}

	}



	return true;
}

bool CodeGeneration::gen_func() {
	int num = 0;

	//var

	//statement 

	//ret
	
	return true;
}





bool CodeGeneration::gen_llvm_ll(){
	cout << "[+] ***CodeGenerate gen_llvm_ll()***" << endl;

	// global var
	// C++11
	llvm::Type *var_type = nullptr;
	for(auto it : _p_past->global_var_table){
		switch (it.second.var_type){
		case Token_INT:
		{
			var_type = llvm::Type::getInt32Ty(_caz_llvm_context);
			break;
		}
		case Token_CHAR:
		{
			// to update
			var_type = llvm::Type::getInt1Ty(_caz_llvm_context);
			break;
		}
		case Token_STRING:
		{
			// to update
			var_type = llvm::Type::getInt32Ty(_caz_llvm_context);
			break;
		}
		default:
			std::cout<<"[-] Unsupport var type" << std::endl;
			break;
		}
		_caz_llvm_module->getOrInsertGlobal(it.first, var_type);
	}

#if DEBUG
	std::cout << "Global VAR handle" << std::endl;
#endif	

	// 2 function
	for(auto func_it : _p_past->func_table){
		// 2.1 Set arguments for function
		int func_args_nums = func_it.second.args_num;
		std::vector<llvm::Type*> func_args_vector;


		while(func_args_nums > 0){
			llvm::Type *func_arg = _caz_builder->getInt32Ty();
			func_args_vector.push_back(func_arg);

			func_args_nums--;
		}

		llvm::Type *func_ret = _caz_builder->getInt32Ty();
		switch (func_it.second.func_type)
		{
		case Token_CHAR:
			break;
		case Token_INT:
			func_ret = _caz_builder->getInt32Ty();
			break;
		case Token_STRING:
			break;
		default:
			break;
		}


		// llvm::FunctionType* functionType = llvm::FunctionType::get(func_ret, func_args_vector, false);

		std::vector<llvm::Type*> parameters(2, _caz_builder->getInt32Ty());
    	llvm::FunctionType* functionType = llvm::FunctionType::get(_caz_builder->getInt32Ty(), parameters, false);
		llvm::Function* function = llvm::Function::Create(functionType, llvm::GlobalValue::ExternalLinkage, func_it.second.fun_name, _caz_llvm_module);

		// llvm::Function::arg_iterator Args = function->arg_begin();
		// Args->setName("a");

		// 2.2 funct var
		for (auto var_iter : func_it.second.var_table) {
			VAR fuct_var = var_iter.second;
			// llvm::AllocaInst *p_var = _caz_builder->CreateAlloca(_caz_builder->getInt32Ty(), nullptr, "x.addr");
#if DEBUG
	std::cout << "[D] funct VAR handle func var: " << fuct_var.var_name << std::endl;
#endif
		}
		


		// main function, create entry
		if (func_it.second.is_main){
			// Create a block
			llvm::BasicBlock* entry_block = llvm::BasicBlock::Create(_caz_llvm_context, "entry", function);
			_caz_builder->SetInsertPoint(entry_block);
		}
		llvm::BasicBlock* base_block = llvm::BasicBlock::Create(_caz_llvm_context, "base_block", function);

#if DEBUG
	std::cout << "[D] funct VAR handle func name: " << func_it.second.fun_name << std::endl;
#endif
/*
		auto *AIX = new llvm::AllocaInst(llvm::Type::getInt32Ty(_caz_llvm_context), 0, "x.addr", base_block);
		AIX->setAlignment(llvm::Align(4));
*/

		// 2.3 statement block
		for (size_t i = 0; i < func_it.second.statement_queue.size(); i++) {
			Statement statement_item = func_it.second.statement_queue.front();
			switch (statement_item.statement_kind)
			{
			case STATE_ASSIGN:
			{


				// ConstantFP::get(TheContext, APFloat(Val));
				AssignStatement* p_assign_statement = statement_item.assign_statement;

				// VAR var =  statement_item.local_var_table->at(p_assign_statement->left_id_name);
				// std::cout << "[D] func statement assign: lef_id_name: " << var.var_name << std::endl;
#if DEBUG
				std::cout << "[D] func statement assign: assign_statement: " << p_assign_statement->left_id_name << std::endl;
#endif
				break;
			}
			case STATE_RET:
			{
#if DEBUG
				std::cout << "func statement ret " << std::endl;
#endif

				break;
			}
			case STATE_WHILE:
				break;
			default:
				break;
			}
			func_it.second.statement_queue.pop();
		}
		
		llvm::Value* arg1 = function->getArg(0);
		


		// 2.4 main ret
		// llvm::Value* main_result = _caz_builder->CreateMul(arg1, three, "multiplyResult");
		llvm::ConstantInt* main_result = _caz_builder->getInt32(0);
		_caz_builder->CreateRet(main_result);

		// 2.5 verify man function
		verifyFunction(*function);

	}

	// 3 gen llvm ll code to file
	// print the IR code to file
    std::error_code EC;
    llvm::raw_fd_ostream file("output.ll", EC, llvm::sys::fs::OF_None);
    _caz_llvm_module->print(file, nullptr);
    file.close();
    if (EC) {
        llvm::errs() << EC.message() << '\n';
        exit(1);
    }
	
	
	cout << "[+] ***CodeGenerate gen_llvm_ll end***" << endl;

	return true;
}

bool CodeGeneration::run_func(llvm::Function* function){
	// 4.JIT enginering and execuate
    llvm::InitializeNativeTarget();
    llvm::InitializeNativeTargetAsmPrinter();

    // std::unique_ptr<llvm_Module> owner(&llvm_Module);

    llvm::ExecutionEngine *jit_engine = llvm::EngineBuilder(std::unique_ptr<llvm::Module>(_caz_llvm_module)).create();

    llvm::GenericValue gv = jit_engine->runFunction(function, {});
	llvm::outs() << "Result: " << gv.IntVal;  // 获取函数调用结果
    std::cout << "Ret:" << std::endl;

	return true;
}
