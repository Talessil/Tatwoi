#ifndef LEX_ANALYZER_H_INCLUDED
#define LEX_ANALYZER_H_INCLUDED

#include <stdio.h>

#define EOF_TOKEN 1
#define SEMICOLON 2
#define COLON 3
#define COMMA 4
#define RBRACE 5
#define LBRACE 6
#define RBRACKET 7
#define LBRACKET 8
#define RPAREN 9
#define LPAREN 10
#define MULT 11
#define MOD 12
#define PLUS 13
#define AND 14
#define AND_BIT 15
#define ALTERNATIVE 16
#define ALTERNATIVE_BIT 17
#define GTE 18
#define GT 19
#define LT 20
#define LTE 21
#define EQUAL 22
#define ASSIGN 23
#define NOTEQUAL 24
#define BANG 25
#define POINTER 26
#define MINUS 27
#define DIV 28
#define ID 29
#define LITERAL 30
#define CHARACTER 31
#define INTEGER 32
#define REAL 33
#define PERIOD 34
#define TYPEDEF 35
#define STRUCT 36
#define PUBLIC 37
#define PRIVATE 38
#define CLASS 39
#define INT 40
#define FLOAT 41
#define BOOL 42
#define CHAR 43
#define IF 44
#define ELSE 45
#define WHILE 46
#define SWITCH 47
#define BREAK 48
#define PRINT 49
#define READLN 50
#define RETURN 51
#define THROW 52
#define TRY 53
#define CATCH 54
#define CASE 55
#define NEW 56
#define TRUE 57
#define FALSE 58
#define THIS 59

char *lexeme_name();
void print_tables();
void initialize_lexeme();
const char *get_token_name(int id);
void init_lex(FILE *input_stream);
void end_lex();
int get_token();
const char *get_lexeme();
int get_line();

#endif // LEX_ANALYZER_H_INCLUDED
