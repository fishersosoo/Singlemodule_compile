#pragma once
#include <vector>
using namespace std;
enum Operand_Type
{
	ADDR_BYTE = 0,//指向BTYE类型的指针
	ADDR_FLOAT,//指向FLOAT类型的指针
	VALUE_BYTE,//BYTE常量
	VALUE_FLOAT,//FLOAT常量
	ADDR,//非指向数据类型的指针
	VALUE//两个BYTE的常量
};
struct Operand_Info
{
	bool IsInput;//是否通过连线或者直接量得到参数值：如果不是输入的，则表示参数需要分配新的内存空间
	Operand_Type Op_Type;
	Operand_Info(bool IsIn, Operand_Type Type)
	{
		IsInput = IsIn;
		Op_Type = Type;
	}
};
vector<Operand_Info> G_InstructionInfoTable[52];
void Build_InstructionInfoTable()
{
	G_InstructionInfoTable[0x01].push_back(Operand_Info(0, ADDR_BYTE));
	G_InstructionInfoTable[0x01].push_back(Operand_Info(1, VALUE_BYTE));
	G_InstructionInfoTable[0x02].push_back(Operand_Info(0, ADDR_FLOAT));
	G_InstructionInfoTable[0x02].push_back(Operand_Info(1, VALUE_FLOAT));
	G_InstructionInfoTable[0x03].push_back(Operand_Info(0, ADDR_BYTE));
	G_InstructionInfoTable[0x03].push_back(Operand_Info(1, ADDR_BYTE));
	G_InstructionInfoTable[0x04].push_back(Operand_Info(0, ADDR_FLOAT));
	G_InstructionInfoTable[0x04].push_back(Operand_Info(1, ADDR_FLOAT));
	G_InstructionInfoTable[0x05].push_back(Operand_Info(0, ADDR));
	G_InstructionInfoTable[0x05].push_back(Operand_Info(1, ADDR));
	G_InstructionInfoTable[0x05].push_back(Operand_Info(1, VALUE));
	G_InstructionInfoTable[0x06].push_back(Operand_Info(1, ADDR));
	G_InstructionInfoTable[0x06].push_back(Operand_Info(1, ADDR));
	G_InstructionInfoTable[0x06].push_back(Operand_Info(1, VALUE));
	G_InstructionInfoTable[0x07].push_back(Operand_Info(0, ADDR_BYTE));
	G_InstructionInfoTable[0x07].push_back(Operand_Info(1, ADDR_FLOAT));
	G_InstructionInfoTable[0x08].push_back(Operand_Info(0, ADDR_FLOAT));
	G_InstructionInfoTable[0x08].push_back(Operand_Info(1, ADDR_BYTE));
	G_InstructionInfoTable[0x09].push_back(Operand_Info(1, ADDR_FLOAT));
	G_InstructionInfoTable[0x09].push_back(Operand_Info(0, ADDR));
	G_InstructionInfoTable[0x09].push_back(Operand_Info(1, ADDR_BYTE));
	G_InstructionInfoTable[0x0A].push_back(Operand_Info(1, ADDR_FLOAT));
	G_InstructionInfoTable[0x0A].push_back(Operand_Info(1, ADDR_FLOAT));
	G_InstructionInfoTable[0x0B].push_back(Operand_Info(1, ADDR_FLOAT));
	G_InstructionInfoTable[0x0B].push_back(Operand_Info(1, ADDR_FLOAT));
	G_InstructionInfoTable[0x0C].push_back(Operand_Info(1, ADDR_FLOAT));
	G_InstructionInfoTable[0x0C].push_back(Operand_Info(1, ADDR_FLOAT));
	G_InstructionInfoTable[0x0D].push_back(Operand_Info(1, ADDR_FLOAT));
	G_InstructionInfoTable[0x0D].push_back(Operand_Info(1, ADDR_FLOAT));
	G_InstructionInfoTable[0x0E].push_back(Operand_Info(1, ADDR_FLOAT));
	G_InstructionInfoTable[0x0F].push_back(Operand_Info(1, ADDR_FLOAT));
	G_InstructionInfoTable[0x10].push_back(Operand_Info(1, ADDR_FLOAT));
	G_InstructionInfoTable[0x10].push_back(Operand_Info(1, ADDR_FLOAT));
	G_InstructionInfoTable[0x12].push_back(Operand_Info(1, ADDR_FLOAT));
	G_InstructionInfoTable[0x13].push_back(Operand_Info(1, ADDR_FLOAT));
	G_InstructionInfoTable[0x14].push_back(Operand_Info(1, ADDR_FLOAT));
	G_InstructionInfoTable[0x15].push_back(Operand_Info(1, ADDR_FLOAT));
	G_InstructionInfoTable[0x15].push_back(Operand_Info(1, ADDR_FLOAT));
	G_InstructionInfoTable[0x16].push_back(Operand_Info(1, ADDR_FLOAT));
	G_InstructionInfoTable[0x16].push_back(Operand_Info(1, ADDR_FLOAT));
	G_InstructionInfoTable[0x16].push_back(Operand_Info(1, ADDR_FLOAT));
	G_InstructionInfoTable[0x16].push_back(Operand_Info(0, ADDR_BYTE));
	G_InstructionInfoTable[0x17].push_back(Operand_Info(1, ADDR_FLOAT));
	G_InstructionInfoTable[0x17].push_back(Operand_Info(1, ADDR_FLOAT));
	G_InstructionInfoTable[0x17].push_back(Operand_Info(1, ADDR_FLOAT));
	G_InstructionInfoTable[0x17].push_back(Operand_Info(0, ADDR_BYTE));
	G_InstructionInfoTable[0x18].push_back(Operand_Info(1, ADDR_FLOAT));
	G_InstructionInfoTable[0x18].push_back(Operand_Info(1, ADDR_FLOAT));
	G_InstructionInfoTable[0x18].push_back(Operand_Info(0, ADDR_FLOAT));
	G_InstructionInfoTable[0x19].push_back(Operand_Info(1, ADDR_FLOAT));
	G_InstructionInfoTable[0x19].push_back(Operand_Info(1, ADDR_FLOAT));
	G_InstructionInfoTable[0x19].push_back(Operand_Info(0, ADDR_BYTE));
	G_InstructionInfoTable[0x1A].push_back(Operand_Info(1, ADDR_FLOAT));
	G_InstructionInfoTable[0x1A].push_back(Operand_Info(1, ADDR_FLOAT));
	G_InstructionInfoTable[0x1A].push_back(Operand_Info(0, ADDR_BYTE));
	G_InstructionInfoTable[0x1B].push_back(Operand_Info(1, ADDR_FLOAT));
	G_InstructionInfoTable[0x1B].push_back(Operand_Info(1, ADDR_FLOAT));
	G_InstructionInfoTable[0x1B].push_back(Operand_Info(0, ADDR_BYTE));
	G_InstructionInfoTable[0x1C].push_back(Operand_Info(1, ADDR_FLOAT));
	G_InstructionInfoTable[0x1C].push_back(Operand_Info(1, ADDR_FLOAT));
	G_InstructionInfoTable[0x1C].push_back(Operand_Info(0, ADDR_BYTE));
	G_InstructionInfoTable[0x1D].push_back(Operand_Info(1, ADDR_FLOAT));
	G_InstructionInfoTable[0x1D].push_back(Operand_Info(1, ADDR_FLOAT));
	G_InstructionInfoTable[0x1D].push_back(Operand_Info(0, ADDR_BYTE));
	G_InstructionInfoTable[0x1E].push_back(Operand_Info(1, ADDR_FLOAT));
	G_InstructionInfoTable[0x1E].push_back(Operand_Info(1, ADDR_FLOAT));
	G_InstructionInfoTable[0x1E].push_back(Operand_Info(0, ADDR_BYTE));
	G_InstructionInfoTable[0x1F].push_back(Operand_Info(1, ADDR_FLOAT));
	G_InstructionInfoTable[0x1F].push_back(Operand_Info(1, ADDR_FLOAT));
	G_InstructionInfoTable[0x1F].push_back(Operand_Info(0, ADDR_BYTE));
	G_InstructionInfoTable[0x20].push_back(Operand_Info(1, ADDR_BYTE));
	G_InstructionInfoTable[0x20].push_back(Operand_Info(1, ADDR_BYTE));
	G_InstructionInfoTable[0x21].push_back(Operand_Info(1, ADDR_BYTE));
	G_InstructionInfoTable[0x21].push_back(Operand_Info(1, ADDR_BYTE));
	G_InstructionInfoTable[0x22].push_back(Operand_Info(1, ADDR_BYTE));
	G_InstructionInfoTable[0x23].push_back(Operand_Info(1, ADDR_BYTE));
	G_InstructionInfoTable[0x23].push_back(Operand_Info(1, ADDR_BYTE));
	G_InstructionInfoTable[0x24].push_back(Operand_Info(1, ADDR));
	G_InstructionInfoTable[0x25].push_back(Operand_Info(1, ADDR_BYTE));
	G_InstructionInfoTable[0x25].push_back(Operand_Info(1, ADDR));
	G_InstructionInfoTable[0x26].push_back(Operand_Info(1, ADDR_BYTE));
	G_InstructionInfoTable[0x26].push_back(Operand_Info(1, ADDR));
	G_InstructionInfoTable[0x27].push_back(Operand_Info(1, ADDR_FLOAT));
	G_InstructionInfoTable[0x27].push_back(Operand_Info(1, ADDR));
	G_InstructionInfoTable[0x28].push_back(Operand_Info(1, ADDR_FLOAT));
	G_InstructionInfoTable[0x29].push_back(Operand_Info(1, VALUE_BYTE));
	G_InstructionInfoTable[0x29].push_back(Operand_Info(1, ADDR));
	G_InstructionInfoTable[0x2A].push_back(Operand_Info(1, VALUE_BYTE));
	G_InstructionInfoTable[0x2C].push_back(Operand_Info(1, ADDR_FLOAT));
	G_InstructionInfoTable[0x2C].push_back(Operand_Info(0, ADDR_FLOAT));
	G_InstructionInfoTable[0x2D].push_back(Operand_Info(1, ADDR_FLOAT));
	G_InstructionInfoTable[0x30].push_back(Operand_Info(0, ADDR_BYTE));
	G_InstructionInfoTable[0x30].push_back(Operand_Info(1, VALUE_BYTE));
	G_InstructionInfoTable[0x30].push_back(Operand_Info(1, VALUE_BYTE));
	G_InstructionInfoTable[0x30].push_back(Operand_Info(1, VALUE_BYTE));
	G_InstructionInfoTable[0x30].push_back(Operand_Info(0, ADDR));

	G_InstructionInfoTable[0x31].push_back(Operand_Info(0, ADDR_BYTE));
	G_InstructionInfoTable[0x31].push_back(Operand_Info(1, VALUE_BYTE));
	G_InstructionInfoTable[0x31].push_back(Operand_Info(1, VALUE_BYTE));
	G_InstructionInfoTable[0x31].push_back(Operand_Info(1, VALUE_BYTE));
	G_InstructionInfoTable[0x31].push_back(Operand_Info(0, ADDR));

	G_InstructionInfoTable[0x32].push_back(Operand_Info(1, VALUE_BYTE));
	G_InstructionInfoTable[0x32].push_back(Operand_Info(1, VALUE_BYTE));
	G_InstructionInfoTable[0x32].push_back(Operand_Info(1, VALUE_BYTE));
	G_InstructionInfoTable[0x32].push_back(Operand_Info(0, ADDR));

	G_InstructionInfoTable[0x33].push_back(Operand_Info(1, VALUE_BYTE));
	G_InstructionInfoTable[0x33].push_back(Operand_Info(1, VALUE_BYTE));
	G_InstructionInfoTable[0x33].push_back(Operand_Info(1, VALUE_BYTE));
	G_InstructionInfoTable[0x33].push_back(Operand_Info(0, ADDR));
}