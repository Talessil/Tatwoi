#include <cstdio>
#include <cstring>
#include <typeinfo>
#include "intermediate_code.h"
#include "translator.h"
#include "ast.h"
#include "lex_analyzer.h"
#include "table.h"
#include "semantic_analyzer.h"

Translator *translator;
int n_while_label = 0;
int n_literal_label = 0;
FunctionSymbol *current_function;

Translator *init_translator(ProgramNode *program) {
  translator = new Translator();
  if (program != NULL) program->accept(translator);
  fprintf(stdout, "\n \n ----- INTERMEDIATE REPRESENTATION ------ \n \n");
  translator->print_ir();

  Canonizer *canonizer = new Canonizer();
  while (canonizer->is_changed()) {
    canonizer->set_changed(false);
    translator->setFragment_list(translator->getFragment_list()->accept(canonizer));
  }
  fprintf(stdout, "\n \n ----- CANONIZED REPRESENTATION ------ \n \n");
  translator->print_ir();
  return translator;
}

void end_translator() {
  delete translator;
}

Translator::Translator() {
  fragment_list = NULL;
  current_frame = NULL;
  loop_labels = NULL;
  func_labels = NULL;
}

Translator::~Translator() {
  delete fragment_list;
//  delete current_frame;
  delete loop_labels;
  delete func_labels;
}

void Translator::print_ir() {
  Fragment *fragments = getFragment_list();
  while (fragments != NULL) {
    if (Procedure *procedure = dynamic_cast<Procedure *>(fragments)) {
      PrintIRVisitor *printer = new PrintIRVisitor(procedure->get_body());
      delete printer;
    }
    fragments = fragments->get_next();
  }
}

void Translator::add_fragment(Fragment *fragment) {
  fragment->set_next(fragment_list);
  fragment_list = fragment;
}

void Translator::push_loop_label(Label *label) {
  LabelStack *aux = (LabelStack *) malloc(sizeof(LabelStack));
  aux->label = label;
  aux->next = loop_labels;
  loop_labels = aux;
}

Label *Translator::peek_loop_label() {
  return loop_labels != NULL ? loop_labels->label : NULL;
}

void Translator::pop_loop_label() {
  if (loop_labels != NULL) {
    LabelStack *aux = loop_labels;
    loop_labels = loop_labels->next;
    aux->label = NULL;
    aux->next = NULL;
    free(aux);
  }
}

void Translator::push_func_label(Label *label) {
  LabelStack *aux = (LabelStack *) malloc(sizeof(LabelStack));
  aux->label = label;
  aux->next = func_labels;
  func_labels = aux;
}

Label *Translator::peek_func_label() {
  return func_labels != NULL ? func_labels->label : NULL;
}

void Translator::pop_func_label() {
  if (func_labels != NULL) {
    LabelStack *aux = func_labels;
    func_labels = func_labels->next;
    aux->label = NULL;
    aux->next = NULL;
    free(aux);
  }
}

void Translator::visit(ProgramNode *node) {
  if (node->get_class_list() != NULL) node->get_class_list()->accept(this);
  if (node->get_type_list() != NULL) node->get_type_list()->accept(this);
  if (node->get_var_func_list() != NULL) node->get_var_func_list()->accept(this);
}

void Translator::visit(ClassListNode *node) {
  if (node->get_id() != NULL) node->get_id()->accept(this);
  if (node->get_var_func() != NULL) node->get_var_func()->accept(this);
  if (node->get_parent_id() != NULL) node->get_parent_id()->accept(this);
  if (node->get_next() != NULL) node->get_next()->accept(this);
}

void Translator::visit(VarDeclNode *node) {
  if (node->get_id_list() != NULL) {
    IdListNode *id_list = node->get_id_list();
    while (id_list != NULL) {
      VarTable *var_table = dynamic_cast<VarTable *>(get_table(VARIABLE));
      VarSymbol *var;
      if (current_function != NULL)
        var = var_table->lookup_in_scope(id_list->get_id()->get_lexeme(), current_function->get_lexeme());
      if (var == NULL)
        var = var_table->lookup(id_list->get_id()->get_lexeme());
      if (var != NULL) {
        if (var->is_parameter())
          var->setLocal_access(current_frame->add_param(ESCAPE, var->get_offset()));
        else if (strcmp(var->get_scope_lex(), NON_LOCAL_SCOPE) == 0) {
          Variable *variable = new Variable(node->get_type()->get_token(), var->get_size(), var->get_lexeme());
          add_fragment(variable);
        } else
          var->setLocal_access(current_frame->add_local(ESCAPE, var->get_size()));
      }
      id_list = id_list->get_next();
    }
  }

  if (node->get_next() != NULL) node->get_next()->accept(this);
}

