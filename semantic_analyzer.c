#include "semantic_analyzer.h"
#include "table.h"
#include "lex_analyzer.h"
#include "error_manager.h"
#include <typeinfo>

#define BOOL_TRUE 1
#define BOOL_FALSE 0
#define BYTE_SIZE 4

FillTableVisitor *fill_table_visitor;
SemanticAnalyzerVisitor *semantic_visitor;
VarTable *var_table;
FunctionTable *function_table;
StructTable *struct_table;
ClassTable *class_table;
int break_enabled = BOOL_FALSE;
const char *active_class = NULL;
FunctionSymbol *active_function = NULL;

void init_semantic(ProgramNode *program) {
  var_table = new VarTable();
  function_table = new FunctionTable();
  struct_table = new StructTable();
  class_table = new ClassTable();
  fill_table_visitor = new FillTableVisitor(program);
  semantic_visitor = new SemanticAnalyzerVisitor(program);
  var_table->print_table();
  function_table->print_table();
  struct_table->print_table();
  class_table->print_table();
}

void end_semantic() {
  delete fill_table_visitor;
  delete semantic_visitor;
  delete var_table;
  delete function_table;
  delete struct_table;
  delete class_table;
  active_function = NULL;
}

void begin_scope(const char *scope_lex) {
  var_table->begin_scope(scope_lex);
  function_table->begin_scope(scope_lex);
  struct_table->begin_scope(scope_lex);
  class_table->begin_scope(scope_lex);
}

void end_scope() {
  var_table->end_scope();
  function_table->end_scope();
  struct_table->end_scope();
  class_table->end_scope();
}

Table *get_table(int table) {
  switch (table) {
    case STRUCT:
      return struct_table;
    case CLASS:
      return class_table;
    case FUNCTION:
      return function_table;
    case VARIABLE:
      return var_table;
    default:
      return NULL;
  }
}

//////////////////////////////// FILLTABLE VISITOR /////////////////////////////////////////////////

FillTableVisitor::FillTableVisitor(ProgramNode *program) {
  visit(program);
}

void FillTableVisitor::visit(ProgramNode *node) {
  if (node->get_class_list() != NULL) node->get_class_list()->accept(this);
  if (node->get_type_list() != NULL) node->get_type_list()->accept(this);
  if (node->get_var_func_list() != NULL) node->get_var_func_list()->accept(this);
}

void FillTableVisitor::visit(ClassListNode *node) {
  const char *class_lex = node->get_id() != NULL ? node->get_id()->get_lexeme() : NULL;
  const char *parent_class_lex = node->get_parent_id() != NULL ? node->get_parent_id()->get_lexeme() : NULL;

  if (node->get_next() != NULL) node->get_next()->accept(this);

  begin_scope(class_lex);
  active_class = class_lex;
  if (node->get_var_func() != NULL) node->get_var_func()->accept(this);
  // Creates base constructor in case it doesn'temp exist
  function_table->insert(new TypeNode(ID, class_lex, 0), class_lex, NULL, 1);
  active_class = NULL;
  end_scope();

  VarDeclNode *var_aux = NULL;
  FunctionDeclNode *func_aux = NULL;

  VarFuncListNode *var_func_aux = node->get_var_func();
  if (var_func_aux != NULL) {
    var_aux = var_func_aux->get_var_decl();
    if (var_aux != NULL) {
      var_func_aux = var_aux->get_next();
      while (var_func_aux != NULL) {
        if (var_func_aux->get_var_decl() != NULL)
          var_func_aux = var_func_aux->get_var_decl()->get_next();
        else {
          func_aux = var_func_aux->getFunc_list();
          var_func_aux = NULL;
        }
      }
    } else
      func_aux = var_func_aux->getFunc_list();
  }

  if (!class_table->insert(class_lex, var_aux, func_aux, parent_class_lex))
    print_semantic_error(ERROR_CLASS_ALREADY_EXISTS, node->get_line());
}

void FillTableVisitor::visit(VarDeclNode *node) {
  IdListNode *id_list_aux = node->get_id_list();
  while (id_list_aux != NULL) {
    id_list_aux->get_id()->set_type(node->get_type()->get_token());
    id_list_aux->get_id()->setType_lex(node->get_type()->get_lexeme());
    id_list_aux = id_list_aux->get_next();
  }
  if (node->get_id_list() != NULL) node->get_id_list()->accept(this);
  if (node->get_next() != NULL) node->get_next()->accept(this);
}

void FillTableVisitor::visit(IdListNode *node) {
  if (node->get_id() != NULL) {
    node->get_id()->accept(this);
    node->get_id()->set_pointer(node->get_pointer() != NULL);
    if (node->get_array() != NULL) {
      node->get_array()->accept(this);
      node->get_id()->set_array_size(node->get_array()->get_array_size());
    }
  }
  if (!var_table->insert(new TypeNode(node->get_id()->get_type(), node->get_id()->getType_lex(), node->get_line()),
                         node->get_id()->get_lexeme(), node->get_id()->is_pointer(),
                         node->get_id()->get_array_size(), BOOL_FALSE))
    print_semantic_error(ERROR_VARIABLE_ALREADY_EXISTS, node->get_line());

  if (node->get_next() != NULL) node->get_next()->accept(this);
}

void FillTableVisitor::visit(TypeNode *node) {}

void FillTableVisitor::visit(FunctionDeclNode *node) {
  const char *function_lex = node->get_id() != NULL ? node->get_id()->get_lexeme() : NULL;

  if (node->get_next() != NULL) node->get_next()->accept(this);

  begin_scope(function_lex);
  if (node->get_id() != NULL) node->get_id()->accept(this);
  if (node->get_formal_list() != NULL) node->get_formal_list()->accept(this);
  if (node->getVar_stmt() != NULL) node->getVar_stmt()->accept(this);
  end_scope();

  if (!function_table->insert(node->get_type(), function_lex, node->get_formal_list(), node->get_pointer() != NULL))
    print_semantic_error(ERROR_FUNCTION_ALREADY_EXISTS, node->get_line());
}

