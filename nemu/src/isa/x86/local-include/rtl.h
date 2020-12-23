#ifndef __X86_RTL_H__
#define __X86_RTL_H__

#include <rtl/rtl.h>
#include "reg.h"

/* RTL pseudo instructions */

static inline def_rtl(lr, rtlreg_t* dest, int r, int width) {
  switch (width) {
    case 4: rtl_mv(s, dest, &reg_l(r)); return;
    case 1: rtl_host_lm(s, dest, &reg_b(r), 1); return;
    case 2: rtl_host_lm(s, dest, &reg_w(r), 2); return;
    default: assert(0);
  }
}

static inline def_rtl(sr, int r, const rtlreg_t* src1, int width) {
  switch (width) {
    case 4: rtl_mv(s, &reg_l(r), src1); return;
    case 1: rtl_host_sm(s, &reg_b(r), src1, 1); return;
    case 2: rtl_host_sm(s, &reg_w(r), src1, 2); return;
    default: assert(0);
  }
}

static inline def_rtl(push, const rtlreg_t* src1) {
  // esp <- esp - 4
  // M[esp] <- src1
  //TODO();
  rtl_subi(s,&cpu.esp,&cpu.esp,4);
  //cpu.esp-=4; 
  rtl_sm(s,&cpu.esp,0,src1,4);
}

static inline def_rtl(pop, rtlreg_t* dest) {
  // dest <- M[esp]
  // esp <- esp + 4
  //TODO();
  rtl_lm(s,dest,&cpu.esp,0,4);
  rtl_addi(s,&cpu.esp,&cpu.esp,4);
  // cpu.esp+=4; 
}

// 后面加了1的为测试时注释的代码，待会需要去掉注释
static inline def_rtl(is_sub_overflow, rtlreg_t* dest,
    const rtlreg_t* res, const rtlreg_t* src1, const rtlreg_t* src2, int width) {
  // dest <- is_overflow(src1 - src2)
  //TODO();
  //两个数异号而结果与被减数符号相反则OF为1
  rtl_msb(s,t0,src1,width); 
  rtl_msb(s,t1,src2,width); 

  //rtl_setrelop(s,RELOP_NE,dest,t0,t1);
  *dest = ((*t0&0x1) != (*t1&0x1)); 
  
  rtl_msb(s,t0,res,width); 

  //rtl_setrelop(s,RELOP_EQ,t1,t0,t1);
  *t1 = ((*t0&0x1) == (*t1&0x1)); 
  rtl_and(s,dest,dest,t1); 
}

static inline def_rtl(is_sub_carry, rtlreg_t* dest,
    const rtlreg_t* src1, const rtlreg_t* src2) {
  // dest <- is_carry(src1 - src2)
  //TODO();
  //如果被减数小于减数，则CF=1
  rtl_setrelop(s,RELOP_LTU,dest,src1,src2);
}

static inline def_rtl(is_add_overflow, rtlreg_t* dest,
    const rtlreg_t* res, const rtlreg_t* src1, const rtlreg_t* src2, int width) {
  // dest <- is_overflow(src1 + src2)
  //TODO();
  //两个数同号而结果与之相反则OF为1
  rtl_msb(s,t0,src1,width); 
  rtl_msb(s,t1,src2,width); 
  //rtl_setrelop(s,RELOP_EQ,dest,t0,t1);
  *dest = ((*t0&0x1) == (*t1&0x1)); 
  rtl_msb(s,t0,res,width); 
  //rtl_setrelop(s,RELOP_NE,t1,t0,t1);
  *t1 = ((*t0&0x1) != (*t1&0x1)); 
  rtl_and(s,dest,dest,t1); 
}

static inline def_rtl(is_add_carry, rtlreg_t* dest,
    const rtlreg_t* res, const rtlreg_t* src1) {
  // dest <- is_carry(src1 + src2)
  //TODO();
  //如果结果小于加数则CF=1
  rtl_setrelop(s,RELOP_LTU,dest,res,src1);
}

#define def_rtl_setget_eflags(f) \
  static inline def_rtl(concat(set_, f), const rtlreg_t* src) { \
    cpu.eflags.f=*src;\
  } \
  static inline def_rtl(concat(get_, f), rtlreg_t* dest) { \
    *dest=cpu.eflags.f; \
  }

def_rtl_setget_eflags(CF)
def_rtl_setget_eflags(OF)
def_rtl_setget_eflags(ZF)
def_rtl_setget_eflags(SF)

static inline def_rtl(update_ZF, const rtlreg_t* result, int width) {
  // eflags.ZF <- is_zero(result[width * 8 - 1 .. 0])
  //TODO();
  rtl_shri(s,t0,result,32-8*width);
  if(*t0 == 0) cpu.eflags.ZF = 1;
  else  cpu.eflags.ZF = 0;
}

static inline def_rtl(update_SF, const rtlreg_t* result, int width) {
  // eflags.SF <- is_sign(result[width * 8 - 1 .. 0])
  //TODO();
  rtl_shri(s,t0,result,8*width-1);
  rtl_set_SF(s,t0);
}

static inline def_rtl(update_ZFSF, const rtlreg_t* result, int width) {
  rtl_update_ZF(s, result, width);
  rtl_update_SF(s, result, width);
}

#endif
