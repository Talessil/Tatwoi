#ifndef AST_H_INCLUDED
#define AST_H_INCLUDED

#include <cstdlib>
#include "visitor.h"
#include "translator.h"
#include "intermediate_code.h"

/// Base abstract node class.
class Node {
  protected:
    int line;
  public:
    Node(int line);
    virtual ~Node();
    virtual void accept(Visitor *visitor) = 0;
    int get_line();
};

/// Inherits from class Node. Abstract statement node class.
class StatementNode : public Node {
  public:
    StatementNode(int line);
    virtual ~StatementNode();
    virtual void accept(Visitor *visitor) = 0;
    virtual Stmt *accept(Translator *translator) = 0;
};

/// Inherits from class StatementNode. Abstract expression node class.
class ExpNode : public StatementNode {
  private:
    int type;
    const char *type_lex;
    const char *value;
    bool lvalue;
    bool pointer;
    int array_size;
  public:
    ExpNode(int line);
    virtual ~ExpNode();
    virtual void accept(Visitor *visitor) = 0;
    virtual Stmt *accept(Translator *translator) = 0;
    int get_type();
    void set_type(int type);
    const char *get_value();
    void set_value(const char *value);
    bool is_lvalue();
    void set_lvalue(bool lvalue);
    bool is_pointer();
    void set_pointer(bool pointer);
    int get_array_size();
    void set_array_size(int array);
    const char *getType_lex();
    void setType_lex(const char *type_lex);
};

/**
 * Inherits from class Node. Can be an INT, FLOAT, BOOL, ID or CHAR.
 * If it's an ID the lexeme will be different of NULL.
 */
class TypeNode : public Node {
  private:
    int token;
    const char *lexeme;
  public:
    TypeNode(int token, const char *lexeme, int line);
    virtual ~TypeNode();
    void accept(Visitor *visitor);
    IRTree *accept(Translator *translator);
    int get_token() const;
    const char *get_lexeme();
};

/// Inherits from class ExpNode. Represents an identifier.
class IdentifierNode : public ExpNode {
  private:
    const char *lexeme;
    int offset;
  public:
    IdentifierNode(const char *lexeme, int line);;
    virtual ~IdentifierNode();
    void accept(Visitor *visitor);
    virtual Exp *accept(Translator *translator);
    const char *get_lexeme();
    int get_offset() const;
    void set_offset(int offset);
};

/// Inherits from class ExpNode. Represents an integer.
class IntegerNode : public ExpNode {
  private:
    const char *lexeme;
  public:
    IntegerNode(const char *lexeme, int line);
    virtual ~IntegerNode();
    void accept(Visitor *visitor);
    virtual Exp *accept(Translator *translator);
    const char *get_lexeme();
};

/// Inherits from class ExpNode. Represents a real.
class RealNode : public ExpNode {
  private:
    const char *lexeme;
  public:
    RealNode(const char *lexeme, int line);;
    virtual ~RealNode();
    void accept(Visitor *visitor);
    virtual Exp *accept(Translator *translator);
    const char *get_lexeme();
};

/// Inherits from class ExpNode. Represents a literal.
class LiteralNode : public ExpNode {
  private:
    const char *lexeme;
  public:
    LiteralNode(const char *lexeme, int line);
    virtual ~LiteralNode();
    void accept(Visitor *visitor);
    virtual Exp *accept(Translator *translator);
    const char *get_lexeme();
};

/// Inherits from class ExpNode. Represents a character.
class CharacterNode : public ExpNode {
  private:
    const char *lexeme;
  public:
    CharacterNode(const char *lexeme, int line);
    virtual ~CharacterNode();
    void accept(Visitor *visitor);
    virtual Exp *accept(Translator *translator);
    const char *get_lexeme();
};

/// Inherits from class Node. Represents a variable declaration.
class VarDeclNode : public Node {
  private:
    TypeNode *type;
    IdListNode *id_list;
    VarFuncListNode *next;
  public:
    VarDeclNode(TypeNode *type, IdListNode *id_list, VarFuncListNode *next, int line);
    VarDeclNode(TypeNode *type, IdListNode *id_list, VarDeclNode *next, int line);
    VarDeclNode(TypeNode *type, IdListNode *id_list, int line);
    virtual ~VarDeclNode();
    void accept(Visitor *visitor);
    Stmt *accept(Translator *translator);
    TypeNode *get_type();
    IdListNode *get_id_list();
    VarFuncListNode *get_next();
    void set_next(VarFuncListNode *next);
};

