#include "../headers/main.h"

char* modeReading(char* perm) {

char indice = perm[0];
static  char* temp;
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
  return temp;
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
                     printf("must have -x -l -p or -z option\n");
                     exit(EXIT_FAILURE);
               }
}
  //printf ("xflag = %d, lflag = %d, pflag = %d, nb_threads = %d, zflag = %d \n", xflag, lflag, pflag, nb_threads, zflag);

if(pflag==0){}
if(zflag==0){}
if(nb_threads==0){}


  ustar buffer;
  int init = 1;
  char dest[11];
  int size = 0;
  char * timecrop;
  char timebuff[20];
  struct tm * timeinfo;



  while(init) {
    printf("Passage de boucle 1\n");
    read(fd, &buffer, 512); //Note: pour visualiser les bytes %02X
    strncpy(dest, buffer.size,11);
    size = convertOctalToDecimal(atoi(dest));
    //printf("%s\n",buffer.magic);
    //printf("size: %d\n",size);
    //printf("flag: %s\n",buffer.typeflag);

    if (lflag==1) { //Listing detaille : -l
      if (strncmp(buffer.magic, "ustar",5) != 0) {
        init = 0;
      }
      else {
        //printf("%c\n", buffer.mode[3]);
        if(atoi(buffer.typeflag)==5){
          printf("d%s%s%s ",modeReading(buffer.mode+4),modeReading(buffer.mode+5), modeReading(buffer.mode+6));
        }else if(atoi(buffer.typeflag)==2){
          printf("l%s%s%s ",modeReading(buffer.mode+4),modeReading(buffer.mode+5), modeReading(buffer.mode+6));
        }else{
          printf("-%s%s%s ",modeReading(buffer.mode+4),modeReading(buffer.mode+5), modeReading(buffer.mode+6));
        }

        //print uid/gid
        long long lldUID=convertOctalToDecimal(atol(buffer.uid));
        long long lldGID=convertOctalToDecimal(atol(buffer.gid));
        printf("%lld/%lld ",lldUID,lldGID);

        //print size
        printf("%lld ",convertOctalToDecimal(atol(buffer.size)));

        //print modification date
        timecrop=strdup(buffer.mtime);
        timecrop=strtok(timecrop," ");
        time_t rawtime = convertOctalToDecimal(atol(timecrop));
        timeinfo = localtime(&rawtime);
        strftime(timebuff,20,"%F %X ",timeinfo);
        printf("%s ",timebuff);

        //print name
        printf("%s",buffer.name);

        //if(simlink) print('-> destination')
        if(atoi(buffer.typeflag)==2){
          printf(" -> %s\n",buffer.linkname);
        }else{
          printf("\n");
        }

      }
    }

    else { //Listing simple
      if (strncmp(buffer.magic, "ustar",5) != 0) {
        init = 0;
      }
      else {
        printf("%s\n",buffer.name);

      }

    }


    if (xflag == 1) {
      char* perm1 = buffer.mode+3;
      int perm = atoi(perm1);
      perm = convertOctalToDecimal(perm);


      if (atoi(buffer.typeflag) == 5) {
        mkdir(buffer.name, perm);

        chown(buffer.name, atoi(buffer.uid), atoi(buffer.gid));
      }
      else if (atoi(buffer.typeflag) == 2) {
        symlink(buffer.linkname, buffer.name);

        chown(buffer.name, atoi(buffer.uid), atoi(buffer.gid));

        struct timeval timeval[2];
        struct timeval const *t;
        timecrop=strdup(buffer.mtime);
        timeval[0].tv_sec = 0;
        timeval[0].tv_usec = 0;
        timeval[1].tv_sec = convertOctalToDecimal(atol(timecrop));
        timeval[1].tv_usec = 0;
        t = timeval;
        lutimes (buffer.name, t);
      }
      else {
        createFile(buffer.name, size, fd, perm);

        chown(buffer.name, atoi(buffer.uid), atoi(buffer.gid));

        struct utimbuf new_times;
        timecrop=strdup(buffer.mtime);
        new_times.modtime = convertOctalToDecimal(atol(timecrop));
        utime(buffer.name, &new_times);
      }




      lseek(fd,(int) (512* ceil((double)size/512.0)) - size, SEEK_CUR);

      //printf("Avancement de : %d\n", (int) (512* ceil((double)size/512.0)) - size);
    }
    else {
      //printf("%d\n",size);
      //printf("Avancement de :%d\n",(int) (512* ceil((double)size/512.0)));

      lseek(fd,(int) (512* ceil((double)size/512.0)), SEEK_CUR);

    }

  }
  close(fd);
  if (xflag == 1) {
    init = 1;
    int fd3 = open(argv[argc-1], O_RDONLY, 0);
    while (init) {
      printf("Passage de boucle 2\n");
      read(fd3, &buffer, 512); //Note: pour visualiser les bytes %02X
      strncpy(dest, buffer.size,11);
      size = convertOctalToDecimal(atoi(dest));

      if (strncmp(buffer.magic, "ustar",5) != 0) {
        init = 0;
        printf("init a 0\n");
      }
      if (xflag == 1) {
        char* perm1 = buffer.mode+3;
        int perm = atoi(perm1);
        perm = convertOctalToDecimal(perm);


        if (atoi(buffer.typeflag) == 5) {
          struct utimbuf new_times;
          timecrop=strdup(buffer.mtime);
          new_times.modtime = convertOctalToDecimal(atol(timecrop));
          int err = utime(buffer.name, &new_times);
          printf("%d\n", err);


        }
        else if (atoi(buffer.typeflag) == 2) {


        }
        else {

        }

        lseek(fd,(int) (512* ceil((double)size/512.0)), SEEK_CUR);

        //printf("Avancement de : %d\n", (int) (512* ceil((double)size/512.0)) - size);
      }
    }
}
  close(fd);
  return 0;
}
