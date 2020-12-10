//这是为了寻找bug是否在check_parenthese 并不会对正确代码进行抄袭

#include <isa.h>
#include <memory/paddr.h>
#include <stdlib.h>

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <regex.h>

enum {
  TK_NOTYPE = 256, TK_LEFT, TK_RIGHT,
  TK_ADDR, TK_NEGNUM, TK_REG, TK_NUM,  //数据类型
  //下面都是运算符,按优先级
  TK_DEREF, TK_MULTI, TK_DIV, TK_PLUS, TK_DIFF, TK_GREQ, TK_LEQ,
  TK_GREAT, TK_LE, TK_EQ, TK_NEQ, TK_AND, TK_OR
    
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
  {"\\(", TK_LEFT},
  {"\\)", TK_RIGHT},
  {"\\*", TK_MULTI},
  {"/", TK_DIV},
  {"\\+", TK_PLUS},         // plus
  {"-", TK_DIFF},
  {"==", TK_EQ},        // equal
  {"0x[0-9a-f]+", TK_ADDR},
  {"[0-9]+U*", TK_NUM},
  {"\\$[a-z]+", TK_REG},
  {"!=", TK_NEQ},
  {"&&", TK_AND},
  {"\\|\\|", TK_OR},
  {"<", TK_LE},
  {"<=", TK_LEQ},
  {">", TK_GREAT},
  {">=", TK_GREQ}
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

static Token tokens[128] __attribute__((used)) = {};
static int nr_token __attribute__((used))  = 0;

bool judgeFront(int j){ //判断前一个符号是否为运算符,不包括解引用
  return tokens[j - 1].type >= TK_MULTI && tokens[j - 1].type <=  TK_OR;
}

static bool make_token(char *e) {
  int position = 0;
  int i;
  regmatch_t pmatch;

  nr_token = 0;
  bool negative = false;
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
        if(i == 0)  //匹配到空格
          break;

        tokens[nr_token].type = rules[i].token_type;
        
        switch (rules[i].token_type) {
          case TK_NUM:
            if(substr_len >= 32 )
              return false;
            if(negative){
              tokens[nr_token].type = TK_NEGNUM;
              negative = false;
            }
            strncpy(tokens[nr_token].str, e + position -substr_len, substr_len);
            break;
          case TK_DIFF:  //判断是否为负号意义
            if(nr_token == 0 || judgeFront(nr_token) || tokens[nr_token - 1].type == TK_LEFT){
              negative = true;
              nr_token--;
            }
            break;
          case TK_ADDR:
            if(substr_len >= 32 )
              return false;
            strncpy(tokens[nr_token].str, e + position -substr_len, substr_len);
            break;
          case TK_REG:
            strncpy(tokens[nr_token].str, e + position -substr_len, substr_len);
            break;
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

bool check_parentheses(int p,int q){
	if(tokens[p].type != '(' || tokens[q].type != ')') return false;
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

bool inner_parentheses(int p,int q){
	int num = 0;
	int i;
	for(i = p;i <= q;++ i){
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


uint32_t eval(int p, int q, bool *success){
  if(!(*success))  //错误表达式
    return 0;
  
  if (p > q) {
    *success = false;
    return 0;
  }
  else if (p == q) {
    if(tokens[p].type < TK_ADDR || tokens[p].type > TK_NUM){
      *success = false;
      return 0;
    }
    uint32_t ans = 0;
    if(tokens[p].type == TK_ADDR){
      sscanf(tokens[p].str + 2, "%x", &ans);
      return ans;
    }

    if(tokens[p].type == TK_REG){
      ans = isa_reg_str2val(tokens[p].str, success);
      if(*success)
        return ans;
      else{
        return 0;
      }
    }
    sscanf(tokens[p].str, "%u", &ans);
    return tokens[p].type == TK_NEGNUM? (0 - ans) : ans;
    /* Single token.
     * For now this token should be a number.
     * Return the value of the number.
     */
  }
  else if (check_parentheses(p, q) == true) {
    /* The expression is surrounded by a matched pair of parentheses.
     * If that is the case, just throw away the parentheses.
     */
    return eval(p + 1, q - 1, success);
  }
  else {
    int op = -1; //the position of 主运算符 in the token expression;
    uint32_t *ans;
    for(int i = p; i <= q; i++){
      switch(tokens[i].type){
        case TK_LEFT: 
        {
          int count = 1;
          while(count){
            if(tokens[++i].type == TK_LEFT)
              count++;
            else if(tokens[i].type == TK_RIGHT)
              count--;
          }
          break;
        }
        case TK_PLUS: case TK_DIFF: 
          if(op == -1 || tokens[op].type <= TK_DIFF) { op = i ; } break;
        case TK_MULTI: case TK_DIV:  
          if(op == -1 || tokens[op].type <= TK_DIV)  { op = i ; } break;
        case TK_DEREF: 
          if(op == -1) { op = i ; } break;
        case TK_LE: case TK_GREAT :
          if(op == -1 || tokens[op].type < TK_LE)  { op = i ; } break;
        case TK_GREQ: case TK_LEQ:
          if(op == -1 || tokens[op].type < TK_LEQ) { op = i ; } break;
        case TK_EQ: case TK_NEQ:
          if(op == -1 || tokens[op].type < TK_NEQ) { op = i ; } break;
        case TK_AND: 
          if(op == -1 || tokens[op].type < TK_AND) { op = i ; } break;
        case TK_OR: 
          if(op == -1 || tokens[op].type < TK_OR) { op = i ; } break;
        default:  continue;
      }
    }

    if(tokens[op].type == TK_DEREF){
      ans = guest_to_host(eval(p+1, q, success)); 
      return *ans;
    }

    uint32_t  val1 = eval(p, op - 1, success);
    uint32_t  val2 = eval(op + 1, q, success);


    switch (tokens[op].type) {
      case TK_PLUS: return val1 + val2;
      case TK_DIFF: return val1 - val2;
      case TK_MULTI: return val1 * val2;
      case TK_DIV: 
          if(val2 !=0)
            return val1 / val2;
          else{
            success = false;
            return 0;
          }
      case TK_AND: return val1 && val2;
      case TK_EQ: return val1 == val2;
      case TK_NEQ: return val1 != val2;
      case TK_OR: return val1 || val2;        
      default: success = false;  return 0;
    }
  }

}

word_t expr(char *e, bool *success) {
  if (!make_token(e)) {
    *success = false;
    return 0;
  }
  for (int j = 0; j < nr_token; j++) {
    if (tokens[j].type == TK_MULTI && (j == 0 || judgeFront(j)) ) {
      tokens[j].type = TK_DEREF;
    }
  }
  *success = inner_parentheses(0,nr_token-1);
  if(*success)
    return eval(0, nr_token - 1, success);
  else
    return 0;
  // /* TODO: Insert codes to evaluate the expression. */
  // TODO();

}


// void test(){
//   system("./tools/gen-expr/gen-expr 30 > input");
//   uint32_t ans1, ans2;
//   char buf[66666];
//   int loop =30;
//   FILE *fp = fopen("./input", "r");
//   bool success =true;
//   while(loop--){
//     success = true;
//     fscanf(fp, "%u", &ans1);
//     fscanf(fp, "%s", buf);
//     ans2 = expr(buf, &success);
//     if(ans1 != ans2){
//       printf("%s\n", buf);
//       break;
//       }else{
//         printf("Right!\n");
//       }
    
//   }
//   fclose(fp);
// }
