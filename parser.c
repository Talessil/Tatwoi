#include "parser.h"
#include "lex_analyzer.h"
#include "error_manager.h"

/****************************************************************************************
********************************* FIRST SET *********************************************
****************************************************************************************/
int First_Program[] = {CLASS, TYPEDEF, EOF_TOKEN, INT, FLOAT, BOOL, ID, CHAR, '\0'};
int First_ProgramFat[] = {LPAREN, LBRACKET, COMMA, SEMICOLON, '\0'};
int First_ProgramClass[] = {LBRACE, COLON, '\0'};
int First_VarStmt[] = {ID, INT, FLOAT, BOOL, CHAR, IF, WHILE, SWITCH, BREAK, PRINT, READLN, RETURN, THROW, LBRACE, TRY,
                       MINUS, BANG, PLUS, MULT, AND_BIT, LPAREN, INTEGER, REAL, CHARACTER, LITERAL, TRUE, FALSE, THIS,
                       NEW, '\0'};
int First_VarStmtAux[] = {LPAREN, MULT, ID, SEMICOLON, PERIOD, POINTER, LBRACKET, ASSIGN, ALTERNATIVE, AND, EQUAL,
                          NOTEQUAL, LTE, GTE, LT, GT, PLUS, MINUS, ALTERNATIVE_BIT, DIV, MOD, AND_BIT, '\0'};
int First_LocalDecl[] = {PUBLIC, PRIVATE, INT, FLOAT, BOOL, ID, CHAR, '\0'};
int First_LocalDeclFat[] = {LPAREN, LBRACKET, COMMA, SEMICOLON, '\0'};
int First_VarDecl[] = {INT, FLOAT, BOOL, ID, CHAR, '\0'};
int First_IdList[] = {MULT, ID, '\0'};
int First_IdListRR[] = {COMMA, '\0'};
int First_Pointer[] = {MULT, '\0'};
int First_Array[] = {LBRACKET, '\0'};
int First_Matrix[] = {LBRACKET, '\0'};
int First_FormalList[] = {INT, FLOAT, BOOL, ID, CHAR, '\0'};
int First_FormalRest[] = {COMMA, '\0'};
int First_Type[] = {INT, FLOAT, BOOL, ID, CHAR, '\0'};
int First_TypePrimitive[] = {INT, FLOAT, BOOL, CHAR, '\0'};
int First_StmtList[] = {IF, WHILE, SWITCH, BREAK, PRINT, READLN, RETURN, THROW, LBRACE, TRY, MINUS, BANG, PLUS, MULT,
                        AND_BIT, ID, LPAREN, INTEGER, REAL, CHARACTER, LITERAL, TRUE, FALSE, THIS, NEW, '\0'};
int First_Stmt[] = {IF, WHILE, SWITCH, BREAK, PRINT, READLN, RETURN, THROW, LBRACE, TRY, MINUS, BANG, PLUS, MULT,
                    AND_BIT, ID, LPAREN, INTEGER, REAL, CHARACTER, LITERAL, TRUE, FALSE, THIS, NEW, '\0'};
int First_Else[] = {ELSE, '\0'};
int First_CaseBlock[] = {CASE, '\0'};
int First_CaseBlockAux[] = {CASE, IF, WHILE, SWITCH, BREAK, PRINT, READLN, RETURN, THROW, LBRACE, TRY, MINUS, BANG,
                            PLUS, MULT, AND_BIT, ID, LPAREN, INTEGER, REAL, CHARACTER, LITERAL, TRUE, FALSE, THIS, NEW,
                            '\0'};
int First_ExprList[] = {MINUS, BANG, PLUS, MULT, AND_BIT, ID, LPAREN, INTEGER, REAL, CHARACTER, LITERAL, TRUE, FALSE,
                        THIS, NEW, '\0'};
int First_ExprListTail[] = {COMMA, '\0'};
int First_ExprAttribution[] = {MINUS, BANG, PLUS, MULT, AND_BIT, ID, LPAREN, INTEGER, REAL, CHARACTER, LITERAL, TRUE,
                               FALSE, THIS, NEW, '\0'};
int First_ExprAttributionAux[] = {ASSIGN, '\0'};
int First_ExprAlternative[] = {MINUS, BANG, PLUS, MULT, AND_BIT, ID, LPAREN, INTEGER, REAL, CHARACTER, LITERAL, TRUE,
                               FALSE, THIS, NEW, '\0'};
int First_ExprAlternativeAux[] = {ALTERNATIVE, '\0'};
int First_ExprAnd[] = {MINUS, BANG, PLUS, MULT, AND_BIT, ID, LPAREN, INTEGER, REAL, CHARACTER, LITERAL, TRUE, FALSE,
                       THIS, NEW, '\0'};
int First_ExprAndAux[] = {AND, '\0'};
int First_ExprEquality[] = {MINUS, BANG, PLUS, MULT, AND_BIT, ID, LPAREN, INTEGER, REAL, CHARACTER, LITERAL, TRUE,
                            FALSE, THIS, NEW, '\0'};
int First_ExprEqualityAux[] = {EQUAL, NOTEQUAL, '\0'};
int First_ExprRelational[] = {MINUS, BANG, PLUS, MULT, AND_BIT, ID, LPAREN, INTEGER, REAL, CHARACTER, LITERAL, TRUE,
                              FALSE, THIS, NEW, '\0'};
int First_ExprRelationalAux[] = {LTE, GTE, LT, GT, '\0'};
int First_ExprAdd[] = {MINUS, BANG, PLUS, MULT, AND_BIT, ID, LPAREN, INTEGER, REAL, CHARACTER, LITERAL, TRUE, FALSE,
                       THIS, NEW, '\0'};
int First_ExprAddAux[] = {PLUS, MINUS, ALTERNATIVE_BIT, '\0'};
int First_ExprMult[] = {MINUS, BANG, PLUS, MULT, AND_BIT, ID, LPAREN, INTEGER, REAL, CHARACTER, LITERAL, TRUE, FALSE,
                        THIS, NEW, '\0'};
