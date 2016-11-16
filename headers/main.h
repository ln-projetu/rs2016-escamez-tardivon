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
#include <math.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <utime.h>


typedef struct header_posix_ustar ustar;
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

char* modeReading(char* perm) {
  char* result = malloc(sizeof(char) * 12);

  for (int i = 0; i<3; i++) {
    char indice = perm[i];

    char* temp;
    switch (indice) {
      case '0':
        temp = "---";
        break;
      case '1':
        temp = "--x";
        break;
      case '2':
        temp = "-w-";
        break;
      case '3':
        temp = "-wx";
        break;
      case '4':
        temp = "r--";
        break;
      case '5':
        temp = "r-x";
        break;
      case '6':
        temp = "rw-";
        break;
      case '7':
        temp = "rwx";
        break;
    }

    strcat(result, temp);
  }

  return result;
}


long long convertOctalToDecimal(long octalNumber)
{
    long decimalNumber = 0, i = 0;

    while(octalNumber != 0)
    {
        decimalNumber += (octalNumber%10) * pow(8,i);
        ++i;
        octalNumber/=10;
    }

    i = 1;

    return decimalNumber;
}

int createFile(char* path, int size, int fd, int mode) {
    //Create a file
    int fd2 = open(path, O_CREAT | O_WRONLY, mode);
    char buffer[size-1];
    read(fd, &buffer, size);
    write(fd2, &buffer, size);
    close(fd2);
    return 0;
}
