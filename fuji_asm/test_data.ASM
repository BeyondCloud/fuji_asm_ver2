.MODEL   SMALL
  BEL      EQU     07H  ;no
  BS       EQU     08H	;no
  CR       EQU     0DH	;no 
  LF       EQU     0AH  ;no
.STACK  200H	;this will change alot
.DATA	;no		;DB+1 DW+2  
    uRval  DB   1			;01  ,DB2 = 02
           DB   CR, LF, '$' ;0D 0A 24
 ;          DB   CR, LF, '$' ;0D 0A 24	;1BF7 ;2FD->765
    uLval  DW   1			;01 00







.CODE	;no

END