void FillTableVisitor::visit(TypeListNode *node) {
  const char *type_lex = node->get_id() != NULL ? node->get_id()->get_lexeme() : NULL;

  if (node->get_next() != NULL) node->get_next()->accept(this);

  begin_scope(type_lex);
  if (node->get_var_list() != NULL) node->get_var_list()->accept(this);
  end_scope();

  if (!struct_table->insert(type_lex, node->get_var_list()))
    print_semantic_error(ERROR_STRUCT_ALREADY_EXISTS, node->get_line());
}

void FillTableVisitor::visit(PointerNode *node) {
  /// TALVEZ NÃO PASSE AQUI
}

void FillTableVisitor::visit(StatementListNode *node) {
  if (node->get_statement() != NULL) node->get_statement()->accept(this);
  if (node->get_next() != NULL) node->get_next()->accept(this);
}

void FillTableVisitor::visit(VarStmtNode *node) {
  if (node->get_var_decl() != NULL) node->get_var_decl()->accept(this);
  if (node->getStmt_list() != NULL) node->getStmt_list()->accept(this);
  if (node->get_next() != NULL) node->get_next()->accept(this);
}

void FillTableVisitor::visit(VarFuncListNode *node) {
  if (node->get_var_decl() != NULL) node->get_var_decl()->accept(this);
  if (node->getFunc_list() != NULL) node->getFunc_list()->accept(this);
}

void FillTableVisitor::visit(IfNode *node) {
  if (node->get_exp() != NULL) node->get_exp()->accept(this);
  if (node->get_if_stmt() != NULL) node->get_if_stmt()->accept(this);
  if (node->get_else_stmt() != NULL) node->get_else_stmt()->accept(this);
}

void FillTableVisitor::visit(WhileNode *node) {
  if (node->get_exp() != NULL) node->get_exp()->accept(this);
  if (node->get_statement() != NULL) node->get_statement()->accept(this);
}

void FillTableVisitor::visit(SwitchNode *node) {
  if (node->get_exp() != NULL) node->get_exp()->accept(this);
  if (node->get_case_block() != NULL) node->get_case_block()->accept(this);
}

void FillTableVisitor::visit(CaseBlockNode *node) {
  if (node->get_next() != NULL) node->get_next()->accept(this);
  if (node->get_num() != NULL) node->get_num()->accept(this);
  if (node->get_statement_list() != NULL) node->get_statement_list()->accept(this);
}

void FillTableVisitor::visit(BreakNode *node) {}

void FillTableVisitor::visit(PrintlnNode *node) {
  if (node->get_exp_list() != NULL) node->get_exp_list()->accept(this);
}

void FillTableVisitor::visit(ReadNode *node) {
  if (node->get_exp() != NULL) node->get_exp()->accept(this);
}

void FillTableVisitor::visit(ReturnNode *node) {
  if (node->get_exp() != NULL) node->get_exp()->accept(this);
}

void FillTableVisitor::visit(ThrowNode *node) {}

void FillTableVisitor::visit(ExpListNode *node) {
  if (node->get_next() != NULL) node->get_next()->accept(this);
  if (node->get_exp() != NULL) node->get_exp()->accept(this);
}

void FillTableVisitor::visit(TryNode *node) {
  if (node->get_try_stmt() != NULL) node->get_try_stmt()->accept(this);
  if (node->get_catch_stmt() != NULL) node->get_catch_stmt()->accept(this);
}

void FillTableVisitor::visit(IdentifierNode *node) {
  node->set_value(node->get_lexeme());
}

void FillTableVisitor::visit(IntegerNode *node) {
  node->set_type(INT);
  node->setType_lex(get_token_name(INT));
  node->set_value(node->get_lexeme());
}

void FillTableVisitor::visit(RealNode *node) {
  node->set_type(FLOAT);
  node->setType_lex(get_token_name(FLOAT));
  node->set_value(node->get_lexeme());
}

void FillTableVisitor::visit(LiteralNode *node) {
  node->set_type(LITERAL);
  node->setType_lex(get_token_name(LITERAL));
  node->set_value(node->get_lexeme());
}

void FillTableVisitor::visit(CharacterNode *node) {
  node->set_type(CHAR);
  node->setType_lex(get_token_name(CHAR));
  node->set_value(node->get_lexeme());
}

void FillTableVisitor::visit(PrimaryNode *node) {
  if (node->get_exp() != NULL) node->get_exp()->accept(this);
  if (node->get_next() != NULL) node->get_next()->accept(this);
}

void FillTableVisitor::visit(FunctionCallNode *node) {
  if (node->get_id() != NULL) node->get_id()->accept(this);
  if (node->get_exp_list() != NULL) node->get_exp_list()->accept(this);
  if (node->get_next() != NULL) node->get_next()->accept(this);
}

void FillTableVisitor::visit(FieldAccessNode *node) {
  if (node->get_exp() != NULL) {
    node->set_lvalue(BOOL_TRUE);
    node->get_exp()->accept(this);
  }
}

void FillTableVisitor::visit(PointerAccessNode *node) {
  if (node->get_exp() != NULL) {
    node->set_lvalue(BOOL_TRUE);
    node->get_exp()->accept(this);
  }
}

void FillTableVisitor::visit(AddressValueNode *node) {
  if (node->get_exp() != NULL) node->get_exp()->accept(this);
}

void FillTableVisitor::visit(PointerValueNode *node) {
  node->set_lvalue(BOOL_TRUE);
  if (node->get_exp() != NULL) node->get_exp()->accept(this);
}

void FillTableVisitor::visit(ArrayAccessNode *node) {
  node->set_lvalue(BOOL_TRUE);
  if (node->get_next() != NULL) node->get_next()->accept(this);
  if (node->get_index_exp() != NULL) node->get_index_exp()->accept(this);
}

