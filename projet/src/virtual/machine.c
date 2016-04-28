#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <machine.h>
#include <pattern_matching.h>
#include <import.h>
#include <xml_builder.h>

void emit( char * file, struct ast * ast){
    assert(file!=NULL && (ast ==NULL || ast!= NULL));
    //build_xml(file, ast);
    return;
}

struct env * mk_env(char * var, struct closure * value, struct env * next){
    struct env * res = malloc(sizeof(struct env));
    res->var = var;
    res->value = value;
    res->next=next;
    return res;
}

void push_env(char * var, struct closure * value, struct env ** e){
    *e = mk_env(var,value, *e);
    return;
}


struct closure * mk_closure(struct ast * value, struct env * env){
    struct closure * res = malloc(sizeof(struct closure));
    res->value = value;
    res->env = env;
    return res;
}

enum ast_type get_ast_type(struct ast * t){
    if(t==NULL){return FOREST;}
    else{return t->type;}
}

char * string_of_int(int k){
    char * c;
    c=malloc(21*sizeof(char));
    sprintf(c,"%d",k);
    return c;
}

struct ast * to_forest(struct ast * t){
    enum ast_type tp = get_ast_type(t);
    switch (tp) {
    case WORD:
    case TREE:
    case INTEGER: {
        return mk_forest(true,t,NULL);
        break;
    }
    case FOREST: {
        return t;
        break;
    }
    default:
        fprintf(stderr,"Erreur de typage, seul les mots, arbres, entiers ou forêts peuvent être convertis en forêt.");
        exit(1);
        break;
    }
    return t;    
}


void pop_stack(struct machine * m){
    assert(m->stack!=NULL);
    struct stack * tmp = m->stack->next;
    switch(m->stack->top->type){
    case FUNCTION:
    case FORESTCOMPHEAD:
    case FORESTCOMPTAIL:
    case CLOSURE:
        break;
    case TREECOMPATT:
        free(m->stack->top->item->tree_att);
        break;
    case TREECOMPFOREST:
        free(m->stack->top->item->tree_forest);
        break;
    case ATTCOMPKEY:
        free(m->stack->top->item->attributes_key);
        break;
    case ATTCOMPVALUE:
        free(m->stack->top->item->attributes_value);
        break;
    case ATTCOMPNEXT:
        free(m->stack->top->item->attributes_next);
        break;
    case MATCHCOMP:
        free(m->stack->top->item->match);
        break;
    case CONDCOMP:
        free(m->stack->top->item->cond);
        break;
    }
    free(m->stack->top->item);
    free(m->stack->top);
    free(m->stack);
    m->stack=tmp;
}


void pop_stack_cond_then(struct machine * m){
    assert(m->stack->top->type==CONDCOMP);
    m->closure = mk_closure(m->stack->top->item->cond->then_br,
                            m->stack->top->item->cond->env);
    pop_stack(m);
}

void pop_stack_cond_else(struct machine * m){
    assert(m->stack->top->type==CONDCOMP);
    m->closure = mk_closure(m->stack->top->item->cond->else_br,
                            m->stack->top->item->cond->env);
    pop_stack(m);
}

void push_stack(union item * it, enum item_type type, struct machine * m){
    struct stack * st= malloc(sizeof(struct stack));
    struct stack_element * se = malloc(sizeof(struct stack_element));
    se->type = type;
    se->item = it;
    st->top = se;
    st->next=m->stack;
    m->stack = st;
}

void push_closure(struct ast * val, struct env * env,
                  enum item_type tp,struct machine * m){
    assert(tp==CLOSURE || tp == FORESTCOMPHEAD || tp == FORESTCOMPTAIL
           || tp== FUNCTION);
    struct closure * cl = mk_closure(val,env);
    union item *  it= malloc(sizeof(union item));
    it->closure = cl;
    push_stack(it,tp,m);
}


void push_stack_attributes_key(struct ast * value,
                               struct attributes * next,
                               struct env * e,
                               struct machine * m){
    struct attributes_computation_key * att = malloc(sizeof(struct attributes_computation_key));
    att->value = value;
    att->next = next;
    att->env=e;
    union item *  it= malloc(sizeof(union item));
    it->attributes_key = att;
    push_stack(it,ATTCOMPKEY,m);
}

