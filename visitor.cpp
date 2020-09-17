#include <iostream>
#include <cstring>
#include "visitor.h"
#include "ast.h"
#include "lex_analyzer.h"

PrintASTVisitor::PrintASTVisitor() {
  this->indentation = 0;
}

/**
 * Increases the indentation used for printing by 2.
 */
void PrintASTVisitor::increase_indent() { this->indentation += 2; }


/**
 * Decreases the indentation used for printing by 2.
 */
void PrintASTVisitor::decrease_indent() { this->indentation -= 2; }

/**
 * Prints white spaces used for indentation.
 */
void PrintASTVisitor::indent() {
  for (unsigned int i = 0; i < this->indentation; i++) std::cout << " ";
}

/**
 * Prints a string with the correct indentation.
 */
void PrintASTVisitor::print(const char *s) {
  indent();
  std::cout << "-" << s << std::endl;
}

void PrintASTVisitor::visit(ProgramNode *node) {
  std::cout << "\n------------------------------" << std::endl;
  std::cout << "---- ABSTRACT SYNTAX TREE ----" << std::endl;
  std::cout << "------------------------------\n" << std::endl;
  print("PROGRAM");
  increase_indent();
  if (node->get_class_list() != NULL) node->get_class_list()->accept(this);
  if (node->get_type_list() != NULL) node->get_type_list()->accept(this);
  if (node->get_var_func_list() != NULL) node->get_var_func_list()->accept(this);
  decrease_indent();
}

void PrintASTVisitor::visit(ClassListNode *node) {
  print("CLASSLIST");
  increase_indent();
  if (node->get_id() != NULL) node->get_id()->accept(this);
  if (node->get_var_func() != NULL) node->get_var_func()->accept(this);
  if (node->get_parent_id() != NULL) node->get_parent_id()->accept(this);
  if (node->get_next() != NULL) node->get_next()->accept(this);
  decrease_indent();
}

void PrintASTVisitor::visit(VarDeclNode *node) {
  print("VARDECL");
  increase_indent();
  if (node->get_type() != NULL) node->get_type()->accept(this);
  if (node->get_id_list() != NULL) node->get_id_list()->accept(this);
  if (node->get_next() != NULL) node->get_next()->accept(this);
  decrease_indent();
}

void PrintASTVisitor::visit(IdListNode *node) {
  print("IDLIST");
  increase_indent();
  if (node->get_pointer() != NULL) node->get_pointer()->accept(this);
  if (node->get_id() != NULL) node->get_id()->accept(this);
  if (node->get_array() != NULL) node->get_array()->accept(this);
  if (node->get_next() != NULL) node->get_next()->accept(this);
  decrease_indent();
}

void PrintASTVisitor::visit(FunctionDeclNode *node) {
  print("FUNCTIONDECL");
  increase_indent();
  if (node->get_type() != NULL) node->get_type()->accept(this);
  if (node->get_pointer() != NULL) node->get_pointer()->accept(this);
  if (node->get_id() != NULL) node->get_id()->accept(this);
  if (node->get_formal_list() != NULL) node->get_formal_list()->accept(this);
  if (node->getVar_stmt() != NULL) node->getVar_stmt()->accept(this);
  if (node->get_next() != NULL) node->get_next()->accept(this);
  decrease_indent();
}

void PrintASTVisitor::visit(TypeListNode *node) {
  print("TYPELIST");
  increase_indent();
  if (node->get_var_list() != NULL) node->get_var_list()->accept(this);
  if (node->get_id() != NULL) node->get_id()->accept(this);
  if (node->get_next() != NULL) node->get_next()->accept(this);
  decrease_indent();
}

void PrintASTVisitor::visit(PointerNode *node) {
  print("POINTER");
}

void PrintASTVisitor::visit(StatementListNode *node) {
  print("STATEMENTLIST");
  increase_indent();
  if (node->get_statement() != NULL) node->get_statement()->accept(this);
  if (node->get_next() != NULL) node->get_next()->accept(this);
  decrease_indent();
}

void PrintASTVisitor::visit(VarStmtNode *node) {
  print("VARSTMT");
  increase_indent();
  if (node->get_var_decl() != NULL) node->get_var_decl()->accept(this);
  if (node->getStmt_list() != NULL) node->getStmt_list()->accept(this);
  if (node->get_next() != NULL) node->get_next()->accept(this);
  decrease_indent();
}

void PrintASTVisitor::visit(VarFuncListNode *node) {
  print("VARFUNCLIST");
  increase_indent();
  if (node->get_var_decl() != NULL) node->get_var_decl()->accept(this);
  if (node->getFunc_list() != NULL) node->getFunc_list()->accept(this);
  decrease_indent();
}

