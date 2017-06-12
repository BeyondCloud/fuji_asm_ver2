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
           MOV    AH, 01H           ; Get user input with echo
           INT    21H
           CMP    AL, '0'           ; Check for invalid input
           JLE    gError
           CMP    AL, '9'
           JG     gError
           MOV    DL, AL
           SUB    AL, 48            ; Convert ASCII to decimal binary
           RET
   gError:
           MOV    DL, BEL           ; Error alert
           CALL   putChr 
           JMP    gLoop
   getDgt  ENDP                     ; End of Subroutine
;
   putChr  PROC                     ; Subroutine to display a character
                                    ; at call register DL carries the
           CMP    DL, 0             ; character to be displayed
           JE     NoMore            ; DL = 00 means end-of-string
           MOV    AH, 02H
           INT    21H
   NoMore: 
           RET
   putChr  ENDP                     ; End of Subroutine
;
   NewLine PROC                     ; Subroutine to start a new line
           MOV    DL, CR
           CALL   putChr
           MOV    DL, LF
           CALL   putChr
           RET
   NewLine ENDP                     ; End of Subroutine
;
   doComp  PROC                     ; Subroutine to compute Sum of 1 .. n
           MOV    AX, 0             ; at call register DL carries n
           MOV    DH, 0
   doLoop: CMP    DX, 0
           JLE    doDone
           ADD    AX, DX
           DEC    DX
           JMP    doLoop
   doDone:                          ; Result is in AX (Two ASCII Chars)
           DB     0D4H              ; Define the AAM instruction
           DB     00AH
           ADD    AX, 3030H         ; Convert decimal binary to ASCII           
           RET
   doComp  ENDP                     ; End of Subroutine
;
   MAIN    PROC
   ;------------------------------------------------------------------------
   ;       PROLOGUE
   ;------------------------------------------------------------------------
   ;
           MOV    AX, @DATA	    ; Set up DATA SEGMENT
           MOV    DS, AX
   ; Show message Start1
           LEA    DX, Start1	    ; Display program start message
           MOV    AH, 09H
           INT    21H





END