void Translator::visit(IdListNode *node) {
  if (node->get_pointer() != NULL) node->get_pointer()->accept(this);
  if (node->get_id() != NULL) node->get_id()->accept(this);
  if (node->get_array() != NULL) node->get_array()->accept(this);
  if (node->get_next() != NULL) node->get_next()->accept(this);
}

void Translator::visit(FunctionDeclNode *node) {
  current_frame = new MIPSFrame();
  FunctionTable *function_table = dynamic_cast<FunctionTable *>(get_table(FUNCTION));
  current_function = function_table->lookup(node->get_id()->get_lexeme());
  current_frame->setCall_size(current_function->getCall_size());

  if (node->get_formal_list() != NULL) node->get_formal_list()->accept(this);

  const char *label_end_format = "FUNCTION_%s_END";
  char func_end[strlen(label_end_format) + strlen(node->get_id()->get_lexeme()) + 1];
  sprintf(func_end, label_end_format, node->get_id()->get_lexeme());
  Label *label_end = new Label(func_end);

  push_func_label(label_end);
  Stmt *stmt = node->getVar_stmt() != NULL ? node->getVar_stmt()->accept(this) : NULL;
  pop_func_label();

  const char *label_begin_format = "FUNCTION_%s";
  char func_begin[strlen(label_begin_format) + strlen(node->get_id()->get_lexeme()) + 1];
  sprintf(func_begin, label_begin_format, node->get_id()->get_lexeme());
  Label *label_begin = new Label(func_begin);

  stmt = new SeqNode(new LabelNode(label_begin), stmt);
  stmt = new SeqNode(stmt, new LabelNode(label_end));

  Procedure *procedure = new Procedure(current_frame, stmt);
  add_fragment(procedure);

  if (node->get_next() != NULL) node->get_next()->accept(this);
}

void Translator::visit(FormalListNode *node) {
  VarTable *var_table = dynamic_cast<VarTable *>(get_table(VARIABLE));
  VarSymbol *var = var_table->lookup_in_scope(node->get_id()->get_lexeme(), current_function->get_lexeme());
  var->setLocal_access(current_frame->add_param(ESCAPE, node->get_id()->get_offset()));

  if (node->get_next() != NULL) node->get_next()->accept(this);
}

Stmt *Translator::visit(StatementListNode *node) {
  if (node->get_statement() != NULL) {
    if (node->get_next() != NULL) {
        ExpNode *exp_node = dynamic_cast<ExpNode *> (node->get_statement());
        if (exp_node != NULL) {
          IRTree *aux = exp_node->accept(this);
          Stmt *stmt = dynamic_cast<Stmt *>(aux);
          Exp *exp = dynamic_cast<Exp *>(aux);
          return new SeqNode(static_cast<Stmt *>(stmt != NULL && exp == NULL ? stmt : new ExpressionNode(exp)),
                             node->get_next()->accept(this));
        } else
          return new SeqNode(node->get_statement()->accept(this),
                             node->get_next()->accept(this));
    } else {
      if (node->get_statement() != NULL) {
        ExpNode *exp_node = dynamic_cast<ExpNode *> (node->get_statement());
        if (exp_node != NULL) {
          IRTree *aux = exp_node->accept(this);
          Stmt *stmt = dynamic_cast<Stmt *>(aux);
          Exp *exp = dynamic_cast<Exp *>(aux);
          return static_cast<Stmt *>(stmt != NULL  && exp == NULL ? stmt : new ExpressionNode(exp));
        } else
          return node->get_statement()->accept(this);
      }
    }
  }
  return NULL;
}

Stmt *Translator::visit(VarStmtNode *node) {
  if (node->get_var_decl() != NULL) node->get_var_decl()->accept(this);
  if (node->getStmt_list() != NULL) return node->getStmt_list()->accept(this);
  if (node->get_next() != NULL) return node->get_next()->accept(this);
  return NULL;
}

