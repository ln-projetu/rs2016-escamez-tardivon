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

char* modeReading(char indice) {
  char* result;
  switch (indice) {
    case '0':
      result = "---";
      break;
    case '1':
      result = "--x";
      break;
    case '2':
      result = "-w-";
      break;
    case '3':
      result = "-wx";
      break;
    case '4':
      result = "r--";
      break;
    case '5':
      result = "r-x";
      break;
    case '6':
      result = "rw-";
      break;
    case '7':
      result = "rwx";
      break;
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

int createFile(char* path) {
    //Create a file
    strcat("./", path);
    int buffer[0];
    int size_buffer = 0;
    int fd = open(path, O_CREAT, 0);
    write(fd, buffer, size_buffer);
    return 0;
}
