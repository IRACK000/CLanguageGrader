#include "Util.h"

void get_option(char * opt_string, int * option) {
    char _ = 0;  // �Լ� ��ȯ�� �޾� ���� ����
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
        printf("ERROR: ȯ�� ������ �ؼ��ϴ� ���� ������ �߻��߽��ϴ�.\n");
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

    // ���ڿ� ���� �������� ���� ���� ��ŵ
    while (isspace((unsigned char)*str)) {
        str++;
    }

    // ���ڿ� �� �������� ������ ���� ����
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) {
        end--;
    }

    // �� ���� �ڿ� �� ���� ����
    *(end + 1) = '\0';

    return str;
}
