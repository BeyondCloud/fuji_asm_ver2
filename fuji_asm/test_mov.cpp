  BEL      EQU     07H
  BS       EQU     08H
  CR       EQU     0DH
  LF       EQU     0AH
   gLoop:  MOV    DL, '?'   ;B2 3F 
           MOV    AH, 01H   ;B4 01  
           MOV    DL, BEL   ;B2 07 
           MOV    AH, 02H   ;B4 02
           MOV    AX, 0     ;B8 00 00 
           MOV    DL, CR    ;B2 0D
           MOV    DL, LF    ;B2 0A
           MOV    DH, 0     ;B6 00
           MOV    AX, @DATA ;B8 00 00  
           MOV    AH, 09H   ;B4 09
           MOV    AH, 09H   ;B4 09
           MOV    AX,4C00H  ;B8 00 4C
           MOV    DL, AL    ;8A D0
           MOV    DL, AL    ;8A D0
           MOV    uIdgt, DL ;88 16 00 00
           MOV    uRval, AX ;A3 00 00
