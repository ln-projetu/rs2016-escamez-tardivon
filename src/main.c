#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>

int main(int argc, char const *argv[]) {
  if (argc < 2) {
    printf("Not enough arguement\n");
    printf("Usage: tarlist filename\n");
    exit(1);
  }
  int fd;
  fd = open(argv[1], O_RDONLY, 0);
  printf("The file descriptor is open: %d\n", fd);
  char buffer[512];
  read(fd, buffer, 512);
  printf("%s\n",buffer);
  read(fd, buffer, 512);
  printf("%s\n",buffer);
  return 0;
}
