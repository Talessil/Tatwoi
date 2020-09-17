#include <stdio.h>
#include <string.h>
#include "generate_assembly.h"
#include "translator.h"
#include "lex_analyzer.h"


Temp zero("$0"); ///default register for the '0' value
Temp *sp = new Temp("$sp");
int num_reg = 20;
AssemblyGenerator *generator;

///GET A FREE REG
Temp *AssemblyGenerator::get_free_reg() {
  if (reg_queue->get_size()) {
    Temp *t = (Temp *) reg_queue->dequeue();
    return t;
  } else {
    fprintf(stderr, "without sufficient registers");
    return NULL;
  }
}

///SET A REG AS FREE
void AssemblyGenerator::set_free_reg(Temp *temp) {
  if (strcmp(temp->get_name(), "$fp") && strcmp(temp->get_name(), "$v0")
      && strcmp(temp->get_name(), "$0") && strcmp(temp->get_name(), "$a0")
      && strcmp(temp->get_name(), "$a1") && strcmp(temp->get_name(), "$a2")
      && strcmp(temp->get_name(), "$a3") && strcmp(temp->get_name(), "$sp")) {
    reg_queue->enqueue(temp);
  }
}

AssemblyGenerator::AssemblyGenerator(const char *file_name) {
  asm_file = fopen(file_name, "w+");
  reg_queue = new Queue();
  reg_queue->enqueue(new Temp("$t1"));
  reg_queue->enqueue(new Temp("$t2"));
  reg_queue->enqueue(new Temp("$t3"));
  reg_queue->enqueue(new Temp("$t4"));
  reg_queue->enqueue(new Temp("$t5"));
  reg_queue->enqueue(new Temp("$t6"));
  reg_queue->enqueue(new Temp("$t7"));
  reg_queue->enqueue(new Temp("$t8"));
  reg_queue->enqueue(new Temp("$t9"));
  reg_queue->enqueue(new Temp("$s0"));
  reg_queue->enqueue(new Temp("$s1"));
  reg_queue->enqueue(new Temp("$s2"));
  reg_queue->enqueue(new Temp("$s3"));
  reg_queue->enqueue(new Temp("$s4"));
  reg_queue->enqueue(new Temp("$s5"));
  reg_queue->enqueue(new Temp("$s6"));
  reg_queue->enqueue(new Temp("$s7"));
}

AssemblyGenerator::~AssemblyGenerator() {
  delete reg_queue;
  fclose(asm_file);
  asm_file = NULL;
}

///SAVE ALL REG
void AssemblyGenerator::save_reg(int offset) {
  fprintf(asm_file, "sw $fp,%d($sp)\n", offset);
  offset = offset + 4;
  fprintf(asm_file, "sw $t0,%d($sp)\n", offset);
  offset = offset + 4;
  fprintf(asm_file, "sw $t1,%d($sp)\n", offset);
  offset = offset + 4;
  fprintf(asm_file, "sw $t2,%d($sp)\n", offset);
  offset = offset + 4;
  fprintf(asm_file, "sw $t3,%d($sp)\n", offset);
  offset = offset + 4;
  fprintf(asm_file, "sw $t4,%d($sp)\n", offset);
  offset = offset + 4;
  fprintf(asm_file, "sw $t5,%d($sp)\n", offset);
  offset = offset + 4;
  fprintf(asm_file, "sw $t6,%d($sp)\n", offset);
  offset = offset + 4;
  fprintf(asm_file, "sw $t7,%d($sp)\n", offset);
  offset = offset + 4;
  fprintf(asm_file, "sw $t8,%d($sp)\n", offset);
  offset = offset + 4;
  fprintf(asm_file, "sw $t9,%d($sp)\n", offset);
  offset = offset + 4;
  fprintf(asm_file, "sw $s0,%d($sp)\n", offset);
  offset = offset + 4;
  fprintf(asm_file, "sw $s1,%d($sp)\n", offset);
  offset = offset + 4;
  fprintf(asm_file, "sw $s2,%d($sp)\n", offset);
  offset = offset + 4;
  fprintf(asm_file, "sw $s3,%d($sp)\n", offset);
  offset = offset + 4;
  fprintf(asm_file, "sw $s4,%d($sp)\n", offset);
  offset = offset + 4;
  fprintf(asm_file, "sw $s5,%d($sp)\n", offset);
  offset = offset + 4;
  fprintf(asm_file, "sw $s6,%d($sp)\n", offset);
  offset = offset + 4;
  fprintf(asm_file, "sw $s7,%d($sp)\n", offset);
  offset = offset + 4;
  fprintf(asm_file, "sw $ra,%d($sp)\n", offset);
}

