#ifndef TABLE_H_INCLUDED
#define TABLE_H_INCLUDED
#define HT_SIZE 313
#define NON_LOCAL_SCOPE "non_local"

#include "symbol.h"

class Symbol;
class ReservedSymbol;
class IdentifierSymbol;
class LiteralSymbol;
class IntegerSymbol;
class RealSymbol;
class VarSymbol;
class FunctionSymbol;
class StructSymbol;
class ClassSymbol;

/**
* Base symbol table class. Has a Symbol array that is used as a hash table.
* The lexemes char array is used to store the lexemes of each symbol in the table.
* lexCurrentPos and lexCurrentSize are used to manage the lexemes array.
*/
class Table {
  protected:
    Table();
    Symbol *table[HT_SIZE];
    char *lexemes;
    unsigned int lex_current_pos;
    unsigned int lex_current_size;
    int current_scope;
    const char *previous_scope_lex;
    const char *current_scope_lex;

    unsigned int hash(const char *s);
    unsigned int get_index_from_hash(const char *value);
    void insert(Symbol *symbol, const char *value);
    Symbol *lookup(const char *value);
    static void print_table_header(const char *table_name);
    virtual void print_table(const char *table_name);
  public:
    virtual ~Table();
    virtual void print_table() = 0;
    void begin_scope(const char *scope_lex);
    void end_scope();
};

/**
* Derived class from Table, used for reserved words.
*/
class ReservedTable : public Table {
  public:
    void insert(const char *value, int token_id);

    ReservedSymbol *lookup(const char *value) { return (ReservedSymbol *) Table::lookup(value); }

    void print_table();
};

/**
* Derived class from Table, used for identifiers.
*/
class IdentifierTable : public Table {
  public:
    void insert(const char *value);

    IdentifierSymbol *lookup(const char *value) { return (IdentifierSymbol *) Table::lookup(value); }

    void print_table() { Table::print_table("IDENTIFIERS"); }
};

/**
* Derived class from Table, used for literals.
*/
class LiteralTable : public Table {
  public:
    void insert(const char *value);

    LiteralSymbol *lookup(const char *value) { return (LiteralSymbol *) Table::lookup(value); }

    void print_table();
};

/**
* Derived class from Table, used for integer numbers.
*/
class IntegerTable : public Table {
  public:
    void insert(const char *value);

    IntegerSymbol *lookup(const char *value) { return (IntegerSymbol *) Table::lookup(value); }

    void print_table() { Table::print_table("INTEGERS"); }
};

/**
* Derived class from Table, used for real numbers.
*/
class RealTable : public Table {
  public:
    void insert(const char *value);

    RealSymbol *lookup(const char *value) { return (RealSymbol *) Table::lookup(value); }

    void print_table() { Table::print_table("REALS"); }
};

class VarTable : public Table {
  public:
    VarTable();
    virtual ~VarTable();

    bool insert(TypeNode *type, const char *value, bool pointer, int array, bool parameter);

    VarSymbol *lookup(const char *value);
    VarSymbol *lookup_in_scope(const char *value, const char *scope_lex);

    void print_table();
};

class FunctionTable : public Table {
  public:
    FunctionTable();
    virtual ~FunctionTable();

    bool insert(TypeNode *type, const char *value, FormalListNode *var_decl, bool pointer);

    FunctionSymbol *lookup(const char *value);

    FunctionSymbol *lookup_in_scope(const char *value, const char *scope_lex);

    void print_table();
};

class StructTable : public Table {
  public:
    StructTable();
    virtual ~StructTable();

    bool insert(const char *value, VarDeclNode *var_decl);

    StructSymbol *lookup(const char *value);

    void print_table();
};

class ClassTable : public Table {
  public:
    ClassTable();
    virtual ~ClassTable();
    bool insert(const char *value, VarDeclNode *var_decl, FunctionDeclNode *func_decl, const char *parent_lexeme);

    ClassSymbol *lookup(const char *value);

    void print_table();
};

#endif // TABLE_H_INCLUDED
