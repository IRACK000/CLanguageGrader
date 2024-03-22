#include "TextProcess.h"


void compile_sources(PathList path, char * dir_prefix, char * dir_list, _Bool debug) {
    char cmd_buffer[CMD_BUFFER] = { 0 };  // formatted string ������
    system("CLS");
    printf("Ÿ�� �л��� %s�Դϴ�.\n", dir_list);

    // Dir Prefix ����
    char __dir_prefix[] = "HOMEWORK\\";
    if (dir_prefix == NULL) {
        dir_prefix = __dir_prefix;
    }

    // �����Ϸ� ���� ��������
    char compiler = 0;
    sprintf(cmd_buffer, "%s%s\\compiler.setting", dir_prefix, dir_list);
    FILE * config = fopen(cmd_buffer, "r");
    if (config != NULL) {
        char compiler_buffer[ARR_SIZ] = { 0 };
        fgets(compiler_buffer, sizeof(compiler_buffer), config);
        fclose(config);
        if (strstr(compiler_buffer, "compiler = msvc") != NULL) {
            compiler = 1;
        } else if (strstr(compiler_buffer, "compiler = clang") != NULL) {
            compiler = 2;
        } else if (strstr(compiler_buffer, "compiler = cmake") != NULL) {
            compiler = 3;
        } else {
            compiler = 0;
        }
    }

    // ������ ����
    if (compiler == 1) {
        printf("\nMSVC++�� �������� �����մϴ�.\n");
        sprintf(cmd_buffer, "\"%s\\VsDevCmd.bat\" && cd %s%s && for %%f in (*.c) do cl /w %%f && del *.obj && dir", path.msvc, dir_prefix, dir_list);
    } else if (compiler == 2) {
        printf("\nClang�� �������� �����մϴ�.\n");
        sprintf(cmd_buffer, "\"%s\\VsDevCmd.bat\" && cd %s%s && for %%f in (*.c) do clang -o %%~nf.exe %%f && dir", path.msvc, dir_prefix, dir_list);
    } else if (compiler == 3) {  // TODO: CMAKE�� ������ ����
        printf("\nCMAKE�� ������Ʈ�� �����մϴ�.\n");
        sprintf(cmd_buffer, "set \"path=%%path%%;%s\" && cd %s%s && cmake .. && dir", path.cmake, dir_prefix, dir_list);
    } else {
        printf("\nGCC(DEFAULT)�� �������� �����մϴ�.\n");
        sprintf(cmd_buffer, "set \"path=%%path%%;%s\" && cd %s%s && for %%f in (*.c) do gcc -o %%~nf.exe %%f && dir", path.gcc, dir_prefix, dir_list);
    }
    printf("%s\n", cmd_buffer);
    system(cmd_buffer);

    // ����� �α�
    if (debug) {
        printf("\n������ �α׸� Ȯ���ϼ���.\n�ƹ�Ű�� �Է��ϸ� ���α׷� �۵��� �簳�մϴ�.\n");
        system("PAUSE");
    }
}


void test_excutables(int hw_amount, char hw_list[MAX_HOMEWORK][HOMEWORK_NAME], char * dir_list, _Bool debug) {
    char cmd_buffer[CMD_BUFFER] = { 0 };  // formatted string ������
    printf("\n\n\nŸ�� �л��� %s�Դϴ�.\n", dir_list);

    sprintf(cmd_buffer, "MKDIR RESULT_GRADE\\%s", dir_list);
    system(cmd_buffer);

    for (int i = 0; i < hw_amount; i++) {
        printf("%d�� ������ �����մϴ�.\n", i+1);

        sprintf(cmd_buffer, "MKDIR RESULT_GRADE\\%s\\%s", dir_list, hw_list[i]);
        system(cmd_buffer);

        for (int j = 0; j < MAX_ANSWER; j++) {
            sprintf(cmd_buffer, "ANSWER\\%s\\%d.stdin", hw_list[i], j);
            if (!is_exist(cmd_buffer, "", 0)) {
                break;
            }
            sprintf(
                cmd_buffer,
                "HOMEWORK\\%s\\%s.exe  < ANSWER\\%s\\%d.stdin >  RESULT_GRADE\\%s\\%s\\%d.txt",
                dir_list, hw_list[i], hw_list[i], j, dir_list, hw_list[i], j
            );
            system(cmd_buffer);
        }

        sprintf(cmd_buffer, "DEL HOMEWORK\\%s\\%s.exe", dir_list, hw_list[i]);
        system(cmd_buffer);
    }

    // ����� �α�
    if (debug) {
        printf("\n�׽�Ʈ ����.\n�ƹ�Ű�� �Է��ϸ� ���� �л����� �Ѿ�ϴ�.\n");
        system("PAUSE");
    }
}


