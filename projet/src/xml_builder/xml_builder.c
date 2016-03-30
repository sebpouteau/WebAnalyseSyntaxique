#include <xml_builder.h>
#include <tree.h>

void build_xml(tree t){
  int fd;
  if((fd = open("god.xml",O_CREAT|O_RDWR)) == -1){
    fprintf(stderr, "failed to open file\n");
    exit(EXIT_FAILURE);
  }
  depth_search(t,0);
}

void attributes_to_xml(attributes a){
  if (a == NULL)
    return;
  
  printf(" ");
  while(a != NULL){
    printf("%s=\"%s\"", attr_get_key(a), attr_get_value(a));
    a = attr_get_next(a);
    if(a != NULL){
      printf(" ");
    }
  }
}

/*
void describe_tree(tree t){
  
}

void describe_word(tree t){
  
}
*/

void print_label(tree t){
  printf("%s", tree_get_label(t));
  if (tree_get_space(t)) {
    printf(" ");
  }
}

void indent_search(int cpt){
  for (int i = 0; i < cpt; i++){
    printf("   ");
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
  } else {
    indent_search(cpt);
    printf("<");
    print_label(t);
    if (tree_get_nullary(t)) {
      printf("/>\n");
    } else {
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



/*
  int main(void){
  attributes a = attr_create("href", "www.google.fr", NULL);
  attributes a1 = attr_create("color", "blue", NULL);
  attributes a2 = attr_create("bold", "false", NULL);
  attr_set_next(a, a1);
  attr_set_next(a1, a2);

  tree t = NULL;
  tree t2 = NULL;
  tree t3 = NULL;
  tree t4 = NULL;
  tree t5 = NULL;
  tree t6 = NULL;
  tree t7 = NULL;
  t7 = tree_create("list", true, false, _word, NULL, NULL, NULL);
  t3 = tree_create("Bonjour", true, false, _word, NULL, NULL, NULL);
  t6 = tree_create("ul", false, false, _tree, NULL, t7, NULL);
  t2 = tree_create("Hello", true, true, _word, NULL, NULL, t6);
  
  t4 = tree_create("a", false, false, _tree, a, t3, NULL);
  t5 = tree_create("div", false, false, _tree, a2, t4, t2);

  t = tree_create("span", false, false, _tree, NULL, t5, NULL);
  
  depth_search(t,0);
  }
*/
