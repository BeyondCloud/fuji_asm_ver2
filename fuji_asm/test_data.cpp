.MODEL   SMALL
;           IDENT   uTV01A           ; This directive is not defined in MASM
                                    ; IDENT directive means IDENTification
                                    ; It is used to declare the program name
  BEL      EQU     07H              ; ASCII Code of BEL (Alarming bell)
  BS       EQU     08H              ; ASCII Code of BS  (Backspace)
  CR       EQU     0DH              ; ASCII Code of CR  (Carriage Return)
  LF       EQU     0AH              ; ASCII Code of LF  (Line Feed)

.STACK  200H

.DATA                               ; The Data segment
   Start1  DB   "%% Hello, World! Execution starts....", BEL, CR, LF, '$'
   size1   DW   $-start1  ;29 00 ,29h = 41 = sizeof start1 string 
   Goodbye DB   "%% Program stops. Exit now!",           BEL, CR, LF, '$'
   size2   DW   $-Goodbye

    uMSG   DB   CR, LF, "   Sum(1 .. "
;   uIdgt  BSS  1                   ; 1-Byte buffer to store input value
    uIdgt  DB  1
           DB   ") = "
    uRval  DW   1
;   uRval  BSS  2                   ; 2-Byte buffer to store result
           DB   CR, LF, '$'
.CODE

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
;
   MAIN    PROC

           MOV    AX, @DATA     ; Set up DATA SEGMENT
 
           MOV    DS, AX


   ; Show message Start1
           LEA    DX, Start1      ; Display program start message
           MOV    AH, 09H

           
           INT    21H
   ;
   mStart: CALL   NewLine
           Call   getDgt
           MOV    uIdgt, DL

           MOV    DL, AL
           
           CALL   doComp
           XCHG   AH, AL
           MOV    uRval, AX

           LEA    DX, uMSG      ; Display result message
           MOV    AH, 09H
           INT    21H
   mEnd:
           CALL   NewLine

   ;       Program exit
           LEA    DX, Goodbye       ; Display goodbye message
           MOV    AH, 09H

           INT    21H
   ;
           MOV    AX,4C00H          ; Program exits using DOS service
           INT    21H
   ;
   MAIN    ENDP
   END 
