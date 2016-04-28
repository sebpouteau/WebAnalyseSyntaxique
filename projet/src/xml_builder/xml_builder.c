#include <xml_builder.h>


void xml_indent(int depth_indent){
  for (int i = 0; i < cpt; i++){
    fprintf(fd, "   ");
  }
}


void xml_ast (FILE * fd, struct ast* a, int depth_indent){
  if (a == NULL){
    return;
  }
    
  switch(a->type){
    case INTEGER:
      fprintf(fd,  " %d ", a->node->num);
      break;
    case BINOP:
      break;
    case UNARYOP:
      break;
    case VAR:      
      break;
    case WORD:
      fprintf(fd, " %s ", a->node->str);
      break;
    case IMPORT:
      break;
    case APP:
      break;
    case TREE:
      xml_tree(fd, a->node->tree, cpt);
      break;
    case FOREST:
      xml_forest(fd, a->node->forest, cpt);
      break;
    case FUN:
      break;
    case MATCH:
      break;
    case COND:
      break;
    case DECLREC:
      break;
    default:
      break;
  }
}


void xml_attributes(FILE* fd, struct attributes* a){  
  if (a == NULL){
    return;
  }

  fprintf(fd, " ");
  xml_ast(fd, a->key);
  fprintf(fd, "=\"");
  xml_ast(fd, a->value);
  fprintf(fd, "\"");

  xml_attributes(fd, a->next);
}


void xml_tree(FILE* fd, struct tree* t, int depth_indent){
  if (t == NULL){
    return;
  }

  if (t->nullary){
    fprintf(fd, "<%s/>", t->label);
    return;
  }
  
  xml_indent(cpt);
  fprintf(fd, "<%s", t->label);
  xml_attributes(fd, t->attributes);
  fprintf(fd, ">\n");
  
  xml_ast(fd, t->daughters, cpt+1);
  xml_indent(cpt);
  fprintf(fd, "</%s>\n", t->label);
}


void xml_forest(FILE* fd, struct forest* f, int depth_indent){
  if (f == NULL){
    return;
  }
    
  xml_ast(fd, f->head, cpt);
  
  xml_ast(fd, f->tail, cpt);
}

void build_xml(struct ast* a){
  FILE *fd = fopen("fichier.xml", "w+");
  
  if (fd == NULL) {
    fprintf(stderr, "\"fichier.xml\": erreur ouverture fichier.");
    exit(EXIT_FAILURE);
  }
  
  fprintf(fd, "<!DOCTYPE html>");
  xml_ast(fd, a, 0);
  
  fclose(fd);
}



// =================================================


void print_label(tree t){
  printf("%s", tree_get_label(t));
  if (tree_get_space(t)) {
    printf(" ");
  }
}

bool is_word(tree t){
  if (t == NULL || tree_get_tp(t) != _word)
    return false;
  return true;
}

void depth_search(tree t, int cpt){
  if(t == NULL)
    return;
  
  if(tree_get_tp(t) == _word) {
    print_label(t);
    if (!is_word(tree_get_right(t)))
      printf("\n");
  }
  else {
    indent_search(cpt);
    printf("<");
    print_label(t);
    if (tree_get_nullary(t))
    {
      printf("/>\n");
    }
    else
    {
      attributes_to_xml(tree_get_attributes(t));
      printf(">\n");
      if (is_word(tree_get_daughters(t)))
        indent_search(cpt+1);
      depth_search(tree_get_daughters(t), cpt+1);
      indent_search(cpt);
      
      printf("</%s>\n", tree_get_label(t));
    }
    if (is_word(tree_get_right(t)))
      indent_search(cpt);
  }
  depth_search(tree_get_right(t), cpt);
}
