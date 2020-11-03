#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include <common.h>

typedef struct watchpoint {
  int NO;
  struct watchpoint *next;
  /* TODO: Add more members if necessary */
  int oldvalue;
  int newvalue;
  char str[32];// expression
} WP;

void init_wp_pool();
WP* new_wp(char*);
void free_wp(int);
bool check_wp();
void print_wp();
void update_wp();

#endif
