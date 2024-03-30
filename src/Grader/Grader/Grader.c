#include "Grader.h"


int main(int argc, char * argv[]) {
    // 콘솔 설정
    system("COLOR 06");
    system("TITLE 컴퓨터프로그래밍3 과제 채점 프로그램");

    // 경로 존재 여부 확인
    PathList path = parse_path("path.config");

    // 실행 모드 설정
    int programmode = 0;
    if (argc > 1) {
        if (strcmp(argv[1], "automate") == 0) {
            programmode = 3;
        }
    }
    if (programmode == 3) {
        printf("INFO: 자동 실행 모드로 진행합니다.\n\n");  // TODO: 로깅 모드 구현
        programmode = 2;
    } else {
        while (1) {
            get_option("[1] 교수님 과제 채점 모드\n[2] 과제 제출 전 학생 확인 모드 (단계마다 PAUSE)\n\n\n>>> 실행 모드를 설정해주세요: ", &programmode);
            if (programmode == 1 || programmode == 2) {
                system("CLS"); break;
            }
            else {
                show_error("ERROR: 1과 2만 입력할 수 있습니다.\n\n");
            }
        }

        // 시작
        while (1) {
            int choice = 0;
            get_option(
                "채점을 시작합니다.\n> HOMEWORK 폴더에 \"학번_이름\"의 형태로 폴더를 만들어 소스파일을 저장해주세요.\n\n\n\n[1] 새 검증식 입력\n[2] 기존 검증식 사용\n\n\n>>> 채점 옵션을 선택해 주세요: ",
                &choice
            );

            if (choice == 1) {  // 1번 선택시 새로운 스트링 입력
                set_new_answer(path);
                printf("\n\n\n"); system("PAUSE"); system("CLS");
            } else if (choice == 2) {  // 2번 선택시 기존 스트링 사용
                break;
            } else {  // 1과 2 이외의 것 입력시
                show_error("1과 2만 입력할 수 있습니다.\n\n");
            }
        } system("CLS");
    }

    // 디렉토리 불러오기
    // - cpp확장자 c로 고치기
    printf("cpp확장자가 있는지 확인하고 c로 변경합니다.\n");
    system("cd HOMEWORK && FOR /r %x in (*.cpp) do REN \"%x\" *.c");
    // - 디렉토리 리스트 파일 생성
    // - 참고 : m.blog.naver.com/PostView.nhn?blogId=dhcho347&logNo=220046884980&proxyReferer=https%3A%2F%2Fwww.google.com%2F
    printf("\n\n디렉토리 리스트 파일을 생성합니다.\n\n");
    system("DIR /a:d /b HOMEWORK > dir_list.txt");
    // - 디렉토리 저장 배열 생성 및 리스트 불러오기
    printf("디렉토리 리스트를 불러옵니다.\n");
    char dir_list[STUDENT_MAX][STUDENT_NAME] = { 0 };  // 최대 100명까지 비교, 51자리 이름까지
    int dir_count = 0;  // 디렉토리 개수
    FILE * dirs = NULL;
    dirs = fopen("dir_list.txt", "r+");
    if (dirs != NULL) {
        for (int i = 0; !feof(dirs); i++) {
            fgets(dir_list[i], sizeof(dir_list[i]), dirs);
            for (int j = 0; j < STUDENT_NAME; j++) {
                if (dir_list[i][j] == '\n') {
                    dir_list[i][j] = '\0';
                    dir_count++;
                    printf("%d개의 디렉토리를 불러왔습니다.\n", dir_count);
                    break;
                }
            }
        }
        fclose(dirs);
    }
    system("DEL dir_list.txt");
    if (programmode == 2) {
        system("PAUSE");
    }

    // 컴파일
    system("COLOR 07");
    for (int directory = 0; directory < dir_count; directory++) {
        compile_sources(path, NULL, dir_list[directory], programmode == 2 /* if debug then true */);
    }
    system("COLOR 06");

    // 답안 목록 불러오기
    system("CLS");
    printf("답안 목록 파일을 생성합니다.\n\n");
    system("DIR /a:d /b ANSWER > hw_list.txt");
    printf("답안 목록을 불러옵니다.\n\n");
    char hw_list[MAX_HOMEWORK][HOMEWORK_NAME] = { 0 };  // 최대 9 문제까지
    int hw_count = 0;  // 과제 개수
    FILE * hws = fopen("hw_list.txt", "r+");
    if (hws != NULL) {
        for (int i = 0; !feof(hws); i++) {
            fgets(hw_list[i], sizeof(hw_list[i]), hws);
            for (int j = 0; j < HOMEWORK_NAME; j++) {
                if (hw_list[i][j] == '\n') {
                    hw_list[i][j] = '\0';
                    hw_count++;
                    printf("%d개의 과제를 불러왔습니다.\n", hw_count);
                    break;
                }
            }
        }
        fclose(hws);
    }
    system("DEL hw_list.txt");
    if (programmode == 2) {
        system("PAUSE");
    }

    // 테스팅 진행
    system("CLS");
    printf("INFO: 결과 저장소를 초기화 합니다.\n");
    system("RMDIR /s RESULT_GRADE");
    system("MKDIR RESULT_GRADE");
    printf("INFO: 테스팅 시작 ------------------------------------------------------------------------\n");
    for (int directory = 0; directory < dir_count; directory++) {
        test_excutables(hw_count, hw_list, dir_list[directory], programmode == 2 /* if debug then true */);
    }

    // 결과 비교
    system("copy result_template.html RESULT_GRADE\\SCORE_OUT.html");  // HTML 템플릿 복사
    FILE * score = fopen("RESULT_GRADE\\SCORE_OUT.html", "a+");
    print_answer(score, hw_count, hw_list);  // 답안 출력
    system("CLS");
    printf("INFO: 결과 비교 시작 ---------------------------------------------------------------------\n");
    for (int directory = 0; directory < dir_count; directory++) {
        process_text(score, hw_count, hw_list, dir_list[directory], programmode == 2 /* if debug then true */);
    }
    fprintf(score, "</div>\n</body>\n</html>\n");  // HTML 파일 마무리
    fclose(score);

    // 결과 출력
    system("PAUSE");
    system("CLS");
    system("COLOR 07");
    printf("INFO: 채점이 완료되어 결과를 출력합니다.\n종료하려면 아무 키나 누르세요.\n\n");
    system("CALL RESULT_GRADE\\SCORE_OUT.html");
    //system("PAUSE");

    // 메모리 해제
    free_path(path);
    return 0;
}


