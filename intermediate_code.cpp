#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "intermediate_code.h"
#include "lex_analyzer.h"

int num_label = 0;
int num_temp = 0;
Temp *fp;

Fragment::~Fragment() {
  delete next;
}

Fragment *Fragment::get_next() { return next; }

void Fragment::set_next(Fragment *next) { this->next = next; }

Fragment::Fragment() {
  this->next = NULL;
}

Procedure::Procedure(Frame *frame, Stmt *body) {
  this->frame = frame;
  this->body = body;
}

Procedure::~Procedure() {
  delete this->frame;
  delete this->body;
}

Frame *Procedure::get_frame() { return frame; }

Stmt *Procedure::get_body() { return body; }

Procedure *Procedure::accept(Canonizer *canonizer) {
  return canonizer->visit(this);
}

void Procedure::set_frame(Frame *frame) {
  Procedure::frame = frame;
}

void Procedure::set_body(Stmt *body) {
  Procedure::body = body;
}

Temp *Procedure::accept(AssemblyGenerator *generator) {
  generator->visit(this);
  return NULL;
}

Literal::Literal(const char *value, int label_num) {
  this->value = value;
  this->label_num = label_num;
}

Literal::~Literal() {
  this->value = NULL;
}

const char *Literal::get_value() { return value; }

int Literal::getLabel_num() { return label_num; }

Literal *Literal::accept(Canonizer *canonizer) {
  return canonizer->visit(this);
}

Temp *Literal::accept(AssemblyGenerator *generator) {
  generator->visit(this);
  return NULL;
}

Variable::Variable(int type, int nbytes, const char *name) {
  this->type = type;
  this->nbytes = nbytes;
  this->name = name;
}

Variable::~Variable() {
  this->name = NULL;
}

int Variable::get_type() { return type; }

int Variable::get_nbytes() { return nbytes; }

const char *Variable::get_name() { return name; }

Variable *Variable::accept(Canonizer *canonizer) {
  return canonizer->visit(this);
}

Temp *Variable::accept(AssemblyGenerator *generator) {
  generator->visit(this);
  return NULL;
}

Temp::Temp() {
  char name[250];
  sprintf(name, "$%d", num_temp++);
  this->temp_name = new char[strlen(name) + 1];
  strcpy(this->temp_name, name);
}

Temp::Temp(const char *name) {
  this->temp_name = new char[strlen(name) + 1];
  strcpy(this->temp_name, name);
}

Temp::~Temp() {
  this->temp_name = NULL;
}

char *Temp::to_string() {
  const char *prefix = "temp";
  char *result = new char[strlen(prefix) + strlen(temp_name) + 1];
  strcpy(result, prefix);
  strcat(result, temp_name);
  return result;
}

char *Temp::get_name() {
  return temp_name;
}

TempList::TempList(Temp *temp, TempList *next) {
  this->temp = temp;
  this->next = next;
}

TempList::~TempList() {
  delete this->temp;
  delete this->next;
}

Temp *TempList::get_temp() { return temp; }

TempList *TempList::get_next() { return next; }

Label::Label() {
  char name[250];
  sprintf(name, "label_%d", num_label++);
  this->label_name = new char[strlen(name) + 1];
  strcpy(this->label_name, name);
}

Label::Label(const char *name) {
  this->label_name = new char[strlen(name) + 1];
  strcpy(this->label_name, name);
}

Label::~Label() {
  label_name = NULL;
}

char *Label::to_string() {
  /*const char *prefix = "label_";
  char *result = new char[strlen(prefix) + strlen(label_name) + 1];
  strcpy(result, prefix);
  strcat(result, label_name);
  return result;*/
  return label_name;
}

char *Label::get_name() {
  return label_name;
}

LabelList::LabelList(Label *label, LabelList *next) {
  this->label = label;
  this->next = next;
}

LabelList::~LabelList() {
  delete this->label;
  delete this->next;
}

Label *LabelList::get_label() { return label; }

LabelList *LabelList::get_next() { return next; }

AccessList::AccessList(LocalAccess *local_access, AccessList *next) {
  this->local_access = local_access;
  this->next = next;
}

AccessList::~AccessList() {
  delete this->local_access;
  delete this->next;
}

LocalAccess *AccessList::getLocal_access() { return local_access; }

AccessList *AccessList::get_next() { return next; }

InFrame::InFrame(int offset) {
  this->offset = offset;
}

InFrame::~InFrame() {

}

Exp *InFrame::access_code() {
  if (fp == NULL) fp = new Temp("$fp");
  return new MemNode(new BinOpNode(PLUS, new TempNode(fp), new ConstNode(get_offset())));
}

int InFrame::get_offset() { return offset; }

InReg::InReg(Temp *temp) {
  this->temp = temp;
}

