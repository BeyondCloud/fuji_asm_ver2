#include "opcode_proc.h"

using namespace std;
oprand_t opr1,opr2;
bool  setup_imm(oprand_t &opr,string const& s,int bits)
{

    //ascii case
    if(s.size()==3)
    {
        if(((int)s[0])==39 &&((int)s[2])==39 )
        {
            opr.type = imm;
            opr.bits = 8;
            opr.imm_bin_st = bitset<8>((int)s[1]).to_string();
            return true;
        }

    }
    //decimal case

    if(s[s.size()-1] != 'H')
    {

        if(is_number(s))
        {
            unsigned long int val = stoi(s);
            assert(val<=4294967295);
            opr.type = imm;
            if(bits==8)
            {
                opr.bits = 8;
                opr.imm_bin_st = bitset<8>(val).to_string();
            }

            else if (bits==16)
            {
                opr.bits = 16;
                string org = bitset<16>(val).to_string();

                string lsb = org.substr(8,8);
                string msb = org.substr(0,8);
                opr.imm_bin_st = lsb+msb;
            }
            else if (bits==32)
            {
                opr.bits = 32;
                opr.imm_bin_st = bitset<32>(val).to_string();
            }
            else
            {
                cout<<"invalid bit size!\n";
            }
            return true;
        }
    }
    //hex case
    else
    {
        string st = s.substr(0,s.size()-1);//remove H
        string str_bin = "";
        if(st.find_first_not_of("0123456789ABCDEFabcdef", 0) != string::npos)
            return false;
        opr.type = imm;
        if(st.size()== 2)
            opr.bits = 8;
        else if(st.size()== 4)
        {
            for(int i =0;i<st.size();i++)
                str_bin += h2b(st[i]);
            string lsb = str_bin.substr(8,8);
            string msb = str_bin.substr(0,8);
            opr.imm_bin_st = lsb+msb;
            opr.bits = 16;
            return true;
        }
        else if(st.size()== 8)
            opr.bits = 32;
        else
            return false;

        for(int i =0;i<st.size();i++)
            str_bin += h2b(st[i]);
        opr.imm_bin_st = str_bin;

        return true;
    }
    return false;
}

bool is_number(const string& s)
{

    string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}
string str_bin2hex(int hex_len,string bin)
{
    int result =0;
  //  int len = (bin.length()-1)/4;
    for(size_t count = 0; count < bin.length() ; ++count)
    {
        result *=2;
        result += (bin[count]=='1')? 1 :0;
    }
    stringstream ss;
    ss << hex << setw(hex_len) << setfill('0')  << result;
    string hexVal(ss.str());
    return hexVal;
}
void init_operand(string str,oprand_t &opr)
{
    opr.name = str;
    if(tbl_find(reg32_tbl,str))
    {
        opr.bits = 32;
        opr.type = reg;
        opr.reg_val = reg32_tbl[str];
    }
    else if(tbl_find(reg16_tbl,str))
    {
        opr.bits = 16;
        opr.type = reg;
        opr.reg_val = reg16_tbl[str];
    }
    else if(tbl_find(reg8_tbl,str))
    {
        opr.bits = 8;
        opr.type = reg;
        opr.reg_val = reg8_tbl[str];
    }
    else if(setup_imm(opr,str,opr1.bits))
    {}
    else
    {
        cout<<"optype not found";
        opr.type = N_A;
    }

}
inline const char* h2b(char c)
{
    // TODO handle default / error
    switch(toupper(c))
    {
        case '0': return "0000";
        case '1': return "0001";
        case '2': return "0010";
        case '3': return "0011";
        case '4': return "0100";
        case '5': return "0101";
        case '6': return "0110";
        case '7': return "0111";
        case '8': return "1000";
        case '9': return "1001";
        case 'A': return "1010";
        case 'B': return "1011";
        case 'C': return "1100";
        case 'D': return "1101";
        case 'E': return "1110";
        case 'F': return "1111";
    }
}