///RECOVER ALL REG
void AssemblyGenerator::recover_reg(int offset) {
  fprintf(asm_file, "lw $fp,%d($sp)\n", offset);
  offset = offset + 4;
  fprintf(asm_file, "lw $t0,%d($sp)\n", offset);
  offset = offset + 4;
  fprintf(asm_file, "lw $t1,%d($sp)\n", offset);
  offset = offset + 4;
  fprintf(asm_file, "lw $t2,%d($sp)\n", offset);
  offset = offset + 4;
  fprintf(asm_file, "lw $t3,%d($sp)\n", offset);
  offset = offset + 4;
  fprintf(asm_file, "lw $t4,%d($sp)\n", offset);
  offset = offset + 4;
  fprintf(asm_file, "lw $t5,%d($sp)\n", offset);
  offset = offset + 4;
  fprintf(asm_file, "lw $t6,%d($sp)\n", offset);
  offset = offset + 4;
  fprintf(asm_file, "lw $t7,%d($sp)\n", offset);
  offset = offset + 4;
  fprintf(asm_file, "lw $t8,%d($sp)\n", offset);
  offset = offset + 4;
  fprintf(asm_file, "lw $t9,%d($sp)\n", offset);
  offset = offset + 4;
  fprintf(asm_file, "lw $s0,%d($sp)\n", offset);
  offset = offset + 4;
  fprintf(asm_file, "lw $s1,%d($sp)\n", offset);
  offset = offset + 4;
  fprintf(asm_file, "lw $s2,%d($sp)\n", offset);
  offset = offset + 4;
  fprintf(asm_file, "lw $s3,%d($sp)\n", offset);
  offset = offset + 4;
  fprintf(asm_file, "lw $s4,%d($sp)\n", offset);
  offset = offset + 4;
  fprintf(asm_file, "lw $s5,%d($sp)\n", offset);
  offset = offset + 4;
  fprintf(asm_file, "lw $s6,%d($sp)\n", offset);
  offset = offset + 4;
  fprintf(asm_file, "lw $s7,%d($sp)\n", offset);
  offset = offset + 4;
  fprintf(asm_file, "lw $ra,%d($sp)\n", offset);
}



//***************************************************************************************************
//***************************************************************************************************
//*************************************ASSEMBLY GENERATOR *******************************************
//***************************************************************************************************
//***************************************************************************************************