void reconstruct_attributes(struct machine * m, struct attributes * tail){
    struct attributes * next, * att;
    char * label;
    bool nullary, space;
    struct ast * daughters;
    struct env * env;
    att = tail;
    while(m->stack !=NULL && m->stack->top->type==ATTCOMPNEXT){
        next = att;
        att = malloc(sizeof(struct attributes));
        att->key = m->stack->top->item->attributes_next->key;
        att->value = m->stack->top->item->attributes_next->value;
        att->is_value = true; 
        att->next =next;
        pop_stack(m);
    }
    if(m->stack!=NULL && m->stack->top->type==TREECOMPATT){
        label = m->stack->top->item->tree_att->label;
        nullary = m->stack->top->item->tree_att->nullary;
        space = m->stack->top->item->tree_att->space;
        daughters = m->stack->top->item->tree_att->daughters;
        env = m->stack->top->item->tree_att->env;
        free(m->stack->top->item->tree_att);
        m->stack->top->item->tree_forest = malloc(sizeof(struct tree_computation_forest));
        m->stack->top->type = TREECOMPFOREST;
        m->stack->top->item->tree_forest->label = label;
        m->stack->top->item->tree_forest->nullary = nullary;
        m->stack->top->item->tree_forest->space = space;
        m->stack->top->item->tree_forest->attributes = att;
        m->closure = mk_closure(daughters,env);
        compute(m);
    }
    else{
        fprintf(stderr,"Erreur de typage : les attributs ne peuvent être utilisés que pour construire de arbres.");
        exit(1);
    }
}

void pop_attcompvalue(struct machine * m){
    struct ast * key, * value;
    struct attributes * next;
    struct env * e;
    enum ast_type tp = get_ast_type(m->closure->value);
    switch(tp){
    case INTEGER:
        value = mk_word(string_of_int(m->closure->value->node->num));
        break;
    case WORD:
        value = m->closure->value;
        break;
    default:
        fprintf(stderr,"Erreur de typage, une valeur d'attribut ne peut qu'être un entier ou un chaîne de caractère.");
        exit(1);
        break;
    }
    key=m->stack->top->item->attributes_value->key;
    e = m->stack->top->item->attributes_value->env;
    next = m->stack->top->item->attributes_value->next;
    free(m->stack->top->item->attributes_value);
    m->stack->top->item->attributes_next = malloc(sizeof(struct attributes_computation_next));
    m->stack->top->type = ATTCOMPNEXT;
    m->stack->top->item->attributes_next->key = key;
    m->stack->top->item->attributes_next->value = value;
    if(next == NULL || next->is_value){
        //ici on vient de terminer le calcul de la liste des attributs
        reconstruct_attributes(m,next);
    }
    else{
        //on doit poursuivre le calcul de la liste des attributs
        push_stack_attributes_key(next->value,next->next,e,m);
        m->closure= mk_closure(next->key,e);
        compute(m);
    }
}

void pop_attcompkey(struct machine * m){
    struct ast * key, * value;
    struct attributes * next;
    struct env * e;
    enum ast_type tp = get_ast_type(m->closure->value);
    switch(tp){
    case WORD:
        key = m->closure->value;
        break;
    default:
        fprintf(stderr,"Erreur de typage, une valeur d'attribut ne peut qu'être un entier ou un chaîne de caractère.");
        exit(1);
        break;
    }
    value=m->stack->top->item->attributes_key->value;
    e = m->stack->top->item->attributes_key->env;
    next = m->stack->top->item->attributes_key->next;
    free(m->stack->top->item->attributes_key);
    m->stack->top->item->attributes_value = malloc(sizeof(struct attributes_computation_value));
    m->stack->top->type = ATTCOMPVALUE;
    m->stack->top->item->attributes_value->key = key;
    m->stack->top->item->attributes_value->next = next;
    m->stack->top->item->attributes_value->env = e;
    m->closure = mk_closure(value,e);
    compute(m);
}


void reconstruct_forest(struct machine * m, struct ast * tail){
    struct ast * forest, * next;
    enum ast_type tp;
    forest=tail;
    while(m->stack !=NULL && m->stack->top->type==FORESTCOMPTAIL){
        tp = get_ast_type(m->stack->top->item->closure->value);
        next = forest;
        switch(tp){
        case TREE:
        case WORD:
        {
            forest = mk_forest(true,
                               m->stack->top->item->closure->value,
                               next);
            break;
        }
        case INTEGER:{
            forest = mk_forest(true,
                               mk_word(string_of_int(m->stack->top->item->closure->value->node->num)),
                               next);
            break;
        }
        case FOREST:{
            if(m->stack->top->item->closure->value!=NULL){
                forest = clone_concat_forest(m->stack->top->item->closure->value,
                                    next);
            }
            else{
                forest = next;
            }
            break;
        }   
        default: {
            exit(1);
        }   
        }
        
        pop_stack(m);
    }
    m->closure = mk_closure(forest,NULL);
    compute(m);
}

