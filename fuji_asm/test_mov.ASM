.MODEL   SMALL
  BEL      EQU     07H 
  BS       EQU     08H          
  CR       EQU     0DH            
  LF       EQU     0AH  
.DATA                              
   Start1  DB   "%% Hello, World! Execution starts....", BEL, CR, LF, '$'
   size1   DW   $-start1
   Goodbye DB   "%% Program stops. Exit now!",           BEL, CR, LF, '$'
   size2   DW   $-Goodbye

    uMSG   DB   CR, LF, "   Sum(1 .. "
    uIdgt  DB  1
           DB   ") = "
    uRval  DW   1
           DB   CR, LF, '$'           
.CODE
   gLoop:  MOV    DL, '!'   ;B2 3F 
           MOV    AH, 01H   ;B4 01  
           MOV    DL, AL    ;8A D0
           MOV    DL, BEL   ;B2 07 
           MOV    AH, 02H   ;B4 02
           MOV    DL, CR    ;B2 0D
           MOV    DL, LF    ;B2 0A
           MOV    AX, 0     ;B8 00 00 
           MOV    DH, 0     ;B6 00
           MOV    AX, @DATA ;B8 00 00  
           MOV    AH, 09H   ;B4 09
           MOV    uIdgt, DL ;88 16 00 00
           MOV    DL, AL    ;8A D0
           MOV    uRval, AX ;A3 00 00
           MOV    AH, 09H   ;B4 09
           MOV    AX,4C00H  ;B8 00 4C
END