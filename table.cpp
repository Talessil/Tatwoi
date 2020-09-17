#include <cstring>
#include <cstdlib>
#include <iostream>
#include "table.h"

#define LEX_SIZE 1024
#define LEX_INCREMENT 64
#define HASH_SEED 0x811C9DC5 // 2166136261
#define HASH_PRIME 0x01000193 // 16777619

/// Table
/// Base table constructor
Table::Table() {
  lexemes = (char *) malloc(LEX_SIZE * sizeof(char));
  lex_current_pos = 0;
  lex_current_size = LEX_SIZE;
  for (int i = 0; i < HT_SIZE; i++)
    table[i] = NULL;
}

/// Base table destructor
Table::~Table() {
  for (int i = 0; i < HT_SIZE; i++)
    if (table[i] != NULL)
      delete table[i];
  free(lexemes);
}

/// Algorithm FNV1a for hashing. Returns the hashed value of the string.
unsigned int Table::hash(const char *str) {
  unsigned int hash_code = HASH_SEED;
  const unsigned char *ptr = (const unsigned char *) str;
  while (*ptr)
    hash_code = (*ptr++ ^ hash_code) * HASH_PRIME;
  return hash_code;
}

/// Makes the hash of the value and returns the index of it in the hash table.
unsigned int Table::get_index_from_hash(const char *value) {
  unsigned int symbol_hash = hash(value);
  return symbol_hash % HT_SIZE;
}

/** Given a Symbol inserts it in the hash table and add the lexeme
* to the lexemes array.
* Reallocs the lexemes array if needed.
*/
void Table::insert(Symbol *symbol, const char *lexeme) {
  if (lexeme != NULL) {
    unsigned int index = get_index_from_hash(lexeme);
    symbol->set_next(table[index]);
    table[index] = symbol;
    if (lex_current_pos + strlen(lexeme) >= lex_current_size) {
      lex_current_size += (1 + strlen(lexeme) / LEX_INCREMENT) * LEX_INCREMENT;
      lexemes = (char *) realloc(lexemes, lex_current_size * sizeof(char));
    }
    strcpy(&lexemes[lex_current_pos], lexeme);
    lex_current_pos += strlen(lexeme) + 1;
  }
}

/** Verify if a symbol is in the table.
* If true, returns the Symbol, else returns NULL.
*/
Symbol *Table::lookup(const char *value) {
  if (value != NULL) {
    unsigned int index = get_index_from_hash(value);
    Symbol *sym;
    for (sym = table[index]; sym != NULL; sym = sym->get_next())
      if (strcmp(sym->get_lexeme(), value) == 0) return sym;
  }
  return NULL;
}

/// Prints the header for the table with the given table_name.
void Table::print_table_header(const char *table_name) {
  std::cout << "\nSYMBOL TABLE: " << table_name << std::endl;
  std::cout << "_____________________________" << std::endl;
}

/** Base implementation of the print_table function.
* Only prints the lexeme of the symbols in the table.
*/
void Table::print_table(const char *table_name) {
  Table::print_table_header(table_name);
  for (int i = 0; i < HT_SIZE; i++)
    for (Symbol *sym = table[i]; sym != NULL; sym = sym->get_next())
      std::cout << sym->get_lexeme() << std::endl;
}

void Table::begin_scope(const char *scope_lex) {
  previous_scope_lex = current_scope_lex;
  current_scope_lex = scope_lex;
  current_scope += 1;
}

void Table::end_scope() {
  current_scope_lex = previous_scope_lex;
  previous_scope_lex = NON_LOCAL_SCOPE;
  current_scope -= 1;
}

/// ReservedTable
/** Verify if the symbol is in the table.
* If not, instantiates a new ReservedSymbol and inserts it.
*/
void ReservedTable::insert(const char *value, int token_id) {
  if (lookup(value) == NULL)
    Table::insert(new ReservedSymbol(value, token_id), value);
}

