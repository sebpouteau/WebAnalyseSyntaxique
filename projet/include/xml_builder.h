/**
 *  \file xml_builder.h
 *  \brief Fichier regrouppant l'ensemble des fonctions utilisées pour générer le code HTML/XML.
 *  \author POUTEAU Sébastien <br>
 *  PALARD Nicolas <br>
 *  GOURAUD Jimmy <br>
 *  KIROV Yordan <br>
 *  ROBERT Etienne
 */
#ifndef XML_BUILDER_H
#define XML_BUILDER_H

#include <ast.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


/**
 *  \brief Cette fonction est le point de départ de la construction de la page web HTML <br>
 *  Elle créer le fichier html et appel xml_ast qui s'occupe de généré le contenu
 *  \param char* file Le nom du fichier html
 *  \param struct ast* a La structure contenant toutes les données de la page a générer
 *  \return void
 */
void build_xml(char* file, struct ast* a);

/**
 *  \brief Affiche des espaces pour indenter le fichier
 *  \param FILE* fd le descripteur de fichier fournis par build_xml
 *  \param int depth_indent Le niveau d'indentation à appliquer
 *  \return void
 */
void xml_indent(FILE * fd, int depth_indent);

/**
 *  \brief Affiche la(es) donnée(s) voulu en fonction du type de struct ast* a
 *  \param FILE* fd Le descriteur de fichier où écrire
 *  \param int depth_indent Le niveau d'indentation à appliquer
 *  \return void
 */

void xml_ast (FILE * fd, struct ast* a, int depth_indent);

/**
 *  \brief Fonction récursive qui génère le code XML des attribues d'une balise
 *  \param FILE* fd Le descripteur de fichier où écrire
 *  \param struct attributes* a La liste chaînée des attribues a parcourir et convertir.
 *  \param int depth_indent Le niveau d'indentation à appliquer
 *  \return void
 */
void xml_attributes(FILE* fd, struct attributes* a, int depth_indent);

/**
 *  \brief Fonction  qui génère le code HTML/XML des arbres/balises (Elle appelle donc xml_attributes)
 *  \param FILE* fd Le descripteur de fichier où écrire
 *  \param struct tree* t L'arbre à convertir
 *  \param int depth_indent Le niveau d'indentation à appliquer
 *  \return void
 */
void xml_tree(FILE* fd, struct tree* t, int depth_indent);

/**
 *  \brief Fonction qui converti une forêt en code HTML/XML
 *  \param FILE* fd Le descripteur de fichier où écrire
 *  \param struct tree* t La foret à convertir
 *  \param int depth_indent Le niveau d'indentation à appliquer
 *  \return void
 */
void xml_forest(FILE* fd, struct forest* f, int depth_indent);


#endif
