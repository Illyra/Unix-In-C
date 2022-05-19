/* Sources Used: https://www.mkssoftware.com/docs/man3/dup2.3.asp
https://www.ibm.com/docs/en/zos/2.3.0?topic=functions-dup2-duplicate-open-file-descriptor-another
https://www.ibm.com/docs/en/i/7.2?topic=ssw_ibm_i_72/apis/dup2.htm
*/

#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <assert.h>
#include <errno.h>
#include <limits.h>

int my_dup2_with_fcntl(int oldfd, int newfd);

int my_dup2_with_fcntl(int oldfd, int newfd) {
    int fd = fcntl(oldfd, F_GETFL);
    if (fd == -1) {
        errno = EBADF;
        return -1;
    }
    else {
        if (oldfd == newfd) {
            return oldfd;
        }
        else {
            close(newfd);
            return fcntl(oldfd, F_DUPFD, newfd);
        }
    }

}

int main(int argc, char const *argv[]) {
    
    char *buf;
    FILE *fp;
    fp = fopen (argv[2], "w");
    int fd = open("test.txt", O_RDONLY);
    //sprintf(argv[2], "proc/self/fd/%d", argv[1]);
  
    int newfd2 = atoi(argv[1]);
    int newfd1 = my_dup2_with_fcntl(fd, fd);
    printf("old fd: %d\n", newfd1);
    newfd2 = my_dup2_with_fcntl(fd, newfd2);
    printf("requested new duped fd: %d\n", newfd2);

    printf("proc/self/fd/%d:", newfd2);
    buf = (char *) malloc(100*sizeof(char));
    getcwd(buf, 100);
    printf("new fd file path: \n %s/%s \n", buf, argv[2]);

    close(fp);
    close(newfd1);
    close(newfd2);

    return 0;
}