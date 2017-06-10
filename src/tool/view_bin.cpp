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
#include "opcode.h"
using namespace std;

ifstream infile("test_mov.cpp");
ofstream outfile ("example.txt");
stringstream ss;
const char* h2b(char c)
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
int main () {
    string line;
    char str[128];
    char *pch;
    char* opr[2];

    assert(infile.is_open());
    while ( getline (infile,line) )
    {
      strcpy(str,line.c_str());
        pch = strtok (str," \t,");

      //match opcode
      while(1)
      {
        string pch_str(pch);
        if(!pch_str.compare("MOV"))
            break;
        pch = strtok (NULL, " \t");
      }
       //match oprand
       int opr_id = 0;
       bool bin_out=false;
      while (pch != NULL)
      {
        if(bin_out)
        {
           cout<<h2b(pch[0])<<" ";
           cout<<h2b(pch[1])<<";";
        }
        //remove comment
        if(pch[0] == ';')
        {
           cout<<h2b(pch[1])<<" ";
           cout<<h2b(pch[2]);
            bin_out = true;
        }
//        if(pch[strlen(pch)-1]!=':')
            cout << pch << "\t";

        if(opr_id < 2)
        {
            opr[opr_id] = pch;
            opr_id++;
        }
        pch = strtok (NULL, " \t,");
      }


      cout<<"\n";
    }
    infile.close();

  return 0;

}
