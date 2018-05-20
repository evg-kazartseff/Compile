cmake_minimum_required(VERSION 3.9)
project(Compile CXX)

set(CMAKE_CXX_STANDARD 17)
message(STATUS "GCC: Enabled c++17 support")

FIND_PACKAGE(FLEX REQUIRED)
FIND_PACKAGE(BISON)

SET(FlexInput ${CMAKE_SOURCE_DIR}/codeGen/flex/flex.l)
SET(FlexOutput ${CMAKE_SOURCE_DIR}/src/lexer.cpp)
IF(FLEX_FOUND)
    FLEX_TARGET(lexer ${FlexInput} ${FlexOutput})
ENDIF()

SET(BisonInput ${CMAKE_SOURCE_DIR}/codeGen/bison/bison.y)
SET(BisonOutput_cpp ${CMAKE_SOURCE_DIR}/src/parser.cpp)
SET(BisonOutput_h ${CMAKE_SOURCE_DIR}/inc/parser.h)
IF(BISON_FOUND)
    BISON_TARGET(parser ${BisonInput} ${BisonOutput_cpp} DEFINES_FILE ${BisonOutput_h})
ENDIF()

SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/build/bin)
ADD_FLEX_BISON_DEPENDENCY(lexer parser)

execute_process(COMMAND /bin/chmod +x "../build/Run.sh")
execute_process(COMMAND /bin/chmod +x "../build/PostRun.sh")

set(SRC src/HashTable.cpp
        src/ClassAst/Ast.cpp
        src/ASM/Asm.cpp
        src/ASM/WriteAdapter.cpp
        src/ClassAst/AstGet.cpp
        src/ClassAst/AstLink.cpp
        src/ClassAst/AstVar.cpp
        src/ClassAst/AstExpr.cpp
        src/ASM/ASM_VARS.cpp
        src/ClassAst/AstFunc.cpp
        src/ASM/StringNameGenerator.cpp)
set(INC inc/HashTable.h
        inc/Ast.h inc/Asm.h
        inc/WriteAdapter.h
        inc/Singleton.h
        inc/ASM_VARS.h
        inc/StringNameGenerator.h)

add_executable(compile ${SRC} ${INC} ${BISON_parser_OUTPUTS} ${FLEX_lexer_OUTPUTS})
target_link_libraries(compile fl y m)