void FillTableVisitor::visit(ArrayDeclNode *node) {
  if (node->get_index_exp() != NULL) node->get_index_exp()->accept(this);

  if (node->get_index_exp()->get_type() != INT) {
    print_semantic_error(ERROR_ARRAY_SIZE_MUST_BE_OF_TYPE_INT, node->get_line());
    node->set_array_size(-1);
  } else
    node->set_array_size(atoi(node->get_index_exp()->get_value()));
}

void FillTableVisitor::visit(AssignNode *node) {
  if (node->get_left_exp() != NULL) node->get_left_exp()->accept(this);
  if (node->get_right_exp() != NULL) node->get_right_exp()->accept(this);
}

void FillTableVisitor::visit(RelationalOpNode *node) {
  if (node->get_left_exp() != NULL) node->get_left_exp()->accept(this);
  if (node->get_right_exp() != NULL) node->get_right_exp()->accept(this);
}

void FillTableVisitor::visit(AdditionOpNode *node) {
  if (node->get_left_exp() != NULL) node->get_left_exp()->accept(this);
  if (node->get_right_exp() != NULL) node->get_right_exp()->accept(this);
}

void FillTableVisitor::visit(MultiplicationOpNode *node) {
  if (node->get_left_exp() != NULL) node->get_left_exp()->accept(this);
  if (node->get_right_exp() != NULL) node->get_right_exp()->accept(this);
}

void FillTableVisitor::visit(BooleanOpNode *node) {
  if (node->get_left_exp() != NULL) node->get_left_exp()->accept(this);
  if (node->get_right_exp() != NULL) node->get_right_exp()->accept(this);
}

void FillTableVisitor::visit(BitwiseOpNode *node) {
  if (node->get_left_exp() != NULL) node->get_left_exp()->accept(this);
  if (node->get_right_exp() != NULL) node->get_right_exp()->accept(this);
}

void FillTableVisitor::visit(NewNode *node) {
  if (node->get_exp_list() != NULL) node->get_exp_list()->accept(this);
  if (node->get_id() != NULL) node->get_id()->accept(this);
}

void FillTableVisitor::visit(NotNode *node) {
  if (node->get_exp() != NULL) node->get_exp()->accept(this);
}

void FillTableVisitor::visit(SignNode *node) {
  if (node->get_exp() != NULL) node->get_exp()->accept(this);
}

void FillTableVisitor::visit(TrueNode *node) {
  node->set_type(BOOL);
  node->setType_lex(get_token_name(BOOL));
  node->set_value(get_token_name(TRUE));
}

void FillTableVisitor::visit(FalseNode *node) {
  node->set_type(BOOL);
  node->setType_lex(get_token_name(BOOL));
  node->set_value(get_token_name(FALSE));
}

void FillTableVisitor::visit(ThisNode *node) {
  node->set_type(ID);
  node->setType_lex(active_class);
}

void FillTableVisitor::visit(FormalListNode *node) {
  const char *id_lex_aux = node->get_id() != NULL ? node->get_id()->get_lexeme() : NULL;
  if (node->get_id() != NULL) node->get_id()->accept(this);
  if (node->get_array() != NULL) node->get_array()->accept(this);
  int array_size = node->get_array() != NULL ? atoi(node->get_array()->get_index_exp()->get_value()) : -1;

  if (!var_table->insert(node->get_type(), id_lex_aux, node->get_pointer() != NULL, array_size, BOOL_TRUE))
    print_semantic_error(ERROR_VARIABLE_ALREADY_EXISTS, node->get_line());

  if (node->get_next() != NULL) node->get_next()->accept(this);
}

/**************************** SEMANTIC ANALYZER ***************************************************/

SemanticAnalyzerVisitor::SemanticAnalyzerVisitor(ProgramNode *program) {
  visit(program);
}

void SemanticAnalyzerVisitor::visit(ProgramNode *node) {
  if (node->get_class_list() != NULL) node->get_class_list()->accept(this);
  if (node->get_type_list() != NULL) node->get_type_list()->accept(this);
  if (node->get_var_func_list() != NULL) node->get_var_func_list()->accept(this);
}

void SemanticAnalyzerVisitor::visit(ClassListNode *node) {
  if (node->get_next() != NULL) node->get_next()->accept(this);

  const char *class_lex = node->get_id() != NULL ? node->get_id()->get_lexeme() : NULL;
  const char *parent_lex = node->get_parent_id() != NULL ? node->get_parent_id()->get_lexeme() : NULL;

  begin_scope(class_lex);
  active_class = class_lex;
  if (node->get_var_func() != NULL) node->get_var_func()->accept(this);
  active_class = NULL;
  end_scope();

  if (parent_lex != NULL && !class_table->lookup(parent_lex))
    print_semantic_error(ERROR_PARENT_CLASS_NOT_FOUND, node->get_line());
}

void SemanticAnalyzerVisitor::visit(VarDeclNode *node) {
  if (node->get_type() != NULL && node->get_id_list() != NULL)
    ///Verify Struct or Class Declaration
    if (node->get_type()->get_token() == ID)
      if (!struct_table->lookup(node->get_type()->get_lexeme()) &&
          !class_table->lookup(node->get_type()->get_lexeme())) ///SEMANTIC ERROR - TYPE NOT DEFINED
        print_semantic_error(ERROR_TYPE_NOT_DEFINED, node->get_line());
  if (node->get_id_list() != NULL) {
    node->get_id_list()->accept(this);
    IdListNode *list_aux = node->get_id_list();
    int total_size_aux = 0;
    while (list_aux != NULL) {
      VarSymbol *var;
      if (active_function != NULL)
        var = var_table->lookup_in_scope(list_aux->get_id()->get_lexeme(), active_function->get_lexeme());
      else
        var = var_table->lookup(list_aux->get_id()->get_lexeme());
      int size_aux = BYTE_SIZE;
      if (list_aux->get_array() != NULL)
        size_aux *= atoi(list_aux->get_array()->get_index_exp()->get_value());
      if (active_function != NULL)
        list_aux->get_id()->set_offset(active_function->getLocal_size() + total_size_aux + size_aux);
      var->set_size(size_aux);
      var->set_offset(list_aux->get_id()->get_offset());
      total_size_aux += size_aux;
      list_aux = list_aux->get_next();
    }
    if (active_function != NULL)
      active_function->incrementLocal_size(total_size_aux);
  }

  if (node->get_next() != NULL) node->get_next()->accept(this);
}

