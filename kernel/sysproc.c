#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_exit(void)
{
  int n;
  char exit_msg[32];
  char *default_exit_msg = "No exit message";
  uint64 exit_msg_addr;
  
  argint(0, &n);

  argaddr(1, &exit_msg_addr);
  if (exit_msg_addr == 0) {
    safestrcpy(myproc()->exit_msg, default_exit_msg, 16);
  }
  else if (argstr(1, exit_msg, sizeof(exit_msg)) < 0 || exit_msg[0] == 0) {
    safestrcpy(myproc()->exit_msg, default_exit_msg, 16);
  }
  else {
    // put the exit message in the pcb
    safestrcpy(myproc()->exit_msg, exit_msg, 32);
  }

  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  uint64 exit_msg;
  argaddr(0, &p);
  argaddr(1, &exit_msg);
  return wait(p, exit_msg);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

uint64
sys_memsize(void)
{
  return myproc()->sz;
}

uint64
sys_set_affinity_mask(void)
{
  int mask;
  argint(0, &mask);
  acquire(&myproc()->lock);
  myproc()->affinity_mask = mask;
  myproc()->effective_affinity_mask = mask;
  release(&myproc()->lock);
  return mask;
}
