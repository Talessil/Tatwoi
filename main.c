#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lex_analyzer.h"
#include "parser.h"
#include "semantic_analyzer.h"
#include "error_manager.h"
#include "generate_assembly.h"

/**
* Returns a FILE* which is the input for the compiler.
* It can be a file with the cpm extension or it can read directly from stdin.
*/
FILE *read_file(int argc, char **argv) {
  FILE *arq = NULL;
  if (argc >= 2) { // verify if there is any input parameter
    const char *file_name = argv[1];
    int size = strlen(file_name);
    // verify file label extension
    if (!strcmp(&file_name[size - 4], ".cpm")) {
      arq = fopen(file_name, "r");
      printf("\nFile found. Starting compilation.\n\n");
    } else { // add extension
      char *bigger = (char *) malloc(sizeof(char *) * (size + 4));
      strcpy(bigger, file_name);
      strcat(bigger, ".cpm");
      arq = fopen(bigger, "r");
      free(bigger);
      printf("\nExtension .cpm added\n\n");
    }
  }
  if (arq == NULL) { // stdin stream
    printf("\nThere'stmt QueueNode file with this label.");
    printf("\nType the input:\n\n");
    arq = stdin;
  }
  return arq;
}

int main(int argc, char **argv) {
  printf("------------------------------------------\n");
  printf("-----------TATWOI COMPILER v4.0-----------\n");
  printf("------------------------------------------\n");
  FILE *file = read_file(argc, argv);
  init_lex(file);
  ProgramNode *program = init_parser();
  init_semantic(program);
  if (!has_errors()) {
    Translator *t = init_translator(program);
    init_assembly_generator("result.asm", t->getFragment_list());
    end_assembly_generator();
    end_translator();
  } else {
    fprintf(stdout, "\n--- Stopping compilation, errors were found ---\n");
    print_all_errors();
  }
  end_semantic();
  end_parser();
  end_lex();
  return 0;
}
