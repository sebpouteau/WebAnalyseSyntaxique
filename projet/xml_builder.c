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
    printf("%s=\"%s\"", get_key(a), get_value(a));
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

static void depth_search(tree t){
  if(t != NULL){
    if(get_tp(t) == _word){
      printf("%s ", get_label(t));
    } else {
      printf("<%s ", get_label(t));
      attributes_to_xml(get_attributes(t));
      printf(">\n");
   
    }
       if(get_daughters(t) != NULL){
        depth_search(get_daughters(t));
        printf("</%s>", get_label(t));
      } else if(get_right(t) != NULL){
        depth_search(get_right(t));
      } else
        return;
  }    
}

static void tree_to_xml(tree t){
  while(t != NULL){
    if(get_label(t) != NULL){
      if(get_tp(t) == _word){ //Si c'est pour def un sous arbre alors
        printf("%s", get_label(t)); //On le balise
        if(get_attributes(t) != NULL){
          if(get_space(t)){
            printf(" ");
          }
          attributes_to_xml(get_attributes(t));
          printf(">");
        }
      } else { //Sinon
        printf("<%s", get_label(t)); //C'est de type word donc du text
      }
      if(get_right(t) != NULL){
        t = get_right(t);
      }
      else {
        t = get_daughters(t);
      }
    }
  }
}

int main(void){
  attributes a = create_attributes("href", "www.google.fr", NULL);
  attributes a1 = create_attributes("color", "blue", NULL);
  attributes a2 = create_attributes("bold", "false", NULL);
  set_next(a, a1);
  set_next(a1, a2);
  //attributes_to_xml(a);

  tree t4 = create_tree("a", true, false, _tree, a, NULL, NULL);
  tree t3 = create_tree("Bonjour", true, false, _word, NULL, NULL, NULL);
  tree t2 = create_tree("Hello", true, false, _word, NULL, t4, t3);
  tree t = create_tree("div", false, true, _tree, a, t2, NULL);

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

