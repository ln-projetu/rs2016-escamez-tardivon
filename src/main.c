#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <ctype.h>
#include <getopt.h>
#include <string.h>
#include "../headers/main.h"

//in this order :  ptar -x -c "test.txt"

int main(int argc, char *argv[]) {

  char *tab;                                       // rajout de l'exception si on oublie le fichier cible
  tab=argv[argc-1];
  if(argc<2){
    printf("No file target\n");
    exit(EXIT_FAILURE);
  }
  if (!(strncmp(&tab[0],"-",1))) {
    printf("No file target\n");
    exit(EXIT_FAILURE);
  }

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
                   printf("optarg = '%s'\n",optarg); //if optarg n'est pas un int, ca dégage.
                   if(isdigit((unsigned char) optarg)){
                     pflag=1;
                     nb_threads=atoi(optarg);
                   }else{
                     printf("Option -p requires an argument of int\n");
                     exit(EXIT_FAILURE);
                   }
                   break;
               case 'z':
                   zflag=1;
                   break;
               case '?' :
                   //if (optopt=='p'){
                  //      printf ("Option -%c requires an argument of int.\n", optopt);
                   //}else {
                     printf("must have -x -l -p or -z option\n");
                  //}
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
