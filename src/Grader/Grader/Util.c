#include "Util.h"

void get_option(char * opt_string, int * option) {
    char _ = 0;  // 함수 반환값 받아 버릴 변수
    printf(opt_string);
    _ = scanf("%d", option);
    _ = getchar();
}


void show_error(char * error_message) {
    system("COLOR F4");
    printf(error_message);
    system("PAUSE");
    system("COLOR 06");
    system("CLS");
}


_Bool is_exist(char * file_path, char * file_name, _Bool override) {
    char expanded_file_path[MAX_PATH];
    DWORD result = ExpandEnvironmentStrings(file_path, expanded_file_path, MAX_PATH);

    if (result == 0 || result > MAX_PATH) {
        printf("ERROR: 환경 변수를 해석하는 도중 오류가 발생했습니다.\n");
        system("PAUSE");
        exit(1);
    }
    if (override) {
        strcpy(file_path, expanded_file_path);
    }

    return PathFileExists(strcat(expanded_file_path, file_name));
}


char * trim_whitespace(char * str) {
    char* end;

    // 문자열 시작 지점에서 왼쪽 공백 스킵
    while (isspace((unsigned char)*str)) {
        str++;
    }

    // 문자열 끝 지점에서 오른쪽 공백 제거
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) {
        end--;
    }

    // 끝 문자 뒤에 널 문자 삽입
    *(end + 1) = '\0';

    return str;
}