void process_text(int hw_amount, char hw_list[MAX_HOMEWORK][HOMEWORK_NAME], char * dir_list, _Bool debug) {
    char cmd_buffer[CMD_BUFFER] = { 0 };  // formatted string ������
    printf("\n\n\nŸ�� �л��� %s�Դϴ�.\n", dir_list);

    FILE * score = fopen("RESULT_GRADE\\SCORE_OUT.html", "a+");
    fprintf(score, "<div class=\"row\"><h4><%s></h4>\n", dir_list);

    for (int i = 0; i < hw_amount; i++) {
        printf("%d�� ������ ä���մϴ�.\n", i+1);
        fprintf(score, "<div class=\"col-sm\"><h5>���� %d (%s.c) ä�� ���</h5>\n", i+1, hw_list[i]);

        char ans[ARR_SIZ] = { 0 }, res[ARR_SIZ] = { 0 };
        int score_num = 0, total_test_case = 0;
        for (int j = 0; j < MAX_ANSWER; j++) {
            sprintf(cmd_buffer, "ANSWER\\%s\\%d.stdout", hw_list[i], j);
            if (!is_exist(cmd_buffer, "", 0)) {
                break;
            }
            FILE * answer = fopen(cmd_buffer, "r+");
            sprintf(cmd_buffer, "RESULT_GRADE\\%s\\%s\\%d.txt", dir_list, hw_list[i], j);
            FILE * result = fopen(cmd_buffer, "r+");

            char * ans_ = NULL, * res_ = NULL;
            _Bool is_eof_ans = 0, is_eof_res = 0;
            _Bool is_correct = 1;
            while (1) {
                while (!is_eof_ans) {
                    ans_ = fgets(ans, ARR_SIZ, answer);
                    if (ans_ == NULL) {
                        is_eof_ans = 1;
                        break;
                    }
                    ans_ = trim_whitespace(ans_);
                    if (ans_[0] == 0) {  // ���� ���� ��ŵ
                        continue;
                    }
                    break;
                }
                while (!is_eof_res) {
                    res_ = fgets(res, ARR_SIZ, result);
                    if (res_ == NULL) {
                        is_eof_res = 1;
                        break;
                    }
                    res_ = trim_whitespace(res_);
                    if (res_[0] == 0) {  // ���� ���� ��ŵ
                        continue;
                    }
                    break;
                }

                if (is_eof_ans && is_eof_res) {   // �� �� EOF�� ����
                    break;
                }

                if (strcmp(ans, res) != 0) {
                    is_correct = 0;  // ������ �ƴϸ� �÷��� ����
                    break;  // ������ �ƴϸ� �� �̻� ���� �ʿ� ����
                }
            }

            if (is_correct) score_num++;
            total_test_case++;

            printf("[%s] ���� %d (%s.c) - %d�� �׽�Ʈ ���̽�: %s\n", dir_list, i+1, hw_list[i], j+1, (is_correct) ? "[PASSED]" : "[FAILED]");
            fprintf(score, "<div class=\"p-2 mb-2 bg-%s text-white test-result\">* �׽�Ʈ ���̽� %d: %s</div>\n", (is_correct) ? "success" : "danger", j+1, (is_correct) ? "[PASSED]" : "[FAILED]");

            fclose(answer);
            fclose(result);
        }
        printf(">> ����: %d / %d��\n", score_num, total_test_case);
        fprintf(score, "<article class=\"markdown-body\"><pre>[����]  %d / %d��</pre><br></article>\n</div>\n", score_num, total_test_case);
    }
    fprintf(score, "</div>\n");
    fclose(score);

    // ����� �α�
    if (debug) {
        printf("\n��� �� ����.\n�ƹ�Ű�� �Է��ϸ� ���� �л����� �Ѿ�ϴ�.\n");
        system("PAUSE");
    }
}


