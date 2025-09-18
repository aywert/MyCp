#ifndef IO_FUNC
#define IO_FUNC

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

ssize_t Write(int fd, char buf[], ssize_t size);
ssize_t Read(int fd, char buf[], ssize_t size);
int Open(char * filename, int o_flags, ...);
void CopyFile(int fd_read, int fd_write, char* buf);

#endif