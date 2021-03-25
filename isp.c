#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define READ_END 0
#define WRITE_END	1
#define MAX_COMMAND_SIZE 64
#define MAX_COMMAND 10

int main(int argc, char *argv[]) {
  pid_t n;

  int size_N = atoi(argv[1]);
  int mode = atoi(argv[2]);

  while (1) {
    int pipe_command = 0;
    int pipe_index = -1;

    /*double elapsed;
    struct timeval start;
    struct timeval end;
    gettimeofday(&start, NULL);*/

    //prompt
    printf("isp$ ");

    //get command from user
    char command[MAX_COMMAND_SIZE]; //command
    fgets(command, sizeof command, stdin);

    //process command here
    char parse_by[] = " ";

    char *args[MAX_COMMAND]; //arguments
    char *arg_ptr = strtok(command, parse_by);

    int word_count = 0;
    while (arg_ptr != NULL) {
      args[word_count] = arg_ptr;
      arg_ptr = strtok(NULL, parse_by);
      word_count++;
    }
    args[word_count - 1][strlen(args[word_count - 1]) - 1] = '\0';

    if (strcmp(args[0], "exit") == 0) {
      break;
    }

    char *arguments[word_count+1];
    for (int i = 0; i < word_count; i++) {
      arguments[i] = args[i];
      if (strcmp(arguments[i], "|") == 0) {
        pipe_command = 1;
        pipe_index = i;
      }
    }
    arguments[word_count] = NULL;

    if (!pipe_command) {
      n = fork();
      if (n < 0) {
    		fprintf(stderr, "Fork Failed");
    		exit(-1);
    	}
      else if (n == 0) { //child process
        execvp(arguments[0], arguments);
      }
      else {
        while (wait(NULL) > 0);
      }
    }
    else {
      char *args_1[pipe_index + 1];
      args_1[pipe_index] = NULL;
      char *args_2[word_count - pipe_index];
      args_2[word_count - pipe_index - 1] = NULL;

      for (int i = 0; i < word_count; i++) {
        if (i < pipe_index) {
          args_1[i] = arguments[i];
        }
        else if (i > pipe_index){
          args_2[i - pipe_index - 1] = arguments[i];
        }
      }

      int fd1[2];
      int fd2[2];

      if (pipe(fd1) == -1) {
        fprintf(stderr,"Pipe failed");
        return 1;
      }

      if (pipe(fd2) == -1) {
        fprintf(stderr,"Pipe failed");
        return 1;
      }

      n = fork();
      if (n == 0) { //first child
        if (mode == 1) {
          close(fd1[READ_END]);
          dup2(fd1[WRITE_END], WRITE_END);
          close(fd1[WRITE_END]);
          close(fd2[READ_END]);
          close(fd2[WRITE_END]);
          execvp(args_1[0], args_1);
        }
        else if (mode == 2){
          close(fd1[READ_END]);
          close(fd2[READ_END]);
          close(fd2[WRITE_END]);
          dup2(fd1[WRITE_END], WRITE_END);
          close(fd1[WRITE_END]);
          execvp(args_1[0], args_1);
        }
      }
      else {
        n = fork();
        if (n == 0) { //second child
          if (mode == 1) {
            close(fd1[WRITE_END]);
            dup2(fd1[READ_END], READ_END);
            close(fd1[READ_END]);
            close(fd2[READ_END]);
            close(fd2[WRITE_END]);
            execvp(args_2[0], args_2);
          }
          else if (mode == 2) {
            close(fd1[READ_END]);
            close(fd1[WRITE_END]);
            close(fd2[WRITE_END]);
            dup2(fd2[READ_END], READ_END);
            close(fd2[READ_END]);
            execvp(args_2[0], args_2);
          }
        }
        else { //parent process
          if (mode == 1) {
            close(fd1[READ_END]);
            close(fd1[WRITE_END]);
            close(fd2[READ_END]);
            close(fd2[WRITE_END]);
            while (wait(NULL) > 0);
          }
          else if (mode == 2) {
            close(fd1[WRITE_END]);
            close(fd2[READ_END]);
            char buf[size_N];
            int character_count = 0;
            int read_count = 0;
            int write_count = 0;
            int read_N;
            int write_N;
            while((read_N = read(fd1[READ_END], buf, size_N)) != 0) {
              read_count++;
              //printf("%s", buf);
              write_N = write(fd2[WRITE_END], buf, read_N);
              write_count++;
              character_count += read_N + write_N;
            }
            close(fd1[READ_END]);
            close(fd2[WRITE_END]);
            while (wait(NULL) > 0);
            printf("\ncharacter-count: %d\n", character_count);
            printf("read-call-count: %d\n", read_count);
            printf("write-call-count: %d\n", write_count);
          }
        }
      }
    }
    /*gettimeofday(&end, NULL);
    elapsed = (end.tv_sec - start.tv_sec) * 1000000 + start.tv_sec - end.tv_sec;
    elapsed += (end.tv_usec - start.tv_usec);
    printf("Time elapsed: %f\n", elapsed);*/
  }
}