/// Overrides the base print_table function to print the reserved words table as expected.
void ReservedTable::print_table() {
  Table::print_table_header("RESERVED WORDS");
  std::cout << "Lexeme" << "\t" << "Token number" << std::endl;
  std::cout << "_____________________________" << std::endl;
  for (int i = 0; i < HT_SIZE; i++)
    for (ReservedSymbol *sym = (ReservedSymbol *) table[i]; sym != NULL; sym = (ReservedSymbol *) sym->get_next())
      std::cout << sym->get_lexeme() << "\t" << sym->get_token_id() << std::endl;
}

/// IdentifierTable
/** Verify if the symbol is in the table.
* If not, instantiates a new IdentifierSymbol and inserts it.
*/
void IdentifierTable::insert(const char *value) {
  if (lookup(value) == NULL)
    Table::insert(new IdentifierSymbol(&lexemes[lex_current_pos]), value);
}

/// LiteralTable
/** Verify if the symbol is in the table.
* If not, instantiates a new LiteralSymbol and inserts it.
*/
void LiteralTable::insert(const char *value) {
  if (lookup(value) == NULL)
    Table::insert(new LiteralSymbol(&lexemes[lex_current_pos]), value);
}

/// Overrides the base print_table function to print the literals table as expected.
void LiteralTable::print_table() {
  Table::print_table_header("LITERALS");
  for (int i = 0; i < HT_SIZE; i++)
    for (Symbol *sym = table[i]; sym != NULL; sym = sym->get_next())
      std::cout << "\"" << sym->get_lexeme() << "\"" << std::endl;
}

/// IntegerTable
/** Verify if the symbol is in the table.
* If not, instantiates a new IntegerSymbol and inserts it.
*/
void IntegerTable::insert(const char *value) {
  if (lookup(value) == NULL)
    Table::insert(new IntegerSymbol(&lexemes[lex_current_pos]), value);
}

/// RealTable
/** Verify if the symbol is in the table.
* If not, instantiates a new RealSymbol and inserts it.
*/
void RealTable::insert(const char *value) {
  if (lookup(value) == NULL)
    Table::insert(new RealSymbol(&lexemes[lex_current_pos]), value);
}

/// VarTable
VarTable::VarTable() {
  current_scope = 0;
  previous_scope_lex = NON_LOCAL_SCOPE;
  current_scope_lex = NON_LOCAL_SCOPE;
}

VarTable::~VarTable() {
  current_scope = 0;
  previous_scope_lex = NULL;
  current_scope_lex = NULL;
}

VarSymbol *VarTable::lookup(const char *value) {
  if (value != NULL) {
    VarSymbol *var_symbol = (VarSymbol *) Table::lookup(value);
    while (var_symbol != NULL && var_symbol->get_scope() <= current_scope) {
      if ((var_symbol->get_lexeme() == value && var_symbol->is_scope(current_scope_lex)) ||
          var_symbol->is_scope(previous_scope_lex) ||
          var_symbol->is_scope(NON_LOCAL_SCOPE))
        return var_symbol;
      var_symbol = (VarSymbol *) var_symbol->get_next();
    }
  }
  return NULL;
}

VarSymbol *VarTable::lookup_in_scope(const char *value, const char *scope_lex) {
  if (value != NULL) {
    VarSymbol *var_symbol = (VarSymbol *) Table::lookup(value);
    while (var_symbol != NULL) {
      if (var_symbol->get_lexeme() == value && var_symbol->is_scope(scope_lex))
        return var_symbol;
      var_symbol = (VarSymbol *) var_symbol->get_next();
    }
  }
  return NULL;
}

/**
 * @return true if inserted and false if not
 */
bool VarTable::insert(TypeNode *type, const char *value, bool pointer, int array, bool parameter) {
  VarSymbol *var_symbol = lookup(value);
  if (var_symbol == NULL || var_symbol->get_scope() < current_scope || !var_symbol->is_scope(current_scope_lex)) {
    Table::insert(new VarSymbol(type, value, pointer, array, current_scope, current_scope_lex, parameter), value);
    return true;
  }
  return false;
  /*return var_symbol != NULL && var_symbol->get_scope() == current_scope &&
  var_symbol->is_scope(current_scope_lex);*/
}

