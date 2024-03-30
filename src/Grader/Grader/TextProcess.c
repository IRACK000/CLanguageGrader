#include "TextProcess.h"


void compile_sources(PathList path, char * dir_prefix, char * dir_list, _Bool debug) {
    char cmd_buffer[CMD_BUFFER] = { 0 };  // formatted string ������

    // Dir Prefix ����
    char __dir_prefix[] = "HOMEWORK\\";
    if (dir_prefix == NULL) {
        system("CLS");
        printf("Ÿ�� �л��� %s�Դϴ�.\n", dir_list);
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

        //sprintf(cmd_buffer, "DEL HOMEWORK\\%s\\%s.exe", dir_list, hw_list[i]);  // ���� ���� ���� ����
        //system(cmd_buffer);
    }

    // ����� �α�
    if (debug) {
        printf("\n�׽�Ʈ ����.\n�ƹ�Ű�� �Է��ϸ� ���� �л����� �Ѿ�ϴ�.\n");
        system("PAUSE");
    }
}


void process_text(FILE * score, int hw_amount, char hw_list[MAX_HOMEWORK][HOMEWORK_NAME], char * dir_list, _Bool debug) {
    char cmd_buffer[CMD_BUFFER] = { 0 };  // formatted string ������
    printf("\n\n\nŸ�� �л��� %s�Դϴ�.\n", dir_list);

    fprintf(score, "<div class=\"row\"><h4><%s></h4>\n", dir_list);

    for (int i = 0; i < hw_amount; i++) {
        printf("%d�� ������ ä���մϴ�.\n", i+1);
        fprintf(score, "<div class=\"col-md-6 col-lg-3\"><h5>���� %d (%s.c) ä�� ���</h5>\n", i+1, hw_list[i]);

        // 1. ����� ���� �ڵ� ���
        sprintf(cmd_buffer, "HOMEWORK\\%s\\%s.exe", dir_list, hw_list[i]);
        printf("%s", cmd_buffer);
        _Bool is_compiled = is_exist(cmd_buffer, "", 0);
        sprintf(cmd_buffer, "HOMEWORK\\%s\\%s.c", dir_list, hw_list[i]);
        _Bool is_code_exists = is_exist(cmd_buffer, "", 0);
        FILE * answer = fopen(cmd_buffer, "r+");
        print_c_code(answer, is_code_exists, is_compiled, score);
        if (answer) {
            fclose(answer);
            answer = 0;
        }

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
            fclose(answer);
            answer = 0;
            fclose(result);
            result = 0;

            result = fopen(cmd_buffer, "r+");
            sprintf(cmd_buffer, "ANSWER\\%s\\%d.stdin", hw_list[i], j);
            FILE * input = fopen(cmd_buffer, "r+");
            print_test_case(result, input, is_correct, j, (is_correct) ? ": [PASSED]" : ": [FAILED]", score);
            fclose(result);
            result = 0;
        }
        printf(">> ����: %d / %d��\n", score_num, total_test_case);
        fprintf(score, "<article class=\"markdown-body\"><pre>[����]  %d / %d��</pre><br></article>\n</div>\n", score_num, total_test_case);
    }
    fprintf(score, "</div>\n");

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
    printf("\n\n���� �ҽ� ������ �ִٸ� ANSWER ���� �ȿ� �־��ּ���. ");
    system("PAUSE");
    compile_sources(path, "ANSWER\\", "", 0);

    // ���� �̸� ����
    char hw_series[HOMEWORK_NAME-1] = { 0 };
    int hw_amount = 0;
    printf("\n\n\n���� ��¥�� �Է��ϼ��� (MMDD, �ִ� 4�ڸ�): ");
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
            printf("\n%d�� ������ %d�� �򰡽��� �Է��ϼ���: ", string_index, test_index+1);
            sprintf(cmd_buffer, "CALL notepad ANSWER\\%s%d\\%d.stdin", hw_series, string_index, test_index+1);
            system(cmd_buffer);
            sprintf(cmd_buffer, "ANSWER\\%s%d.exe", hw_series, string_index);
            if (is_exist(cmd_buffer, "", 0)) {
                printf("\n%d�� ������ ���� ���� ������ �����մϴ�. �ڵ����� �ش� �����͸� �����մϴ�.", string_index+1);
                sprintf(
                    cmd_buffer,
                    "ANSWER\\%s%d.exe  < ANSWER\\%s%d\\%d.stdin",
                    hw_series, string_index, hw_series, string_index, test_index + 1
                );
                system(cmd_buffer);
                sprintf(
                    cmd_buffer,
                    "ANSWER\\%s%d.exe  < ANSWER\\%s%d\\%d.stdin >  ANSWER\\%s%d\\%d.stdout",
                    hw_series, string_index, hw_series, string_index, test_index+1, hw_series, string_index, test_index+1
                );
                system(cmd_buffer);
            } else {
                printf("\n%d�� ������ %d�� ���� �Է��ϼ���: ", string_index, test_index+1);
                sprintf(cmd_buffer, "CALL notepad ANSWER\\%s%d\\%d.stdout", hw_series, string_index, test_index+1);
                system(cmd_buffer);
            }
        }
    }
}


