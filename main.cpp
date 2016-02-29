#pragma once
using namespace std;
#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include "Singlemodule_compile.h"
void main()
{

// 	c.clear();
// 	int	FloatNum ;
// 	cin >> FloatNum;
// 	typedef union
// 	{
// 		int FloatNum;
// 		unsigned char  data_b[4];
// 	}FloatYByte;
// 	FloatYByte temp;
// 	temp.FloatNum = FloatNum;
// 	for (int i =0; i >=0;--i)
// 	{
// 		c<< temp.data_b[i];
// 
// 	}
	 

	All_Module mo;
	for (unsigned int i = 0; i < mo.Module[1001]->NumOfMode;++i)
	{
		cout << "mode: " << i + 1 << endl;
		for (int j = 0; j < mo.Module[1001]->NumOfOp[i];++j)
		{
			cout << "operation: " << j + 1 << endl;
			for (int k = 0; k < mo.Module[1001]->Mode[i][j].size(); ++k)
			{
				cout << "op_code: 0x" << hex << mo.Module[1001]->Mode[i][j][k].Opcode ;
				for (int l = 0; l < mo.Module[1001]->Mode[i][j][k].Operand.size();++l)
				{
					cout <<" "<< dec << mo.Module[1001]->Mode[i][j][k].Operand[l] ;
				}
				cout << endl;
			}
		}
	}


	system("pause");
}