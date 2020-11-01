#include <isa.h>
#include <stdlib.h>
#include <time.h>
#include "local-include/reg.h"

const char *regsl[] = {"eax", "ecx", "edx", "ebx", "esp", "ebp", "esi", "edi"};
const char *regsw[] = {"ax", "cx", "dx", "bx", "sp", "bp", "si", "di"};
const char *regsb[] = {"al", "cl", "dl", "bl", "ah", "ch", "dh", "bh"};

void reg_test() {
  srand(time(0));
  word_t sample[8];
  word_t pc_sample = rand();
  cpu.pc = pc_sample;

  int i;
  for (i = R_EAX; i <= R_EDI; i ++) {
    sample[i] = rand();
    reg_l(i) = sample[i];
    assert(reg_w(i) == (sample[i] & 0xffff));
  }

  assert(reg_b(R_AL) == (sample[R_EAX] & 0xff));
  assert(reg_b(R_AH) == ((sample[R_EAX] >> 8) & 0xff));
  assert(reg_b(R_BL) == (sample[R_EBX] & 0xff));
  assert(reg_b(R_BH) == ((sample[R_EBX] >> 8) & 0xff));
  assert(reg_b(R_CL) == (sample[R_ECX] & 0xff));
  assert(reg_b(R_CH) == ((sample[R_ECX] >> 8) & 0xff));
  assert(reg_b(R_DL) == (sample[R_EDX] & 0xff));
  assert(reg_b(R_DH) == ((sample[R_EDX] >> 8) & 0xff));

  assert(sample[R_EAX] == cpu.eax);
  assert(sample[R_ECX] == cpu.ecx);
  assert(sample[R_EDX] == cpu.edx);
  assert(sample[R_EBX] == cpu.ebx);
  assert(sample[R_ESP] == cpu.esp);
  assert(sample[R_EBP] == cpu.ebp);
  assert(sample[R_ESI] == cpu.esi);
  assert(sample[R_EDI] == cpu.edi);

  assert(pc_sample == cpu.pc);
}

void isa_reg_display() {
	printf("eax: 0x%08x\n",cpu.eax);
	printf("ecx: 0x%08x\n",cpu.ecx);
	printf("edx: 0x%08x\n",cpu.edx);
	printf("ebx: 0x%08x\n",cpu.ebx);
	printf("esp: 0x%08x\n",cpu.esp);
	printf("ebp: 0x%08x\n",cpu.ebp);
	printf("esi: 0x%08x\n",cpu.esi);
	printf("edi: 0x%08x\n",cpu.edi);
	return;
}

word_t isa_reg_str2val(const char *s, bool *success) {
  if(strcmp(s,"$eax")==0)     return cpu.eax; 
	else if(strcasecmp(s,"$ecx")==0)return cpu.ecx; 		
	else if(strcasecmp(s,"$edx")==0)return cpu.edx; 
	else if(strcasecmp(s,"$ebx")==0)return cpu.ebx; 
	else if(strcasecmp(s,"$esp")==0)return cpu.esp; 
	else if(strcasecmp(s,"$ebp")==0)return cpu.ebp; 
	else if(strcasecmp(s,"$esi")==0)return cpu.esi; 
	else if(strcasecmp(s,"$edi")==0)return cpu.edi;
  else *success = false;
  return 0;
}
