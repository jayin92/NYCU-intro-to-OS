#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(){
    char *str = malloc(100);
    
    pid_t pid;
    while(1){
        printf("osh> ");
        char c;
        int idx = 0;
        while((c = getchar())){
            if(c == '\n') break;
            str[idx++] = c;
        }
        str[idx] = '\0';
        if(strcmp(str, "exit") == 0){
            printf("Process end\n");
            exit(0);
        }
        char *cmd = malloc(100);
        char *arg1 = malloc(100);
        char *arg2 = malloc(100);
        str[idx] = '\0';
        char* token = strtok(str, " ");
        int cnt = 0;
        while(token != NULL){
            if(cnt == 0){
                strcpy(cmd, token);
            } else if(cnt == 1){
                strcpy(arg1, token);
            } else if(cnt == 2){
                strcpy(arg2, token);
            }
            token = strtok(NULL, " ");
            cnt++;
        }
        pid = fork();
        if(pid == 0){
            char* path = malloc(strlen(cmd) + 6);
            strcpy(path, "/bin/");
            strcat(path, cmd);
            if(strlen(arg1) == 0){
                execlp(path, cmd, NULL);
            } else if(strlen(arg2) == 0){
                execlp(path, cmd, arg1, NULL);
            } else {
                execlp(path, cmd, arg1, arg2, NULL);
            }
            exit(0);
        } else {
            wait(NULL);
        }
    }
}
