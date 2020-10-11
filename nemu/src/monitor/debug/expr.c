#include <isa.h>

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <regex.h>

enum {
  TK_NOTYPE = 256, TK_EQ,TK_NUM,

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
  {"==", TK_EQ},        // equal
  {"\\(", '('},         // left parenthesis
  {"\\)", ')'},         // right parenthesis
  {"\\*", '*'},         // multiply
  {"/",  '/'},          // divide
  {"\\+", '+'},         // plus
  {"-", '-'},           // minus
  {"[0-9]{1,10}",TK_NUM}, // numbers
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

static Token tokens[32] __attribute__((used)) = {};
static int nr_token __attribute__((used))  = 0;

static bool make_token(char *e) {
  int position = 0;
  int i;
  regmatch_t pmatch;

  nr_token = 0;

  while (e[position] != '\0') {
    /* Try all rules one by one. */
    for (i = 0; i < NR_REGEX; i ++) {
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
        char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;

        Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
            i, rules[i].regex, position, substr_len, substr_len, substr_start);

        position += substr_len;

        /* TODO: Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
         */

        switch (rules[i].token_type) {
			case 257: {
						tokens[nr_token].type=257;
						strcpy(tokens[nr_token].str,"==");
						nr_token++;
						break;
					  }

			case 258 :{
					    tokens[nr_token].type=258;
						strncpy(tokens[nr_token].str,substr_start,substr_len);
						nr_token++;
						break;
					  }

			case 40: {
						tokens[nr_token].type=40;
						nr_token++;
						break;
					 }

			case 41: {
						tokens[nr_token].type=41;
						nr_token++;
						break;
					 }

			case 42: {
						tokens[nr_token].type=42;
						nr_token++;
						break;
					 }


			case 43: {
						tokens[nr_token].type=43;
						nr_token++;
						break;
					 }

			case 45: {
						tokens[nr_token].type=45;
						nr_token++;
						break;
					 }

			case 47: {
						tokens[nr_token].type=47;
						nr_token++;
						break;
					 }

            default: {
						break;
				     }
        }
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

bool check_parentheses(int p,int q){
	if(tokens[p].type != '(') return false;
	int num = 1; //record the number of unmatched left_parentheses
	int i;
	for(i = p + 1;i <= q;++ i){
		if(tokens[i].type == '('){
		  num++;
		}
		else if(tokens[i].type == ')'){
		  num--;
		  if(num == 0 && i != q) return false; 
		}
	}
	if(num == 0) return true;
	return false;
}

int judge_operator(int i){
	if(tokens[i].type == '+' || tokens[i].type == '-')
		return 1;   
	if(tokens[i].type == '*' || tokens[i].type == '/' )
		return 2;
	else return 0;
}

int find_main_operator(int p,int q){
	int op=p;
	int rank=3;
	int i;
	for(i = p;i <= q;++i){
	  if(tokens[i].type == '('){
		while(i <= q && tokens[i].type != ')'){
		  i++;
		}
		i--;
		continue;
	  }
	  else if(judge_operator(i)){
	    if(judge_operator(i) <= rank){
		  rank=judge_operator(i);
		  op=i;
		}
	  }
	  else continue;
	}
	return op;
}

int eval(int p,int q){
	if(p > q){
	  assert(0);
	}
	else if(p == q){
	  int num;
	  sscanf(tokens[p].str,"%d",&num);
	  return num;
	}
	else if(check_parentheses(p,q)==true){
	  return eval(p+1,q-1);
	}
	else{
	  int op = find_main_operator(p,q);//TODO to find the main 
	  int val1 = eval(p , op - 1);
	  int val2 = eval(op + 1, q);
	  switch(tokens[op].type){
		  case '+': return val1+val2;
		  case '-': return val1-val2;
		  case '*': return val1*val2;
		  case '/': return val1/val2;
		  default: assert(0);
	  }
	}
}

word_t expr(char *e, bool *success) {
  if (!make_token(e)) {
    *success = false;
    return 0;
  }

  /* TODO: Insert codes to evaluate the expression. */
 // printf("%d\n",nr_token);//delete after debuggging
  return eval(0,nr_token-1);
}
