#include <cstdlib>
#include "symbol.h"

/// Constructor of the base Symbol class. Receives index from the lexeme array.
Symbol::Symbol(const char *lex) {
  this->lex = lex;
  this->next = NULL;
}

/// Destructor of the base Symbol class.
Symbol::~Symbol() {
  this->lex = NULL;
  free(this->next);
}

/// Sets the symbol that is pointed by this.
void Symbol::set_next(Symbol *next_symbol) {
  next = next_symbol;
}

/// Returns the symbol that is pointed by this, can be NULL.
Symbol *Symbol::get_next() {
  return next;
}

/// Returns the index for the lexeme in the lexeme array.
const char *Symbol::get_lexeme() {
  return lex;
}

/// ReservedSymbol

/// ReservedSymbol constructor, receives a token as an additional parameter.
ReservedSymbol::ReservedSymbol(const char *lex, int token) : Symbol(lex) {
  this->token_id = token;
}

/// Returns the token of the reserved word.
int ReservedSymbol::get_token_id() {
  return token_id;
}

VarSymbol::VarSymbol(TypeNode *type, const char *lexeme, bool pointer, int array, int scope, const char *scope_lex,
                     bool parameter) :
        Symbol(lexeme) {
  this->type = type;
  this->pointer = pointer;
  this->parameter = parameter;
  this->array_size = array;
  this->scope = scope;
  this->scope_lex = scope_lex;
  this->offset = 0;
  this->size = 0;
  this->local_access = NULL;
}

VarSymbol::~VarSymbol() {
  type = NULL;
  local_access = NULL;
}

int VarSymbol::get_offset() {
  return offset;
}

void VarSymbol::set_offset(int offset) {
  this->offset = offset;
}

int VarSymbol::get_size() {
  return size;
}

void VarSymbol::set_size(int size) {
  this->size = size;
}

LocalAccess *VarSymbol::getLocal_access() const {
  return local_access;
}

void VarSymbol::setLocal_access(LocalAccess *local_access) {
  this->local_access = local_access;
}

StructSymbol::StructSymbol(const char *lexeme, VarDeclNode *var_decl, int scope, const char *scope_lex) : Symbol(
        lexeme) {
  this->var_decl = var_decl;
  this->scope = scope;
  this->scope_lex = scope_lex;
}

StructSymbol::~StructSymbol() {
  var_decl = NULL;
}

int StructSymbol::get_size() {
  return size;
}

void StructSymbol::set_size(int size) {
  this->size = size;
}

FunctionSymbol::FunctionSymbol(TypeNode *type, const char *lexeme, FormalListNode *var_decl, bool pointer, int scope,
                               const char *scope_lex) : Symbol(lexeme) {
  this->return_type = type;
  this->var_decl = var_decl;
  this->pointer = pointer;
  this->scope = scope;
  this->scope_lex = scope_lex;
  this->local_size = 0;
  this->param_size = 0;
  this->call_size = 0;
}

FunctionSymbol::~FunctionSymbol() {
  this->return_type = NULL;
  this->var_decl = NULL;
}

TypeNode *FunctionSymbol::getReturn_type() {
  return return_type;
}

void FunctionSymbol::setReturn_type(TypeNode *return_type) {
  this->return_type = return_type;
}

FormalListNode *FunctionSymbol::getVar_decl() {
  return var_decl;
}

void FunctionSymbol::setVar_decl(FormalListNode *var_decl) {
  this->var_decl = var_decl;
}

bool FunctionSymbol::is_pointer() {
  return pointer;
}

void FunctionSymbol::set_pointer(bool pointer) {
  this->pointer = pointer;
}

int FunctionSymbol::getLocal_size() {
  return local_size;
}

void FunctionSymbol::setLocal_size(int local_size) {
  this->local_size = local_size;
}

int FunctionSymbol::getParam_size() {
  return param_size;
}

void FunctionSymbol::setParam_size(int param_size) {
  this->param_size = param_size;
}

int FunctionSymbol::getCall_size() {
  return call_size;
}

void FunctionSymbol::setCall_size(int call_size) {
  this->call_size = call_size;
}

void FunctionSymbol::incrementLocal_size(int size) {
  local_size += size;
}

void FunctionSymbol::incrementParam_size(int size) {
  param_size += size;
}

ClassSymbol::ClassSymbol(const char *lexeme, VarDeclNode *var_decl, FunctionDeclNode *func_decl,
                         const char *parent_lexeme, int scope, const char *scope_lex) : Symbol(lexeme) {
  this->var_decl = var_decl;
  this->func_decl = func_decl;
  this->parent_lexeme = parent_lexeme;
  this->scope = scope;
  this->scope_lex = scope_lex;
}

ClassSymbol::~ClassSymbol() {
  this->var_decl = NULL;
  this->func_decl = NULL;
  this->parent_lexeme = NULL;
}
