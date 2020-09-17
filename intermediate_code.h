#ifndef INTERMEDIATE_CODE_H_INCLUDED
#define INTERMEDIATE_CODE_H_INCLUDED

#include "ir_visitor.h"

class Canonizer;
class AssemblyGenerator;
class PrintIRVisitor;
class Frame;
class Stmt;

#include "generate_assembly.h"

class Fragment {
  protected:
    Fragment *next;
  public:
    Fragment();
    virtual ~Fragment();
    virtual Fragment *get_next();
    void set_next(Fragment *next);

    virtual Fragment *accept(Canonizer *canonizer) = 0;
    virtual Temp *accept(AssemblyGenerator *generator) = 0;
};

class Procedure : public Fragment {
  private:
    Frame *frame;
    Stmt *body;
  public:
    Procedure(Frame *frame, Stmt *body);
    virtual ~Procedure();
    Frame *get_frame();
    Stmt *get_body();
    void set_frame(Frame *frame);
    void set_body(Stmt *body);
    virtual Procedure *accept(Canonizer *canonizer);
    virtual Temp *accept(AssemblyGenerator *generator);
};

class Literal : public Fragment {
  private:
    const char *value;
    int label_num;
  public:
    Literal(const char *value, int label_num);
    virtual ~Literal();
    const char *get_value();
    int getLabel_num();
    virtual Literal *accept(Canonizer *canonizer);
    virtual Temp *accept(AssemblyGenerator *generator);
};

class Variable : public Fragment {
  private:
    int type;
    int nbytes;
    const char *name;
  public:
    Variable(int type, int nbytes, const char *name);
    virtual ~Variable();
    int get_type();
    int get_nbytes();
    const char *get_name();
    virtual Variable *accept(Canonizer *canonizer);
    virtual Temp *accept(AssemblyGenerator *generator);
};

class Temp {
  private:
    char *temp_name;
  public:
    Temp();
    Temp(const char *name);
    virtual ~Temp();
    char *to_string();
    char *get_name();
};

class TempList {
  private:
    Temp *temp;
    TempList *next;
  public:
    TempList(Temp *temp, TempList *next);
    virtual ~TempList();
    Temp *get_temp();
    TempList *get_next();
};

class Label {
  private:
    char *label_name;
  public:
    Label();
    Label(const char *name);
    virtual ~Label();
    char *to_string();
    char *get_name();
};

class LabelList {
  private:
    Label *label;
    LabelList *next;
  public:
    LabelList(Label *label, LabelList *next);
    virtual ~LabelList();
    Label *get_label();
    LabelList *get_next();
};

class LocalAccess {
  public:
    virtual ~LocalAccess();
    virtual Exp *access_code() = 0;
};

class AccessList {
  private:
    LocalAccess *local_access;
    AccessList *next;
  public:
    AccessList(LocalAccess *local_access, AccessList *next);
    virtual ~AccessList();
    LocalAccess *getLocal_access();
    AccessList *get_next();
};

class Frame {
  public:
    virtual ~Frame();
    virtual LocalAccess *add_param(bool escape, int bytes_num) = 0;
    virtual LocalAccess *add_local(bool escape, int bytes_num) = 0;
};

class InFrame : public LocalAccess {
  private:
    int offset;
  public:
    InFrame(int offset);
    virtual ~InFrame();
    int get_offset();
    virtual Exp *access_code();
};

class InReg : public LocalAccess {
  private:
    Temp *temp;
  public:
    InReg(Temp *temp);
    virtual ~InReg();
    Temp *get_temp();
    virtual Exp *access_code();
};

class MIPSFrame : public Frame {
  private:
    Label *label;
    Temp *return_value;
    AccessList *local_data;
    int call_size;
    int frame_size;
  public:
    MIPSFrame();
    virtual ~MIPSFrame();
    LocalAccess *add_param(bool escape, int size);
    LocalAccess *add_local(bool escape, int size);
    int getCall_size();
    Label *get_label();
    Temp *getReturn_value();
    AccessList *getLocal_data();
    int getFrame_size();
    void setCall_size(int call_size);
};

/// ************************************** Intermediate Representation *************************************************

class IRTree {
  public:
    virtual ~IRTree();
    virtual void accept(PrintIRVisitor *visitor) = 0;
    virtual Stmt *accept(Canonizer *canonizer) = 0;
    virtual Temp *accept(AssemblyGenerator *generator) = 0;
};

