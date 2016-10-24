#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <getopt.h>


int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("Not enough arguments\n");
    printf("Usage: tarlist filename\n");
    exit(1);
  }

int opt;

while((opt = getopt(argc, argv, "xlpz")) != -1) { // : if arg required, :: if arg optionnal
  printf("avant switch %d", opt);
  switch (opt) {
    printf("avant switch %d", opt);
               case 'x':
                   printf("x\n");
                   break;
               case 'l':
                   printf("l\n");
                   break;
               case 'p':
                   printf("p\n");
                   break;
               case 'z':
                   printf("z\n");
                   break;
               case '?' :
                   printf("invalid argument\n");
               }
}
/*
struct header_posix_ustar {
                char name[100];
                char mode[8];
                char uid[8];
                char gid[8];
                char size[12];
                char mtime[12];
                char checksum[8];
                char typeflag[1];
                char linkname[100];
                char magic[6];
                char version[2];
                char uname[32];
                char gname[32];
                char devmajor[8];
                char devminor[8];
                char prefix[155];
                char pad[12];
              };

  int fd;
  struct header_posix_ustar ownstruct;
  fd = open(argv[1], O_RDONLY, 0);
  printf("The file descriptor %d is open \n", fd);
  read(fd, &ownstruct, 512);
  printf("name: %s\n",ownstruct.name);
  printf("mode: %s\n",ownstruct.mode);
  printf("uid: %s\n",ownstruct.uid);
  printf("gid: %s\n",ownstruct.gid);
  printf("size: %s\n",ownstruct.size);

  //char buffer[512];
  //read(fd, buffer, 512);
  //printf("%s\n",buffer);
  //read(fd, buffer, 512);
  //printf("%s\n",buffer);
  //printf("%s\n",&ma_struct);
  */
  return 0;
}