///At the beginning of each procedure, the stack pointer is modified,
///the frame pointer is saved from the calling function of the procedure
///in question, after saving the registers the body of the procedure is visitd.
///After writing the assembly relative to the body of the function,
///it recovers the frame pointer of the calling function and returns to the address where the call of the function occurred
void AssemblyGenerator::visit(Procedure *node) {
  fprintf(asm_file, ".text\n");
  MIPSFrame *frame = (MIPSFrame *) node->get_frame();

  int regs_size = WORD_SIZE * num_reg;
  SeqNode *seq = dynamic_cast<SeqNode *>(node->get_body());
  LabelNode *label = dynamic_cast<LabelNode *>(seq->get_stmt1());
//  label->accept(this);
  seq->get_stmt1()->accept(this);

  int total_size =
          -frame->getFrame_size() + frame->getCall_size() +
          regs_size; ///aloca todos os registradores e par�metros passados
  fprintf(asm_file, "subu $sp,$sp,%d\n", total_size);  ///SUBU - UNSIGNED INTEGER SUBTRACTION INSTRUCTION
  save_reg(frame->getCall_size());  ///aloca todos os registradores
  fprintf(asm_file, "addu $fp,$sp,%d\n", total_size);  ///ADDU - UNSIGNED INTEGER ADDITION INSTRUCTION

  seq->get_stmt2()->accept(this);
  recover_reg(frame->getCall_size());  ///desaloca todos os registradores
  fprintf(asm_file, "addu $sp,%d\n", total_size);
  fprintf(asm_file, "jr $ra\n");
}

///Write the definition of a literal in the output file, using the command .rdata,
///followed by the label of the literal, defined according to the number of the literal.
///Finally, the .asciiz command is added, followed by the literal value
///RDATA - reallocable data
///ASCIIZ - literal string terminated with 0
void AssemblyGenerator::visit(Literal *node) {
  fprintf(asm_file, ".rdata\nLITERAL_%d:\n.asciiz \"%s\"\n", node->getLabel_num(), node->get_value());
}

///The .data command is added to the output file, followed by the variable label (created from the variable name)
///and the .space command, with the size of the variable
void AssemblyGenerator::visit(Variable *node) {
  fprintf(asm_file, ".data\n");
  fprintf(asm_file, "VARIABLE_%s: .space %d\n", node->get_name(), node->get_nbytes());
}

///This node is translated with an lw, where the value to be loaded is transferred to an available register and then returned;
Temp *AssemblyGenerator::visit(MemNode *node) { ///ENTENDER
  Temp *temp = get_free_reg();
  BinOpNode *bin = dynamic_cast<BinOpNode *>(node->get_exp());
  if (bin != NULL && bin->getToken_op() == PLUS) {
    ConstNode *const1 = dynamic_cast<ConstNode *>(bin->get_exp1());
    ConstNode *const2 = dynamic_cast<ConstNode *>(bin->get_exp2());
    if (const1 != NULL) {
      Temp *t = bin->get_exp2()->accept(this);
      fprintf(asm_file, "lw %s, %d(%s)\n", temp->get_name(), const1->get_value(),
              t->get_name()); ///carrega valor no
/// temp d
      set_free_reg(t);
    } else if (const2 != NULL) {
      Temp *t = bin->get_exp1()->accept(this);
      fprintf(asm_file, "lw %s, %d(%s)\n", temp->get_name(), const2->get_value(),
              t->get_name()); ///carrega valor no
/// temp d
      set_free_reg(t);
    } else {
      Temp *t = bin->accept(this);
      fprintf(asm_file, "lw %s, 0(%s)\n", temp->get_name(), t->get_name());
      set_free_reg(t);
    }
  } else {
    NameNode *name = dynamic_cast<NameNode *>(node->get_exp());
    if (name != NULL) {
      fprintf(asm_file, "lw %s, %s(%s)\n", temp->get_name(), name->get_label()->get_name(), zero.get_name());
    } else {
      Temp *temp2 = node->get_exp()->accept(this);
      fprintf(asm_file, "lw %s, 0(%s)\n", temp->get_name(), temp2->get_name());
      set_free_reg(temp2);
    }
  }
  return temp;
}

