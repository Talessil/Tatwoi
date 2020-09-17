#include <iostream>
#include "ir_visitor.h"
#include "lex_analyzer.h"
#include "translator.h"

PrintIRVisitor::PrintIRVisitor(IRTree *node) {
  indentation = 0;
  if (node != NULL) node->accept(this);
}

/**
 * Increases the indentation used for printing by 2.
 */
void PrintIRVisitor::increase_indent() { this->indentation += 2; }


/**
 * Decreases the indentation used for printing by 2.
 */
void PrintIRVisitor::decrease_indent() { this->indentation -= 2; }

/**
 * Prints white spaces used for indentation.
 */
void PrintIRVisitor::indent() {
  for (unsigned int i = 0; i < this->indentation; i++) std::cout << " ";
}

/**
 * Prints a string with the correct indentation.
 */
void PrintIRVisitor::print(const char *s) {
  indent();
  std::cout << "-" << s << std::endl;
}

PrintIRVisitor::~PrintIRVisitor() {

}

void PrintIRVisitor::visit(IRTree *node) {
  if (node != NULL) node->accept(this);
}

void PrintIRVisitor::visit(Exp *node) {
  if (node != NULL) node->accept(this);
}

void PrintIRVisitor::visit(ExpList *node) {
  print("ExpList");
  increase_indent();
  if (node->get_exp() != NULL) node->get_exp()->accept(this);
  if (node->get_next() != NULL) node->get_next()->accept(this);
  decrease_indent();
}

void PrintIRVisitor::visit(ConstNode *node) {
  indent();
  std::cout << "-ConstNode." << node->get_value() << std::endl;
}

void PrintIRVisitor::visit(ConstfNode *node) {
  indent();
  std::cout << "-ConstfNode." << node->get_value() << std::endl;
}

void PrintIRVisitor::visit(NameNode *node) {
  if (node->get_label() != NULL) {
    indent();
    std::cout << "-NameNode." << node->get_label()->to_string() << std::endl;
  }
}

void PrintIRVisitor::visit(TempNode *node) {
  if (node->get_temp() != NULL) {
    indent();
    std::cout << "-TempNode." << node->get_temp()->to_string() << std::endl;
  }
}

void PrintIRVisitor::visit(BinOpNode *node) {
  indent();
  std::cout << "-BinOpNode." << get_token_name(node->getToken_op()) << std::endl;
  increase_indent();
  if (node->get_exp1() != NULL) node->get_exp1()->accept(this);
  if (node->get_exp2() != NULL) node->get_exp2()->accept(this);
  decrease_indent();
}

void PrintIRVisitor::visit(MemNode *node) {
  print("MemNode");
  increase_indent();
  if (node->get_exp() != NULL) node->get_exp()->accept(this);
  decrease_indent();
}

void PrintIRVisitor::visit(CallNode *node) {
  print("CallNode");
  increase_indent();
  if (node->get_function() != NULL) node->get_function()->accept(this);
  if (node->get_arguments() != NULL) node->get_arguments()->accept(this);
  decrease_indent();
}

void PrintIRVisitor::visit(EseqNode *node) {
  print("EseqNode");
  increase_indent();
  if (node->get_stmt() != NULL) node->get_stmt()->accept(this);
  if (node->get_exp() != NULL) node->get_exp()->accept(this);
  decrease_indent();
}

void PrintIRVisitor::visit(Stmt *node) {
  if (node != NULL) node->accept(this);
}

void PrintIRVisitor::visit(StmtList *node) {
  print("StmtList");
  increase_indent();
  if (node->get_stmt() != NULL) node->get_stmt()->accept(this);
  if (node->get_next() != NULL) node->get_next()->accept(this);
  decrease_indent();
}

void PrintIRVisitor::visit(MoveNode *node) {
  print("MoveNode");
  increase_indent();
  if (node->get_destiny() != NULL) node->get_destiny()->accept(this);
  if (node->get_source() != NULL) node->get_source()->accept(this);
  decrease_indent();
}

void PrintIRVisitor::visit(ExpressionNode *node) {
  print("ExpressionNode");
  increase_indent();
  if (node->get_exp() != NULL) node->get_exp()->accept(this);
  decrease_indent();
}

