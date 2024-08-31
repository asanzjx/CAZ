@echo on
@REM g++ main.cpp Lexer.cpp Parser1.cpp -o main.exe
@REM ::clang++ main.cpp Lexer.cpp Parser1.cpp -o main.exe
@REM main.exe Test.txt >out.txt


"C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.EXE" -DCMAKE_BUILD_TYPE:STRING=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE "-DCMAKE_C_COMPILER:FILEPATH=C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\Llvm\ARM64\bin\clang-cl.exe" "-DCMAKE_CXX_COMPILER:FILEPATH=C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\Llvm\ARM64\bin\clang-cl.exe" --no-warn-unused-cli -S./caz -B./caz/build -G "Visual Studio 17 2022" -T ClangCL,host=ARM64 -A ARM64
