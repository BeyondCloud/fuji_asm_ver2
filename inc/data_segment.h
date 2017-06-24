#ifndef __DATA_SEGMENT_H
#define __DATA_SEGMENT_H
#include <string.h>
#include <sstream>
#include <fstream>
using namespace std;
extern int data_PC;
extern stringstream ss;
extern fstream org_in;
extern fstream lst_out;
extern fstream obj_out;

extern int is_out_i;
string data_seg();

extern string data_seg_str;
#endif