void SemanticAnalyzerVisitor::visit(IdListNode *node) {
//  if (node->get_pointer() != NULL) node->get_pointer()->accept(this);
  if (node->get_id() != NULL) node->get_id()->accept(this);
  if (node->get_array() != NULL) node->get_array()->accept(this);
  if (node->get_next() != NULL) node->get_next()->accept(this);
}

void SemanticAnalyzerVisitor::visit(TypeNode *node) {}

void SemanticAnalyzerVisitor::visit(FunctionDeclNode *node) {
  if (node->get_next() != NULL) node->get_next()->accept(this);

  const char *function_lex = node->get_id() != NULL ? node->get_id()->get_lexeme() : NULL;

  ///*** Check if type was declared
  if (node->get_type() != NULL && node->get_id() != NULL)
    if (node->get_type()->get_token() == ID)
      if (!struct_table->lookup(node->get_type()->get_lexeme()) &&
          !class_table->lookup(node->get_type()->get_lexeme())) ///SEMANTIC ERROR - TYPE NOT DEFINED
        print_semantic_error(ERROR_RETURN_TYPE_NOT_DEFINED, node->get_line());

  active_function = function_table->lookup(function_lex);
  begin_scope(function_lex);
  if (node->get_id() != NULL) node->get_id()->accept(this);
  if (node->get_formal_list() != NULL) node->get_formal_list()->accept(this);
  if (node->getVar_stmt() != NULL) node->getVar_stmt()->accept(this);
  VarStmtNode *var_stmt_aux = node->getVar_stmt();
  while (var_stmt_aux != NULL && var_stmt_aux->get_var_decl() != NULL)
    var_stmt_aux = var_stmt_aux->get_next();
  if (var_stmt_aux != NULL && var_stmt_aux->getStmt_list() != NULL) {
    StatementListNode *stmt_list_aux = var_stmt_aux->getStmt_list();
    while (stmt_list_aux != NULL && stmt_list_aux->get_statement() != NULL) {
      if (typeid(*stmt_list_aux->get_statement()) == typeid(ReturnNode)) {
        stmt_list_aux->get_statement()->accept(this);
        ReturnNode *return_stmt = dynamic_cast<ReturnNode *>(stmt_list_aux->get_statement());
        if (node->get_type()->get_token() != return_stmt->get_exp()->get_type())
          print_semantic_error(ERROR_INCOMPATIBLE_RETURN_TYPE, node->get_line());
        stmt_list_aux = stmt_list_aux->get_next();
      } else {
        ///if there's an expected return value
        if (stmt_list_aux->get_next() == NULL) {///SEMANTIC ERROR - MISSING RETURN STATEMENT
          print_semantic_error(ERROR_MISSING_RETURN_STATEMENT, node->get_line());
          break;
        } else stmt_list_aux = stmt_list_aux->get_next();
      }
    }
  }
  end_scope();
  active_function = NULL;
}

void SemanticAnalyzerVisitor::visit(TypeListNode *node) {
  if (node->get_next() != NULL) node->get_next()->accept(this);

  const char *type_lex = node->get_id() != NULL ? node->get_id()->get_lexeme() : NULL;
  begin_scope(type_lex);
  if (node->get_var_list() != NULL) {
    node->get_var_list()->accept(this);
    VarDeclNode *var_list_aux = node->get_var_list();
    int total_size_aux = 0;
    while (var_list_aux != NULL) {
      IdListNode *id_list_aux = var_list_aux->get_id_list();
      while (id_list_aux != NULL) {
        VarSymbol *var = var_table->lookup_in_scope(id_list_aux->get_id()->get_lexeme(),
                                                    node->get_id()->get_lexeme());
        int size_aux = BYTE_SIZE;
        if (id_list_aux->get_array() != NULL)
          size_aux *= atoi(id_list_aux->get_array()->get_index_exp()->get_value());
        var->set_size(size_aux);
        var->set_offset(total_size_aux);
        total_size_aux += size_aux;
        id_list_aux = id_list_aux->get_next();
      }
      var_list_aux = var_list_aux->get_next()->get_var_decl();
    }
    StructSymbol *struct_symbol = struct_table->lookup(node->get_id()->get_lexeme());
    struct_symbol->set_size(total_size_aux);
  }
  end_scope();
}

void SemanticAnalyzerVisitor::visit(PointerNode *node) {
  /// DOES NOTHING
}

void SemanticAnalyzerVisitor::visit(StatementListNode *node) {
  if (node->get_statement() != NULL) node->get_statement()->accept(this);
  if (node->get_next() != NULL) node->get_next()->accept(this);
}

void SemanticAnalyzerVisitor::visit(VarStmtNode *node) {
  if (node->get_var_decl() != NULL) node->get_var_decl()->accept(this);
  if (node->getStmt_list() != NULL) node->getStmt_list()->accept(this);
  if (node->get_next() != NULL) node->get_next()->accept(this);
}

void SemanticAnalyzerVisitor::visit(VarFuncListNode *node) {
  if (node->get_var_decl() != NULL) node->get_var_decl()->accept(this);
  if (node->getFunc_list() != NULL) node->getFunc_list()->accept(this);
}