void pop_forestcomphead(struct machine * m){

    struct ast * head;
    struct ast * tail;
    struct env * env;
    enum ast_type tp = get_ast_type(m->closure->value);
    switch(tp){
    case INTEGER:
        head= mk_word(string_of_int(m->closure->value->node->num));
        break;
    case TREE:
    case WORD:
    case FOREST:
        head = m->closure->value;
        break;
    default:
        fprintf(stderr,("Erreur de typage, la tête d'une forêt ne peut contenir que des chaîne, des mots ou des nombres"));
        exit(1);
        break;
    }
    tail = m->stack->top->item->closure->value;
    env = m->stack->top->item->closure->env;
    m->stack->top->item->closure->value = head;
    m->stack->top->type = FORESTCOMPTAIL;
    if(tail==NULL || tail->node->forest->is_value){
        reconstruct_forest(m,tail);
    }
    else{
        push_closure(tail->node->forest->tail,env,FORESTCOMPHEAD,m);
        m->closure = mk_closure(tail->node->forest->head,env);
        compute(m);
    }
}

void pop_function(struct machine * m){
    enum ast_type tp = get_ast_type(m->closure->value);
    assert((tp == TREE ||
            tp == FOREST ||
            tp == INTEGER ||
            tp == FUN ||
            tp == BINOP ||
            tp == UNARYOP ||
            tp == WORD) &&
           m->stack->top->type == FUNCTION
        );
    struct closure * cl=m->closure;
    m->closure = m->stack->top->item->closure;
    m->stack->top->item->closure =cl;
    m->stack->top->type=CLOSURE;
    compute(m);
}

void pop_match(struct machine * m){
    enum ast_type tp = get_ast_type(m->closure->value);
    struct ast * res;
    assert((tp == TREE ||
            tp == FOREST ||
            tp == WORD) &&
           m->stack->top->type == MATCHCOMP);
    struct env * e=m->stack->top->item->match->env;
    struct patterns * pats = m->stack->top->item->match->patterns;
    while(pats!=NULL){
      if(match(pats,m->closure->value,&res,&e)){
        m->closure = mk_closure(res,e);
        pop_stack(m);
        compute(m);
        return;
      }
      pats = pats->next;
    }
    fprintf(stderr,"Filtrage non-exhaustif, calcul interrompu.");
    exit(1);
}

void pop_cond(struct machine * m){
    if(m->closure->value==NULL){
        pop_stack_cond_else(m);
    }
    else{
        switch(m->closure->value->type){
        case INTEGER:
            if(m->closure->value->node->num){
                pop_stack_cond_then(m);
                compute(m);
            }
            else{
                pop_stack_cond_else(m);
                compute(m);
            }
            break;
        default:
            pop_stack_cond_then(m);
            break;
        }
    }    
}

void pop_forestcomptail(struct machine * m){
    enum ast_type tp = get_ast_type(m->closure->value);
    struct ast * forest;
    switch(tp){
    case FOREST:
        forest = m->closure->value;
        break;
    case TREE:
    case WORD:
        forest = mk_forest(true,
                           m->closure->value,
                           NULL);
            
        break;
    case INTEGER:
        forest = mk_forest(true,
                           mk_word(string_of_int(m->closure->value->node->num)),
                           NULL);
        break;
    default:
        fprintf(stderr,"Erreur de typage: une forêt ne peut être construite qu'à partir d'une forêt, d'un arbre, d'un mot ou d'un entier");
        exit(1);
    }
    m->closure = mk_closure(clone_concat_forest(m->stack->top->item->closure->value,forest),NULL);
    pop_stack(m);
    compute(m);
}

