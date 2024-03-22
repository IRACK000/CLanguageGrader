#define _CRT_SECURE_NO_WARNINGS  // SDL �˻� ���� ��� ����

#ifndef __STDLIB__
    #define __STDLIB__
    #include <stdio.h>  // Standard Input/Output Header Library
    #include <windows.h>  // system
#endif // !__STDLIB__

#include "Util.h"

#define PATH_LEN_MAX 1000  // ���� ����� �ִ� ���̸� 1000�ڷ� ����
#define MAX_HOMEWORK 9  // �ѹ��� �˻��� �� �ִ� ���� ���� 9���� ����
#define HOMEWORK_NAME 5+1  // ���� �̸� - 0101 1~9 : 5�ڸ�
#define MAX_ANSWER 10  // �� C ���Ͽ� ���� �ִ� �׽�Ʈ ������ 10������ ����
#define STUDENT_MAX 100  // ä���� �� �ִ� �ο� ���� 100������ ����
#define STUDENT_NAME 21  // �̸� 20�ڸ� �̻��� ��� ���� �߻�
#define CMD_BUFFER 2000  // ��ɾ� ������ �ִ� ���̸� 500�ڷ� ����
#define ARR_SIZ 101  // �򰡽��� �� �ٿ� 100�ڸ� �̻��� ��� ���� �߻�

// ICON FILE : <a href="https://www.freepik.com/free-photos-vectors/business">Business vector created by macrovector_official - www.freepik.com</a>

#ifndef __PATHCONTAINER_DEFINED__
    #define __PATHCONTAINER_DEFINED__
    typedef struct {
        char * msvc, * gcc, * cmake;
    } PathList;
#endif
