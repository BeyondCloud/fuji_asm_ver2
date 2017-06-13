#include "data_segment.h"
#include <iostream>
#include <string.h>
#include <sstream>
#include <bitset>
#include <stdio.h>
#include <map>
#include  <iomanip>
#include  <assert.h>
#include <algorithm>
#include "opcode_proc.h"

void data_seg()
{
    string line;
    char str[128];
    char *pch;
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
             // cout<<endl;
                if(isNewAddr)
                {
                    name= lowerCase(name);
                    cout<<"  tbl:"<<name<<">"<<getPCstr(data_PC)<<endl;
                    addr_tbl[name] =getPCstr(data_PC);
                }
                data_PC+= (result.size()/2);
                printPC(data_PC);
                name = lowerCase(name);

                break;
                //cout<<lowerCase(name);
                //for(;;){}
            }
            else if(pch_str == "DW")
            {

                strcpy(str,line.c_str());
                pch = strtok(str," \t");
                string name(pch);
                pch = strtok(NULL," \t,");//remove DW
                pch = strtok (NULL," \t$-");
                string s(pch);
                if(isNewAddr)
                {
                    name= lowerCase(name);
                    cout<<"  tbl:"<<name<<">"<<getPCstr(data_PC)<<endl;
                    addr_tbl[name] =getPCstr(data_PC);
                }
                if(is_number(s))
                {
                    int num;
                    ss.str("");
                    ss << hex << setfill('0')<<setw(4)<<s;
                    data_PC+=2;
                    break;
                }
                s = lowerCase(s);

                if(tbl_find(addr_tbl,s))
                {
                   // printPC(data_PC);
                    data_PC+=2;

                }

                break;
            }

            pch = strtok (NULL," \t,");

        }
    }
}