void PrintIRVisitor::visit(JumpNode *node) {
  print("JumpNode");
  increase_indent();
  if (node->get_destiny() != NULL) node->get_destiny()->accept(this);
//  if (node->getLabel_list() != NULL) node->getLabel_list()->accept(this);
  decrease_indent();
}

void PrintIRVisitor::visit(CJumpNode *node) {
  indent();
  std::cout << "-CJumpNode.Op." << get_token_name(node->getToken_op()) << std::endl;
  increase_indent();
  if (node->get_exp1() != NULL) node->get_exp1()->accept(this);
  if (node->get_exp2() != NULL) node->get_exp2()->accept(this);
  if (node->getCond_true() != NULL) node->getCond_true()->accept(this);
  if (node->getCond_false() != NULL) node->getCond_false()->accept(this);
  decrease_indent();
}

void PrintIRVisitor::visit(SeqNode *node) {
  print("SeqNode");
  increase_indent();
  if (node->get_stmt1() != NULL) node->get_stmt1()->accept(this);
  if (node->get_stmt2() != NULL) node->get_stmt2()->accept(this);
  decrease_indent();
}

void PrintIRVisitor::visit(LabelNode *node) {
  if (node->get_label() != NULL) {
    indent();
    std::cout << "-LabelNode." << node->get_label()->to_string() << std::endl;
  }
}

IRVisitor::IRVisitor() {}

IRVisitor::~IRVisitor() {}

/////////////////// Canonizer //////////////////////////////////

MIPSFrame *current_frame;

Canonizer::Canonizer() { changed = true; }

Canonizer::~Canonizer() {

}

Fragment *Canonizer::visit(Fragment *fragment) {
  if (fragment->get_next() != NULL) fragment->set_next(fragment->get_next()->accept(this));
  return fragment;
}

Procedure *Canonizer::visit(Procedure *fragment) {
  current_frame = dynamic_cast<MIPSFrame *>(fragment->get_frame());
  if (fragment->get_body() != NULL) fragment->set_body(fragment->get_body()->accept(this));
  if (fragment->get_next() != NULL) fragment->set_next(fragment->get_next()->accept(this));
  return fragment;
}

Literal *Canonizer::visit(Literal *fragment) {
  if (fragment->get_next() != NULL) fragment->set_next(fragment->get_next()->accept(this));
  return fragment;
}

Variable *Canonizer::visit(Variable *fragment) {
  if (fragment->get_next() != NULL) fragment->set_next(fragment->get_next()->accept(this));
  return fragment;
}

Exp *Canonizer::visit(Exp *node) {
  node = node->accept(this);
  return node;
}

Exp *Canonizer::visit(ExpList *node) {
  if (node->get_next() != NULL) node->set_next(dynamic_cast<ExpList *>(node->get_next()->accept(this)));
  if (node->get_exp() != NULL) node->set_exp(node->get_exp()->accept(this));
  return node;
}

Exp *Canonizer::visit(ConstNode *node) {
  return node;
}

Exp *Canonizer::visit(ConstfNode *node) {
  return node;
}

Exp *Canonizer::visit(NameNode *node) {
  return node;
}

Exp *Canonizer::visit(TempNode *node) {
  return node;
}