///----------------------------------------------------------------------------------------------
///The called function can be a print. In this case, we can have a printInt, printFloat or printLiteral.
///For these cases, the system call is made to print, passing as a parameter the value to be printed;
///----------------------------------------------------------------------------------------------
///The calling function can be a readln. In this case, we can have a readlnInt or readlnFloat.
///In both cases, the system call is made to the readln and then the return is copied to a register,
///which is then returned by the visit function;
///----------------------------------------------------------------------------------------------
///It can also be any function. In this case, the parameters passed to the function are copied to the parameter registers.
///At the same time, the parameters are copied to the stack. Once this is done, a jump is made to the function label.
///Finally, the function return is copied to an available register, which is then returned by the visit function;
///----------------------------------------------------------------------------------------------
Temp *AssemblyGenerator::visit(CallNode *node) {
  NameNode *name = dynamic_cast<NameNode *>(node->get_function());
  Temp *temp = get_free_reg();
  if (name != NULL) {
    if (strcmp("PRINT_INT", name->get_label()->get_name()) == 0) { ///comparar strings
      ExpList *aux = node->get_arguments();
      Temp *reg = aux->get_exp()->accept(this);
      fprintf(asm_file, "move $a0,%s\n", reg->get_name());
      fprintf(asm_file, "li $v0,0x01\n");
      fprintf(asm_file, "syscall\n");                ///syscall - generates a software interrupt
      set_free_reg(reg);
      return temp;
    } else if (strcmp("PRINT_REAL", name->get_label()->get_name()) == 0) {
      ExpList *aux = node->get_arguments();
      Temp *reg = aux->get_exp()->accept(this);
      fprintf(asm_file, "move $a0,%s\n", reg->get_name());
      fprintf(asm_file, "li $v0,0x02\n");           ///syscall - generates a software interrupt
      fprintf(asm_file, "syscall\n");
      set_free_reg(reg);
      return temp;
    } else if (strcmp("PRINT_LITERAL", name->get_label()->get_name()) == 0) {
    ExpList *aux = node->get_arguments();
      Temp *reg = aux->get_exp()->accept(this);
      fprintf(asm_file, "move $a0,%s\n", reg->get_name());
      fprintf(asm_file, "li $v0,0x04\n");
      fprintf(asm_file, "syscall\n");
      set_free_reg(reg);
      return temp;
    } else if (strcmp("READLN_INT", name->get_label()->get_name()) == 0) {
      fprintf(asm_file, "li $v0,0x05\n");
      fprintf(asm_file, "syscall\n");
      fprintf(asm_file, "move %s,$v0\n", temp->get_name());
      return temp;
    } else if (strcmp("READLN_FLOAT", name->get_label()->get_name()) == 0) {
      fprintf(asm_file, "li $v0,0x06\n");
      fprintf(asm_file, "syscall\n");
      fprintf(asm_file, "move %s,$f0\n", temp->get_name());
      return temp;
    } else {/// normal function
      ExpList *list = NULL;
      ExpList *aux = node->get_arguments();
      while (aux != NULL) { //list vai ser uma c�pia de aux
        list = new ExpList(aux->get_exp(), list);
        aux = aux->get_next();
      }
      aux = list;
      int i = 0;
      while (aux != NULL && aux->get_exp() != NULL) {
        Temp *param = aux->get_exp()->accept(this);
        if (i < 4) {
          fprintf(asm_file, "move $a%d, %s\n", i, param->get_name());
        }
        fprintf(asm_file, "sw %s, %d($sp)\n", param->get_name(), i * WORD_SIZE);
        set_free_reg(param);
        aux = aux->get_next();
        i++;
      }
      fprintf(asm_file, "jal %s\n",
              name->get_label()->get_name()); ///jal = jump to the calculated address and stores the return address
      fprintf(asm_file, "move %s,$v0\n", temp->get_name());
      return temp;
    }
  }
  return temp;
}

///If the constant is 0, the register $r0 is returned.
///Otherwise, the constant is copied to an available register and it is returned
Temp *AssemblyGenerator::visit(ConstNode *node) {
  if (node->get_value() == 0) {
    return &zero;
  } else {
    Temp *t = get_free_reg();
    fprintf(asm_file, "li %s, %d\n", t->get_name(),
            node->get_value()); ///li = loads a specific numeric value into register
    return t;
  }
}

