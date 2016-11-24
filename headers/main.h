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
#include <sys/time.h>
#include <utime.h>
#include <time.h>
#include <stddef.h>
#include <semaphore.h>

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

typedef struct
{
   sem_t* sema;
   pthread_mutex_t mutex;
   char* path;
   int size;
   int fd;
   int mode;
}
argument;

char* modeReading(char* perm);

long long convertOctalToDecimal(long octalNumber);

int createFile(char* path, int size, int fd, int mode);

void *createFileP(void* arg);
