#include "Singlemodule_compile.h"
#include "ComplieInstructionInfo.h"

Singlemodule_compile::Singlemodule_compile()
{

}


Singlemodule_compile::~Singlemodule_compile()
{
}

void  Singlemodule_compile::Get_modulestring(int module_type, int module_mode)
{
	for (int i = 0; i < allmodule.Module[module_type]->NumOfOp[module_mode]; ++i)//模块模式对应i步
	{
		if (allmodule.Module[module_type]->Mode[module_mode][i].size() == 1)//如果i步只有一条可能指令，则不进行检查
		{
			//将汇编指令操作码转换为二进制，写入modulestring
			unsigned short opcodetemp = allmodule.Module[module_type]->Mode[module_mode][i][0].Opcode;
			vector<BYTE>binary_OpCode = Word_convertbyte(opcodetemp);
			for (int i = 0; i < 2; ++i)
			{
				modulestring += binary_OpCode[i];
			}
			//遍历i步的第1条汇编指令的操作数，替换并且写入到modulestring里面
			ReplacePara(module_type,module_mode,i,0);
			continue;//查询下一步
		}
		for (int k = 0; k < allmodule.Module[module_type]->Mode[module_mode][i].size(); ++k)//遍历i步的第k条可能的汇编指令
		{
			bool ismatch;
			if (G_InstructionInfoTable[allmodule.Module[module_type]->Mode[module_mode][i][k].Opcode].size() != allmodule.Module[module_type]->Mode[module_mode][i][k].Operand.size())//如果参数数量不匹配
			{
				ismatch = 0;
				continue;//查询下一条指令
			}
			ismatch=IsMatch(allmodule.Module[module_type]->Mode[module_mode][i][k]);
			if (ismatch)//如果全部匹配
			{
				//将汇编指令操作码转换为二进制，写入modulestring
				unsigned short opcodetemp = allmodule.Module[module_type]->Mode[module_mode][i][k].Opcode;
				vector<BYTE>binary_OpCode = Word_convertbyte(opcodetemp);
				for (int i = 0; i < 2; ++i)
				{
					modulestring += binary_OpCode[i];
				}
				//遍历i步的第k条汇编指令的操作数，替换并且写入到modulestring里面
				ReplacePara(module_type,module_mode,i,k);
			}
		}
	}
	Store_module_byte();
}
bool Singlemodule_compile::IsMatch(Single_Module::instruction TestInstruction)
{
	for (int m = 0; m < G_InstructionInfoTable[TestInstruction.Opcode].size(); ++m)//遍历i步第k条汇编指令中的第m个参数
	{
		int ParaID = TestInstruction.Operand[m];
		//下面开始进行参数类型判断
		if (ParaID != 0)//不是端口
		{
			if (G_InstructionInfoTable[TestInstruction.Opcode][m].IsInput)//如果参数不需要新建空间，则数据来自连线或者输入的参数
			{
				bool isLinked = allocateinstance->ispara_havastartlink(module_type, ParaID);
				bool isValue = allocateinstance->ispara_havainitvalue(module_type, ParaID);
				ParaType InputType = allocateinstance->get_paratype(module_type, ParaID);
				if (isLinked&&InputType == PARA_LOGIC&&G_InstructionInfoTable[TestInstruction.Opcode][m].Op_Type != ADDR_BYTE)//判断是否连线byte
				{
					return 0;
				}
				if (isLinked&&InputType == PARA_NUM&&G_InstructionInfoTable[TestInstruction.Opcode][m].Op_Type != ADDR_FLOAT)//判断是否连线float
				{
					return 0;
				}
				if (isValue&&InputType == PARA_NUM&&G_InstructionInfoTable[TestInstruction.Opcode][m].Op_Type != VALUE_FLOAT)//判断是否直接float
				{
					return 0;
				}
				if (isValue&&InputType == PARA_LOGIC&&G_InstructionInfoTable[TestInstruction.Opcode][m].Op_Type != VALUE_BYTE)//判断是否直接byte
				{
					return 0;
				}
			}
			else//此参数来自新分配的空间
			{
				continue;
			}
		}
		else//是端口
		{
			if (G_InstructionInfoTable[TestInstruction.Opcode][m].Op_Type==VALUE_BYTE)
			{
				if (allocateinstance->get_porttype(module_type)!=WS_PORT_DOUBLE&&allocateinstance->get_porttype(module_type)!=WS_PORT_SINGLE)
				{
					return 0;
				}
			}
			if (G_InstructionInfoTable[TestInstruction.Opcode][m].Op_Type==ADDR)
			{
				//尝试往下读一个参数
				if (m+1<G_InstructionInfoTable[TestInstruction.Opcode].size())//
				{
					Operand_Type NextType=G_InstructionInfoTable[TestInstruction.Opcode][m].Op_Type;
					if (NextType==VALUE)//这里需要接口类型为
					{

					}
				}
			}
		}
	}
	return 1;
}
void Singlemodule_compile::ReplacePara(int module_type,int module_mode,int ProcedureID,int InstructionID)
{
	for (int m = 0; m < G_InstructionInfoTable[allmodule.Module[module_type]->Mode[module_mode][ProcedureID][InstructionID].Opcode].size(); ++m)//遍历i步第k条汇编指令中的第m个参数
	{
		//下面将对应参数替换并写入modulestring中
		int ParaID = allmodule.Module[module_type]->Mode[module_mode][ProcedureID][InstructionID].Operand[m];
		if (ParaID==0)//如果参数为端口
		{
			vector<BYTE> temp=Port_convertbyte(allocateinstance->get_portvalue(module_type));
			for (int i=0;i<temp.size();++i)
			{
				modulestring+=temp[i];
			}
		}
		else//参数不为端口
		{
			switch (G_InstructionInfoTable[allmodule.Module[module_type]->Mode[module_mode][ProcedureID][InstructionID].Opcode][m].Op_Type)
			{
			case ADDR_BYTE:
			case ADDR_FLOAT://这2种类型的参数替换操作都相同
				{
					if (G_InstructionInfoTable[allmodule.Module[module_type]->Mode[module_mode][ProcedureID][InstructionID].Opcode][m].IsInput)//地址来自连线
					{
						vector<BYTE>temp = Word_convertbyte(allocateinstance->get_module_linkparaaddr(module_type, ParaID));
						for (int i = 0; i < temp.size(); ++i)
						{
							modulestring += temp[i];
						}
					}
					else//地址来自新分配的空间
					{
						vector<BYTE>temp = Word_convertbyte(allocateinstance->get_modulepara_addr(module_type, ParaID));
						for (int i = 0; i < temp.size(); ++i)
						{
							modulestring += temp[i];
						}
					}
					break;
				}
			case VALUE_BYTE:
				{
					stringstream temp;
					temp << allocateinstance->get_paravalue(module_type, ParaID);
					int tempValue;
					temp >> tempValue;//将文本转化为int
					modulestring += Int_convertbyte(tempValue);
					break;
				}
			case VALUE_FLOAT:
				{
					stringstream temp;
					temp << allocateinstance->get_paravalue(module_type, ParaID);
					float tempValue;
					temp >> tempValue;
					vector<BYTE> tempByte = Float_convertbyte(tempValue);
					for (int i = 0; i < tempByte.size(); ++i)
					{
						modulestring += tempByte[i];
					}
					break;
				}
			}
		}

	}
}
void Singlemodule_compile::Store_module_byte()
{
	for (int i = 0; i < modulestring.length(); ++i)
	{
		module_byte.push_back(modulestring[i]);
	}
}
BYTE Singlemodule_compile::Get_textlength(string Text)//需要修改
{
	return Text.length();
}
vector<BYTE> Singlemodule_compile::Port_convertbyte(string port)//需要修改
{
	vector<BYTE> Byte;
	switch (allocateinstance->get_porttype(module_type))
	{
	case WS_PORT_SINGLE:
		{
			int temp;
			if(port=="A")
			{
				temp=5;
			}
			if(port=="B")
			{
				temp=6;
			}
			if(port=="C")
			{
				temp=7;
			}
			if(port=="D")
			{
				temp=8;
			}
			else
			{
				stringstream str;
				str<<port;
				str>>temp;
			}
			Byte.push_back(Int_convertbyte(temp));
			break;
		}
	case WS_PORT_DOUBLE:
		{
			char Now_Port1=port[0];
			Now_Port1=(Now_Port1-65)*2;
			char Now_Port2=port[2]-65;
			int PortNum=Now_Port2+Now_Port1;
			Byte.push_back(Int_convertbyte(PortNum));
			break;
		}
	case WS_VALUE_INT:
		{
			int Port_value;
			stringstream str;
			str<<port;
			str>>Port_value;
			typedef union
			{
				int Num;
				char data_b[4];
			}Int2Byte;
			Int2Byte convert;
			convert.Num=Port_value;
			for(int i=0;i<4;++i)
			{
				Byte.push_back(convert.data_b[i]);
			}
			break;
		}
	case WS_VALUE_DOUBLE://这里应该是float
		{
			stringstream str;
			str<<port;
			float temp;
			str>>temp;
			Byte=Float_convertbyte(temp);
			break;
		}
	case WS_VALUE_BOOL://这里当作输入的string是数字
		{
			stringstream str; 
			str<<port;
			int temp;
			str>>temp;
			Byte.push_back(Int_convertbyte(temp));
			break;
		}
	case WS_VALUE_TEXT://还是有点问题
		{
			break;
		}
	case WS_INPORT://连线输入应该是byte或者float类型的addr
		{
		//不在这里处理
		}
	case WS_FILE_PATH:

	}

	return Byte;
}
vector<BYTE> Singlemodule_compile::Float_convertbyte(float FloatNum)
{
	vector<BYTE> Byte;
	typedef union
	{
		float Num;
		BYTE  data_b[4];
	}FloatYByte;
	FloatYByte temp;
	temp.Num = FloatNum;
	for (int i = 0; i < 4; ++i)
	{
		Byte.push_back(temp.data_b[i]);
	}
	return Byte;
}
vector<BYTE> Singlemodule_compile::Word_convertbyte(WORD WordAddress)
{
	vector<BYTE> Byte;
	typedef union
	{
		WORD Num;
		BYTE  data_b[2];
	}FloatYByte;
	FloatYByte temp;
	temp.Num = WordAddress;
	for (int i = 0; i < 2; ++i)
	{
		Byte.push_back(temp.data_b[i]);
	}
	return Byte;
}
BYTE Singlemodule_compile::Int_convertbyte(int IntNum)
{
	typedef union
	{
		int Num;
		BYTE data_b[4];
	}IntYByte;
	IntYByte temp;
	temp.Num = IntNum;
	return temp.data_b[3];
}