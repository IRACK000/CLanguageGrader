#define _CRT_SECURE_NO_WARNINGS  // SDL 검사 오류 경고 무시

#ifndef __STDLIB__
    #define __STDLIB__
    #include <stdio.h>  // Standard Input/Output Header Library
    #include <windows.h>  // system
#endif // !__STDLIB__

#include "Util.h"

#define PATH_LEN_MAX 1000  // 파일 경로의 최대 길이를 1000자로 설정
#define MAX_HOMEWORK 9  // 한번에 검사할 수 있는 과제 수를 9개로 설정
#define HOMEWORK_NAME 5+1  // 과제 이름 - 0101 1~9 : 5자리
#define MAX_ANSWER 10  // 한 C 파일에 대해 최대 테스트 점수를 10점으로 설정
#define STUDENT_MAX 100  // 채점할 수 있는 인원 수를 100명으로 설정
#define STUDENT_NAME 21  // 이름 20자리 이상인 경우 오류 발생
#define CMD_BUFFER 2000  // 명령어 버퍼의 최대 길이를 500자로 설정
#define ARR_SIZ 101  // 평가식이 한 줄에 100자리 이상인 경우 오류 발생

// ICON FILE : <a href="https://www.freepik.com/free-photos-vectors/business">Business vector created by macrovector_official - www.freepik.com</a>

#ifndef __PATHCONTAINER_DEFINED__
    #define __PATHCONTAINER_DEFINED__
    typedef struct {
        char * msvc, * gcc, * cmake;
    } PathList;
#endif
