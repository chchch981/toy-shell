#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_LEN_LINE    100
#define BUFSIZE 10000
int main(void)
{
    char argv[MAX_LEN_LINE];
    char command[MAX_LEN_LINE];
    char *args[] = {command, argv, NULL}; //command 는 주소, 실행 파일 모두 포함 ex /bin/ls
    char rootdir[BUFSIZ];
    

    int ret, status;
    pid_t pid, cpid;
    
    getcwd(rootdir, sizeof(rootdir));
    while (true) {
        char *s;
        int rd;
        int len;
        int bufsize;
        char buf[BUFSIZE]; // 경로 저장될곳
        char chbuf[BUFSIZE];
      


        getcwd(buf, sizeof(buf));
        strcpy(chbuf, buf);

        printf("\033[1;32m MyShell: %s $\033[0m", buf);
        s = fgets(command, MAX_LEN_LINE, stdin);
      //  printf("%s", s); 추가
        if (s == NULL) {
            fprintf(stderr, "fgets failed\n");
            exit(1);
        }
        
        len = strlen(command);
        printf("command length  : %d\n", len);
        if (command[len - 1] == '\n') {
            command[len - 1] = '\0'; 
        }
        
        printf("\033[1;33m [%s]\033[0m\n", command);
       
        if (!strncmp(command, "cd", 2)) {
            //printf("")////--------------------------------change directory-----------------   buf 는 현재경로
            if (!strcmp(command, "cd ..")) {
                
                strcat(chbuf, "/..");
                rd = chdir(chbuf);
                if (!rd)
                    printf("-- change dir -- \n");
                //getcwd(buf, sizeof(buf));
                //printf("\033[1;32m)) MyShell: %s $\033[0m\n\n", buf);
            }
            else {
                //strcpy(chbuf, command[3:len]);
                for (int i = 3; i < len; i++)
                    chbuf[i - 3] = command[i];
                rd = chdir(chbuf);
                if (!rd)
                    printf("-- change dir -- \n");
                else
                    printf("check your path");
            }
        }

        pid = fork();


        if (pid < 0) {
            fprintf(stderr, "fork failed\n");
            exit(1);
        } 
        if (pid != 0) {  /* parent */
            if (!strcmp(command, "exit")) {
                //printf("")////-----------------exit    
                printf("exit shell\n");
                break;
            }
            cpid = waitpid(pid, &status, 0); //자식이 끝나면 자식이 사라짐 사라지고 바로 진행
            if (cpid != pid) {
                fprintf(stderr, "waitpid failed\n");        
            }
            printf("Child process terminated\n"); // 정상종료시 exitstauts 출력
            strcpy(command, "NULL");
            strcpy(args[1], "NULL");
            if (WIFEXITED(status)) {
                printf("Exit status is %d\n", WEXITSTATUS(status)); 
            }
        }
        else {  /* child */ 
            //printf("child process ID : %d", pid);
            //-------------------------bj
            if (strncmp(command, "ls", 2) == 0) { ////------------------list
                
                if (!strcmp(command, "ls -i")) {
                    strcat(rootdir, "/list");
                    strcpy(command, rootdir);
                    strcpy(args[1], "-i");
                    printf("1\n");
                    ret = execve(args[0], args, NULL);
                }
                if (!strcmp(command, "ls -l")) {
                    strcat(rootdir, "/list");
                    strcpy(command, rootdir);
                    strcpy(args[1], "-l");
                    printf("2\n");
                    ret = execve(args[0], args, NULL);

                }
                if (!strcmp(command, "ls")) {
                    strcat(rootdir, "/list");
                    strcpy(command, rootdir);
                    strcpy(args[1], "NULL");
                    printf("3\n");
                    ret = execve(args[0], args, NULL);
                    
                }
                else
                    printf("\npress -help for info\n");
             }

            if (!strcmp(command, "pwd")) { ////-----------------pwd 
                strcat(rootdir, "/pwd");
                strcpy(command, rootdir);
                //strcpy(command, "./pwd");
                ret = execve(args[0], args, NULL);
                strcpy(command, "NULL");
            }
            if (!strcmp(command, "-help")) { ////-----------------help
                strcat(rootdir, "/help");
                strcpy(command, rootdir);
                //strcpy(command, "./help");
                ret = execve(args[0], args, NULL);
                strcpy(command, "NULL");
            }
            if (!strcmp(command, "clear")) {
                system("clear");
                ret = 0;
            }

            //else { ////-----------------지원하지 않는경우
            //    ret = execve(args[0], args, NULL);
            //    printf("\npress -help for info\n");
            //    strcpy(command, "NULL");
            //}
             //---------------------------------------------


            ret = execve(args[0], args, NULL); //만들지 않은 명령어 입력시 통과
            if (!strncmp(command, "cd", 2)) {

                ret = 0;
                printf("ret : %d\n", ret);
            }
            if (ret < 0) {
                printf("\npress -help for info\n");
                fprintf(stderr, "execve failed\n");   
                return 1;
            }
        } 
    }
    return 0;
}