void SemanticAnalyzerVisitor::visit(IfNode *node) {
  if (node->get_exp() != NULL) {
    node->get_exp()->accept(this);
    if (node->get_exp()->get_type() != BOOL)
      print_semantic_error(ERROR_BOOLEAN_EXPRESSION_REQUIRED, node->get_line());
  }
  if (node->get_if_stmt() != NULL) node->get_if_stmt()->accept(this);
  if (node->get_else_stmt() != NULL) node->get_else_stmt()->accept(this);
}

void SemanticAnalyzerVisitor::visit(WhileNode *node) {
  if (node->get_exp() != NULL) {
    node->get_exp()->accept(this);
    if (node->get_exp()->get_type() != BOOL)
      print_semantic_error(ERROR_BOOLEAN_EXPRESSION_REQUIRED, node->get_line());
  }
  break_enabled = BOOL_TRUE;
  if (node->get_statement() != NULL) node->get_statement()->accept(this);
  break_enabled = BOOL_FALSE;
}

void SemanticAnalyzerVisitor::visit(SwitchNode *node) {
  if (node->get_exp() != NULL) {
    node->get_exp()->accept(this);
    if (node->get_exp()->get_type() != INT)
      print_semantic_error(ERROR_INT_VALUE_REQUIRED, node->get_line());
  }
  if (node->get_case_block() != NULL) {
    break_enabled = BOOL_TRUE;
    node->get_case_block()->accept(this);
    break_enabled = BOOL_FALSE;
  } else
    print_semantic_error(ERROR_CASE_STATEMENT_REQUIRED, node->get_line());
}

void SemanticAnalyzerVisitor::visit(BreakNode *node) {
  if (break_enabled != BOOL_TRUE)
    print_semantic_error(ERROR_UNEXPECTED_BREAK, node->get_line());
}

void SemanticAnalyzerVisitor::visit(PrintlnNode *node) {
  if (node->get_exp_list() != NULL) node->get_exp_list()->accept(this);
}

void SemanticAnalyzerVisitor::visit(ReadNode *node) {
  if (node->get_exp() != NULL) node->get_exp()->accept(this);
}

void SemanticAnalyzerVisitor::visit(ReturnNode *node) {
  if (node->get_exp() != NULL) node->get_exp()->accept(this);
}

void SemanticAnalyzerVisitor::visit(CaseBlockNode *node) {
  if (node->get_statement_list() != NULL) node->get_statement_list()->accept(this);
  if (node->get_next() != NULL) node->get_next()->accept(this);
}

void SemanticAnalyzerVisitor::visit(ThrowNode *node) {

}

void SemanticAnalyzerVisitor::visit(ExpListNode *node) {
  if (node->get_exp() != NULL) node->get_exp()->accept(this);
  if (node->get_next() != NULL) node->get_next()->accept(this);
}

void SemanticAnalyzerVisitor::visit(TryNode *node) {
  if (node->get_try_stmt() != NULL) node->get_try_stmt()->accept(this);
  if (node->get_catch_stmt() != NULL) node->get_catch_stmt()->accept(this);
}

void SemanticAnalyzerVisitor::visit(IdentifierNode *node) {
  if (VarSymbol *v = var_table->lookup(node->get_lexeme())) {
    node->set_type(v->get_type()->get_token());
    node->setType_lex(v->get_type()->get_lexeme());
  }
  node->set_value(node->get_lexeme());
}

void SemanticAnalyzerVisitor::visit(IntegerNode *node) {

}

void SemanticAnalyzerVisitor::visit(RealNode *node) {

}

void SemanticAnalyzerVisitor::visit(LiteralNode *node) {

}

void SemanticAnalyzerVisitor::visit(CharacterNode *node) {

}

