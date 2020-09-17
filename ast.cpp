#include "ast.h"

/**
 * Module with constructors and destructors of some classes from the AST.
 * Only those that could not be inlined were defined here.
 */

IdListNode::IdListNode(PointerNode *pointer, IdentifierNode *id, ArrayDeclNode *array, IdListNode *id_list, int line)
        : Node(line) {
  this->pointer = pointer;
  this->id = id;
  this->array = array;
  this->next = id_list;
}

IdListNode::~IdListNode() {
  delete this->next;
  delete this->pointer;
  delete this->id;
  delete this->array;
}

void IdListNode::accept(Visitor *visitor) { visitor->visit(this); }

IRTree *IdListNode::accept(Translator *translator) {
    translator->visit(this);
    return NULL;
}

PointerNode *IdListNode::get_pointer() { return pointer; }

IdentifierNode *IdListNode::get_id() { return id; }

ArrayDeclNode *IdListNode::get_array() { return array; }

IdListNode *IdListNode::get_next() { return next; }

IfNode::IfNode(ExpNode *exp, StatementNode *if_stmt, StatementNode *else_stmt, int line) : StatementNode(line) {
  this->exp = exp;
  this->if_stmt = if_stmt;
  this->else_stmt = else_stmt;
}

IfNode::~IfNode() {
  delete this->exp;
  delete this->if_stmt;
  delete this->else_stmt;
}

void IfNode::accept(Visitor *visitor) { visitor->visit(this); }

Stmt *IfNode::accept(Translator *translator) { return translator->visit(this); }

ExpNode *IfNode::get_exp() { return exp; }

StatementNode *IfNode::get_if_stmt() { return if_stmt; }

StatementNode *IfNode::get_else_stmt() { return else_stmt; }

WhileNode::WhileNode(ExpNode *exp, StatementNode *statement, int line) : StatementNode(line) {
  this->exp = exp;
  this->statement = statement;
}

WhileNode::~WhileNode() {
  delete this->exp;
  delete this->statement;
}

void WhileNode::accept(Visitor *visitor) { visitor->visit(this); }

Stmt *WhileNode::accept(Translator *translator) { return translator->visit(this); }

ExpNode *WhileNode::get_exp() { return exp; }

StatementNode *WhileNode::get_statement() { return statement; }

CaseBlockNode::CaseBlockNode(IntegerNode *num, StatementListNode *statement_list,
                             CaseBlockNode *case_block, int line) : Node(line) {
  this->num = num;
  this->statement_list = statement_list;
  this->next = case_block;
}

CaseBlockNode::~CaseBlockNode() {
  delete this->num;
  delete this->statement_list;
  delete this->next;
}

void CaseBlockNode::accept(Visitor *visitor) { visitor->visit(this); }

IRTree *CaseBlockNode::accept(Translator *translator) { return translator->visit(this); }

IntegerNode *CaseBlockNode::get_num() { return num; }

StatementListNode *CaseBlockNode::get_statement_list() { return statement_list; }

CaseBlockNode *CaseBlockNode::get_next() { return next; }

SwitchNode::SwitchNode(ExpNode *exp, CaseBlockNode *case_block, int line) : StatementNode(line) {
  this->exp = exp;
  this->case_block = case_block;
}

SwitchNode::~SwitchNode() {
  delete this->exp;
  delete this->case_block;
}

void SwitchNode::accept(Visitor *visitor) { visitor->visit(this); }

Stmt *SwitchNode::accept(Translator *translator) { return translator->visit(this); }

ExpNode *SwitchNode::get_exp() { return exp; }

CaseBlockNode *SwitchNode::get_case_block() { return case_block; }

ExpListNode::ExpListNode(ExpNode *exp, ExpListNode *exp_list, int line) : ExpNode(line) {
  this->exp = exp;
  this->next = exp_list;
}

ExpListNode::~ExpListNode() {
  delete this->exp;
  delete this->next;
}

