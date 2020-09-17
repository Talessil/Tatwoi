cmake_minimum_required(VERSION 3.8)
project(compilador_cpm)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        error_manager.c
        lex_analyzer.c
        main.c
        symbol.cpp
        table.cpp
        ast.cpp
        visitor.cpp
        parser.c
        semantic_analyzer.c
        intermediate_code.cpp
        translator.cpp
        ir_visitor.c
        generate_assembly.c
        queue.cpp)
set_source_files_properties(${SOURCE_FILES} PROPERTIES LANGUAGE CXX)
add_executable(compilador_cpm ${SOURCE_FILES})