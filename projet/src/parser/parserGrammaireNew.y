
%token LABEL TEXT LET NB EMIT WHERE FUN ARROW IF THEN ELSE MATCH WITH END_MATCH COND BINARY REC EOL IN XML FILTER FILTER_SPACE


                        
%%

start:          start block
        |       
        ;

block:          NB
        |       tag container
        |       LABEL '/'
        |       LABEL '['  assign ']' '/'
        |       LABEL '[' assign  ']' separator container
        |       LET declaration ';'
        |       expression
        |       local_affect
        |       MATCH block WITH extended_filter  END_MATCH
                ;


local_affect:   LET declaration IN block 
        |       block WHERE declaration
        ;

tag:            LABEL
        |       /* empty */
        ;

expression:     '(' expression ')'
        |       IF block THEN block ELSE block
        |       block COND block
        ;


container:     '{' extended_content '}'
        |      '{'extended_content args '}'
                ;


extended_content:
                extended_content content
        |       /* empty */
        ;


content:        TEXT
        |       block
        |       args ','
        ;


assign:         LABEL '=' TEXT assign
        |       LABEL '=' TEXT
                ;


declaration:    rec args '=' block 
        |       rec args '=' FUN args ARROW block 
        ;

rec:            REC
        |       /* empty */
        ;

args:           args LABEL
        |       LABEL
        ;



extended_filter:
                extended_filter separator '|' filter ARROW block
        |       separator '|' filter ARROW block
                ;


filter:         filter filter_contenu separator
        |       filter_contenu
        ;


tag2:           FILTER
        |       LABEL
        ;

entete:
                tag2 
        |       /* empty */
        ;


filter_contenu: 
                '*' tag2 '*'
        |       '/' tag2 '/'
        |       filter_options
        ;

filter_options:
                entete '{' filter '}'
        |       FILTER_SPACE 
                ;


separator:
                separator ' '
        |       separator EOL
        |       separator '\t'
        |       /* empty */
                ;


%%
              