int First_ExprMultAux[] = {MULT, DIV, MOD, AND_BIT, '\0'};
int First_ExprUnary[] = {MINUS, BANG, PLUS, MULT, AND_BIT, ID, LPAREN, INTEGER, REAL, CHARACTER, LITERAL, TRUE, FALSE,
                         THIS, NEW, '\0'};
int First_Primary[] = {ID, LPAREN, INTEGER, REAL, CHARACTER, LITERAL, TRUE, FALSE, THIS, NEW, '\0'};
int First_PrimaryFat[] = {LPAREN, PERIOD, POINTER, LBRACKET, '\0'};
int First_FieldAccess[] = {PERIOD, POINTER, LBRACKET, '\0'};

/****************************************************************************************
******************************** FIRST SET END ******************************************
****************************************************************************************/

/****************************************************************************************
********************************** SYNC SET *********************************************
****************************************************************************************/
int Sync_Program[] = {EOF_TOKEN, CLASS, TYPEDEF, INT, FLOAT, BOOL, ID, CHAR, '\0'};
int Sync_ProgramFat[] = {EOF_TOKEN, CLASS, TYPEDEF, INT, FLOAT, BOOL, ID, CHAR, LBRACE, '\0'};
int Sync_ProgramClass[] = {EOF_TOKEN, CLASS, TYPEDEF, INT, FLOAT, BOOL, ID, CHAR, PUBLIC, PRIVATE, '\0'};
int Sync_VarStmt[] = {EOF_TOKEN, RBRACE, SEMICOLON, '\0'};
int Sync_VarStmtAux[] = {EOF_TOKEN, RBRACE, SEMICOLON, '\0'};
int Sync_LocalDecl[] = {EOF_TOKEN, RBRACE, '\0'};
int Sync_LocalDeclFat[] = {EOF_TOKEN, RBRACE, SEMICOLON, '\0'};
int Sync_VarDecl[] = {EOF_TOKEN, RBRACE, SEMICOLON, '\0'};
int Sync_IdList[] = {EOF_TOKEN, SEMICOLON, COMMA, RBRACE, '\0'};
int Sync_IdListRR[] = {EOF_TOKEN, SEMICOLON, RBRACE, COMMA, '\0'};
int Sync_Pointer[] = {EOF_TOKEN, ID, '\0'};
int Sync_Array[] = {EOF_TOKEN, COMMA, CLASS, TYPEDEF, INT, FLOAT, BOOL, ID, CHAR, SEMICOLON, RBRACE, RBRACKET,
                    '\0'};
int Sync_FormalList[] = {EOF_TOKEN, RPAREN, COMMA, '\0'};
int Sync_FormalRest[] = {EOF_TOKEN, RPAREN, COMMA, '\0'};
int Sync_Type[] = {EOF_TOKEN, MULT, EOF_TOKEN, ID, SEMICOLON, RBRACE, RPAREN, '\0'};
int Sync_TypePrimitive[] = {EOF_TOKEN, MULT, RBRACE, ID, SEMICOLON, '\0'};
int Sync_StmtList[] = {EOF_TOKEN, ID, RBRACE, INT, FLOAT, BOOL, CHAR, IF, WHILE, SWITCH, BREAK, PRINT, READLN, RETURN,
                       THROW, LBRACE, TRY, MINUS, BANG, PLUS, MULT, AND_BIT, LPAREN, INTEGER, REAL, CHARACTER, LITERAL,
                       TRUE, FALSE, THIS, NEW, CASE, '\0'};
int Sync_Stmt[] = {EOF_TOKEN, ID, RBRACE, INT, FLOAT, BOOL, CHAR, IF, WHILE, SWITCH, BREAK, PRINT, READLN, RETURN,
                   THROW, LBRACE, TRY, MINUS, BANG, PLUS, MULT, AND_BIT, LPAREN, INTEGER, REAL, CHARACTER, LITERAL,
                   TRUE, FALSE, THIS, NEW, ELSE, CATCH, CASE, '\0'};
int Sync_Else[] = {EOF_TOKEN, ID, RBRACE, INT, FLOAT, BOOL, CHAR, IF, WHILE, SWITCH, BREAK, PRINT, READLN, RETURN,
                   THROW, LBRACE, TRY, MINUS, BANG, PLUS, MULT, AND_BIT, LPAREN, INTEGER, REAL, CHARACTER, LITERAL,
                   TRUE, FALSE, THIS, NEW, ELSE, CATCH, CASE, '\0'};
int Sync_CaseBlock[] = {EOF_TOKEN, RBRACE, CASE, '\0'};
int Sync_CaseBlockAux[] = {EOF_TOKEN, RBRACE, '\0'};
int Sync_ExprList[] = {EOF_TOKEN, RPAREN, '\0'};
int Sync_ExprListTail[] = {EOF_TOKEN, RPAREN, '\0'};
int Sync_Expr[] = {EOF_TOKEN, RPAREN, SEMICOLON, COMMA, RBRACKET, '\0'};
int Sync_ExprAttribution[] = {EOF_TOKEN, RPAREN, SEMICOLON, COMMA, ALTERNATIVE, RBRACE, AND, EQUAL, NOTEQUAL, LTE, GTE,
                              LT, GT, PLUS, MINUS, ALTERNATIVE_BIT, MULT, DIV, MOD, AND_BIT, RBRACKET, '\0'};
int Sync_ExprAtributionAux[] = {EOF_TOKEN, ALTERNATIVE, RBRACE, AND, EQUAL, NOTEQUAL, LTE, GTE, LT, GT, PLUS, MINUS,
                                ALTERNATIVE_BIT, MULT, DIV, MOD, AND_BIT, SEMICOLON, RPAREN, COMMA, RBRACKET, '\0'};
int Sync_ExprAlternative[] = {EOF_TOKEN, ASSIGN, RPAREN, SEMICOLON, COMMA, AND, RBRACE, EQUAL, NOTEQUAL, LTE, GTE, LT,
                              GT, PLUS, MINUS, ALTERNATIVE_BIT, MULT, DIV, MOD, AND_BIT, ALTERNATIVE, RBRACKET, '\0'};