void ExpListNode::accept(Visitor *visitor) { visitor->visit(this); }

Exp *ExpListNode::accept(Translator *translator) { return translator->visit(this); }

ExpNode *ExpListNode::get_exp() { return exp; }

ExpListNode *ExpListNode::get_next() { return next; }

TryNode::TryNode(StatementNode *try_stmt, StatementNode *catch_stmt, int line) : StatementNode(line) {
  this->try_stmt = try_stmt;
  this->catch_stmt = catch_stmt;
}

TryNode::~TryNode() {
  delete this->try_stmt;
  delete this->catch_stmt;
}

void TryNode::accept(Visitor *visitor) { visitor->visit(this); }

Stmt *TryNode::accept(Translator *translator) { return translator->visit(this); }

StatementNode *TryNode::get_try_stmt() { return try_stmt; }

StatementNode *TryNode::get_catch_stmt() { return catch_stmt; }

StatementListNode::StatementListNode(StatementNode *statement, StatementListNode *statement_list, int line)
        : StatementNode(line) {
  this->statement = statement;
  this->next = statement_list;
}

StatementListNode::~StatementListNode() {
  delete this->next;
  delete this->statement;
}

void StatementListNode::accept(Visitor *visitor) { visitor->visit(this); }

Stmt *StatementListNode::accept(Translator *translator) { return translator->visit(this); }

StatementNode *StatementListNode::get_statement() { return statement; }

StatementListNode *StatementListNode::get_next() { return next; }

FunctionDeclNode::FunctionDeclNode(TypeNode *type, PointerNode *pointer, IdentifierNode *id,
                                   FormalListNode *formal_list, VarStmtNode *var_stmt, VarFuncListNode *next, int line)
        : Node(line) {
  this->type = type;
  this->pointer = pointer;
  this->id = id;
  this->formal_list = formal_list;
  this->var_stmt = var_stmt;
  this->next = next;
}

FunctionDeclNode::~FunctionDeclNode() {
  delete this->type;
  delete this->pointer;
  delete this->id;
  delete this->formal_list;
  delete this->var_stmt;
}

void FunctionDeclNode::accept(Visitor *visitor) { visitor->visit(this); }

IRTree *FunctionDeclNode::accept(Translator *translator) {
    translator->visit(this);
    return NULL;
}

TypeNode *FunctionDeclNode::get_type() { return type; }

PointerNode *FunctionDeclNode::get_pointer() { return pointer; }

IdentifierNode *FunctionDeclNode::get_id() { return id; }

FormalListNode *FunctionDeclNode::get_formal_list() { return formal_list; }

VarStmtNode *FunctionDeclNode::getVar_stmt() { return var_stmt; }

VarFuncListNode *FunctionDeclNode::get_next() { return next; }

void FunctionDeclNode::set_next(VarFuncListNode *next) { this->next = next; }

ClassListNode::ClassListNode(IdentifierNode *id, VarFuncListNode *var_func,
                             IdentifierNode *parent_id, ClassListNode *class_list, int line) : Node(line) {
  this->id = id;
  this->var_func = var_func;
  this->parent_id = parent_id;
  this->next = class_list;
}

ClassListNode::~ClassListNode() {
  delete this->next;
  delete this->id;
  delete this->var_func;
  delete this->parent_id;
}

void ClassListNode::accept(Visitor *visitor) { visitor->visit(this); }

IRTree *ClassListNode::accept(Translator *translator) {
    translator->visit(this);
    return NULL;
}

IdentifierNode *ClassListNode::get_id() { return id; }

VarFuncListNode *ClassListNode::get_var_func() { return var_func; }

IdentifierNode *ClassListNode::get_parent_id() { return parent_id; }

ClassListNode *ClassListNode::get_next() { return next; }

TypeListNode::TypeListNode(VarDeclNode *var_list, IdentifierNode *id, TypeListNode *next, int line) : Node(line) {
  this->var_list = var_list;
  this->id = id;
  this->next = next;
}

