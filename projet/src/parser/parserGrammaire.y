%{
%}

%%

start:          start node 
        |       start EMIT TEXT node 
        |       start local_affect 
        |       start error 
        |       /* empty */
                ;


expression:     node
        |       NB COMPARE NB
        |       NB
        |       expression COMPBINAIRE expression
        ;


cond:           IF cond THEN cond ELSE cond
        |       expression
        ;

rec:            REC
        |       /* empty */
        ;

node:           LABEL '/'
        |       tag container
        |       attributes
        |       LET rec  affectation END_LET
        ;


container:     '{' content '}' 
        |      '{' content declaration '}'
                ;

content:        content TEXT 
        |       content attributes 
        |       content LABEL '/' 
        |       content tag container 
        |       content declaration VIRGULE 
        |       /* empty */
                ;

tag:         LABEL
        |       /* empty */
        ;


attributes:     LABEL '[' assign ']' '/'
        |       LABEL '[' assign ']' separator container
                ;

separator:
                separator ' '
        |       separator EOL
        |       separator '\t'
        |       /* empty */
                ;



assign:         LABEL '=' TEXT assign
        |       LABEL '=' TEXT
                ;



local_affect:   LET rec affectation IN node
        |       node WHERE rec affectation
                ;


affectation:	LABEL declaration AFFECT in_affectation
                ;

in_affectation: node
        |       FUN declaration '-' '>' node
                ;

declaration:    delcaration LABEL
        |       LABEL
        ;








%%
