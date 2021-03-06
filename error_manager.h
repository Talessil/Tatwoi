#ifndef ERROR_MANAGER_H_INCLUDED
#define ERROR_MANAGER_H_INCLUDED

/// Definition of lexical errors
#define ERROR_EOF_COMMENT 0
#define ERROR_EOF_LITERAL 1
#define ERROR_EOF_CHARACTER 2
#define ERROR_INVALID_CHAR_CONSTRUCTION 3
#define ERROR_WRONG_NUMBER 4
#define ERROR_INVALID_CHARACTER 5

/// Definition of syntactic errors
#define ERROR_UNEXPECTED_TOKEN 0
#define ERROR_LBRACE_OR_COLON_EXPECTED 1
#define ERROR_TYPE_EXPECTED 2
#define ERROR_TYPE_PRIMITIVE_EXPECTED 3
#define ERROR_STATEMENT_EXPECTED 4
#define ERROR_CASE_EXPECTED 5
#define ERROR_EXPRESSION_EXPECTED 6

/// Definition of semantic errors
#define ERROR_CLASS_ALREADY_EXISTS 0
#define ERROR_VARIABLE_ALREADY_EXISTS 1
#define ERROR_FUNCTION_ALREADY_EXISTS 2
#define ERROR_STRUCT_ALREADY_EXISTS 3
#define ERROR_ARRAY_SIZE_MUST_BE_OF_TYPE_INT 4
#define ERROR_LVALUE_REQUIRED_AS_LEFT_OPERAND_OF_ASSIGNMENT 5
#define ERROR_CANNOT_ASSIGN_TO_DIFFERENT_TYPE 6
#define ERROR_RELATIONAL_OP_TYPE 7
#define ERROR_ARITHMETIC_OP_TYPE 8
#define ERROR_LOGICAL_OP_TYPE 9
#define ERROR_BINARY_OP_TYPE 10
#define ERROR_TYPE_NOT_DEFINED 11
#define ERROR_RETURN_TYPE_NOT_DEFINED 12
#define ERROR_INCOMPATIBLE_RETURN_TYPE 13
#define ERROR_MISSING_RETURN_STATEMENT 14
#define ERROR_BOOLEAN_EXPRESSION_REQUIRED 15
#define ERROR_INT_VALUE_REQUIRED 16
#define ERROR_CASE_STATEMENT_REQUIRED 17
#define ERROR_UNEXPECTED_BREAK 18
#define ERROR_VARIABLE_NOT_DECLARED 19
#define ERROR_FUNCTION_NOT_DECLARED 20
#define ERROR_CLASS_DOES_NOT_HAVE_MEMBER 21
#define ERROR_STRUCT_DOES_NOT_HAVE_MEMBER 22
#define ERROR_UNEXPECTED_ACCESS_IN_NOT_ID_TYPE 23
#define ERROR_CLASS_NOT_FOUND 24
#define ERROR_PARENT_CLASS_NOT_FOUND 25
#define ERROR_INVALID_USE_OF_THIS 26
#define ERROR_PERIOD_POINTER_ACCESS 27
#define ERROR_ARROW_NON_POINTER 28
#define ERROR_PARAMETER_TYPE_MISMATCH 29
#define ERROR_TOO_FEW_ARGUMENTS 30
#define ERROR_TOO_MANY_ARGUMENTS 31

void print_lexical_error(int error_id, int line_number);
const char *eat_or_skip(int expected_token, const int *sync_set);
void synchronize(int error_id, const int *sync_set);
void print_semantic_error(int error_id, int line_number);
void print_all_errors();
int has_errors();

#endif // ERROR_MANAGER_H_INCLUDED
