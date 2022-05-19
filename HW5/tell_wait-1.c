#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include "apue.h"
#include <errno.h>
#include <signal.h>
#include <assert.h>

#define readwrite (S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH)

static volatile sig_atomic_t sigflag; 
static sigset_t newmask, oldmask, zeromask;

static void
sig_usr(int signo)  
{
    sigflag = 1; 
}

void
TELL_WAIT(void)
{
    if (signal(SIGUSR1, sig_usr) == SIG_ERR)
        err_sys("signal(SIGUSR1) error");
    if (signal(SIGUSR2, sig_usr) == SIG_ERR)
        err_sys("signal(SIGUSR2) error");
    sigemptyset(&zeromask);
    sigemptyset(&newmask);
    sigaddset(&newmask, SIGUSR1);
    sigaddset(&newmask, SIGUSR2);
    if (sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0)
        err_sys("SIG_BLOCK error");
}

void
TELL_PARENT(pid_t pid)
{
    kill(pid, SIGUSR2);
}

void
WAIT_PARENT(void)
{

    while (sigflag == 0)
        sigsuspend(&zeromask); 

    sigflag = 0;
}


void
TELL_CHILD(pid_t pid)
{
    kill(pid, SIGUSR1);
}

void
WAIT_CHILD(void)

{
    while (sigflag == 0)
        sigsuspend(&zeromask);  
    sigflag = 0;
}

void END(void)
{
    if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0)
        err_sys("SIG_SETMASK error");
}

int counter;
int buf[1];
int main(int argc, char const *argv[])
{
    int pid;
    counter = 0;
    TELL_WAIT();
    printf("TELL_WAIT: counter: %d pid: %d\n", counter,getpid());
    umask(0);
    creat("./tmp.txt", readwrite);
    int fd = open("./tmp.txt", O_CREAT|O_RDWR, readwrite);
    write(fd, &counter, 4);
    if ((pid = fork()) < 0) {
        err_sys("fork error");
    }
    else if (pid == 0)
    {
        while(1)
        {
            lseek(fd, 0, SEEK_SET);
            read(fd, buf, 4);
            printf("Child incremented file counter: %d\n", ++buf[0]);
            lseek(fd, 0, SEEK_SET);
            write(fd, &buf, 4);
            TELL_PARENT(getppid());     
            WAIT_PARENT();
            if (counter == 1000) {
                exit(0);
            }
        }

        END();

        exit(0); 
    }

    while(1)
    {
        WAIT_CHILD();
        lseek(fd, 0, SEEK_SET);
        read(fd, buf, 4);
        printf("Parent incremented file counter: %d\n", ++buf[0]);
        lseek(fd, 0, SEEK_SET);
        write(fd, &buf, 4);
        TELL_CHILD(pid);
        if (counter == 1000) {
            exit(0);
        } 
    }


    END();

    return 0;
}