void VarTable::print_table() {
  Table::print_table_header("VARS");
  std::cout << "Lexeme" << "\t" << "Scope" << "\t" << "Scope Lexeme" << std::endl;
  std::cout << "_____________________________" << std::endl;
  for (int i = 0; i < HT_SIZE; i++)
    for (VarSymbol *sym = (VarSymbol *) table[i]; sym != NULL; sym = (VarSymbol *) sym->get_next())
      std::cout << sym->get_lexeme() << "\t" << sym->get_scope() << "\t" << sym->get_scope_lex() << std::endl;
}

/// FunctionTable
FunctionTable::FunctionTable() {
  current_scope = 0;
  previous_scope_lex = NON_LOCAL_SCOPE;
  current_scope_lex = NON_LOCAL_SCOPE;
}

FunctionTable::~FunctionTable() {
  current_scope = 0;
  previous_scope_lex = NULL;
  current_scope_lex = NULL;
}

FunctionSymbol *FunctionTable::lookup(const char *value) {
  if (value != NULL) {
    FunctionSymbol *func_symbol = (FunctionSymbol *) Table::lookup(value);
    while (func_symbol != NULL && func_symbol->get_scope() <= current_scope) {
      if ((func_symbol->get_lexeme() == value && func_symbol->is_scope(current_scope_lex)) ||
          func_symbol->is_scope(previous_scope_lex) ||
          func_symbol->is_scope(NON_LOCAL_SCOPE))
        return func_symbol;
      func_symbol = (FunctionSymbol *) func_symbol->get_next();
    }
  }
  return NULL;
}

FunctionSymbol *FunctionTable::lookup_in_scope(const char *value, const char *scope_lex) {
  if (value != NULL) {
    FunctionSymbol *func_symbol = (FunctionSymbol *) Table::lookup(value);
    while (func_symbol != NULL) {
      if (func_symbol->get_lexeme() == value && func_symbol->is_scope(scope_lex))
        return func_symbol;
      func_symbol = (FunctionSymbol *) func_symbol->get_next();
    }
  }
  return NULL;
}

bool FunctionTable::insert(TypeNode *type, const char *value, FormalListNode *var_decl, bool pointer) {
  FunctionSymbol *func_symbol = lookup(value);
  /*bool is_equal_func = false;
  if (func_symbol != NULL) {
    FormalListNode *aux1 = func_symbol->getVar_decl();
    FormalListNode *aux2 = var_decl;
    if (!(aux1 == NULL && aux2 == NULL)) {
      while (aux1 != NULL && aux1->is_equal(aux2)) {
        aux1 = aux1->get_next();
        aux2 = aux2->get_next();
      }
      if (aux1 == NULL && aux2 == NULL) is_equal_func = true;
    } else is_equal_func = true;
  }*/
  if (/*!is_equal_func ||*/ func_symbol == NULL || func_symbol->get_scope() < current_scope || !func_symbol->is_scope
          (current_scope_lex)) {
    Table::insert(new FunctionSymbol(type, value, var_decl, pointer, current_scope, current_scope_lex), value);
    return true;
  }
  return false;
  /*return func_symbol != NULL && func_symbol->get_scope() == current_scope &&
         func_symbol->is_scope(current_scope_lex);*/
}

void FunctionTable::print_table() {
  Table::print_table_header("FUNCTIONS");
  std::cout << "Lexeme" << "\t" << "Scope" << "\t" << "Scope Lexeme" << std::endl;
  std::cout << "_____________________________" << std::endl;
  for (int i = 0; i < HT_SIZE; i++)
    for (FunctionSymbol *sym = (FunctionSymbol *) table[i]; sym != NULL; sym = (FunctionSymbol *) sym->get_next())
      std::cout << sym->get_lexeme() << "\t" << sym->get_scope() << "\t" << sym->get_scope_lex() << std::endl;
}

/// StructTable
StructTable::StructTable() {
  current_scope = 0;
  previous_scope_lex = NON_LOCAL_SCOPE;
  current_scope_lex = NON_LOCAL_SCOPE;
}

