#define _CRT_SECURE_NO_WARNINGS  // SDL �˻� ���� ��� ����

#ifndef __STDLIB__
    #define __STDLIB__
    #include <stdio.h>  // Standard Input/Output Header Library
    #include <windows.h>  // system
#endif // !__STDLIB__

#include <Shlwapi.h>  // PathFileExists �Լ��� ����ϱ� ���� �ʿ��� ��� ����

#pragma comment(lib, "Shlwapi.lib") // Shlwapi ���̺귯���� ��ũ�ϱ� ���� pragma ���ù�

// ANSI/ISO C �Լ� ������Ÿ��
void get_option(char *, int *);
void show_error(char *);
_Bool is_exist(char *, char *, _Bool);
char * trim_whitespace(char*);
