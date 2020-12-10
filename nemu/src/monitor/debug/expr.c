#include <isa.h>

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <regex.h>
#include <stdlib.h>
#include <memory/vaddr.h>

enum {
  TK_NOTYPE = 256, 
  TK_EQ,TK_NUM,TK_HEX,TK_REG,TK_NEQ,TK_AND,
  TK_NEG,TK_POINT,

  /* TODO: Add more token types */

};

static struct rule {
  char *regex;
  int token_type;
} rules[] = {

  /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   */

  {" +", TK_NOTYPE},    // spaces
  {"\\(", '('},         // left parenthesis
  {"\\)", ')'},         // right parenthesis
  {"\\*", '*'},         // multiply
  {"/",  '/'},          // divide
  {"\\+", '+'},         // plus
  {"-", '-'},           // minus
  {"0[xX][0-9a-fA-F]+",TK_HEX}, //hex number
  {"\\$[a-zA-Z]+",TK_REG}, // register
  {"[0-9]+",TK_NUM}, // number
  {"==", TK_EQ},        // equal
  {"!=", TK_NEQ},
  {"&&", TK_AND},
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX] = {};

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
  int i;
  char error_msg[128];
  int ret;

  for (i = 0; i < NR_REGEX; i ++) {
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
    if (ret != 0) {
      regerror(ret, &re[i], error_msg, 128);
      panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
    }
  }
}

typedef struct token {
  int type;
  char str[32];
} Token;

static Token tokens[65536] __attribute__((used)) = {};
static int nr_token __attribute__((used))  = 0;

static bool make_token(char *e) {
  int position = 0;
  int i;
  regmatch_t pmatch;

  nr_token = 0;
  bool flag = false;
  while (e[position] != '\0') {
    /* Try all rules one by one. */
    for (i = 0; i < NR_REGEX; i ++) {
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
        char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;

        Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
            i, rules[i].regex, position, substr_len, substr_len, substr_start);

        position += substr_len;

        //if(i==0) break;
        /* TODO: Now a new token is recognized with rules[i]. Add codes
	         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
         */
        tokens[nr_token].type = rules[i].token_type;
        
      switch (rules[i].token_type) { 
			case TK_NOTYPE: break;
			case TK_NEQ:{
						//tokens[nr_token].type = rules[i].token_type;
						strcpy(tokens[nr_token].str,"!=");
						//nr_token++;
						break;
					  }
			case TK_AND:{
						//tokens[nr_token].type = rules[i].token_type;
						strcpy(tokens[nr_token].str,"&&");
						//nr_token++;
						break;
					  }
			case TK_EQ: {
						//tokens[nr_token].type = rules[i].token_type;
						strcpy(tokens[nr_token].str,"==");
						//nr_token++;
						break;
					  }

			case TK_NUM: case TK_HEX: case TK_REG:{
						//memset(tokens[nr_token].str,0,strlen(tokens[nr_token].str));
						//tokens[nr_token].type = rules[i].token_type;
						if(substr_len<32)
              strncpy(tokens[nr_token].str,substr_start,substr_len);
            else assert(0);
            if(flag){
              tokens[nr_token].type = TK_NEG; 
              flag=false;
            }
						/*
						int j;
						for(j=0;j<substr_len;++j){
							tokens[nr_token].str[j] = e[position-substr_len+j];
						}
						tokens[nr_token].str[j]='\0';
						*/
						//nr_token++;
						break;
					  }
			case '-': {
						if(nr_token==0 || ( tokens[nr_token-1].type != TK_REG && tokens[nr_token-1].type != TK_HEX && tokens[nr_token-1].type !=TK_NUM && tokens[nr_token-1].type != ')')){
              flag = true;
              nr_token--;
							break;
						}
			}
			case '*': {
						if(nr_token==0 || ( tokens[nr_token-1].type != TK_REG && tokens[nr_token-1].type != TK_HEX && tokens[nr_token-1].type !=TK_NUM && tokens[nr_token-1].type != ')')){
							tokens[nr_token].type = TK_POINT;
							nr_token++;
							break;
						}
			}
      /*
      default: {
						tokens[nr_token].type = rules[i].token_type;
						nr_token++;
						break;
				     }
       */
        }
        nr_token++;
        break;
	  }      
    }

    if (i == NR_REGEX) {
      printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
      return false;
    }
  }

  return true;
}

