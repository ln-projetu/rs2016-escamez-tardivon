#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <getopt.h>
#include "../headers/main.h"

//in this order :  ptar -x -c "test.txt"

int main(int argc, char *argv[]) {

  /*if (argc < 2) {                                       useless for now
    printf("Not enough arguments\n");
    printf("Usage: tarlist filename\n");
    exit(1);
  }*/

int opt;
int xflag=0;
int lflag=0;
int pflag=0;
int zflag=0;
int nb_threads=0;

while((opt = getopt(argc, argv, "xlp:z")) != -1) {
  switch (opt) {
               case 'x':
                   xflag=1;
                   break;
               case 'l':
                   lflag=1;
                   break;
               case 'p':
                   pflag=1;
                   nb_threads=atoi(optarg);
                   break;
               case 'z':
                   zflag=1;
                   break;
               case '?' :
                   if (optopt=='p'){
                        printf ("Option -%c requires an argument.\n", optopt);
                   }else{
                     printf("invalid option\n");
                  }
                   exit(EXIT_FAILURE);
               }
}
printf ("xflag = %d, lflag = %d, pflag = %d, nb_threads = %d, zflag = %d \n", xflag, lflag, pflag, nb_threads, zflag);


  int fd;
  fd = open(argv[argc-1], O_RDONLY, 0);
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
