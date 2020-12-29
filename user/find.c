#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

const char currdir[DIRSIZ] = ".             ";
const char parentdir[DIRSIZ] = "..            ";

char*
fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
  return buf;
}

void
find(char *path, const char *name) 
{
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;
  if((fd = open(path, 0)) < 0){
      fprintf(2, "ls: cannot open %s\n", path);
      return;
  }
  if(fstat(fd, &st) < 0){
      fprintf(2, "ls: cannot stat %s\n", path);
      close(fd);
      return;
  }
  switch(st.type){
    char *fname;
    case T_FILE:
      fname = fmtname(path);
      // printf("searching |%s|, target = |%s|, strlen = %d \n", fname, name, strlen(fname));    
      if (strcmp(fname, name) == 0) {
          printf("%s \n", path);
      }
      close(fd);
      break;

    case T_DIR:
      if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
        printf("ls: path too long\n");
        return;
      }
      strcpy(buf, path);
      p = buf+strlen(buf);
      *p++ = '/';
      while(read(fd, &de, sizeof(de)) == sizeof(de)){
        if(de.inum == 0)
          continue;
        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;
        if(stat(buf, &st) < 0){
          printf("ls: cannot stat %s\n", buf);
          continue;
        }
        fname = fmtname(buf);
        if (memcmp(fname, currdir, DIRSIZ) == 0 || memcmp(fname, parentdir, DIRSIZ) == 0) {
          continue;
        } 
        // printf("searching |%s|, name = |%s|, strlen = %d \n", buf, fname, strlen(fname));    
        find(buf, name);
        // printf("finished searching |%s|, name = |%s|, strlen = %d \n", buf, fname, strlen(fname));
      }
      break;
  }  
  return;
}

void constructFileName(char *input, char *fname) {
  int i = strlen(input);
  memcpy(fname, input, i);
  for (; i < DIRSIZ; ++i) {
    fname[i] = ' ';
  }
  fname[i] = '\0';
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        exit(0);
    }
    char fname[DIRSIZ + 1];
    constructFileName(argv[2], fname);
    // printf("Looking for |%s|, strlen = %d\n", fname, strlen(fname));
    find(argv[1], fname);
    exit(0);
}