void SemanticAnalyzerVisitor::visit(PrimaryNode *node) {
  if (node->get_exp() != NULL) {
    node->get_exp()->accept(this);
    if (IdentifierNode *id = dynamic_cast<IdentifierNode *>(node->get_exp())) {
      node->set_lvalue(BOOL_TRUE);
      if (node->get_next() != NULL && typeid(*node->get_next()) == typeid(FunctionCallNode)) {
        FunctionCallNode *function_call = dynamic_cast<FunctionCallNode *>(node->get_next());
        function_call->setOwner_lex(node->getOwner_lex());
        node->get_next()->accept(this);
        node->set_type(node->get_next()->get_type());
        node->setType_lex(node->get_next()->getType_lex());
      } else {
        VarSymbol *var_symbol = node->getOwner_lex() == NULL ? var_table->lookup(id->get_lexeme())
                                                             : var_table->lookup_in_scope(id->get_lexeme(),
                                                                                          node->getOwner_lex());
        if (var_symbol != NULL) {
          const char *owner = NULL;
          if (StructSymbol *struct_symbol = struct_table->lookup(var_symbol->get_type()->get_lexeme()))
            owner = struct_symbol->get_lexeme();
          else if (ClassSymbol *class_symbol = class_table->lookup(var_symbol->get_type()->get_lexeme()))
            owner = class_symbol->get_lexeme();
          else {
            id->set_type(var_symbol->get_type()->get_token());
            id->setType_lex(var_symbol->get_type()->get_lexeme());
          }

          if (owner != NULL && node->get_next() != NULL) {
            if (!var_symbol->is_pointer()) {
              if (typeid(*node->get_next()) == typeid(FieldAccessNode)) {
                FieldAccessNode *field = dynamic_cast<FieldAccessNode *>(node->get_next());
                field->setOwner_lex(owner);
                node->get_next()->accept(this);

                node->set_type(field->get_exp()->get_type());
                node->setType_lex(field->get_exp()->getType_lex());
              } else if (typeid(*node->get_next()) == typeid(PointerAccessNode))
                print_semantic_error(ERROR_ARROW_NON_POINTER, node->get_line());
            } else {
              if (typeid(*node->get_next()) == typeid(PointerAccessNode)) {
                PointerAccessNode *field = dynamic_cast<PointerAccessNode *>(node->get_next());
                field->setOwner_lex(owner);
                node->get_next()->accept(this);

                node->set_type(field->get_exp()->get_type());
                node->setType_lex(field->get_exp()->getType_lex());
              } else if (typeid(*node->get_next()) == typeid(FieldAccessNode))
                print_semantic_error(ERROR_PERIOD_POINTER_ACCESS, node->get_line());
            }
          } else if (var_symbol->get_type()->get_token() != INT
                     && var_symbol->get_type()->get_token() != FLOAT
                     && var_symbol->get_type()->get_token() != CHAR
                     && var_symbol->get_type()->get_token() != LITERAL
                     && var_symbol->get_type()->get_token() != BOOL)
            print_semantic_error(ERROR_TYPE_NOT_DEFINED, node->get_line());
          else {
            node->set_type(node->get_exp()->get_type());
            node->setType_lex(node->get_exp()->getType_lex());
          }
        } else
          print_semantic_error(ERROR_VARIABLE_NOT_DECLARED, node->get_line());

      }
    } else if (typeid(*node->get_exp()) == typeid(IntegerNode) || typeid(*node->get_exp()) == typeid(RealNode) ||
               typeid(*node->get_exp()) == typeid(CharacterNode) || typeid(*node->get_exp()) == typeid(LiteralNode) ||
               typeid(*node->get_exp()) == typeid(TrueNode) || typeid(*node->get_exp()) == typeid(FalseNode)) {
      if (node->get_next() != NULL)
        print_semantic_error(ERROR_UNEXPECTED_ACCESS_IN_NOT_ID_TYPE, node->get_line());
      node->set_type(node->get_exp()->get_type());
      node->setType_lex(node->get_exp()->getType_lex());
    } else if (NewNode *new_node = dynamic_cast<NewNode *>(node->get_exp())) {
      if (FieldAccessNode *field = dynamic_cast<FieldAccessNode *>(node->get_next()))
        field->setOwner_lex(new_node->get_id()->get_lexeme());
      if (node->get_next() != NULL) {
        node->get_next()->accept(this);
        node->set_type(node->get_next()->get_type());
        node->setType_lex(node->get_next()->getType_lex());
      }
    } else if (ThisNode *this_node = dynamic_cast<ThisNode *>(node->get_exp())) {
      if (FieldAccessNode *field = dynamic_cast<FieldAccessNode *>(node->get_next()))
        field->setOwner_lex(this_node->getType_lex());
      if (node->get_next() != NULL) {
        node->get_next()->accept(this);
        node->set_type(node->get_next()->get_type());
        node->setType_lex(node->get_next()->getType_lex());
      }
    } else {
      if (node->get_next() != NULL) {
        node->get_next()->accept(this);
        node->set_type(node->get_next()->get_type());
        node->setType_lex(node->get_next()->getType_lex());
      } else {
        node->set_type(node->get_exp()->get_type());
        node->setType_lex(node->get_exp()->getType_lex());
      }
    }

    node->set_pointer(node->get_exp()->is_pointer());
    node->set_array_size(node->get_exp()->get_array_size());
    node->set_value(node->get_exp()->get_value());
  }
}

void SemanticAnalyzerVisitor::visit(FunctionCallNode *node) {
  FunctionSymbol *func_symbol;
  if (node->getOwner_lex() != NULL) {
    func_symbol = function_table->lookup_in_scope(node->get_id()->get_lexeme(), node->getOwner_lex());
    const char *owner = node->getOwner_lex();
    while (func_symbol == NULL && owner != NULL) {
      if (ClassSymbol *class_symbol = class_table->lookup(owner)) {
        func_symbol = function_table->lookup_in_scope(node->get_id()->get_lexeme(), owner);
        owner = class_symbol->getParent_lexeme();
      }
    }
  } else
    func_symbol = function_table->lookup(node->get_id()->get_lexeme());
  if (func_symbol != NULL) {
    if (node->get_id() != NULL) {
      node->get_id()->accept(this);
      FunctionSymbol *func = function_table->lookup(node->get_id()->get_lexeme());
      if (func->getParam_size() > active_function->getCall_size())
        active_function->setCall_size(func->getParam_size());
    }
    if (node->get_exp_list() != NULL) {
      node->get_exp_list()->accept(this);
      ExpListNode *exp = dynamic_cast<ExpListNode *>(node->get_exp_list());
      FormalListNode *param = func_symbol->getVar_decl();
      while (exp != NULL && param != NULL) {
        if (exp->get_exp()->get_type() != param->get_type()->get_token() ||
            exp->get_exp()->getType_lex() != param->get_type()->get_lexeme
                    ()) {
          print_semantic_error(ERROR_PARAMETER_TYPE_MISMATCH, node->get_line());
          exp = exp->get_next();
          param = param->get_next();
          break;
        }
        exp = exp->get_next();
        param = param->get_next();
      }
      if (exp == NULL && param != NULL)
        print_semantic_error(ERROR_TOO_FEW_ARGUMENTS, node->get_line());
      else if (exp != NULL && param == NULL)
        print_semantic_error(ERROR_TOO_MANY_ARGUMENTS, node->get_line());
    }
    if (node->get_next() != NULL) {
      if (FieldAccessNode *field = dynamic_cast<FieldAccessNode *>(node->get_next()))
        field->setOwner_lex(func_symbol->getReturn_type()->get_lexeme());
      node->get_next()->accept(this);
      node->set_type(node->get_next()->get_type());
      node->setType_lex(node->get_next()->getType_lex());
    } else {
      node->set_type(func_symbol->getReturn_type()->get_token());
      node->setType_lex(func_symbol->getReturn_type()->get_lexeme());
    }
  } else
    print_semantic_error(ERROR_FUNCTION_NOT_DECLARED, node->get_line());
}

