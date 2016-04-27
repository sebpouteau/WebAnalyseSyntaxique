#ifndef PATTERN_MATCHING
#define PATTERN_MATCHING
#include <stdlib.h>
#include "ast.h"
#include "pattern.h"
#include "machine.h"

void free_match_created_closure(struct closure * value);
void free_match_created_env(struct env * e);
int is_compatible(struct ast * ast, enum pvar tp);
struct ast * clone_forest(struct ast ** start, enum pvar tp);
void free_match_created_forest(struct ast * start);
struct env * concat_env(struct env * e1, struct env * e2);

// Fonction de filtrage
bool match_wildcard(enum wildcard wd, struct ast ** ast);
bool match_tree (struct ptree * ptree, struct tree * tree, struct env ** e);
bool match_string(struct pstring * pstring, struct ast * word);
bool match_var(struct pattern_var * pat, struct ast ** ast, struct env ** e);
bool match_forest(struct pforest *  pforest, struct ast ** ast, struct env ** e);
bool match_anytree(struct anytree * anytree, struct tree * tree, struct env ** e);
bool match_null(struct pattern * pat, struct env ** e);
bool match_ind(struct pattern * pat, struct ast ** ast, struct env ** e);

bool match_aux(struct pattern * pat, struct ast * ast, struct env ** e);

bool match(struct patterns * patterns, struct ast * ast, struct ast ** res, struct env ** e);

#endif
