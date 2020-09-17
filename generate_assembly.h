#ifndef COMPILADOR_CPM_GENERATE_ASSEMBLY_H
#define COMPILADOR_CPM_GENERATE_ASSEMBLY_H

class Temp;
#include <stdio.h>
#include "intermediate_code.h"
#include "queue.h"

void init_assembly_generator(const char *output_filename, Fragment *fragments);
void end_assembly_generator();

class AssemblyGenerator {
  private:
    FILE *asm_file;
    Queue *reg_queue;
  public:
    AssemblyGenerator(const char *output_filename);
    virtual ~AssemblyGenerator();

    Temp *get_free_reg();
    void set_free_reg(Temp *temp);
    void save_reg(int offset);
    void recover_reg(int offset);

    void visit(Procedure *fragment);
    void visit(Literal *fragment);
    void visit(Variable *fragment);
    Temp *visit(ConstNode *node);
    Temp *visit(ConstfNode *node);
    Temp *visit(NameNode *node);
    Temp *visit(TempNode *node);
    Temp *visit(BinOpNode *node);
    Temp *visit(MemNode *node);
    Temp *visit(CallNode *node);
    void visit(Stmt *node);
    void visit(MoveNode *node);
    void visit(ExpressionNode *node);
    void visit(JumpNode *node);
    void visit(CJumpNode *node);
    void visit(SeqNode *node);
    void visit(LabelNode *node);
};

#endif //COMPILADOR_CPM_GENERATE_ASSEMBLY_H