void pop_treecompforest(struct machine * m){
    assert(m->stack!=NULL && m->stack->top->type==TREECOMPFOREST);
    enum ast_type tp = get_ast_type(m->closure->value);
    struct ast * forest;
    switch(tp){
    case FOREST:
        forest = m->closure->value;
        break;
    case TREE:
    case WORD:
        forest = mk_forest(true,
                           m->closure->value,
                           NULL);
            
        break;
    case INTEGER:
        forest = mk_forest(true,
                           mk_word(string_of_int(m->closure->value->node->num)),
                           NULL);
        break;
    default:
        fprintf(stderr,"Erreur de typage: un forêt ne peut être construite qu'à partir d'une forêt, d'un arbre, d'un mot ou d'un entier");
        exit(1);
    }
    m->closure = mk_closure(mk_tree(
        m->stack->top->item->tree_forest->label,
        true,
        m->stack->top->item->tree_forest->nullary,
        m->stack->top->item->tree_forest->space,
        m->stack->top->item->tree_forest->attributes,
        forest), NULL);
    pop_stack(m);
    compute(m);
}

void on_integer(struct machine * m){
    enum ast_type tp = get_ast_type(m->closure->value);
    assert(tp == INTEGER);
    if(m->stack!=NULL){
        switch(m->stack->top->type){
        case CLOSURE:
            fprintf(stderr,"Erreur de typage, un entier ne peut recevoir un argument.");
            exit(1);
        case TREECOMPATT:
            fprintf(stderr,"Erreur de typage, un entier ne peut constituer la liste des attributs d'un arbre.");
            exit(1);
            break;
        case TREECOMPFOREST:
            pop_treecompforest(m);
            break;
        case ATTCOMPKEY:
            fprintf(stderr,"Erreur de typage, un entier ne peut être la clé d'un attribut.");
            exit(1);
            break;
        case ATTCOMPVALUE:
            pop_attcompvalue(m);
            break;
        case ATTCOMPNEXT:
            fprintf(stderr,"Erreur de typage, un entier ne peut être utilisé comme séquence d'attribut.");
            exit(1);
            break;
        case FORESTCOMPHEAD:
            pop_forestcomphead(m);
            break;
        case FORESTCOMPTAIL:
            pop_forestcomptail(m);
            break;
        case FUNCTION:
            pop_function(m);
            break;
        case MATCHCOMP:
            fprintf(stderr,"Erreur de typage, un entier ne peut être filtré par une construction 'match'.");
            exit(1);
            break;
        case CONDCOMP:
            pop_cond(m);  
            break;
        }
    }
}


struct ast * clone_concat_forest(struct ast * f1, struct ast * f2){
    assert(get_ast_type(f1)==FOREST &&
           get_ast_type(f2)==FOREST);
    if(f1==NULL){
        return f2;
    }
    else if(f2==NULL){
        return f1;
    }
    else{
        return
            mk_forest(true,
                      f1->node->forest->head,
                      clone_concat_forest(f1->node->forest->tail,
                                          f2)
                );
    }

}


