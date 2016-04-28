#ifndef XML_BUILDER_H
#define XML_BUILDER_H

#include <tree.h>
#include <string.h>

void build_xml(struct ast* a);

void xml_indent(int depth_indent);

void xml_ast (FILE * fd, struct ast* a, int depth_indent);

void xml_attributes(FILE* fd, struct attributes* a);

void xml_tree(FILE* fd, struct tree* t, int depth_indent);

void xml_forest(FILE* fd, struct forest* f, int depth_indent);


#endif
