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
using namespace std;
stringstream ss;

#include "opcode_proc.h"
#include "opcode.h"
#include "map_tbl.h"



ifstream infile("test_mov.cpp");
ofstream outfile ("example.txt");
int main () {


//    setup_imm(opr1,"34H");
//    cout<<opr1.imm_bin_st;



    tbl_init();
    string line;
    char str[128];
    char *pch;
    char* opr[2];
    string mnem="0";
    assert(infile.is_open());

    while ( getline (infile,line) )
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
        if(!pch_str.compare("EQU"))
        {
            strcpy(str,line.c_str());
            cout<<str;
            pch = strtok (str," \t");
            string def_name(pch);
            cout<<def_name<<" define to";
            pch = strtok (NULL, " \t");
            pch = strtok (NULL, " \t");
            string to_name(pch);
            equ_tbl[def_name]=to_name;
            cout<<to_name;

            break;
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
    infile.close();

  return 0;

}
