/***********
* file:Parser.h
* date:2016-12-10
* describe:文法分析
* author:asanzjx
* 递归下降：每个非终结符为一个子程序(函数)；每个产生式对应程序内的一个分支
*/

#pragma once


#include "Lexer.h"


// key_status
#define KEY_ADD 1
#define KEY_DEC 0
#define KEY_CUR 2


// statement_kind
#define STATE_IF 1
#define STATE_ELSE 2
#define STATE_WHILE 3
#define STATE_FOR 4
#define STATE_ASSIGN 5
#define STATE_CALL 6
#define STATE_EXPR 7
#define STATE_RET 8

// action scope
#define GLOBAL_SCOPE 0
#define MAIN_SCOPE 1
//common function
#define LOCAL_SCOPE 2
//like for/while
#define CHUNK_SCOPE 3


// func type
#define FUNC_DEC 1
#define FUNC_DEF 2

/*******
* 文法错误类型
* 符号丢失和符号匹配错误
********/
enum SynERR{
	TYPE_LOST,TYPE_WRONG,		//类型
	ID_LOST,ID_WRONG,	//标识符
	SEMICON_LOST,SEMICON_WRONG,	//分号
	ASSIGN_LOST,ASSIGN_WRONG,	//=
	CLOSE2_LOST,CLOSE2_WRONG,	//]
	END_LOST,END_WRONG,	//}
	NUM_LOST,NUM_WRONG,	//数组长度
	LITERAL_LOST,LITERAL_WRONG,	//常量
	COMMA_LOST,COMMA_WRONG,	//,
	EXPR_WRONG,
	OPEN1_LOST,OPEN1_WRONG,	//(
	CLOSE1_LOST,CLOSE1_WRONG,	//)
	TOKEN_INDEX_ERR
};


using VAR = struct VarStruct{
	int start_index = -1;
	int back_index = -1;

	int scope;
	int var_type = Token_VOID;
	string var_name;

	bool is_inited = false;
	bool is_arg = false;

	int int_value;
	char char_value;
	string string_value;
};

using VarMapTable = std::map<std::string, VAR>;

typedef struct {
	string fun_name;
	vector<Token> fun_arg_v;
}call_fun_struct;



typedef struct {
	Token ret_id;
	std::queue<Token> expr_queue;
}RetStatement;

typedef struct {
	string left_id_name;
	std::queue<Token> expr_queue;
}AssignStatement;



using Statement = struct StatementStruct{
	int start_index = -1;
	int back_index = -1;
	
	//action scope
	int scope = -1;
	//exec statemen num
	int num = -1;

	//Statement kind
	int statement_kind = -1;

	VarMapTable *local_var_table = nullptr;
	VarMapTable *global_var_table = nullptr;
	
	AssignStatement *assign_statement = nullptr;
	RetStatement *ret_statement = nullptr;


};
using StatementQueue = std::queue<Statement>;

typedef struct{
	queue<Token> while_condition;
	vector<Statement> while_body_v;
}WhileBlock;

using FUNC =  struct FuncStruct {
	int start_index = -1;
	int back_index = -1;

	int func_type;
	bool is_main = false;
	int scope;
	string fun_name;

	int ret_type;
	int args_num = 0;

	VarMapTable var_table;
	StatementQueue statement_queue;
};
using FuncMapTable = std::map<std::string, FUNC>;

using PAST =  struct ProgramASTStruct {
	FuncMapTable func_table;
	VarMapTable global_var_table;
	
	//main func
	FUNC *main_func;

};

class Parser{
private:
	Lexer lexer;
	Token lo;	// 存放search到的Token类型
	int _token_index;	// _token_index 时刻表示当前Token位置
	vector<Token> _token_table;	// 获取到的Token
	enum SynERR _SynErr;
	int SynErrorNum;

public:


	PAST past;

	Parser();
	~Parser();

	void printAST(PAST *past);

	void SynErrorHandle(int code);	//语法错误处理程序

	Token get_token_with_blance(int key_status=KEY_ADD);
	bool match(int need_tag,int key_status=KEY_ADD);
	bool match_op();
	bool next_is_type();

	PAST Analyse();
	void Program();
	void Segment();
	/*
	* Shunting Yard Algorithm
	* 中缀表示法转 RPN
	*/
	void ShuntingYard(vector<Token> *TokenV, queue<Token> *RPNQueue);
	queue<Token> expr();

	int func_tail(FUNC* func);
	int var_tail(VAR* vari, bool is_arg = false);
	int state_tail(Statement* statem);
	// int whilestate(Statement* statem);

};

/*
void Parser::Segment() {
string name;

cout << "\n*Segment begin*" << endl;
if (next_is_type()) {
get_token_with_blance();
if (match(TK_ID)) {
get_token_with_blance();
name = _token_table[_token_index].s;
//func -> func decalaration | func define
if (match(TK_OPEN1,KEY_CUR)) {
cout << "this is fun" << endl;
_token_index = _token_index - 2;
FUNC funct;
func_tail(&funct);

//if main function
if (lexer.Keywords(name) == KW_MAIN) {
funct.scope = MAIN_SCOPE;
past.main_func = funct;
}
else {
funct.scope = LOCAL_SCOPE;
past.func_dec[name] = funct;
}

return;
//sym_tab.fun_tab[funct.fun_name] = funct;

}
//var -> uninited_var | inited_var
else if (match(TK_ASSIGN, KEY_CUR) || match(TK_SEMICOM, KEY_CUR)) {	//var
_token_index = _token_index - 1;

//check the same var name
if (sym_tab.var_tab_0.count(name)>0) {
cout << "the same var name" << endl;
exit(1);
}

VAR var_g;
var_g.scope = GLOBAL_SCOPE;
var_tail(&var_g);

if (match(TK_SEMICOM, KEY_CUR)) {
//sym_tab.var_tab_0[var_g.var_name] = var_g;
past.global_var_v.push_back(var_g);
}

//don't support multi def/decalaration
else {
cout << "can't use multi def/desc" << endl;
exit(1);
}
return;

}else {
cout << "segment syn error" << endl;
exit(1);
}
}else {
cout << "id lost" << endl;
exit(1);
}
}else {
cout << "type lost" << endl;
exit(1);
}
cout << "*Segment end*" << endl;
}
*/


