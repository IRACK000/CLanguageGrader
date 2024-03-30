#include "Project.h"

// ANSI/ISO C 함수 프로토타입
void compile_sources(PathList, char *, char *, _Bool);
void test_excutables(int, char[MAX_HOMEWORK][HOMEWORK_NAME], char *, _Bool);
void process_text(FILE *, int, char[MAX_HOMEWORK][HOMEWORK_NAME], char *, _Bool);
void set_new_answer(PathList);
void print_answer(FILE *, int, char[MAX_HOMEWORK][HOMEWORK_NAME]);
void print_c_code(FILE *, _Bool, short, FILE *);
void print_test_case(FILE *, FILE *, short, int, char *, FILE *);
int adjust_include_str(char *);
