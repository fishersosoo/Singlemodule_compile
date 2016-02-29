#include "All_Module.h"


All_Module::All_Module()
{
	c.open("C:/Users/展宁/Desktop/新建文本文档.txt", ios::in);
	unsigned int Now_SetModule;
	unsigned int Now_SetMode;
	unsigned int Now_SetOp;
	while (!c.eof())
	{
		string head;
		c >> head;
		unsigned int value;
		if (head=="op_code")
		{
			c >> hex>> value;
			Single_Module::instruction instruct(value);
			unsigned int operance;
			char isend;
			c.get(isend);
			while (isend != ';')
			{
				c >> operance;
				instruct.Operand.push_back(operance);
				c.get(isend);
			}
				Module[Now_SetModule]->Mode[Now_SetMode][Now_SetOp].push_back(instruct);
		}
		else
		{
			c >> dec >> value;
		}
		if (head=="module_type")
		{
			Now_SetModule = value;
			unsigned int mode;
			c >> dec >> mode;
			Single_Module* x = new Single_Module(mode);
			Module.insert(map<unsigned int, Single_Module*>::value_type(value,x));
		}
		if (head=="mode_type")
		{
			Now_SetMode = value-1;
			unsigned int temp;
			c >> dec >> temp;
			Module[Now_SetModule]->NumOfOp[Now_SetMode] = temp;
			Module[Now_SetModule]->Mode[Now_SetMode] = new vector<Single_Module::instruction>[temp];
		}
		if (head=="operation")
		{
			Now_SetOp = value-1;
			vector<unsigned int> opvector;
		}
	}
}
All_Module::~All_Module()
{
}
