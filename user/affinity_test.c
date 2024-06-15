#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  set_affinity_mask(5);
  for(;;) {
    printf("PID: %d\n", getpid());
  }
  exit(0, 0);
}