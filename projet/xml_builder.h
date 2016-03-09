#ifndef XML_BUILDER_H
#define XML_BUILDER_H

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "tree.h"

void build_xml(tree t);

void attributes_to_xml(attributes a);

void describe_tree(tree t);

void describe_word(tree t);

void print_label(tree t);

/**
 *  Gére l'indentation du fichier selon le "depth" passé en paramètre
 *  depth = correspond à la profondeur de l'indentation
 */
void indent_search(int depth);

/**
 * Affiche le xml en fonction du tree passé en paramètre
 * tree = arbre que l'on souhaite
 */
void depth_search(tree t, int depth);

#endif