Stmt *Translator::visit(VarFuncListNode *node) {
  if (node->get_var_decl() != NULL) {
    if (node->getFunc_list() != NULL)
      return new SeqNode(dynamic_cast<Stmt *>(node->get_var_decl()),
                         dynamic_cast<Stmt *>(node->getFunc_list()->accept(this)));
    else
      return node->get_var_decl()->accept(this);
  } else if (node->getFunc_list() != NULL)
    return dynamic_cast<Stmt *>(node->getFunc_list()->accept(this));
  return NULL;
}

Stmt *Translator::visit(IfNode *node) {
  Label *then_label = new Label();
  Label *end_label = new Label();
  if (node->get_else_stmt() == NULL) {
    SeqNode *seq_stmt = new SeqNode(
        node->get_if_stmt() != NULL ? node->get_if_stmt()->accept(this) : NULL, new
            LabelNode(end_label));
    SeqNode *seq_then = new SeqNode(new LabelNode(then_label), seq_stmt);
    return new SeqNode(new CJumpNode(NOTEQUAL, node->get_exp() != NULL ? dynamic_cast<Exp *>(node->get_exp()->accept
        (this)) : NULL, new ConstNode(0), new NameNode(then_label), new NameNode(end_label)), seq_then);
  } else {
    Label *else_label = new Label();
    SeqNode *seq_else_stmt = new SeqNode(node->get_else_stmt()->accept(this),
                                         new LabelNode(end_label));
    SeqNode *seq_else = new SeqNode(new LabelNode(else_label), seq_else_stmt);
    SeqNode *seq_if_stmt = new SeqNode(
        new SeqNode(node->get_if_stmt() != NULL ? node->get_if_stmt()->accept(this) : NULL,
                    new JumpNode(new NameNode(end_label))), seq_else);
    SeqNode *seq_if = new SeqNode(new LabelNode(then_label), seq_if_stmt);
    return new SeqNode(
        new CJumpNode(NOTEQUAL, node->get_exp() != NULL ? dynamic_cast<Exp *>(node->get_exp()->accept(this)) :
                                NULL,
                      new ConstNode(0), new NameNode(then_label), new NameNode(else_label)), seq_if);
  }
  return NULL;
}

Stmt *Translator::visit(WhileNode *node) {
  const char *label_begin_format = "WHILE_%d";
  char while_begin[strlen(label_begin_format) + 10];
  sprintf(while_begin, label_begin_format, n_while_label);
  Label *label_begin = new Label(while_begin);

  const char *label_end_format = "WHILE_%d_END";
  char while_end[strlen(label_end_format) + 10];
  sprintf(while_end, label_end_format, n_while_label);
  Label *label_end = new Label(while_end);
  n_while_label++;

  Label *label_test = new Label();
  push_loop_label(label_end);

  SeqNode *seq_test = new SeqNode(new JumpNode(new NameNode(label_test)), new LabelNode(label_end));
  SeqNode *seq_stmt = new SeqNode(node->get_statement() != NULL ?
                                  node->get_statement()->accept(this) : NULL, seq_test);
  SeqNode *seq_begin = new SeqNode(new LabelNode(label_begin), seq_stmt);
  SeqNode *seq_cond = new SeqNode(
      new CJumpNode(NOTEQUAL, node->get_exp() != NULL ? dynamic_cast<Exp *>(node->get_exp()->accept(this)) : NULL,
                    new ConstNode(0), new NameNode(label_begin), new NameNode(label_end)), seq_begin);
  pop_loop_label();
  return new SeqNode(new LabelNode(label_test), seq_cond);
}

Stmt *Translator::visit(SwitchNode *node) {
  if (node->get_exp() != NULL) node->get_exp()->accept(this);
  if (node->get_case_block() != NULL) node->get_case_block()->accept(this);
  return NULL;
}

Stmt *Translator::visit(BreakNode *node) {
  Label *label = peek_loop_label();
  return new JumpNode(new NameNode(label), new LabelList(label, NULL));
}

