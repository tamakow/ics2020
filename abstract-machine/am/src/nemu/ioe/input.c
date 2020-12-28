#include <am.h>
#include <nemu.h>

#define KEYDOWN_MASK 0x8000

void __am_input_keybrd(AM_INPUT_KEYBRD_T *kbd) {
  int temp = inl(KBD_ADDR);
  if(temp != KEYDOWN_MASK){
    kbd->keycode = temp;
    kbd->keydown = true;
  }
  else{
    kbd->keycode = AM_KEY_NONE;
    kbd->keydown = false;
  }
}
