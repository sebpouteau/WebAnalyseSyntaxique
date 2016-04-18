#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 10000


char* convert_to_html(char* string){
  char * base = "&#x";
  char final_string[MAX_SIZE];
  for(int i = 0 ; i < strlen(string) ; i++){
    if((65 < string[i] && string[i] < 90) || (97 < string[i] && string[i] < 122)){ //Char entre a-z ou A-Z
      char *t = malloc(2*sizeof(char));
      t[0] = string[i];
      t[1] = '\0';
      strcat(final_string, t);
      free(t);
    }
    else if(string[i] == 32){
      
    }
    else {
      char *buffer = malloc(10 * sizeof(char));
      sprintf(buffer, "%s%d;", base, string[i]);
      strcat(final_string, buffer);
      free(buffer);
    }
  }
  return strdup(final_string);
}



char* convert_to_html2(char* string){
  char * base = "&#x";
  char final_string[MAX_SIZE];
  int j=0;
  for(int i = 0 ; i < strlen(string) ; i++){
    if((65 < string[i] && string[i] < 90) || (97 < string[i] && string[i] < 122)){ //Char entre a-z ou A-Z
      final_string[j] = string[i];
      j++;
    }
    else if(string[i] != 32){
      sprintf(final_string+j, "%s%d;", base, string[i]);
      while(final_string[j]!='\0')
        j++;
    }
  }
  final_string[j]='\0';
  return strdup(final_string);
}

int main(){
  char* string = "coucou c'est moi # & < >";
  char * s = convert_to_html2(string);
  printf("%s\n", s);
  free(s);
}
