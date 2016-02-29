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
			//�����ָ�������ת��Ϊ�����ƣ�д��modulestring
			unsigned short opcodetemp = allmodule.Module[module_type]->Mode[module_mode][i][0].Opcode;
			vector<BYTE>binary_OpCode = Word_convertbyte(opcodetemp);
			for (int i = 0; i < 2; ++i)
			{
				modulestring += binary_OpCode[i];
			}
			//����i���ĵ�1�����ָ��Ĳ��������滻����д�뵽modulestring����
			ReplacePara(module_type,module_mode,i,0);
			continue;//��ѯ��һ��
		}
		for (int k = 0; k < allmodule.Module[module_type]->Mode[module_mode][i].size(); ++k)//����i���ĵ�k�����ܵĻ��ָ��
		{
			bool ismatch;
			if (G_InstructionInfoTable[allmodule.Module[module_type]->Mode[module_mode][i][k].Opcode].size() != allmodule.Module[module_type]->Mode[module_mode][i][k].Operand.size())//�������������ƥ��
			{
				ismatch = 0;
				continue;//��ѯ��һ��ָ��
			}
			ismatch=IsMatch(allmodule.Module[module_type]->Mode[module_mode][i][k]);
			if (ismatch)//���ȫ��ƥ��
			{
				//�����ָ�������ת��Ϊ�����ƣ�д��modulestring
				unsigned short opcodetemp = allmodule.Module[module_type]->Mode[module_mode][i][k].Opcode;
				vector<BYTE>binary_OpCode = Word_convertbyte(opcodetemp);
				for (int i = 0; i < 2; ++i)
				{
					modulestring += binary_OpCode[i];
				}
				//����i���ĵ�k�����ָ��Ĳ��������滻����д�뵽modulestring����
				ReplacePara(module_type,module_mode,i,k);
			}
		}
	}
	Store_module_byte();
}
bool Singlemodule_compile::IsMatch(Single_Module::instruction TestInstruction)
{
	for (int m = 0; m < G_InstructionInfoTable[TestInstruction.Opcode].size(); ++m)//����i����k�����ָ���еĵ�m������
	{
		int ParaID = TestInstruction.Operand[m];
		//���濪ʼ���в��������ж�
		if (ParaID != 0)//���Ƕ˿�
		{
			if (G_InstructionInfoTable[TestInstruction.Opcode][m].IsInput)//�����������Ҫ�½��ռ䣬�������������߻�������Ĳ���
			{
				bool isLinked = allocateinstance->ispara_havastartlink(module_type, ParaID);
				bool isValue = allocateinstance->ispara_havainitvalue(module_type, ParaID);
				ParaType InputType = allocateinstance->get_paratype(module_type, ParaID);
				if (isLinked&&InputType == PARA_LOGIC&&G_InstructionInfoTable[TestInstruction.Opcode][m].Op_Type != ADDR_BYTE)//�ж��Ƿ�����byte
				{
					return 0;
				}
				if (isLinked&&InputType == PARA_NUM&&G_InstructionInfoTable[TestInstruction.Opcode][m].Op_Type != ADDR_FLOAT)//�ж��Ƿ�����float
				{
					return 0;
				}
				if (isValue&&InputType == PARA_NUM&&G_InstructionInfoTable[TestInstruction.Opcode][m].Op_Type != VALUE_FLOAT)//�ж��Ƿ�ֱ��float
				{
					return 0;
				}
				if (isValue&&InputType == PARA_LOGIC&&G_InstructionInfoTable[TestInstruction.Opcode][m].Op_Type != VALUE_BYTE)//�ж��Ƿ�ֱ��byte
				{
					return 0;
				}
			}
			else//�˲��������·���Ŀռ�
			{
				continue;
			}
		}
		else//�Ƕ˿�
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
				//�������¶�һ������
				if (m+1<G_InstructionInfoTable[TestInstruction.Opcode].size())//
				{
					Operand_Type NextType=G_InstructionInfoTable[TestInstruction.Opcode][m].Op_Type;
					if (NextType==VALUE)//������Ҫ�ӿ�����Ϊ
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
	for (int m = 0; m < G_InstructionInfoTable[allmodule.Module[module_type]->Mode[module_mode][ProcedureID][InstructionID].Opcode].size(); ++m)//����i����k�����ָ���еĵ�m������
	{
		//���潫��Ӧ�����滻��д��modulestring��
		int ParaID = allmodule.Module[module_type]->Mode[module_mode][ProcedureID][InstructionID].Operand[m];
		if (ParaID==0)//�������Ϊ�˿�
		{
			vector<BYTE> temp=Port_convertbyte(allocateinstance->get_portvalue(module_type));
			for (int i=0;i<temp.size();++i)
			{
				modulestring+=temp[i];
			}
		}
		else//������Ϊ�˿�
		{
			switch (G_InstructionInfoTable[allmodule.Module[module_type]->Mode[module_mode][ProcedureID][InstructionID].Opcode][m].Op_Type)
			{
			case ADDR_BYTE:
			case ADDR_FLOAT://��2�����͵Ĳ����滻��������ͬ
				{
					if (G_InstructionInfoTable[allmodule.Module[module_type]->Mode[module_mode][ProcedureID][InstructionID].Opcode][m].IsInput)//��ַ��������
					{
						vector<BYTE>temp = Word_convertbyte(allocateinstance->get_module_linkparaaddr(module_type, ParaID));
						for (int i = 0; i < temp.size(); ++i)
						{
							modulestring += temp[i];
						}
					}
					else//��ַ�����·���Ŀռ�
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
					temp >> tempValue;//���ı�ת��Ϊint
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
BYTE Singlemodule_compile::Get_textlength(string Text)//��Ҫ�޸�
{
	return Text.length();
}
vector<BYTE> Singlemodule_compile::Port_convertbyte(string port)//��Ҫ�޸�
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
	case WS_VALUE_DOUBLE://����Ӧ����float
		{
			stringstream str;
			str<<port;
			float temp;
			str>>temp;
			Byte=Float_convertbyte(temp);
			break;
		}
	case WS_VALUE_BOOL://���ﵱ�������string������
		{
			stringstream str; 
			str<<port;
			int temp;
			str>>temp;
			Byte.push_back(Int_convertbyte(temp));
			break;
		}
	case WS_VALUE_TEXT://�����е�����
		{
			break;
		}
	case WS_INPORT://��������Ӧ����byte����float���͵�addr
		{
		//�������ﴦ��
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