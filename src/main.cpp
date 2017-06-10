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


char org_name[]="test_data.cpp";
char pass1_name[]="pass1.txt";

fstream org_in;
fstream pass1;
string init_str = "800F000D746573745F646174612E41534D9C962B000005535441434B055F44415441064447524F5550055F5445585405535441434B044441544104434F44450E9807004800000508010B9807004800000307010E980700740";
string postinit_str ="020601E29A060004FF02FF035988040000A201D1";
int main () {


//    setup_imm(opr1,"34H");
//    cout<<opr1.imm_bin_st;
/*
    char *fooch;
    char foo[128];
    string s= " \t";
    strcpy(foo,s.c_str());
    fooch = strtok (foo," \t,");
    cout<<fooch;
    return 0;
*/
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
                    init_str+=postinit_str;

                }
            }
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
