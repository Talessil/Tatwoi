#ifndef TRANSLATOR_H_INCLUDED
#define TRANSLATOR_H_INCLUDED

#define WORD_SIZE 4
#define ESCAPE true

class ProgramNode;
class ClassListNode;
class VarDeclNode;
class IdListNode;
class TypeNode;
class FunctionDeclNode;
class TypeListNode;
class PointerNode;
class StatementNode;
class StatementListNode;
class VarStmtNode;
class VarFuncListNode;
class IfNode;
class WhileNode;
class SwitchNode;
class BreakNode;
class PrintlnNode;
class ReadNode;
class ReturnNode;
class CaseBlockNode;
class ThrowNode;
class ExpNode;
class ExpListNode;
class TryNode;
class IdentifierNode;
class IntegerNode;
class RealNode;
class LiteralNode;
class CharacterNode;
class PrimaryNode;
class FunctionCallNode;
class FieldAccessNode;
class PointerAccessNode;
class AddressValueNode;
class PointerValueNode;
class ArrayAccessNode;
class ArrayDeclNode;
class AssignNode;
class RelationalOpNode;
class AdditionOpNode;
class MultiplicationOpNode;
class BooleanOpNode;
class BitwiseOpNode;
class NewNode;
class NotNode;
class SignNode;
class TrueNode;
class FalseNode;
class ThisNode;
class FormalListNode;

class Label;
class Fragment;
class MIPSFrame;
class Exp;
class Stmt;

typedef struct LabelStack {
  Label *label;
  LabelStack *next;
} label_stack;

class Translator {
  private:
    Fragment *fragment_list;
    MIPSFrame *current_frame;
    LabelStack *loop_labels;
    LabelStack *func_labels;

    void add_fragment(Fragment *fragment);

    void push_loop_label(Label *label);
    Label *peek_loop_label();
    void pop_loop_label();

    void push_func_label(Label *label);
    Label *peek_func_label();
    void pop_func_label();
  public:
    Translator();
    virtual ~Translator();

    Fragment *getFragment_list() { return fragment_list; }

    void setFragment_list(Fragment *fragment_list);
    void print_ir();

    virtual void visit(ProgramNode *node);
    virtual void visit(ClassListNode *node);
    virtual void visit(VarDeclNode *node);
    virtual void visit(IdListNode *node);
    virtual void visit(FunctionDeclNode *node);
    virtual void visit(FormalListNode *node);
    virtual Stmt *visit(StatementListNode *node);
    virtual Stmt *visit(VarStmtNode *node);
    virtual Stmt *visit(VarFuncListNode *node);
    virtual Stmt *visit(IfNode *node);
    virtual Stmt *visit(WhileNode *node);
    virtual Stmt *visit(SwitchNode *node);
    virtual Stmt *visit(BreakNode *node);
    virtual Stmt *visit(PrintlnNode *node);
    virtual Stmt *visit(ReadNode *node);
    virtual Stmt *visit(ReturnNode *node);
    virtual Stmt *visit(CaseBlockNode *node);
    virtual Stmt *visit(ThrowNode *node);
    virtual Stmt *visit(TryNode *node);
    virtual Stmt *visit(AssignNode *node);
    virtual Exp *visit(TypeListNode *node);
    virtual Exp *visit(TypeNode *node);
    virtual Exp *visit(PointerNode *node);
    virtual Exp *visit(ExpListNode *node);
    virtual Exp *visit(IdentifierNode *node);
    virtual Exp *visit(IntegerNode *node);
    virtual Exp *visit(RealNode *node);
    virtual Exp *visit(LiteralNode *node);
    virtual Exp *visit(CharacterNode *node);
    virtual Exp *visit(PrimaryNode *node);
    virtual Exp *visit(FunctionCallNode *node);
    virtual Exp *visit(FieldAccessNode *node);
    virtual Exp *visit(PointerAccessNode *node);
    virtual Exp *visit(AddressValueNode *node);
    virtual Exp *visit(PointerValueNode *node);
    virtual Exp *visit(ArrayAccessNode *node);
    virtual Exp *visit(ArrayDeclNode *node);
    virtual Exp *visit(RelationalOpNode *node);
    virtual Exp *visit(AdditionOpNode *node);
    virtual Exp *visit(MultiplicationOpNode *node);
    virtual Exp *visit(BooleanOpNode *node);
    virtual Exp *visit(BitwiseOpNode *node);
    virtual Exp *visit(NewNode *node);
    virtual Exp *visit(NotNode *node);
    virtual Exp *visit(SignNode *node);
    virtual Exp *visit(TrueNode *node);
    virtual Exp *visit(FalseNode *node);
    virtual Exp *visit(ThisNode *node);
};

Translator *init_translator(ProgramNode *program);
void end_translator();

#endif // TRANSLATOR_H_INCLUDED
