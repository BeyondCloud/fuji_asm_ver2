// reading a text file
#include <iostream>
#include <fstream>
#include <string.h>
#include <sstream>
#include <bitset>
#include <stdio.h>
#include <map>
#include  <iomanip>
#include  <assert.h>
#include  <vector>

#include <algorithm>

using namespace std;
stringstream ss;

#include "opcode_proc.h"
#include "map_tbl.h"
#include "misc_str.h"
#include "data_segment.h"
void jxx(char *pch,string J_hex);
char org_name[]="test_data.cpp";
char pass1_name[]="pass1.txt";
char pass2_name[]="pass2.txt";
char lst_name[]="lst.txt";
bool is_out[256]={false};
int is_out_i=0;

string ident = "uASM-TV01S.ASM";
fstream org_in;
fstream pass1;
fstream pass2;
fstream p2_addr;

fstream lst_out;

string final_str;
int data_PC = 0;//data counter
int code_PC = 0;//
vector<string> ins_vec = {"MOV","CMP","SUB","ADD","DEC","XCHG"};

void pFinal()
{

    cout<<"    00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F";
    int line = 0;
    for(int i =0;i<final_str.size();i++)
    {

        if(i%2==0)
            cout<<" ";
        if(i%32==0)
        {
            cout<<endl;
            ss.str("");
            ss << hex << setfill('0')<<setw(3)<<line;
            cout<<ss.str()<<" ";
            line++;
        }
        cout<<final_str[i];
    }
}