int Sync_ExprAlternativeAux[] = {EOF_TOKEN, AND, RBRACE, EQUAL, NOTEQUAL, LTE, GTE, LT, GT, PLUS, MINUS,
                                 ALTERNATIVE_BIT, MULT, DIV, MOD, AND_BIT, SEMICOLON, ASSIGN, RPAREN, COMMA,
                                 ALTERNATIVE, RBRACKET, '\0'};
int Sync_ExprAnd[] = {EOF_TOKEN, ALTERNATIVE, ASSIGN, RPAREN, SEMICOLON, COMMA, EQUAL, NOTEQUAL, RBRACE, LTE, GTE, LT,
                      GT, PLUS, MINUS, ALTERNATIVE_BIT, MULT, DIV, MOD, AND_BIT, AND, RBRACKET, '\0'};
int Sync_ExprAndAux[] = {EOF_TOKEN, EQUAL, NOTEQUAL, RBRACE, LTE, GTE, LT, GT, PLUS, MINUS, ALTERNATIVE_BIT, MULT, DIV,
                         MOD, AND_BIT, SEMICOLON, ALTERNATIVE, ASSIGN, RPAREN, COMMA, AND, RBRACKET, '\0'};
int Sync_ExprEquality[] = {EOF_TOKEN, AND, ALTERNATIVE, ASSIGN, RPAREN, SEMICOLON, COMMA, LTE, GTE, LT, GT, RBRACE,
                           PLUS, MINUS, ALTERNATIVE_BIT, MULT, DIV, MOD, AND_BIT, EQUAL, NOTEQUAL, RBRACKET, '\0'};
int Sync_ExprEqualityAux[] = {EOF_TOKEN, LTE, GTE, LT, GT, RBRACE, PLUS, MINUS, ALTERNATIVE_BIT, MULT, DIV, MOD,
                              AND_BIT, SEMICOLON, AND, ALTERNATIVE, ASSIGN, RPAREN, COMMA, EQUAL, NOTEQUAL, RBRACKET,
                              '\0'};
int Sync_ExprRelational[] = {EOF_TOKEN, EQUAL, NOTEQUAL, AND, ALTERNATIVE, ASSIGN, RPAREN, SEMICOLON, COMMA, PLUS,
                             MINUS, ALTERNATIVE_BIT, RBRACE, MULT, DIV, MOD, AND_BIT, LTE, GTE, LT, GT, RBRACKET, '\0'};
int Sync_ExprRelationalAux[] = {EOF_TOKEN, PLUS, MINUS, ALTERNATIVE_BIT, RBRACE, MULT, DIV, MOD, AND_BIT, SEMICOLON,
                                EQUAL, NOTEQUAL, AND, ALTERNATIVE, ASSIGN, RPAREN, COMMA, LTE, GTE, LT, GT, RBRACKET,
                                '\0'};
int Sync_ExprAdd[] = {EOF_TOKEN, LTE, GTE, LT, GT, EQUAL, NOTEQUAL, AND, ALTERNATIVE, ASSIGN, RPAREN, SEMICOLON, COMMA,
                      MULT, DIV, MOD, AND_BIT, RBRACE, PLUS, MINUS, ALTERNATIVE_BIT, RBRACKET, '\0'};
int Sync_ExprAddAux[] = {EOF_TOKEN, MULT, DIV, MOD, AND_BIT, RBRACE, SEMICOLON, LTE, GTE, LT, GT, EQUAL, NOTEQUAL, AND,
                         ALTERNATIVE, ASSIGN, RPAREN, COMMA, PLUS, MINUS, ALTERNATIVE_BIT, RBRACKET, '\0'};
int Sync_ExprMult[] = {EOF_TOKEN, PLUS, MINUS, ALTERNATIVE_BIT, LTE, GTE, LT, GT, EQUAL, NOTEQUAL, AND, ALTERNATIVE,
                       ASSIGN, RPAREN, SEMICOLON, COMMA, MULT, DIV, MOD, AND_BIT, RBRACE, RBRACKET, '\0'};
int Sync_ExprMultAux[] = {EOF_TOKEN, SEMICOLON, PLUS, MINUS, ALTERNATIVE_BIT, LTE, GTE, LT, GT, EQUAL, NOTEQUAL, AND,
                          ALTERNATIVE, ASSIGN, RPAREN, COMMA, MULT, DIV, MOD, AND_BIT, RBRACE, RBRACKET, '\0'};
int Sync_ExprUnary[] = {EOF_TOKEN, MULT, DIV, MOD, AND_BIT, PLUS, MINUS, ALTERNATIVE_BIT, LTE, GTE, LT, GT, EQUAL,
                        NOTEQUAL, AND, ALTERNATIVE, ASSIGN, RPAREN, SEMICOLON, COMMA, RBRACE, RBRACKET, '\0'};
int Sync_Primary[] = {EOF_TOKEN, MULT, DIV, MOD, AND_BIT, PLUS, MINUS, ALTERNATIVE_BIT, LTE, GTE, LT, GT, EQUAL,
                      NOTEQUAL, AND, ALTERNATIVE, ASSIGN, RPAREN, SEMICOLON, COMMA, RBRACE, RBRACKET, '\0'};
int Sync_PrimaryFat[] = {EOF_TOKEN, ASSIGN, RBRACE, ALTERNATIVE, AND, EQUAL, NOTEQUAL, LTE, GTE, LT, GT, PLUS, MINUS,
                         ALTERNATIVE_BIT, MULT, DIV, MOD, AND_BIT, SEMICOLON, RPAREN, COMMA, RBRACKET, '\0'};
int Sync_FieldAccess[] = {EOF_TOKEN, MULT, DIV, MOD, AND_BIT, PLUS, MINUS, ALTERNATIVE_BIT, LTE, GTE, LT, GT, EQUAL,
                          NOTEQUAL, AND, ALTERNATIVE, ASSIGN, RPAREN, SEMICOLON, COMMA, RBRACE, RBRACKET, '\0'};