TypeListNode::~TypeListNode() {
  delete this->next;
  delete this->var_list;
  delete this->id;
}

void TypeListNode::accept(Visitor *visitor) { visitor->visit(this); }

IRTree *TypeListNode::accept(Translator *translator) { return translator->visit(this); }

VarDeclNode *TypeListNode::get_var_list() { return var_list; }

IdentifierNode *TypeListNode::get_id() { return id; }

TypeListNode *TypeListNode::get_next() { return next; }

ProgramNode::ProgramNode(ClassListNode *class_list,
                         TypeListNode *type_list,
                         VarFuncListNode *var_func_list, int line) : Node(line) {
  this->class_list = class_list;
  this->type_list = type_list;
  this->var_func_list = var_func_list;
}

ProgramNode::~ProgramNode() {
  delete this->class_list;
  delete this->type_list;
  delete this->var_func_list;
}

void ProgramNode::accept(Visitor *visitor) { visitor->visit(this); }

IRTree *ProgramNode::accept(Translator *translator) {
    translator->visit(this);
    return NULL;
}

ClassListNode *ProgramNode::get_class_list() { return class_list; }

TypeListNode *ProgramNode::get_type_list() { return type_list; }

VarFuncListNode *ProgramNode::get_var_func_list() { return var_func_list; }

AssignNode::AssignNode(ExpNode *left_exp, ExpNode *right_exp, int line) : ExpNode(line) {
  this->left_exp = left_exp;
  this->right_exp = right_exp;
}

AssignNode::~AssignNode() {
  delete this->left_exp;
  this->right_exp = NULL;
}

void AssignNode::accept(Visitor *visitor) { visitor->visit(this); }

Stmt *AssignNode::accept(Translator *translator) { return translator->visit(this); }

ExpNode *AssignNode::get_left_exp() { return left_exp; }

ExpNode *AssignNode::get_right_exp() { return right_exp; }

PrimaryNode::PrimaryNode(ExpNode *exp, ExpNode *next, int line) : ExpNode(line) {
  this->exp = exp;
  this->next = next;
  this->owner_lex = NULL;
}

PrimaryNode::~PrimaryNode() {
  delete this->next;
  delete this->exp;
  this->owner_lex = NULL;
}

void PrimaryNode::accept(Visitor *visitor) { visitor->visit(this); }

Exp *PrimaryNode::accept(Translator *translator) { return translator->visit(this); }

ExpNode *PrimaryNode::get_exp() { return exp; }

ExpNode *PrimaryNode::get_next() { return next; }

const char *PrimaryNode::getOwner_lex() { return owner_lex; }

void PrimaryNode::setOwner_lex(const char *owner_lex) { this->owner_lex = owner_lex; }

FunctionCallNode::FunctionCallNode(IdentifierNode *id, ExpListNode *exp_list, ExpNode *next, int line) : ExpNode(line) {
  this->id = id;
  this->exp_list = exp_list;
  this->next = next;
}

FunctionCallNode::~FunctionCallNode() {
  delete this->exp_list;
  delete this->next;
}

void FunctionCallNode::accept(Visitor *visitor) { visitor->visit(this); }

Exp *FunctionCallNode::accept(Translator *translator) { return translator->visit(this); }

IdentifierNode *FunctionCallNode::get_id() { return id; }

ExpNode *FunctionCallNode::get_exp_list() { return exp_list; }

ExpNode *FunctionCallNode::get_next() { return next; }

const char *FunctionCallNode::getOwner_lex() { return owner_lex; }

void FunctionCallNode::setOwner_lex(const char *owner_lex) { this->owner_lex = owner_lex; }

FieldAccessNode::FieldAccessNode(ExpNode *exp, int line) : ExpNode(line) {
  this->exp = exp;
}

FieldAccessNode::~FieldAccessNode() {
  delete this->exp;
}

void FieldAccessNode::accept(Visitor *visitor) { visitor->visit(this); }

