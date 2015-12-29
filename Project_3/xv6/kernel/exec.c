#include "types.h"
#include "param.h"
#include "mmu.h"
#include "proc.h"
#include "defs.h"
#include "x86.h"
#include "elf.h"

int
exec(char *path, char **argv)
{
  char *s, *last;
  int i, off;
  uint argc, sz, sp, ustack[3+MAXARG+1], new_user_stk;
  struct elfhdr elf;
  struct inode *ip;
  struct proghdr ph;
  pde_t *pgdir, *oldpgdir;

  if((ip = namei(path)) == 0)
    return -1;
  ilock(ip);
  pgdir = 0;

  // Check ELF header
  if(readi(ip, (char*)&elf, 0, sizeof(elf)) < sizeof(elf))
    goto bad;
  if(elf.magic != ELF_MAGIC)
    goto bad;

  if((pgdir = setupkvm()) == 0)
    goto bad;

  // Load program into memory starting from page 1 instead of 0 for null pointer def error handling.
  sz = PGSIZE;
  //sz = 0;
  for(i=0, off=elf.phoff; i<elf.phnum; i++, off+=sizeof(ph)){
    if(readi(ip, (char*)&ph, off, sizeof(ph)) != sizeof(ph))
      goto bad;
    if(ph.type != ELF_PROG_LOAD)
      continue;
    if(ph.memsz < ph.filesz)
      goto bad;
    //cprintf("loading program from %d to %d file sz = %d\n", sz, ph.va + ph.memsz, ph.memsz);
    if((sz = allocuvm(pgdir, sz, ph.va + ph.memsz)) == 0)
      goto bad;
    if(loaduvm(pgdir, (char*)ph.va, ip, ph.offset, ph.filesz) < 0)
      goto bad;
  }
  iunlockput(ip);
  ip = 0;	

  // cprintf("sz before roundup: %d\n", sz);
  sz = PGROUNDUP(sz);
  //size of process code segment
  proc->proc_code_sz = sz; 
  //cprintf("process code size: %d\n", proc->proc_code_sz);

  //Allocate a one-page stack at the end of user address space
  if((new_user_stk = allocuvm(pgdir, USERTOP-PGSIZE, USERTOP)) == 0)
    goto bad;

  //cprintf("stack is allocated from %d to %d\n", USERTOP-PGSIZE, new_user_stk);
  //cprintf("size of process after stack allocation: %d\n", sz);

  // Push argument strings, prepare rest of stack in ustack.
  sp = new_user_stk;
  //cprintf("initialising stack pointer: %d\n", sp);


  for(argc = 0; argv[argc]; argc++) {
    if(argc >= MAXARG)
      goto bad;
    sp -= strlen(argv[argc]) + 1;
    sp &= ~3;
    if(copyout(pgdir, sp, argv[argc], strlen(argv[argc]) + 1) < 0)
      goto bad;
    ustack[3+argc] = sp;

    //cprintf("stack pointer %d: \n",sp);
  }



  ustack[3+argc] = 0;

  ustack[0] = 0xffffffff;  // fake return PC
  ustack[1] = argc;
  ustack[2] = sp - (argc+1)*4;  // argv pointer

  sp -= (3+argc+1) * 4;
  if(copyout(pgdir, sp, ustack, (3+argc+1)*4) < 0)
    goto bad;

  // cprintf("ustack: %d, %d, %d \n", ustack[0], ustack[1], ustack[2] );
  // cprintf("stack pointer %d: \n",sp);

  // Save program name for debugging.
  for(last=s=path; *s; s++)
    if(*s == '/')
      last = s+1;
  safestrcpy(proc->name, last, sizeof(proc->name));


  //allocating a page at usertop 636-640kb for the process
  // uint new_user_stk = allocuvm(pgdir, USERTOP-PGSIZE, USERTOP);
  // if(new_user_stk == 0)
  //   panic("error: allocating extra page at the USERTOP (636-640KB)");


  //allocate one invalid page between stack and heap
  proc->proc_invalid_page = new_user_stk - (2 * PGSIZE);
  


  // Commit to the user image.
  oldpgdir = proc->pgdir;
  proc->pgdir = pgdir;
  proc->sz = sz + PGSIZE; // one extra invalid page between code segment and headp 
  proc->tf->eip = elf.entry;  // main
  proc->tf->esp = sp;
  proc->proc_stack_sz = 1; // 1 page is currently allocated
  switchuvm(proc);
  freevm(oldpgdir);

  //cprintf("stack pointer: %d\n",sp);
  // cprintf("Total va available : %d no of pages: %d \n",USERTOP,USERTOP/PGSIZE);
  // cprintf("starting null page size: %d no of pages: %d \n",PGSIZE, PGSIZE/PGSIZE);
  // cprintf("process size: %d no of pages: %d \n",proc->sz, proc->sz/PGSIZE);
  // cprintf("stack size: %d no of pages: %d \n",PGSIZE, proc->proc_stack_sz);
  // cprintf("invalid page: %d no of pages: %d \n",PGSIZE, PGSIZE/PGSIZE);


  // // char  *dummy_addr = (char *)654944;

  // // *dummy_addr  = 'a'; 
  // // cprintf("at dummy_addr = %d: %c\n",dummy_addr, *dummy_addr);

  // cprintf("mem left for heap: %d no of pages: %d \n", USERTOP - proc->sz - PGSIZE - PGSIZE - PGSIZE, (USERTOP - proc->sz - PGSIZE - PGSIZE - PGSIZE )/PGSIZE );
  // cprintf("mem left for stack: %d no of pages: %d \n",  USERTOP - proc->sz - PGSIZE - PGSIZE, (USERTOP - proc->sz - PGSIZE - PGSIZE)/PGSIZE ); 


  return 0;

 bad:
  if(pgdir)
    freevm(pgdir);
  if(ip)
    iunlockput(ip);
  return -1;
}