/****************************************************************************************
********************************** SYNC SET END *****************************************
****************************************************************************************/

int lookahead;
ProgramNode *program;
PrintASTVisitor *visitor;

ProgramNode *
Program(ClassListNode *class_list, TypeListNode *type_list, VarFuncListNode *var_func_list);
VarFuncListNode *ProgramFat(TypeNode *type, PointerNode *pointer, IdentifierNode *id, VarFuncListNode *var_func);
ClassListNode *ProgramClass(IdentifierNode *id, ClassListNode *class_list);
VarFuncListNode *LocalDecl();
VarFuncListNode *LocalDeclFat(TypeNode *type, PointerNode *pointer, IdentifierNode *id);
VarStmtNode *VarStmt();
VarStmtNode *VarStmtAux(IdentifierNode *id);
VarDeclNode *VarDecl();
IdListNode *IdList();
IdListNode *IdListRR();
PointerNode *Pointer();
ArrayDeclNode *Array();
FormalListNode *FormalList();
FormalListNode *FormalRest();
TypeNode *Type();
TypeNode *TypePrimitive();
StatementListNode *StmtList();
StatementNode *Stmt();
StatementNode *Else();
CaseBlockNode *CaseBlock();
CaseBlockNode *CaseBlockAux(IntegerNode *integer);
ExpListNode *ExprList();
ExpListNode *ExprListTail();
ExpNode *ExprAttribution();
ExpNode *ExprAttributionAux(ExpNode *expr);
ExpNode *ExprAlternative();
ExpNode *ExprAlternativeAux(ExpNode *expr);
ExpNode *ExprAnd();
ExpNode *ExprAndAux(ExpNode *expr);
ExpNode *ExprEquality();
ExpNode *ExprEqualityAux(ExpNode *expr);
ExpNode *ExprRelational();
ExpNode *ExprRelationalAux(ExpNode *expr);
ExpNode *ExprAdd();
ExpNode *ExprAddAux(ExpNode *expr);
ExpNode *ExprMult();
ExpNode *ExprMultAux(ExpNode *expr);
ExpNode *ExprUnary();
ExpNode *Primary();
ExpNode *PrimaryFat(IdentifierNode *id);
ExpNode *FieldAccess();

/** Starts parser function with the first production
* lookahead variable gets the first token from 'lex.analyzer.c'
*/
ProgramNode *init_parser() {
  lookahead = get_token();
  program = Program(NULL, NULL, NULL);
  visitor = new PrintASTVisitor();
  program->accept(visitor);
  return program;
}

void end_parser() {
  delete visitor;
  delete program;
}

/**
* Return 1 if lookahead is equal to a symbol in the set "first" passed
*/
int contains_lookahead(const int *first_set) {
  for (int i = 0; first_set[i] != '\0'; i++)
    if (lookahead == first_set[i]) return 1;
  return 0;
}

/**
* Return lookahead variable
*/
int get_lookahead() {
  return lookahead;
}

/** Print the token matched
* lookahead variable gets the next token from 'lex.analyzer.c'
*/
void eat_token() {
  /*if (lookahead == LITERAL || lookahead == INTEGER || lookahead == REAL || lookahead == ID ||
      lookahead == CHARACTER)
     printf("\nMATCH: %stmt.%stmt", get_token_name(lookahead), lexeme_name());
  else
     printf("\nMATCH: %stmt", get_token_name(lookahead));*/
  lookahead = get_token();
}

/****************************************************************************************
********************************* PRODUCTIONS *******************************************
****************************************************************************************/

ProgramNode *
Program(ClassListNode *class_list, TypeListNode *type_list, VarFuncListNode *var_func_list) {
  // printf("\nProgram");
  int line = get_line();
  if (lookahead == TYPEDEF) {
    eat_token();
    eat_or_skip(STRUCT, Sync_Program);
    eat_or_skip(LBRACE, Sync_Program);

    TypeNode *type = Type();
    IdListNode *id_list = IdList();

    eat_or_skip(SEMICOLON, Sync_Program);
    VarDeclNode *var_decl = new VarDeclNode(type, id_list, VarDecl(), line);

    eat_or_skip(RBRACE, Sync_Program);
    const char *aux = eat_or_skip(ID, Sync_Program);
    IdentifierNode *id = new IdentifierNode(aux, line);

    eat_or_skip(SEMICOLON, Sync_Program);
    type_list = new TypeListNode(var_decl, id, type_list, line);
    return Program(class_list, type_list, var_func_list);
  } else if (lookahead == CLASS) {
    eat_token();
    const char *aux = eat_or_skip(ID, Sync_Program);
    IdentifierNode *id = new IdentifierNode(aux, line);
    return Program(ProgramClass(id, class_list), type_list, var_func_list);
  } else if (contains_lookahead(First_Type)) {
    TypeNode *type = Type();
    PointerNode *pointer = Pointer();
    const char *aux = eat_or_skip(ID, Sync_Program);
    IdentifierNode *id = new IdentifierNode(aux, line);
    return Program(class_list, type_list, ProgramFat(type, pointer, id, var_func_list));
  } else if (lookahead == EOF_TOKEN) {
    eat_token();
    return new ProgramNode(class_list, type_list, var_func_list, line); ///END OF FILE - return program node
  } else
    synchronize(ERROR_UNEXPECTED_TOKEN, Sync_Program);
  return Program(class_list, type_list, var_func_list);
}

VarFuncListNode *ProgramFat(TypeNode *type, PointerNode *pointer, IdentifierNode *id, VarFuncListNode *var_func) {
  // printf("\nProgramFat");
  int line = get_line();
  if (lookahead == LPAREN) {
    eat_token();
    FormalListNode *formal_list = FormalList();
    eat_or_skip(RPAREN, Sync_ProgramFat);
    eat_or_skip(LBRACE, Sync_ProgramFat);
    VarStmtNode *var_stmt = VarStmt();
    eat_or_skip(RBRACE, Sync_ProgramFat);
    return new VarFuncListNode(new FunctionDeclNode(type, pointer, id, formal_list, var_stmt, var_func, line),
                               line);
  } else {
    ArrayDeclNode *array = Array();
    IdListNode *list = IdListRR();
    eat_or_skip(SEMICOLON, Sync_ProgramFat);
    return new VarFuncListNode(
            new VarDeclNode(type, new IdListNode(pointer, id, array, list, line), var_func, line),
            line);
  }
}

