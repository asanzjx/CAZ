/************
* file:main.cpp
* date:2016-12-11
* author:asanzjx
* describe:Main file
*************/

#include "Parser.h"
#include "CodeGeneration.h"

ifstream g_fp;

int main(int argc,char *argv[]){
	CodeGeneration CCodeGen;
	Parser parser;

	printf("CAZ @asanzjx\n");
	if (argc == 2)	//暂且先读取一个文件
	{
		printf("FileName:%s\n\n", argv[1]);
		g_fp.open(argv[1]);
		if (g_fp.is_open()) {
			//cout << g_fp.rdbuf() << endl;	//输出文件内所有字符串
			//g_fp.seekg(ios::beg);	//重置文件流指针
			parser.Analyse();
			parser.printAST(&(parser.past));
			CCodeGen._p_past = &(parser.past);
			CCodeGen.gen_llvm_ll();
		}else cout << "File can't open" << endl;
		g_fp.close();
	}
	else cout << "argument wrong" << endl;

	return 0;
}