InReg::~InReg() {
  delete this->temp;
}

Exp *InReg::access_code() {
  return new TempNode(get_temp());
}

Temp *InReg::get_temp() { return temp; }

MIPSFrame::MIPSFrame() {
  this->label = NULL;
  this->return_value = new Temp("$v0");
  this->local_data = NULL;
  this->frame_size = 0;
}

MIPSFrame::~MIPSFrame() {
  delete this->label;
  delete this->return_value;
  delete this->local_data;
}

LocalAccess *MIPSFrame::add_param(bool escape, int size) {
  LocalAccess *frame;
  if (escape) frame = new InFrame(size);
  else frame = new InReg(new Temp());
  this->local_data = new AccessList(frame, this->local_data);
  return frame;
}

LocalAccess *MIPSFrame::add_local(bool escape, int size) {
  LocalAccess *frame;
  if (escape) {
    frame_size -= size;
    frame = new InFrame(frame_size);
  } else frame = new InReg(new Temp());
  this->local_data = new AccessList(frame, this->local_data);
  return frame;
}

int MIPSFrame::getCall_size() {
  return call_size;
}

void MIPSFrame::setCall_size(int call_size) {
  MIPSFrame::call_size = call_size;
}

int MIPSFrame::getFrame_size() {
  return frame_size;
}

Label *MIPSFrame::get_label() {
  return label;
}

Temp *MIPSFrame::getReturn_value() {
  return return_value;
}

AccessList *MIPSFrame::getLocal_data() {
  return local_data;
}

ExpList::ExpList(Exp *exp, ExpList *next) {
  this->exp = exp;
  this->next = next;
}

ExpList::~ExpList() {
  delete this->exp;
  delete this->next;
}

Exp *ExpList::get_exp() { return exp; }

ExpList *ExpList::get_next() { return next; }

void ExpList::accept(PrintIRVisitor *visitor) { visitor->visit(this); }

Exp *ExpList::accept(Canonizer *canonizer) {
  Exp *exp = canonizer->visit(this);
  return exp;
}

void ExpList::set_exp(Exp *exp) {
  this->exp = exp;
}

void ExpList::set_next(ExpList *next) {
  ExpList::next = next;
}

Temp *ExpList::accept(AssemblyGenerator *generator) {
  return NULL;
}

ConstNode::ConstNode(int value) {
  this->value = value;
}

ConstNode::~ConstNode() {

}

int ConstNode::get_value() { return value; }

void ConstNode::accept(PrintIRVisitor *visitor) { visitor->visit(this); }

Exp *ConstNode::accept(Canonizer *canonizer) {
  return canonizer->visit(this);
}

Temp *ConstNode::accept(AssemblyGenerator *generator) {
  return generator->visit(this);
}

ConstfNode::ConstfNode(float value) {
  this->value = value;
}

ConstfNode::~ConstfNode() {

}

float ConstfNode::get_value() { return value; }

void ConstfNode::accept(PrintIRVisitor *visitor) { visitor->visit(this); }

Exp *ConstfNode::accept(Canonizer *canonizer) {
  return canonizer->visit(this);
}

Temp *ConstfNode::accept(AssemblyGenerator *generator) {
  return generator->visit(this);
}

NameNode::NameNode(Label *label) {
  this->label = label;
}

NameNode::~NameNode() {
  this->label = NULL;
}

Label *NameNode::get_label() { return label; }

void NameNode::accept(PrintIRVisitor *visitor) { visitor->visit(this); }

Exp *NameNode::accept(Canonizer *canonizer) {
  return canonizer->visit(this);
}

Temp *NameNode::accept(AssemblyGenerator *generator) {
  return generator->visit(this);
}

TempNode::TempNode(Temp *temp) {
  this->temp = temp;
}

TempNode::~TempNode() {
  this->temp = NULL;
}

Temp *TempNode::get_temp() { return temp; }

void TempNode::accept(PrintIRVisitor *visitor) { visitor->visit(this); }

Exp *TempNode::accept(Canonizer *canonizer) {
  return canonizer->visit(this);
}

Temp *TempNode::accept(AssemblyGenerator *generator) {
  return generator->visit(this);
}


BinOpNode::BinOpNode(int op, Exp *exp1, Exp *exp2) {
  this->token_op = op;
  this->exp1 = exp1;
  this->exp2 = exp2;
}

BinOpNode::~BinOpNode() {
  delete exp1;
  delete exp2;
}

int BinOpNode::getToken_op() { return token_op; }

Exp *BinOpNode::get_exp1() { return exp1; }

Exp *BinOpNode::get_exp2() { return exp2; }

void BinOpNode::accept(PrintIRVisitor *visitor) { visitor->visit(this); }