void on_binop(struct machine * m){
    enum ast_type tp = get_ast_type(m->closure->value);
    enum ast_type tp1, tp2;
    assert(tp == BINOP);
    int k;
    char * c;
    struct ast * ast;
    struct ast * f1, *f2;
    if(m->stack->top != NULL){
        if(m->stack->top->type==CLOSURE){
            if(m->stack->next!=NULL){
                if(m->stack->top->type==CLOSURE){
                    tp1 = get_ast_type(m->stack->top->item->closure->value);
                    tp2 = get_ast_type(m->stack->next->top->item->closure->value);
                    if(tp1 == INTEGER && tp2 == INTEGER){
                        switch(m->closure->value->node->binop){
                        case PLUS:
                            k=m->stack->top->item->closure->value->node->num+
                                m->stack->next->top->item->closure->value->node->num;
                            break;
                        case MINUS:
                            k=m->stack->top->item->closure->value->node->num-
                                m->stack->next->top->item->closure->value->node->num;
                            break;
                        case MULT:
                            k=m->stack->top->item->closure->value->node->num*
                                m->stack->next->top->item->closure->value->node->num;
                            break;
                        case DIV:
                            if(m->stack->next->top->item->closure->value->node->num!=0){
                                k=m->stack->top->item->closure->value->node->num/
                                    m->stack->next->top->item->closure->value->node->num;
                            }
                            else{
                                fprintf(stderr,"Division par 0");
                                exit(1);
                            }
                            break;
                        case LEQ:
                            k=m->stack->top->item->closure->value->node->num <=
                                m->stack->next->top->item->closure->value->node->num;
                            break;
                        case LE:
                            k=m->stack->top->item->closure->value->node->num <
                                m->stack->next->top->item->closure->value->node->num;
                            break;
                        case GEQ:
                            k=m->stack->top->item->closure->value->node->num >=
                                m->stack->next->top->item->closure->value->node->num;
                            break;
                        case GE:
                            k=m->stack->top->item->closure->value->node->num >
                                m->stack->next->top->item->closure->value->node->num;
                            break;
                        case EQ:
                            k=m->stack->top->item->closure->value->node->num==
                                m->stack->next->top->item->closure->value->node->num;
                            break;
                        case NEQ:
                            k=m->stack->top->item->closure->value->node->num !=
                                m->stack->next->top->item->closure->value->node->num;
                            break;
                        case OR:
                            k=m->stack->top->item->closure->value->node->num ||
                                m->stack->next->top->item->closure->value->node->num;
                            break;
                        case AND:
                            k=m->stack->top->item->closure->value->node->num &&
                                m->stack->next->top->item->closure->value->node->num;
                            break;
                        case EMIT:
                            fprintf(stderr,"Erreur de typage, emit ne peut être appliqué à des entiers.");
                            exit(1);
                            break;
                        }
                        m->closure = mk_closure(mk_integer(k), NULL);
                        pop_stack(m);
                        pop_stack(m);
                        compute(m);
                        return;
                    }
                    else if(tp1 == WORD && tp2 == WORD)
                    {
                        if(m->closure->value->node->binop==PLUS){
                            c = malloc((1+
                                        strlen(m->stack->top->item->closure->value->node->str)+
                                        strlen(m->stack->next->top->item->closure->value->node->str)) *
                                       sizeof(char)
                                );
                            strcat(strcpy(c,m->stack->top->item->closure->value->node->str),
                                   m->stack->next->top->item->closure->value->node->str
                                );
                            m->closure = mk_closure(mk_word(c),NULL);
                            pop_stack(m);
                            pop_stack(m);
                            compute(m);
                            return;
                        }                       
                        else{
                            k=strcmp(m->stack->top->item->closure->value->node->str,
                                     m->stack->next->top->item->closure->value->node->str);
                            switch(m->closure->value->node->binop){
                            case LEQ:
                                k= k<=0;
                                break;
                            case LE:
                                k=k<0;
                                break;
                            case GEQ:
                                k=k>=0;
                                break;
                            case GE:
                                k=k>0;
                                break;
                            case EQ:
                                k=k==0;
                                break;
                            case NEQ:
                                k=k!=0;
                                break;
                            default:
                                fprintf(stderr,"Erreur de typage, les opérateurs possibles entre chaînes sont +, <=, <, >=, >, ==, et !=");
                                exit(1);
                                break;
                            }
                            m->closure = mk_closure(mk_integer(k), NULL);
                            pop_stack(m);
                            pop_stack(m);
                            compute(m);
                        }
                    }
                    else if(tp1==WORD &&
                            (tp2==FOREST || tp2 == TREE) &&
                            m->closure->value->node->binop==EMIT
                        ){
                        emit(m->stack->top->item->closure->value->node->str,
                             m->stack->next->top->item->closure->value);
                        pop_stack(m);
                        pop_stack(m);
                        if(m->stack !=NULL && m->stack->top->type==CLOSURE){
                            m->closure = m->stack->top->item->closure;
                            pop_stack(m);
                            compute(m);
                        }
                    }
                    else if((tp1==FOREST || tp1 == WORD || tp1 ==TREE || tp1 == INTEGER) 
                            &&
                            (tp2==FOREST || tp2 ==WORD || tp2 == TREE || tp2 == INTEGER)){
                        if(m->closure->value->node->binop==PLUS){
                            f1 = to_forest(m->stack->top->item->closure->value);
                            f2 = to_forest(m->stack->next->top->item->closure->value);
                            ast = clone_concat_forest(f1 ,f2);
                            m->closure= mk_closure(ast, NULL);
                            pop_stack(m);
                            pop_stack(m);
                            compute(m);
                        }
                        else{
                            fprintf(stderr,"Erreur de typage, le seul opérateur possible entre forêts est +\n"); 
                        }                
                    }

                    else{
                        fprintf(stderr,"Erreur de typage, les opérations binaires ne peuvent prendre en argument que des entiers, des mots ou des forêts.");
                    }
                }
                else{
                    fprintf(stderr,"Erreur de typage, les opérations binaires ne peuvent prendre en argument que des entiers, des mots ou des forêts.");
                }
            }
            else{
                if(m->stack->top->item->closure->value->type == INTEGER ||
                   (m->stack->top->item->closure->value->type == FOREST &&
                    m->closure->value->node->binop == PLUS)){
                    return;
                }
                else {
                    fprintf(stderr,"Erreur de typage, les opérations binaires ne peuvent prendre en argument que des entiers des mots ou des forêts.");
                }
            }
            
        }
        else{
            fprintf(stderr,"Erreur de typage, les opérations binaires ne peuvent prendre en argument que des entiers, des mots ou des forêts.");
        }
    }
}

