#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "machine.h"
#include "pattern_matching.h"


void free_match_created_closure(struct closure * value){
    if(value!=NULL){
        free_match_created_forest(value->value);
        free_match_created_env(value->env);
        free(value);
    }
}

void free_match_created_env(struct env * e){
    if(e!=NULL){
        //on ne libÃ¨re pas le champ var, car le pointeur peut Ãªtre utilisÃ© ailleurs
        free_match_created_closure(e->value);
        free_match_created_env(e->next);
        free(e);
    }
}

struct env * concat_env(struct env * e1, struct env * e2){
    if(e1==NULL){return e2;}
    struct env * e =e1;
    while(e->next !=NULL){
        e=e->next;
    }
    e->next=e2;
    return e1;
}


struct machine * mk_machine(struct closure * cl, struct stack * stack){
    struct machine * res = malloc(sizeof(struct machine));
    res->closure = cl;
    res->stack = stack;
    return res;
}

int is_compatible(struct ast * ast, enum pvar tp){
    return
        ast &&
        ast->type==FOREST && (
            (tp==STRINGVAR && ast->node->forest->head->type==WORD) ||
            (tp==FORESTVAR && ast->node->forest->head->type==TREE) ||
            tp==ANYVAR
            )        
        ;
}

struct ast * clone_forest(struct ast ** start, enum pvar tp){
    if(is_compatible(*start,tp)){
        struct ast * head = (*start)->node->forest->head;
        *start = (*start)->node->forest->tail;
        return mk_forest(true, head,
                      clone_forest(start,tp));
    }
    else{return NULL;}
}

void free_match_created_forest(struct ast * start){
    if(start!=NULL){
        if(start->type==FOREST){
            free_match_created_forest(start->node->forest->tail);
            free(start->node->forest);
            free(start->node);
            free(start);
        }
    }
    return;
}


bool match_wildcard(enum wildcard wd, struct ast ** ast){
    if(*ast == NULL){
        return wd!=ANY;
    }
    switch(wd){
    case ANY:
        if((*ast)->type==TREE){*ast = NULL; return true;}
        else if((*ast)->type==FOREST && (*ast)->node->forest->head->type==TREE){
            *ast = (*ast)->node->forest->tail;
            return true;
        }
        else{return false;}
    case ANYSTRING:
        if((*ast)->type==FOREST){
            while(*ast && (*ast)->node->forest->head->type==WORD){
                *ast = (*ast)->node->forest->tail;
            }
            return true;
        }
        break;
    case ANYFOREST:
        if((*ast)->type==FOREST){
            while(*ast && (*ast)->node->forest->head->type==TREE){
                *ast = (*ast)->node->forest->tail;
            }
            return true;
        }
        break;
    case ANYSEQ:
        if((*ast)->type==FOREST){
            *ast = NULL;
            return true;
        }
        break;
    default:
        return false;
    }
    return false;
}

bool match_tree (struct ptree * ptree, struct tree * tree, struct env ** e){
    return
        !strcmp(ptree->label,tree->label) &&
        ptree->nullary == tree->nullary &&
        (ptree->nullary ||
         match_aux(ptree->daughters,tree->daughters,e)
            );
}

bool match_string(struct pstring * pstring, struct ast * word){
    assert(word->type==WORD);
    return !strcmp(pstring->string,word->node->str);
}


bool match_var(struct pattern_var * pat, struct ast ** ast, struct env ** e){
    switch(pat->pvar){
            case TREEVAR:
                if((*ast)->type==TREE){
                    push_env(pat->id, mk_closure((*ast),NULL),e);
                    *ast = NULL;
                    return true;
                    // ici on met un environement null parce que
                    // on ne filtre que sur des valeurs.
                }
                else if((*ast)->type==FOREST &&
                        (*ast)->node->forest->head->type==TREE){
                    push_env(pat->id, mk_closure((*ast)->node->forest->head,NULL),e);
                    *ast = (*ast)->node->forest->tail;
                    return true;
                }
                else{return false;}
                break;
            case STRINGVAR:
                if((*ast)->type==FOREST){
                    push_env(pat->id,
                             mk_closure(clone_forest(ast,STRINGVAR),NULL),
                             e);
                    return true;
                }
                else{return false;}
                break;
            case FORESTVAR:
                if((*ast)->type==FOREST){
                    push_env(pat->id,
                             mk_closure(clone_forest(ast,FORESTVAR),NULL),
                             e);
                    return true;
                }
                else{return false;}
                break;
            case ANYVAR:
                if((*ast)->type==FOREST){
                    push_env(pat->id,
                             mk_closure(clone_forest(ast,ANYVAR),NULL),
                             e);
                    return true;
                }
                else{return false;}
                break;
            default:
                break;
            }
    return false;
}

