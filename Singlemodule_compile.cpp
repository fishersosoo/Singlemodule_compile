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
	for (int i = 0; i < allmodule.Module[module_type]->NumOfOp[module_mode]; ++i)//ģ��ģʽ��Ӧi��
	{
		if (allmodule.Module[module_type]->Mode[module_mode][i].size() == 1)//���i��ֻ��һ������ָ��򲻽��м��
		{
			//����ת��
			continue;//��ѯ��һ��
		}
		for (int k = 0; k < allmodule.Module[module_type]->Mode[module_mode][i].size(); ++k)//����i���ĵ�k�����ܵĻ��ָ��
		{
			bool ismatch = 1;
			if (G_InstructionInfoTable[allmodule.Module[module_type]->Mode[module_mode][i][k].Opcode].size() != allmodule.Module[module_type]->Mode[module_mode][i][k].Operand.size())//�������������ƥ��
			{
				ismatch = 0;
				continue;//��ѯ��һ��ָ��
			}
			for (int m = 0; m < G_InstructionInfoTable[allmodule.Module[module_type]->Mode[module_mode][i][k].Opcode].size(); ++m)//����i����k�����ָ���еĵ�m������
			{
				int ParaID = allmodule.Module[module_type]->Mode[module_mode][i][k].Operand[m];
				//���濪ʼ���в��������ж�
				if (ParaID != 0)//���Ƕ˿�
				{
					if (!G_InstructionInfoTable[allmodule.Module[module_type]->Mode[module_mode][i][k].Opcode][m].IsInput)//���������Ҫ�½��ռ�
					{
						bool isLinked = allocateinstance->ispara_havastartlink(module_type, ParaID);
						bool isValue = allocateinstance->ispara_havainitvalue(module_type, ParaID);
						ParaType InputType = allocateinstance->get_paratype(module_type, ParaID);
						if (isLinked&&InputType == PARA_LOGIC&&G_InstructionInfoTable[allmodule.Module[module_type]->Mode[module_mode][i][k].Opcode][m].Op_Type != ADDR_BYTE)//�ж��Ƿ�����byte
						{
							ismatch = 0;
						}
						if (isLinked&&InputType == PARA_NUM&&G_InstructionInfoTable[allmodule.Module[module_type]->Mode[module_mode][i][k].Opcode][m].Op_Type != ADDR_FLOAT)//�ж��Ƿ�����float
						{
							ismatch = 0;
						}
						if (isValue&&InputType == PARA_NUM&&G_InstructionInfoTable[allmodule.Module[module_type]->Mode[module_mode][i][k].Opcode][m].Op_Type != VALUE_FLOAT)//�ж��Ƿ�ֱ��float
						{
							ismatch = 0;
						}
						if (isValue&&InputType == PARA_LOGIC&&G_InstructionInfoTable[allmodule.Module[module_type]->Mode[module_mode][i][k].Opcode][m].Op_Type != VALUE_BYTE)//�ж��Ƿ�ֱ��byte
						{
							ismatch = 0;
						}
					}
				}
				else
				{
					if (!G_InstructionInfoTable[allmodule.Module[module_type]->Mode[module_mode][i][k].Opcode][m].IsInput)//���������Ҫ�½��ռ�
					{
						bool isLinked = allocateinstance->isport_havestartlink(module_type);
						bool isValue = allocateinstance->isport_haveinitvalue(module_type);
						if (isLinked&&G_InstructionInfoTable[allmodule.Module[module_type]->Mode[module_mode][i][k].Opcode][m].Op_Type != ADDR_BYTE)//�ж��Ƿ���Ҫ�˿ڵ�ַ
						{
							ismatch = 0;
						}
						if (isValue&&G_InstructionInfoTable[allmodule.Module[module_type]->Mode[module_mode][i][k].Opcode][m].Op_Type != VALUE_BYTE)//�ж��Ƿ���Ҫ�˿�ֱ��ֵ
						{
							ismatch = 0;
						}
					}
				}
				if (!ismatch)//�����ƥ��,���������²�����ƥ�䳢��
				{
					break;
				}
			}
			if (ismatch)//���ȫ��ƥ��
			{
				//����i���ĵ�k�����ܵĻ��ָ��Ĳ����룬�����滻����д�뵽modulestring����
				for (int m = 0; m < G_InstructionInfoTable[allmodule.Module[module_type]->Mode[module_mode][i][k].Opcode].size(); ++m)//����i����k�����ָ���еĵ�m������
				{
					unsigned short opcodetemp = allmodule.Module[module_type]->Mode[module_mode][i][k].Opcode;
					vector<BYTE>binary_OpCode = Word_convertbyte(opcodetemp);
					for (int i = 0; i < 2; ++i)
					{
						modulestring += binary_OpCode[i];
					}//�����ָ�����ת��Ϊ�����ƣ�д��modulestring
					//���潫��Ӧ�����滻��д��modulestring��
					int ParaID = allmodule.Module[module_type]->Mode[module_mode][i][k].Operand[m];
					switch (G_InstructionInfoTable[allmodule.Module[module_type]->Mode[module_mode][i][k].Opcode][m].Op_Type)
					{
					case ADDR_BYTE://������Ҫ�����Ƿ��Ƕ˿�
					case ADDR_FLOAT://��2�����͵Ĳ�����������ͬ
					{
										if (G_InstructionInfoTable[allmodule.Module[module_type]->Mode[module_mode][i][k].Opcode][m].IsInput)//�Ѿ����ڵĵ�ַ
										{
											vector<BYTE>temp = Word_convertbyte(allocateinstance->get_module_linkparaaddr(module_type, ParaID));
											for (int i = 0; i < temp.size(); ++i)
											{
												modulestring += temp[i];
											}
										}
										else//��Ҫ�½���ַ
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
										   temp >> tempValue;//���ı�ת��Ϊint
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