void PrintASTVisitor::visit(IfNode *node) {
  print("IF");
  increase_indent();
  if (node->get_exp() != NULL) node->get_exp()->accept(this);
  if (node->get_if_stmt() != NULL) node->get_if_stmt()->accept(this);
  if (node->get_else_stmt() != NULL) node->get_else_stmt()->accept(this);
  decrease_indent();
}

void PrintASTVisitor::visit(WhileNode *node) {
  print("WHILE");
  increase_indent();
  if (node->get_exp() != NULL) node->get_exp()->accept(this);
  if (node->get_statement() != NULL) node->get_statement()->accept(this);
  decrease_indent();
}

void PrintASTVisitor::visit(SwitchNode *node) {
  print("SWITCH");
  increase_indent();
  if (node->get_exp() != NULL) node->get_exp()->accept(this);
  if (node->get_case_block() != NULL) node->get_case_block()->accept(this);
  decrease_indent();
}

void PrintASTVisitor::visit(BreakNode *node) { print("BREAK"); }

void PrintASTVisitor::visit(PrintlnNode *node) {
  print("PRINTLN");
  increase_indent();
  if (node->get_exp_list() != NULL) node->get_exp_list()->accept(this);
  decrease_indent();
}

void PrintASTVisitor::visit(ReadNode *node) {
  print("READ");
  increase_indent();
  if (node->get_exp() != NULL) node->get_exp()->accept(this);
  decrease_indent();
}

void PrintASTVisitor::visit(ReturnNode *node) {
  print("RETURN");
  increase_indent();
  if (node->get_exp() != NULL) node->get_exp()->accept(this);
  decrease_indent();
}

void PrintASTVisitor::visit(CaseBlockNode *node) {
  print("CASEBLOCK");
  increase_indent();
  if (node->get_num() != NULL) node->get_num()->accept(this);
  if (node->get_statement_list() != NULL) node->get_statement_list()->accept(this);
  if (node->get_next() != NULL) node->get_next()->accept(this);
  decrease_indent();
}

void PrintASTVisitor::visit(ThrowNode *node) { print("THROW"); }

void PrintASTVisitor::visit(ExpListNode *node) {
  print("EXPLIST");
  increase_indent();
  if (node->get_exp() != NULL) node->get_exp()->accept(this);
  if (node->get_next() != NULL) node->get_next()->accept(this);
  decrease_indent();
}

void PrintASTVisitor::visit(TryNode *node) {
  print("TRY");
  increase_indent();
  if (node->get_try_stmt() != NULL) node->get_try_stmt()->accept(this);
  if (node->get_catch_stmt() != NULL) node->get_catch_stmt()->accept(this);
  decrease_indent();
}

void PrintASTVisitor::visit(TypeNode *node) {
  if (node->get_lexeme() != NULL) {
    indent();
    std::cout << "-" << get_token_name(node->get_token()) << "." << node->get_lexeme() << std::endl;
  } else print(get_token_name(node->get_token()));
}

void PrintASTVisitor::visit(IdentifierNode *node) {
  indent();
  std::cout << "-ID." << node->get_lexeme() << std::endl;
}

void PrintASTVisitor::visit(IntegerNode *node) {
  indent();
  std::cout << "-INTEGER." << node->get_lexeme() << std::endl;
}

void PrintASTVisitor::visit(RealNode *node) {
  indent();
  std::cout << "-REAL." << node->get_lexeme() << std::endl;
}

void PrintASTVisitor::visit(LiteralNode *node) {
  indent();
  std::cout << "-LITERAL." << node->get_lexeme() << std::endl;
}

void PrintASTVisitor::visit(CharacterNode *node) {
  indent();
  std::cout << "-CHARACTER." << node->get_lexeme() << std::endl;
}

void PrintASTVisitor::visit(PrimaryNode *node) {
  print("PRIMARY");
  increase_indent();
  if (node->get_exp() != NULL) node->get_exp()->accept(this);
  if (node->get_next() != NULL) node->get_next()->accept(this);
  decrease_indent();
}

void PrintASTVisitor::visit(FunctionCallNode *node) {
  print("FUNCTIONCALL");
  increase_indent();
  if (node->get_exp_list() != NULL) node->get_exp_list()->accept(this);
  if (node->get_next() != NULL) node->get_next()->accept(this);
  decrease_indent();
}

void PrintASTVisitor::visit(FieldAccessNode *node) {
  print("FIELDACCESS");
  increase_indent();
  if (node->get_exp() != NULL) node->get_exp()->accept(this);
  decrease_indent();
}