Exp *FieldAccessNode::accept(Translator *translator) { return translator->visit(this); }

ExpNode *FieldAccessNode::get_exp() { return exp; }

const char *FieldAccessNode::getOwner_lex() { return owner_lex; }

void FieldAccessNode::setOwner_lex(const char *owner_lex) { this->owner_lex = owner_lex; }

PointerAccessNode::PointerAccessNode(ExpNode *exp, int line) : ExpNode(line) {
  this->exp = exp;
}

PointerAccessNode::~PointerAccessNode() {
  delete this->exp;
}

void PointerAccessNode::accept(Visitor *visitor) { visitor->visit(this); }

Exp *PointerAccessNode::accept(Translator *translator) { return translator->visit(this); }

ExpNode *PointerAccessNode::get_exp() { return exp; }

const char *PointerAccessNode::getOwner_lex() { return owner_lex; }

void PointerAccessNode::setOwner_lex(const char *owner_lex) { this->owner_lex = owner_lex; }

ArrayAccessNode::ArrayAccessNode(ExpNode *index_exp, ExpNode *next, int line) : ExpNode(line) {
  this->index_exp = index_exp;
  this->next = next;
}

ArrayAccessNode::~ArrayAccessNode() {
  delete this->index_exp;
  delete this->next;
}

void ArrayAccessNode::accept(Visitor *visitor) { visitor->visit(this); }

Exp *ArrayAccessNode::accept(Translator *translator) { return translator->visit(this); }

ExpNode *ArrayAccessNode::get_index_exp() { return index_exp; }

ExpNode *ArrayAccessNode::get_next() { return next; }

NewNode::NewNode(IdentifierNode *id, ExpListNode *exp_list, int line) : ExpNode(line) {
  this->id = id;
  this->exp_list = exp_list;
}

NewNode::~NewNode() {
  delete this->id;
  delete this->exp_list;
}

void NewNode::accept(Visitor *visitor) { visitor->visit(this); }

Exp *NewNode::accept(Translator *translator) { return translator->visit(this); }

IdentifierNode *NewNode::get_id() { return id; }

ExpListNode *NewNode::get_exp_list() { return exp_list; }

RelationalOpNode::RelationalOpNode(int token_op, ExpNode *left_exp, ExpNode *right_exp, int line) : ExpNode(line) {
  this->token_op = token_op;
  this->left_exp = left_exp;
  this->right_exp = right_exp;
}

RelationalOpNode::~RelationalOpNode() {
  delete this->left_exp;
  this->right_exp = NULL;
}

void RelationalOpNode::accept(Visitor *visitor) { visitor->visit(this); }

Exp *RelationalOpNode::accept(Translator *translator) { return translator->visit(this); }

int RelationalOpNode::get_op() { return token_op; }

ExpNode *RelationalOpNode::get_left_exp() { return left_exp; }

ExpNode *RelationalOpNode::get_right_exp() { return right_exp; }

AdditionOpNode::AdditionOpNode(int token_op, ExpNode *left_exp, ExpNode *right_exp, int line) : ExpNode(line) {
  this->token_op = token_op;
  this->left_exp = left_exp;
  this->right_exp = right_exp;
}

AdditionOpNode::~AdditionOpNode() {
  delete this->left_exp;
  this->right_exp = NULL;
}

void AdditionOpNode::accept(Visitor *visitor) { visitor->visit(this); }

Exp *AdditionOpNode::accept(Translator *translator) { return translator->visit(this); }

int AdditionOpNode::get_op() { return token_op; }

ExpNode *AdditionOpNode::get_left_exp() { return left_exp; }

ExpNode *AdditionOpNode::get_right_exp() { return right_exp; }

MultiplicationOpNode::MultiplicationOpNode(int token_op, ExpNode *left_exp, ExpNode *right_exp, int line) : ExpNode(
        line) {
  this->token_op = token_op;
  this->left_exp = left_exp;
  this->right_exp = right_exp;
}

