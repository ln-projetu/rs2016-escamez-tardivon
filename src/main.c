#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include "../headers/main.h"

int main(int argc, char const *argv[]) {
  if (argc < 2) {
    printf("Not enough arguement\n");
    printf("Usage: tarlist filename\n");
    exit(1);
  }
  int fd;
  fd = open(argv[1], O_RDONLY, 0);
  printf("The file descriptor is open: %d\n", fd);
  ustar buffer;
  read(fd, &buffer, 512);
  printf("%s\n",buffer.name);
  //printf("%s\n",buffer.size);
  lseek(fd, 512, SEEK_CUR);
  while (*buffer.size!=0) {
    read(fd, &buffer, 512);
    printf("%s\n",buffer.name);
    //printf("%s\n",buffer.size);
    lseek(fd, 512, SEEK_CUR);
  }

  return 0;
}