Stmt *Translator::visit(PrintlnNode *node) {
  if (node->get_exp_list() != NULL) {
    ExpListNode *exp_list_node = node->get_exp_list();
    ExpList *exp_list = NULL;
    Stmt *seq = NULL;
    ExpressionNode *exp_node;
    while (exp_list_node != NULL) {
      exp_list = new ExpList(dynamic_cast<Exp *>(exp_list_node->get_exp()->accept(this)), NULL);
      CallNode *call = NULL;
      switch (exp_list_node->get_exp()->get_type()) {
        case INT:
          call = new CallNode(new NameNode(new Label("PRINT_INT")), exp_list);
          break;
        case FLOAT:
          call = new CallNode(new NameNode(new Label("PRINT_REAL")), exp_list);
          break;
        case LITERAL:
          call = new CallNode(new NameNode(new Label("PRINT_LITERAL")), exp_list);
          break;
      }
      exp_node = new ExpressionNode(call);
      if (call == NULL)
        continue;
      call->set_canonized(true);
      if (seq != NULL)
        seq = new SeqNode(seq, exp_node);
      else
        seq = exp_node;
      exp_list_node = exp_list_node->get_next();
    }
    return seq;
  }
  return NULL;
}

Stmt *Translator::visit(ReadNode *node) {
  if (node->get_exp() != NULL) {
    ExpList *exp_list = new ExpList(dynamic_cast<Exp *>(node->get_exp()->accept(this)), NULL);
    CallNode *call = NULL;
    switch (node->get_exp()->get_type()) {
      case INT:
        call = new CallNode(new NameNode(new Label("READLN_INT")), exp_list);
        break;
      case FLOAT:
        call = new CallNode(new NameNode(new Label("READLN_FLOAT")), exp_list);
        break;
    }
    call->set_canonized(true);
    return new MoveNode(dynamic_cast<Exp *>(node->get_exp()->accept(this)), call);
  }
  return NULL;
}

Stmt *Translator::visit(ReturnNode *node) {
  if (node->get_exp() != NULL) {
    Label *func_label = peek_func_label();
    return new SeqNode(new MoveNode(new TempNode(current_frame->getReturn_value()),
                                    dynamic_cast<Exp *>(node->get_exp()->accept(this))),
                       new JumpNode(new NameNode(func_label)));
  }
  return NULL;
}

Stmt *Translator::visit(CaseBlockNode *node) {
  if (node->get_num() != NULL) node->get_num()->accept(this);
  if (node->get_statement_list() != NULL) node->get_statement_list()->accept(this);
  if (node->get_next() != NULL) node->get_next()->accept(this);
  return NULL;
}

Stmt *Translator::visit(ThrowNode *node) {
  return NULL;
}

Stmt *Translator::visit(TryNode *node) {
  if (node->get_try_stmt() != NULL) node->get_try_stmt()->accept(this);
  if (node->get_catch_stmt() != NULL) node->get_catch_stmt()->accept(this);
  return NULL;
}

Stmt *Translator::visit(AssignNode *node) {
  if (node->get_left_exp() != NULL && node->get_right_exp() != NULL)
    return new MoveNode(dynamic_cast<Exp *>(node->get_left_exp()->accept(this)),
                        dynamic_cast<Exp *>(node->get_right_exp()->accept(this)));
  return NULL;
}

Exp *Translator::visit(TypeListNode *node) {
  if (node->get_var_list() != NULL) node->get_var_list()->accept(this);
  if (node->get_id() != NULL) node->get_id()->accept(this);
  if (node->get_next() != NULL) node->get_next()->accept(this);
  return NULL;
}

Exp *Translator::visit(TypeNode *node) {
  return NULL;
}

Exp *Translator::visit(PointerNode *node) {
  return NULL;
}

Exp *Translator::visit(ExpListNode *node) {
  if (node->get_exp() != NULL) {
    if (node->get_next() != NULL)
      return new EseqNode(node->get_exp()->accept(this), node->get_next()->accept(this));
    else
      return dynamic_cast<Exp *>(node->get_exp()->accept(this));
  }
  return NULL;
}

Exp *Translator::visit(IdentifierNode *node) {
  return NULL;
}

Exp *Translator::visit(IntegerNode *node) {
  return new ConstNode(atoi(node->get_lexeme()));
}

Exp *Translator::visit(RealNode *node) {
  return new ConstfNode(atof(node->get_lexeme()));
}

Exp *Translator::visit(LiteralNode *node) {
  Literal *literal = new Literal(node->get_lexeme(), n_literal_label);
  char name[100];
  sprintf(name, "LITERAL_%d", n_literal_label);
  n_literal_label++;
  add_fragment(literal);
  return new NameNode(new Label(name));
}

