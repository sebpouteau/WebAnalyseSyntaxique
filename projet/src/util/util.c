#include "../../include/util.h"
#include <stdio.h>
/* ===============================
      strucuture pour les TEXT
   =============================== */

struct text_t{
  char* text;
  bool space;
};

text create_text(char* text, bool space){
  text* t = malloc(sizeof(struct text_t));
  t->text = text;
  t->space = space;
  return t;
}

