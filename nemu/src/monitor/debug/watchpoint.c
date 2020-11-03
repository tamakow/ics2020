#include "watchpoint.h"
#include "expr.h"

#define NR_WP 32

static WP wp_pool[NR_WP] = {};
static WP *head = NULL, *free_ = NULL;

void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].next = &wp_pool[i + 1];
  }
  wp_pool[NR_WP - 1].next = NULL;

  head = NULL;
  free_ = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */
WP* new_wp(char* s){
  if(free_ == NULL){
    printf("No watchpoint to new.\n Please check the NP_WP\n");
    assert(0);
  }
  bool succ = true;
  WP* new_ = free_;
  free_ = free_->next;
  //TODO: update new_ if there are more members in WP struct
  strcpy(new_->str,s);
  new_->oldvalue = expr(s,&succ); 
  if(succ == false){
    printf("There are something wrong with expression calculation!\n");
    panic("Please input the legal expression");
    return 0;
  }
  new_->newvalue = new_->oldvalue;


  WP* temp_head=head;
  if(temp_head==NULL){
    head = new_;
    head->next = NULL;
    head->NO = 1;
  }
  else{
    int num=2;
    while(temp_head->next!=NULL){
      temp_head=temp_head->next;
      num++;
    }
    temp_head->next=new_;
    new_->next=NULL;
    new_->NO=num;
  }
  return new_; 
}

WP* search_wp(int w_NO){
  WP* temp_head = head;
  if(temp_head == NULL){
    printf("Can't seach the NO.%d watchpoint!\n",w_NO);
    assert(0);
  }
  while(temp_head->next != NULL && temp_head->next->NO != w_NO)
    temp_head=temp_head->next;
  if(temp_head == NULL){
    printf("Can't search the NO.%d watchpoint!\n",w_NO);
    assert(0);
  }
  return temp_head;
}

void update_wp_NO(WP* h){
  while(h!=NULL){
    h->NO--;
    h=h->next;
  }
  return;
}

void free_wp(int w_NO){
  if(head == NULL){
    printf("No watchpoint to delete!\n");
    assert(0);
  }
  WP* to_delete;
  if(head->NO == w_NO){
    to_delete = head;
    head = head->next;
    update_wp_NO(head);
  }
  else{
    WP* pre = search_wp(w_NO);
    to_delete = pre->next;
    pre->next = to_delete->next;
    update_wp_NO(pre->next);
  }
  to_delete->next = free_;
  free_ = to_delete;
  printf("Delete the NO.%d watchpoint : %s\n",to_delete->NO,to_delete->str);
  memset(free_->str,0,sizeof(free_));
  return;
}

bool check_wp(){
  if(head == NULL) return 0;
  bool succ = true;
  WP* temp_head = head;
  while(temp_head != NULL){
    temp_head->newvalue=expr(temp_head->str,&succ);
    if(succ == false){
      printf("There are something wrong with expression calculation!\n");
      assert(0);
    }
    if(temp_head->newvalue!=temp_head->oldvalue)
      return 1;
    temp_head = temp_head->next;
  }
  return 0;
}

void print_wp(){
  if(head == NULL){
    printf("No watchpoint now\n");
    return;
  }
  printf("Num\t\tNew_value(HEX)\tOld_value(HEX)\tNew_vale(DEC)\tOld_value(DEC)\tExpression\n");
  WP* temp_head = head;
  while(temp_head != NULL){
    printf("%4d\t\t0X%x\t0X%x\t%d\t%d\t%s",temp_head->NO,temp_head->newvalue,temp_head->oldvalue,temp_head->newvalue,temp_head->oldvalue,temp_head->str);
    temp_head = temp_head->next;
  }
  return;
}

void update_wp(){
  WP* temp_head = head;
  while(temp_head != NULL){
    temp_head->oldvalue = temp_head->newvalue;
    temp_head =temp_head->next;
  }
  return;
}