void on_unaryop(struct machine * m){
    int k;
    enum ast_type tp;
    if(m->stack->top != NULL){
        if(m->stack->top->type==CLOSURE){
            tp = get_ast_type(m->stack->top->item->closure->value);
            if(tp==INTEGER){
                if(m->closure->value->node->unaryop==NOT){
                    k = ! m->stack->top->item->closure->value->node->num;
                }
                else if(m->closure->value->node->unaryop==NEG){
                    k = -m->stack->top->item->closure->value->node->num;
                }
                else{
                    fprintf(stderr,"Erreur de typage, les opérations unaires compatibles avec les entiers sont ! et -.");
                    exit(1);
                }
                m->closure=mk_closure(mk_integer(k),NULL);
                pop_stack(m);
                compute(m);
            }
            else if(tp==FOREST){
                if(m->closure->value->node->unaryop==NOT){
                    k= m->stack->top->item->closure->value !=NULL;
                }
                else{
                    fprintf(stderr,"Erreur de typage, la seule opération unaire compatible avec les forêts est !.");
                    exit(1);
                }
                m->closure=mk_closure(mk_integer(k),NULL);
                pop_stack(m);
                compute(m);
                
            }
        }
    }
    
}

void on_var(struct machine * m){
    char * c = m->closure->value->node->str;
    struct env * e = m->closure->env;
    while(e!=NULL){
        if(!strcmp(c,e->var)){
            m->closure = e->value;
            compute(m);
            return;
        }
        else{
            e=e->next;
        }
    }
    fprintf(stderr,
            "Variable %s non trouvée",
            c);
    exit(1);
}

static char * find_declname(struct dir* d){
  if(d == NULL)
    return NULL;
  if(d->descr == DECLNAME){
    return d->str;
  }
  return find_declname(d->dir);
}

static void print_queue_state(struct files* file_list){
  printf("=========\nQUEUE STATE:\n");
  struct files* tmp = file_list;
  while(tmp != NULL){
    printf("The file %s is loaded\n", tmp->file_name);
    tmp = tmp->next;
  }
  printf("=========\n");
}

void on_import(struct machine * m){
    assert(m!=NULL);
    struct path * p = m->closure->value->node->chemin;
    printf("\nPATH: %s\n", from_path_to_name(p));
    char* func_name = find_declname(p->dir);
    if(func_name != NULL){
      printf("FUNC: %s\n", func_name);
    } else {
      printf("Have to load full file, no var name specified\n");
    }

    struct files* file_list = NULL;    
    //Apres avoir récup le nom de ma variable (ou non) je dois la trouver
    if(retrieve_tree(p, file_list) == NULL){//Si le fichier n'est pas chargé
      printf("The file %s wasn't loaded\n", from_path_to_name(p));
      file_list = add_file(p, m->closure, file_list); //On le charge
    }
    print_queue_state(file_list);
    //Calculer les arbres des fichiers pour pouvoir avoir les fonctions etc  
    if(func_name != NULL){
      printf("Function is not empty\n");
      struct closure* tmp_cl = retrieve_name(p, func_name, file_list);
      if(tmp_cl != NULL){
        printf("Found the function %s in %s GG!\n", from_path_to_name(p), func_name);
      }
    }
}

void on_app(struct machine * m){
    struct ast * fun = m->closure->value->node->app->fun;
    m->closure->value = m->closure->value->node->app->arg;
    push_closure(fun,m->closure->env,FUNCTION,m);
    compute(m);
}