MultiplicationOpNode::~MultiplicationOpNode() {
  delete this->left_exp;
  this->right_exp = NULL;
}

void MultiplicationOpNode::accept(Visitor *visitor) { visitor->visit(this); }

Exp *MultiplicationOpNode::accept(Translator *translator) { return translator->visit(this); }

int MultiplicationOpNode::get_op() { return token_op; }

ExpNode *MultiplicationOpNode::get_left_exp() { return left_exp; }

ExpNode *MultiplicationOpNode::get_right_exp() { return right_exp; }

BooleanOpNode::BooleanOpNode(int token_op, ExpNode *left_exp, ExpNode *right_exp, int line) : ExpNode(line) {
  this->token_op = token_op;
  this->left_exp = left_exp;
  this->right_exp = right_exp;
}

BooleanOpNode::~BooleanOpNode() {
  delete this->left_exp;
  this->right_exp = NULL;
}

void BooleanOpNode::accept(Visitor *visitor) { visitor->visit(this); }

Exp *BooleanOpNode::accept(Translator *translator) { return translator->visit(this); }

int BooleanOpNode::get_op() { return token_op; }

ExpNode *BooleanOpNode::get_left_exp() { return left_exp; }

ExpNode *BooleanOpNode::get_right_exp() { return right_exp; }

BitwiseOpNode::BitwiseOpNode(int token_op, ExpNode *left_exp, ExpNode *right_exp, int line) : ExpNode(line) {
  this->token_op = token_op;
  this->left_exp = left_exp;
  this->right_exp = right_exp;
}

BitwiseOpNode::~BitwiseOpNode() {
  delete this->left_exp;
  this->right_exp = NULL;
}

void BitwiseOpNode::accept(Visitor *visitor) { visitor->visit(this); }

Exp *BitwiseOpNode::accept(Translator *translator) { return translator->visit(this); }

int BitwiseOpNode::get_op() { return token_op; }

ExpNode *BitwiseOpNode::get_left_exp() { return left_exp; }

ExpNode *BitwiseOpNode::get_right_exp() { return right_exp; }

VarDeclNode::VarDeclNode(TypeNode *type, IdListNode *id_list, VarFuncListNode *next, int line) : Node(line) {
  this->type = type;
  this->id_list = id_list;
  this->next = next;
}

VarDeclNode::VarDeclNode(TypeNode *type, IdListNode *id_list, VarDeclNode *next, int line) : Node(line) {
  this->type = type;
  this->id_list = id_list;
  this->next = new VarFuncListNode(next, line);
}

VarDeclNode::VarDeclNode(TypeNode *type, IdListNode *id_list, int line) : Node(line) {
  this->type = type;
  this->id_list = id_list;
  this->next = NULL;
}

VarDeclNode::~VarDeclNode() {
  delete this->next;
  delete this->type;
  delete this->id_list;
}

void VarDeclNode::accept(Visitor *visitor) { visitor->visit(this); }

Stmt *VarDeclNode::accept(Translator *translator) {
    translator->visit(this);
    return NULL;
}

TypeNode *VarDeclNode::get_type() { return type; }

IdListNode *VarDeclNode::get_id_list() { return id_list; }

VarFuncListNode *VarDeclNode::get_next() { return next; }

void VarDeclNode::set_next(VarFuncListNode *next) { this->next = next; }

TypeNode::TypeNode(int token, const char *lexeme, int line) : Node(line) {
  this->token = token;
  this->lexeme = lexeme;
}

TypeNode::~TypeNode() { this->lexeme = NULL; }

void TypeNode::accept(Visitor *visitor) { visitor->visit(this); }

IRTree *TypeNode::accept(Translator *translator) { return translator->visit(this); }

int TypeNode::get_token() const { return token; }

const char *TypeNode::get_lexeme() { return lexeme; }

VarStmtNode::VarStmtNode(VarDeclNode *decl, VarStmtNode *next, int line) : Node(line) {
  this->var_decl = decl;
  this->next = next;
}

