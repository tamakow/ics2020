#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include "monitor/debug/expr.h"

void init_monitor(int, char *[]);
void engine_start();
int is_exit_status_bad();

int main(int argc, char *argv[]) {
  /* Initialize the monitor. */
  init_monitor(argc, argv);
  char e[1000];
  FILE* fp = fopen("../tools/gen-expr/input","r");
  assert(fp!=NULL);
  int ans=0;
  while(fscanf(fp,"%d",&ans)==1){
    if(ans == -1) continue;
    if(fscanf(fp,"%s",e)==EOF) continue;
    bool success =true;
    int my_ans = expr(e,&success);
    if(success == false){
      printf("not a legal expression!\n");
      continue;
    }
    if(my_ans != ans){
      printf("Wrong answer:my answer is %d, but the true answer is %d\n",my_ans,ans);
      printf("the test is %s\n",e);
    }
  }
  /* Start engine. */
  engine_start();

  return is_exit_status_bad();
}