/// Inherits from class Node. Represents an identifier in an identifier list.
class IdListNode : public Node {
  private:
    PointerNode *pointer;
    IdentifierNode *id;
    ArrayDeclNode *array;
    IdListNode *next;
  public:
    IdListNode(PointerNode *pointer, IdentifierNode *id, ArrayDeclNode *array, IdListNode *id_list, int line);
    virtual ~IdListNode();
    void accept(Visitor *visitor);
    IRTree *accept(Translator *translator);
    PointerNode *get_pointer();
    IdentifierNode *get_id();
    ArrayDeclNode *get_array();
    IdListNode *get_next();
};

/**
 * Inherits from class Node. Because of it's constructors it will either have a VarDeclNode
 * or a StatementListNode. It can be an item in a chained list.
 */
class VarStmtNode : public Node {
  private:
    VarDeclNode *var_decl = NULL;
    StatementListNode *stmt_list = NULL;
    VarStmtNode *next = NULL;
  public:
    VarStmtNode(VarDeclNode *decl, VarStmtNode *next, int line);
    VarStmtNode(StatementListNode *stmt_list, VarStmtNode *next, int line);
    virtual ~VarStmtNode();
    void accept(Visitor *visitor);
    Stmt *accept(Translator *translator);
    VarDeclNode *get_var_decl();
    StatementListNode *getStmt_list();
    VarStmtNode *get_next();
};

/**
 * Inherits from class Node. Because of it's constructors it will either have a VarDeclNode
 * or a FunctionDeclNode. It can be an item in a chained list.
 */
class VarFuncListNode : public Node {
  private:
    VarDeclNode *var_decl = NULL;
    FunctionDeclNode *func_decl = NULL;
  public:
    VarFuncListNode(VarDeclNode *var_decl, int line);
    VarFuncListNode(FunctionDeclNode *func_decl, int line);
    virtual ~VarFuncListNode();
    void accept(Visitor *visitor);
    IRTree *accept(Translator *translator);
    VarDeclNode *get_var_decl();
    FunctionDeclNode *getFunc_list();
};

class StatementListNode;

/// Inherits from class StatementNode. Represents an If/Else structure.
class IfNode : public StatementNode {
    ExpNode *exp;
    StatementNode *if_stmt;
    StatementNode *else_stmt;
  public:
    IfNode(ExpNode *exp, StatementNode *if_stmt, StatementNode *else_stmt, int line);
    virtual ~IfNode();
    void accept(Visitor *visitor);
    Stmt *accept(Translator *translator);
    ExpNode *get_exp();
    StatementNode *get_if_stmt();
    StatementNode *get_else_stmt();
};

/// Inherits from class StatementNode. Represents a While structure.
class WhileNode : public StatementNode {
  private:
    ExpNode *exp;
    StatementNode *statement;
  public:
    WhileNode(ExpNode *exp, StatementNode *statement, int line);
    virtual ~WhileNode();
    void accept(Visitor *visitor);
    Stmt *accept(Translator *translator);
    ExpNode *get_exp();
    StatementNode *get_statement();
};

/**
 * Inherits from class Node. Represents a case block inside a switch structure.
 * It can be an item in a chained list of case blocks.
 */
class CaseBlockNode : public Node {
  private:
    IntegerNode *num;
    StatementListNode *statement_list;
    CaseBlockNode *next;
  public:
    CaseBlockNode(IntegerNode *num, StatementListNode *statement_list, CaseBlockNode *case_block, int line);
    virtual ~CaseBlockNode();
    void accept(Visitor *visitor);
    IRTree *accept(Translator *translator);
    IntegerNode *get_num();
    StatementListNode *get_statement_list();
    CaseBlockNode *get_next();
};