void set_new_answer(PathList path) {
    int _;

    system("CLS");
    printf("���� ��ϵ� �׽�Ʈ ������ ���� ����� ���� �����մϴ�. ��� �����ұ��? (press y to yes): ");
    if (getchar() != 'y') {
        char _ = getchar();
        return;
    }

    // ���� ���� �ʱ�ȭ
    system("RMDIR /s ANSWER");
    system("MKDIR ANSWER");

    // ���� �ҽ� ���� ������ ������
    compile_sources(path, "ANSWER\\", "", 0);

    // ���� �̸� ����
    char hw_series[HOMEWORK_NAME-1] = { 0 };
    int hw_amount = 0;
    printf("\n���� ��¥�� �Է��ϼ��� (MMDD, �ִ� 4�ڸ�): ");
    _ = scanf("%s", hw_series);
    while (1) {
        printf("\n���� ������ �Է��ϼ��� (1~9): ");
        _ = scanf("%d", &hw_amount);
        if (hw_amount <= MAX_HOMEWORK && hw_amount > 0) {
            break;
        }
        printf("ERROR: 1~9 ������ ���ڸ� �Է��ϼ���.\n");
    }

    // ���� �Է�
    char cmd_buffer[CMD_BUFFER] = { 0 };  // formatted string ������
    for (int string_index = 1; string_index <= hw_amount; string_index++) {
        sprintf(cmd_buffer, "mkdir ANSWER\\%s%d", hw_series, string_index);
        system(cmd_buffer);

        int test_count = 0;
        while (1) {
            printf("\n\n\n%d�� ������ �׽�Ʈ ���̽� ������ �Է��ϼ��� (1~10): ", string_index);
            _ = scanf("%d", &test_count);
            if (test_count < MAX_ANSWER && test_count >= 0) {
                break;
            }
            printf("ERROR: 1~10 ������ ���ڸ� �Է��ϼ���.\n");
        }

        for (int test_index = 0; test_index < test_count; test_index++) {
            printf("\n%d�� ������ %d�� �򰡽��� �Է��ϼ���: ", string_index, test_index);
            sprintf(cmd_buffer, "CALL notepad ANSWER\\%s%d\\%d.stdin", hw_series, string_index, test_index);
            system(cmd_buffer);
            sprintf(cmd_buffer, "ANSWER\\%s%d.exe", hw_series, string_index);
            if (is_exist(cmd_buffer, "", 0)) {
                printf("\n%d�� ������ ���� ���� ������ �����մϴ�. �ڵ����� �ش� �����͸� �����մϴ�.", string_index);
                sprintf(
                    cmd_buffer,
                    "ANSWER\\%s%d.exe  < ANSWER\\%s%d\\%d.stdin >  ANSWER\\%s%d\\%d.stdout",
                    hw_series, string_index, hw_series, string_index, test_index, hw_series, string_index, test_index
                );
                system(cmd_buffer);
            } else {
                printf("\n%d�� ������ %d�� ���� �Է��ϼ���: ", string_index, test_index);
                sprintf(cmd_buffer, "CALL notepad ANSWER\\%s%d\\%d.stdout", hw_series, string_index, test_index);
                system(cmd_buffer);
            }
        }
    }
}
