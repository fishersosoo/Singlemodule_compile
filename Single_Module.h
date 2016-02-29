#pragma once
#include <vector>
using namespace std;

class Single_Module
{
public:
	struct instruction
	{
		unsigned int Opcode;
		vector<unsigned int> Operand;
		instruction(int code){ Opcode = code; };
	};
	unsigned int NumOfMode;
	unsigned int* NumOfOp;
	vector<unsigned int>* Op;
	vector<instruction>** Mode;
	Single_Module(int NumOfMode);
	~Single_Module();
	Single_Module();
};

