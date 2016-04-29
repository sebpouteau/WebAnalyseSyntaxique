
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <ast.h>
#include <graphviz.h>
#include <queue.h>
#include <import.h>
#include <util.h>
#define MAX_SIZE 500


/* ===========================
   CONSTRUCTEURS UTILS
   =========================== */


struct dir* mk_dir(char* name, enum descr type, struct dir* next){
  struct dir* d = malloc(sizeof(*d));
  d->str = name;
  d->descr = type;
  d->dir = next;
  return d;
}

struct path* mk_path(int srep, struct dir * d){
  struct path* p = malloc(sizeof(*p));
  p->n = srep;
  p->dir = d;
  return p;
}
     
struct ast* mk_comp_ast(char * str){
  if(! strcmp(str, "==")){
    return mk_binop(EQ);
  }
  if(! strcmp(str, "!=")){
    return mk_binop(NEQ);
  }
  if(! strcmp(str, "<=")){
    return mk_binop(LEQ);
  }
  if(! strcmp(str, ">=")){
    return mk_binop(GEQ);
  }
  if(! strcmp(str, "<")){
    return mk_binop(LE);
  }
  if(! strcmp(str, ">")){
    return mk_binop(GE);
  }
  if(! strcmp(str, "||")){
    return mk_binop(OR);
  }
  if(! strcmp(str, "&&")){
    return mk_binop(AND);
  }
  return NULL;
}

struct attributes* mk_attributes(struct ast* key, struct ast* value, struct attributes* next){
  struct attributes * a = malloc(sizeof(*a));
  a->is_value = true;
  a->key = key;
  a->value = value;
  a->next = next;
  return a;
}


struct patterns* mk_patterns(struct  pattern * pattern, struct ast * res, struct patterns * next)   {
  struct patterns *pat = malloc(sizeof(struct patterns));
  pat->next = next;
  pat->res = res;
  pat->pattern = pattern;
  return pat;
}

struct declare*  mk_declare(struct queue_t* q, struct ast* a){
  struct declare * d = malloc(sizeof(*d));
  d->args = q;
  d->body = a;
  return d;
}


/* ===========================
   ADD RIGHT UTILS
   =========================== */


void add_pforest_right(struct pattern* filter, struct pattern* element){
  struct pforest* forest = filter->pnode->pforest;
  while (forest->tail != NULL){
    forest =  forest->tail->pnode->pforest;
  }
  if(element != NULL){
    struct pattern* f = mk_pforest(element, NULL);
    forest->tail = f;
  }
}

void add_patterns_right(struct patterns* filter, struct patterns* element){
  while (filter->next != NULL){
    filter =  filter->next;
  }
  filter->next = element;
}  
                          

struct ast* add_right(struct ast* frst, struct ast* element){
  if (frst == NULL)
    return mk_forest(false,element, NULL);

  struct forest* f = frst->node->forest;
  while (f->tail != NULL){
    f=f->tail->node->forest;
  }
  struct ast* new = mk_forest(false,element, NULL);
  f->tail = new; 
  return frst;
}

struct dir* add_dir_right(struct dir* src, struct dir* next){
  if(src == NULL){
    return next;
  }
  struct dir* tmp = src;
  while(tmp->dir != NULL){
    tmp = tmp->dir;
  }
  tmp->dir = next;
  return src;
}

void add_head(struct ast* head, struct ast* head_tree, struct env* e){
  if (head_tree != NULL){
    struct closure* cl_node = process_content(head_tree, e);
    struct ast* node = cl_node->value;
    if ( head == NULL){
      head = mk_forest(false, node, NULL);
      return;
    }
    struct forest* t = head->node->forest;
    while (t->tail != NULL){
      t = (struct forest*) t->tail;
    }
    struct ast* f = mk_forest(false, node, NULL);
    t->tail = f;
  }
}


/* ===========================
   FONCTION UTILS
   =========================== */


struct ast* create_ast_in(struct ast* body, struct declare* decl, bool is_rec){
  char* name = queue_front(decl->args);
  queue_pop(decl->args);
  
  struct ast* fun = mk_fun(name, body);
  struct ast* args = create_args(decl, name, is_rec);
 
  struct ast* a = mk_app(fun,args);
  a = mk_forest(false, a, NULL);
  return a;
}

struct ast* create_args(struct declare* decl, char*name, bool is_rec){
  if(queue_empty(decl->args)){
    return decl->body;
  }
  char * n =(char *) queue_front(decl->args);
  queue_pop(decl->args);
  struct ast* a = mk_fun(n , create_args(decl, n, false));
  if (is_rec)
    a = mk_declrec(name, a);
  return a;
}

char* convert_to_html(bool *is_emit, char* string){
  if (*is_emit){
    *is_emit = false;
    return strdup(string);
  }
  char * base = "&#";
  char final_string[MAX_SIZE];
  int j=0;
  for(unsigned long i = 0 ; i < strlen(string) ; i++){
    //Char entre a-z ou A-Z
    if((65 <= string[i] && string[i] <= 90) || (97 <= string[i] && string[i] <= 122) || ( 48 <=string[i] && string[i] <= 57) ){ 
      final_string[j] = string[i];
      j++;
    }
    else if(string[i] != 32){
      sprintf(final_string+j, "%s%d;", base, string[i]);
      while(final_string[j]!='\0')
        j++;
    }
  }
  final_string[j]='\0';
  return strdup(final_string);
}


struct path* split_path(char * str){
  struct dir* src = NULL;
  int srep = 0;
  char* dir_name = malloc(200*sizeof(char));
  int count = 0;
  unsigned long i = 1;
  
  //on compte le nombre de retour
  while(str[i] == '.'){
    srep++;
    i++;
  }
  
  for(  ; i < strlen(str) ; i++){
    if (str[i] == '/'){
      src = add_dir_right(src, mk_dir(strdup(dir_name), DIR, NULL));    
      memset(dir_name, 0, sizeof(*dir_name));
      count = 0;
    }
    else {
      dir_name[count] = str[i];
      count++;
    }
  }
  
  char* file_name = malloc(200 * sizeof(char));
  count = 0;
  unsigned long j = 0;
  for( ; j < strlen(dir_name); j++){
    if(j+1 < strlen(dir_name) && dir_name[j] == '-' && dir_name[j+1] == '>'){
      j+=2;
      break;
    } else {
      if(dir_name[j] == ';'){
        break;
      }
      file_name[count] = dir_name[j];
      count++;
    }
  }
  
  src = add_dir_right(src, mk_dir(strdup(file_name), FILENAME, NULL));
  memset(file_name, 0, sizeof(*file_name));
  count = 0;
  
  for( ; j < strlen(dir_name); j++){
    if (dir_name[j] == ';')
      break;
    file_name[count] = dir_name[j];
    count++;
  }
  
  if(count != 0){
    src = add_dir_right(src, mk_dir(strdup(file_name), DECLNAME, NULL));
  }

  struct path* p = mk_path(srep, src);
  free(file_name);
  free(dir_name);
  return p;
}
