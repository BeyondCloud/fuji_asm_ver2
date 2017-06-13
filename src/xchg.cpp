#include "xchg.h"
void xchg(char *chp1, char *chp2)
{
    //np stand for no prefix
    string str[2] ={string(chp1),string(chp2)};
    //determine oprand type
    init_operand(str[0],opr1);
    init_operand(str[1],opr2);
    string opstr;

    if(type_r(opr1) && type_r(opr2))
    {
        if(opr1.bits==8 && opr2.bits==8)
        {
            code_PC+=2;
            opstr+="10000110";//86
            opstr+="11";
            opstr+=opr1.reg_val;
            opstr+=opr2.reg_val;

        }
        else
            cout<<"no such rule";
        printHex(opstr);
        return;
    }
    cout<<"no such rule";
}