void PrintASTVisitor::visit(PointerAccessNode *node) {
  print("POINTERACCESS");
  increase_indent();
  if (node->get_exp() != NULL) node->get_exp()->accept(this);
  decrease_indent();
}

void PrintASTVisitor::visit(AddressValueNode *node) {
  print("ADDRESSVALUE");
  increase_indent();
  if (node->get_exp() != NULL) node->get_exp()->accept(this);
  decrease_indent();
}

void PrintASTVisitor::visit(PointerValueNode *node) {
  print("POINTERVALUE");
  increase_indent();
  if (node->get_exp() != NULL) node->get_exp()->accept(this);
  decrease_indent();
}

void PrintASTVisitor::visit(ArrayAccessNode *node) {
  print("ARRAYACCESS");
  increase_indent();
  if (node->get_index_exp() != NULL) node->get_index_exp()->accept(this);
  if (node->get_next() != NULL) node->get_next()->accept(this);
  decrease_indent();
}

void PrintASTVisitor::visit(ArrayDeclNode *node) {
  print("ARRAYDECL");
  increase_indent();
  if (node->get_index_exp() != NULL) node->get_index_exp()->accept(this);
  decrease_indent();
}

void PrintASTVisitor::visit(AssignNode *node) {
  print("ASSIGN");
  increase_indent();
  if (node->get_left_exp() != NULL) node->get_left_exp()->accept(this);
  if (node->get_right_exp() != NULL) node->get_right_exp()->accept(this);
  decrease_indent();
}

void PrintASTVisitor::visit(RelationalOpNode *node) {
  print(get_token_name(node->get_op()));
  increase_indent();
  if (node->get_left_exp() != NULL) node->get_left_exp()->accept(this);
  if (node->get_right_exp() != NULL) node->get_right_exp()->accept(this);
  decrease_indent();
}

void PrintASTVisitor::visit(AdditionOpNode *node) {
  print(get_token_name(node->get_op()));
  increase_indent();
  if (node->get_left_exp() != NULL) node->get_left_exp()->accept(this);
  if (node->get_right_exp() != NULL) node->get_right_exp()->accept(this);
  decrease_indent();
}

void PrintASTVisitor::visit(MultiplicationOpNode *node) {
  print(get_token_name(node->get_op()));
  increase_indent();
  if (node->get_left_exp() != NULL) node->get_left_exp()->accept(this);
  if (node->get_right_exp() != NULL) node->get_right_exp()->accept(this);
  decrease_indent();
}

void PrintASTVisitor::visit(BooleanOpNode *node) {
  print(get_token_name(node->get_op()));
  increase_indent();
  if (node->get_left_exp() != NULL) node->get_left_exp()->accept(this);
  if (node->get_right_exp() != NULL) node->get_right_exp()->accept(this);
  decrease_indent();
}

void PrintASTVisitor::visit(BitwiseOpNode *node) {
  print(get_token_name(node->get_op()));
  increase_indent();
  if (node->get_left_exp() != NULL) node->get_left_exp()->accept(this);
  if (node->get_right_exp() != NULL) node->get_right_exp()->accept(this);
  decrease_indent();
}

void PrintASTVisitor::visit(NewNode *node) {
  print("NEW");
  increase_indent();
  if (node->get_id() != NULL) node->get_id()->accept(this);
  if (node->get_exp_list() != NULL) node->get_exp_list()->accept(this);
  decrease_indent();
}

void PrintASTVisitor::visit(NotNode *node) {
  print("NOT");
  increase_indent();
  if (node->get_exp() != NULL) node->get_exp()->accept(this);
  decrease_indent();
}

void PrintASTVisitor::visit(SignNode *node) {
  print("SIGN");
  increase_indent();
  if (node->get_exp() != NULL) node->get_exp()->accept(this);
  decrease_indent();
}

void PrintASTVisitor::visit(TrueNode *node) { print("TRUE"); }

void PrintASTVisitor::visit(FalseNode *node) { print("FALSE"); }

void PrintASTVisitor::visit(ThisNode *node) { print("THIS"); }

void PrintASTVisitor::visit(FormalListNode *node) {
  print("FORMALLIST");
  increase_indent();
  if (node->get_type() != NULL) node->get_type()->accept(this);
  if (node->get_pointer() != NULL) node->get_pointer()->accept(this);
  if (node->get_id() != NULL) node->get_id()->accept(this);
  if (node->get_array() != NULL) node->get_array()->accept(this);
  if (node->get_next() != NULL) node->get_next()->accept(this);
  decrease_indent();
}