Exp *Translator::visit(CharacterNode *node) {
  Literal *literal = new Literal(node->get_lexeme(), n_literal_label);
  char name[100];
  sprintf(name, "CHARACTER_%d", n_literal_label);
  n_literal_label++;
  add_fragment(literal);
  return new NameNode(new Label(name));
}

Exp *Translator::visit(PrimaryNode *node) {
  if (node->get_exp() != NULL) {
    if (IdentifierNode *id = dynamic_cast<IdentifierNode *>(node->get_exp())) {
      VarTable *var_table = dynamic_cast<VarTable *>(get_table(VARIABLE));
      VarSymbol *var = var_table->lookup_in_scope(id->get_lexeme(), current_function->get_lexeme());
      if (var != NULL) {
        if (ArrayAccessNode *array_access = dynamic_cast<ArrayAccessNode *>(node->get_next())) {
          BinOpNode *op = new BinOpNode(MULT, new ConstNode(WORD_SIZE), dynamic_cast<Exp *>(array_access->get_index_exp()->accept(this)));
          if (MemNode *mem = dynamic_cast<MemNode *>(var->getLocal_access()->access_code()))
            return new MemNode(new BinOpNode(PLUS, mem->get_exp(), op));
        }
        return var->getLocal_access()->access_code();
      } else { /// non local
        char name[strlen(id->get_lexeme()) + 1];
        if (node->get_next() != NULL && typeid(*node->get_next()) == typeid(FunctionCallNode)) {
          sprintf(name, "FUNCTION_%s", id->get_lexeme());
          return dynamic_cast<Exp *>(node->get_next()->accept(this));
        } else {
          sprintf(name, "VARIABLE_%s", id->get_lexeme());
          Label *label = new Label(name);
          return node->get_array_size() > 0 ? dynamic_cast<Exp *>(new NameNode(label))
                                            : dynamic_cast<Exp *>(new MemNode(
                  new NameNode(label)));
        }
      }

    } else
      return dynamic_cast<Exp *>(node->get_exp()->accept(this));
  }
  return NULL;
}

Exp *Translator::visit(FunctionCallNode *node) {
  if (node->get_id() != NULL) {
    ExpList *exp_list = NULL;
    if (node->get_exp_list() != NULL) {
      node->get_exp_list()->accept(this);
      ExpListNode *exp_list_node = dynamic_cast<ExpListNode *>(node->get_exp_list());
      while (exp_list_node != NULL) {
        Exp *exp = dynamic_cast<Exp *>(exp_list_node->get_exp()->accept(this));
        if (exp_list_node->get_exp()->get_array_size() > 0) {
          PrimaryNode *primary = dynamic_cast<PrimaryNode *>(exp_list_node->get_exp());
          if (IdentifierNode *id = dynamic_cast<IdentifierNode *>(primary->get_exp())) {
            VarSymbol *var = ((VarTable *) get_table(VARIABLE))->lookup_in_scope(id->get_lexeme(),
                                                                                 current_function->get_lexeme());
            if (var == NULL) {
              if (MemNode *mem = dynamic_cast<MemNode *>(exp))
                exp = mem->get_exp();
            }
          }
        }
        exp_list = new ExpList(exp, exp_list);
        exp_list_node = exp_list_node->get_next();
      }
    }
    char name[200];
    sprintf(name, "FUNCTION_%s", node->get_id()->get_lexeme());
    return new CallNode(new NameNode(new Label(name)), exp_list);
  }
  if (node->get_next() != NULL) node->get_next()->accept(this);
}

Exp *Translator::visit(FieldAccessNode *node) {
  StructTable *struct_table = dynamic_cast<StructTable *>(get_table(STRUCT));
  VarTable *var_table = dynamic_cast<VarTable *>(get_table(VARIABLE));
  StructSymbol *struct_symbol = struct_table->lookup(node->get_exp()->getType_lex());
  IdentifierNode *id = dynamic_cast<IdentifierNode *>(node->get_exp());
  VarSymbol *var = var_table->lookup_in_scope(id->get_lexeme(), struct_symbol->get_lexeme());
  Exp *exp = dynamic_cast<Exp *>(node->get_exp()->accept(this));
  MemNode *mem = dynamic_cast<MemNode *>(exp);
  if (mem != NULL)
    exp = mem->get_exp();
  return new MemNode(new BinOpNode(PLUS, exp, new ConstNode(var->get_offset())));
}