void print_answer(FILE * score, int hw_amount, char hw_list[MAX_HOMEWORK][HOMEWORK_NAME]) {
    char cmd_buffer[CMD_BUFFER] = { 0 };  // formatted string ������

    fprintf(score, "<div class=\"row\"><h4><���� ������></h4>\n");

    for (int i = 0; i < hw_amount; i++) {
        fprintf(score, "<div class=\"col-md-6 col-lg-3\"><h5>���� %d (%s.c) ���� ����Ʈ</h5>\n", i + 1, hw_list[i]);

        // 1. ���� ���� �ڵ� ���
        sprintf(cmd_buffer, "ANSWER\\%s.c", hw_list[i]);
        _Bool is_code_exists = is_exist(cmd_buffer, "", 0);
        FILE * answer = fopen(cmd_buffer, "r+");
        print_c_code(answer, is_code_exists, -1, score);
        if (answer) {
            fclose(answer);
            answer = 0;
        }

        // 2. ���� �׽�Ʈ ���̽� ���� ���
        for (int j = 0; j < MAX_ANSWER; j++) {
            sprintf(cmd_buffer, "ANSWER\\%s\\%d.stdout", hw_list[i], j);
            if (!is_exist(cmd_buffer, "", 0)) {
                break;
            }
            FILE * answer = fopen(cmd_buffer, "r+");
            sprintf(cmd_buffer, "ANSWER\\%s\\%d.stdin", hw_list[i], j);
            FILE * input = fopen(cmd_buffer, "r+");
            print_test_case(answer, input, -1, j, "", score);
            fclose(answer);
            answer = 0;
            fclose(input);
            input = 0;
        }
        fprintf(score, "</div>\n");
    }
    fprintf(score, "</div><br><br>\n");
}


void print_c_code(FILE * answer, _Bool is_code_exists, short compile_status, FILE * score) {
    const char source_found[] = "���� �ڵ� �߰ߵ�";
    const char source_not_found[] = "���� �ڵ� ����";
    const char compile_success[] = "������ ����";
    const char compile_failed[] = "������ ����";
    const char not_submitted[] = "�ڵ� ������";

    char * state_text = 0;
    _Bool status = 0;
    if (compile_status < 0) {
        state_text = (is_code_exists) ? source_found : source_not_found;
        status = is_code_exists;
    } else if (compile_status == 0) {
        state_text = (is_code_exists) ? compile_failed : not_submitted;
        status = 0;
    } else {
        state_text = compile_success;
        status = 1;
    }

    fprintf(score, "<div class=\"p-2 mb-2 bg-%s text-white test-result\"><details><summary>%s</summary><pre class=\"mb-0\">\n", (status) ? "dark" : "secondary ", state_text);

    char code[ARR_SIZ] = { 0 };
    char * code_ = NULL;
    while (is_code_exists) {
        code_ = fgets(code, ARR_SIZ, answer);
        if (code_ != NULL) {
            code_ = trim_whitespace(code_);
            if (code_[0] == 0) {  // ���� ���� ��ŵ
                continue;
            }
            int cursor = adjust_include_str(code);
            fprintf(score, "%s\n", code);
        } else {
            break;
        }
    }
    fprintf(score, "</pre></details></div>\n");
}


void print_test_case(FILE * answer, FILE * input, short case_status, int case_number, char * state, FILE * score) {
    char * backgroud_color = (case_status > 0) ? "success" : (case_status == 0) ? "danger" : "primary";

    fprintf(score, "<div class=\"p-2 mb-2 bg-%s text-white test-result\"><details><summary>�׽�Ʈ ���̽� %d%s</summary><pre class=\"mb-0\">\n", backgroud_color, case_number+1, state);

    char ans[ARR_SIZ] = { 0 };
    char * ans_ = NULL;
    while (input) {
        ans_ = fgets(ans, ARR_SIZ, input);
        if (ans_ != NULL) {
            ans_ = trim_whitespace(ans);
            if (ans_[0] == 0) {  // ���� ���� ��ŵ
                continue;
            }
            fprintf(score, ">> %s\n", ans);
        } else {
            break;
        }
    }
    if (input) {
        fprintf(score, "---------------------------------\n");
    }
    while (answer) {
        ans_ = fgets(ans, ARR_SIZ, answer);
        if (ans_ != NULL) {
            ans_ = trim_whitespace(ans);
            if (ans_[0] == 0) {  // ���� ���� ��ŵ
                continue;
            }
            fprintf(score, "%s\n", ans);
        } else {
            break;
        }
    }
    fprintf(score, "</pre></details></div>\n");
}


int adjust_include_str(char * str) {
    _Bool is_found = 0;
    int cursor = 0;
    const char include_str[] = "#include <";
    char * _str = include_str;

    while (1) {
        if (*_str == 0) {
            break;
        } else if (*str == 0) {
            cursor = -1;
            break;
        }
        if (*(str++) == *_str) {
            is_found = 1;
            _str++;
        } else if (is_found) {
            is_found = 0;
            cursor = -1;
            break;
        }
        cursor++;
    }

    if (cursor >= 0) {
        str--;
        (*str) = '"';
        while (*str) {
            if (*str == '>') {
                (*str) = '"';
                break;
            }
            str++;
        }
    }

    return cursor;
}
