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
			//进行转换
			continue;//查询下一步
		}
		for (int k = 0; k < allmodule.Module[module_type]->Mode[module_mode][i].size(); ++k)//遍历i步的第k条可能的汇编指令
		{
			bool ismatch = 1;
			if (G_InstructionInfoTable[allmodule.Module[module_type]->Mode[module_mode][i][k].Opcode].size() != allmodule.Module[module_type]->Mode[module_mode][i][k].Operand.size())//如果参数数量不匹配
			{
				ismatch = 0;
				continue;//查询下一条指令
			}
			for (int m = 0; m < G_InstructionInfoTable[allmodule.Module[module_type]->Mode[module_mode][i][k].Opcode].size(); ++m)//遍历i步第k条汇编指令中的第m个参数
			{
				int ParaID = allmodule.Module[module_type]->Mode[module_mode][i][k].Operand[m];
				//下面开始进行参数类型判断
				if (ParaID != 0)//不是端口
				{
					if (!G_InstructionInfoTable[allmodule.Module[module_type]->Mode[module_mode][i][k].Opcode][m].IsInput)//如果参数需要新建空间
					{
						bool isLinked = allocateinstance->ispara_havastartlink(module_type, ParaID);
						bool isValue = allocateinstance->ispara_havainitvalue(module_type, ParaID);
						ParaType InputType = allocateinstance->get_paratype(module_type, ParaID);
						if (isLinked&&InputType == PARA_LOGIC&&G_InstructionInfoTable[allmodule.Module[module_type]->Mode[module_mode][i][k].Opcode][m].Op_Type != ADDR_BYTE)//判断是否连线byte
						{
							ismatch = 0;
						}
						if (isLinked&&InputType == PARA_NUM&&G_InstructionInfoTable[allmodule.Module[module_type]->Mode[module_mode][i][k].Opcode][m].Op_Type != ADDR_FLOAT)//判断是否连线float
						{
							ismatch = 0;
						}
						if (isValue&&InputType == PARA_NUM&&G_InstructionInfoTable[allmodule.Module[module_type]->Mode[module_mode][i][k].Opcode][m].Op_Type != VALUE_FLOAT)//判断是否直接float
						{
							ismatch = 0;
						}
						if (isValue&&InputType == PARA_LOGIC&&G_InstructionInfoTable[allmodule.Module[module_type]->Mode[module_mode][i][k].Opcode][m].Op_Type != VALUE_BYTE)//判断是否直接byte
						{
							ismatch = 0;
						}
					}
				}
				else
				{
					if (!G_InstructionInfoTable[allmodule.Module[module_type]->Mode[module_mode][i][k].Opcode][m].IsInput)//如果参数需要新建空间
					{
						bool isLinked = allocateinstance->isport_havestartlink(module_type);
						bool isValue = allocateinstance->isport_haveinitvalue(module_type);
						if (isLinked&&G_InstructionInfoTable[allmodule.Module[module_type]->Mode[module_mode][i][k].Opcode][m].Op_Type != ADDR_BYTE)//判断是否需要端口地址
						{
							ismatch = 0;
						}
						if (isValue&&G_InstructionInfoTable[allmodule.Module[module_type]->Mode[module_mode][i][k].Opcode][m].Op_Type != VALUE_BYTE)//判断是否需要端口直接值
						{
							ismatch = 0;
						}
					}
				}
				if (!ismatch)//如果不匹配,不进行余下参数的匹配尝试
				{
					break;
				}
			}
			if (ismatch)//如果全部匹配
			{
				//遍历i步的第k条可能的汇编指令的操作码，参数替换并且写入到modulestring里面
				for (int m = 0; m < G_InstructionInfoTable[allmodule.Module[module_type]->Mode[module_mode][i][k].Opcode].size(); ++m)//遍历i步第k条汇编指令中的第m个参数
				{
					unsigned short opcodetemp = allmodule.Module[module_type]->Mode[module_mode][i][k].Opcode;
					vector<BYTE>binary_OpCode = Word_convertbyte(opcodetemp);
					for (int i = 0; i < 2; ++i)
					{
						modulestring += binary_OpCode[i];
					}//将汇编指令代码转换为二进制，写入modulestring
					//下面将对应参数替换并写入modulestring中
					int ParaID = allmodule.Module[module_type]->Mode[module_mode][i][k].Operand[m];
					switch (G_InstructionInfoTable[allmodule.Module[module_type]->Mode[module_mode][i][k].Opcode][m].Op_Type)
					{
					case ADDR_BYTE://好像不需要考虑是否是端口
					case ADDR_FLOAT://这2种类型的参数操作都相同
					{
										if (G_InstructionInfoTable[allmodule.Module[module_type]->Mode[module_mode][i][k].Opcode][m].IsInput)//已经存在的地址
										{
											vector<BYTE>temp = Word_convertbyte(allocateinstance->get_module_linkparaaddr(module_type, ParaID));
											for (int i = 0; i < temp.size(); ++i)
											{
												modulestring += temp[i];
											}
										}
										else//需要新建地址
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
									   if (ParaID == 0)
									   {

									   }
									   else
									   {
										   stringstream temp;
										   temp << allocateinstance->get_paravalue(module_type, ParaID);
										   int tempValue;
										   temp >> tempValue;//将文本转化为int
										   modulestring += Int_convertbyte(tempValue);
									   }
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
	}
	Store_module_byte();
}
void Singlemodule_compile::Store_module_byte()
{
	for (int i = 0; i < modulestring.length(); ++i)
	{
		module_byte.push_back(modulestring[i]);
	}
}
BYTE Singlemodule_compile::Get_textlength(string Text)
{
	return Text.length();
}
vector<BYTE> Singlemodule_compile::Port_convertbyte(string port)
{
	vector<BYTE> Byte;
	for (int i = 0; i < port.length(); ++i)
	{
		unsigned char temp = (unsigned char)port[i];
		Byte.push_back(temp);
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