VarStmtNode::VarStmtNode(StatementListNode *stmt_list, VarStmtNode *next, int line) : Node(line) {
  this->stmt_list = stmt_list;
  this->next = next;
}

VarStmtNode::~VarStmtNode() {
  delete this->next;
  delete this->var_decl;
  delete this->stmt_list;
}

void VarStmtNode::accept(Visitor *visitor) { visitor->visit(this); }

Stmt *VarStmtNode::accept(Translator *translator) { return translator->visit(this); }

VarDeclNode *VarStmtNode::get_var_decl() { return var_decl; }

StatementListNode *VarStmtNode::getStmt_list() { return stmt_list; }

VarStmtNode *VarStmtNode::get_next() { return next; }

FormalListNode::FormalListNode(TypeNode *type, PointerNode *pointer, IdentifierNode *id, ArrayDeclNode *array,
                               FormalListNode *next, int line) : Node(line) {
  this->type = type;
  this->pointer = pointer;
  this->id = id;
  this->array = array;
  this->next = next;
}

FormalListNode::~FormalListNode() {
  delete this->next;
  delete this->type;
  delete this->pointer;
  delete this->id;
  delete this->array;
}

bool FormalListNode::is_equal(FormalListNode *formal) {
  if (formal == NULL) return false;
  if (this->get_type() != NULL && formal->get_type() != NULL &&
      this->get_type()->get_token() != formal->get_type()->get_token())
    return false;
  if ((this->get_pointer() != NULL && formal->get_pointer() == NULL) ||
      (this->get_pointer() == NULL && formal->get_pointer() != NULL))
    return false;
  if ((this->get_array() != NULL && formal->get_array() == NULL) ||
      (this->get_array() == NULL && formal->get_array() != NULL))
    return false;
  if ((this->get_next() != NULL && formal->get_next() == NULL) || (this->get_next() == NULL && formal->get_next() !=
                                                                                               NULL))
    return false;
  return true;
}

void FormalListNode::accept(Visitor *visitor) { visitor->visit(this); }

IRTree *FormalListNode::accept(Translator *translator) {
    translator->visit(this);
    return NULL;
}

TypeNode *FormalListNode::get_type() { return type; }

PointerNode *FormalListNode::get_pointer() { return pointer; }

IdentifierNode *FormalListNode::get_id() { return id; }

ArrayDeclNode *FormalListNode::get_array() { return array; }

FormalListNode *FormalListNode::get_next() { return next; }

VarFuncListNode::VarFuncListNode(VarDeclNode *var_decl, int line) : Node(line) {
  this->var_decl = var_decl;
}

VarFuncListNode::VarFuncListNode(FunctionDeclNode *func_decl, int line) : Node(line) {
  this->func_decl = func_decl;
}

VarFuncListNode::~VarFuncListNode() {
  delete this->var_decl;
  delete this->func_decl;
}

void VarFuncListNode::accept(Visitor *visitor) { visitor->visit(this); }

IRTree *VarFuncListNode::accept(Translator *translator) { return translator->visit(this); }

VarDeclNode *VarFuncListNode::get_var_decl() { return var_decl; }

FunctionDeclNode *VarFuncListNode::getFunc_list() { return func_decl; }

Node::Node(int line) { this->line = line; }

Node::~Node() {}

int Node::get_line() { return line; }

StatementNode::StatementNode(int line) : Node(line) {}

StatementNode::~StatementNode() {}

ExpNode::ExpNode(int line) : StatementNode(line) {
  this->type = 0;
  this->type_lex = NULL;
  this->lvalue = false;
  this->pointer = false;
  this->value = NULL;
  this->array_size = -1;

}

ExpNode::~ExpNode() {}

int ExpNode::get_type() { return type; }

void ExpNode::set_type(int type) { this->type = type; }

const char *ExpNode::get_value() { return value; }