Exp *Translator::visit(PointerAccessNode *node) {
  StructTable *struct_table = dynamic_cast<StructTable *>(get_table(STRUCT));
  VarTable *var_table = dynamic_cast<VarTable *>(get_table(VARIABLE));
  StructSymbol *struct_symbol = struct_table->lookup(node->get_exp()->getType_lex());
  IdentifierNode *id = dynamic_cast<IdentifierNode *>(node->get_exp());
  VarSymbol *var = var_table->lookup_in_scope(id->get_lexeme(), struct_symbol->get_lexeme());
  return new MemNode(
      new BinOpNode(PLUS, dynamic_cast<Exp *>(node->get_exp()->accept(this)), new ConstNode(var->get_offset())));
}

Exp *Translator::visit(AddressValueNode *node) {
  if (node->get_exp() != NULL) {
    Exp *exp = dynamic_cast<Exp *>(node->get_exp()->accept(this));
    MemNode *mem = dynamic_cast<MemNode *>(node->get_exp()->accept(this));
    if (mem != NULL)
      exp = mem->get_exp();
    return exp;
  }
  return NULL;
}

Exp *Translator::visit(PointerValueNode *node) {
  if (node->get_exp() != NULL)
    return new MemNode(dynamic_cast<Exp *>(node->get_exp()->accept(this)));
  return NULL;
}

Exp *Translator::visit(ArrayAccessNode *node) {
  // processed in PrimaryNode
  return NULL;
}

Exp *Translator::visit(ArrayDeclNode *node) {
  if (node->get_index_exp() != NULL) node->get_index_exp()->accept(this);
  return NULL;
}

Exp *Translator::visit(RelationalOpNode *node) {
  LocalAccess *local = this->current_frame->add_local(ESCAPE, WORD_SIZE);
  Label *true_label = new Label();
  Label *false_label = new Label();
  int op = node->get_op();
  if (node->get_left_exp() != NULL && node->get_right_exp() != NULL) {
    SeqNode *move_false_seq = new SeqNode(new MoveNode(local->access_code(), new ConstNode(0)),
                                          new LabelNode(true_label));
    SeqNode *false_seq = new SeqNode(new LabelNode(false_label), move_false_seq);
    SeqNode *cond_seq = new SeqNode(new CJumpNode(op, dynamic_cast<Exp *>(node->get_left_exp()->accept(this)),
                                                  dynamic_cast<Exp *>(node->get_right_exp()->accept(this)),
                                                  new NameNode(true_label), new NameNode(false_label)), false_seq);
    SeqNode *move_true_seq = new SeqNode(new MoveNode(local->access_code(), new ConstNode(1)), cond_seq);
    return new EseqNode(move_true_seq, local->access_code());
  }
  return NULL;
}

Exp *Translator::visit(AdditionOpNode *node) {
  if (node->get_left_exp() != NULL && node->get_right_exp() != NULL) {
    LocalAccess *local = current_frame->add_local(ESCAPE, WORD_SIZE);
    MoveNode *move = new MoveNode(local->access_code(), new BinOpNode(node->get_op(),
                                                                      dynamic_cast<Exp *>(node->get_left_exp()->accept(
                                                                          this)),
                                                                      dynamic_cast<Exp *>(node->get_right_exp()->accept(
                                                                          this))));
    return new EseqNode(move, local->access_code());
  }
  return NULL;
}

Exp *Translator::visit(MultiplicationOpNode *node) {
  if (node->get_left_exp() != NULL && node->get_right_exp() != NULL) {
    LocalAccess *local = current_frame->add_local(ESCAPE, WORD_SIZE);
    MoveNode *move = new MoveNode(local->access_code(), new BinOpNode(node->get_op(),
                                                                      dynamic_cast<Exp *>(node->get_left_exp()->accept(
                                                                          this)),
                                                                      dynamic_cast<Exp *>(node->get_right_exp()->accept(
                                                                          this))));
    return new EseqNode(move, local->access_code());
  }
  return NULL;
}