ClassListNode *ProgramClass(IdentifierNode *id, ClassListNode *class_list) {
  // printf("\nProgramClass");
  int line = get_line();
  if (lookahead == LBRACE) {
    eat_token();
    VarFuncListNode *var_func = LocalDecl();
    eat_or_skip(RBRACE, Sync_ProgramClass);
    eat_or_skip(SEMICOLON, Sync_ProgramClass);
    return new ClassListNode(id, var_func, NULL, class_list, line);
  } else if (lookahead == COLON) {
    eat_token();
    const char *aux = eat_or_skip(ID, Sync_ProgramClass);
    IdentifierNode *id_parent = new IdentifierNode(aux, line);
    eat_or_skip(LBRACE, Sync_ProgramClass);
    VarFuncListNode *var_func = LocalDecl();
    eat_or_skip(RBRACE, Sync_ProgramClass);
    eat_or_skip(SEMICOLON, Sync_ProgramClass);
    return new ClassListNode(id, var_func, id_parent, class_list, line);
  } else
    synchronize(ERROR_LBRACE_OR_COLON_EXPECTED, Sync_ProgramClass);
  return NULL;
}

VarFuncListNode *LocalDecl() {
  // printf("\nLocalDecl");
  int line = get_line();
  if (contains_lookahead(First_Type)) {
    TypeNode *type = Type();
    PointerNode *pointer = Pointer();
    const char *aux = eat_or_skip(ID, Sync_LocalDecl);
    IdentifierNode *id = new IdentifierNode(aux, line);
    return LocalDeclFat(type, pointer, id);
  } else if (lookahead == PUBLIC || lookahead == PRIVATE) {
    eat_token();
    eat_or_skip(COLON, Sync_LocalDecl);
    return LocalDecl();
  }
  ///else epsilon
  return NULL;
}

VarFuncListNode *LocalDeclFat(TypeNode *type, PointerNode *pointer, IdentifierNode *id) {
  // printf("\nLocalDeclFat");
  int line = get_line();
  if (lookahead == LPAREN) {
    eat_token();
    FormalListNode *formal_list = FormalList();
    eat_or_skip(RPAREN, Sync_LocalDeclFat);
    eat_or_skip(LBRACE, Sync_LocalDeclFat);
    VarStmtNode *var_stmt = VarStmt();
    eat_or_skip(RBRACE, Sync_LocalDeclFat);
    return new VarFuncListNode(new FunctionDeclNode(type, pointer, id, formal_list, var_stmt, LocalDecl(), line),
                               line);
  } else {
    ArrayDeclNode *array = Array();
    IdListNode *list = IdListRR();
    eat_or_skip(SEMICOLON, Sync_LocalDeclFat);
    return new VarFuncListNode(
            new VarDeclNode(type, new IdListNode(pointer, id, array, list, line), LocalDecl(), line),
            line);
  }
}

VarStmtNode *VarStmt() {
  // printf("\nVarStmt");
  int line = get_line();
  if (lookahead == ID) {
    IdentifierNode *id = new IdentifierNode(get_lexeme(), line);
    eat_token();
    return VarStmtAux(id);
  } else if (contains_lookahead(First_TypePrimitive)) {
    TypeNode *primitive = TypePrimitive();
    IdListNode *list = IdList();
    eat_or_skip(SEMICOLON, Sync_VarStmt);
    return new VarStmtNode(new VarDeclNode(primitive, list, line), VarStmt(), line);
  } else if (contains_lookahead(First_StmtList))
    return new VarStmtNode(StmtList(), VarStmt(), line);
  ///else epsilon
  return NULL;
}

VarStmtNode *VarStmtAux(IdentifierNode *id) {
  // printf("\nVarStmtAux");
  int line = get_line();
  if (contains_lookahead(First_IdList)) {
    IdListNode *list = IdList();
    eat_or_skip(SEMICOLON, Sync_VarStmtAux);
    TypeNode *type = new TypeNode(ID, id->get_lexeme(), line);
    return new VarStmtNode(new VarDeclNode(type, list, line), VarStmt(), line);
  } else if (contains_lookahead(First_PrimaryFat) || contains_lookahead(First_ExprAttributionAux)
             || contains_lookahead(First_ExprAlternativeAux) || contains_lookahead(First_ExprAndAux)
             || contains_lookahead(First_ExprEqualityAux) || contains_lookahead(First_ExprRelationalAux)
             || contains_lookahead(First_ExprAddAux) || contains_lookahead(First_ExprMultAux)) {
    ExpNode *exp = new PrimaryNode(id, PrimaryFat(id), line);
    exp = ExprAttributionAux(exp);
    exp = ExprAlternativeAux(exp);
    exp = ExprAndAux(exp);
    exp = ExprEqualityAux(exp);
    exp = ExprRelationalAux(exp);
    exp = ExprAddAux(exp);
    exp = ExprMultAux(exp);
    eat_or_skip(SEMICOLON, Sync_VarStmtAux);
    return new VarStmtNode(new StatementListNode(exp, StmtList(), line), NULL, line);
  } else
    synchronize(ERROR_UNEXPECTED_TOKEN, Sync_VarStmtAux);
  return NULL;
}

VarDeclNode *VarDecl() {
  // printf("\nVarDecl");
  int line = get_line();
  if (contains_lookahead(First_Type)) {
    TypeNode *type = Type();
    IdListNode *list = IdList();
    eat_or_skip(SEMICOLON, Sync_VarDecl);
    return new VarDeclNode(type, list, VarDecl(), line);
  }
  ///else epsilon
  return NULL;
}

IdListNode *IdList() {
  // printf("\nIdList");
  int line = get_line();
  PointerNode *pointer = Pointer();
  const char *aux = eat_or_skip(ID, Sync_IdList);
  IdentifierNode *id = new IdentifierNode(aux, line);
  return new IdListNode(pointer, id, Array(), IdListRR(), line);
}

