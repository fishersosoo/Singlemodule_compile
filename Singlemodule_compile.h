#pragma once
#include <vector>
#include <string>
#include "compile_allocatememory.h"
#include "All_Module.h"
#include <sstream>
using namespace std;
class Singlemodule_compile
{
public:
	static All_Module allmodule;
	int module_type;             							  //模块类型
	int module_mode;             							  //模块模式
	vector<BYTE>  module_byte;								  //存储模块模式对应的二进制数组
	vector<int>  default_info;								  //记录该模块的缺少值，default_info 记录该模块缺省值的相对地址，其地址是相对地址，发生这种情况只有出现资源模块中，大小为两个字节
	string  modulestring;									  //表示模块模式对应的字符串
	compile_allocatememory* allocateinstance;				  //一个指向内存分配类的指针
public:														  
	void  Get_modulestring(int module_type, int module_mode); //获取模块模式对应的二进制字符串，存储在成员变量modulestring中，获取的字符串一定要以行为单位进行处理
	void Store_module_byte();                                 //将modulestring的二进制数据按BYTE存储在module_byte中
	string Get_modulelinestring(string S_module_mode);		  //获取模块模式的行字符串，然后在单独进行处理
	vector<BYTE> Word_convertbyte(WORD WordAddress);		  //将word表示的地址值转换成字节（双字节）表示
	vector<BYTE> Float_convertbyte(float FloatNum);			  //将浮点数转换成四个字节表示
	vector<BYTE> Port_convertbyte(string port);				  //将端口和接口值转换成BYTE表示
	BYTE Get_textlength(string  Text);						  //获取文本的长度，一般只在显示文本模块中调用该函数
	BYTE Int_convertbyte(int IntNum);						  //将int截取最低字节
	void ReplacePara(int module_type,int module_mode,int ProcedureID,int InstructionID);//替换模块模式中指定行的参数，并写入modulestring中
	bool IsMatch(Single_Module::instruction TestInstruction); //判断实际参数类型是否和汇编指令期待参数类型一致
public:
	Singlemodule_compile();
	Singlemodule_compile(int type, int mode) :module_type(type), module_mode(mode){};
	~Singlemodule_compile();
};

