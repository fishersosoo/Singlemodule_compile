#pragma once
#include <map>
#include <iomanip>
#include <fstream>
#include <string>
#include "Single_Module.h"
using namespace std;

class All_Module
{
public:
	fstream c;
	map<unsigned int, Single_Module*> Module;
	All_Module();
	~All_Module();
};

