#include <am.h>
#include <nemu.h>
#include <string.h>

#define SYNC_ADDR (VGACTL_ADDR + 4)
// #define W 400
// #define H 300
static inline int min(int x, int y) { return (x < y) ? x : y; }

void __am_gpu_init() {
  /*
  int i;
  int w = 400 ;  // TODO: get the correct width
  int h = 300 ;  // TODO: get the correct height
  uint32_t *fb = (uint32_t *)(uintptr_t)FB_ADDR;
  for (i = 0; i < w * h; i ++) fb[i] = i;
  outl(SYNC_ADDR, 1);
  */
  
}

void __am_gpu_config(AM_GPU_CONFIG_T *cfg) {
  *cfg = (AM_GPU_CONFIG_T) {
    .present = true, .has_accel = false,
    .width = 400, .height = 300,
    .vmemsz = 0
  };
}

void __am_gpu_fbdraw(AM_GPU_FBDRAW_T *ctl) {
  uint32_t tmp = inl(VGACTL_ADDR);
  int W = tmp >> 16;
  int H = tmp & 0xffff;
  int x = ctl->x, y = ctl->y, w = ctl->w, h = ctl->h;
  uint32_t *pixels = ctl->pixels;
  int cp_bytes = sizeof(uint32_t) * min(w, W - x);
  uint32_t *fb = (uint32_t *)(uintptr_t)FB_ADDR;
  for (int j = 0; (j < h) && (y + j < H); j ++) {
    memcpy(&fb[(y + j) * W + x], pixels, cp_bytes);
    pixels += w;
  }
  if (ctl->sync) {
    outl(SYNC_ADDR, 1);
  }
}

void __am_gpu_status(AM_GPU_STATUS_T *status) {
  status->ready = true;
}