Exp *Canonizer::visit(BinOpNode *node) {
  EseqNode *eseq = dynamic_cast<EseqNode *>(node->get_exp1());
  if (eseq != NULL) {
    changed = true;
    BinOpNode *bin_op = new BinOpNode(node->getToken_op(), eseq->get_exp()->accept(this),
                                      node->get_exp2()->accept(this));
    return new EseqNode(eseq->get_stmt()->accept(this), bin_op);
  }
  eseq = dynamic_cast<EseqNode *>(node->get_exp2());
  if (eseq != NULL) {
    LocalAccess *local = current_frame->add_local(ESCAPE, WORD_SIZE);
    MoveNode *move = new MoveNode(local->access_code(), node->get_exp1()->accept(this));
    BinOpNode *bin_op = new BinOpNode(node->getToken_op(), local->access_code(), eseq->get_exp()->accept(this));
    EseqNode *eseq2 = new EseqNode(eseq->get_stmt()->accept(this), bin_op);
    return new EseqNode(move, eseq2);
  } else {
    if (node->get_exp1() != NULL) node->set_exp1(node->get_exp1()->accept(this));
    if (node->get_exp2() != NULL) node->set_exp2(node->get_exp2()->accept(this));
  }
  if (node->get_exp1() && node->get_exp2()) {
    ConstNode *const1 = dynamic_cast<ConstNode *>(node->get_exp1());
    ConstNode *const2 = dynamic_cast<ConstNode *>(node->get_exp2());
    if (const1 != NULL && const2 != NULL) {
      int value;
      switch (node->getToken_op()) {
        case PLUS:
          value = const1->get_value() + const2->get_value();
          return new ConstNode(value);
        case MINUS:
          value = const1->get_value() - const2->get_value();
          return new ConstNode(value);
        case MULT:
          value = const1->get_value() * const2->get_value();
          return new ConstNode(value);
        case DIV:
          value = const1->get_value() / const2->get_value();
          return new ConstNode(value);
      }
    }
  }
  return node;
}

Exp *Canonizer::visit(MemNode *node) {
  EseqNode *eseq = dynamic_cast<EseqNode *>(node->get_exp());
  if (eseq != NULL) {
    changed = true;
    MemNode *mem = new MemNode(eseq->get_exp()->accept(this));
    return new EseqNode(eseq->get_stmt()->accept(this), mem);
  } else
    node->set_exp(node->get_exp()->accept(this));
  return node;
}

Exp *Canonizer::visit(CallNode *node) {
  if (!node->is_canonized()) {
    changed = true;
    node->set_function(node->get_function()->accept(this));
    if (node->get_arguments() != NULL)
      node->set_arguments(dynamic_cast<ExpList *>(node->get_arguments()->accept(this)));
    node->set_canonized(true);
    LocalAccess *local = current_frame->add_local(ESCAPE, WORD_SIZE);
    MoveNode *move = new MoveNode(local->access_code(), node);
    return new EseqNode(move, local->access_code());
  }
  node->set_function(node->get_function()->accept(this));
  if (node->get_arguments() != NULL) {
    ExpList *list = node->get_arguments();
    while (list != NULL) {
      EseqNode *eseq = dynamic_cast<EseqNode *>(list->get_exp());
      if (eseq != NULL) {
        changed = true;
        list->set_exp(eseq->get_exp()->accept(this));
        return new EseqNode(eseq->get_stmt(), node);
      }
      list = list->get_next();
    }
    node->set_arguments(dynamic_cast<ExpList *>(node->get_arguments()->accept(this)));
  }
  return node;
}

Exp *Canonizer::visit(EseqNode *node) {
  EseqNode *eseq = dynamic_cast<EseqNode *>(node->get_exp());
  if (eseq != NULL) {
    changed = true;
    return new EseqNode(new SeqNode(node->get_stmt()->accept(this), eseq->get_stmt()->accept(this)), eseq->get_exp()
            ->accept(this));
  } else {
    node->set_stmt(node->get_stmt()->accept(this));
    node->set_exp(node->get_exp()->accept(this));
  }
  return node;
}

Stmt *Canonizer::visit(Stmt *node) {
  node = node->accept(this);
  return node;
}

Stmt *Canonizer::visit(StmtList *node) {
  if (node->get_next() != NULL) node->set_stmt(node->get_next()->accept(this));
  if (node->get_stmt() != NULL) node->set_stmt(node->get_stmt()->accept(this));
  return node;
}

Stmt *Canonizer::visit(MoveNode *node) {
  EseqNode *eseq = dynamic_cast<EseqNode *>(node->get_source());
  if (eseq != NULL) {
    changed = true;
    return new SeqNode(eseq->get_stmt()->accept(this),
                       new MoveNode(node->get_destiny()->accept(this), eseq->get_exp()->accept(this)));
  }
  eseq = dynamic_cast<EseqNode *>(node->get_destiny());
  if (eseq != NULL) {
    changed = true;
    return new SeqNode(eseq->get_stmt()->accept(this),
                       new MoveNode(eseq->get_exp()->accept(this), node->get_source()->accept(this)));
  } else {
    node->set_source(node->get_source()->accept(this));
    node->set_destiny(node->get_destiny()->accept(this));
  }
  return node;
}