void on_word(struct machine * m){
    enum ast_type tp = get_ast_type(m->closure->value);
    assert(tp ==WORD);
    if(m->stack!=NULL){
        switch(m->stack->top->type){
        case CLOSURE:
            fprintf(stderr,"Erreur de typage, un mot ne peut recevoir un argument.");
            exit(1);
        case TREECOMPATT:
            fprintf(stderr,"Erreur de typage, un mot ne peut constituer la liste des attributs d'un arbre.");
            exit(1);
            break;
        case TREECOMPFOREST:
            pop_treecompforest(m);
            break;
        case ATTCOMPKEY:
            pop_attcompkey(m);
            break;
        case ATTCOMPVALUE:
            pop_attcompvalue(m);
            break;
        case ATTCOMPNEXT:
            fprintf(stderr,"Erreur de typage, un mot ne peut être utilisé comme séquence d'attribut.");
            exit(1);
            break;
        case FORESTCOMPHEAD:
            pop_forestcomphead(m);
            break;
        case FORESTCOMPTAIL:
            pop_forestcomptail(m);
            break;
        case FUNCTION:
            pop_function(m);
            break;
        case MATCHCOMP:
          pop_match(m);
            break;
        case CONDCOMP:
            pop_cond(m);  
            break;
        }
    }
}

void on_tree(struct machine * m){
    struct tree_computation_att * tree_att;
    struct tree_computation_forest * tree_forest;
    struct attributes_computation_key * att_key;
    union item * it1, * it2;
    if(m->closure->value->node->tree->is_value){
        //on a fini de calculer une valeur, il faut voir la suite
        if(m->stack==NULL){
            //si il n'y a rien sur la pile, on termine
            return;            
        }
        else{
            //sinon, suivant l'élément sur la pile on doit poursuivre le calcul
            //ou renvoyer une erreur de typage.
            switch(m->stack->top->type){
            case FUNCTION:
                pop_function(m);
                break;
            case TREECOMPFOREST:
                pop_treecompforest(m);
                break;
            case FORESTCOMPHEAD:
                pop_forestcomphead(m);
                break;
            case FORESTCOMPTAIL:
                pop_forestcomptail(m);
                break;
            case MATCHCOMP:
                pop_match(m);
                break;
            default:
                fprintf(stderr,"Erreur de typage, un arbre ne peut être utilisé que comme argument d'une fonction, la tête d'une forêt, ou dans une expression de filtrage (match)");
                exit(1);
                break;
            }
        
        }
    }
    else if(m->closure->value->node->tree->attributes==NULL ||
            m->closure->value->node->tree->attributes->is_value){
        //Les attributs sont calculés, il suffit de calculer la forêt
        if(m->closure->value->node->tree->daughters == NULL ||
           (m->closure->value->node->tree->daughters->type==FOREST &&
            m->closure->value->node->tree->daughters->node->forest->is_value)){
            m->closure->value->node->tree->is_value = true;
            on_tree(m);
        }
        else{
            it1=malloc(sizeof(union item));
            tree_forest = malloc(sizeof(struct tree_computation_forest));
            tree_forest->label=m->closure->value->node->tree->label;
            tree_forest->nullary=m->closure->value->node->tree->nullary;
            tree_forest->space=m->closure->value->node->tree->space;
            tree_forest->attributes=m->closure->value->node->tree->attributes;
            it1->tree_forest=tree_forest;
            push_stack(it1,TREECOMPFOREST,m);
            m->closure = mk_closure(m->closure->value->node->tree->daughters,
                                    m->closure->env);
            compute(m);
        }
    }
    else{
        //on doit calculer le contenu de l'arbre
        tree_att = malloc(sizeof(struct tree_computation_att));
        tree_att->label=m->closure->value->node->tree->label;
        tree_att->nullary=m->closure->value->node->tree->nullary;
        tree_att->space=m->closure->value->node->tree->space;
        tree_att->daughters=m->closure->value->node->tree->daughters;
        tree_att->env=m->closure->env;
        it1=malloc(sizeof(union item));
        it1->tree_att=tree_att;
        push_stack(it1,TREECOMPATT,m);
        att_key = malloc(sizeof(struct attributes_computation_key));
        att_key->value = m->closure->value->node->tree->attributes->value;
        att_key->next = m->closure->value->node->tree->attributes->next;
        att_key->env = m->closure->env;
        it2=malloc(sizeof(union item));
        it2->attributes_key=att_key;
        push_stack(it2,ATTCOMPKEY,m);
        m->closure = mk_closure(m->closure->value->node->tree->attributes->key,
                                m->closure->env);
        compute(m);
    }
    
}

