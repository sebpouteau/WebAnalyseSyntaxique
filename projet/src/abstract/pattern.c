#include <stdlib.h>
#include <pattern.h>

static struct pattern * mk_pnode(void){
    struct pattern *e = malloc(sizeof(struct pattern));
    e->pnode = malloc(sizeof(union pnode));
    return e;
}

struct pattern * mk_ptree(char * label, bool nullary, struct pattern * daughters){
    struct pattern * e = mk_pnode();
    e->ptype = PTREE;
    e->pnode->ptree->label=label;
    e->pnode->ptree->nullary=nullary;
    e->pnode->ptree->daughters = daughters;
    return e;
}

struct pattern * mk_anytree(bool nullary, struct pattern * daughters){
    struct pattern * e = mk_pnode();
    e->ptype = ANYTREE; 
    e->pnode->anytree->nullary=nullary;
    e->pnode->anytree->daughters = daughters;
    return e;
}

struct pattern * mk_pforest(struct pattern * head, struct pattern * tail){
    struct pattern * e = mk_pnode();
    e->ptype = PFOREST;
    e->pnode->pforest->head = head;
    e->pnode->pforest->head = tail;
    return e;
}

struct pattern * mk_pstring(char * string){
    struct pattern * e = mk_pnode();
    e->ptype = PSTRING;
    e->pnode->pstring->string = string; 
    return e;
}

struct pattern * mk_pattern_var(char * var, enum pvar pvar){
    struct pattern * e = mk_pnode();
    e->ptype = PVAR;
    e->pnode->pvar->id = var;
    e->pnode->pvar->pvar = pvar;
    return e;
}

struct pattern * mk_wildcard(enum wildcard wildcard){
    struct pattern * e = mk_pnode();
    e->ptype = WILDCARD;
    e->pnode->wildcard = wildcard;
    return e;
}
