#include "TextProcess.h"


void compile_sources(PathList path, char * dir_prefix, char * dir_list, _Bool debug) {
    char cmd_buffer[CMD_BUFFER] = { 0 };  // formatted string 보관용
    system("CLS");
    printf("타켓 학생은 %s입니다.\n", dir_list);

    // Dir Prefix 설정
    char __dir_prefix[] = "HOMEWORK\\";
    if (dir_prefix == NULL) {
        dir_prefix = __dir_prefix;
    }

    // 컴파일러 설정 가져오기
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

    // 컴파일 진행
    if (compiler == 1) {
        printf("\nMSVC++로 컴파일을 진행합니다.\n");
        sprintf(cmd_buffer, "\"%s\\VsDevCmd.bat\" && cd %s%s && for %%f in (*.c) do cl /w %%f && del *.obj && dir", path.msvc, dir_prefix, dir_list);
    } else if (compiler == 2) {
        printf("\nClang로 컴파일을 진행합니다.\n");
        sprintf(cmd_buffer, "\"%s\\VsDevCmd.bat\" && cd %s%s && for %%f in (*.c) do clang -o %%~nf.exe %%f && dir", path.msvc, dir_prefix, dir_list);
    } else if (compiler == 3) {  // TODO: CMAKE로 컴파일 진행
        printf("\nCMAKE로 프로젝트를 빌드합니다.\n");
        sprintf(cmd_buffer, "set \"path=%%path%%;%s\" && cd %s%s && cmake .. && dir", path.cmake, dir_prefix, dir_list);
    } else {
        printf("\nGCC(DEFAULT)로 컴파일을 진행합니다.\n");
        sprintf(cmd_buffer, "set \"path=%%path%%;%s\" && cd %s%s && for %%f in (*.c) do gcc -o %%~nf.exe %%f && dir", path.gcc, dir_prefix, dir_list);
    }
    printf("%s\n", cmd_buffer);
    system(cmd_buffer);

    // 디버그 로그
    if (debug) {
        printf("\n컴파일 로그를 확인하세요.\n아무키나 입력하면 프로그램 작동을 재개합니다.\n");
        system("PAUSE");
    }
}


void test_excutables(int hw_amount, char hw_list[MAX_HOMEWORK][HOMEWORK_NAME], char * dir_list, _Bool debug) {
    char cmd_buffer[CMD_BUFFER] = { 0 };  // formatted string 보관용
    printf("\n\n\n타켓 학생은 %s입니다.\n", dir_list);

    sprintf(cmd_buffer, "MKDIR RESULT_GRADE\\%s", dir_list);
    system(cmd_buffer);

    for (int i = 0; i < hw_amount; i++) {
        printf("%d번 과제를 실행합니다.\n", i+1);

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

    // 디버그 로그
    if (debug) {
        printf("\n테스트 종료.\n아무키나 입력하면 다음 학생으로 넘어갑니다.\n");
        system("PAUSE");
    }
}


void process_text(int hw_amount, char hw_list[MAX_HOMEWORK][HOMEWORK_NAME], char * dir_list, _Bool debug) {
    char cmd_buffer[CMD_BUFFER] = { 0 };  // formatted string 보관용
    printf("\n\n\n타켓 학생은 %s입니다.\n", dir_list);

    FILE * score = fopen("RESULT_GRADE\\SCORE_OUT.html", "a+");
    fprintf(score, "<div class=\"row\"><h4><%s></h4>\n", dir_list);

    for (int i = 0; i < hw_amount; i++) {
        printf("%d번 과제를 채점합니다.\n", i+1);
        fprintf(score, "<div class=\"col-sm\"><h5>과제 %d (%s.c) 채점 결과</h5>\n", i+1, hw_list[i]);

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
                    if (ans_[0] == 0) {  // 공백 라인 스킵
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
                    if (res_[0] == 0) {  // 공백 라인 스킵
                        continue;
                    }
                    break;
                }

                if (is_eof_ans && is_eof_res) {   // 둘 다 EOF면 종료
                    break;
                }

                if (strcmp(ans, res) != 0) {
                    is_correct = 0;  // 정답이 아니면 플래그 설정
                    break;  // 정답이 아니면 더 이상 비교할 필요 없음
                }
            }

            if (is_correct) score_num++;
            total_test_case++;

            printf("[%s] 과제 %d (%s.c) - %d번 테스트 케이스: %s\n", dir_list, i+1, hw_list[i], j+1, (is_correct) ? "[PASSED]" : "[FAILED]");
            fprintf(score, "<div class=\"p-2 mb-2 bg-%s text-white test-result\">* 테스트 케이스 %d: %s</div>\n", (is_correct) ? "success" : "danger", j+1, (is_correct) ? "[PASSED]" : "[FAILED]");

            fclose(answer);
            fclose(result);
        }
        printf(">> 총점: %d / %d점\n", score_num, total_test_case);
        fprintf(score, "<article class=\"markdown-body\"><pre>[총점]  %d / %d점</pre><br></article>\n</div>\n", score_num, total_test_case);
    }
    fprintf(score, "</div>\n");
    fclose(score);

    // 디버그 로그
    if (debug) {
        printf("\n결과 비교 종료.\n아무키나 입력하면 다음 학생으로 넘어갑니다.\n");
        system("PAUSE");
    }
}