void ExpNode::set_value(const char *value) { this->value = value; }

bool ExpNode::is_lvalue() { return lvalue; }

void ExpNode::set_lvalue(bool lvalue) { this->lvalue = lvalue; }

bool ExpNode::is_pointer() { return pointer; }

void ExpNode::set_pointer(bool pointer) { this->pointer = pointer; }

int ExpNode::get_array_size() { return array_size; }

void ExpNode::set_array_size(int array) { this->array_size = array; }

const char *ExpNode::getType_lex() { return type_lex; }

void ExpNode::setType_lex(const char *type_lex) { this->type_lex = type_lex; }

IdentifierNode::IdentifierNode(const char *lexeme, int line) : ExpNode(line) {
  this->lexeme = lexeme;
  this->offset = 0;
}

IdentifierNode::~IdentifierNode() { this->lexeme = NULL; }

void IdentifierNode::accept(Visitor *visitor) { visitor->visit(this); }

Exp *IdentifierNode::accept(Translator *translator) { return translator->visit(this); }

const char *IdentifierNode::get_lexeme() { return lexeme; }

int IdentifierNode::get_offset() const {
  return offset;
}

void IdentifierNode::set_offset(int offset) {
  IdentifierNode::offset = offset;
}

IntegerNode::IntegerNode(const char *lexeme, int line) : ExpNode(line) { this->lexeme = lexeme; }

IntegerNode::~IntegerNode() { this->lexeme = NULL; }

void IntegerNode::accept(Visitor *visitor) { visitor->visit(this); }

Exp *IntegerNode::accept(Translator *translator) { return translator->visit(this); }

const char *IntegerNode::get_lexeme() { return lexeme; }

RealNode::RealNode(const char *lexeme, int line) : ExpNode(line) { this->lexeme = lexeme; }

RealNode::~RealNode() { this->lexeme = NULL; }

void RealNode::accept(Visitor *visitor) { visitor->visit(this); }

Exp *RealNode::accept(Translator *translator) { return translator->visit(this); }

const char *RealNode::get_lexeme() { return lexeme; }

const char *LiteralNode::get_lexeme() { return lexeme; }

Exp *LiteralNode::accept(Translator *translator) { return translator->visit(this); }

void LiteralNode::accept(Visitor *visitor) { visitor->visit(this); }

LiteralNode::~LiteralNode() { this->lexeme = NULL; }

LiteralNode::LiteralNode(const char *lexeme, int line) : ExpNode(line) { this->lexeme = lexeme; }

CharacterNode::CharacterNode(const char *lexeme, int line) : ExpNode(line) { this->lexeme = lexeme; }

CharacterNode::~CharacterNode() { this->lexeme = NULL; }

void CharacterNode::accept(Visitor *visitor) { visitor->visit(this); }

Exp *CharacterNode::accept(Translator *translator) { return translator->visit(this); }

const char *CharacterNode::get_lexeme() { return lexeme; }

BreakNode::BreakNode(int line) : StatementNode(line) {}

BreakNode::~BreakNode() {}

void BreakNode::accept(Visitor *visitor) { visitor->visit(this); }

Stmt *BreakNode::accept(Translator *translator) { return translator->visit(this); }

PrintlnNode::PrintlnNode(ExpListNode *exp_list, int line) : StatementNode(line) { this->exp_list = exp_list; }

PrintlnNode::~PrintlnNode() { delete this->exp_list; }

ExpListNode *PrintlnNode::get_exp_list() { return exp_list; }

Stmt *PrintlnNode::accept(Translator *translator) { return translator->visit(this); }

void PrintlnNode::accept(Visitor *visitor) { visitor->visit(this); }

ReadNode::ReadNode(ExpNode *exp, int line) : StatementNode(line) { this->exp = exp; }

ReadNode::~ReadNode() { delete this->exp; }

void ReadNode::accept(Visitor *visitor) { visitor->visit(this); }

Stmt *ReadNode::accept(Translator *translator) { return translator->visit(this); }

