#include <stdio.h>
#include "error_manager.h"
#include "lex_analyzer.h"
#include "parser.h"

const char *LEXICAL_ERROR_MESSAGES[] = {"END OF FILE IN COMMENT",
                                        "END OF FILE IN LITERAL",
                                        "END OF FILE IN CHARACTER",
                                        "INVALID CHAR CONSTRUCTION",
                                        "WRONG NUMBER",
                                        "INVALID CHARACTER"};

const char *SYNTACTIC_ERROR_MESSAGES[] = {"UNEXPECTED TOKEN",
                                          "LBRACE OR COLON EXPECTED, UNEXPECTED",
                                          "INT, FLOAT, BOOL, CHAR OR ID EXPECTED, UNEXPECTED ",
                                          "INT, FLOAT, BOOL OR CHAR EXPECTED, UNEXPECTED ",
                                          "STATEMENT EXPECTED, UNEXPECTED ",
                                          "CASE EXPECTED, UNEXPECTED ",
                                          "EXPRESSION EXPECTED, UNEXPECTED "};

const char *SEMANTIC_ERROR_MESSAGES[] = {"CLASS ALREADY EXISTS",
                                         "VARIABLE ALREADY EXISTS",
                                         "FUNCTION ALREADY EXISTS",
                                         "STRUCT ALREADY EXISTS",
                                         "ARRAY SIZE MUST BE OF TYPE INT",
                                         "LVALUE REQUIRED AS LEFT OPERAND OF ASSIGNMENT",
                                         "CANNOT ASSIGN TO DIFFERENT TYPE",
                                         "RELATIONAL OPERATOR CAN ONLY BE USED WITH INT OR FLOAT TYPE",
                                         "ARITHMETIC OPERATOR CAN ONLY BE USED WITH INT OR FLOAT TYPE",
                                         "LOGICAL OPERATOR CAN ONLY BE USED WITH BOOL TYPE",
                                         "BINARY OPERATOR CAN ONLY BE USED WITH EXPRESSIONS OF THE SAME TYPE",
                                         "TYPE NOT DEFINED",
                                         "RETURN TYPE NOT DEFINED",
                                         "INCOMPATIBLE RETURN TYPE",
                                         "MISSING RETURN STATEMENT",
                                         "BOOLEAN EXPRESSION REQUIRED",
                                         "INT VALUE REQUIRED",
                                         "AT LEAST 1 CASE STATEMENT REQUIRED",
                                         "UNEXPECTED BREAK",
                                         "VARIABLE NOT DECLARED",
                                         "FUNCTION NOT DECLARED",
                                         "CLASS DOES NOT HAVE MEMBER",
                                         "STRUCT DOES NOT HAVE MEMBER",
                                         "UNEXPECTED ACCESS IN NOT ID TYPE",
                                         "CLASS NOT FOUND",
                                         "PARENT CLASS NOT FOUND",
                                         "INVALID USE OF 'this'",
                                         "REQUEST FOR MEMBER FROM POINTER TYPE (USE '->' INSTEAD)",
                                         "BASE OPERAND OF '->' IS NOT POINTER",
                                         "PARAMETER TYPE MISMATCH",
                                         "TOO FEW ARGUMENTS",
                                         "TOO MANY ARGUMENTS"};

typedef struct {
  int error_id;
  int line_number;
  int expected_token;
  int token_name;
} Error;

///List of found errors.
Error *lexical_list = (Error *) malloc(100 * sizeof(Error));
Error *syntactic_list = (Error *) malloc(100 * sizeof(Error));
Error *semantic_list = (Error *) malloc(100 * sizeof(Error));
int x = 0;
int u = 0;
int v = 0;

/**
* Prints an error message with the line number, according to the error_id.
*/
void print_lexical_error(int error_id, int line_number) {
  //fprintf(stderr, "-LEXICAL ERROR - %stmt IN LINE: %d\n", LEXICAL_ERROR_MESSAGES[error_id], line_number);
  if (lexical_list == NULL) { fprintf(stderr, "Not Enough Memory for lexical errors!!! "); }
  else {
    if (x != 0 && x % 100 == 0) {
      lexical_list = (Error *) realloc(lexical_list, 100 * sizeof(Error));
    }

    lexical_list[x].error_id = error_id;
    lexical_list[x].line_number = line_number;
    x++;
  }
}

