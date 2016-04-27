#ifndef IMPORT_FILE
#define IMPORT_FILE
#include "machine.h"

struct files {
    char * file_name;
    struct closure * cl;
    struct files * next;
};


//fonction qui renvoie le chemin absolu dans la hiérarchie des répertoires
//d'un fichier avec sa désignation à partir de la structure chemin
char * from_path_to_name(struct path * chemin);

struct closure * retreive_tree(struct path * chemin,struct files * f);

struct closure * retrieve_name(struct path * chemin, char * name, struct files * f);

struct env * initial_env;

struct env * process_binding_instruction(char * name, struct ast * a, struct env * e);

void  process_instruction(struct ast * a, struct env * e);

struct closure * process_content(struct ast * a, struct env * e);

struct files * add_file(struct path * chemin, struct closure * cl, struct files * f);

#endif
