#############
# file:Makefile
# date:
# author:asanzjx
#############

CC = clang++
LLVM_CXXFLAGS := `llvm-config --cxxflags`
LLVM_LDFLAGS := `llvm-config --ldflags --system-libs --libs core`

caz: Lexer.o Parser.o CodeGeneration.o main.o
	$(CC) -o caz Lexer.o Parser.o main.o CodeGeneration.o $(LLVM_LDFLAGS)
	rm -rf ./*.o

main.o: main.cpp
	$(CC) -c -o main.o ./main.cpp

CodeGeneration.o: CodeGeneration.cpp
	$(CC) -c -o CodeGeneration.o ./CodeGeneration.cpp $(LLVM_CXXFLAGS) -fexceptions

Parser.o: Parser.cpp
	$(CC) -c -o Parser.o ./Parser.cpp

Lexer.o: Lexer.cpp
	$(CC) -c -o Lexer.o ./Lexer.cpp

clear:
	rm -rf caz *.o *.ll