void on_forest(struct machine * m){
    struct closure * cl;
    union item * it;
    
    if(m->closure->value==NULL ||
            m->closure->value->node->forest->is_value){
        //une valeur vient d'être produite, mais il faut poursuivre le calcul
        if(m->stack!=NULL){
            switch(m->stack->top->type){
            case TREECOMPFOREST:
                pop_treecompforest(m);
                break;
            case FORESTCOMPHEAD:
                pop_forestcomphead(m);
                break;
            case FORESTCOMPTAIL:
                pop_forestcomptail(m);
                break;
            case MATCHCOMP:
                pop_match(m);
                break;
            case CONDCOMP:
                pop_cond(m);
                break;
            case FUNCTION:
                pop_function(m);
                break;
            default:
                fprintf(stderr,"Erreur de typage, une forêt peut être utilisée dans la construction d'une autre forêt ou alors dans celle d'un arbre, ou encore comme argument de fonction.");
                exit(1);
                break;
            }
        }
        else{
            //un forêt vient d'être construite et le programme est terminé
            return;}
    }
    else{
        //la forêt n'est pas complètement calculée, il faut calculer ses éléments
        cl = malloc(sizeof(struct closure));
        cl->value=m->closure->value->node->forest->tail;
        cl->env=m->closure->env;
        it = malloc(sizeof(union item));
        it->closure = cl;
        push_stack(it,FORESTCOMPHEAD,m);
        m->closure = mk_closure(m->closure->value->node->forest->head,
                                m->closure->env);
        compute(m);
        
    }
}

void on_fun(struct machine * m){
    if(m->stack!=NULL){
        switch(m->stack->top->type){
        case CLOSURE:
            m->closure =
                mk_closure(
                    m->closure->value->node->fun->body,
                    mk_env(m->closure->value->node->fun->id,
                           m->stack->top->item->closure,
                           m->closure->env));
            pop_stack(m);
            compute(m);
            break;
        case FUNCTION:
            pop_function(m);
            break;
        default:
            fprintf(stderr,"Erreur de typage, fonction utilisée dans un contexte inadatpé.");
            exit(1);
            break;
        }
    }
}

void on_match(struct machine * m){
    struct match_computation * mc = malloc(sizeof(struct match_computation));
    mc->patterns = m->closure->value->node->match->patterns;
    mc->env = m->closure->env;
    union item * it = malloc(sizeof(union item));
    it->match = mc;
    push_stack(it, MATCHCOMP, m);
    m->closure = mk_closure(m->closure->value->node->match->ast,
                            m->closure->env);
    compute(m);   
}

void on_cond(struct machine * m){
    struct cond_computation * cc = malloc(sizeof(struct cond_computation));
    cc->then_br = m->closure->value->node->cond->then_br;
    cc->else_br = m->closure->value->node->cond->else_br;
    cc->env = m->closure->env;
    union item * it = malloc(sizeof(union item));
    it->cond=cc;
    push_stack(it, CONDCOMP,m);
    m->closure = mk_closure(m->closure->value->node->cond->cond,
                            m->closure->env);
    compute(m);
}

void on_decl_rec(struct machine * m){
    struct closure * cl = mk_closure(m->closure->value->node->fun->body,
                                     m->closure->env);
    struct env * e = 
        mk_env(m->closure->value->node->fun->id,
               cl,
               cl->env);
    cl->env = e;
    m->closure = cl;
    compute(m);    
}

void compute(struct machine * m){
    enum ast_type tp = get_ast_type(m->closure->value);
    switch(tp){
    case INTEGER:
      on_integer(m);
      break;
    case BINOP:
      on_binop(m);
      break;
    case UNARYOP:
      on_unaryop(m);
      break;
    case VAR:
      on_var(m);
      break;
    case IMPORT:
      on_import(m);
      break;
    case APP:
      on_app(m);
      break;
    case WORD:
      on_word(m);
      break;
    case TREE:
      on_tree(m);
      break;
    case FOREST:
      on_forest(m);
      break;
    case FUN:
      on_fun(m);
      break;
    case MATCH:
      on_match(m);
      break;
    case COND:
      on_cond(m);
      break;
    case DECLREC:
      on_decl_rec(m);
      break;
    default:
      break;
    }
    return;
}

