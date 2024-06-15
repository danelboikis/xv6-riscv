#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  write(1, "hello world\n", 12);
  exit(0, 0);
}