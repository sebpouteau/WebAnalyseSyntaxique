#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "tree.h"

static void build_xml(tree t){
  int fd;
  if((fd = open("god.xml",O_CREAT|O_RDWR)) == -1){
    fprintf(stderr, "failed to open file\n");
    exit(EXIT_FAILURE);
  }
}

static void attributes_to_xml(attributes a){
  while(a != NULL){
    printf(" %s=\"%s\"", get_key(a), get_value(a));
    a = get_next(a);
    if(a != NULL){
      printf(" ");
    }
  }
}


static void describe_tree(tree t){
  
}

static void describe_word(tree t){
  
}


static void print_label(tree t){
  printf("%s", get_label(t));
  if (get_space(t)) {
    printf(" ");
  }
}

static void depth_search(tree t){
  if(t == NULL)
    return;
  
  if(get_tp(t) == _word) {
    print_label(t);
  } else {
    printf("<");
    print_label(t);
    if (get_nullary(t)) {
      printf("/>");
    } else {
      attributes_to_xml(get_attributes(t));
      printf(">");
      depth_search(get_daughters(t));
      printf("</%s>", get_label(t));
    }
  }
  if(get_right(t) != NULL){
    depth_search(get_right(t));
  }
}




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
  t3 = create_tree("Bonjour", true, false, _word, NULL, NULL, NULL);
  t4 = create_tree("a", false, false, _tree, NULL, t3, NULL);
  t2 = create_tree("Hello", true, true, _word, NULL, NULL, t4);
  t = create_tree("div", false, false, _tree, NULL, t2, NULL);


  depth_search(t);
  /*
  tree t = NULL;
  get_label(t);
  tree t2 = create_empty_tree();
  char* label = "coucou";
  set_label(t2, label);
  label = "enfait non";
  char* test = get_label(t2);
  if(test != NULL)
    printf("%s\n", test);
  printf("%s\n", test);//CAUSE A SEGFAULT SI test=NULL a cause du %s printf
  */ 
}

