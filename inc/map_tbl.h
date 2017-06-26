#ifndef __MAP_TBL_H
#define __MAP_TBL_H
#include <map>
#include <string>

using namespace std;
typedef void (*op_fp)(char* str1,char* str2);
extern map<string, op_fp> op_tbl;
extern map<string, string> jxx_tbl;
extern map<string, string> reg32_tbl;
extern map<string, string> reg16_tbl;
extern map<string, string> reg8_tbl;
extern map<string, string> sreg_tbl;

extern map<string, string> mod_tbl;

extern map<string, string> equ_tbl;
extern map<string, string> addr_tbl;//record addr of function and PROC,4 hex
extern map<string, string> proc_tbl;//record PROC,this has included on addr_tbl
                                    //we create this table to output obj


//map<string, op_fp>::iterator iter;
//eax 000 , ebx 011 ,ecx 001 ,edx  010
void tbl_init();

#endif
