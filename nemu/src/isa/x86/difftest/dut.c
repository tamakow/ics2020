#include <isa.h>
#include <monitor/difftest.h>
#include "../local-include/reg.h"
#include "difftest.h"

bool isa_difftest_checkregs(CPU_state *ref_r, vaddr_t pc) {
  if( ref_r->eax != cpu.eax ||
      ref_r->ecx != cpu.ecx ||
      ref_r->edx != cpu.edx ||
      ref_r->ebx != cpu.ebx ||
      ref_r->esp != cpu.esp ||
      ref_r->ebp != cpu.ebp ||
      ref_r->esi != cpu.esi ||
      ref_r->edi != cpu.edi ||
      ref_r->pc  != cpu.pc){
      printf("WRONG!\n");
      printf("ref_r->eax: 0x%x\n",ref_r->eax);
      printf("ref_r->ecx: 0x%x\n",ref_r->ecx);
      printf("ref_r->edx: 0x%x\n",ref_r->edx);
      printf("ref_r->ebx: 0x%x\n",ref_r->ebx);
      printf("ref_r->esp: 0x%x\n",ref_r->esp);
      printf("ref_r->ebp: 0x%x\n",ref_r->ebp);
      printf("ref_r->esi: 0x%x\n",ref_r->esi);
      printf("ref_r->edi: 0x%x\n",ref_r->edi);
      printf("ref_r->pc: 0x%x\n",ref_r->pc);
       return false; 
      }
  return true;

}

void isa_difftest_attach() {
}
