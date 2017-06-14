#include "opcode_proc.h"
#include <algorithm>
using namespace std;
oprand_t opr1,opr2;
void printHex(string str)
{
    string hex_str  = str_bin2hex(str.size()/4,str);
    cout<<hex_str;
    pass2<<hex_str;


}
void printPC(int pc)
{
    cout<<"\nPC:"<<hex<<setw(4)<<setfill('0')<<pc<<endl;
}
string subHexStr(string s1,string s2,int len)
{
    stringstream ss;
    int i1,i2;
    ss << hex<<s1;
    ss >> i1;
    ss.str("");
    ss.clear();

    ss << hex<<s2;
    ss >> i2;
    ss.str("");
    ss.clear();
    int result = i1-i2;
    ss<<hex<<setw(4)<<setfill('0')<<result;
    string r_str = ss.str();
    r_str = r_str.substr(r_str.size()-len,len);
    return r_str;
}
string getPCstr(const int pc)
{
    stringstream ss;
    ss<<hex<<setw(4)<<setfill('0')<<pc;
    return ss.str();
}
char last_letter(char *str)
{
  int len = strlen(str);
  return len > 0 ? *(str + len - 1 ): *str;
}
bool is_hex_str(string str)
{
    return (str.find_first_not_of("0123456789ABCDEFabcdef", 0) == string::npos);
}
std::string string_to_hex(const std::string& input)
{
    static const char* const lut = "0123456789ABCDEF";
    size_t len = input.length();

    std::string output;
    output.reserve(2 * len);
    for (size_t i = 0; i < len; ++i)
    {
        const unsigned char c = input[i];
        output.push_back(lut[c >> 4]);
        output.push_back(lut[c & 15]);
    }
    return output;
}
string lowerCase(string data)
{
    transform(data.begin(), data.end(), data.begin(), ::tolower);
    return data;
}
string upperCase(string data)
{
    transform(data.begin(), data.end(), data.begin(), ::toupper);
    return data;
}
std::string hex_to_string(const std::string& input)
{
    static const char* const lut = "0123456789ABCDEF";
    size_t len = input.length();
    if (len & 1) throw std::invalid_argument("odd length");

    std::string output;
    output.reserve(len / 2);
    for (size_t i = 0; i < len; i += 2)
    {
        char a = input[i];
        const char* p = std::lower_bound(lut, lut + 16, a);
        if (*p != a) throw std::invalid_argument("not a hex digit");

        char b = input[i + 1];
        const char* q = std::lower_bound(lut, lut + 16, b);
        if (*q != b) throw std::invalid_argument("not a hex digit");

        output.push_back(((p - lut) << 4) | (q - lut));
    }
    return output;
}
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
                //convert to binary
                string org = bitset<16>(val).to_string();
                string lsb = org.substr(8,8);
                string msb = org.substr(0,8);
                //opr.imm_bin_st = lsb+msb;//swap (for obj)
                opr.imm_bin_st = msb+lsb;//no swap(for lst)

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
        if(!is_hex_str(st))
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
            //opr.imm_bin_st = lsb+msb;//swap(for obj)
            opr.imm_bin_st = msb+lsb;//no swap (for lst)

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
string str_hex2bin(string hex)
{
    string result;
    for(int i =0;i<hex.size();i++)
        result+=h2b(hex[i]);
    return result;
}
void init_operand(string str,oprand_t &opr)
{
    opr.clear();

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
    else if(tbl_find(sreg_tbl,str))
    {
        opr.bits = 8;
        opr.type = sreg;
        opr.reg_val = sreg_tbl[str];
    }
    else if(tbl_find(addr_tbl,lowerCase(str)))
    {
        opr.name = lowerCase(str);
        opr.bits = 16;
        opr.type = addr;
    }
    else if(setup_imm(opr,str,opr1.bits))
    {}
    else if(opr.name[0]=='@')
    {}
    else
    {
        cout<<"optype not found\n";
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

