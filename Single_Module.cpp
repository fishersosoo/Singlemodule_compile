#include "Single_Module.h"



Single_Module::Single_Module(int Num)
{
	NumOfMode = Num;
	NumOfOp = new unsigned int[NumOfMode];
	Mode = new vector<instruction>*[NumOfMode];
}


Single_Module::~Single_Module()
{
}
Single_Module::Single_Module()
{
}

