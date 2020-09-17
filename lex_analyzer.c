#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "lex_analyzer.h"
#include "error_manager.h"
#include "table.h"

#define BUFFER_SIZE 4096
#define LEXEME_INCREASE 32

/**
* Struct used for building the lexemes that
* are being read by the lexical analyzer.
*/
typedef struct Lexeme {
  int qnt, pos;
  char *lex;
} lexeme;

const char *TOKENS[] = {"EOF", "SEMICOLON", "COLON", "COMMA", "RBRACE", "LBRACE", "RBRACKET",
                        "LBRACKET", "RPAREN", "LPAREN", "MULT", "MOD", "PLUS", "AND", "AND_BIT",
                        "ALTERNATIVE", "ALTERNATIVE_BIT", "GTE", "GT", "LT", "LTE", "EQUAL",
                        "ASSIGN", "NOTEQUAL", "BANG", "POINTER", "MINUS", "DIV", "ID", "LITERAL",
                        "CHARACTER", "INTEGER", "REAL", "PERIOD", "TYPEDEF", "STRUCT", "PUBLIC",
                        "PRIVATE", "CLASS", "INT", "FLOAT", "BOOL", "CHAR", "IF", "ELSE",
                        "WHILE", "SWITCH", "BREAK", "PRINT", "READLN", "RETURN", "THROW", "TRY",
                        "CATCH", "CASE", "NEW", "TRUE", "FALSE", "THIS"
};
char character;
int line_count;
int buffer_position;
FILE *arq;
char *buffer;
lexeme lex_buffer;
ReservedTable *reserved_table;
IdentifierTable *identifier_table;
LiteralTable *literal_table;
IntegerTable *integer_table;
RealTable *real_table;

/// Helper function that returns the current built lexeme.
char *lexeme_name() {
  return (lex_buffer.lex);
}

/// Returns the current line count
int get_line() {
  return line_count;
}

/// Prints all the symbol tables.
void print_tables() {
  printf("\n\n");
  reserved_table->print_table();
  identifier_table->print_table();
  literal_table->print_table();
  integer_table->print_table();
  real_table->print_table();
}

/// Initializes the lexeme buffer.
void initialize_lexeme() {
  lex_buffer.pos = 0;
  lex_buffer.lex[0] = '\0';
}

/// Returns the token name from the TOKEN array.
const char *get_token_name(int id) {
  return (TOKENS[id - 1]);
}

/// Increases the buffers and returns the next read character
char next_character() {
  // Increases lexeme_buffer
  if (lex_buffer.pos == lex_buffer.qnt - 1) {
    lex_buffer.qnt = lex_buffer.qnt + LEXEME_INCREASE;
    lex_buffer.lex = (char *) realloc(lex_buffer.lex, lex_buffer.qnt);
  }
  if (character != ' ' && character != '\t' && character != '\r' && character != '\n' &&
      character != '\"' && character != '\'') {
    lex_buffer.lex[lex_buffer.pos] = character;
    lex_buffer.pos++;
    lex_buffer.lex[lex_buffer.pos] = '\0';
  }
  // Increases buffer
  if (buffer[buffer_position] == '\0') {
    buffer_position = 0;
    int aux = fread(buffer, sizeof(char), BUFFER_SIZE - 1, arq);
    if (aux < BUFFER_SIZE - 1) buffer[aux] = EOF;
    else buffer[aux] = '\0';
  }
  return (buffer[buffer_position++]);
}

/** Helper function that calls the error manager function
* and sets the state back to 0.
*/
void print_error_and_restart(unsigned int error, int &state) {
  print_lexical_error(error, line_count);
  state = 0;
}