/// Inherits from class StatementNode. Represents a switch structure.
class SwitchNode : public StatementNode {
  private:
    ExpNode *exp;
    CaseBlockNode *case_block;
  public:
    SwitchNode(ExpNode *exp, CaseBlockNode *case_block, int line);
    virtual ~SwitchNode();
    void accept(Visitor *visitor);
    Stmt *accept(Translator *translator);
    ExpNode *get_exp();
    CaseBlockNode *get_case_block();
};

/// Inherits from class StatementNode. Represents a break statement.
class BreakNode : public StatementNode {
  public:
    BreakNode(int line);
    virtual ~BreakNode();
    void accept(Visitor *visitor);
    Stmt *accept(Translator *translator);
};

/// Inherits from class ExpNode. It is one item in a chained list of expressions.
class ExpListNode : public ExpNode {
  private:
    ExpNode *exp;
    ExpListNode *next;
  public:
    ExpListNode(ExpNode *exp, ExpListNode *exp_list, int line);
    virtual ~ExpListNode();
    void accept(Visitor *visitor);
    virtual Exp *accept(Translator *translator);
    ExpNode *get_exp();
    ExpListNode *get_next();
};

/// Inherits from class StatementNode. Represents a print statement.
class PrintlnNode : public StatementNode {
  private:
    ExpListNode *exp_list;
  public:
    PrintlnNode(ExpListNode *exp_list, int line);
    virtual ~PrintlnNode();
    void accept(Visitor *visitor);
    Stmt *accept(Translator *translator);
    ExpListNode *get_exp_list();
};

/// Inherits from class StatementNode. Represents a read statement.
class ReadNode : public StatementNode {
  private:
    ExpNode *exp;
  public:
    ReadNode(ExpNode *exp, int line);
    virtual ~ReadNode();
    void accept(Visitor *visitor);
    Stmt *accept(Translator *translator);
    ExpNode *get_exp();
};

/// Inherits from class StatementNode. Represents a return statement.
class ReturnNode : public StatementNode {
  private:
    ExpNode *exp;
  public:
    ReturnNode(ExpNode *exp, int line);
    virtual ~ReturnNode();
    void accept(Visitor *visitor);
    Stmt *accept(Translator *translator);
    ExpNode *get_exp();
};

/// Inherits from class StatementNode. Represents a throw statement.
class ThrowNode : public StatementNode {
  public:
    ThrowNode(int line);;
    virtual ~ThrowNode();;
    void accept(Visitor *visitor);
    Stmt *accept(Translator *translator);
};

/// Inherits from class StatementNode. Represents a try-catch structure.
class TryNode : public StatementNode {
  private:
    StatementNode *try_stmt;
    StatementNode *catch_stmt;
  public:
    TryNode(StatementNode *try_stmt, StatementNode *catch_stmt, int line);
    virtual ~TryNode();
    void accept(Visitor *visitor);
    Stmt *accept(Translator *translator);
    StatementNode *get_try_stmt();
    StatementNode *get_catch_stmt();
};

/// Inherits from class StatementNode. It is one item in a chained list of statements.
class StatementListNode : public StatementNode {
  private:
    StatementNode *statement;
    StatementListNode *next;
  public:
    StatementListNode(StatementNode *statement, StatementListNode *statement_list, int line);
    virtual ~StatementListNode();
    virtual void accept(Visitor *visitor);
    virtual Stmt *accept(Translator *translator);
    StatementNode *get_statement();
    StatementListNode *get_next();
};

/// Inherits from class Node. Represents a declaration of a function.
class FunctionDeclNode : public Node {
  private:
    TypeNode *type;
    PointerNode *pointer;
    IdentifierNode *id;
    FormalListNode *formal_list;
    VarStmtNode *var_stmt;
    VarFuncListNode *next;
  public:
    FunctionDeclNode(TypeNode *type, PointerNode *pointer, IdentifierNode *id, FormalListNode *formal_list,
                     VarStmtNode *var_stmt, VarFuncListNode *next, int line);
    virtual ~FunctionDeclNode();
    void accept(Visitor *visitor);
    IRTree *accept(Translator *translator);
    TypeNode *get_type();
    PointerNode *get_pointer();
    IdentifierNode *get_id();
    FormalListNode *get_formal_list();
    VarStmtNode *getVar_stmt();
    VarFuncListNode *get_next();
    void set_next(VarFuncListNode *next);
};

