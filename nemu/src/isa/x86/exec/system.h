#include <monitor/difftest.h>

uint32_t pio_read_l(ioaddr_t);
uint32_t pio_read_w(ioaddr_t);
uint32_t pio_read_b(ioaddr_t);
void pio_write_l(ioaddr_t, uint32_t);
void pio_write_w(ioaddr_t, uint32_t);
void pio_write_b(ioaddr_t, uint32_t);
void raise_intr(DecodeExecState *s, uint32_t NO, vaddr_t ret_addr);
static inline def_EHelper(lidt) {
  // TODO();
  rtl_li(s,s0,*ddest);
  printf("addr = 0x%x\n",*ddest);
  cpu.idtr.limit=vaddr_read(*s0,2);
  rtl_li(s,&cpu.idtr.base, vaddr_read(*s0+2,4));
  printf("base is 0x%x\n",cpu.idtr.base);
  print_asm_template1(lidt);
}

static inline def_EHelper(mov_r2cr) {
  TODO();

  print_asm("movl %%%s,%%cr%d", reg_name(id_src1->reg, 4), id_dest->reg);
}

static inline def_EHelper(mov_cr2r) {
  TODO();

  print_asm("movl %%cr%d,%%%s", id_src1->reg, reg_name(id_dest->reg, 4));

#ifndef __DIFF_REF_NEMU__
  difftest_skip_ref();
#endif
}

static inline def_EHelper(int) {
  //TODO();
  printf("operand is %d\n",*ddest);
  if(s->opcode == 0xcc) raise_intr(s,0x3, s->seq_pc);
  else if(s->opcode == 0xce) raise_intr(s,0x4, s->seq_pc);
  else raise_intr(s,*ddest, s->seq_pc);
  print_asm("int %s", id_dest->str);

#ifndef __DIFF_REF_NEMU__
  difftest_skip_dut(1, 2);
#endif
}

static inline def_EHelper(iret) {
  //TODO();
  rtl_pop(s,&s->jmp_pc);
  rtl_j(s,s->jmp_pc);
  rtl_pop(s,&cpu.cs);
  rtl_pop(s,&cpu.eflags.val);
  print_asm("iret");

#ifndef __DIFF_REF_NEMU__
  difftest_skip_ref();
#endif
}

//IN OUT 都有问题

static inline def_EHelper(in) {
  //TODO();
  switch(id_dest->width){
    case 4: rtl_li(s,s0,pio_read_l(*dsrc1)); break;
    case 2: rtl_li(s,s0,pio_read_w(*dsrc1)); break;
    case 1: rtl_li(s,s0,pio_read_b(*dsrc1)); break;
    default: assert(0);
  }
  operand_write(s,id_dest,s0);
  print_asm_template2(in);
}

static inline def_EHelper(out) {
  //TODO();
  switch(id_dest->width){
    case 4: pio_write_l(*ddest,*dsrc1); break;
    case 2: pio_write_w(*ddest,*dsrc1); break;
    case 1: pio_write_b(*ddest,*dsrc1); break;
    default: assert(0);
  }
  print_asm_template2(out);
}