Temp *AssemblyGenerator::visit(ConstfNode *node) {
  if (node->get_value() == 0.0) {
    return &zero;
  } else {
    Temp *t = get_free_reg();
    fprintf(asm_file, "li %s, %d\n", t->get_name(),
            (int) node->get_value()); ///li = loads a specific numeric value into register
    return t;
  }
}


///Copy the address of the label to an available register and return it
Temp *AssemblyGenerator::visit(NameNode *node) {
  Temp *t = get_free_reg();
  fprintf(asm_file, "la %s %s\n", t->get_name(), node->get_label()->get_name()); ///la = load address
  return t;
}

///This node generates the MIPS arithmetic commands (add, mul, div, addi, etc.).
///To decide which instruction to use, just check which operator of the operation.
///If one of the values involved in the operation is a constant, the instructions are used with immediate
Temp *AssemblyGenerator::visit(BinOpNode *node) {
  Temp *t = get_free_reg();
  if (node->getToken_op() == PLUS) {
    ConstNode *const1 = dynamic_cast<ConstNode *>(node->get_exp1());
    ConstNode *const2 = dynamic_cast<ConstNode *>(node->get_exp2());
    if (const1 != NULL) {
      Temp *t2 = node->get_exp2()->accept(this);
      ///addi = Adds a register and a sign-extended immediate value and stores the result in a register
      fprintf(asm_file, "addi %s,%s,%d\n", t->get_name(), t2->get_name(), const1->get_value());
      set_free_reg(t2);
    } else if (const2 != NULL) {
      Temp *t1 = node->get_exp1()->accept(this);
      fprintf(asm_file, "addi %s,%s,%d\n", t->get_name(), t1->get_name(), const2->get_value());
      set_free_reg(t1);
    } else {
      Temp *t1 = node->get_exp1()->accept(this);
      Temp *t2 = node->get_exp2()->accept(this);
      fprintf(asm_file, "add %s,%s,%s\n", t->get_name(), t1->get_name(), t2->get_name());
      set_free_reg(t1);
      set_free_reg(t2);
    }
  }
  if (node->getToken_op() == MULT) {
    ConstNode *c1 = dynamic_cast<ConstNode *>(node->get_exp1());
    ConstNode *c2 = dynamic_cast<ConstNode *>(node->get_exp2());;
    if (c1) {
      Temp *t2 = node->get_exp2()->accept(this);
      fprintf(asm_file, "mul %s,%s,%d\n", t->get_name(), t2->get_name(), c1->get_value());
      set_free_reg(t2);
    } else if (c2) {
      Temp *t1 = node->get_exp1()->accept(this);
      fprintf(asm_file, "mul %s,%s,%d\n", t->get_name(), t1->get_name(), c2->get_value());
      set_free_reg(t1);
    } else {
      Temp *t1 = node->get_exp1()->accept(this);
      Temp *t2 = node->get_exp2()->accept(this);
      fprintf(asm_file, "mul %s,%s,%s\n", t->get_name(), t1->get_name(), t2->get_name());
      set_free_reg(t1);
      set_free_reg(t2);
    }
  }
  if (node->getToken_op() == DIV) {
    ConstNode *c2 = dynamic_cast<ConstNode *>(node->get_exp2());;
    if (c2) {
      Temp *t1 = node->get_exp1()->accept(this);
      fprintf(asm_file, "div %s,%s,%d\n", t->get_name(), t1->get_name(), c2->get_value());
      set_free_reg(t1);
    } else {
      Temp *t1 = node->get_exp1()->accept(this);
      Temp *t2 = node->get_exp2()->accept(this);
      fprintf(asm_file, "div %s,%s,%s\n", t->get_name(), t1->get_name(), t2->get_name());
      set_free_reg(t1);
      set_free_reg(t2);
    }
  }
  if (node->getToken_op() == MINUS) {
    ConstNode *c2 = dynamic_cast<ConstNode *>(node->get_exp2());;
    if (c2) {
      Temp *t1 = node->get_exp1()->accept(this);
      fprintf(asm_file, "addi %s,%s,%d\n", t->get_name(), t1->get_name(), -c2->get_value());
      set_free_reg(t1);
    } else {
      Temp *t1 = node->get_exp1()->accept(this);
      Temp *t2 = node->get_exp2()->accept(this);
      fprintf(asm_file, "sub %s,%s,%s\n", t->get_name(), t1->get_name(), t2->get_name());
      set_free_reg(t1);
    }
  }
  return t;
}