// from here I will implement eval,check_parentheses and so on


bool inner_parentheses(int p,int q){
	int num = 1; //record the number of unmatched left_parentheses
	while(p<=q && num){
		if(tokens[p].type == '('){
		  num++;
		}
		else if(tokens[p].type == ')'){
		  num--;
		}
    p++;
  }
	if(num == 1) return true;
	return false;
}

bool check_parentheses(int p, int q){  
  if(tokens[p].type == '(' && tokens[q].type == ')'){
    return inner_parentheses(p + 1, q - 1);
  }
  return false;
}


int judge_operator(int i){
	if(tokens[i].type == TK_AND)
		return 1;
	if(tokens[i].type == TK_EQ|| tokens[i].type == TK_NEQ)
		return 2;
	if(tokens[i].type == '+' || tokens[i].type == '-')
		return 3;   
	if(tokens[i].type == '*' || tokens[i].type == '/' )
		return 4;
	//if(tokens[i].type == TK_NEG) return 6;
	else return 1000;
}


int find_main_operator(int p,int q){
	int op=-1;
	int rank=100;
	int i;
	for(i = p;i <= q;++i){
	  if(tokens[i].type == '('){
		int num=1;
		i++;
		while(num!=0){
		  if(tokens[i].type == '(')
			  num++;
		  else if(tokens[i].type == ')')
			  num--;
		  i++;
		}
		i--;
		if(i > q) assert(0);
	  }
	  else if(judge_operator(i) <= rank){
		  rank=judge_operator(i);
		  op=i;
	  }
	  else continue;
	}
	return op;
}

uint32_t eval(int p,int q){
	if(p > q){
		printf("%d %d\n",p,q);
		assert(0);
	}
	else if(p == q){
		int num;
		switch(tokens[p].type){
			case TK_NUM:{
	  			sscanf(tokens[p].str,"%d",&num);
	  			return num;
			}
      case TK_NEG:{
          sscanf(tokens[p].str,"%d",&num);
	  			return 0-num;
      }
			case TK_HEX:{
				num = strtol(tokens[p].str,NULL,16);
				return num;
			}
			case TK_REG:{
				bool succ = true;
				num = isa_reg_str2val(tokens[p].str,&succ);
				if(succ == false){
					printf("illegal register!\n");
					return 0;
				}
				return num;
			}

		} 
	}
	else if(check_parentheses(p,q)==true){
	  	return eval(p+1,q-1);
	}
	else{
	  	//if(tokens[p].type==TK_NEG) return 0-eval(p+1,q);
		if(tokens[p].type==TK_POINT)   return vaddr_read(eval(p+1,q),4); // this is not true if the following expression is not TK_NUM or '('+ expressoin+')'
	  int op = find_main_operator(p,q);//TODO to find the main
    if(op == -1) assert(0);
    //if(op==-1&&tokens[p].type==TK_NEG) return 0-eval(p+1,q);
	  uint32_t val1 = eval(p , op - 1);
	  uint32_t val2 = eval(op + 1, q);

	  	switch(tokens[op].type){
		  	case '+': return val1+val2;
		  	case '-': return val1-val2;
		 	case '*': return val1*val2;
		  	case '/': return val1/val2;
			case TK_EQ:return val1==val2;
			case TK_NEQ:return val1!=val2;
			case TK_AND:return val1&&val2;
		  	default: assert(0);
	  	}
	}
	printf("Invalid!\n");
	return 0;
}

word_t expr(char *e, bool *success) {
  if (!make_token(e)) {
    *success = false;
    return 0;
  }

  /* TODO: Insert codes to evaluate the expression. */
 // printf("%d\n",nr_token);//delete after debuggging
  *success = inner_parentheses(0,nr_token-1);
  if(*success)
    return eval(0,nr_token-1);
  else
    return 0;
}
