#include "../headers/main.h"

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
          printf("d%s ",modeReading(buffer.mode+4));
        }else if(atoi(buffer.typeflag)==2){
          printf("l%s ",modeReading(buffer.mode+4));
        }else{
          printf("-%s ",modeReading(buffer.mode+4));
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

        struct utimbuf *new_times = malloc(sizeof(struct utimbuf));
        timecrop=strdup(buffer.mtime);
        timecrop=strtok(timecrop," ");
        new_times->modtime = convertOctalToDecimal(atol(timecrop));
        utime(buffer.name, new_times);
        free(new_times);
        chown(buffer.name, atoi(buffer.uid), atoi(buffer.gid));
      }
      else if (atoi(buffer.typeflag) == 2) {
        symlink(buffer.linkname, buffer.name);

        struct utimbuf *new_times = malloc(sizeof(struct utimbuf));
        timecrop=strdup(buffer.mtime);
        timecrop=strtok(timecrop," ");
        new_times->modtime = convertOctalToDecimal(atol(timecrop));
        utime(buffer.name, new_times);
        free(new_times);
        chown(buffer.name, atoi(buffer.uid), atoi(buffer.gid));
      }
      else {
        createFile(buffer.name, size, fd, perm);

        /*struct utimbuf *new_times = malloc(sizeof(struct utimbuf));
        timecrop=strdup(buffer.mtime);
        timecrop=strtok(timecrop," ");
        new_times->modtime = convertOctalToDecimal(atol(timecrop));
        utime(buffer.name, new_times);
        free(new_times);*/
        chown(buffer.name, atoi(buffer.uid), atoi(buffer.gid));
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

  return 0;
}