void SemanticAnalyzerVisitor::visit(FieldAccessNode *node) {
  if (PrimaryNode *primary = dynamic_cast<PrimaryNode *>(node->get_exp())) {
    const char *owner = node->getOwner_lex();
    primary->setOwner_lex(owner);
    node->get_exp()->accept(this);
    if (IdentifierNode *id = dynamic_cast<IdentifierNode *>(primary->get_exp())) {
      if (class_table->lookup(owner)) {
        VarSymbol *var = NULL;
        FunctionSymbol *func = NULL;
        while (var == NULL && func == NULL && owner != NULL) {
          ClassSymbol *class_symbol = class_table->lookup(owner);
          VarSymbol *var = var_table->lookup_in_scope(id->get_lexeme(), owner);
          func = function_table->lookup_in_scope(id->get_lexeme(), owner);
          if (class_symbol) {
            if (var) {
              node->set_type(var->get_type()->get_token());
              node->setType_lex(var->get_type()->get_lexeme());
            }
            if (func) {
              node->set_type(func->getReturn_type()->get_token());
              node->setType_lex(func->getReturn_type()->get_lexeme());
            }
            owner = class_symbol->getParent_lexeme();
          }
        }
        if (!var && !func)
          print_semantic_error(ERROR_CLASS_DOES_NOT_HAVE_MEMBER, node->get_line());
      } else {
        VarSymbol *var = var_table->lookup_in_scope(id->get_lexeme(), owner);
        FunctionSymbol *func = function_table->lookup_in_scope(id->get_lexeme(), owner);
        if (var) {
          node->set_type(var->get_type()->get_token());
          node->setType_lex(var->get_type()->get_lexeme());
        }
        if (func) {
          node->set_type(func->getReturn_type()->get_token());
          node->setType_lex(func->getReturn_type()->get_lexeme());
        }
        if (!var && !func)
          print_semantic_error(ERROR_STRUCT_DOES_NOT_HAVE_MEMBER, node->get_line());
      }
    }
  }
}

void SemanticAnalyzerVisitor::visit(PointerAccessNode *node) {
  if (PrimaryNode *primary = dynamic_cast<PrimaryNode *>(node->get_exp())) {
    const char *owner = node->getOwner_lex();
    primary->setOwner_lex(owner);
    node->get_exp()->accept(this);
    if (IdentifierNode *id = dynamic_cast<IdentifierNode *>(primary->get_exp())) {
      if (class_table->lookup(owner)) {
        VarSymbol *var = NULL;
        FunctionSymbol *func = NULL;
        while (var == NULL && func == NULL && owner != NULL) {
          ClassSymbol *class_symbol = class_table->lookup(owner);
          VarSymbol *var = var_table->lookup_in_scope(id->get_lexeme(), owner);
          func = function_table->lookup_in_scope(id->get_lexeme(), owner);
          if (class_symbol) {
            if (var) {
              node->set_type(var->get_type()->get_token());
              node->setType_lex(var->get_type()->get_lexeme());
            }
            if (func) {
              node->set_type(func->getReturn_type()->get_token());
              node->setType_lex(func->getReturn_type()->get_lexeme());
            }
            owner = class_symbol->getParent_lexeme();
          }
        }
        if (!var && !func)
          print_semantic_error(ERROR_CLASS_DOES_NOT_HAVE_MEMBER, node->get_line());
      } else {
        VarSymbol *var = var_table->lookup_in_scope(id->get_lexeme(), owner);
        FunctionSymbol *func = function_table->lookup_in_scope(id->get_lexeme(), owner);
        if (var) {
          node->set_type(var->get_type()->get_token());
          node->setType_lex(var->get_type()->get_lexeme());
        }
        if (func) {
          node->set_type(func->getReturn_type()->get_token());
          node->setType_lex(func->getReturn_type()->get_lexeme());
        }
        if (!var && !func)
          print_semantic_error(ERROR_STRUCT_DOES_NOT_HAVE_MEMBER, node->get_line());
      }
    }
  }
}

void SemanticAnalyzerVisitor::visit(AddressValueNode *node) {
  if (node->get_exp() != NULL) {
    node->get_exp()->accept(this);
    //if ()///quais são as restrições referentes local_access endereço??
  }
  ///testar LVALUE?
}

void SemanticAnalyzerVisitor::visit(PointerValueNode *node) {
  if (node->get_exp() != NULL) {
    node->get_exp()->accept(this);
    ///fazer verificação node->get_exp() == pointer ??
    //TODO: AVALIAR EXPRESSÃO
  }
}

void SemanticAnalyzerVisitor::visit(ArrayAccessNode *node) {
  //TODO: RECEBER LEXEMA PARA PODER verificar se valor do index está fora do limite
  if (node->get_index_exp() != NULL) node->get_index_exp()->accept(this);
  if (node->get_next() != NULL) node->get_next()->accept(this);
}

void SemanticAnalyzerVisitor::visit(ArrayDeclNode *node) {
  if (node->get_index_exp() != NULL) node->get_index_exp()->accept(this);
}

void SemanticAnalyzerVisitor::visit(AssignNode *node) {
  if (node->get_left_exp() != NULL) node->get_left_exp()->accept(this);
  if (node->get_right_exp() != NULL) node->get_right_exp()->accept(this);

  if (!node->get_left_exp()->is_lvalue())
    print_semantic_error(ERROR_LVALUE_REQUIRED_AS_LEFT_OPERAND_OF_ASSIGNMENT, node->get_line());
  if (node->get_left_exp()->get_type() != node->get_right_exp()->get_type())
    print_semantic_error(ERROR_CANNOT_ASSIGN_TO_DIFFERENT_TYPE, node->get_line());
}

