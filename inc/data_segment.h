#ifndef __DATA_SEGMENT_H
#define __DATA_SEGMENT_H
#include <string.h>
#include <sstream>
#include <fstream>
using namespace std;
extern int data_PC;
extern stringstream ss;
extern fstream org_in;
void data_seg();
#endif
