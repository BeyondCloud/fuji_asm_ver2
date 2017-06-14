#include "cmp.h"
void cmp(char *chp1, char *chp2)
{
    string opstr; //string to store binary to print
    string str[2] ={string(chp1),string(chp2)};
    init_operand(str[0],opr1);
    init_operand(str[1],opr2);
    if(type_r(opr1)&&type_imm(opr2))
    {
        string imm_bin;
        if(opr1.bits==8)
        {
            if(opr1.reg_val== "000")
            {
                code_PC+=2;
                opstr+="00111100";//3C
            }
            else
            {
                code_PC+=3;
                opstr+="10000000";//80
                opstr+="11111";    //  /7
                opstr+=opr1.reg_val;
            }
            imm_bin = opr2.imm_bin_st;
        }
        else if(opr1.bits==16)
        {
            code_PC+=3;
            opstr="10000011";//83
            opstr+="11111";    //  /7
            opstr+=opr1.reg_val;
            imm_bin = opr2.imm_bin_st.substr(8,8);
        }
        opstr+=imm_bin;
    }
    if(opstr.size()>=16)
        printHex(opstr);
}
