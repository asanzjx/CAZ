/************
* file:Parser.cpp
* date:2017-12-1
* author:asanzjx
* describe:Parser
*************/

#include "Parser.h"

Parser::Parser() {
	SynErrorNum = 0;
	_token_index = -1;
}

Parser::~Parser() {
}

void Parser::SynErrorHandle(int code) {
	static const char *SynErrorTab[] = {
		"The type lost","The type wrong","ID lost","ID wrong",
		"SEMICON Lost","SEMICON wrong","assign lost","assign wrong",
		"close2 lost","close2 wrong","end lost","end wrong"
		"num lost","num wrong","literal lost","literal wrong",
		"comma lost","comma wrong",
		"expr wrong",
		"open1 lost","open1 wrong","close1 lost","close1 wrong","TOKEN_INDEX error"
	};
	SynErrorNum++;
	printf("SynErrorHandle>>>_token_index:%d,LineNumber:%d,ColNumber:%d %s\n", _token_index, _token_table[_token_index].LineNumber, _token_table[_token_index].ColNumber, SynErrorTab[code]);
	exit(-1);
}

/*
* @brief: get token with blance index
*/
Token Parser::get_token_with_blance(int key_status) {
	if (key_status == KEY_ADD)	_token_index++;
	else if (key_status == KEY_DEC)	_token_index--;
	else {
		cout << "key_status error" << endl;
		exit(1);
	}
	return _token_table[_token_index];
}


// 对 _token_index 进行复原
bool Parser::match(int need_tag, int key_status) {
	if (key_status == KEY_ADD) {
		if (get_token_with_blance().tag == need_tag) {
			_token_index--;
			return true;
		}
		else {
			_token_index--;
			return false;
		}
	}
	else if (key_status == KEY_DEC) {
		if (get_token_with_blance(KEY_DEC).tag == need_tag) {
			_token_index++;
			return true;
		}
		else {
			_token_index++;
			return false;
		}
	}
	else if (key_status == KEY_CUR) {
		if (_token_table[_token_index].tag == need_tag)	return true;
		else return false;
	}
	else {
		cout << "key_status arg error" << endl;
		exit(1);
	}
}

bool Parser::match_op() {
	if (get_token_with_blance().is_op) {
		_token_index--;
		return true;
	}else{
		_token_index--;
		return false;
	}
}

bool Parser::next_is_type() {
	if (match(KW_CHAR) || match(KW_INT) || match(KW_STR) || match(KW_VOID)) return true;
	else return false;
}

void Parser::ShuntingYard(vector<Token> *TokenV, queue<Token> *RPNQueue) {
	stack<Token> OpStack;
	Token OpToken1, OpToken2;

	//Read all Token
	for (auto Tokeniter = TokenV->begin(); next(Tokeniter) != TokenV->end(); Tokeniter++) {
		// op
		if ((*Tokeniter).is_op) {
			OpToken1 = *Tokeniter;

			while (!OpStack.empty()) {
				OpToken2 = OpStack.top();
				if (OpToken1.priority <= OpToken2.priority) {
					OpStack.pop();
					RPNQueue->push(OpToken2);
				}
				else break;
			}
			OpStack.push(OpToken1);
		}
		//num
		else RPNQueue->push((*Tokeniter));
	}

	//if not token,analyse op stack
	while (!OpStack.empty()) {
		RPNQueue->push(OpStack.top());
		OpStack.pop();
	}

#if DEBUG
	while(!RPNQueue->empty()) {
		cout << RPNQueue->front().s << endl;
		RPNQueue->pop();
	}
#endif
}

/*
* expr -> (number | id) |
* 	(number | id) | op (num | expr)
* 1.只支持四则运算，即不支持 (),[]
* 2.不支持 ++i
*/
queue<Token> Parser::expr() {
	queue<Token> ExprTokenQueue;
	vector<Token> ExprToken;

	//first expr token must be id | number
	do {
		if(match(TK_ID) || match(TK_INT)){
			/*
			ExprToken.push_back(_token_table[_token_index]);
			get_token_with_blance();
			if (_token_table[_token_index].is_op) {
				ExprToken.push_back(_token_table[_token_index]);
				get_token_with_blance();
			}else break;
			*/
			ExprToken.push_back(get_token_with_blance());

			std::cout << "expr " << _token_index << std::endl;

			if(match_op()){
				ExprToken.push_back(get_token_with_blance());
			}else if(!match(TK_SEMICOM)){
				std::cout << "expr semicom _token_index" << _token_index << std::endl;
				SynErrorHandle(SEMICON_LOST);
			}
		}else{
			SynErrorHandle(TYPE_WRONG);
		}
	} while (!match(TK_SEMICOM));
	//;
	get_token_with_blance();

	ShuntingYard(&ExprToken, &ExprTokenQueue);

	return ExprTokenQueue;
}