int main ()
{
/*
    final_str = "80";
    ss.str("");
    ss << hex << setfill('0')<<setw(2)<<ident.size()+2;
    final_str +=ss.str();
    final_str+="00";
    ss.str("");
    ss << hex << setfill('0')<<setw(2)<<ident.size();
    final_str+= ss.str();
    final_str+=string_to_hex(ident);
    final_str+="xx";
    final_str+=init_str;
    final_str+=stack_size_str;
    final_str+=after_stack_size_str;
*/



  //  pFinal();
  //  return 0;
    tbl_init();
    string line;
    string line_org;
    string line_p2;


    char str[128];
    char *pch;
    char* opr[2];
    string mnem="0";
    org_in.open(org_name,ios::in);
    if(!org_in)
    {
        cout<<"cannot open source file";
        return 0;
    }
    pass1.open(pass1_name,ios::out);

    lst_out.open(lst_name,ios::out);
    //copy data segment to lst
    while ( getline (org_in,line) )
    {
        strcpy(str,line.c_str());
        pch = strtok (str," \t,");   //first word
        if(pch==NULL)   //avoid empty line
            continue;
        string pch_str(pch);
        if(str[0]=='.')
        {
            pch_str = pch_str.substr(1,pch_str.size()-1);
            if(pch_str=="DATA")
            {
               break;
            }
        }
        lst_out<<line<<endl;

    }


    org_in.clear();
    org_in.seekg(0, ios::beg);

    //cout<<"==========PASS1==========\n";
    bool CODE_OUT = false;

    while ( getline (org_in,line) )
    {
     //   cout<<endl<<"l:"<<line<<endl;
        is_out_i++;

        strcpy(str,line.c_str());
        pch = strtok (str," \t,");   //first word
        if(pch==NULL)   //avoid empty line
            continue;
        string pch_str(pch);
        //init segment
        if(str[0]=='.')
        {

            pch_str = pch_str.substr(1,pch_str.size()-1);

            if(pch_str=="STACK")
            {
                pch = strtok (NULL," \t");
                string pch_str2(pch);
                if(pch_str2.back()=='H')
                {
                    pch_str2 = pch_str2.substr(0,pch_str.size()-2);
                    reverse(pch_str2.begin(), pch_str2.end());
                    init_str+=pch_str2;
              //      init_str+=after_stack_size_str;

                }
                continue;
            }
            else if(pch_str=="DATA")
            {
                pch_str  = data_seg();

            }
            if(pch_str=="CODE")
            {
                CODE_OUT = true;

               continue;
            }
        }
        //build EQU table
        do
        {
            string find_equ_str(pch);
            if(find_equ_str[0]==';')
            {

                break;
            }
            if(!find_equ_str.compare("EQU"))
            {

                strcpy(str,line.c_str());
                cout<<str;
                pch = strtok (str," \t");
                string def_name(pch);
                pch = strtok (NULL, " \t");
                pch = strtok (NULL, " \t");
                string to_name(pch);
                equ_tbl[def_name]=to_name;
                cout<<" equ tbl:"<<def_name<<">"<<to_name<<endl;
                break;
            }
            pch = strtok (NULL, " \t");
        }while(pch!= NULL);
        if(CODE_OUT)
        {
            is_out[is_out_i] = true;
            strcpy(str,line.c_str());

            pch = strtok (str," \t,");
            string str(pch);
            if(pch[0]==';')
            {
                is_out[is_out_i] =false;
            }
            else
            {
                if(str.back()==':')
                {
                    pass1<<str<<"\t";

                     pch = strtok (NULL," \t,");

                     if(pch==NULL ||pch[0]==';')
                        is_out[is_out_i] =false;

                }
                while(pch!=NULL)
                {
                    if(pch[0]==';')
                    {
                        break;
                    }
                    pass1<<pch<<"\t";
                    pch = strtok (NULL, " \t");
                }
                if(is_out[is_out_i])
                    pass1<<endl;
            }

        }


    }
    /*
    org_in.clear();
    org_in.seekg(0, ios::beg);
    */
    pass1.close();
    pass1.open("pass1.txt",ios::in);
    pass2.open("pass2.txt",ios::out);

    if(!pass1)
    {
        cout<<"cannot open pass1 file";
        return 0;
    }
    if(!pass2)
    {
        cout<<"cannot open pass2 file";
        return 0;
    }
/*
    is_out_i++;

    cout<<"i="<<dec<<is_out_i;

    for(int i =0;i<128;i++)
    {
        if(is_out[i])
            cout<<"line"<<i<<"out\n";
        else
            cout<<i<<"block\n";

    }
    return 0;
*/

    //return to file start


/*
    for(int i =0;i<init_str.size();i+=2)
    {
        if(i%32==0)
            cout<<endl;
        cout<<init_str[i]<<init_str[i+1]<<" ";

    }
*/
    //pass2
    cout<<"==========PASS2==========\n";
    lst_out<<".CODE"<<endl;

    while ( getline (pass1,line) )
    {
        getline (org_in,line_org);
        mnem = "0";
        strcpy(str,line.c_str());
        pch = strtok (str," \t,");
        if(pch==NULL)   //avoid empty line
            continue;


        //match opcode
        string pch_str(pch);
        if(pch_str.back() == ':')
        {
            //remove last ':'
            string name = pch_str.substr(0,pch_str.size()-1);
            name = lowerCase(name);
            addr_tbl[name] =getPCstr(code_PC);
            cout<<"  tbl:"<<name<<">"<<getPCstr(code_PC)<<endl;
            //cout<<str;
             pch = strtok (NULL," \t,");
             if(pch == NULL)
             {
                continue;
             }

        }
        cout<<getPCstr(code_PC)<<"\t";
        pass2<<getPCstr(code_PC)<<"\t";

        while(1)
        {

            string pch_str(pch);
            pch_str = upperCase(pch_str);
            if(op_tbl.find(pch_str) != op_tbl.end())
            {
                for(int i =0;i<ins_vec.size();i++)
                {
                    if(!pch_str.compare(ins_vec[i]))
                        mnem = ins_vec[i];
                }
                break;
            }
            if(!pch_str.compare("PROC"))
            {
                strcpy(str,line.c_str());
                pch = strtok (str," \t");
                string name(pch);
                name= lowerCase(name);
                addr_tbl[name] =getPCstr(code_PC);
                cout<<"  tbl:"<<name<<">"<<getPCstr(code_PC)<<endl;

                //cout<<str<<"\t"<<"PROC"<<endl;
                break;
            }
            if(!pch_str.compare("DB"))
            {
                pch = strtok (NULL," \t");
                string name(pch);
                if(name[name.size()-1]=='H')
                {
                    if(name.size()>2)
                        name = name.substr(name.size()-3,2);
                }
                string bin_str =str_hex2bin(name);
                printHex(bin_str);
                code_PC+=1;
                break;
            }
            if(tbl_find(jxx_tbl,pch_str))
            {
                jxx(pch,jxx_tbl[pch_str]);
                break;
            }
            if(!pch_str.compare("INT"))
            {
                pch = strtok (NULL, " \t");
                string name(pch);
                if(name.back() == 'H')
                {
                    name = name.substr(0,name.size()-1);
                    if(is_hex_str(name))
                    {
                        cout<<"cd"<<name;
                        pass2<<"cd"<<name;

                        code_PC+=2;
                        break;
                    }
                }
            }
            if(!pch_str.compare("LEA"))
            {
                code_PC+=4;
                pch = strtok (NULL, " \t,");
                string reg_str(pch);
                init_operand(reg_str,opr1);
                string opstr = "10001101";//8D
                opstr+="00";
                opstr+=opr1.reg_val;
                opstr+="110";
                pch = strtok (NULL, " \t,");
                string name(pch);
                name = lowerCase(name);
                printHex(opstr);
                pass2<<"\t"<<name;
                pass2<<"\t4_";
                break;
            }
            if(!pch_str.compare("RET"))
            {
                cout<<"c3";
                pass2<<"c3";
                code_PC+=1;
                break;
            }
            if(!pch_str.compare("ENDP"))
            {
                cout<<"\t";
                pass2<<"\t";

                break;
            }

            if(!pch_str.compare("END"))
            {
                cout<<"\t";
                pass2<<"\t";
                break;
            }


            pch = strtok (NULL, " \t");
            if(pch==NULL)
            {
                cout<<line<<" fail to match\n";
                break;
            }

        }
        //match oprand

        if( mnem !="0")
        {
           int opr_id = 0;
           if(pch==NULL)
            cout<<getPCstr(code_PC)<<"\t";
          while (pch != NULL)
          {

            //remove comment
            if(pch[0] == ';')
            {
                break;
            }
         //       cout << pch << "\t";
            pch = strtok (NULL, " \t,");
            if(opr_id < 2)
            {
                opr[opr_id] = pch;
                opr_id++;
            }
          }
          if(opr_id==2)
          {
            op_tbl[mnem](opr[0],opr[1]);
          }else if(opr_id ==1)
          {
            opr[1]=NULL;
            op_tbl[mnem](opr[0],opr[1]);
          }
        }

          cout<<"\t"<<line;
          cout<<"\n";
        if(is_out[is_out_i])
            pass2<<line_org<<endl;

          //pass2<<"\n";



    }


    pass1.close();
    pass2.close();
    pass2.open("pass2.txt",ios::in);
    p2_addr.open("p2_addr.txt",ios::out);
    string j_to = "";
    string j_from = "";
    int j_len = 0;

    while ( getline (pass2,line_p2) )
    {
        if(j_len != 0)
        {
            strcpy(str,line_p2.c_str());
            pch = strtok (str,"\t");
            string pch_str(pch);
            if(j_from == "0")
                j_from = "0000";
            else
                j_from = pch_str;
            string r_addr = subHexStr(j_to,j_from,j_len);
            p2_addr<<r_addr<<endl;
            //p2_addr<<"f:"<<j_from<<"\t";
            //p2_addr<<"t:"<<j_to<<endl;


            j_from ="";
            j_to = "";
            j_len = 0;
        }
        if(line_p2.back() == '<' ||line_p2.back() == '_')
        {
            strcpy(str,line_p2.c_str());
            pch = strtok (str,"\t"); //PC
            p2_addr<<pch<<"\t";
            pch = strtok (NULL,"\t");
            p2_addr<<pch<<"\t";
            pch = strtok (NULL,"\t"); //OP
            string pch_str(pch);
            if(tbl_find(addr_tbl,pch_str))
            {
                j_to = addr_tbl[pch_str];
            }
            else
            {
                cout<<pch_str<<":pass2 tbl not found!"<<endl;
            }
            pch = strtok (NULL,"\t"); //len
            string len(pch);
            j_len = stoi(len);
            if(line_p2.back()== '_')
             j_from ="0";
        }
        else
             p2_addr<<line_p2<<endl;

    }


   // lst_out.open("lst.txt",ios::out);

    //return 0;
    p2_addr.close();
    p2_addr.open("p2_addr.txt",ios::in);

    org_in.clear();
    org_in.seekg(0, ios::beg);
    is_out_i=1;
    while ( getline (org_in,line_org) )
    {
        if(is_out[is_out_i])
        {
            getline(p2_addr,line_p2);
            lst_out<<line_p2;
            lst_out<<line_org<<endl;
        }
        is_out_i++;

    }
  return 0;

}
void jxx(char *pch,string J_hex)
{
    pch = strtok (NULL, " \t");

    cout<<J_hex;
    pass2<<J_hex;

    string name(pch);
    pass2<<"\t"<<lowerCase(name);
    if(J_hex=="e8")//CALL
    {
        code_PC+=3;
        pass2<<"\t4<";
    }
    else
    {
        code_PC+=2;
        pass2<<"\t2<";
    }



}