/// Inherits from class Node. Represents a class structure, it can be a item in a chained list.
class ClassListNode : public Node {
  private:
    IdentifierNode *id;
    VarFuncListNode *var_func;
    IdentifierNode *parent_id;
    ClassListNode *next;
  public:
    ClassListNode(IdentifierNode *id, VarFuncListNode *var_func,
                  IdentifierNode *parent_id, ClassListNode *class_list, int line);
    virtual ~ClassListNode();
    void accept(Visitor *visitor);
    IRTree *accept(Translator *translator);
    IdentifierNode *get_id();
    VarFuncListNode *get_var_func();
    IdentifierNode *get_parent_id();
    ClassListNode *get_next();
};

/**
 * Inherits from class Node. Represents a typedef struct structure.
 * It can be an item in a chained list.
 */
class TypeListNode : public Node {
  private:
    VarDeclNode *var_list;
    IdentifierNode *id;
    TypeListNode *next;
  public:
    TypeListNode(VarDeclNode *var_list, IdentifierNode *id, TypeListNode *next, int line);
    virtual ~TypeListNode();
    void accept(Visitor *visitor);
    IRTree *accept(Translator *translator);
    VarDeclNode *get_var_list();
    IdentifierNode *get_id();
    TypeListNode *get_next();
};

/**
 * Inherits from class Node. Represents the main program being analyzed.
 * This will be the root of the AST returned by the parser.
 */
class ProgramNode : public Node {
  private:
    ClassListNode *class_list;
    TypeListNode *type_list;
    VarFuncListNode *var_func_list;
  public:
    ProgramNode(ClassListNode *class_list,
                TypeListNode *type_list, VarFuncListNode *var_func_list, int line);
    virtual ~ProgramNode();
    void accept(Visitor *visitor);
    IRTree *accept(Translator *translator);
    ClassListNode *get_class_list();
    TypeListNode *get_type_list();
    VarFuncListNode *get_var_func_list();
};

/// Inherits from class Node. Represents a * before a identifier to indicate that it's a pointer.
class PointerNode : public Node {
  public:
    PointerNode(int line);
    virtual ~PointerNode();;
    void accept(Visitor *visitor);
    IRTree *accept(Translator *translator);
};

/// Inherits from class ExpNode. Represents an assignment expression.
class AssignNode : public ExpNode {
  private:
    ExpNode *left_exp;
    ExpNode *right_exp;
  public:
    AssignNode(ExpNode *left_exp, ExpNode *right_exp, int line);
    virtual ~AssignNode();
    void accept(Visitor *visitor);
    Stmt *accept(Translator *translator);
    ExpNode *get_left_exp();
    ExpNode *get_right_exp();
};

/**
 * Inherits from class ExpNode. Represents a node from the Primary production.
 * It can be an item in a chained list.
 */
class PrimaryNode : public ExpNode {
  private:
    const char *owner_lex;
    ExpNode *exp;
    ExpNode *next;
  public:
    PrimaryNode(ExpNode *exp, ExpNode *next, int line);
    virtual ~PrimaryNode();
    virtual void accept(Visitor *visitor);
    virtual Exp *accept(Translator *translator);
    ExpNode *get_exp();
    ExpNode *get_next();
    const char *getOwner_lex();
    void setOwner_lex(const char *owner_lex);
};

/**
 * Inherits from class ExpNode. Represents the calling of a function.
 * It can be a item in a chained list of expressions.
 */
class FunctionCallNode : public ExpNode {
  private:
    IdentifierNode *id;
    ExpNode *exp_list;
    ExpNode *next;
    const char *owner_lex;
  public:
    FunctionCallNode(IdentifierNode *id, ExpListNode *exp_list, ExpNode *next, int line);
    virtual ~FunctionCallNode();
    void accept(Visitor *visitor);
    virtual Exp *accept(Translator *translator);
    IdentifierNode *get_id();
    ExpNode *get_exp_list();
    ExpNode *get_next();
    const char *getOwner_lex();
    void setOwner_lex(const char *owner_lex);
};

