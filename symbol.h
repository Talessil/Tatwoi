#ifndef SYMBOL_H_INCLUDED
#define SYMBOL_H_INCLUDED

#include "ast.h"
#include "table.h"

class VarTable;
class FunctionTable;

/**
* Base symbol class. Has a pointer for another symbol (used in the hash table collision).
* The lex_index keeps the index from the lexeme array where the symbol lexeme is stored.
*/
class Symbol {
  private:
    const char *lex;
    Symbol *next;
  protected:
    int scope;
    const char *scope_lex;
    Symbol(const char *lex);
  public:
    virtual ~Symbol();
    void set_next(Symbol *next_symbol);
    Symbol *get_next();
    const char *get_lexeme();

    int get_scope() { return scope; }

    const char *get_scope_lex() { return scope_lex; }

    bool is_scope(const char *scope_lex) { return this->scope_lex == scope_lex; }
};

/**
* Inherits from class Symbol. Used for the reserved words of the language.
* Has an additional field for the token.
*/
class ReservedSymbol : public Symbol {
  private:
    int token_id;
  public:
    ReservedSymbol(const char *lex, int token);
    int get_token_id();
};

/**
* Inherits from class Symbol. Used for the identifiers.
*/
class IdentifierSymbol : public Symbol {
  public:
    IdentifierSymbol(const char *lex) : Symbol(lex) {};
};

/**
* Inherits from class Symbol. Used for the literals.
*/
class LiteralSymbol : public Symbol {
  public:
    LiteralSymbol(const char *lex) : Symbol(lex) {};
};

/**
* Inherits from class Symbol. Used for the integer numbers.
*/
class IntegerSymbol : public Symbol {
  public:
    IntegerSymbol(const char *lex) : Symbol(lex) {};
};

/**
* Inherits from class Symbol. Used for the real numbers.
*/
class RealSymbol : public Symbol {
  public:
    RealSymbol(const char *lex) : Symbol(lex) {};
};

class VarSymbol : public Symbol {
  private:
    TypeNode *type;
    bool pointer;
    bool parameter;
    int array_size;
    int offset;
    int size;
    LocalAccess *local_access;
  public:
    VarSymbol(TypeNode *type, const char *lexeme, bool pointer, int array, int scope, const char *scope_lex,
              bool parameter);
    virtual ~VarSymbol();

    TypeNode *get_type() { return type; }

    bool is_pointer() { return pointer; }

    bool is_parameter() { return parameter; }

    int getArray_size() { return array_size; }

    LocalAccess *getLocal_access() const;
    void setLocal_access(LocalAccess *local_access);
    int get_offset();
    void set_offset(int offset);
    int get_size();
    void set_size(int size);
};

class FunctionSymbol : public Symbol {
  private:
    TypeNode *return_type;
    FormalListNode *var_decl;
    bool pointer;
    int local_size;
    int param_size;
    int call_size;
  public:
    FunctionSymbol(TypeNode *type, const char *lexeme, FormalListNode *var_decl, bool pointer, int scope,
                   const char *scope_lex);
    virtual ~FunctionSymbol();
    TypeNode *getReturn_type();
    void setReturn_type(TypeNode *return_type);
    FormalListNode *getVar_decl();
    void setVar_decl(FormalListNode *var_decl);
    bool is_pointer();
    void set_pointer(bool pointer);
    int getLocal_size();
    void setLocal_size(int local_size);
    int getParam_size();
    void setParam_size(int param_size);
    int getCall_size();
    void setCall_size(int call_size);
    void incrementLocal_size(int size);
    void incrementParam_size(int size);
};

class StructSymbol : public Symbol {
  private:
    VarDeclNode *var_decl;
    int size;
  public:
    StructSymbol(const char *lexeme, VarDeclNode *var_decl, int scope, const char *scope_lex);
    virtual ~StructSymbol();

    VarDeclNode *getVar_decl() { return var_decl; }

    int get_size();
    void set_size(int size);
};

class ClassSymbol : public Symbol {
  private:
    VarDeclNode *var_decl;
    FunctionDeclNode *func_decl;
    const char *parent_lexeme;
  public:
    ClassSymbol(const char *lexeme, VarDeclNode *var_decl, FunctionDeclNode *func_decl, const char *parent_lexeme,
                int scope, const char *scope_lex);
    virtual ~ClassSymbol();

    VarDeclNode *getVar_decl() { return var_decl; }

    FunctionDeclNode *getFunc_decl() { return func_decl; }

    const char *getParent_lexeme() { return parent_lexeme; }
};

#endif // SYMBOL_H_INCLUDED
