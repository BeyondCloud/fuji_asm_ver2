#include "mov.h"
void mov(char *chp1, char *chp2)
{
    //cout<<"> "<<opr1<<opr2<<" <";
    //xxxxx+opr1+xxxxx+opr2
    string opstr; //string to store binary to print
    string str[2] ={string(chp1),string(chp2)};
    if(tbl_find(equ_tbl,str[0]) )
    {
        str[0] = equ_tbl[str[0]];
    }
    if(tbl_find(equ_tbl,str[1]) )
    {
        str[1] = equ_tbl[str[1]];
    }
    init_operand(str[0],opr1);
    init_operand(str[1],opr2);

    if(type_r(opr1)&&type_imm(opr2))
    {
        if(opr1.bits==8)
        {
            code_PC+=2;
            opstr="10110";
            opstr+=opr1.reg_val;
        }
        else if(opr1.bits==16)
        {
            code_PC+=3;
            opstr="10111";
            opstr+=opr1.reg_val;
        }
        else
        {
            opstr="10111000";
        }
        opstr+=opr2.imm_bin_st;
    //    cout<<str_bin2hex(opstr.size()/4,opstr)<<"\t"<<opstr<<endl;
    }
    else if(type_r(opr1)&&type_r(opr2))
    {
        if(opr1.bits==8 &&opr2.bits==8)
        {
            code_PC+=2;
            opstr="10001010";//8A,
            opstr+="11";
            opstr+=opr1.reg_val;
            opstr+=opr2.reg_val;
       //     opstr+=bitset<8>opr2.imm_bin_st;
        }
        else if(opr1.bits==16&&opr2.bits==16)
        {
            opstr="1011111";
            opstr+=opr1.reg_val;


      //      opstr+=bitset<16>imm_bin_st;
        }
        else
        {
            opstr="10111000";
        }
    }
    else if(type_sr(opr1)&&type_r(opr2))
    {
        opstr="1000111011";
        opstr+=sreg_tbl[str[0]];
        if(opr2.bits == 16)
            opstr+=reg16_tbl[str[1]];
        else
            opstr+="000";

    }
    else if(type_r(opr1)&&str[1][0]=='@')
    {
        opstr="10111";
        if(opr1.bits == 16)
            opstr+=reg16_tbl[str[0]];
        opstr +="0000000000000000";

    }
    else
        opstr = "0000";
    if(opstr.size()>=16)
        cout<<str_bin2hex(opstr.size()/4,opstr)<<"\t"<<opstr<<endl;

    /*
    operand_type optype[2];
    optype[0] = get_operand_type(str1);
    if(optype[0] == reg16)
    */
}