///Return register
Temp *AssemblyGenerator::visit(TempNode *node) {
  return node->get_temp();
}

///nothing is returned
void AssemblyGenerator::visit(Stmt *stmt) {}

///If the destination is a MemNode, then the sw statement is used.
///Otherwise, the li is used for when the origin is a constant,
///the la for when the origin is a label and the instruction moves to the general case
void AssemblyGenerator::visit(MoveNode *node) {
  MemNode *mem = dynamic_cast<MemNode *>(node->get_destiny());
  TempNode *temp = dynamic_cast<TempNode *>(node->get_destiny());
  //NoMem *memo = dynamic_cast<NoMem*>(m->origem);
  if (mem != NULL) {///If the destination is a MemNode, then the sw statement is used
    BinOpNode *bin = dynamic_cast<BinOpNode *>(mem->get_exp());
    if (bin != NULL && bin->getToken_op() == PLUS) {  /// se tiver uma soma por constante � incluida na instru��o
      ConstNode *const1 = dynamic_cast<ConstNode *>(bin->get_exp1());
      Temp *t1 = NULL;
      if (const1 != NULL)
        t1 = bin->get_exp2()->accept(this);
      else {
        const1 = dynamic_cast<ConstNode *>(bin->get_exp2());
        if (const1 != NULL)
          t1 = bin->get_exp1()->accept(this);
      }
      if (const1 != NULL && t1 != NULL) {
        Temp *t = node->get_source()->accept(this);
        fprintf(asm_file, "sw %s, %d(%s)\n", t->get_name(), const1->get_value(), t1->get_name());
        set_free_reg(t1);
        set_free_reg(t);
      } else {
        Temp *t = node->get_source()->accept(this);
        Temp *dest = mem->get_exp()->accept(this);
        fprintf(asm_file, "sw %s, %d(%s)\n", t->get_name(), 0, dest->get_name());
        set_free_reg(t);
        set_free_reg(dest);
      }
    } else {
      Temp *t = node->get_source()->accept(this);
      NameNode *name = dynamic_cast<NameNode *>(mem->get_exp());
      if (name) { /// if it's label
        fprintf(asm_file, "sw %s, %s(%s)\n", t->get_name(), name->get_label()->get_name(), zero.get_name());
      } else { /// gerenal case
        Temp *t2 = mem->get_exp()->accept(this);
        fprintf(asm_file, "sw %s, %d(%s)\n", t->get_name(), 0, t2->get_name());
        set_free_reg(t2);
      }
      set_free_reg(t);
    }
  } else if (temp) {
    ConstNode *c = dynamic_cast<ConstNode *>(node->get_source());
    NameNode *name = dynamic_cast<NameNode *>(node->get_source());
    if (c != NULL) {
      fprintf(asm_file, "li %s,%d\n", temp->get_temp()->get_name(), c->get_value());
    } else if (name != NULL) {
      fprintf(asm_file, "la %s,%s\n", temp->get_temp()->get_name(), name->get_label()->get_name());
    } else {
      Temp *aux = node->get_source()->accept(this);
      fprintf(asm_file, "move %s,%s\n", temp->get_temp()->get_name(), aux->get_name());
    }
  }
}

///A visit is made to the expression of this node and at the end, the register allocated by the node is released
void AssemblyGenerator::visit(ExpressionNode *node) {
  set_free_reg(node->get_exp()->accept(this));
}