IdListNode *IdListRR() {
  // printf("\nIdListRR");
  int line = get_line();
  if (lookahead == COMMA) {
    eat_token();
    PointerNode *pointer = Pointer();
    const char *aux = eat_or_skip(ID, Sync_IdListRR);
    return new IdListNode(pointer, new IdentifierNode(aux, line), Array(), IdListRR(), line);
  }
  ///else epsilon
  return NULL;
}

PointerNode *Pointer() {
  // printf("\nPointer");
  int line = get_line();
  if (lookahead == MULT) {
    eat_token();
    return new PointerNode(line);
  }
  ///else epsilon
  return NULL;
}

ArrayDeclNode *Array() {
  // printf("\nArray");
  int line = get_line();
  if (lookahead == LBRACKET) {
    eat_token();
    const char *aux = eat_or_skip(INTEGER, Sync_Array);
    IntegerNode *integer = new IntegerNode(aux, line);
    eat_or_skip(RBRACKET, Sync_Array);
    return new ArrayDeclNode(integer, line);
  }
  ///else epsilon
  return NULL;
}

FormalListNode *FormalList() {
  // printf("\nFormalList");
  int line = get_line();
  if (contains_lookahead(First_Type)) {
    TypeNode *type = Type();
    PointerNode *pointer = Pointer();
    const char *aux = eat_or_skip(ID, Sync_FormalList);
    IdentifierNode *id = new IdentifierNode(aux, line);
    ArrayDeclNode *array = Array();
    return new FormalListNode(type, pointer, id, array, FormalRest(), line);
  }
  ///else epsilon
  return NULL;
}

FormalListNode *FormalRest() {
  // printf("\nFormalRest");
  int line = get_line();
  if (lookahead == COMMA) {
    eat_token();
    TypeNode *type = Type();
    PointerNode *pointer = Pointer();
    const char *aux = eat_or_skip(ID, Sync_FormalRest);
    IdentifierNode *id = new IdentifierNode(aux, line);
    ArrayDeclNode *array = Array();
    return new FormalListNode(type, pointer, id, array, FormalRest(), line);
  }
  ///else epsilon
  return NULL;
}

TypeNode *Type() {
  // printf("\nType");
  int line = get_line();
  if (contains_lookahead(First_Type)) {
    TypeNode *type;
    type = new TypeNode(lookahead, lookahead == ID ? get_lexeme() : get_token_name(lookahead), line);
    eat_token();
    return type;
  } else
    synchronize(ERROR_TYPE_EXPECTED, Sync_Type);
  return NULL;
}

TypeNode *TypePrimitive() {
  // printf("\nTypePrimitive");
  int line = get_line();
  if (contains_lookahead(First_TypePrimitive)) {
    TypeNode *type = new TypeNode(lookahead, get_token_name(lookahead), line);
    eat_token();
    return type;
  } else
    synchronize(ERROR_TYPE_PRIMITIVE_EXPECTED, Sync_TypePrimitive);
  return NULL;
}

StatementListNode *StmtList() {
  // printf("\nStmtList");
  int line = get_line();
  if (contains_lookahead(First_Stmt)) {
    return new StatementListNode(Stmt(), StmtList(), line);
  }
  return NULL;
}

StatementNode *Stmt() {
  // printf("\nStmt");
  int line = get_line();
  if (lookahead == IF) {
    eat_token();
    eat_or_skip(LPAREN, Sync_Stmt);
    ExpNode *expr = ExprAttribution();
    eat_or_skip(RPAREN, Sync_Stmt);
    StatementNode *if_stmt = Stmt();
    StatementNode *else_stmt = Else();
    return new IfNode(expr, if_stmt, else_stmt, line);
  } else if (lookahead == WHILE) {
    eat_token();
    eat_or_skip(LPAREN, Sync_Stmt);
    ExpNode *expr = ExprAttribution();
    eat_or_skip(RPAREN, Sync_Stmt);
    return new WhileNode(expr, Stmt(), line);
  } else if (lookahead == SWITCH) {
    eat_token();
    eat_or_skip(LPAREN, Sync_Stmt);
    ExpNode *expr = ExprAttribution();
    eat_or_skip(RPAREN, Sync_Stmt);
    eat_or_skip(LBRACE, Sync_Stmt);
    CaseBlockNode *case_block = CaseBlock();
    eat_or_skip(RBRACE, Sync_Stmt);
    return new SwitchNode(expr, case_block, line);
  } else if (lookahead == BREAK) {
    eat_token();
    eat_or_skip(SEMICOLON, Sync_Stmt);
    return new BreakNode(line);
  } else if (lookahead == PRINT) {
    eat_token();
    eat_or_skip(LPAREN, Sync_Stmt);
    ExpListNode *list = ExprList();
    eat_or_skip(RPAREN, Sync_Stmt);
    eat_or_skip(SEMICOLON, Sync_Stmt);
    return new PrintlnNode(list, line);
  } else if (lookahead == READLN) {
    eat_token();
    eat_or_skip(LPAREN, Sync_Stmt);
    ExpNode *expr = ExprAttribution();
    eat_or_skip(RPAREN, Sync_Stmt);
    eat_or_skip(SEMICOLON, Sync_Stmt);
    return new ReadNode(expr, line);
  } else if (lookahead == RETURN) {
    eat_token();
    ExpNode *expr = ExprAttribution();
    eat_or_skip(SEMICOLON, Sync_Stmt);
    return new ReturnNode(expr, line);
  } else if (lookahead == THROW) {
    eat_token();
    eat_or_skip(SEMICOLON, Sync_Stmt);
    return new ThrowNode(line);
  } else if (lookahead == LBRACE) {
    eat_token();
    StatementListNode *list = StmtList();
    eat_or_skip(RBRACE, Sync_Stmt);
    return list;
  } else if (lookahead == TRY) {
    eat_token();
    StatementNode *try_stmt = Stmt();
    eat_or_skip(CATCH, Sync_Stmt);
    eat_or_skip(LPAREN, Sync_Stmt);
    eat_or_skip(PERIOD, Sync_Stmt);
    eat_or_skip(PERIOD, Sync_Stmt);
    eat_or_skip(PERIOD, Sync_Stmt);
    eat_or_skip(RPAREN, Sync_Stmt);
    return new TryNode(try_stmt, Stmt(), line);
  } else if (contains_lookahead(First_ExprAttribution)) {
    ExpNode *expr = ExprAttribution();
    eat_or_skip(SEMICOLON, Sync_Stmt);
    return expr;
  } else {
    synchronize(ERROR_STATEMENT_EXPECTED, Sync_Stmt);

  }
  return NULL;
}