Stmt *Canonizer::visit(ExpressionNode *node) {
  EseqNode *eseq = dynamic_cast<EseqNode *>(node->get_exp());
  if (eseq != NULL) {
    changed = true;
    return new SeqNode(eseq->get_stmt(), new ExpressionNode(eseq->get_exp()));
  }
  node->set_exp(node->get_exp()->accept(this));
  return node;
}

Stmt *Canonizer::visit(JumpNode *node) {
  EseqNode *eseq = dynamic_cast<EseqNode *>(node->get_destiny());
  if (eseq != NULL) {
    changed = true;
    return new SeqNode(eseq->get_stmt()->accept(this), new JumpNode(eseq->get_exp()->accept(this)));
  } else
    node->set_destiny(node->get_destiny()->accept(this));
  return node;
}

Stmt *Canonizer::visit(CJumpNode *node) {
  EseqNode *eseq = dynamic_cast<EseqNode *>(node->get_exp1());
  if (eseq != NULL) {
    changed = true;
    CJumpNode *cjump = new CJumpNode(node->getToken_op(), eseq->get_exp()->accept(this),
                                     node->get_exp2()->accept(this),
                                     node->getCond_true()->accept(this), node->getCond_false()->accept(this));
    return new SeqNode(eseq->get_stmt()->accept(this), cjump);
  }
  eseq = dynamic_cast<EseqNode *>(node->get_exp2());
  if (eseq != NULL) {
    LocalAccess *local = current_frame->add_local(ESCAPE, WORD_SIZE);
    MoveNode *move = new MoveNode(local->access_code(), node->get_exp1()->accept(this));
    CJumpNode *cjmp = new CJumpNode(node->getToken_op(), local->access_code(), eseq->get_exp()->accept(this),
                                        node->getCond_true()->accept(this), node->getCond_false()->accept(this));
    SeqNode *seq = new SeqNode(eseq->get_stmt()->accept(this), cjmp);
    return new SeqNode(move, seq);
  } else {
    node->set_exp1(node->get_exp1()->accept(this));
    node->set_exp2(node->get_exp2()->accept(this));
    node->setCond_true(node->getCond_true()->accept(this));
    node->setCond_false(node->getCond_false()->accept(this));
  }
  return node;
}

Stmt *Canonizer::visit(SeqNode *node) {
  SeqNode *seq = dynamic_cast<SeqNode *>(node->get_stmt1());
  CJumpNode *cjump1 = dynamic_cast<CJumpNode *>(node->get_stmt1());
  CJumpNode *cjump2 = dynamic_cast<CJumpNode *>(node->get_stmt2());
  if (seq != NULL) {
    changed = true;
    return new SeqNode(seq->get_stmt1()->accept(this),
                       new SeqNode(seq->get_stmt2()->accept(this), node->get_stmt2()->accept(this)));
  }

  if (cjump1 && !cjump1->is_canonized()) {
    changed = true;
    Label *label = new Label();
    SeqNode *next = new SeqNode(new LabelNode(label), new JumpNode(cjump1->getCond_false()));
    cjump1->setCond_false(new NameNode(label));
    cjump1->set_canonized(true);
    SeqNode *seq1 = new SeqNode(cjump1, next);
    node->set_stmt1(seq1);
  }

  if (cjump2 && !cjump2->is_canonized()) {
    changed = true;
    Label *label = new Label();
    SeqNode *next = new SeqNode(new LabelNode(label), new JumpNode(cjump2->getCond_false()));
    cjump2->setCond_false(new NameNode(label));
    cjump2->set_canonized(true);
    SeqNode *seq2 = new SeqNode(cjump2, next);
    node->set_stmt2(seq2);
  }
  if (node->get_stmt1() != NULL) node->set_stmt1(node->get_stmt1()->accept(this));
  if (node->get_stmt2() != NULL) node->set_stmt2(node->get_stmt2()->accept(this));
  return node;
}

Stmt *Canonizer::visit(LabelNode *node) {
  return node;
}

bool Canonizer::is_changed() {
  return changed;
}

void Canonizer::set_changed(bool changed) {
  this->changed = changed;
}