void SemanticAnalyzerVisitor::visit(RelationalOpNode *node) {
  if (node->get_left_exp() != NULL) node->get_left_exp()->accept(this);
  if (node->get_right_exp() != NULL) node->get_right_exp()->accept(this);

  if ((node->get_left_exp()->get_type() != INT && node->get_left_exp()->get_type() != FLOAT) &&
      (node->get_right_exp()->get_type() == INT || node->get_right_exp()->get_type() == FLOAT))
    print_semantic_error(ERROR_RELATIONAL_OP_TYPE, node->get_line());
  node->set_type(BOOL);
  node->setType_lex(get_token_name(BOOL));
}

void SemanticAnalyzerVisitor::visit(AdditionOpNode *node) {
  if (node->get_left_exp() != NULL) node->get_left_exp()->accept(this);
  if (node->get_right_exp() != NULL) node->get_right_exp()->accept(this);

  if ((node->get_left_exp()->get_type() != INT && node->get_left_exp()->get_type() != FLOAT) &&
      (node->get_right_exp()->get_type() == INT || node->get_right_exp()->get_type() == FLOAT))
    print_semantic_error(ERROR_ARITHMETIC_OP_TYPE, node->get_line());
  else {
    node->set_type(
            node->get_left_exp()->get_type() == node->get_right_exp()->get_type() ? node->get_left_exp()->get_type
                    () : FLOAT);
    node->setType_lex(get_token_name(node->get_type()));
  }
}

void SemanticAnalyzerVisitor::visit(MultiplicationOpNode *node) {
  if (node->get_left_exp() != NULL) node->get_left_exp()->accept(this);
  if (node->get_right_exp() != NULL) node->get_right_exp()->accept(this);

  if ((node->get_left_exp()->get_type() != INT && node->get_left_exp()->get_type() != FLOAT) &&
      (node->get_right_exp()->get_type() == INT || node->get_right_exp()->get_type() == FLOAT))
    print_semantic_error(ERROR_ARITHMETIC_OP_TYPE, node->get_line());
  else {
    node->set_type(
            node->get_left_exp()->get_type() == node->get_right_exp()->get_type() ? node->get_left_exp()->get_type
                    () : FLOAT);
    node->setType_lex(get_token_name(node->get_type()));
  }
}

void SemanticAnalyzerVisitor::visit(BooleanOpNode *node) {
  if (node->get_left_exp() != NULL) node->get_left_exp()->accept(this);
  if (node->get_right_exp() != NULL) node->get_right_exp()->accept(this);

  if (node->get_left_exp()->get_type() != BOOL || node->get_right_exp()->get_type() != BOOL)
    print_semantic_error(ERROR_LOGICAL_OP_TYPE, node->get_line());
  node->set_type(BOOL);
  node->setType_lex(get_token_name(BOOL));
}

void SemanticAnalyzerVisitor::visit(BitwiseOpNode *node) {
  if (node->get_left_exp() != NULL) node->get_left_exp()->accept(this);
  if (node->get_right_exp() != NULL) node->get_right_exp()->accept(this);

  if (node->get_left_exp()->get_type() != node->get_right_exp()->get_type())
    print_semantic_error(ERROR_BINARY_OP_TYPE, node->get_line());
  node->set_type(node->get_left_exp()->get_type());
  node->setType_lex(node->get_left_exp()->getType_lex());
}

void SemanticAnalyzerVisitor::visit(NewNode *node) {
  if (node->get_exp_list() != NULL) node->get_exp_list()->accept(this);
  if (node->get_id() != NULL) {
    node->get_id()->accept(this);
    node->set_type(ID);
    node->setType_lex(node->get_id()->get_lexeme());
    node->set_value(node->get_id()->get_lexeme());
  }
  ClassSymbol *class_symbol = class_table->lookup(node->get_id()->get_lexeme());
  if (!class_symbol)
    print_semantic_error(ERROR_CLASS_NOT_FOUND, node->get_line());
}

void SemanticAnalyzerVisitor::visit(NotNode *node) {
  if (node->get_exp() != NULL) node->get_exp()->accept(this);
  if (node->get_exp()->get_type() != BOOL)
    print_semantic_error(ERROR_LOGICAL_OP_TYPE, node->get_line());
  node->set_type(BOOL);
  node->setType_lex(get_token_name(BOOL));
  node->set_value(get_token_name(TRUE) == node->get_value() ? get_token_name(FALSE) : get_token_name(TRUE));
}

void SemanticAnalyzerVisitor::visit(SignNode *node) {
  if (node->get_exp() != NULL) {
    node->get_exp()->accept(this);
    node->set_type(node->get_exp()->get_type());
    node->setType_lex(node->get_exp()->getType_lex());
  }
}

void SemanticAnalyzerVisitor::visit(TrueNode *node) {
  node->set_type(BOOL);
  node->setType_lex(get_token_name(BOOL));
  node->set_value(get_token_name(TRUE));
}

void SemanticAnalyzerVisitor::visit(FalseNode *node) {
  node->set_type(BOOL);
  node->setType_lex(get_token_name(BOOL));
  node->set_value(get_token_name(FALSE));
}

void SemanticAnalyzerVisitor::visit(ThisNode *node) {
  node->set_type(ID);
  node->setType_lex(active_class);
  if (active_class == NULL)
    print_semantic_error(ERROR_INVALID_USE_OF_THIS, node->get_line());
}

void SemanticAnalyzerVisitor::visit(FormalListNode *node) {
  if (node->get_id() != NULL && active_function != NULL) {
    VarSymbol *var = var_table->lookup_in_scope(node->get_id()->get_lexeme(), active_function->get_lexeme());
    int size_aux = BYTE_SIZE;
    if (node->get_array() != NULL)
      size_aux *= atoi(node->get_array()->get_index_exp()->get_value());
    var->set_size(size_aux);
    var->set_offset(active_function->getParam_size());
    node->get_id()->set_offset(var->get_offset());
    active_function->incrementParam_size(size_aux);
  }
  if (node->get_next() != NULL) node->get_next()->accept(this);
}
