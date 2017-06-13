#ifndef __OPCODE_PROC_H
#define __OPCODE_PROC_H
#include <string>

#include "map_tbl.h"
#include <iostream>
#include <bitset>
#include  <assert.h>
#include <sstream>
#include <iomanip>
#include <string.h>
#include <stdexcept>
#include <fstream>
using namespace std;
void printHex(string str);
void printPC(int pc);
string subHexStr(string s1,string s2,int length);
string getPCstr(const int pc);
char last_letter(char *str);
std::string string_to_hex(const std::string& input);
std::string hex_to_string(const std::string& input);
string upperCase(string data);
string lowerCase(string data);
inline const char* h2b(char c);
extern fstream lst_out;

typedef enum{reg,sreg,mem,imm,addr,N_A} type_t;
struct oprand_t
{
    string name; //ex: eax, ebx...
    int bits;
    int type;
    string imm_bin_st;
    string reg_val;//ex:000,001
    void clear()
    {
        name = ""; //ex: eax, ebx...
        bits = -1;
        type = N_A;
        imm_bin_st="";
        reg_val="";//ex:000,001
    }

};
bool is_hex_str(string str);
extern oprand_t opr1,opr2;
bool setup_imm(oprand_t &opr,string const& s,int bits);

string str_bin2hex(int hex_len,string bin);
string str_hex2bin(string hex);

bool is_number(const string& s);


void init_operand(string str,oprand_t &opr);

inline const char* h2b(char c);
#define tbl_find(tbl,str) (tbl.find(str) != tbl.end())

#define type_imm8(opr_t) (opr_t.type == imm && opr_t.bits == 8)
#define type_imm16(opr_t) (opr_t.type == imm && opr_t.bits == 16)
#define type_imm32(opr_t) (opr_t.type == imm && opr_t.bits == 32)
#define type_rm16_32(opr_t) (((opr_t.type == reg)||(opr_t.type == mem))&& \
                              ((opr_t.bits == 16)||(opr_t.bits == 32)))
#define type_r16_32(opr_t) ((opr_t.type == reg)&& \
                            ((opr_t.bits == 16)||(opr_t.bits == 32)))
#define type_rm8(opr_t) ((opr_t.type == reg ||opr_t.type == mem)&& opr_t.bits == 8)

#define type_r(opr_t) (opr_t.type == reg)
#define type_sr(opr_t) (opr_t.type == sreg)
#define type_imm(opr_t) (opr_t.type == imm)
#define type_addr(opr_t) (opr_t.type == addr)

#define type_rm(opr_t) ((opr_t.type == reg)||(opr_t.type == mem))

#define type_regA(opr_t) ( opr_t.type == reg && opr_t.reg_val == "000")
#endif