StructTable::~StructTable() {
  current_scope = 0;
  previous_scope_lex = NULL;
  current_scope_lex = NULL;
}

StructSymbol *StructTable::lookup(const char *value) {
  if (value != NULL) {
    StructSymbol *struct_symbol = (StructSymbol *) Table::lookup(value);
    while (struct_symbol != NULL && struct_symbol->get_scope() <= current_scope) {
      if ((struct_symbol->get_lexeme() == value && struct_symbol->is_scope(current_scope_lex)) ||
          struct_symbol->is_scope(previous_scope_lex) ||
          struct_symbol->is_scope(NON_LOCAL_SCOPE))
        return struct_symbol;
      struct_symbol = (StructSymbol *) struct_symbol->get_next();
    }
  }
  return NULL;
}

bool StructTable::insert(const char *value, VarDeclNode *var_decl) {
  StructSymbol *struct_symbol = lookup(value);
  if (struct_symbol == NULL || struct_symbol->get_scope() < current_scope ||
      !struct_symbol->is_scope(current_scope_lex)) {
    Table::insert(new StructSymbol(value, var_decl, current_scope, current_scope_lex), value);
    return true;
  }
  return false;
  /*return struct_symbol != NULL && struct_symbol->get_scope() == current_scope &&
         struct_symbol->is_scope(current_scope_lex);*/
}

void StructTable::print_table() {
  Table::print_table_header("STRUCTS");
  std::cout << "Lexeme" << "\t" << "Scope" << "\t" << "Scope Lexeme" << std::endl;
  std::cout << "_____________________________" << std::endl;
  for (int i = 0; i < HT_SIZE; i++)
    for (StructSymbol *sym = (StructSymbol *) table[i]; sym != NULL; sym = (StructSymbol *) sym->get_next())
      std::cout << sym->get_lexeme() << "\t" << sym->get_scope() << "\t" << sym->get_scope_lex() << std::endl;
}

/// ClassTable
ClassTable::ClassTable() {
  current_scope = 0;
  previous_scope_lex = NON_LOCAL_SCOPE;
  current_scope_lex = NON_LOCAL_SCOPE;
}

ClassTable::~ClassTable() {
  current_scope = 0;
  previous_scope_lex = NULL;
  current_scope_lex = NULL;
}

ClassSymbol *ClassTable::lookup(const char *value) {
  if (value != NULL) {
    ClassSymbol *class_symbol = (ClassSymbol *) Table::lookup(value);
    while (class_symbol != NULL && class_symbol->get_scope() <= current_scope) {
      if ((class_symbol->get_lexeme() == value && class_symbol->is_scope(current_scope_lex)) ||
          class_symbol->is_scope(previous_scope_lex) ||
          class_symbol->is_scope(NON_LOCAL_SCOPE))
        return class_symbol;
      class_symbol = (ClassSymbol *) class_symbol->get_next();
    }
  }
  return NULL;
}

bool
ClassTable::insert(const char *value, VarDeclNode *var_decl, FunctionDeclNode *func_decl, const char *parent_lexeme) {
  ClassSymbol *class_symbol = lookup(value);
  if (class_symbol == NULL || class_symbol->get_scope() < current_scope || !class_symbol->is_scope(current_scope_lex)) {
    Table::insert(new ClassSymbol(value, var_decl, func_decl, parent_lexeme, current_scope, current_scope_lex), value);
    return true;
  }
  return false;
  /*return class_symbol != NULL && class_symbol->get_scope() == current_scope &&
         class_symbol->is_scope(current_scope_lex);*/
}

void ClassTable::print_table() {
  Table::print_table_header("CLASSES");
  std::cout << "Lexeme" << "\t" << "Scope" << "\t" << "Scope Lexeme" << std::endl;
  std::cout << "_____________________________" << std::endl;
  for (int i = 0; i < HT_SIZE; i++)
    for (ClassSymbol *sym = (ClassSymbol *) table[i]; sym != NULL; sym = (ClassSymbol *) sym->get_next())
      std::cout << sym->get_lexeme() << "\t" << sym->get_scope() << "\t" << sym->get_scope_lex() << std::endl;
}
