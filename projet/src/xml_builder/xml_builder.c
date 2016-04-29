#include <xml_builder.h>


void xml_indent(FILE* fd, int depth_indent){
  for (int i = 0; i < depth_indent; i++){
    fprintf(fd, "   ");
  }
}


void xml_ast (FILE* fd, struct ast* a, int depth_indent){
  if (a == NULL){
    return;
  }
    
  switch(a->type){
    case INTEGER:
      fprintf(fd,  "%d", a->node->num);
      break;
    case WORD:
      fprintf(fd, "%s", a->node->str);
      break;
    case TREE:
      xml_tree(fd, a->node->tree, depth_indent);
      break;
    case FOREST:
      xml_forest(fd, a->node->forest, depth_indent);
      break;
    case BINOP:
    case UNARYOP:
    case VAR:      
    case IMPORT:
    case APP:
    case FUN:
    case MATCH:
    case COND:
    case DECLREC:
    default:
      break;
  }
}


void xml_attributes(FILE* fd, struct attributes* a, int depth_indent){  
  if (a == NULL){
    return;
  }

  fprintf(fd, " ");
  xml_ast(fd, a->key, depth_indent);
  fprintf(fd, "=\"");
  xml_ast(fd, a->value, depth_indent);
  fprintf(fd, "\"");

  xml_attributes(fd, a->next, depth_indent);
}


void xml_tree(FILE* fd, struct tree* t, int depth_indent){
  if (t == NULL){
    return;
  }

  if (t->nullary){
    fprintf(fd, "<%s/>", t->label);
    return;
  }
  
  if (strcmp(t->label, "word") == 0){
    xml_ast(fd, t->daughters, depth_indent+1);
    if (t->space){
      fprintf(fd, " ",NULL);
    }
    return;
  }
  
  xml_indent(fd, depth_indent);
  fprintf(fd, "<%s", t->label);
  xml_attributes(fd, t->attributes, depth_indent);
  fprintf(fd, ">\n");

  xml_ast(fd, t->daughters, depth_indent+1);
  
  fprintf(fd, "\n", NULL);
  xml_indent(fd, depth_indent);
  fprintf(fd, "</%s>\n", t->label);
}


void xml_forest(FILE* fd, struct forest* f, int depth_indent){
  if (f == NULL){
    return;
  }
    
  xml_ast(fd, f->head, depth_indent);
  
  xml_ast(fd, f->tail, depth_indent);
}

void build_xml(char * file,struct ast* a){
  FILE *fd = fopen(file, "w+");
  
  if (fd == NULL) {
    fprintf(stderr, "\"%s\": erreur ouverture fichier.", file);
    exit(EXIT_FAILURE);
  }
  
  fprintf(fd, "<!DOCTYPE html>\n");
  xml_ast(fd, a, 0);
  
  fclose(fd);
}