/**
 * Inherits from class ExpNode. Represents an access to a field with '.'.
 * It can be an item in a chained list.
 */
class FieldAccessNode : public ExpNode {
  private:
    const char *owner_lex;
    ExpNode *exp;
  public:
    FieldAccessNode(ExpNode *exp, int line);
    virtual ~FieldAccessNode();
    void accept(Visitor *visitor);
    virtual Exp *accept(Translator *translator);
    ExpNode *get_exp();
    const char *getOwner_lex();
    void setOwner_lex(const char *owner_lex);
};

/**
 * Inherits from class ExpNode. Represents an access to a field with '->'.
 * It can be an item in a chained list.
 */
class PointerAccessNode : public ExpNode {
  private:
    const char *owner_lex;
    ExpNode *exp;
  public:
    PointerAccessNode(ExpNode *exp, int line);
    virtual ~PointerAccessNode();
    void accept(Visitor *visitor);
    virtual Exp *accept(Translator *translator);
    ExpNode *get_exp();
    const char *getOwner_lex();
    void setOwner_lex(const char *owner_lex);
};

/**
 * Inherits from class ExpNode.
 * Represents a & before a identifier to indicate that it is accessing the address.
 */
class AddressValueNode : public ExpNode {
  private:
    ExpNode *exp;
  public:
    AddressValueNode(ExpNode *exp, int line);
    virtual ~AddressValueNode();
    void accept(Visitor *visitor);
    virtual Exp *accept(Translator *translator);
    ExpNode *get_exp() const;
};

/// Inherits from class ExpNode. Represents an access to a pointer value.
class PointerValueNode : public ExpNode {
  private:
    ExpNode *exp;
  public:
    PointerValueNode(ExpNode *exp, int line);
    virtual ~PointerValueNode();
    void accept(Visitor *visitor);
    virtual Exp *accept(Translator *translator);
    ExpNode *get_exp() const;
};

/// Inherits from class ExpNode. Represents a access to a array.
class ArrayAccessNode : public ExpNode {
  private:
    ExpNode *index_exp;
    ExpNode *next;
  public:
    ArrayAccessNode(ExpNode *index_exp, ExpNode *next, int line);
    virtual ~ArrayAccessNode();
    void accept(Visitor *visitor);
    virtual Exp *accept(Translator *translator);
    ExpNode *get_index_exp();
    ExpNode *get_next();
};

/// Inherits from class ExpNode. Represents a array declaration.
class ArrayDeclNode : public ExpNode {
  private:
    ExpNode *index_exp;
  public:
    ArrayDeclNode(ExpNode *index_exp, int line);
    virtual ~ArrayDeclNode();
    void accept(Visitor *visitor);
    virtual Exp *accept(Translator *translator);
    ExpNode *get_index_exp();
};

/// Inherits from class ExpNode. Represents the creation of an instance with the new keyword.
class NewNode : public ExpNode {
  private:
    IdentifierNode *id;
    ExpListNode *exp_list;
  public:
    NewNode(IdentifierNode *id, ExpListNode *exp_list, int line);
    virtual ~NewNode();
    void accept(Visitor *visitor);
    virtual Exp *accept(Translator *translator);
    IdentifierNode *get_id();
    ExpListNode *get_exp_list();
};

/// Inherits from class ExpNode. Represents a relational operation (==, !=, <=, >=, <, >).
class RelationalOpNode : public ExpNode {
  private:
    int token_op;
    ExpNode *left_exp;
    ExpNode *right_exp;
  public:
    RelationalOpNode(int token_op, ExpNode *left_exp, ExpNode *right_exp, int line);
    virtual ~RelationalOpNode();
    void accept(Visitor *visitor);
    virtual Exp *accept(Translator *translator);
    int get_op();
    ExpNode *get_left_exp();
    ExpNode *get_right_exp();
};

/// Inherits from class ExpNode. Represents an addition operation (+, -).
class AdditionOpNode : public ExpNode {
  private:
    int token_op;
    ExpNode *left_exp;
    ExpNode *right_exp;
  public:
    AdditionOpNode(int token_op, ExpNode *left_exp, ExpNode *right_exp, int line);
    virtual ~AdditionOpNode();
    void accept(Visitor *visitor);
    virtual Exp *accept(Translator *translator);
    int get_op();
    ExpNode *get_left_exp();
    ExpNode *get_right_exp();
};

