.MODEL   SMALL
  BEL      EQU     07H  ;no
  BS       EQU     08H	;no
  CR       EQU     0DH	;no 
  LF       EQU     0AH  ;no
.STACK  200H	;this will change alot
.DATA	;no
    uRval  DW   1
           DB   CR, LF, '$'  
.CODE	;no

END