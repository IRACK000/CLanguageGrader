#define _CRT_SECURE_NO_WARNINGS  // SDL 검사 오류 경고 무시

#ifndef __STDLIB__
    #define __STDLIB__
    #include <stdio.h>  // Standard Input/Output Header Library
    #include <windows.h>  // system
#endif // !__STDLIB__

#include <Shlwapi.h>  // PathFileExists 함수를 사용하기 위해 필요한 헤더 파일

#pragma comment(lib, "Shlwapi.lib") // Shlwapi 라이브러리를 링크하기 위한 pragma 지시문

// ANSI/ISO C 함수 프로토타입
void get_option(char *, int *);
void show_error(char *);
_Bool is_exist(char *, char *, _Bool);
char * trim_whitespace(char*);
