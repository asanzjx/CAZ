/***********
* file:Lexer.cpp
* date:2016-12-11
* author:asanzjx
* describe:Scanner;Recognizer;lexError
************/

#include "Lexer.h"

extern ifstream g_fp;

Lexer::Lexer(){
    ReadPos = -1;
    LineNum = 0;
    ColNum = 0;
	keywords["int"] = KW_INT;
	keywords["char"] = KW_CHAR;
	keywords["void"] = KW_VOID;
	keywords["string"] = KW_STR;
	keywords["const"] = KW_CONST;
	keywords["while"] = KW_WHILE;
	keywords["if"] = KW_IF;
	keywords["else"] = KW_ELSE;
	keywords["for"] = KW_FOR;
	keywords["continue"] = KW_CONTINUE;
	keywords["break"]= KW_BREAK;
	keywords["return"] = KW_RETURN;
	keywords["sizeof"] = KW_SIZEOF;
	keywords["main"] = KW_MAIN;
	/*
	keywords["allgn"] = KW_ALLGN;
	keywords["cdecl"] = KW_CDECL;
	keywords["stdcall"] = KW_STDCALL;
	*/
	tag = TK_INIT;
}

int Lexer::Keywords(string name)
{
	tag = TK_ID;
	return keywords.find(name) != keywords.end() ?keywords[name]:tag;
}

/**********
* Scanner
* date:2016-12-11 改成两个函数，实现函数重载
* date:2017-3-18 重构代码，用C++文件流读取实现，这样可以修复读取无法结束文件的Bug
***********/
void Lexer::scan(){
	Ch = g_fp.get();
	ReadPos++;
	if(Ch == '\n'){
		LineNum++;
		ColNum=1;
	}else if(Ch == '\t')	ColNum+=4;
	else ColNum++;
	
#if 0
	printf("%c\n",Ch);
#endif
}

bool Lexer::scan(char need){
	scan();
	if(need){
		if(Ch != need)	return false;
		return true;
	}
	return true;
}

//输出错误
void Lexer::lexError(int code)
{
	static const char *lexErrorTable[]={
		"String don't have right quotation marks",
		"binary don't have real data",
		"hex don't have real data",
		"char don't have right signal quotation marks",
		"char is NULL",
		"Error or ",
		"multi annotation end with error",
		"Token isn't exist",
		"NEQU don't have EQU"
	};
	LexerErrorNum++;	//错误数量
	printf("lexcial Error:\n>Line:%d Col:%d %s\n",
		LineNum,ColNum,lexErrorTable[code]
	);
	exit(1);
}

