#include "./io.h"
#include <unistd.h>
#include <getopt.h>
#include <sys/stat.h>
#include <string.h>
#include <stdbool.h>

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
  no_flag,
  force, 
  verbose,
  interactive,
};


int main(int argc, char* argv[])
{
  int opt;
  int flag = no_flag;
  int option_index = 0;

  
  // if (mkdir("dir_000", 0000) == -1) {
  //     perror("Ошибка создания папки");
  //     return -1;
  // }

  if (access("dir_000", W_OK) == 0) 
  {
    printf("Есть право на запись\n");
  }
  else
  {
    printf("нет права на запись\n");
  }
  // fd = open("secret.txt", O_RDONLY);
  // char buf[128];
  // printf("fd = %d\n", fd);
  // close(fd);

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
  // for (int i = 0; i < argc; i++)
  // {
  //   printf("%s ", argv[i]);
  // }

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

  if (argc-optind > 3 && !S_ISDIR(file_stat.st_mode))
  {
    fprintf(stderr, "MyCp: target '%s': Not a directory\n", argv[argc-1]);
    exit(-1);
  }

  char buf[128]; buf[127] = '\0';
  if (S_ISDIR(file_stat.st_mode)) // determening that the last string referring to dir
  {
    for (int i = optind; i < argc -1; i++)
    {
      strcpy(buf, argv[argc-1]);
      buf[strlen(buf)] = '/';
      char * result = strcat(buf, argv[i]);

      if (flag == interactive)
      {
        printf("MyCp overwrite '%s'? ", result);
        if(getchar() != 'y')
        {
          while((getchar())!= '\n'); //cleaning buffer
          continue;
        }

        while((getchar())!= '\n'); //cleaning buffer
      }

      if (flag == force)
      {
        if (chmod(argv[argc-1], 0777) != 0)
        {
          fprintf(stderr, "MyCp: couldn't opened %s: %s\n", argv[argc-1], strerror(errno));
          
          return 0;
        }

        if (access(argv[argc-1], F_OK) != 0) 
          {
            fprintf(stderr, "MyCat: file %s does not exist\n", argv[argc-1]);
            return 1;
          }
      }

      int fd_1 = open(argv[i], O_RDONLY);
      if (fd_1 < 0)
      {
        fprintf(stderr, "MyCp: %s: %s\n", argv[i], strerror(errno));
        exit(-1);
      }

      int fd_2 = open(result, O_WRONLY|O_CREAT|O_TRUNC, 0777);
      if (fd_2 < 0)
      {
        fprintf(stderr, "MyCp: %s: %s\n", result, strerror(errno));
        exit(-1);
      }

      printf("result = %s\n", result);

      if (flag == verbose)
        printf("'%s' -> '%s'\n", argv[i], result);

      CopyFile(fd_1, fd_2, buf);
      
      // if (flag == force)
      // {
      //   if (chmod(argv[argc-1], 0000) != 0)
      //   {
      //     printf("Mistake in returning state of directory\n");
      //   }
      // }      

      for(int i = 0; i < 128; i++)
      {
        buf[i] = 0;
      }

      close(fd_1);
      close(fd_2);
    }
  }

  if(S_ISREG(file_stat.st_mode))
  {
    if (flag == interactive)
    {
      printf("MyCp overwrite '%s'? ", argv[optind+1]);
      if(getchar() != 'y')
        {
          while((getchar())!= '\n'); //cleaning buffer
          return 0;
        }

      while((getchar())!= '\n'); //cleaning buffer
    }

    int fd_1 = open(argv[optind], O_RDONLY); //openning file to copy 
    if (fd_1 < 0)
      {
        fprintf(stderr, "MyCat: %s: %s\n", argv[optind], strerror(errno));
        exit(-1);
      }

    int fd_2 = open(argv[optind + 1], O_WRONLY|O_CREAT|O_TRUNC, 0666);
    if (fd_1 < 0)
      {
        fprintf(stderr, "MyCat: %s: %s\n", argv[optind+1], strerror(errno));
        exit(-1);
      }

    if (flag == verbose)
      printf("'%s' -> '%s'\n", argv[optind], argv[optind+1]);

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