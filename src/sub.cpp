#include "sub.h"
void sub(char *chp1, char *chp2)
{
    char* pch[2];
    //np stand for no prefix
    string str[2] ={string(chp1),string(chp2)};

    //determine oprand type
    int opSrc_type,opDst_type;
    init_operand(str[0],opr1);
    init_operand(str[1],opr2);

    string opstr;
    bool same_size = (opr1.bits ==opr2.bits);
    //8 opcode bits
    if(type_r(opr1) && type_imm(opr2))
    {
        if(opr1.reg_val=="000")
        {
            if(opr1.bits == 8)
            {
                opstr+="00101100";//2C
                code_PC+=2;
            }
            else
                opstr+="00111101";//2D
            opstr+=opr2.imm_bin_st;
        }

    }
    cout<<str_bin2hex(opstr.size()/4,opstr)<<"\t"<<opstr<<endl;

}