/// Inherits from class ExpNode. Represents a multiplication or division operation (*, /, %).
class MultiplicationOpNode : public ExpNode {
  private:
    int token_op;
    ExpNode *left_exp;
    ExpNode *right_exp;
  public:
    MultiplicationOpNode(int token_op, ExpNode *left_exp, ExpNode *right_exp, int line);
    virtual ~MultiplicationOpNode();
    void accept(Visitor *visitor);
    virtual Exp *accept(Translator *translator);
    int get_op();
    ExpNode *get_left_exp();
    ExpNode *get_right_exp();
};

/// Inherits from class ExpNode. Represents a boolean operation (&&, ||).
class BooleanOpNode : public ExpNode {
  private:
    int token_op;
    ExpNode *left_exp;
    ExpNode *right_exp;
  public:
    BooleanOpNode(int token_op, ExpNode *left_exp, ExpNode *right_exp, int line);
    virtual ~BooleanOpNode();
    void accept(Visitor *visitor);
    virtual Exp *accept(Translator *translator);
    int get_op();
    ExpNode *get_left_exp();
    ExpNode *get_right_exp();
};

/// Inherits from class ExpNode. Represents a bitwise operation (&, |).
class BitwiseOpNode : public ExpNode {
  private:
    int token_op;
    ExpNode *left_exp;
    ExpNode *right_exp;
  public:
    BitwiseOpNode(int token_op, ExpNode *left_exp, ExpNode *right_exp, int line);
    virtual ~BitwiseOpNode();
    void accept(Visitor *visitor);
    virtual Exp *accept(Translator *translator);
    int get_op();
    ExpNode *get_left_exp();
    ExpNode *get_right_exp();
};

/// Inherits from class ExpNode. Represents a true expression.
class TrueNode : public ExpNode {
  public:
    TrueNode(int line);;
    virtual ~TrueNode();;
    void accept(Visitor *visitor);
    virtual Exp *accept(Translator *translator);
};

/// Inherits from class ExpNode. Represents a false expression.
class FalseNode : public ExpNode {
  public:
    FalseNode(int line);;
    virtual ~FalseNode();;
    void accept(Visitor *visitor);
    virtual Exp *accept(Translator *translator);
};

/// Inherits from class ExpNode. Represents a this expression.
class ThisNode : public ExpNode {
  public:
    ThisNode(int line);;
    virtual ~ThisNode();;
    void accept(Visitor *visitor);
    virtual Exp *accept(Translator *translator);
};

/// Inherits from class ExpNode. Represents a not expression.
class NotNode : public ExpNode {
  private:
    ExpNode *exp;
  public:
    NotNode(ExpNode *exp, int line);
    virtual ~NotNode();
    void accept(Visitor *visitor);
    virtual Exp *accept(Translator *translator);
    ExpNode *get_exp();
};

/// Inherits from class ExpNode. Represents a sign expression.
class SignNode : public ExpNode {
  private:
    ExpNode *exp;
  public:
    SignNode(ExpNode *exp, int line);
    virtual ~SignNode();
    void accept(Visitor *visitor);
    virtual Exp *accept(Translator *translator);
    ExpNode *get_exp();
};

/**
 * Inherits from class Node. Represents a parameter in a function.
 * It can be a item in a chained list.
 */
class FormalListNode : public Node {
  private:
    TypeNode *type;
    PointerNode *pointer;
    IdentifierNode *id;
    ArrayDeclNode *array;
    FormalListNode *next;
  public:
    FormalListNode(TypeNode *type, PointerNode *pointer, IdentifierNode *id, ArrayDeclNode *array,
                   FormalListNode *next, int line);
    virtual ~FormalListNode();
    void accept(Visitor *visitor);
    IRTree *accept(Translator *translator);
    TypeNode *get_type();
    PointerNode *get_pointer();
    IdentifierNode *get_id();
    ArrayDeclNode *get_array();
    FormalListNode *get_next();
    bool is_equal(FormalListNode *formal);
};

#endif // AST_H_INCLUDED
