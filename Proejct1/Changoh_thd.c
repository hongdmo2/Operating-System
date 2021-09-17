/*ChangohHong 23104347 */

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>// using p thread

#include <sys/wait.h> // for the wait() that parent process waiting for child process.

int func_cpfile(const char *srcname, const char *tgtname)
{
    //  in c char array is represented in vector and it passes as pointer.
    // called by parent process

    int f1, f2;
    //file f1 is source folder and f2 is targer folder.

    if ((f1 = open(srcname, O_RDONLY, 0)) == -1)
    {                                        //open the source folder f1 for reading only. R_RDONLY is read only
        printf("Can't open %s \n", srcname); // error messeage will be printed if system call fails and returns -1
        return 4;
    }

    if ((f2 = creat(tgtname, 0644)) == -1)
    {                                          // create the target folder f2. 0644 is (owning)user : read and write, Group : read, Other: read
        printf("Can't create %s \n", tgtname); // error messege will be printed if system call fails and return -1
        return 5;
    }

    int SIZ = 128;                       // size is 128 byte
    char buf[SIZ];                       // buffer with size of 128 is declared
    int n;                               // read() will be into integer n
    while ((n = read(f1, buf, SIZ)) > 0) // copy the source file to the target file in while loop.
        if (write(f2, buf, n) != n)
        {
            printf("Can't write file"); // error messege print out if it can't wrtie the file.
            close(f1);                  // closing the f1, f2
            close(f2);

            return 6;
        }
    close(f1);                      // closing the open
    close(f2);                      //and written files
    printf("Success! By parent\n"); // Parent tgt folder is successfully created.

    return 0;
}
// fork and exec system calls

int main(int argc, const char *argv[])
{
    //cpfile.exe, srcname, tgt1file, tgt2file
    if (argc != 4)
    {                                                       // number of argument should be 4 and file from the chil process should be placed 4th argument.
        printf("Wrong number of command line arguments\n"); // error message
        //exit(1);
        return 3;
    }

    pid_t pid; // declare pid, process ID

    func_cpfile(argv[1], argv[2]); // Child process is created and it is copied from the parent process, argv[1] is sourcefile and argv[2] is targetfile

    pid = fork(); //this returns the value of the fork() system call.

    if (pid < 0)
    {                                   // if pid is less than 0 it fails to create the child process
        fprintf(stderr, "Fork Failed"); // when it fails, error messege prints
        return 1;
    }
    else if (pid == 0)
    { //  child is processed. child process has its own pid.

        int ret = execlp("./cpfile.exe", "cpfile.exe", argv[1], argv[3], NULL); // if success go to else. if it fails it returns.  Child process only runs through the cpfile.exe.
        fprintf(stderr, "execlp Failed %d", ret);                               // execlp funtion is used to overlay a process image that has been created by a call to the fork function.
        return 2;
    }

    else
    {
        wait(NULL);               // parent process is waiting for the child process is completed.
        printf("Child complete"); //child process is completed
    }
    return 0;
}