bool match_forest(struct pforest *  pforest, struct ast ** ast, struct env ** e){
    assert(ast !=NULL);
    if(pforest==NULL || *ast==NULL){
        return pforest==NULL && *ast==NULL;
    }
    else{
        assert(pforest->head);
        switch(pforest->head->ptype){
        case WILDCARD:
            return match_wildcard(pforest->head->pnode->wildcard,ast) &&
                match_ind(pforest->tail,ast,e);
        case PTREE:
            if((*ast)->type==FOREST &&
               (*ast)->node->forest->head->type==TREE &&
               match_tree(pforest->head->pnode->ptree,
                          (*ast)->node->forest->head->node->tree,
                          e)){
                *ast = (*ast)->node->forest->tail;
                return match_ind(pforest->tail,ast,e);}
            else{
                return false;
            }
        case PSTRING:
            if((*ast)->type==FOREST &&
                (*ast)->node->forest->head->type==WORD &&
                ! strcmp(pforest->head->pnode->pstring->string,
                         (*ast)->node->forest->head->node->str)){
                *ast = (*ast)->node->forest->tail;
                return match_ind(pforest->tail,ast,e);}
            else{return false;}
        case PVAR:
            return match_var(pforest->head->pnode->pvar,ast,e) &&
                match_ind(pforest->tail, ast,e);
                
        case PFOREST:
            return match_forest(pforest->head->pnode->pforest,ast,e) &&
                match_ind(pforest->tail,ast,e);
          case ANYTREE:
            if((*ast)->type==FOREST &&
               (*ast)->node->forest->head->type==TREE &&
               match_anytree(pforest->head->pnode->anytree,
                          (*ast)->node->forest->head->node->tree,
                          e)){
                *ast = (*ast)->node->forest->tail;
                return match_ind(pforest->tail,ast,e);}
            else{
                return false;
            }
            ;
          default:
            return false;
        }
    }
}


bool match_anytree(struct anytree * anytree, struct tree * tree, struct env ** e){
    return
        anytree->nullary == tree->nullary &&
        (anytree->nullary ||
         match_aux(anytree->daughters,tree->daughters,e)
            );
}

bool match_null(struct pattern * pat, struct env ** e){
    if(pat==NULL){return true;}
    else{
        switch(pat->ptype){
        case PTREE:
        case ANYTREE:
        case PSTRING:
            return false;
        case PFOREST:
            return match_null(pat->pnode->pforest->head,e) &&
                match_null(pat->pnode->pforest->tail,e);
        case PVAR:
            switch(pat->pnode->pvar->pvar){
            case TREEVAR:
                return false;
            case STRINGVAR:
            case FORESTVAR:
            case ANYVAR:
                push_env(pat->pnode->pvar->id,
                         mk_closure(NULL,NULL),
                         e);
                return true;
            }
        case WILDCARD:
            switch(pat->pnode->wildcard){
            case ANY:
                return false;
            case ANYSTRING:
            case ANYFOREST:
            case ANYSEQ:
                return true;
                
            }
        }
    }
    return false;
    
}

bool match_ind(struct pattern * pat, struct ast ** ast, struct env ** e){
    if(pat==NULL){
        return pat==NULL && *ast==NULL;
    }
    else{
        if(*ast==NULL){
            return match_null(pat,e);
        }
        if ((*ast)->type!=TREE && (*ast)->type!=FOREST && (*ast)->type!=WORD) {
            printf("Matching a something that is not a tree or a forest!\n");
            return 0;
        }
        switch (pat->ptype) {
        case WILDCARD:
            return match_wildcard(pat->pnode->wildcard, ast);
        case PTREE:
            if((*ast)->type==TREE &&
                match_tree(pat->pnode->ptree,
                           (*ast)->node->tree,
                           e))
            {*ast = NULL; return true;}
            break;
        case PSTRING:
            if( match_string(pat->pnode->pstring,*ast))
            {*ast = NULL; return true;}
            break;
        case PVAR:
            return match_var(pat->pnode->pvar, ast,e);
            break;
        case PFOREST:
            return match_forest(pat->pnode->pforest, ast,e);
        case ANYTREE:
            if(  (*ast)->type==TREE &&
                match_anytree(pat->pnode->anytree,
                              (*ast)->node->tree,
                              e))
            {*ast = NULL; return true;}
            break;
        }
    }
    return false;
}

bool match_aux(struct pattern * pat, struct ast * ast, struct env ** e){
    struct ast * tmp = ast;
    return match_ind(pat, &tmp,e) && tmp==NULL;
}


bool match(struct patterns * patterns, struct ast * ast, struct ast ** res, struct env ** e){
    if(patterns==NULL){return false;}
    else{
        struct env * res_env = NULL;
        if(match_aux(patterns->pattern, ast,&res_env)){
            concat_env(res_env, *e);
            *e=res_env;
            *res = patterns->res;
            return true;
        }
        else{
            free_match_created_env(res_env);
            return match(patterns->next,ast,res,e);
        }
    }
    return false;
}