/// Statements

class Stmt : public IRTree {
  public:
    virtual ~Stmt();
    virtual void accept(PrintIRVisitor *visitor) = 0;
    virtual Stmt *accept(Canonizer *canonizer) = 0;
    virtual Temp *accept(AssemblyGenerator *generator) = 0;
};

class StmtList : public Stmt {
  private:
    Stmt *stmt;
    StmtList *next;
  public:
    StmtList(Stmt *stmt, StmtList *next);
    virtual ~StmtList();
    Stmt *get_stmt();
    StmtList *get_next();
    void set_stmt(Stmt *stmt);
    void set_next(StmtList *next);
    virtual void accept(PrintIRVisitor *visitor);
    virtual Stmt *accept(Canonizer *canonizer);
};

class MoveNode : public Stmt {
  private:
    Exp *destiny;
    Exp *source;
  public:
    MoveNode(Exp *destiny, Exp *source);
    virtual ~MoveNode();
    Exp *get_destiny();
    Exp *get_source();
    void set_destiny(Exp *destiny);
    void set_source(Exp *source);
    virtual void accept(PrintIRVisitor *visitor);
    virtual Stmt *accept(Canonizer *canonizer);
    virtual Temp *accept(AssemblyGenerator *generator);
};

class ExpressionNode : public Stmt {
  private:
    Exp *exp;
  public:
    ExpressionNode(Exp *exp);
    virtual ~ExpressionNode();
    Exp *get_exp();
    void set_exp(Exp *exp);
    virtual void accept(PrintIRVisitor *visitor);
    virtual Stmt *accept(Canonizer *canonizer);
    virtual Temp *accept(AssemblyGenerator *generator);
};

class JumpNode : public Stmt {
  private:
    Exp *destiny;
    LabelList *label_list;
  public:
    JumpNode(Exp *destiny);
    JumpNode(Exp *destiny, LabelList *label_list);
    virtual ~JumpNode();
    Exp *get_destiny();
    void set_destiny(Exp *destiny);
    void setLabel_list(LabelList *label_list);
    LabelList *getLabel_list();

    virtual void accept(PrintIRVisitor *visitor);
    virtual Stmt *accept(Canonizer *canonizer);
    virtual Temp *accept(AssemblyGenerator *generator);
};

class CJumpNode : public Stmt {
  private:
    int token_op;
    Exp *exp1;
    Exp *exp2;
    Exp *cond_true;
    Exp *cond_false;
    bool canonized;
  public:
    CJumpNode(int op, Exp *exp1, Exp *exp2, Exp *cond_true, Exp *cond_false);
    virtual ~CJumpNode();
    int getToken_op();
    Exp *get_exp1();
    Exp *get_exp2();
    Exp *getCond_true();
    Exp *getCond_false();
    void setToken_op(int token_op);
    void set_exp1(Exp *exp1);
    void set_exp2(Exp *exp2);
    void setCond_true(Exp *cond_true);
    void setCond_false(Exp *cond_false);
    bool is_canonized() const;
    void set_canonized(bool canonized);
    virtual void accept(PrintIRVisitor *visitor);
    virtual Stmt *accept(Canonizer *canonizer);
    virtual Temp *accept(AssemblyGenerator *generator);
};

class SeqNode : public Stmt {
  private:
    Stmt *stmt1;
    Stmt *stmt2;
  public:
    SeqNode(Stmt *stmt1, Stmt *stmt2);
    virtual ~SeqNode();
    Stmt *get_stmt1();
    Stmt *get_stmt2();
    void set_stmt1(Stmt *stmt1);
    void set_stmt2(Stmt *stmt2);
    virtual void accept(PrintIRVisitor *visitor);
    virtual Stmt *accept(Canonizer *canonizer);
    virtual Temp *accept(AssemblyGenerator *generator);
};

class LabelNode : public Stmt {
  private:
    Label *label;
  public:
    LabelNode(Label *label);
    ~LabelNode();
    Label *get_label();
    virtual void accept(PrintIRVisitor *visitor);
    virtual Stmt *accept(Canonizer *canonizer);
    virtual Temp *accept(AssemblyGenerator *generator);
};

