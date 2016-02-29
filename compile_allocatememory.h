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
//后台相关变量
//指向前台创建的工程实例
extern logic_Project *mvl_curproject;
//指向待编译的项目，根据项目id获取其相应的program实例
extern logic_Program* mvl_program;
//访问后台的变量
extern map<_IdDataType, logic_Tree*> mvm_programforest;



class compile_allocatememory
{
public:
	compile_allocatememory();
	~compile_allocatememory();
	static compile_allocatememory* Get_memoryinstance();

	//遍历操作
	void Traverse_forest();
	void Deep_traversetree(logic_TreeNode* node);
	//分配
	void Allocate_memory();
	//调用获取参数端口类型来实现
	int             get_portsize(int moduleid);
	int             get_parasize(int moduleid, int paraid);
	//获取参数端口信息
	WinSwitcherType get_porttype(int moduleid);
	string          get_portvalue(int moduleid);
	ParaType        get_paratype(int moduleid, int paraid);
	string          get_paravalue(int moduleid, int paraid);
	//获取模块模式值
	int             get_modulemodevalue(int moduleid);
	//先判断是否有起始端连线
	bool            ispara_havastartlink(int moduleid, int paraid);
	bool            ispara_havainitvalue(int moduleid, int paraid);
	//如果端口的类型是输入类型，则其必然有连线进入
	bool            isport_havestartlink(int moduleid);
	bool            isport_haveinitvalue(int moduleid);
	
	//获取参数连线起始端的信息
	whPort          get_linkstartinfo(int moduleid, int paraid);
	//增加一个判断参数连线两段数据类型是否相同

	//获取模块参数地址
	WORD            get_modulepara_addr(int moduleid, int paraid);
	WORD            get_module_linkparaaddr(int moduleid, int paraid);
    bool Isrootnode(logic_TreeNode *node);
private:
	//记录内存分配情况
	int mvi_memoryinfo;
	//记录所有需分配参数id的集合,如果有零则意味着有该模块有端口
	vector<addr_mapkeyinfo> mvv_totalpara;
	//模块参数id分配情况
	map < addr_mapkeyinfo, WORD> mvm_modulepara_mapadddr;
	//记录变量模块名称到模块id的映射
	map<string, int> mvm_varnamemapid;
	//记录循环模块名称到模块id的映射
	map<string, int> mvm_fornamemapid;

	static compile_allocatememory* memoryinstance;
};