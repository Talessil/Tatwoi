#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED

#include "ast.h"

ProgramNode *init_parser();
void end_parser();
int get_lookahead();
void eat_token();

#endif //PARSER_H_INCLUDED
