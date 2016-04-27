#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <import.h>

char * from_path_to_name(struct path * chemin){
    fprintf(stderr,"Fonction à implémenter");
    exit(1);
    return "";
}

struct closure * retreive_tree(struct path * chemin,struct files * f){
    char * name = from_path_to_name(chemin);
    struct files * tmp = f;
    while(tmp!=NULL){
        if(!strcmp(name, f->file_name)){
            return tmp->cl;
        }
        else{
            tmp=tmp->next;
        }
    }
    return NULL;
}

struct closure * retrieve_name(struct path * chemin, char * name, struct files * f){
    struct closure * cl = retreive_tree(chemin,f);
    struct env * e = cl->env;
    while(e!=NULL){
        if(!strcmp(name,e->var)){
            return  e->value;
        }
        else{
            e=e->next;
        }
    }
    fprintf(stderr,
            "Variable %s du fichier %s non trouvée",
            name, from_path_to_name(chemin));
    exit(1);
}

struct env * initial_env = NULL;

struct env * process_binding_instruction(char * name, struct ast * a, struct env * e){
    struct machine * m = malloc(sizeof(struct machine));
    m->closure = mk_closure(a,e);
    m->stack=NULL;
    compute(m);
    free(m);
    //should free stack...
    return mk_env(name,m->closure,e);
}
    

void process_instruction(struct ast * a, struct env * e){
    struct machine * m = malloc(sizeof(struct machine));
    m->closure = mk_closure(a,e);
    m->stack=NULL;
    compute(m);
    free(m);
}

struct closure * process_content(struct ast * a, struct env * e){
    struct machine * m = malloc(sizeof(struct machine));
    m->closure = mk_closure(a,e);
    m->stack=NULL;
    compute(m);
    if(m->closure->value->type==TREE || m->closure->value->type==FOREST){
        free(m);
        return m->closure;
    }
    else{
        fprintf(stderr,"Le contenu d'un fichier doit être un arbre ou une forêt");
        exit(1);
    }
}

struct files * add_file(struct path * chemin, struct closure * cl, struct files * f){
    struct files * res = malloc(sizeof(struct files));
    res->file_name = from_path_to_name(chemin);
    res ->cl = cl;
    res->next = f;
    return res;
}
