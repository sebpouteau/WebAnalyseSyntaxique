#include <stdlib.h>
#include <ast.h>

static struct ast * mk_node(void){
    struct ast *e = malloc(sizeof(struct ast));
    e->node = malloc(sizeof(union node));
    return e;
}

struct ast * mk_integer(int n){
    struct ast * e = mk_node();
    e->type = INTEGER;
    e->node->num = n;
    return e;
}

struct ast * mk_binop(enum binop binop){
    struct ast * e = mk_node();
    e->type = BINOP;
    e->node->binop = binop;
    return e;
}

struct ast * mk_unaryop(enum unaryop unaryop){
    struct ast * e = mk_node();
    e->type = UNARYOP;
    e->node->unaryop = unaryop;
    return e;
}

struct ast * mk_var(char * var){
    struct ast * e = mk_node();
    e->type = VAR;
    e->node->str = var;
    return e;

}

struct ast * mk_import(struct path * chemin){
    struct ast * e = mk_node();
    e->node->chemin = malloc(sizeof(struct path));
    e->type = IMPORT;
    e->node->chemin = chemin;
    return e;
}

struct ast * mk_app(struct ast * fun, struct ast * arg){
    struct ast * e = mk_node();
    e->node->app = malloc(sizeof(struct app));
    e->type = APP;
    e->node->app->fun = fun;
    e->node->app->arg = arg;
    return e;
}

struct ast * mk_word(char * str){
    struct ast * e = mk_node();
    e->type = WORD;
    e->node->str = str;
    return e;
}

struct ast * mk_tree(char * label, bool is_value, bool nullary, bool space,
                     struct attributes * att, struct ast * daughters){
    struct ast * e = mk_node();
    e->node->tree = malloc(sizeof(struct tree));
    e->type = TREE;
    e->node->tree->label = label;
    e->node->tree->is_value=is_value;
    e->node->tree->nullary=nullary;
    e->node->tree->space=space;
    e->node->tree->attributes=att;
    e->node->tree->daughters=daughters;
    return e;
}

struct ast * mk_forest(bool is_value, struct ast * head, struct ast * tail){
    struct ast * e = mk_node();
    e->node->forest = malloc(sizeof(struct forest));
    e->type = FOREST;
    e->node->forest->is_value = is_value;
    e->node->forest->head=head;
    e->node->forest->tail=tail;
    return e;
}

struct ast * mk_fun(char * id, struct ast * body){
    struct ast * e = mk_node();
    e->node->fun = malloc(sizeof(struct fun));
    e->type = FUN;
    e->node->fun->id = id;
    e->node->fun->body=body;
    return e;
}

struct ast * mk_match(struct ast * ast, struct patterns * patterns){
    struct ast * e = mk_node();
    e->node->match = malloc(sizeof(struct match));
    e->type = MATCH;
    e->node->match->ast = ast;
    e->node->match->patterns=patterns;
    return e;
}

struct ast * mk_cond(struct ast * cond, struct ast * then_br, struct ast * else_br){
    struct ast * e = mk_node();
    e->node->cond = malloc(sizeof(struct cond));
    e->type = COND;
    e->node->cond->cond = cond;
    e->node->cond->then_br=then_br;
    e->node->cond->else_br=else_br;
    return e;
}

struct ast * mk_declrec(char * id, struct ast * body){
    struct ast * e = mk_node();
    e->node->fun = malloc(sizeof(struct fun));
    e->type = DECLREC;
    e->node->fun->id = id;
    e->node->fun->body=body;
    return e;
}