/** Works like the lexical analyzer constructor.
* Initialize all the fields necessary for the lex_analyzer use.
*/
void init_lex(FILE *input_stream) {
  arq = input_stream;
  line_count = 1;
  // Initialize lexeme buffer
  lex_buffer.qnt = LEXEME_INCREASE;
  lex_buffer.lex = (char *) malloc(sizeof(char) * lex_buffer.qnt);
  initialize_lexeme();
  // Initializes buffer
  buffer_position = 0;
  buffer = (char *) malloc(sizeof(char) * BUFFER_SIZE);
  int aux = fread(buffer, sizeof(char), BUFFER_SIZE - 1, arq);
  buffer[aux] = '\0';
  // Instantiates all the symbol tables and insert the reserved words in the reserved_table.
  reserved_table = new ReservedTable();
  identifier_table = new IdentifierTable();
  literal_table = new LiteralTable();
  integer_table = new IntegerTable();
  real_table = new RealTable();
  // Insert all reserved words in the ReservedTable
  reserved_table->insert("typedef", TYPEDEF);
  reserved_table->insert("struct", STRUCT);
  reserved_table->insert("public", PUBLIC);
  reserved_table->insert("private", PRIVATE);
  reserved_table->insert("class", CLASS);
  reserved_table->insert("int", INT);
  reserved_table->insert("float", FLOAT);
  reserved_table->insert("bool", BOOL);
  reserved_table->insert("char", CHAR);
  reserved_table->insert("if", IF);
  reserved_table->insert("else", ELSE);
  reserved_table->insert("while", WHILE);
  reserved_table->insert("switch", SWITCH);
  reserved_table->insert("break", BREAK);
  reserved_table->insert("print", PRINT);
  reserved_table->insert("readln", READLN);
  reserved_table->insert("return", RETURN);
  reserved_table->insert("throw", THROW);
  reserved_table->insert("try", TRY);
  reserved_table->insert("catch", CATCH);
  reserved_table->insert("case", CASE);
  reserved_table->insert("new", NEW);
  reserved_table->insert("true", TRUE);
  reserved_table->insert("false", FALSE);
  reserved_table->insert("this", THIS);
  // Calls first character
  character = next_character();
}

/** Works like the lexical analyzer destructor.
* Frees the buffers and symbol tables.
*/
void end_lex() {
  // Deallocates all the symbol tables.
  delete reserved_table;
  delete identifier_table;
  delete literal_table;
  delete integer_table;
  delete real_table;
  // Deallocates the read and lexeme buffer.
  free(buffer);
  free(lex_buffer.lex);
}

