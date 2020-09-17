#ifndef SEMANTIC_ANALYZER_H_INCLUDED
#define SEMANTIC_ANALYZER_H_INCLUDED

#include "visitor.h"
#include "table.h"

#define FUNCTION 998
#define VARIABLE 999

void init_semantic(ProgramNode *program);
void end_semantic();
Table *get_table(int table);

class FillTableVisitor : public Visitor {
  public:
    FillTableVisitor(ProgramNode *program);

    virtual ~FillTableVisitor() {}

    virtual void visit(ProgramNode *node);
    virtual void visit(ClassListNode *node);
    virtual void visit(VarDeclNode *node);
    virtual void visit(IdListNode *node);
    virtual void visit(TypeNode *node);
    virtual void visit(FunctionDeclNode *node);
    virtual void visit(TypeListNode *node);
    virtual void visit(PointerNode *node);
    virtual void visit(StatementListNode *node);
    virtual void visit(VarStmtNode *node);
    virtual void visit(VarFuncListNode *node);
    virtual void visit(IfNode *node);
    virtual void visit(WhileNode *node);
    virtual void visit(SwitchNode *node);
    virtual void visit(BreakNode *node);
    virtual void visit(PrintlnNode *node);
    virtual void visit(ReadNode *node);
    virtual void visit(ReturnNode *node);
    virtual void visit(CaseBlockNode *node);
    virtual void visit(ThrowNode *node);
    virtual void visit(ExpListNode *node);
    virtual void visit(TryNode *node);
    virtual void visit(IdentifierNode *node);
    virtual void visit(IntegerNode *node);
    virtual void visit(RealNode *node);
    virtual void visit(LiteralNode *node);
    virtual void visit(CharacterNode *node);
    virtual void visit(PrimaryNode *node);
    virtual void visit(FunctionCallNode *node);
    virtual void visit(FieldAccessNode *node);
    virtual void visit(PointerAccessNode *node);
    virtual void visit(AddressValueNode *node);
    virtual void visit(PointerValueNode *node);
    virtual void visit(ArrayAccessNode *node);
    virtual void visit(ArrayDeclNode *node);
    virtual void visit(AssignNode *node);
    virtual void visit(RelationalOpNode *node);
    virtual void visit(AdditionOpNode *node);
    virtual void visit(MultiplicationOpNode *node);
    virtual void visit(BooleanOpNode *node);
    virtual void visit(BitwiseOpNode *node);
    virtual void visit(NewNode *node);
    virtual void visit(NotNode *node);
    virtual void visit(SignNode *node);
    virtual void visit(TrueNode *node);
    virtual void visit(FalseNode *node);
    virtual void visit(ThisNode *node);
    virtual void visit(FormalListNode *node);
};

class SemanticAnalyzerVisitor : public Visitor {
  public:
    SemanticAnalyzerVisitor(ProgramNode *program);

    virtual ~SemanticAnalyzerVisitor() {}

    virtual void visit(ProgramNode *node);
    virtual void visit(ClassListNode *node);
    virtual void visit(VarDeclNode *node);
    virtual void visit(IdListNode *node);
    virtual void visit(TypeNode *node);
    virtual void visit(FunctionDeclNode *node);
    virtual void visit(TypeListNode *node);
    virtual void visit(PointerNode *node);
    virtual void visit(StatementListNode *node);
    virtual void visit(VarStmtNode *node);
    virtual void visit(VarFuncListNode *node);
    virtual void visit(IfNode *node);
    virtual void visit(WhileNode *node);
    virtual void visit(SwitchNode *node);
    virtual void visit(BreakNode *node);
    virtual void visit(PrintlnNode *node);
    virtual void visit(ReadNode *node);
    virtual void visit(ReturnNode *node);
    virtual void visit(CaseBlockNode *node);
    virtual void visit(ThrowNode *node);
    virtual void visit(ExpListNode *node);
    virtual void visit(TryNode *node);
    virtual void visit(IdentifierNode *node);
    virtual void visit(IntegerNode *node);
    virtual void visit(RealNode *node);
    virtual void visit(LiteralNode *node);
    virtual void visit(CharacterNode *node);
    virtual void visit(PrimaryNode *node);
    virtual void visit(FunctionCallNode *node);
    virtual void visit(FieldAccessNode *node);
    virtual void visit(PointerAccessNode *node);
    virtual void visit(AddressValueNode *node);
    virtual void visit(PointerValueNode *node);
    virtual void visit(ArrayAccessNode *node);
    virtual void visit(ArrayDeclNode *node);
    virtual void visit(AssignNode *node);
    virtual void visit(RelationalOpNode *node);
    virtual void visit(AdditionOpNode *node);
    virtual void visit(MultiplicationOpNode *node);
    virtual void visit(BooleanOpNode *node);
    virtual void visit(BitwiseOpNode *node);
    virtual void visit(NewNode *node);
    virtual void visit(NotNode *node);
    virtual void visit(SignNode *node);
    virtual void visit(TrueNode *node);
    virtual void visit(FalseNode *node);
    virtual void visit(ThisNode *node);
    virtual void visit(FormalListNode *node);
};

#endif //SEMANTIC_ANALYZER_H_INCLUDED
