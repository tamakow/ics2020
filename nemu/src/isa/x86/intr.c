#include <cpu/exec.h>
#include "local-include/rtl.h"

void raise_intr(DecodeExecState *s, uint32_t NO, vaddr_t ret_addr) {
  /* TODO: Trigger an interrupt/exception with ``NO''.
   * That is, use ``NO'' to index the IDT.
   */
  //TODO();
  rtl_push(s,(rtlreg_t*)&cpu.eflags.val);
  rtl_push(s,(rtlreg_t*)&cpu.cs);
  rtl_push(s,(rtlreg_t*)&ret_addr);

  cpu.eflags.IF = 0;

  uint32_t addr = cpu.idtr.base + NO*8;

  uint32_t off_h = vaddr_read(addr+4,4);
  uint32_t off_l = vaddr_read(addr,4);
  uint32_t p = vaddr_read(addr+5,1) >> 7;
  if(!p) assert(0);
  
  uint32_t offset = (off_h&0xffff0000) | (off_l&0xffff);
  rtl_j(s,offset);
}

void query_intr(DecodeExecState *s) {
  TODO();
}
