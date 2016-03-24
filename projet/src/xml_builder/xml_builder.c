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
    printf("%s=\"%s\"", get_key(a), get_value(a));
    a = get_next(a);
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
  printf("%s", get_label(t));
  if (get_space(t)) {
    printf(" ");
  }
}

void indent_search(int cpt){
  for (int i = 0; i < cpt; i++){
    printf("   ");
  }
}

bool is_word(tree t){
  if (t == NULL || get_tp(t) != _word)
    return false;
  return true;
}

void depth_search(tree t, int cpt){
  if(t == NULL)
    return;
  
  if(get_tp(t) == _word) {
    print_label(t);
    if (!is_word(get_right(t)))
      printf("\n");
  } else {
    indent_search(cpt);
    printf("<");
    print_label(t);
    if (get_nullary(t)) {
      printf("/>\n");
    } else {
      attributes_to_xml(get_attributes(t));
      printf(">\n");
      if (is_word(get_daughters(t)))
        indent_search(cpt+1);
      depth_search(get_daughters(t), cpt+1);
      indent_search(cpt);
      printf("</%s>\n", get_label(t));
    }
    if (is_word(get_right(t)))
      indent_search(cpt);
  }
  depth_search(get_right(t), cpt);
}



/*
  int main(void){
  attributes a = create_attributes("href", "www.google.fr", NULL);
  attributes a1 = create_attributes("color", "blue", NULL);
  attributes a2 = create_attributes("bold", "false", NULL);
  set_next(a, a1);
  set_next(a1, a2);

  tree t = NULL;
  tree t2 = NULL;
  tree t3 = NULL;
  tree t4 = NULL;
  tree t5 = NULL;
  tree t6 = NULL;
  tree t7 = NULL;
  t7 = create_tree("list", true, false, _word, NULL, NULL, NULL);
  t3 = create_tree("Bonjour", true, false, _word, NULL, NULL, NULL);
  t6 = create_tree("ul", false, false, _tree, NULL, t7, NULL);
  t2 = create_tree("Hello", true, true, _word, NULL, NULL, t6);
  
  t4 = create_tree("a", false, false, _tree, a, t3, NULL);
  t5 = create_tree("div", false, false, _tree, a2, t4, t2);

  t = create_tree("span", false, false, _tree, NULL, t5, NULL);
  
  depth_search(t,0);
  }
*/