StatementNode *Else() {
  // printf("\nElse");
  if (lookahead == ELSE) {
    eat_token();
    return Stmt();
  }
  ///else epsilon
  return NULL;
}

CaseBlockNode *CaseBlock() {
  // printf("\nCaseBlock");
  int line = get_line();
  if (lookahead == CASE) {
    eat_token();
    const char *aux = eat_or_skip(INTEGER, Sync_CaseBlock);
    IntegerNode *integer = new IntegerNode(aux, line);
    eat_or_skip(COLON, Sync_CaseBlock);
    return CaseBlockAux(integer);
  } else
    synchronize(ERROR_CASE_EXPECTED, Sync_CaseBlock);
  return NULL;
}

CaseBlockNode *CaseBlockAux(IntegerNode *integer) {
  // printf("\nCaseBlockAux");
  int line = get_line();
  if (contains_lookahead(First_StmtList))
    return new CaseBlockNode(integer, StmtList(), CaseBlock(), line);
  else if (contains_lookahead(First_CaseBlock))
    return new CaseBlockNode(integer, NULL, CaseBlock(), line);
  ///else epsilon
  return NULL;
}

ExpListNode *ExprList() {
  // printf("\nExprList");
  int line = get_line();
  if (contains_lookahead(First_ExprAttribution)) {
    return new ExpListNode(ExprAttribution(), ExprListTail(), line);
  }
  ///else epsilon
  return NULL;
}

ExpListNode *ExprListTail() {
  // printf("\nExprListTail");
  if (lookahead == COMMA) {
    eat_token();
    return ExprList();
  }
  ///else epsilon
  return NULL;
}

ExpNode *ExprAttribution() {
  // printf("\nExprAttribution");
  if (contains_lookahead(First_ExprAlternative))
    return ExprAttributionAux(ExprAlternative());
  else
    synchronize(ERROR_EXPRESSION_EXPECTED, Sync_ExprAttribution);
  return NULL;
}

ExpNode *ExprAttributionAux(ExpNode *expr) {
  // printf("\nExprAttributionAux");
  int line = get_line();
  if (lookahead == ASSIGN) {
    eat_token();
    return new AssignNode(expr, ExprAttribution(), line);
  }
  ///else epsilon
  return expr;
}

ExpNode *ExprAlternative() {
  // printf("\nExprAlternative");
  if (contains_lookahead(First_ExprAnd))
    return ExprAlternativeAux(ExprAnd());
  else
    synchronize(ERROR_EXPRESSION_EXPECTED, Sync_ExprAlternative);
  return NULL;
}

ExpNode *ExprAlternativeAux(ExpNode *expr) {
  // printf("\nExprAlternativeAux");
  int line = get_line();
  if (lookahead == ALTERNATIVE) {
    eat_token();
    return new BooleanOpNode(ALTERNATIVE, expr, ExprAlternative(), line);
  }
  ///else epsilon
  return expr;
}

ExpNode *ExprAnd() {
  // printf("\nExprAnd");
  if (contains_lookahead(First_ExprEquality))
    return ExprAndAux(ExprEquality());
  else
    synchronize(ERROR_EXPRESSION_EXPECTED, Sync_ExprAnd);
  return NULL;
}

ExpNode *ExprAndAux(ExpNode *expr) {
  // printf("\nExprAndAux");
  int line = get_line();
  if (lookahead == AND) {
    eat_token();
    return new BooleanOpNode(AND, expr, ExprAnd(), line);
  }
  return expr;
}

ExpNode *ExprEquality() {
  // printf("\nExprEquality");
  if (contains_lookahead(First_ExprRelational))
    return ExprEqualityAux(ExprRelational());
  else
    synchronize(ERROR_EXPRESSION_EXPECTED, Sync_ExprEquality);
  return NULL;
}

ExpNode *ExprEqualityAux(ExpNode *expr) {
  // printf("\nExprEqualityAux");
  int line = get_line();
  if (lookahead == EQUAL) {
    eat_token();
    return new RelationalOpNode(EQUAL, expr, ExprEquality(), line);
  } else if (lookahead == NOTEQUAL) {
    eat_token();
    return new RelationalOpNode(NOTEQUAL, expr, ExprEquality(), line);
  }
  return expr;
}

ExpNode *ExprRelational() {
  // printf("\nExprRelational");
  if (contains_lookahead(First_ExprAdd))
    return ExprRelationalAux(ExprAdd());
  else
    synchronize(ERROR_EXPRESSION_EXPECTED, Sync_ExprRelational);
  return NULL;
}

ExpNode *ExprRelationalAux(ExpNode *expr) {
  // printf("\nExprRelationalAux");
  int line = get_line();
  if (lookahead == LTE) {
    eat_token();
    return new RelationalOpNode(LTE, expr, ExprRelational(), line);
  } else if (lookahead == GTE) {
    eat_token();
    return new RelationalOpNode(GTE, expr, ExprRelational(), line);
  } else if (lookahead == LT) {
    eat_token();
    return new RelationalOpNode(LT, expr, ExprRelational(), line);
  } else if (lookahead == GT) {
    eat_token();
    return new RelationalOpNode(GT, expr, ExprRelational(), line);
  }
  return expr;
}

