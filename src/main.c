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
#include <math.h>

long long convertOctalToDecimal(int octalNumber)
{
    int decimalNumber = 0, i = 0;

    while(octalNumber != 0)
    {
        decimalNumber += (octalNumber%10) * pow(8,i);
        ++i;
        octalNumber/=10;
    }

    i = 1;

    return decimalNumber;
}

//in this order :  ptar -x -c "target file"

int main(int argc, char *argv[]) {

  int fd;
  fd = open(argv[argc-1], O_RDONLY, 0);

  if(argc<2){
    printf("No target file\n");
    exit(EXIT_FAILURE);
  }
  if (fd==-1) {
    printf("No target file\n");
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
                     nb_threads=atoi(optarg);
                     if(nb_threads){
                       pflag=1;
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








  printf("The file descriptor is open: %d\n", fd);
  ustar buffer;
  int init = 1;
  char dest[11];
  int size = 0;

  while(init) {
    read(fd, &buffer, 512); //Note: pour visualiser les bytes %02X

    strncpy(dest, buffer.size,11);
    size = convertOctalToDecimal(atoi(dest));
    //printf("size: %d\n",size);
    //printf("flag: %s\n",buffer.typeflag);

    if (size == 0) {
       if (atoi(buffer.typeflag) == 5) {
         printf("%s\n",buffer.name);
       }
       else {
         init = 0;
       }
    }
    else {
      printf("%s\n",buffer.name);
    }
    //printf("Avancement de :%d\n",(int) (512* ceil((double)size/512.0))); //Problème avec les valeurs en octal
    lseek(fd,(int) (512* ceil((double)size/512.0)), SEEK_CUR);
  }
  return 0;
}
