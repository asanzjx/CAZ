/********
* file:Lexer.h
* date:2016-12-11
* describe:Scanner;Recognizer;lexError
* author:asanzjx
*********/
#pragma once
#include "common.h"


/*color setting*/
/*
// confict with llvm raw_ostream.h Colors
#define NOCOLOR "\033[m"
#define RED "\033[0;32;31m"
#define GREEN "\033[1;32;32m"
#define BLUE "\033[0;32;34m"
#define YELLOW "\033[1;33m"
#define WHITE "\033[1;37m"
#define LIGHT_RED    "\033[1;31m"
#define LIGHT_GREEN  "\033[1;32m"
#define LIGHT_BLUE   "\033[1;34m"
#define DARY_GRAY    "\033[1;30m"
#define CYAN         "\033[0;36m"
#define LIGHT_CYAN   "\033[1;36m"
#define PURPLE       "\033[0;35m"
#define LIGHT_PURPLE "\033[1;35m"
#define BROWN        "\033[0;33m"
#define LIGHT_GRAY   "\033[0;37m"
*/

/*
Type 1:s;
Type 2:c;
Type 3:i
*/
#define Token_VOID	0
#define Token_STRING		1
#define Token_CHAR		2
#define Token_INT		3


/**********
词法错误类别
***********/
enum LexERR{
	STR_NO_R_QUTION,	//字符串没有右引号
	NUM_BIN_TYPE,	//二进制数没有实体数据
	NUM_HEX_TYPE,	//十六进制数没有实体数据
	CHAR_NO_R_QUTION,	//字符没有右引号
	CHAR_NO_DATA,	//不支持空字符
	OR_NO_PAIR,		//||只有一个
	COMMENT_NO_END,	//多行注释错误
	TOKEN_NO_EXIST,	//词法记号不存在
	NEQU_NO_EQU	//!=没有=
};

class Lexer{
private:
	char Ch;		//保存读取的字符
	int ReadPos;	//读取当前的位置
	int tag;
	char LastCh; //last character
	int LexerErrorNum;
	map<string,int> keywords;
	Token l;
	
	//int LineLen;	//Data length in buffer
	//char Line[BUFLEN];		//Buffer
	
	
public:
	int LineNum;	//行号
	int ColNum;	//列号

	Lexer();
	void scan();
	bool scan(char need);
	void Recognizer();
	void lexError(int code);
	Token NextToken();
	int Keywords(string name);
	vector<Token> SaveToken();
	// void print_token(Token l);

	void SetColor();	//词法着色
	//void CloseFile();	//测试读取文件用
};

