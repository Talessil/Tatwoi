#ifndef CANONIZER_H_INCLUDED
#define CANONIZER_H_INCLUDED

class IRTree;
class Exp;
class ExpList;
class ConstNode;
class ConstfNode;
class NameNode;
class TempNode;
class BinOpNode;
class MemNode;
class CallNode;
class EseqNode;
class Stmt;
class StmtList;
class MoveNode;
class ExpressionNode;
class JumpNode;
class CJumpNode;
class SeqNode;
class LabelNode;

class Fragment;
class Procedure;
class Literal;
class Variable;

#include "intermediate_code.h"

class IRVisitor {
  public:
    IRVisitor();
    virtual ~IRVisitor();
    virtual void visit(IRTree *node) = 0;
    virtual void visit(Exp *node) = 0;
    virtual void visit(ExpList *node) = 0;
    virtual void visit(ConstNode *node) = 0;
    virtual void visit(ConstfNode *node) = 0;
    virtual void visit(NameNode *node) = 0;
    virtual void visit(TempNode *node) = 0;
    virtual void visit(BinOpNode *node) = 0;
    virtual void visit(MemNode *node) = 0;
    virtual void visit(CallNode *node) = 0;
    virtual void visit(EseqNode *node) = 0;
    virtual void visit(Stmt *node) = 0;
    virtual void visit(StmtList *node) = 0;
    virtual void visit(MoveNode *node) = 0;
    virtual void visit(ExpressionNode *node) = 0;
    virtual void visit(JumpNode *node) = 0;
    virtual void visit(CJumpNode *node) = 0;
    virtual void visit(SeqNode *node) = 0;
    virtual void visit(LabelNode *node) = 0;
};

class PrintIRVisitor : public IRVisitor {
  private:
    unsigned int indentation;
    void indent();
    void print(const char *s);
    void increase_indent();
    void decrease_indent();
  public:
    PrintIRVisitor(IRTree *node);
    virtual ~PrintIRVisitor();
    void visit(IRTree *node);
    void visit(Exp *node);
    void visit(ExpList *node);
    void visit(ConstNode *node);
    void visit(ConstfNode *node);
    void visit(NameNode *node);
    void visit(TempNode *node);
    void visit(BinOpNode *node);
    void visit(MemNode *node);
    void visit(CallNode *node);
    void visit(EseqNode *node);
    void visit(Stmt *node);
    void visit(StmtList *node);
    void visit(MoveNode *node);
    void visit(ExpressionNode *node);
    void visit(JumpNode *node);
    void visit(CJumpNode *node);
    void visit(SeqNode *node);
    void visit(LabelNode *node);
};

class Canonizer {
  private:
    bool changed;
  public:
    Canonizer();
    virtual ~Canonizer();

    bool is_changed();
    void set_changed(bool changed);

    Fragment *visit(Fragment *fragment);
    Procedure *visit(Procedure *fragment);
    Literal *visit(Literal *fragment);
    Variable *visit(Variable *fragment);
    Exp *visit(Exp *node);
    Exp *visit(ExpList *node);
    Exp *visit(ConstNode *node);
    Exp *visit(ConstfNode *node);
    Exp *visit(NameNode *node);
    Exp *visit(TempNode *node);
    Exp *visit(BinOpNode *node);
    Exp *visit(MemNode *node);
    Exp *visit(CallNode *node);
    Exp *visit(EseqNode *node);
    Stmt *visit(Stmt *node);
    Stmt *visit(StmtList *node);
    Stmt *visit(MoveNode *node);
    Stmt *visit(ExpressionNode *node);
    Stmt *visit(JumpNode *node);
    Stmt *visit(CJumpNode *node);
    Stmt *visit(SeqNode *node);
    Stmt *visit(LabelNode *node);
};

#endif // CANONIZER_H_INCLUDED
