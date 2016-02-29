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
	int module_type;             							  //ģ������
	int module_mode;             							  //ģ��ģʽ
	vector<BYTE>  module_byte;								  //�洢ģ��ģʽ��Ӧ�Ķ���������
	vector<int>  default_info;								  //��¼��ģ���ȱ��ֵ��default_info ��¼��ģ��ȱʡֵ����Ե�ַ�����ַ����Ե�ַ�������������ֻ�г�����Դģ���У���СΪ�����ֽ�
	string  modulestring;									  //��ʾģ��ģʽ��Ӧ���ַ���
	compile_allocatememory* allocateinstance;				  //һ��ָ���ڴ�������ָ��
public:														  
	void  Get_modulestring(int module_type, int module_mode); //��ȡģ��ģʽ��Ӧ�Ķ������ַ������洢�ڳ�Ա����modulestring�У���ȡ���ַ���һ��Ҫ����Ϊ��λ���д���
	void Store_module_byte();                                 //��modulestring�Ķ��������ݰ�BYTE�洢��module_byte��
	string Get_modulelinestring(string S_module_mode);		  //��ȡģ��ģʽ�����ַ�����Ȼ���ڵ������д���
	vector<BYTE> Word_convertbyte(WORD WordAddress);		  //��word��ʾ�ĵ�ֵַת�����ֽڣ�˫�ֽڣ���ʾ
	vector<BYTE> Float_convertbyte(float FloatNum);			  //��������ת�����ĸ��ֽڱ�ʾ
	vector<BYTE> Port_convertbyte(string port);				  //���˿ںͽӿ�ֵת����BYTE��ʾ
	BYTE Get_textlength(string  Text);						  //��ȡ�ı��ĳ��ȣ�һ��ֻ����ʾ�ı�ģ���е��øú���
	BYTE Int_convertbyte(int IntNum);						  //��int��ȡ����ֽ�
	void ReplacePara(int module_type,int module_mode,int ProcedureID,int InstructionID);//�滻ģ��ģʽ��ָ���еĲ�������д��modulestring��
	bool IsMatch(Single_Module::instruction TestInstruction); //�ж�ʵ�ʲ��������Ƿ�ͻ��ָ���ڴ���������һ��
public:
	Singlemodule_compile();
	Singlemodule_compile(int type, int mode) :module_type(type), module_mode(mode){};
	~Singlemodule_compile();
};