ExpNode *ExprAdd() {
  // printf("\nExprAdd");
  if (contains_lookahead(First_ExprMult))
    return ExprAddAux(ExprMult());
  else
    synchronize(ERROR_EXPRESSION_EXPECTED, Sync_ExprAdd);
  return NULL;
}

ExpNode *ExprAddAux(ExpNode *expr) {
  // printf("\nExprAddAux");
  int line = get_line();
  if (lookahead == PLUS) {
    eat_token();
    return new AdditionOpNode(PLUS, expr, ExprAdd(), line);
  } else if (lookahead == MINUS) {
    eat_token();
    return new AdditionOpNode(MINUS, expr, ExprAdd(), line);
  } else if (lookahead == ALTERNATIVE_BIT) {
    eat_token();
    return new BitwiseOpNode(ALTERNATIVE_BIT, expr, ExprAdd(), line);
  }
  return expr;
}

ExpNode *ExprMult() {
  // printf("\nExprMult");
  if (contains_lookahead(First_ExprUnary))
    return ExprMultAux(ExprUnary());
  else
    synchronize(ERROR_EXPRESSION_EXPECTED, Sync_ExprMult);
  return NULL;
}

ExpNode *ExprMultAux(ExpNode *expr) {
  // printf("\nExprMultAux");
  int line = get_line();
  if (lookahead == MULT) {
    eat_token();
    return new MultiplicationOpNode(MULT, expr, ExprMult(), line);
  } else if (lookahead == DIV) {
    eat_token();
    return new MultiplicationOpNode(DIV, expr, ExprMult(), line);
  } else if (lookahead == MOD) {
    eat_token();
    return new MultiplicationOpNode(MOD, expr, ExprMult(), line);
  } else if (lookahead == AND_BIT) {
    eat_token();
    return new BitwiseOpNode(AND_BIT, expr, ExprMult(), line);
  }
  return expr;
}

ExpNode *ExprUnary() {
  // printf("\nExprUnary");
  int line = get_line();
  if (lookahead == MINUS) {
    eat_token();
    return new SignNode(ExprUnary(), line);
  } else if (lookahead == BANG) {
    eat_token();
    return new NotNode(ExprUnary(), line);
  } else if (lookahead == PLUS) {
    eat_token();
    return ExprUnary();
  } else if (lookahead == MULT) {
    eat_token();
    return new PointerValueNode(ExprUnary(), line);
  } else if (lookahead == AND_BIT) {
    eat_token();
    return new AddressValueNode(ExprUnary(), line);
  } else if (contains_lookahead(First_Primary))
    return Primary();
  else
    synchronize(ERROR_UNEXPECTED_TOKEN, Sync_ExprUnary);

  return NULL;
}

ExpNode *Primary() {
  // printf("\nPrimary");
  int line = get_line();
  if (lookahead == ID) {
    IdentifierNode *id = new IdentifierNode(get_lexeme(), line);
    eat_token();
    return new PrimaryNode(id, PrimaryFat(id), line);
  } else if (lookahead == INTEGER) {
    IntegerNode *integer = new IntegerNode(get_lexeme(), line);
    eat_token();
    return new PrimaryNode(integer, FieldAccess(), line);
  } else if (lookahead == REAL) {
    RealNode *real = new RealNode(get_lexeme(), line);
    eat_token();
    return new PrimaryNode(real, FieldAccess(), line);
  } else if (lookahead == CHARACTER) {
    CharacterNode *character = new CharacterNode(get_lexeme(), line);
    eat_token();
    return new PrimaryNode(character, FieldAccess(), line);
  } else if (lookahead == LITERAL) {
    LiteralNode *literal = new LiteralNode(get_lexeme(), line);
    eat_token();
    return new PrimaryNode(literal, FieldAccess(), line);
  } else if (lookahead == TRUE) {
    eat_token();
    return new PrimaryNode(new TrueNode(line), FieldAccess(), line);
  } else if (lookahead == FALSE) {
    eat_token();
    return new PrimaryNode(new FalseNode(line), FieldAccess(), line);
  } else if (lookahead == THIS) {
    eat_token();
    return new PrimaryNode(new ThisNode(line), FieldAccess(), line);
  } else if (lookahead == LPAREN) {
    eat_token();
    ExpNode *expr = ExprAttribution();
    eat_or_skip(RPAREN, Sync_Primary);
    return new PrimaryNode(expr, FieldAccess(), line);
  } else if (lookahead == NEW) {
    eat_token();
    const char *aux = eat_or_skip(ID, Sync_Expr);
    IdentifierNode *id = new IdentifierNode(aux, line);
    eat_or_skip(LPAREN, Sync_Expr);
    ExpListNode *list = ExprList();
    eat_or_skip(RPAREN, Sync_Expr);
    return new PrimaryNode(new NewNode(id, list, line), FieldAccess(), line);
  } else
    synchronize(ERROR_UNEXPECTED_TOKEN, Sync_Primary);
  return NULL;
}

ExpNode *PrimaryFat(IdentifierNode *id) {
  // printf("\nPrimaryFat");
  if (lookahead == LPAREN) {
    int line = get_line();
    eat_token();
    ExpListNode *exp_list = ExprList();
    eat_or_skip(RPAREN, Sync_PrimaryFat);
    return new FunctionCallNode(id, exp_list, FieldAccess(), line);
  } else
    return FieldAccess();
}

ExpNode *FieldAccess() {
  // printf("\nFieldAccess");
  int line = get_line();
  if (lookahead == PERIOD) {
    eat_token();
    if (contains_lookahead(First_Primary))
      return new FieldAccessNode(Primary(), line);
  } else if (lookahead == POINTER) {
    eat_token();
    if (contains_lookahead(First_Primary))
      return new PointerAccessNode(Primary(), line);
  } else if (lookahead == LBRACKET) {
    eat_token();
    ExpNode *index_expr = ExprAttribution();
    eat_or_skip(RBRACKET, Sync_FieldAccess);
    return new ArrayAccessNode(index_expr, FieldAccess(), line);
  }
  ///else epsilon
  return NULL;
}

/****************************************************************************************
******************************** PRODUCTIONS END ****************************************
****************************************************************************************/