/**
* Prints an error message according to the error_id.
*/
void print_syntactic_error(int expected_token, int error_id) {
  /*if (expected_token == -1) /// specific error (called by synchronize)
    fprintf(stderr, "-SYNTACTIC ERROR - %stmt TOKEN: %stmt RECEIVED IN LINE: %d\n", SYNTACTIC_ERROR_MESSAGES[error_id],
            get_token_name(get_lookahead()), get_line());
  else /// (called by eat_or_skip)
    fprintf(stderr, "-SYNTACTIC ERROR - TOKEN: %stmt EXPECTED, BUT TOKEN: %stmt RECEIVED IN LINE: %d\n",
            get_token_name(expected_token),
            get_token_name(get_lookahead()), get_line());*/
  if (syntactic_list == NULL) { fprintf(stderr, "Not Enough Memory for syntactic errors!!! "); }
  else {
    if (expected_token == -1) { /// specific error (called by synchronize)
      if (u != 0 && u % 100 == 0) {
        syntactic_list = (Error *) realloc(syntactic_list, 100 * sizeof(Error));
      }
      syntactic_list[u].error_id = error_id;
      syntactic_list[u].line_number = get_line();
      syntactic_list[u].expected_token = expected_token;
      syntactic_list[u].token_name = get_lookahead();
      u++;
    } else { /// (called by eat_or_skip)
      if (u != 0 && u % 100 == 0) {
        syntactic_list = (Error *) realloc(syntactic_list, 100 * sizeof(Error));
      }
      syntactic_list[u].error_id = error_id;
      syntactic_list[u].line_number = get_line();
      syntactic_list[u].expected_token = expected_token;
      syntactic_list[u].token_name = get_lookahead();
      u++;
    }
  }
}

/**
* Skips all tokens until it finds one that is in the sync set.
*/
void synchronize(int error_id, const int *sync_set) {
  print_syntactic_error(-1, error_id); /// specific error
  int i = 0;
  while (get_lookahead() != EOF_TOKEN) {
    while (sync_set[i] != '\0') {
      if (get_lookahead() != sync_set[i]) i++;
      else return;
    }
    i = 0;
    eat_token();
  }
}

/**
* If the current token is the expected eats the token and return the lexeme if the current token is LITERAL, INTEGER, REAL, ID or CHARACTER.
* Else prints an error message and skips all tokens until it finds one that is in the sync set.
*/
const char *eat_or_skip(int expected_token, const int *sync_set) {
  if (get_lookahead() == expected_token) {
    const char *aux = get_lookahead() == LITERAL || get_lookahead() == INTEGER || get_lookahead() == REAL ||
                      get_lookahead() == ID || get_lookahead() == CHARACTER ? get_lexeme() : NULL;
    eat_token();
    return aux;
  } else {
    print_syntactic_error(expected_token, -1); ///generic error
    int i = 0;
    while (get_lookahead() != EOF_TOKEN) {
      while (sync_set[i] != '\0') {
        if (get_lookahead() != sync_set[i]) i++;
        else return NULL;
      }
      i = 0;
      eat_token();
      printf("-->ERROR");
    }
    return NULL;
  }
}

void print_semantic_error(int error_id, int line_number) {
  ///fprintf(stderr, "-SEMANTIC ERROR - %s IN LINE: %d\n", SEMANTIC_ERROR_MESSAGES[error_id], line_number);
  if (semantic_list == NULL) { fprintf(stderr, "Not Enough Memory for semantic errors!!! "); }
  else {
    if (v != 0 && v % 100 == 0) {
      semantic_list = (Error *) realloc(semantic_list, 100 * sizeof(Error));
    }
    semantic_list[v].error_id = error_id;
    semantic_list[v].line_number = line_number;
    v++;
  }
}


void print_all_errors() {
  if (has_errors()) {
    fprintf(stderr, "\n \n ----- LIST OF FOUND ERRORS ------ \n \n");
    for (int a = 0; a < x; a++)
      fprintf(stderr, "-LEXICAL ERROR - %s, LINE: %d\n", LEXICAL_ERROR_MESSAGES[lexical_list[a].error_id],
              lexical_list[a].line_number);
    for (int b = 0; b < u; b++) {
      if (syntactic_list[b].expected_token == -1)
        fprintf(stderr, "-SYNTACTIC ERROR - %s TOKEN: %s RECEIVED, LINE: %d\n",
                SYNTACTIC_ERROR_MESSAGES[syntactic_list[b].error_id], get_token_name(syntactic_list[b].token_name),
                syntactic_list[b].line_number);
      else
        fprintf(stderr, "-SYNTACTIC ERROR - TOKEN: %s EXPECTED, BUT TOKEN: %s RECEIVED, LINE: %d\n",
                get_token_name(syntactic_list[b].expected_token), get_token_name(syntactic_list[b].token_name),
                syntactic_list[b].line_number);
    }
    for (int c = 0; c < v; c++)
      fprintf(stderr, "-SEMANTIC ERROR - %s, LINE: %d\n", SEMANTIC_ERROR_MESSAGES[semantic_list[c].error_id],
              semantic_list[c].line_number);
  }
}

int has_errors() { return x > 0 || u > 0 || v > 0; }
