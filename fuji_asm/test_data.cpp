.MODEL   SMALL
  BEL      EQU     07H  ;no
  BS       EQU     08H	;no
  CR       EQU     0DH	;no 
  LF       EQU     0AH  ;no
    
.STACK  200H	;this will change alot
.DATA	;no
   Start1  DB   "%% Hello, World! Execution starts....", BEL, CR, LF, '$'
   size1   DW   $-start1  ;29 00 ,29h = 41 = sizeof start1 string 
   Goodbye DB   "%% Program stops. Exit now!",           BEL, CR, LF, '$'
   size2   DW   $-Goodbye

    uMSG   DB   CR, LF, "   Sum(1 .. "
    uIdgt  DB  1
           DB   ") = "
    uRval  DW   1
           DB   CR, LF, '$'
.CODE	;no
   getDgt  PROC                     ; Subroutine to get a decimal digit
                                    ; and convert to binary
   gLoop:  MOV    DL, '?'
           CALL   putChr
           INT    21H
           CMP    AL, '0'           ; Check for invalid input
           CMP    AL, '9'
          
           MOV    AH, 01H           ; Get user input with echo
           MOV    DL, AL
   gError:
           MOV    DL, BEL           ; Error alert
   getDgt  ENDP                     ; End of Subroutine
END