/*
* 
* vari -> type id = value; |
*	type id;
*/
int Parser::var_tail(VAR* vari, bool is_arg){
	int backup_token_index = _token_index;
	vari->start_index = _token_index;

	cout << "\t------var------>begin token[" << _token_index << "]: " << _token_table[_token_index].s << std::endl;
	if (next_is_type()) {
		vari->var_type = get_token_with_blance().Type;
		if(match(TK_ID)){
			vari->var_name = get_token_with_blance().s;
			std::cout << "var_name: var_type:" << vari->var_name << vari->var_type << std::endl;

			if(match(TK_SEMICOM)){
				get_token_with_blance();
				
			}else if(match(TK_ASSIGN)){
				get_token_with_blance();

				vari->is_inited = true;

				switch (vari->var_type){
				case Token_CHAR:
				{
					vari->char_value = get_token_with_blance().c;
					break;
				}
				case Token_INT:
				{
					vari->int_value = get_token_with_blance().i;
					break;
				}
				case Token_STRING:
				{
					vari->string_value = get_token_with_blance().s;
					break;
				}
				case Token_VOID:
				{
					cout << "-------vari not void-------" << endl;
					break;
				}
				default:
				{
					cout << "-------vari err-------" << endl;
					goto VARI_BAK;
					break;
				}
				}

				if(!is_arg){
					if (!match(TK_SEMICOM))
					{
						SynErrorHandle(SEMICON_LOST);
					}
					get_token_with_blance();
				}
			}else{
				if(!is_arg){
					goto VARI_BAK;
				}
			}

		}else{
			cout << "-------vari err not tk_id-------" << endl;
			goto VARI_BAK;
		}
	}else {
VARI_BAK:
		std::cout<< "[-] vari bak" << std::endl;
		vari = nullptr;
		return backup_token_index;
	}

	cout << "\t------vari------>end token_index: " << _token_index << std::endl;
	return _token_index;
}

/*
* statement -> id = expr() / value; | 
*	while_statement |
*	if_statement |
*	return id / expr |
*/
int Parser::state_tail(Statement* statem){
	int backup_token_index = _token_index;
	statem->start_index = _token_index;

	cout << "------statem------>begin";
	if(match(KW_RETURN)){
		get_token_with_blance();
		statem->statement_kind = STATE_RET;

		statem->ret_statement = new RetStatement;

		if (match(TK_ID)) {
			statem->ret_statement->ret_id = get_token_with_blance();
			if (!match(TK_SEMICOM))
			{
				SynErrorHandle(SEMICON_LOST);
			}
			get_token_with_blance();
			
		}else{
			statem->ret_statement->expr_queue = expr();
		}

	}else if(match(TK_ID)){
		statem->statement_kind = STATE_ASSIGN;
		statem->assign_statement = new AssignStatement;
		
		statem->assign_statement->left_id_name = get_token_with_blance().s;
		VarMapTable::iterator var_iter = statem->local_var_table->find(statem->assign_statement->left_id_name);
		if(var_iter != statem->local_var_table->end()){
			// to update
		}else{
			// invaild TK_ID
		}

		// id = expr() | value;
		if (match(TK_ASSIGN)){
			get_token_with_blance();
#if DEBUG
			std::cout << "BC statement expr _token_index:" << _token_index << std::endl;
#endif
			statem->assign_statement->expr_queue = expr();
		}else{
			SynErrorHandle(ASSIGN_LOST);
		}
	}else if(match(KW_WHILE)){
		// while statement
		// todo...
	}else if(match(KW_IF)){
		// if statement
		// todo...
	}else{
STATEMENT_BACK:
		statem = nullptr;
		return backup_token_index;
	}
	cout << "------statem------>end" << std::endl;
	return _token_index;
}