Exp *Translator::visit(BooleanOpNode *node) {
  LocalAccess *local = current_frame->add_local(ESCAPE, WORD_SIZE);
  Label *true_label = new Label();
  Label *false_label = new Label();
  int op = node->get_op();
  if (node->get_left_exp() != NULL && node->get_right_exp() != NULL) {
    if (op == AND) {
      Label *true2_label = new Label();
      SeqNode *seq1 = new SeqNode(new MoveNode(local->access_code(), new ConstNode(1)), new LabelNode(false_label));
      SeqNode *seq2 = new SeqNode(new LabelNode(true2_label), seq1);
      SeqNode *seq3 = new SeqNode(new CJumpNode(NOTEQUAL, dynamic_cast<Exp *>(node->get_right_exp()->accept(this)),
                                                new ConstNode(0), new NameNode(true2_label), new NameNode(false_label)),
                                  seq2);
      SeqNode *seq4 = new SeqNode(new LabelNode(true_label), seq3);
      SeqNode *seq5 = new SeqNode(new CJumpNode(NOTEQUAL, dynamic_cast<Exp *>(node->get_left_exp()->accept(this)), new
          ConstNode(1), new NameNode(true_label), new NameNode(false_label)), seq4);
      SeqNode *seq6 = new SeqNode(new MoveNode(local->access_code(), new ConstNode(0)), seq5);
      return new EseqNode(seq6, local->access_code());
    } else if (op == ALTERNATIVE) {
      SeqNode *seq1 = new SeqNode(new MoveNode(local->access_code(), new ConstNode(1)), new LabelNode(false_label));
      SeqNode *seq2 = new SeqNode(new LabelNode(true_label), seq1);
      SeqNode *seq3 = new SeqNode(new CJumpNode(NOTEQUAL, dynamic_cast<Exp *>(node->get_right_exp()->accept(this)),
                                                new ConstNode(0), new NameNode(true_label), new NameNode(false_label)),
                                  seq2);
      SeqNode *seq4 = new SeqNode(new MoveNode(local->access_code(),
                                               dynamic_cast<Exp *>(node->get_left_exp()->accept(this))), seq3);
      return new EseqNode(seq4, local->access_code());
    }
  }
  return NULL;
}

Exp *Translator::visit(BitwiseOpNode *node) {
  if (node->get_left_exp() != NULL) node->get_left_exp()->accept(this);
  if (node->get_right_exp() != NULL) node->get_right_exp()->accept(this);
  return NULL;
}

Exp *Translator::visit(NewNode *node) {
  if (node->get_id() != NULL) node->get_id()->accept(this);
  if (node->get_exp_list() != NULL) node->get_exp_list()->accept(this);
  return NULL;
}

Exp *Translator::visit(NotNode *node) {
  if (node->get_exp() != NULL) {
    LocalAccess *local = current_frame->add_local(ESCAPE, WORD_SIZE);
    Label *true_label = new Label();
    Label *false_label = new Label();
    SeqNode *seq1 = new SeqNode(new MoveNode(local->access_code(), new ConstNode(0)), new LabelNode(true_label));
    SeqNode *seq2 = new SeqNode(new LabelNode(false_label), seq1);
    SeqNode *seq3 = new SeqNode(
        new CJumpNode(EQUAL, dynamic_cast<Exp *>(node->get_exp()->accept(this)), new ConstNode(0),
                      new NameNode(true_label), new NameNode(false_label)), seq2);
    SeqNode *seq4 = new SeqNode(new MoveNode(local->access_code(), new ConstNode(1)), seq3);
    return new EseqNode(seq4, local->access_code());
  }
  return NULL;
}

Exp *Translator::visit(SignNode *node) {
  if (node->get_exp() != NULL) {
    LocalAccess *local = current_frame->add_local(ESCAPE, WORD_SIZE);
    MoveNode *move = new MoveNode(local->access_code(), new BinOpNode(MULT, new ConstNode(-1),
                                                                      dynamic_cast<Exp *>(node->get_exp()->accept(
                                                                          this))));
    return new EseqNode(move, local->access_code());
  }
  return NULL;
}

Exp *Translator::visit(TrueNode *node) {
  return new ConstNode(1);
}

Exp *Translator::visit(FalseNode *node) {
  return new ConstNode(0);
}

Exp *Translator::visit(ThisNode *node) {
  return NULL;
}

void Translator::setFragment_list(Fragment *fragment_list) {
  this->fragment_list = fragment_list;
}
