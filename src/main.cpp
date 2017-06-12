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
#include <algorithm>

using namespace std;
stringstream ss;

#include "opcode_proc.h"
#include "opcode.h"
#include "map_tbl.h"
#include "misc_str.h"
#include "data_segment.h"

char org_name[]="test_data.cpp";
char pass1_name[]="pass1.txt";
char pass2_name[]="pass2.txt";

string ident = "uASM-TV01S.ASM";
fstream org_in;
fstream pass1;
fstream pass2;


string final_str;
int data_PC = 0;//data counter
int code_PC = 0;//

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
    pass1.open("pass1.txt",ios::out);

    //pass 1
    cout<<"==========PASS1==========\n";
    bool pass_out;
    while ( getline (org_in,line) )
    {
     //   cout<<endl<<"l:"<<line<<endl;
        pass_out = true;
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
                if(pch_str2[pch_str2.size()-1]=='H')
                {
                    pch_str2 = pch_str2.substr(0,pch_str.size()-2);
                    reverse(pch_str2.begin(), pch_str2.end());
                    init_str+=pch_str2;
              //      init_str+=after_stack_size_str;

                }
            }
            else if(pch_str=="DATA")
            {
                data_seg();
               continue;
            }
            pass_out = false;
        }

        do
        {
            string find_equ_str(pch);
            if(find_equ_str[0]==';')
                break;
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

                pass_out = false;
                break;

            }
            cout<<pch<<"\t";
            pch = strtok (NULL, " \t");

        }while(pch!= NULL);


        if(pass_out)
        {
            strcpy(str,line.c_str());
            pch = strtok (str," \t,");
            while(pch!=NULL)
            {
                if(pch[0]==';')
                    break;
                pass1<<pch<<"\t";
                pch = strtok (NULL, " \t");
            }
            pass1<<endl;
        }

        cout<<endl;
    }
    org_in.close();
    pass1.close();


 //   return 0;
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
    //return to file start
    /*
    org_in.clear();
    org_in.seekg(0, ios::beg);
    */
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

    while ( getline (pass1,line) )
    {
        mnem = "0";
        strcpy(str,line.c_str());
        pch = strtok (str," \t,");
        if(pch==NULL)   //avoid empty line
            continue;
        //match opcode
        string pch_str(pch);
        if(pch_str[pch_str.size()-1] == ':')
        {
            //remove last ':'
            string name = pch_str.substr(0,pch_str.size()-1);
            name = lowerCase(name);
            cout<<"  tbl:"<<name<<">"<<getPCstr(code_PC)<<endl;
            addr_tbl[name] =getPCstr(code_PC);
            cout<<getPCstr(code_PC)<<"\t"<<str<<endl;
             pch = strtok (NULL," \t,");
             if(pch == NULL)
                continue;
        }
        while(1)
        {

            string pch_str(pch);
            if(op_tbl.find(pch_str) != op_tbl.end())
            {
                if(!pch_str.compare("MOV"))
                {
                    mnem = "MOV";
                    break;
                }
                if(!pch_str.compare("CMP"))
                {
                    mnem = "CMP";
                    break;
                }
            }
            if(!pch_str.compare("PROC"))
            {
                strcpy(str,line.c_str());
                pch = strtok (str," \t");
                string name(pch);
                name= lowerCase(name);
                cout<<"  tbl:"<<name<<">"<<getPCstr(code_PC)<<endl;
                addr_tbl[name] =getPCstr(code_PC);
                cout<<getPCstr(code_PC)<<"\t"<<str<<endl;
                break;
            }
            if(!pch_str.compare("CALL"))
            {
                cout<<getPCstr(code_PC)<<"\t"<<"b8\t";
                pch = strtok (NULL, " \t");
                string name(pch);
                cout<<name;
                code_PC+=3;
                break;
            }
            if(!pch_str.compare("INT"))
            {
                pch = strtok (NULL, " \t");
                string name(pch);
                if(name[name.size()-1] == 'H')
                {
                    name = name.substr(0,name.size()-1);
                    if(is_hex_str(name))
                    {
                        cout<<getPCstr(code_PC)<<"\t";
                        cout<<"cd"<<name<<endl;
                        code_PC+=2;
                        break;
                    }
                }
            }
            if(!pch_str.compare("ENDP"))
                break;
            if(!pch_str.compare("END"))
                break;

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
            cout<<getPCstr(code_PC)<<"\t";
          while (pch != NULL)
          {

            //remove comment
            if(pch[0] == ';')
            {
                break;
            }
                cout << pch << "\t";
            pch = strtok (NULL, " \t,");
            if(opr_id < 2)
            {
                opr[opr_id] = pch;
                opr_id++;
            }
          }
          if(opr_id==2)
            op_tbl[mnem](opr[0],opr[1]);
        }

          cout<<"\n";
    }

    pass1.close();
    pass2.close();


  return 0;

}

