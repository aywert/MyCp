#include "./io.h"
#include <unistd.h>
#include <getopt.h>
#include <sys/stat.h>
#include <string.h>

extern char *optarg;
extern int optind, opterr, optopt;

struct option long_options[] = {
    {"force",    no_argument,       0, 'f'}, 
    {"verbose",  no_argument,       0, 'v'},
    {"interactive",   no_argument, 0, 'i'},
    {0, 0, 0, 0}
};

enum flags
{
  force, 
  verbose,
  interactive,
};


int main(int argc, char* argv[])
{
  int opt;
  int flag = 0;
  int option_index = 0;

  while ((opt = getopt_long(argc, argv, "fvi", long_options, &option_index)) != -1) 
  {
    switch (opt) 
    {
      case 'v':
        flag = verbose;
        break;

      case 'f':
        flag = force;
        break;

      case 'i':
        flag = interactive;
        break;
      default:
        return 0;
    }
  }

  struct stat file_stat;
  for (int i = 0; i < argc; i++)
  {
    printf("%s ", argv[i]);
  }

  printf("\nit is me: %s\n", argv[argc-1]);
  if (stat(argv[argc-1], &file_stat) == -1) 
  {
    perror("stat");
    return 0;
  }

  if (argc-optind == 1)
  {
    fprintf(stderr, "MyCp: missing file operand\n");
    exit(-1);
  }

  // if (argc-optind == 2)
  // {
  //   fprintf(stderr, "MyCp: missing distination file operand after '%s'\n", argv[argc-1]);
  //   exit(-1);
  // }

  if (argc-optind > 3 && !S_ISDIR(file_stat.st_mode))
  {
    fprintf(stderr, "MyCp: target '%s': Not a directory\n", argv[argc-1]);
    exit(-1);
  }

  char buf[128];
  if (S_ISDIR(file_stat.st_mode)) // determening that the last string referring to dir
  {
    for (int i = optind; i < argc -1; i++)
    {
      strcpy(buf, argv[argc-1]);
      buf[strlen(buf)] = '/';
      char * result = strcat(buf, argv[i]);

      int fd_1 = open(argv[i], O_RDONLY);
      if (fd_1 < 0)
      {
        fprintf(stderr, "MyCat: %s: %s\n", argv[i], strerror(errno));
        exit(-1);
      }

      int fd_2 = open(result, O_WRONLY|O_CREAT, 0666);
      if (fd_2 < 0)
      {
        fprintf(stderr, "MyCat: %s: %s\n", result, strerror(errno));
        exit(-1);
      }

      CopyFile(fd_1, fd_2, buf);

      close(fd_1);
      close(fd_2);
    }
  }

  if(S_ISREG(file_stat.st_mode))
  {
    int fd_1 = Open(argv[optind], O_RDONLY); //openning file to copy 
    int fd_2 = Open(argv[optind + 1], O_WRONLY|O_CREAT, 0666);

    CopyFile(fd_1, fd_2, buf);

    close(fd_1);
    close(fd_2);
  }

  //for (int i = optind; i < argc; i++) {
    
   // printf("  %s\n", argv[i]);
  //}

  // printf("\n");
  // for (int i = 0; i < argc; i++)
  // {
  //   printf("%s ", argv[i]);
  // }
  // printf("\n");


  return 0;
}