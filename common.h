/************
* file:common.h
* date:2016-12-11
* author:asanzjx
* descirbe:none
**************/
#pragma once

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <string>
#include <assert.h>
#include <map>
#include <vector>
#include <stack>
#include <queue>


#define DEBUG	1


using namespace std;

/*
//TokenTag,keyword ptr "caz"
typedef enum TokenTag
{
	TK_INIT,
	//运算符及分隔符
	TK_ADD,TK_SUB,TK_MUL,TK_DIV,TK_MOD,	//+,-.*,/,%
	TK_OPEN1,TK_CLOSE1,TK_OPEN2,TK_CLOSE2,TK_BEGIN,TK_END,	//(,),[,],{,}
	TK_ASSIGN,TK_COMMA,TK_COLON,TK_SEMICOM,	//=,,,:,;
	TK_EQU,TK_NEQU,TK_LT,TK_GT,TK_LE,TK_GE,	//==,!=.<,>,<=,>=
	TK_AND,TK_OR,TK_LEA,TK_INC,TK_DEC,	//&&,||,&,++,--
	
	TK_EOF,	//THE END OF FILE
	TK_ERR,	//ERROR
	TK_ID,	//identifer
	
	//const
	TK_INT,TK_CHAR,TK_STR,TK_VOID,
	
	//Key Word
	KW_CHAR,KW_INT,KW_VOID,	//CHAR,INT,VOID
	KW_IF,KW_ELSE,KW_FOR,KW_WHILE,KW_CONTINUE,KW_BREAK,KW_RETURN,KW_SIZEOF,	//if,else,for,continue,break,return,sizeof
	
	KW_EXTERN,KW_ALLGN,KW_CDECL,KW_STDCALL	//extern,__aligen,__cdecl,__stdcall
}TokenTag;
*/

#define TK_INIT		0
//运算符及分隔符
#define TK_ADD		1	//+
#define TK_SUB		2	//-
#define TK_MUL		3	//*
#define TK_DIV		4	// /
#define TK_MOD		5	//%
#define	 TK_OPEN1	6	//(
#define TK_CLOSE1	7	//)
#define TK_OPEN2	8	//[
#define TK_CLOSE2	9	//]
#define TK_BEGIN	10	//{	
#define TK_END		11	//}
#define TK_ASSIGN	12	//=
#define TK_COMMA	13	//,
#define TK_COLON	14	//:
#define TK_SEMICOM	15	//;
#define TK_EQU		16	//==
#define TK_NEQU		17	//!=
#define TK_LT		18	//<
#define TK_GT		19	//>
#define TK_LE		20	//<=
#define TK_GE		21	//>=
#define TK_AND		22	//&&
#define TK_OR		23	//||
#define TK_LEA		24	//&
#define TK_INC		25	//++
#define TK_DEC		26	//--
//const
#define TK_INT		27
#define TK_CHAR		28
#define TK_STR		29
#define TK_VOID		30
//Key Word
#define KW_CHAR		31
#define KW_INT		32
#define KW_VOID		33

#define KW_IF		34
#define KW_ELSE		35

#define KW_FOR		36
#define KW_WHILE	37
#define KW_CONTINUE 38
#define KW_BREAK	39

#define KW_RETURN	40
#define KW_SIZEOF	41
#define KW_EXTERN	42
#define TK_ID		43	//identifer
#define TK_ERR		44	//error
#define TK_EOF		45	//the end of file
#define KW_STR	46
#define KW_CONST 47
#define KW_MAIN	48


typedef struct Token {
	int tag;
	string s = "l";
	char c;
	int i;
	int Type;
	int LineNumber;
	int ColNumber;
	bool is_op = false;	//if the operate
	int priority;	//0~10,the number more small the priority more high
}Token;


//int Arithmetic(int LeftValue, int RightValue, int OpTag);

//int RPNCount(queue<Token> *RPNQueue);

/*
int Arithmetic(int LeftValue, int RightValue, int OpTag) {
	switch (OpTag) {
	case TK_ADD:
		return LeftValue + RightValue;
	case TK_MUL:
		return LeftValue * RightValue;
	case TK_SUB:
		return LeftValue - RightValue;
	case TK_DIV:
		if (RightValue != 0) {
			return LeftValue / RightValue;
		}
		else {
			cout << "RightValue == 0" << endl;
			return -1;
		}
	default:
		return -1;
	}
}
*/






/*
* RPNCount()
*/
/*
int RPNCount(queue<Token> *RPNQueue) {
	stack<int> NumStack;
	int Num1 = -1;
	int Num2 = -1;
	int Result = 0;

	while (!RPNQueue->empty()) {
		if (RPNQueue->front().is_op) {
			//Left num
			Num1 = NumStack.top();
			NumStack.pop();

			//Right num
			Num2 = NumStack.top();
			NumStack.pop();

			Result = Arithmetic(Num2, Num1, RPNQueue->front().tag);

			NumStack.push(Result);
		}
		else NumStack.push(RPNQueue->front().i);
		RPNQueue->pop();
	}

#if 0
	cout << Result << endl;
#endif 
	return Result;
}
*/