/***************************
* Recognizer,识别器
* date:2016-11-13
* date:2016-12-07 关键字识别完成
* date:2016-12-09 注释和界符识别实现
* date:2016-12-11 封装到Lexer类中
* Reference:cit
* 基于硬编码方式实现
****************************/
void Lexer::Recognizer()
{
	//标识符识别
	if (Ch == '_' || Ch >= 'a' && Ch <= 'z' || Ch >= 'A' && Ch <= 'Z') {
		string name;
		do {
			name.push_back(Ch);
			scan();
		} while (Ch == '_' || Ch >= 'a' && Ch <= 'z' || Ch >= 'A' && Ch <= 'Z' || Ch >= 0 && Ch <= 9);

		ReadPos--;
		g_fp.unget();

		tag = Keywords(name);	//Search keyword
		if (tag == TK_ID) {
			l.tag = TK_ID;
			l.s = name;
			//cout<<"TK_ID:"<<l.tag<<" l.s:"<<l.s<<endl;
		}
		else { //keyword
			l.tag = tag;
			switch (tag)
			{
			case KW_CHAR:
				l.Type = Token_CHAR;
				break;
			case KW_INT:
				l.Type = Token_INT;
				break;
			case KW_STR:
				l.Type = Token_STRING;
				break;
			case KW_VOID:
				l.Type = Token_VOID;
				break;
			default:
				break;
			}
			
			l.s = name;
			//cout<<"keyword:"<<l.tag<<" l.s:"<<l.s<<endl;
		}
		return;
	}

	/*常量识别:数字常量；字符常量；字符串常量*/
	//字符本身就是一个整数，下列代码有点进制转换的感觉
	//数字常量分为十进制；十六进制；二进制和八进制
	if (Ch >= '0' && Ch <= '9') {
		int val =0;
		// printf("\nCh:%c\n", Ch);
		if(Ch != '0'){	//十进制
			do{
				val = val*10+Ch-'0';
				scan();
			}while(Ch>='0' && Ch<= '9');
			l.tag = TK_INT;
			l.i = val;
			l.s = to_string(val);
			l.Type = Token_INT;
			//cout<<"0D:"<<l.i<<endl;
		}else{	//十六进制
			if(scan('x')){
                scan();
#if 0	//Linux下无法识别
                printf("0x Test:%c\n",Ch);
#endif
				if(Ch >= '0' && Ch <= '9' || Ch>= 'A' && Ch <= 'F' || Ch>= 'a' && Ch <= 'F'){
					do{
						val = val*16+Ch;
						if(Ch>='0' && Ch <= '9')	val -= '0';
						else if( Ch>='A' && Ch<='F' )	val += 10 - 'A';
						else if( Ch>='a' && Ch<='f' )	val += 10 - 'a';
						scan();
					}while(Ch >= '0' && Ch <= '9' || Ch>= 'A' && Ch <= 'F' || Ch>= 'a' && Ch <= 'F');
				}else{
#if 0
					lexError(NUM_HEX_TYPE);
#endif
				}
				l.tag = TK_INT;
				l.i = val;
				l.s = to_string(val);
				l.Type = Token_INT;
				//cout<<"0x:"<<val<<endl;
			}else if(Ch == 'b'){	//二进制
				scan();
				if(Ch >= '0' && Ch <='1'){
					do{
						val = val * 2+Ch-'0';
						scan();
					}while(Ch >= '0' && Ch <= '1');
			
				}else{
					lexError(NUM_BIN_TYPE);
				}
				l.tag = TK_INT;
				l.i = val;
				l.s = to_string(val);
				l.Type = Token_INT;
				//cout<<"0b:"<<val<<endl;
			}else if(Ch >='0' && Ch <= '7'){		//八进制
				do{
					val = val * 8+Ch-'0';
					scan();
				}while(Ch>= '0' && Ch <= '7');
				l.tag = TK_INT;
				l.i = val;
				l.s = to_string(val);
				l.Type = Token_INT;
				//cout<<"0O:"<<val<<endl;
			}
		}
		ReadPos--;
		g_fp.unget();
		return ;
	}

	if(Ch == '\''){	//字符常量
		char c;
		scan();
		if(Ch == '\\'){	//转义字符
			scan();
			if(Ch == 'n')	c = '\n';
			else if(Ch == '\\')	c = '\\';
			else if(Ch == 't')	c = '\t';
			else if(Ch == '0')	c = '\0';
			else if(Ch == '\'')	c = '\'';
			// else if(Ch == EOF || Ch == '\n')	//文件结束，换行
			else if(g_fp.eof() || Ch == '\n')
			{
				lexError(STR_NO_R_QUTION);
			}
			else c=Ch;
		// }else if(Ch == EOF || Ch == '\n')	//文件结束，换行
		}else if(g_fp.eof() || Ch == '\n')
		{
			lexError(CHAR_NO_R_QUTION);
		}else if(Ch == '\'')//没有数据	
		{
			lexError(CHAR_NO_DATA);
			//scan();	//读掉左边的引号,不需要，因为这就是第二个引号
			l.tag = TK_ERR;
			return ;
		}else	c=Ch;	//普通字符
		
		if(scan('\'')){	//匹配左边引号
			l.tag = TK_CHAR;
			l.c = c;
			l.s = c;
			//l.s = c;
			//cout<<"Ch:"<<c<<" line:"<<LineNum<<" ColNum:"<<ColNum<<endl;
		}else{
			lexError(CHAR_NO_R_QUTION);
		}
		return ;
	}
	
	if(Ch == '"')	//字符串
	{
		string str;
		
		while(!scan('"')){
			if(Ch == '\\')	//转义
			{
				scan();
				if(Ch == 'n')	str.push_back('\n');
				else if(Ch == '\\')	str.push_back('\\');
				else if(Ch == 't')	str.push_back('\t');
				else if(Ch == '"')	str.push_back('"');
				else if(Ch == '0')	str.push_back('\0');
				else if(Ch == '\n' || Ch == '\n'){
					//异常处理
					break;
				}
				else {
					str.push_back(Ch);
				}
			// }else if(Ch == EOF || Ch == '\n')	//文件结束
			}else if(g_fp.eof() || Ch == '\n')
			{
				//异常处理
				break;
			}
			else {
				str.push_back(Ch);
			}
		}
		l.tag = TK_STR;
		l.s = str;
		//cout<<"`string:`"<<str<<endl;
		return ;
	}
	
	/*界符：单字节界符和双字节界符*/
	switch(Ch){
		case '+':
			if(scan('+')){
				l.tag = TK_INC;
				l.s = "++";
				l.is_op = true;
				l.Type = 1;
				l.priority = 3;
				//cout<<"l.s:"<<l.s<<endl;
			}else{
				l.tag = TK_ADD;
				l.c = '+';
				l.s = "+";
				l.Type = 1;
				l.is_op = true;
				l.priority = 3;
				--ReadPos;
				g_fp.unget();
				//cout<<"l.c:"<<l.c<<endl;
			}
			return ;
		case '-':
			if(scan('-')){
				l.tag = TK_DEC;
				l.s = "--";
				l.Type = 1;
				l.is_op = true;
				l.priority = 3;
			}else{
				l.tag = TK_SUB;
				l.c = '-';
				l.s = "-";
				l.Type = 2;
				l.is_op = true;
				l.priority = 3;
				--ReadPos;
				g_fp.unget();
			}
			return ;
		case '*':
			l.tag = TK_MUL;
			l.c = '*';
			l.s = "*";
			l.Type = 2;
			l.is_op = true;
			l.priority = 4;
			return ;
		case '/':	//注释处理
			scan();
			if(Ch == '/'){	//单行注释
				//printf("------------begin\nSignal annotation:");
				// while(!(Ch == '\n' || Ch == EOF)){
				while(!(Ch == '\n' || g_fp.eof())){
					scan();
					//printf("%c",Ch);
				}
				//printf("\n-------------END\n");
				NextToken();	//注释不记录，直接识别下一个标识符
			}else if(Ch == '*'){	//多行注释
				//printf("------------begin\nmulti annotation:");
				
				while(!scan(-1)){
					if(Ch == '*'){
						while(scan('*')) ;
						if(Ch == '/')	break;
					}else	;//printf("%c",Ch);
					if(ReadPos == -1)	lexError(COMMENT_NO_END);
				}
				//printf("\n-------------END\n");
				// if(Ch == EOF)	lexError(COMMENT_NO_END);
				if(g_fp.eof())	lexError(COMMENT_NO_END);
				NextToken();	//注释不分析，直接识别下一个标识符
			}else{
				l.tag = TK_DIV;
				l.c = '/';
				l.s = "/";
				l.Type = Token_CHAR;
				l.is_op = true;
				l.priority = 4;
				ReadPos--;
				g_fp.unget();
				//cout<<"TK_DIV:"<<l.tag<<" l.c:"<<l.c<<endl;	//当作除法记号
			}
			return ;
		case '%':
			l.tag = TK_MOD;
			l.c = '%';
			l.s = "%";
			l.Type = Token_CHAR;
			l.is_op = true;
			l.priority = 4;
			return ;
		case '>':
			if(scan('=')){
				l.tag = TK_GE;
				l.s = ">=";
				l.Type = Token_STRING;
				l.is_op = true;
				l.priority = 6;
			}else{
				l.tag = TK_GT;
				l.c = '>';
				l.s = ">";
				l.is_op = true;
				l.priority = 6;
				l.Type = 2;
				ReadPos--;
				g_fp.unget();
			}
			return ;
		case '<':
			if(scan('=')){
				l.tag = TK_LE;
				l.s = "<=";
				l.Type = 1;
				l.is_op = true;
				l.priority = 6;
			}else{
				l.tag = TK_LT;
				l.c = '<';
				l.s = "<";
				l.Type = 2;
				l.is_op = true;
				l.priority = 6;
				ReadPos--;
				g_fp.unget();
			}
			return ;
		case '=':
			if(scan('=')){
				l.tag = TK_EQU;
				l.s = "==";
				l.Type = 1;
				l.is_op = true;
				l.priority = 7;
			}else{
				l.tag = TK_ASSIGN;
				l.c = '=';
				l.s = "=";
				l.is_op = true;
				l.priority = 0;
				l.Type = 2;
				//cout<<"l.c:"<<l.c<<endl;
				ReadPos--;
				g_fp.unget();
			}
			return ;
		case '&':
			if(scan('&')){
				l.tag = TK_AND;
				l.s = "&&";
				l.Type = 1;
				l.is_op = true;
				l.priority = 7;
			}else{
				l.tag = TK_LEA;
				l.c = '&';
				l.Type = 2;
				ReadPos--;
				g_fp.unget();
			}
			return ;
		case '|':
			if(scan('|')){
				l.tag = TK_OR;
				l.s = "||";
				l.Type = 1;
				l.is_op = true;
				l.priority = 8;
				return ;
			}else{
				lexError(OR_NO_PAIR);
				exit(1);
			}
		case '!':
			if(scan('=')){
				l.tag = TK_NEQU;
				l.s = "!=";
				l.Type = 1;
				l.is_op = true;
				l.priority = 7;
				return ;
			}else{
				lexError(NEQU_NO_EQU);
				exit(1);
			}
		case ',':
			l.tag = TK_COMMA;
			l.c = ',';
			l.s = ",";
			l.is_op = true;
			l.priority = 10;
			return ;
		case ':':
			l.tag = TK_COLON;
			l.c = ':';
			l.s = ":";
			return ;
		case ';':
			l.tag = TK_SEMICOM;
			l.c = ';';
			l.s = ";";
			l.Type = 2;
			return ;
		case '(':
			l.tag = TK_OPEN1;
			l.c = '(';
			l.s = "(";
			l.Type = 2;
			l.is_op = true;
			l.priority = 9;
			//cout<<"l.c:"<<l.c<<endl;
			return ;
		case ')':
			l.tag = TK_CLOSE1;
			l.c = ')';
			l.s = ")";
			l.Type = 2;
			l.is_op = true;
			l.priority = 9;
			//cout<<"l.c:"<<l.c<<endl;
			return ;
		case '[':
			l.tag = TK_OPEN2;
			l.c = '[';
			l.s = "[";
			l.Type = 2;
			l.is_op = true;
			l.priority = 8;
			return ;
		case ']':
			l.tag = TK_CLOSE2;
			l.c = ']';
			l.s = "]";
			l.Type = 2;
			l.is_op = true;
			l.priority = 8;
			return ;
		case '{':
			l.tag = TK_BEGIN;
			l.c = '{';
			l.s = "{";
			l.Type = 2;
			//cout<<"l.c:"<<l.c<<endl;
			return ;
		case '}':
			l.tag = TK_END;
			l.c = '}';
			l.s = "}";
			l.Type = 2;
			//cout<<"l.c:"<<l.c<<endl;
			return ;
		default:	NextToken();//error处理，不能scan()，可能会出现error
	}
}

void Lexer::SetColor(){
	if(l.tag>= TK_ADD && l.tag <=TK_DEC){
		// printf(RED);
		printf("\033[0;32;31m");
	}else if(l.tag >= TK_EOF && l.tag <= TK_ID){
		// printf(GREEN);
		printf("\033[1;32;32m");
	}else if(l.tag >= TK_INT && l.tag <= TK_VOID){
		// printf(YELLOW);
		printf("\033[1;33m");
	}else{
		// printf(WHITE);
		printf("\033[1;37m");
	}
	cout<<l.tag<<endl;
	// printf(NOCOLOR);
	printf("\033[m");
}

Token Lexer::NextToken(){
	if(!g_fp.eof()){
		scan();
		//cout << Ch << endl;
		Recognizer();
		l.LineNumber = LineNum;
		l.ColNumber = ColNum;
	}else	l.tag = TK_EOF;
	return l;
}

vector<Token> Lexer::SaveToken() {
	vector<Token> v;
	while (NextToken().tag != TK_EOF) {
		v.push_back(l);
		memset(&l, 0, sizeof(l));	//清除结构体l内容，防止带入垃圾数据
		l.s = "l";	//解决输出到终端乱码
	}
	v.push_back(l);	//TK_EOF
	return v;
}

