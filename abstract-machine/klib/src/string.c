#include <klib.h>
#include <stdint.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char *s) { 
  size_t len = 0;
  while(*(s+len)!='\0'){
    ++len;
  }
  return len;
}

char *strcpy(char* dst,const char* src) {  
  char *tmp = dst;
  while(*src!='\0'){
    *tmp++ = *src++;
  }
  return dst;
}

char* strncpy(char* dst, const char* src, size_t n) {
  char *tmp = dst;
  while(n>0&&*src!='\0'){
    *tmp++= *src++;
    n--;
  }
  while(n--){
    *tmp++='\0';
  }
  return dst;
}

char* strcat(char* dst, const char* src) { 
  char *tmp= dst;
  while(*tmp!='\0') tmp++;
  while(*src!='\0'){
    *tmp++=*src++;
  }
  *tmp='\0';
  return dst;
}

int strcmp(const char* s1, const char* s2) {
  while(*s1!='\0'&&*s2!='\0'&&*s1==*s2){
    s1++;
    s2++;
  }
  if(*s1 == *s2) return 0;
  else if(*s1 > *s2) return 1;
  else return -1;
}

int strncmp(const char* s1, const char* s2, size_t n) {
  return 0;
}

void* memset(void* v,int c,size_t n) { //TODO
  return NULL;
}

void* memmove(void* dst,const void* src,size_t n) {
  return NULL;
}

void* memcpy(void* out, const void* in, size_t n) { //TODO
  return NULL;
}

int memcmp(const void* s1, const void* s2, size_t n) { //TODO
  return 0;
}

#endif
