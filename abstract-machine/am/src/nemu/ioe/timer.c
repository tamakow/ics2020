#include <am.h>
#include <nemu.h>

static uint32_t start_us,start_s;

void __am_timer_init() {
  start_us=inl(RTC_ADDR);
  start_s=inl(RTC_ADDR+4);
}

void __am_timer_uptime(AM_TIMER_UPTIME_T *uptime) {
  long second = inl(RTC_ADDR+4) - start_s;
  long usecond = inl(RTC_ADDR) - start_us;
  uptime->us = second*1000000 + (usecond+500);
}

void __am_timer_rtc(AM_TIMER_RTC_T *rtc) {
  rtc->second = 0;
  rtc->minute = 0;
  rtc->hour   = 0;
  rtc->day    = 0;
  rtc->month  = 0;
  rtc->year   = 1900;
}
