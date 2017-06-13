#include "dec.h"
void dec(char *chp1, char *chp2)
{
    char* pch[2];
    //np stand for no prefix
    string str(chp1);

    //determine oprand type
    init_operand(str,opr1);
    string opstr;
    //8 opcode bits
    if(type_r(opr1))
    {
        if(opr1.bits==16)
        {
            code_PC+=1;
            opstr+="01001";//48
            opstr+=opr1.reg_val;
        }

    }
    printHex(opstr);
}
