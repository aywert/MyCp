#include "./io.h"
#include <unistd.h>
#include <getopt.h>
#include <sys/stat.h>

extern char *optarg;
extern int optind, opterr, optopt;

struct option long_options[] = {
    {"force",    no_argument,       0, 'f'},  // --force
    {"verbose",  no_argument,       0, 'v'},
    {"interactive",   required_argument, 0, 'i'},
    {0, 0, 0, 0}  // Завершающий элемент
};


int main(int argc, char* argv[])
{
  struct stat file_stat;
  printf("%s\n", argv[argc-1]);
  if (stat(argv[argc-1], &file_stat) == -1) 
  {
    perror("stat");
    return 0;
  }

  char buf[128];
  if (S_ISDIR(file_stat.st_mode)) // determening that the last string referring to dir
  {
    for (int i = 1; i < argc -1; i++)
    {
      strcpy(buf, argv[argc-1]);
      buf[strlen(buf)] = '/';
      char * result = strcat(buf, argv[i]);

      int fd_1 = Open(argv[i]); //openning file to copy 
      int fd_2 = open(result, O_WRONLY|O_CREAT, 0777);
      printf("fd_2 = %d\n", fd_2);
      printf("result = %s\n", result);

      close(fd_1);
      close(fd_2);
    }
  }

  if (argc == 1)
  {
    fprintf(stderr, "MyCp: missing file operand\n");
    exit(-1);
  }

  int opt;
  int option_index = 0;

  while ((opt = getopt_long(argc, argv, "fv:h", long_options, &option_index)) != -1) 
  {
    //printf("option = %d\n", opt);
    switch (opt) 
    {
      case 'v':
        printf("Option 'v' found\n");
        break;
      case 'f':
        printf("Option 'f' found\n");
        break;
      case 'i':
        printf("Option 'i' found %s\n", optarg);
        break;
      default:
      {
        return 0;
      }
    }
  }


  return 0;
}