/// Expressions

class Exp : public Stmt {
  public:
    virtual ~Exp();
    virtual void accept(PrintIRVisitor *visitor) = 0;
    virtual Exp *accept(Canonizer *canonizer);
    virtual Temp *accept(AssemblyGenerator *generator) = 0;
};

class ExpList : public Exp {
  private:
    Exp *exp;
    ExpList *next;
  public:
    ExpList(Exp *exp, ExpList *next);
    virtual ~ExpList();
    Exp *get_exp();
    ExpList *get_next();
    void set_exp(Exp *exp);
    void set_next(ExpList *next);
    virtual void accept(PrintIRVisitor *visitor);
    virtual Exp *accept(Canonizer *canonizer);
    virtual Temp *accept(AssemblyGenerator *generator);
};

class ConstNode : public Exp {
  private:
    int value;
  public:
    ConstNode(int value);
    virtual ~ConstNode();
    int get_value();
    virtual void accept(PrintIRVisitor *visitor);
    virtual Exp *accept(Canonizer *canonizer);
    virtual Temp *accept(AssemblyGenerator *generator);
};

class ConstfNode : public Exp {
  private:
    float value;
  public:
    ConstfNode(float value);
    virtual ~ConstfNode();
    float get_value();
    virtual void accept(PrintIRVisitor *visitor);
    virtual Exp *accept(Canonizer *canonizer);
    virtual Temp *accept(AssemblyGenerator *generator);
};

class NameNode : public Exp {
  private:
    Label *label;
  public:
    NameNode(Label *label);
    virtual ~NameNode();
    Label *get_label();
    virtual void accept(PrintIRVisitor *visitor);
    virtual Exp *accept(Canonizer *canonizer);
    virtual Temp *accept(AssemblyGenerator *generator);
};

class TempNode : public Exp {
  private:
    Temp *temp;
  public:
    TempNode(Temp *temp);
    virtual ~TempNode();
    Temp *get_temp();
    virtual void accept(PrintIRVisitor *visitor);
    virtual Exp *accept(Canonizer *canonizer);
    virtual Temp *accept(AssemblyGenerator *generator);
};

class BinOpNode : public Exp {
  private:
    int token_op;
    Exp *exp1;
    Exp *exp2;
  public:
    BinOpNode(int op, Exp *exp1, Exp *exp2);
    virtual ~BinOpNode();
    int getToken_op();
    Exp *get_exp1();
    Exp *get_exp2();
    void set_exp1(Exp *exp1);
    void set_exp2(Exp *exp2);
    virtual void accept(PrintIRVisitor *visitor);
    virtual Exp *accept(Canonizer *canonizer);
    virtual Temp *accept(AssemblyGenerator *generator);
};

class MemNode : public Exp {
  private:
    Exp *exp;
  public:
    MemNode(Exp *exp);
    virtual ~MemNode();
    Exp *get_exp();
    void set_exp(Exp *exp);
    virtual void accept(PrintIRVisitor *visitor);
    virtual Exp *accept(Canonizer *canonizer);
    virtual Temp *accept(AssemblyGenerator *generator);
};

class CallNode : public Exp {
  private:
    Exp *function;
    ExpList *arguments;
    bool canonized;
  public:
    CallNode(Exp *exp, ExpList *expression_list);
    virtual ~CallNode();
    Exp *get_function();
    ExpList *get_arguments();
    void set_function(Exp *function);
    void set_arguments(ExpList *arguments);
    bool is_canonized() const;
    void set_canonized(bool canonized);
    virtual void accept(PrintIRVisitor *visitor);
    virtual Exp *accept(Canonizer *canonizer);
    virtual Temp *accept(AssemblyGenerator *generator);
};

class EseqNode : public Exp {
  private:
    Stmt *stmt;
    Exp *exp;
  public:
    EseqNode(Stmt *stmt, Exp *exp);
    virtual ~EseqNode();
    Stmt *get_stmt();
    Exp *get_exp();
    void set_stmt(Stmt *stmt);
    void set_exp(Exp *exp);
    virtual void accept(PrintIRVisitor *visitor);
    virtual Exp *accept(Canonizer *canonizer);
    virtual Temp *accept(AssemblyGenerator *generator);
};

#endif // INTERMEDIATE_CODE_H_INCLUDED