Exp *BinOpNode::accept(Canonizer *canonizer) {
  return canonizer->visit(this);
}

void BinOpNode::set_exp1(Exp *exp1) {
  BinOpNode::exp1 = exp1;
}

void BinOpNode::set_exp2(Exp *exp2) {
  BinOpNode::exp2 = exp2;
}

Temp *BinOpNode::accept(AssemblyGenerator *generator) {
  return generator->visit(this);
}

MemNode::MemNode(Exp *exp) {
  this->exp = exp;
}

MemNode::~MemNode() {
  delete this->exp;
}

Exp *MemNode::get_exp() { return exp; }

void MemNode::accept(PrintIRVisitor *visitor) { visitor->visit(this); }

void MemNode::set_exp(Exp *exp) {
  this->exp = exp;
}

Exp *MemNode::accept(Canonizer *canonizer) {
  return canonizer->visit(this);
}

Temp *MemNode::accept(AssemblyGenerator *generator) {
  return generator->visit(this);
}

CallNode::CallNode(Exp *exp, ExpList *expression_list) {
  this->function = exp;
  this->arguments = expression_list;
}

CallNode::~CallNode() {
  delete this->function;
  delete this->arguments;
}

Exp *CallNode::get_function() { return function; }

ExpList *CallNode::get_arguments() { return arguments; }

void CallNode::accept(PrintIRVisitor *visitor) { visitor->visit(this); }

Exp *CallNode::accept(Canonizer *canonizer) {
  return canonizer->visit(this);
}

void CallNode::set_function(Exp *function) {
  CallNode::function = function;
}

void CallNode::set_arguments(ExpList *arguments) {
  CallNode::arguments = arguments;
}

bool CallNode::is_canonized() const {
  return canonized;
}

void CallNode::set_canonized(bool canonized) {
  CallNode::canonized = canonized;
}

Temp *CallNode::accept(AssemblyGenerator *generator) {
  return generator->visit(this);
}

EseqNode::EseqNode(Stmt *stmt, Exp *exp) {
  this->stmt = stmt;
  this->exp = exp;
}

EseqNode::~EseqNode() {
  delete this->stmt;
  delete this->exp;
}

Stmt *EseqNode::get_stmt() { return stmt; }

Exp *EseqNode::get_exp() { return exp; }

void EseqNode::accept(PrintIRVisitor *visitor) { visitor->visit(this); }

Exp *EseqNode::accept(Canonizer *canonizer) {
  return canonizer->visit(this);
}

void EseqNode::set_stmt(Stmt *stmt) {
  EseqNode::stmt = stmt;
}

void EseqNode::set_exp(Exp *exp) {
  EseqNode::exp = exp;
}

Temp *EseqNode::accept(AssemblyGenerator *generator) {
  return NULL;
}

StmtList::StmtList(Stmt *stmt, StmtList *next) {
  this->stmt = stmt;
  this->next = next;
}

StmtList::~StmtList() {
  delete this->stmt;
  delete this->next;
}

Stmt *StmtList::get_stmt() { return stmt; }

StmtList *StmtList::get_next() { return next; }

void StmtList::accept(PrintIRVisitor *visitor) {
  visitor->visit(this);
}

Stmt *StmtList::accept(Canonizer *canonizer) {
  return canonizer->visit(this);
}

void StmtList::set_stmt(Stmt *stmt) {
  StmtList::stmt = stmt;
}

void StmtList::set_next(StmtList *next) {
  StmtList::next = next;
}

MoveNode::MoveNode(Exp *destiny, Exp *source) {
  this->destiny = destiny;
  this->source = source;
}

MoveNode::~MoveNode() {
  delete this->destiny;
  delete this->source;
}

Exp *MoveNode::get_destiny() { return destiny; }

Exp *MoveNode::get_source() { return source; }

void MoveNode::accept(PrintIRVisitor *visitor) { visitor->visit(this); }

Stmt *MoveNode::accept(Canonizer *canonizer) {
  return canonizer->visit(this);
}

void MoveNode::set_destiny(Exp *destiny) {
  MoveNode::destiny = destiny;
}

void MoveNode::set_source(Exp *source) {
  MoveNode::source = source;
}

Temp *MoveNode::accept(AssemblyGenerator *generator) {
  generator->visit(this);
  return NULL;
}

ExpressionNode::ExpressionNode(Exp *exp) {
  this->exp = exp;
}

ExpressionNode::~ExpressionNode() {
  delete this->exp;
}

Exp *ExpressionNode::get_exp() { return exp; }

void ExpressionNode::accept(PrintIRVisitor *visitor) { visitor->visit(this); }

Stmt *ExpressionNode::accept(Canonizer *canonizer) {
  return canonizer->visit(this);
}

