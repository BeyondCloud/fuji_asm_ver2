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

char org_name[]="test_data.cpp";
char pass1_name[]="pass1.txt";
string ident = "uASM-TV01S.ASM";
fstream org_in;
fstream pass1;

string final_str;
int data_PC = 0;//program counter
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
void printPC(int pc)
{
     cout<<"\nPC:"<<hex<<setw(4)<<setfill('0')<<pc<<endl;
}
string getPCstr(const int pc)
{
    ss.str("");
    ss<<hex<<setw(4)<<setfill('0')<<pc;
    return ss.str();
}
char last_letter(char *str)
{
  int len = strlen(str);
  return len > 0 ? *(str + len - 1 ): *str;
}
string lowerCase(string data)
{
    transform(data.begin(), data.end(), data.begin(), ::tolower);
    return data;
}
void data_seg()
{
    string line;
    char str[128];
    char *pch;
    printPC(data_PC);
    bool isNewAddr = true;
    while ( getline (org_in,line) )
    {
        isNewAddr = true;
        strcpy(str,line.c_str());
        if(str[0]=='.') //exit data field
            break;
        pch = strtok (str," \t,");
        if(pch==NULL)   //avoid empty line
            continue;

        while(1)
        {

            if(pch==NULL)
                break;
            string pch_str(pch);
            if(pch_str == "DB")
            {
                strcpy(str,line.c_str());
                bool strREC= false;
                string scan1;
                //expand "xxx",'x' to ascii HEX code
                for(int i=0;i<line.size();i++)
                {
                    if(str[i] == '\"'|| str[i] == '\'')
                    {
                        strREC = !strREC;
                        continue;
                    }
                    if(strREC)
                    {
                        string asc;
                        asc+=str[i];
                        scan1+= string_to_hex(asc);

                    }else
                    {
                        scan1+=str[i];
                    }
                }
                strcpy(str,scan1.c_str());
              //  cout<<endl<<"scan1:"<<str;
                pch = strtok (str," \t,");//remove name
                string name(pch);
                if(name == "DB" ||name == "DW")
                {
                    isNewAddr = false;
                }
                pch = strtok (NULL," \t,"); //remove DB

                string result;
                if(isNewAddr)
                    pch = strtok (NULL," \t,");
                while(pch!=NULL)
                {

                    string st(pch);
                    if(tbl_find(equ_tbl,st))
                    {
                        //result +=equ_tbl[st];
                        string tmp = equ_tbl[st];
                        if (tmp[tmp.size()-1] == 'H')
                        {
                            string noH = tmp.substr(0,tmp.size()-1);
                            if(is_hex_str(noH))
                                result+=noH;
                        }

                    }
                    else if(is_number(st))
                    {
                        int num;
                        ss.str("");
                        ss << hex << setfill('0')<<setw(2)<<st;
                        result +=ss.str();
                    }
                    else
                        result+=pch;
                    pch = strtok (NULL," \t,");

                }
              //  printPC(data_PC);
              cout<<endl;
                data_PC+= (result.size()/2);
                cout<<result;
                printPC(data_PC);
                name = lowerCase(name);
                if(isNewAddr)
                    addr_tbl[name] =getPCstr(data_PC);
                break;
                //cout<<lowerCase(name);
                //for(;;){}
            }
            else if(pch_str == "DW")
            {
                pch = strtok (NULL," \t$-");//remove name
                string s(pch);
                if(is_number(s))
                {
                    int num;
                    ss.str("");
                    ss << hex << setfill('0')<<setw(4)<<s;
                    data_PC+=2;
                    cout<<ss.str();
                    printPC(data_PC);
                    break;
                }
                s = lowerCase(s);

                if(tbl_find(addr_tbl,s))
                {
                    cout<<addr_tbl[s];
                    data_PC+=2;
                    printPC(data_PC);
                }
                break;
            }
            pch = strtok (NULL," \t,");

        }
    }
    for(;;){}
}

int main ()
{

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
        pch = strtok (str," \t,");
        //avoid space
        if(pch==NULL)
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
                data_seg();
            pass_out = false;
        }
        //init EQU table
        do
        {
            string find_equ_str(pch);
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


    return 0;

    pass1.open("pass1.txt",ios::in);
    if(!pass1)
    {
        cout<<"cannot open pass1 file";
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
      strcpy(str,line.c_str());
        pch = strtok (str," \t,");
      //match opcode
      while(1)
      {
        if(pch==NULL)
        {
            cout<<line<<" fail to match\n";
            break;
        }
        string pch_str(pch);
        if(op_tbl.find(pch_str) != op_tbl.end())
        {
            if(!pch_str.compare("MOV"))
            {
                mnem = "MOV";
                break;
            }
        }

        pch = strtok (NULL, " \t");

      }
       //match oprand

       if( mnem !="0")
       {
           int opr_id = 0;

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


  return 0;

}

