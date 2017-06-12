#include "add.h"
void add(char *chp1, char *chp2)
{
    char* pch[2];
    //np stand for no prefix
    string str[2] ={string(chp1),string(chp2)};

    //determine oprand type
    init_operand(str[0],opr1);
    init_operand(str[1],opr2);

    string opstr;
    //8 opcode bits
    if(type_r(opr1) && type_r(opr2))
    {
        if((opr1.bits == 16) &&(opr2.bits == 16))
        {
            code_PC+=2;
            opstr+="00000011";//03
            opstr+="11";//03
            opstr+=opr1.reg_val;
            opstr+=opr2.reg_val;
        }
        opstr+=opr2.imm_bin_st;
    }
    else if(type_r(opr1) && type_imm(opr2))
    {
        if(opr1.reg_val=="000")
        {
            if((opr1.bits == 16) &&(opr2.bits == 16))
            {
                code_PC+=2;
                opstr+="00000101";//05
                opstr+=opr2.imm_bin_st;
            }
        }

    }else
    {
        cout<<"no such rule";
    }
    cout<<str_bin2hex(opstr.size()/4,opstr)<<"\t"<<opstr<<endl;

}
