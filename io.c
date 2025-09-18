#include "./io.h"

ssize_t Write(int fd, char buf[], ssize_t size)
{
  size_t n = write(fd, buf, size);

  if(n < 0)
  {
    fprintf(stderr, "MyCat: %s\n", strerror(errno));
    exit(-1);
  }

  return n;
}

ssize_t Read(int fd, char buf[], ssize_t size)
{
  int n = read(fd, buf, size);
  if(n < 0)
  {
    fprintf(stderr, "MyCat: %s\n", strerror(errno));
    exit(-1);
  }

  return n;
}

int Open(char * filename)
{
  int fd = open(filename, O_RDONLY);
    if (fd < 0)
    {
      fprintf(stderr, "MyCat: %s: %s\n", filename, strerror(errno));
      exit(-1);
    }

  return fd;
}