/*
* func -> type id ( ) { } ; |
*	type id ( args ) { } ;
*/
int Parser::func_tail(FUNC *funct) {
	int backup_token_index = _token_index;
	funct->start_index = _token_index;

	cout << "------func------>begin _token[" << _token_index << "]: " << _token_table[_token_index].s << std::endl;
	if (next_is_type()) {
		funct->ret_type = get_token_with_blance().Type;

		if (match(TK_ID) || match(KW_MAIN)) {
			if(match(KW_MAIN)){
				// is_main
				funct->is_main = true;	
			}
			funct->fun_name = get_token_with_blance().s;

			if (match(TK_OPEN1)) {
				get_token_with_blance();
				//args
				do {
					VAR *func_arg = new VAR;
					
					// if(_token_index != var_tail(func_arg, true)){
					// 	std::cout << "func args _token[" << _token_index << "]: " << _token_table[_token_index].s << std::endl;
					// 	SynErrorHandle(TOKEN_INDEX_ERR);
					// }
					int var_tari_ret = var_tail(func_arg, true);
					if (_token_index == var_tari_ret) {
						func_arg->scope = MAIN_SCOPE;
						func_arg->is_arg = true;
						funct->args_num++;
						funct->var_table.insert(VarMapTable::value_type(func_arg->var_name, *func_arg));
					}else{
						std::cout << "cur  _token_index: " << _token_index << std::endl;
						SynErrorHandle(TOKEN_INDEX_ERR);
					}

					if(match(TK_COMMA)){
						get_token_with_blance();
					}

				} while (!match(TK_CLOSE1));
				get_token_with_blance();

				if (match(TK_BEGIN)) {
					get_token_with_blance();

					funct->func_type = FUNC_DEF;

					do {
						// var
						if(next_is_type()){
							VAR *func_var = new VAR;
							int var_tari_ret = var_tail(func_var);
							if(_token_index != var_tari_ret){
								SynErrorHandle(TOKEN_INDEX_ERR);
							}
							if(funct->is_main)	func_var->scope = MAIN_SCOPE;
							else func_var->scope = LOCAL_SCOPE;
							funct->var_table.insert(VarMapTable::value_type(func_var->var_name, *func_var));
						}
						
						// statement block
						Statement *func_statement = new Statement;
						func_statement->local_var_table = &(funct->var_table);
						func_statement->global_var_table = &(past.global_var_table);
						int state_tail_ret = state_tail(func_statement);
						if(_token_index != state_tail_ret){
							SynErrorHandle(TOKEN_INDEX_ERR);
						}
						func_statement->scope = MAIN_SCOPE;

						funct->statement_queue.push(*func_statement);

					} while (!match(TK_END));
					get_token_with_blance();

				}else if (match(TK_SEMICOM)) {
					// 函数声明
					funct->func_type = FUNC_DEC;
				}
				else {
					SynErrorHandle(SEMICON_LOST);
				}
			}
			else goto FUNC_BAK;
		}else{
			std::cout << "Func tk_id back" << std::endl;
			goto FUNC_BAK;
		}
	}else {
FUNC_BAK:
		std::cout << "Func back" << std::endl;
		funct = nullptr;
		return backup_token_index;
	}

	cout << "------func------>end";
	return _token_index;
}

/*
* segment -> func | var
* segment -> type id; |
*	type id = KW_const ; |
*	type id ( ) ; |
*	type id ( args ) ; |
*	type id ( ) { } ; |
*	type id ( args ) { } ;
*/
void Parser::Segment() {
	int cur_token_index = _token_index;
	cout << "***Segment begin***" << endl;
	FUNC funct;
	VAR global_var;

	if (cur_token_index != func_tail(&funct)) {
		past.func_table.insert(FuncMapTable::value_type(funct.fun_name, funct));

		if (funct.is_main)	past.main_func = &(past.func_table["main"]);

	}else if(cur_token_index != var_tail(&global_var)){
		global_var.scope = GLOBAL_SCOPE;
		past.global_var_table.insert(VarMapTable::value_type(global_var.var_name, global_var));
	}else {
		cout << "segment syn error";
		exit(1);
	}

	cout << "***Segment end***" << endl;
}

void Parser::Program() {
	do {
		Segment();
	} while (!match(TK_EOF));	//end read singal file
}

PAST Parser::Analyse() {
	_token_table = lexer.SaveToken();
#if DEBUG
	while (!match(TK_EOF)) {
		get_token_with_blance();
		cout << "_token_index:"<< _token_index << " " <<_token_table[_token_index].s << endl;
	}
	_token_index = -1;
#endif
	Program();

	cout << "[+] Parser END tag:" << _token_table[_token_index].tag << "\n\t Cur _token_index:" << _token_index << endl;
	cout << "[+] ***Parser analysed***" << endl;
	return past;
}

// debug
void Parser::printAST(PAST *p_past) {
	cout << "------<BEGIN>------" << endl;
	cout << "[+]Found func:" << p_past->func_table.size() << endl;
	cout << "[+]Found vars:" << p_past->global_var_table.size() << endl;

	std::cout << "[+]main func vars size:" << p_past->main_func->var_table.size() << std::endl;



	//cout << "[+]main func args:" << p_past->main_func.args_num << endl;

	cout << "------<END>------" << endl;
}