PathList parse_path(char * config_file) {
    FILE * env = fopen(config_file, "r");
    PathList path = {"", "", ""};
    char ** list[] = {
        &(path.msvc), &(path.gcc), &(path.cmake)
    };
    char * list_name[] = {
        "MSVC ", "GCC ", "CMAKE "
    };
    char * list_excutable[] = {
        "\\VsDevCmd.bat", "\\gcc.exe", "\\cmake.exe"
    };
    system("COLOR F5");

    printf("경로 변수를 확인합니다.\n\n");

    if (env != NULL) {
        for (short i = 0; !feof(env); i++) {
            char buffer[PATH_LEN_MAX];
            if (fgets(buffer, sizeof(buffer), env) == NULL) {
                break;
            }

            for (short j = 0; j < sizeof(list_name)/sizeof(char *); j++) {
                char * found = strstr(buffer, list_name[j]);
                if (found != NULL) {
                    found += strlen(list_name[j]);
                    found[strlen(found) - 1] = '\0';  // 마지막 개행문자 제거
                    found += 2;  // 환경변수 표시 제거
                    if (!is_exist(found, list_excutable[j], 1)) {
                        printf("ERROR: 지정된 ");
                        printf("%s", list_name[j]);
                        printf("경로가 잘못되었습니다.\n");
                        printf("> %s is not found\n\n", found);
                    } else {
                        printf("INFO: 지정된 ");
                        printf("%s", list_name[j]);
                        printf("경로가 확인되었습니다.\n\n");
                    }
                    *list[j] = (char *)malloc(strlen(found));
                    if (*list[j] == NULL) {
                        printf("ERROR: 메모리 할당에 실패했습니다.\n");
                        system("PAUSE");
                        exit(1);
                    }
                    else {
                        strcpy(*list[j], found);
                    }
                    break;
                }
            }
        }
        fclose(env);
    } else {
        printf("ERROR: 경로 설정 파일을 찾을 수 없습니다.\n\n");
        system("PAUSE");
        exit(1);
    }
    
    system("PAUSE");
    system("COLOR 06");
    system("CLS");

    return path;
}


void free_path(PathList path) {
    char * msvc = path.msvc;
    path.msvc = NULL;
    char * gcc = path.gcc;
    path.gcc = NULL;
    char * cmake = path.cmake;
    path.cmake = NULL;

    //free(msvc);
    //free(gcc);
    //free(cmake);  // 왜 오류가 나는지 모르겠음
}