/// Automaton implementation. Returns the identified token;
int get_token() {
  int state = 0;
  while (true) {
    switch (state) {
      case 0:
        initialize_lexeme();
        switch (character) {
          case ' ':
          case '\t':
          case '\r':
            character = next_character();
            state = 0;
            break;
          case EOF:
            state = 1;
            break;
          case ';':
            character = next_character();
            state = 2;
            break;
          case ':':
            character = next_character();
            state = 3;
            break;
          case ',':
            character = next_character();
            state = 4;
            break;
          case '}':
            character = next_character();
            state = 5;
            break;
          case '{':
            character = next_character();
            state = 6;
            break;
          case ']':
            character = next_character();
            state = 7;
            break;
          case '[':
            character = next_character();
            state = 8;
            break;
          case ')':
            character = next_character();
            state = 9;
            break;
          case '(':
            character = next_character();
            state = 10;
            break;
          case '*':
            character = next_character();
            state = 11;
            break;
          case '%':
            character = next_character();
            state = 12;
            break;
          case '+':
            character = next_character();
            state = 13;
            break;
          case '&':
            character = next_character();
            state = 14;
            break;
          case '|':
            character = next_character();
            state = 17;
            break;
          case '>':
            character = next_character();
            state = 20;
            break;
          case '<':
            character = next_character();
            state = 23;
            break;
          case '=':
            character = next_character();
            state = 26;
            break;
          case '!':
            character = next_character();
            state = 30;
            break;
          case '-':
            character = next_character();
            state = 33;
            break;
          case '/':
            character = next_character();
            state = 36;
            break;
          case '\n':
            character = next_character();
            state = 40;
            break;
          case '\"':
            character = next_character();
            state = 43;
            break;
          case '\'':
            character = next_character();
            state = 47;
            break;
          case '.':
            character = next_character();
            state = 59;
            break;
          default:
            if (isalpha(character) || character == '_') {
              character = next_character();
              state = 41;
              break;
            } else if (isdigit(character)) {
              character = next_character();
              state = 51;
              break;
            } else {
              character = next_character();
              state = 61;
              break;
            }
        }
        break;
      case 1:
        return EOF_TOKEN;
      case 2:
        return SEMICOLON;
      case 3:
        return COLON;
      case 4:
        return COMMA;
      case 5:
        return RBRACE;
      case 6:
        return LBRACE;
      case 7:
        return RBRACKET;
      case 8:
        return LBRACKET;
      case 9:
        return RPAREN;
      case 10:
        return LPAREN;
      case 11:
        return MULT;
      case 12:
        return MOD;
      case 13:
        return PLUS;
      case 14:
        if (character == '&') {
          character = next_character();
          state = 15;
          break;
        } else {
          state = 16;
          break;
        }
      case 15:
        return AND;
      case 16:
        return AND_BIT;
      case 17:
        if (character == '|') {
          character = next_character();
          state = 18;
          break;
        } else {
          state = 19;
          break;
        }
      case 18:
        return ALTERNATIVE;
      case 19:
        return ALTERNATIVE_BIT;
      case 20:
        if (character == '=') {
          character = next_character();
          state = 21;
          break;
        } else {
          state = 22;
          break;
        }
      case 21:
        return GTE;
      case 22:
        return GT;
      case 23:
        if (character == '=') {
          character = next_character();
          state = 25;
          break;
        } else {
          state = 24;
          break;
        }
      case 24:
        return LT;
      case 25:
        return LTE;
      case 26:
        if (character == '=') {
          character = next_character();
          state = 27;
          break;
        } else {
          state = 28;
          break;
        }
      case 27:
        return EQUAL;
      case 28:
        return ASSIGN;
      case 30:
        if (character == '=') {
          character = next_character();
          state = 31;
          break;
        } else {
          state = 32;
          break;
        }
      case 31:
        return NOTEQUAL;
      case 32:
        return BANG;
      case 33:
        if (character == '>') {
          character = next_character();
          state = 34;
          break;
        } else {
          state = 35;
          break;
        }
      case 34:
        return POINTER;
      case 35:
        return MINUS;
      case 36:
        if (character == '*') {
          character = next_character();
          state = 38;
          break;
        } else {
          state = 37;
          break;
        }
      case 37:
        return DIV;
      case 38:
        if (character == '*') {
          character = next_character();
          state = 39;
          break;
        } else if (character == EOF) {
          state = 46;
          break;
        } else if (character == '\n') {
          character = next_character();
          state = 38;
          line_count++;
          break;
        } else {
          character = next_character();
          state = 38;
          break;
        }
      case 39:
        if (character == '/') {
          character = next_character();
          state = 0;
          break;
        } else {
          character = next_character();
          state = 38;
          break;
        }
      case 40:
        line_count++;
        state = 0;
        break;
      case 41:
        if (isalnum(character) || character == '_') {
          character = next_character();
          state = 41;
          break;
        } else {
          state = 42;
          break;
        }
      case 42: {
        ReservedSymbol *r_symbol = reserved_table->lookup(lexeme_name());
        if (r_symbol != NULL)
          return r_symbol->get_token_id();
        else {
          identifier_table->insert(lexeme_name());
          return ID;
        }
      }
      case 43:
        if (character == '\"') {
          character = next_character();
          state = 45;
          break;
        } else if (character == '\\') {
          character = next_character();
          state = 44;
          break;
        } else if (character == EOF) {
          state = 63;
          break;
        } else {
          character = next_character();
          state = 43;
          break;
        }
      case 44:
        if (character == EOF) {
          state = 63;
          break;
        } else {
          character = next_character();
          state = 43;
          break;
        }
      case 45:
        literal_table->insert(lexeme_name());
        return LITERAL;
        //error - End of File
      case 46:
        print_error_and_restart(ERROR_EOF_COMMENT, state);
        break;
      case 47:
        if (character == EOF) {
          state = 64;
          break;
        } else if (character == '\\') {
          character = next_character();
          state = 48;
          break;
        } else {
          character = next_character();
          state = 49;
          break;
        }
      case 48:
        if (character == EOF) {
          state = 64;
          break;
        } else {
          character = next_character();
          state = 49;
          break;
        }
      case 49:
        if (character == EOF) {
          state = 64;
          break;
        } else if (character == '\'') {
          character = next_character();
          state = 50;
          break;
        } else {
          state = 62;
          break;
        }
      case 50:
        literal_table->insert(lexeme_name());
        return CHARACTER;
      case 51:
        if (isdigit(character)) {
          character = next_character();
          state = 51;
          break;
        } else if (character == '.') {
          character = next_character();
          state = 58;
          break;
        } else if (character == 'e' || character == 'E') {
          character = next_character();
          state = 53;
          break;
        } else {
          state = 52;
          break;
        }
      case 52:
        integer_table->insert(lexeme_name());
        return INTEGER;
      case 53:
        if (character == '+' || character == '-') {
          character = next_character();
          state = 55;
          break;
        } else if (isdigit(character)) {
          character = next_character();
          state = 56;
          break;
        } else {
          state = 54;
          break;
        }
        //error - Wrong Number
      case 54:
        print_error_and_restart(ERROR_WRONG_NUMBER, state);
        break;
      case 55:
        if (isdigit(character)) {
          character = next_character();
          state = 56;
          break;
        } else {
          state = 54;
          break;
        }
      case 56:
        if (isdigit(character)) {
          character = next_character();
          state = 56;
          break;
        } else {
          state = 57;
          break;
        }
      case 57:
        real_table->insert(lexeme_name());
        return REAL;
      case 58:
        if (isdigit(character)) {
          character = next_character();
          state = 58;
          break;
        } else if (character == 'e' || character == 'E') {
          character = next_character();
          state = 53;
          break;
        } else {
          state = 57;
          break;
        }
      case 59:
        if (isdigit(character)) {
          character = next_character();
          state = 58;
          break;
        } else {
          state = 60;
          break;
        }
      case 60:
        return PERIOD;
        //error - Invalid Character
      case 61:
        print_error_and_restart(ERROR_INVALID_CHARACTER, state);
        break;
        //error - Invalid Char Construction
      case 62:
        print_error_and_restart(ERROR_INVALID_CHAR_CONSTRUCTION, state);
        break;
      case 63:
        print_error_and_restart(ERROR_EOF_LITERAL, state);
        break;
      case 64:
        print_error_and_restart(ERROR_EOF_CHARACTER, state);
        break;
    }
  }
  return 0;
}

/**
 * Gets a pointer to the current lexeme in the respective SymbolTable.
 * @return
 */
const char *get_lexeme() {
  if (reserved_table->lookup(lexeme_name()) != NULL)
    return reserved_table->lookup(lexeme_name())->get_lexeme();
  if (identifier_table->lookup(lexeme_name()) != NULL)
    return identifier_table->lookup(lexeme_name())->get_lexeme();
  if (literal_table->lookup(lexeme_name()) != NULL)
    return literal_table->lookup(lexeme_name())->get_lexeme();
  if (integer_table->lookup(lexeme_name()) != NULL)
    return integer_table->lookup(lexeme_name())->get_lexeme();
  if (real_table->lookup(lexeme_name()) != NULL)
    return real_table->lookup(lexeme_name())->get_lexeme();
  return NULL;
}
