static inline def_EHelper(mov) {
  operand_write(s, id_dest, dsrc1);
  print_asm_template2(mov);
}

static inline def_EHelper(push) {
  //TODO();
  rtl_push(s,ddest);
  print_asm_template1(push);
}

static inline def_EHelper(pop) {
  //TODO();
  rtl_pop(s, ddest);
  // operand_write(s,id_dest,s0);
  print_asm_template1(pop);
}

static inline def_EHelper(pusha) {
  // TODO();
  rtl_mv(s,s0,&cpu.esp);
  rtl_push(s,&cpu.eax);
	rtl_push(s,&cpu.ecx);
	rtl_push(s,&cpu.edx);
	rtl_push(s,&cpu.ebx);
	rtl_push(s,s0);
	rtl_push(s,&cpu.ebp);
	rtl_push(s,&cpu.esi);
	rtl_push(s,&cpu.edi);
  print_asm("pusha");
}

static inline def_EHelper(popa) {
  // TODO();
  rtl_pop(s,&cpu.edi);
  rtl_pop(s,&cpu.esi);
  rtl_pop(s,&cpu.ebp);
  rtl_pop(s,s0);
  rtl_pop(s,&cpu.ebx);
  rtl_pop(s,&cpu.edx);
  rtl_pop(s,&cpu.ecx);
  rtl_pop(s,&cpu.eax);
  print_asm("popa");
}

// High Level Procedure Exit
static inline def_EHelper(leave) {
  //TODO();
  rtl_mv(s,&cpu.esp,&cpu.ebp);
  rtl_pop(s,&cpu.ebp);
  print_asm("leave");
}

static inline def_EHelper(cltd) { //实现的很蛇皮，以后看能不能用rtl优化
  if (s->isa.is_operand_size_16) {
    //符号扩展
    if(0x1&(cpu.eax>>15)) cpu.edx |= 0x0000ffff;
    else cpu.edx &= 0xffff0000;
  }
  else {
  //TODO();
  if(0x1&(cpu.eax>>31)) cpu.edx |= 0xffffffff;
    else cpu.edx &= 0x0;
  }
  print_asm(s->isa.is_operand_size_16 ? "cwtl" : "cltd");
}

static inline def_EHelper(cwtl) {
  if (s->isa.is_operand_size_16) {
  //TODO();
  rtl_sext(s,s0,&reg_l(R_EAX),1);
  rtl_mv(s,&reg_l(R_EAX),s0);
  }
  else {
  //TODO();
  rtl_sext(s,s0,&reg_l(R_EAX),2);
  rtl_mv(s,&reg_l(R_EAX),s0);
  }

  print_asm(s->isa.is_operand_size_16 ? "cbtw" : "cwtl");
}

static inline def_EHelper(movsx) {
  id_dest->width = s->isa.is_operand_size_16 ? 2 : 4;
  rtl_sext(s, ddest, dsrc1, id_src1->width);
  operand_write(s, id_dest, ddest);
  print_asm_template2(movsx);
}

static inline def_EHelper(movsb) {
  rtl_li(s,s1,s->dest.width);
  rtl_lm(s,s0,&cpu.esi,0,*s1);
  rtl_sm(s,&cpu.edi,0,s0,*s1);
  cpu.esi += *s1;
  cpu.edi += *s1;
  print_asm("movsb");
}

static inline def_EHelper(movzx) {
  id_dest->width = s->isa.is_operand_size_16 ? 2 : 4;
  operand_write(s, id_dest, dsrc1);
  print_asm_template2(movzx);
}

static inline def_EHelper(xchg) {
  *s0 = *ddest;
  operand_write(s, id_dest, dsrc1);
  operand_write(s, id_src1, s0);
}

static inline def_EHelper(lea) {
  rtl_addi(s, ddest, s->isa.mbase, s->isa.moff);
  operand_write(s, id_dest, ddest);
  print_asm_template2(lea);
}

