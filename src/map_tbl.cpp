#include "map_tbl.h"
#include "opcode_proc.h"
#include "mov.h"
#include "cmp.h"
#include "add.h"
#include "sub.h"
#include "mul.h"
#include "div.h"
#include "dec.h"


map<string, op_fp> op_tbl;
map<string, string> jxx_tbl;
map<string, string> reg32_tbl;
map<string, string> reg16_tbl;
map<string, string> reg8_tbl;
map<string, string> sreg_tbl;
map<string, string> mod_tbl;
map<string, string> equ_tbl;
map<string, string> addr_tbl;


void tbl_init()
{
    reg32_tbl["EAX"] = "000";
    reg32_tbl["ECX"] = "001";
    reg32_tbl["EDX"] = "010";
    reg32_tbl["EBX"] = "011";
    reg32_tbl["ESP"] = "100";
    reg32_tbl["EBP"] = "101";
    reg32_tbl["ESI"] = "110";
    reg32_tbl["EDI"] = "111";

    reg16_tbl["AX"] = "000";
    reg16_tbl["CX"] = "001";
    reg16_tbl["DX"] = "010";
    reg16_tbl["BX"] = "011";
    reg16_tbl["SP"] = "100";
    reg16_tbl["BP"] = "101";
    reg16_tbl["SI"] = "110";
    reg16_tbl["DI"] = "111";

    reg8_tbl["AL"] = "000";
    reg8_tbl["CL"] = "001";
    reg8_tbl["DL"] = "010";
    reg8_tbl["BL"] = "011";
    reg8_tbl["AH"] = "100";
    reg8_tbl["CH"] = "101";
    reg8_tbl["DH"] = "110";
    reg8_tbl["BH"] = "111";

    sreg_tbl["ES"] = "000";
    sreg_tbl["CS"] = "001";//8E c8
    sreg_tbl["SS"] = "010";//8E d0
    sreg_tbl["DS"] = "011";//8E d8

    jxx_tbl["JLE"] = "7e";
    jxx_tbl["JG"]  = "7f";
    jxx_tbl["JMP"] = "eb";
    jxx_tbl["JE"]  = "74";
    jxx_tbl["CALL"]= "e8";


    op_tbl["MOV"]= mov;
    op_tbl["CMP"]= cmp;
    op_tbl["SUB"]= sub;
    op_tbl["ADD"]= add;
    op_tbl["DEC"]= dec;




    /*
    op_tbl["add"] = add;//01C3  add %eax, %ebx


    op_tbl["sub"] = sub;
    op_tbl["mul"] = mul; // F7E0  mul %eax
    op_tbl["div"] = div;

    //immediate address



    op_tbl["jmp"]= ;//
    //note: jxx will change opcode if jump too far
    op_tbl["ja"]= ja;//
    op_tbl["jae"]=jae;//
    op_tbl["jb"]= jb;//jb jc same op code?
    op_tbl["jbe"]=jbe;//
    op_tbl["jc"]= jc;//
    op_tbl["jcxz"]= jcxz;//

    op_tbl["je"]= je;//
    op_tbl["jg"]= jg;//
    op_tbl["jge"]=jge;//
    op_tbl["jl"]= jl;//
    op_tbl["jle"]=jle;//

    op_tbl["call"]=call;//
    op_tbl["ret"]= ret;//
*/

}
