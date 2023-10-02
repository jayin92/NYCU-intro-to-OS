#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

void show(int fork_id) {
    printf("Fork %d. I'm the child %d, my parent is %d.\n", fork_id, getpid(), getppid());
}

int main() {
    printf("Main Process ID: %d\n\n", getpid());
    pid_t pid;
    pid = fork();
    if (pid == 0){
        show(1);
    }
    wait(NULL);
    pid = fork();
    if(pid == 0){
        show(2);
    } else {
        wait(NULL);
        pid = fork();
        if(pid == 0){
            show(3);
        }
        wait(NULL);
        pid = fork();
        if(pid == 0){
            show(4);
        } else {
            wait(NULL);
        }
    }
    return 0;
}
