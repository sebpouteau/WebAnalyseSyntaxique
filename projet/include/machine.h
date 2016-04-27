#ifndef MACHINE
#define MACHINE
#include "ast.h"
#include "pattern.h"

struct env;
struct closure;
struct stack;
struct machine;

struct env{
    char * var;
    struct closure * value;
    struct env * next;
};

enum item_type{
    CLOSURE, TREECOMPATT, TREECOMPFOREST, ATTCOMPKEY,ATTCOMPVALUE, ATTCOMPNEXT,
    FORESTCOMPHEAD, FORESTCOMPTAIL, FUNCTION, MATCHCOMP, CONDCOMP
};

struct closure{
    struct ast * value;
    struct env * env;
};

struct tree_computation_att{
    char * label;
    bool nullary;
    bool space;
    struct ast * daughters;
    struct env * env;
};

struct tree_computation_forest{
    char * label;
    bool nullary;
    bool space;
    struct attributes * attributes;
};

struct attributes_computation_key{
    struct ast * value;
    struct attributes * next;
    struct env * env;
};

struct attributes_computation_value{
    struct ast * key;
    struct attributes * next;
    struct env * env;
};

struct attributes_computation_next{
    struct ast * key;
    struct ast * value;
};
    
struct match_computation{
    struct patterns * patterns;
    struct env * env;
};

struct cond_computation{
    struct ast * then_br;
    struct ast * else_br;
    struct env * env;
};

union item{
    struct closure * closure;
    struct tree_computation_att * tree_att;
    struct tree_computation_forest * tree_forest;
    struct attributes_computation_key * attributes_key;
    struct attributes_computation_value * attributes_value;
    struct attributes_computation_next * attributes_next;
    struct match_computation * match;
    struct cond_computation * cond;
};

struct stack_element{
    enum item_type type;
    union item * item;
};

struct stack{
    struct stack_element * top;
    struct stack * next;
};

struct machine{
    struct closure * closure;
    struct stack * stack;
};

//Helper functions
void emit( char * file, struct ast * ast);
enum ast_type get_ast_type(struct ast * ast);
void reconstruct_attributes(struct machine * m, struct attributes * tail);
void reconstruct_forest(struct machine * m, struct ast * tail);
struct ast * clone_concat_forest(struct ast * f1, struct ast * f2);
char * string_of_int(int k);
struct ast * to_forest(struct ast * t);


// Constructeurs et destructeurs
struct env * mk_env(char * var, struct closure * value, struct env * next);
void push_env(char * var, struct closure * value, struct env ** e);
struct closure * mk_closure(struct ast * prog, struct env * env);

struct machine * mk_machine(struct closure * cl, struct stack * stack);

// push on stack
void push_stack(union item * it, enum item_type type, struct machine * m);
void push_closure(struct ast * val, struct env * env,
                  enum item_type tp,struct machine * m);
void push_stack_closure(struct closure * cl, struct machine * m);
void push_stack_attributes_key(struct ast * value,
                               struct attributes * next,
                               struct env * e,
                               struct machine * m);

// pop stack
void pop_stack(struct machine * m);
void pop_attcompvalue(struct machine * m);
void pop_attcompkey(struct machine * m);
void pop_forestcomphead(struct machine * m);
void pop_function(struct machine * m);
void pop_match(struct machine * m);
void pop_cond(struct machine * m);
void pop_forestcomptail(struct machine * m);
void pop_treecompforest(struct machine * m);
void pop_stack_cond_then(struct machine * m);
void pop_stack_cond_else(struct machine * m);


//Controle de la machine en fonction du terme a reduire
void on_integer(struct machine * m);
void on_binop(struct machine * m);
void on_unaryop(struct machine * m);
void on_var(struct machine * m);
void on_import(struct machine * m);
void on_app(struct machine * m);
void on_word(struct machine * m);
void on_tree(struct machine * m);
void on_forest(struct machine * m);
void on_match(struct machine * m);
void on_cond(struct machine * m);
void on_decl_rec(struct machine * m);
void on_fun(struct machine * m);



void compute(struct machine * m);
#endif