///The b label command is used in case the destination is a direct label.
///If the destination is the address stored in a register, then a visit is made to the destination expression of the NoJump,
///its return is stored in an available register and then jr (jump register)
///jr = Jump to the address contained in register $s
void AssemblyGenerator::visit(JumpNode *node) {
  NameNode *name = dynamic_cast<NameNode *>(node->get_destiny());
  if (name != NULL)///jumps to a label
    fprintf(asm_file, "b %s\n", name->get_label()->get_name());
  else {///jumps to a register
    Temp *t = node->get_destiny()->accept(this);
    fprintf(asm_file, "jr %s\n", t->get_name());
    set_free_reg(t);
  }
}

///Uses MIPS conditional branch instructions, depending on the relational operator used
void AssemblyGenerator::visit(CJumpNode *node) {
  char p1[100], p2[100];
  switch (node->getToken_op()) {
    case EQUAL:
      sprintf(p1, "beq ");
      break;
    case NOTEQUAL:
      sprintf(p1, "bne ");
      break;
    case GT:
      sprintf(p1, "bgt ");
      break;
    case GTE:
      sprintf(p1, "bge ");
      break;
    case LT:
      sprintf(p1, "blt ");
      break;
    case LTE:
      sprintf(p1, "ble ");
      break;
  }
  ConstNode *ci1 = dynamic_cast<ConstNode *>(node->get_exp1());
  ConstfNode *cf1 = dynamic_cast<ConstfNode *>(node->get_exp1());
  ConstNode *ci2 = dynamic_cast<ConstNode *>(node->get_exp2());
  ConstfNode *cf2 = dynamic_cast<ConstfNode *>(node->get_exp2());
  if (ci1 != NULL || cf1 != NULL) {
    Temp *aux = node->get_exp2()->accept(this);
    if (ci1 != NULL)
      sprintf(p2, "%s %d ", aux->get_name(), ci1->get_value());
    set_free_reg(aux);
  } else if (ci2 != NULL || cf2 != NULL) {
    Temp *aux = node->get_exp1()->accept(this);
    if (ci2)
      sprintf(p2, "%s %d ", aux->get_name(), ci2->get_value());
    set_free_reg(aux);
  } else {
    Temp *aux = node->get_exp1()->accept(this);
    Temp *aux2 = node->get_exp2()->accept(this);
    sprintf(p2, "%s %s ", aux->get_name(), aux2->get_name());
    set_free_reg(aux);
    set_free_reg(aux2);
  }
  NameNode *label = dynamic_cast<NameNode *>(node->getCond_true());
  if (label != NULL)
    fprintf(asm_file, "%s%s%s\n", p1, p2, label->get_label()->get_name());
}

///Visits SeqNode Sentence nodes
void AssemblyGenerator::visit(SeqNode *node) {
  node->get_stmt1()->accept(this);
  node->get_stmt2()->accept(this);
}

///Write in the output file the name of the label
void AssemblyGenerator::visit(LabelNode *node) {
  if (strcmp("FUNCTION_main", node->get_label()->get_name()) == 0) /// rotulo que o spim quer
    fprintf(asm_file, "main:\n");
  fprintf(asm_file, "%s:\n", node->get_label()->get_name());
}

void init_assembly_generator(const char *output_filename, Fragment *fragments) {
  generator = new AssemblyGenerator(output_filename);
  Fragment *aux = fragments;
  while (aux != NULL) {
    Variable *variable = dynamic_cast<Variable *>(aux);
    if (variable != NULL)
      variable->accept(generator);
    aux = aux->get_next();
  }
  aux = fragments;
  while (aux != NULL) {
    Variable *variable = dynamic_cast<Variable *>(aux);
    if (variable == NULL)
      aux->accept(generator);
    aux = aux->get_next();
  }
}

void end_assembly_generator() {
  delete generator;
}