ExpNode *ReadNode::get_exp() { return exp; }

ReturnNode::ReturnNode(ExpNode *exp, int line) : StatementNode(line) { this->exp = exp; }

ReturnNode::~ReturnNode() { delete this->exp; }

void ReturnNode::accept(Visitor *visitor) { visitor->visit(this); }

Stmt *ReturnNode::accept(Translator *translator) { return translator->visit(this); }

ExpNode *ReturnNode::get_exp() { return exp; }

ThrowNode::ThrowNode(int line) : StatementNode(line) {}

ThrowNode::~ThrowNode() {}

void ThrowNode::accept(Visitor *visitor) { visitor->visit(this); }

Stmt *ThrowNode::accept(Translator *translator) { return translator->visit(this); }

PointerNode::PointerNode(int line) : Node(line) {}

PointerNode::~PointerNode() {}

void PointerNode::accept(Visitor *visitor) { visitor->visit(this); }

IRTree *PointerNode::accept(Translator *translator) { return translator->visit(this); }

AddressValueNode::AddressValueNode(ExpNode *exp, int line) : ExpNode(line) { this->exp = exp; }

AddressValueNode::~AddressValueNode() { delete this->exp; }

void AddressValueNode::accept(Visitor *visitor) { visitor->visit(this); }

Exp *AddressValueNode::accept(Translator *translator) { return translator->visit(this); }

ExpNode *AddressValueNode::get_exp() const { return exp; }

PointerValueNode::PointerValueNode(ExpNode *exp, int line) : ExpNode(line) { this->exp = exp; }

PointerValueNode::~PointerValueNode() { delete this->exp; }

void PointerValueNode::accept(Visitor *visitor) { visitor->visit(this); }

Exp *PointerValueNode::accept(Translator *translator) { return translator->visit(this); }

ExpNode *PointerValueNode::get_exp() const { return exp; }

ArrayDeclNode::ArrayDeclNode(ExpNode *index_exp, int line) : ExpNode(line) { this->index_exp = index_exp; }

ArrayDeclNode::~ArrayDeclNode() { delete this->index_exp; }

void ArrayDeclNode::accept(Visitor *visitor) { visitor->visit(this); }

Exp *ArrayDeclNode::accept(Translator *translator) { return translator->visit(this); }

ExpNode *ArrayDeclNode::get_index_exp() { return index_exp; }

TrueNode::TrueNode(int line) : ExpNode(line) {}

TrueNode::~TrueNode() {}

void TrueNode::accept(Visitor *visitor) { visitor->visit(this); }

Exp *TrueNode::accept(Translator *translator) { return translator->visit(this); }

FalseNode::FalseNode(int line) : ExpNode(line) {}

FalseNode::~FalseNode() {}

void FalseNode::accept(Visitor *visitor) { visitor->visit(this); }

Exp *FalseNode::accept(Translator *translator) { return translator->visit(this); }

ThisNode::ThisNode(int line) : ExpNode(line) {}

ThisNode::~ThisNode() {}

void ThisNode::accept(Visitor *visitor) { visitor->visit(this); }

Exp *ThisNode::accept(Translator *translator) { return translator->visit(this); }

NotNode::NotNode(ExpNode *exp, int line) : ExpNode(line) { this->exp = exp; }

NotNode::~NotNode() { delete this->exp; }

void NotNode::accept(Visitor *visitor) { visitor->visit(this); }

Exp *NotNode::accept(Translator *translator) { return translator->visit(this); }

ExpNode *NotNode::get_exp() { return exp; }

SignNode::SignNode(ExpNode *exp, int line) : ExpNode(line) { this->exp = exp; }

SignNode::~SignNode() { delete this->exp; }

void SignNode::accept(Visitor *visitor) { visitor->visit(this); }

Exp *SignNode::accept(Translator *translator) { return translator->visit(this); }

ExpNode *SignNode::get_exp() { return exp; }
