//16.2.25

#pragma once

#ifndef ALLOCATE_MEMORY
#define ALLOCATE_MEMORY
#endif

#ifndef DEBUG
#define DEBUG
#endif

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <Windows.h>
#include "logic/logic_Project.h"

using namespace std;

class addr_mapkeyinfo
{
public:
	addr_mapkeyinfo()
	{
		mvi_moduleid = -1;
		mvi_paraid = -1;
	}

	bool operator==(const addr_mapkeyinfo some)
	{
		if (this->mvi_moduleid == some.mvi_moduleid&&this->mvi_paraid == some.mvi_paraid)
		{
			return true;
		}
		return false;
	}
public:
	int mvi_moduleid;
	int mvi_paraid;
};
//��̨��ر���
//ָ��ǰ̨�����Ĺ���ʵ��
extern logic_Project *mvl_curproject;
//ָ����������Ŀ��������Ŀid��ȡ����Ӧ��programʵ��
extern logic_Program* mvl_program;
//���ʺ�̨�ı���
extern map<_IdDataType, logic_Tree*> mvm_programforest;



class compile_allocatememory
{
public:
	compile_allocatememory();
	~compile_allocatememory();
	static compile_allocatememory* Get_memoryinstance();

	//��������
	void Traverse_forest();
	void Deep_traversetree(logic_TreeNode* node);
	//����
	void Allocate_memory();
	//���û�ȡ�����˿�������ʵ��
	int             get_portsize(int moduleid);
	int             get_parasize(int moduleid, int paraid);
	//��ȡ�����˿���Ϣ
	WinSwitcherType get_porttype(int moduleid);
	string          get_portvalue(int moduleid);
	ParaType        get_paratype(int moduleid, int paraid);
	string          get_paravalue(int moduleid, int paraid);
	//��ȡģ��ģʽֵ
	int             get_modulemodevalue(int moduleid);
	//���ж��Ƿ�����ʼ������
	bool            ispara_havastartlink(int moduleid, int paraid);
	bool            ispara_havainitvalue(int moduleid, int paraid);
	//����˿ڵ��������������ͣ������Ȼ�����߽���
	bool            isport_havestartlink(int moduleid);
	bool            isport_haveinitvalue(int moduleid);
	
	//��ȡ����������ʼ�˵���Ϣ
	whPort          get_linkstartinfo(int moduleid, int paraid);
	//����һ���жϲ��������������������Ƿ���ͬ

	//��ȡģ�������ַ
	WORD            get_modulepara_addr(int moduleid, int paraid);
	WORD            get_module_linkparaaddr(int moduleid, int paraid);
    bool Isrootnode(logic_TreeNode *node);
private:
	//��¼�ڴ�������
	int mvi_memoryinfo;
	//��¼������������id�ļ���,�����������ζ���и�ģ���ж˿�
	vector<addr_mapkeyinfo> mvv_totalpara;
	//ģ�����id�������
	map < addr_mapkeyinfo, WORD> mvm_modulepara_mapadddr;
	//��¼����ģ�����Ƶ�ģ��id��ӳ��
	map<string, int> mvm_varnamemapid;
	//��¼ѭ��ģ�����Ƶ�ģ��id��ӳ��
	map<string, int> mvm_fornamemapid;

	static compile_allocatememory* memoryinstance;
};