void ExpressionNode::set_exp(Exp *exp) {
  ExpressionNode::exp = exp;
}

Temp *ExpressionNode::accept(AssemblyGenerator *generator) {
  generator->visit(this);
  return NULL;
}

JumpNode::JumpNode(Exp *destiny) {
  this->destiny = destiny;
  this->label_list = NULL;
}

JumpNode::JumpNode(Exp *destiny, LabelList *label_list) {
  this->destiny = destiny;
  this->label_list = label_list;
}

JumpNode::~JumpNode() {
  delete this->destiny;
  delete this->label_list;
}

Exp *JumpNode::get_destiny() { return destiny; }

LabelList *JumpNode::getLabel_list() { return label_list; }

void JumpNode::accept(PrintIRVisitor *visitor) { visitor->visit(this); }

Stmt *JumpNode::accept(Canonizer *canonizer) {
  return canonizer->visit(this);
}

void JumpNode::set_destiny(Exp *destiny) {
  JumpNode::destiny = destiny;
}

void JumpNode::setLabel_list(LabelList *label_list) {
  JumpNode::label_list = label_list;
}

Temp *JumpNode::accept(AssemblyGenerator *generator) {
  generator->visit(this);
  return NULL;
}

CJumpNode::CJumpNode(int op, Exp *exp1, Exp *exp2, Exp *cond_true, Exp *cond_false) {
  this->token_op = op;
  this->exp1 = exp1;
  this->exp2 = exp2;
  this->cond_true = cond_true;
  this->cond_false = cond_false;
}

CJumpNode::~CJumpNode() {
  delete this->exp1;
  delete this->exp2;
  delete this->cond_true;
  delete this->cond_false;
}

int CJumpNode::getToken_op() { return token_op; }

Exp *CJumpNode::get_exp1() { return exp1; }

Exp *CJumpNode::get_exp2() { return exp2; }

Exp *CJumpNode::getCond_true() { return cond_true; }

Exp *CJumpNode::getCond_false() { return cond_false; }

void CJumpNode::accept(PrintIRVisitor *visitor) { visitor->visit(this); }

Stmt *CJumpNode::accept(Canonizer *canonizer) {
  return canonizer->visit(this);
}

void CJumpNode::setToken_op(int token_op) {
  this->token_op = token_op;
}

void CJumpNode::set_exp1(Exp *exp1) {
  CJumpNode::exp1 = exp1;
}

void CJumpNode::set_exp2(Exp *exp2) {
  CJumpNode::exp2 = exp2;
}

void CJumpNode::setCond_true(Exp *cond_true) {
  CJumpNode::cond_true = cond_true;
}

void CJumpNode::setCond_false(Exp *cond_false) {
  CJumpNode::cond_false = cond_false;
}

bool CJumpNode::is_canonized() const {
  return canonized;
}

void CJumpNode::set_canonized(bool canonized) {
  CJumpNode::canonized = canonized;
}

Temp *CJumpNode::accept(AssemblyGenerator *generator) {
  generator->visit(this);
  return NULL;
}

SeqNode::SeqNode(Stmt *stmt1, Stmt *stmt2) {
  this->stmt1 = stmt1;
  this->stmt2 = stmt2;
}

SeqNode::~SeqNode() {
  delete this->stmt1;
  delete this->stmt2;
}

Stmt *SeqNode::get_stmt1() { return stmt1; }

Stmt *SeqNode::get_stmt2() { return stmt2; }

void SeqNode::accept(PrintIRVisitor *visitor) { visitor->visit(this); }

Stmt *SeqNode::accept(Canonizer *canonizer) {
  return canonizer->visit(this);
}

void SeqNode::set_stmt1(Stmt *stmt1) {
  SeqNode::stmt1 = stmt1;
}

void SeqNode::set_stmt2(Stmt *stmt2) {
  SeqNode::stmt2 = stmt2;
}

Temp *SeqNode::accept(AssemblyGenerator *generator) {
  generator->visit(this);
  return NULL;
}

LabelNode::LabelNode(Label *label) {
  this->label = label;
}

LabelNode::~LabelNode() {
  this->label = NULL;
}

Label *LabelNode::get_label() { return label; }

void LabelNode::accept(PrintIRVisitor *visitor) { visitor->visit(this); }

Stmt *LabelNode::accept(Canonizer *canonizer) {
  return canonizer->visit(this);
}

Temp *LabelNode::accept(AssemblyGenerator *generator) {
  generator->visit(this);
  return NULL;
}

Stmt::~Stmt() {}

LocalAccess::~LocalAccess() {}

Frame::~Frame() {}

IRTree::~IRTree() {}

Exp::~Exp() {}

Exp *Exp::accept(Canonizer *canonizer) {
  return canonizer->visit(this);
}
