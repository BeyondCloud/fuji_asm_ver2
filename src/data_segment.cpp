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
string data_seg_str;
string data_seg()
{
    string line;
    char str[128];
    char *pch;
    bool isNewAddr = true;
    int old_dataPC;
    while ( getline (org_in,line) )
    {
        is_out_i++;
        old_dataPC = data_PC;
        isNewAddr = true;
        string result;//use for storing long string hex code
        strcpy(str,line.c_str());
        if(str[0]==';')
           continue;
        if(str[0]=='.') //exit data field
        {
            string pch_str(str);
            pch_str = pch_str.substr(1,pch_str.size()-1);
            return pch_str;
            break;
        }
        pch = strtok (str," \t,");
        if(pch==NULL)   //avoid empty line
            continue;

        while(1)
        {

            if(pch==NULL)
                break;
            string pch_str(pch);

            if(pch_str == "DB" ||pch_str == "BSS" )
            {

                strcpy(str,line.c_str());
                bool strREC= false;
                string scan1;
                //expand "xxx",'x' to ascii HEX code if there is
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

                result = "";
                if(isNewAddr)
                    pch = strtok (NULL," \t,");
                while(pch!=NULL)
                {
                    if(pch[0]==';')
                        break;
                    string st(pch);

                    if(tbl_find(equ_tbl,st))
                    {
                        //result +=equ_tbl[st];
                        string tmp = equ_tbl[st];
                        if (tmp.back() == 'H')
                        {
                            string noH = tmp.substr(0,tmp.size()-1);
                            if(is_hex_str(noH))
                                result+=noH;
                        }

                    }
                    else if(is_number(st))
                    {
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
                    ss.str("");
                    ss << hex << setfill('0')<<setw(4)<<s;
                    data_PC+=2;
                    s = ss.str();
                }
                else if(tbl_find(addr_tbl,lowerCase(s)))
                {
                    s = addr_tbl[lowerCase(s)];
                    s = subHexStr(getPCstr(data_PC),s,4);
                    data_PC+=2;
                }
                result = s;
                break;
            }

            pch = strtok (NULL," \t,");

        }
        lst_out<<getPCstr(old_dataPC)<<"\t";
        lst_out<<result<<endl;
        lst_out<<line<<endl;
        data_seg_str+=result;


    }
    return "END";
}
