#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>

// this should be enough
static char buf[65536] = {};
static char code_buf[65536 + 128] = {}; // a little larger than `buf`
static char *code_format =
"#include <stdio.h>\n"
"int main() { "
"  unsigned result = %s; "
"  printf(\"%%u\", result); "
"  return 0; "
"}";
static int buf_l = 0; //buf_length

static inline uint32_t choose(uint32_t t){
  return rand() % t;
}

static inline void gen(char c){
  if(buf_l > 655) return;
  buf[buf_l++] = c;
  return;
}

static inline void gen_num(){
  if(buf_l > 655) return;
  else{
    int i = rand() % 2000;
    if(i == 0) i++;
    int tmp = 1,cnt=0;
    while(i>=tmp){
      tmp*=10;
      cnt++;
    }
    sprintf(buf+buf_l,"%d",i);
    buf_l+=cnt;
  }  
}


static inline void gen_rand_op(){
  switch(choose(4)){
    case 0: gen('+'); break;
    case 1: gen('-'); break;
    case 2: gen('*'); break;
    case 3: gen('/'); break;
    default: assert(0); break;
  }
}

static inline void gen_rand_expr() {
  switch(choose(3)){
    case 0: gen_num(); break;
    case 1: gen('('); gen_rand_expr(); gen(')'); break;
    default: gen_rand_expr(); gen_rand_op(); gen_rand_expr(); break;
  }  
}

int main(int argc, char *argv[]) {
  int seed = time(0);
  srand(seed);
  int loop = 1;
  if (argc > 1) {
    sscanf(argv[1], "%d", &loop);
  }
  int i;
  for (i = 0; i < loop; i ++) {
    buf_l = 0;
    memset(buf,'\0',sizeof(buf));
    gen_rand_expr();
    if(buf_l > 655){
      i--;
      continue;
    }

    sprintf(code_buf, code_format, buf);

    FILE *fp = fopen("/tmp/.code.c", "w");
    assert(fp != NULL);
    fputs(code_buf, fp);
    fclose(fp);

    int ret = system("gcc /tmp/.code.c -o /tmp/.expr");
    if (ret != 0) continue;

    fp = popen("/tmp/.expr", "r");
    assert(fp != NULL);

    int result;
    if(fscanf(fp, "%d", &result)==EOF) 
    printf("-1\n");
    pclose(fp);

    printf("%u %s\n", result, buf);
  }
  return 0;
}
