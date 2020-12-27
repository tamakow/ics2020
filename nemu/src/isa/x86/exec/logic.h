#include "cc.h"


static inline def_EHelper(test) {
  //  TODO();
  rtl_and(s,s0,ddest,dsrc1);
  rtl_update_ZFSF(s,s0,id_dest->width);
  rtl_li(s,s1,0);
  rtl_set_OF(s,s1);
  rtl_set_CF(s,s1);
  print_asm_template2(test);
}

static inline def_EHelper(and) {
  //TODO();
  rtl_and(s,s0,ddest,dsrc1);
  operand_write(s,id_dest,s0);

  //CF = 0, OF = 0
  rtl_li(s,s1,0);
  rtl_set_CF(s,s1);
  rtl_set_OF(s,s1);
  rtl_update_ZFSF(s,s0,id_dest->width);

  print_asm_template2(and);
}

static inline def_EHelper(xor) {
  //TODO();
  rtl_xor(s,s0,ddest,dsrc1);
  operand_write(s,id_dest,s0);
  rtl_li(s,s1,0);
  rtl_set_OF(s,s1);
  rtl_set_CF(s,s1);
  rtl_update_ZFSF(s,s0,id_dest->width);
  print_asm_template2(xor);
}

static inline def_EHelper(or) {
  //TODO();
  rtl_or(s,s0,ddest,dsrc1);
  operand_write(s,id_dest,s0);
  rtl_li(s,s1,0);
  rtl_set_OF(s,s1);
  rtl_set_CF(s,s1);
  rtl_update_ZFSF(s,s0,id_dest->width);
  print_asm_template2(or);
}

static inline def_EHelper(sar) {
  //TODO();
  // unnecessary to update CF and OF in NEMU
  rtl_sext(s,ddest,ddest,id_dest->width);
  rtl_sar(s,s0,ddest,dsrc1);
  operand_write(s,id_dest,s0);
  rtl_update_ZFSF(s,s0,id_dest->width);
  print_asm_template2(sar);
}

static inline def_EHelper(shl) {
  //TODO();
  // unnecessary to update CF and OF in NEMU
  rtl_shl(s,s0,ddest,dsrc1);
  operand_write(s,id_dest,s0);
  rtl_update_ZFSF(s,s0,id_dest->width);
  print_asm_template2(shl);
}

static inline def_EHelper(shr) {
  //TODO();
  // unnecessary to update CF and OF in NEMU
  rtl_shr(s,s0,ddest,dsrc1);
  operand_write(s,id_dest,s0);
  rtl_update_ZFSF(s,s0,id_dest->width);
  print_asm_template2(shr);
}

static inline def_EHelper(rol) {
  for(int i=0;i<*dsrc1;++i){
    rtl_shri(s,s0,ddest,id_dest->width*8-1);
    rtl_shli(s,s1,ddest,1);
    rtl_xor(s,s2,s0,s1);
  }
  operand_write(s,id_dest,s2);
  rtl_set_CF(s,s0);
  print_asm_template2(rol);
}


static inline def_EHelper(setcc) {
  uint32_t cc = s->opcode & 0xf;
  rtl_setcc(s, ddest, cc);
  operand_write(s, id_dest, ddest);

  print_asm("set%s %s", get_cc_name(cc), id_dest->str);
}

static inline def_EHelper(not) {
  //TODO();
  rtl_not(s,s0,ddest);
  operand_write(s,id_dest,s0);
  print_asm_template1(not);
}