void set_new_answer(PathList path) {
    int _;

    system("CLS");
    printf("현재 등록된 테스트 정답을 전부 지우고 새로 지정합니다. 계속 진행할까요? (press y to yes): ");
    if (getchar() != 'y') {
        char _ = getchar();
        return;
    }

    // 정답 파일 초기화
    system("RMDIR /s ANSWER");
    system("MKDIR ANSWER");

    // 정답 소스 파일 있으면 컴파일
    compile_sources(path, "ANSWER\\", "", 0);

    // 과제 이름 지정
    char hw_series[HOMEWORK_NAME-1] = { 0 };
    int hw_amount = 0;
    printf("\n과제 날짜를 입력하세요 (MMDD, 최대 4자리): ");
    _ = scanf("%s", hw_series);
    while (1) {
        printf("\n과제 개수를 입력하세요 (1~9): ");
        _ = scanf("%d", &hw_amount);
        if (hw_amount <= MAX_HOMEWORK && hw_amount > 0) {
            break;
        }
        printf("ERROR: 1~9 사이의 숫자를 입력하세요.\n");
    }

    // 정답 입력
    char cmd_buffer[CMD_BUFFER] = { 0 };  // formatted string 보관용
    for (int string_index = 1; string_index <= hw_amount; string_index++) {
        sprintf(cmd_buffer, "mkdir ANSWER\\%s%d", hw_series, string_index);
        system(cmd_buffer);

        int test_count = 0;
        while (1) {
            printf("\n\n\n%d번 문제의 테스트 케이스 개수를 입력하세요 (1~10): ", string_index);
            _ = scanf("%d", &test_count);
            if (test_count < MAX_ANSWER && test_count >= 0) {
                break;
            }
            printf("ERROR: 1~10 사이의 숫자를 입력하세요.\n");
        }

        for (int test_index = 0; test_index < test_count; test_index++) {
            printf("\n%d번 문제의 %d번 평가식을 입력하세요: ", string_index, test_index);
            sprintf(cmd_buffer, "CALL notepad ANSWER\\%s%d\\%d.stdin", hw_series, string_index, test_index);
            system(cmd_buffer);
            sprintf(cmd_buffer, "ANSWER\\%s%d.exe", hw_series, string_index);
            if (is_exist(cmd_buffer, "", 0)) {
                printf("\n%d번 문제의 정답 실행 파일이 존재합니다. 자동으로 해답 데이터를 생성합니다.", string_index);
                sprintf(
                    cmd_buffer,
                    "ANSWER\\%s%d.exe  < ANSWER\\%s%d\\%d.stdin >  ANSWER\\%s%d\\%d.stdout",
                    hw_series, string_index, hw_series, string_index, test_index, hw_series, string_index, test_index
                );
                system(cmd_buffer);
            } else {
                printf("\n%d번 문제의 %d번 답을 입력하세요: ", string_index, test_index);
                sprintf(cmd_buffer, "CALL notepad ANSWER\\%s%d\\%d.stdout", hw_series, string_index, test_index);
                system(cmd_buffer);
            }
        }
    }
}
