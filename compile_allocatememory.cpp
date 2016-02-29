#include "compile_allocatememory.h"
#include <assert.h>

compile_allocatememory* memoryinstance = NULL;

compile_allocatememory* compile_allocatememory::Get_memoryinstance()
{
	if (memoryinstance == NULL)
	{
		return new compile_allocatememory;
	}
	return memoryinstance;
}

compile_allocatememory::compile_allocatememory()
{
	mvi_memoryinfo = 0;
}


bool compile_allocatememory::Isrootnode(logic_TreeNode *node)
{
	for (auto i = mvm_programforest.begin(); i != mvm_programforest.end(); ++i)
	{
		logic_TreeNode* lvl_tempnode = i->second->getRoot();
		if (node == lvl_tempnode)
		{
			return true;
		}
	}
	return false;
}

//�ص�
void compile_allocatememory::Traverse_forest()
{
	for (auto i = mvm_programforest.begin(); i != mvm_programforest.end(); ++i)
	{
		logic_Tree* lvl_temptree = i->second;
		logic_TreeNode* lvl_tempnode = lvl_temptree->getRoot();
		Deep_traversetree(lvl_tempnode);
	}
}

void compile_allocatememory::Deep_traversetree(logic_TreeNode* node)
{
	//������ʲô����������ڵ㶼���账��
	if (!Isrootnode(node))
	{
		int lvi_moduleid = node->getID();
		addr_mapkeyinfo lva_tempmapkey;
		lva_tempmapkey.mvi_moduleid = lvi_moduleid;
		//��û�ж˿���������䣬������䣬�����Ӧ�Ĳ���idΪ0
		if (get_porttype(lvi_moduleid) != 0)
		{
			lva_tempmapkey.mvi_paraid = 0;
			mvv_totalpara.push_back(lva_tempmapkey);
		}
		int lvi_mode = get_modulemodevalue(lvi_moduleid);
		vector<int> lvv_paraidset = mvl_program->getModuleParamsIds(lvi_moduleid, lvi_mode);
		for (auto i = lvv_paraidset.begin(); i != lvv_paraidset.end(); ++i)
		{
			lva_tempmapkey.mvi_paraid = *i;
			mvv_totalpara.push_back(lva_tempmapkey);
		}//������һ��ģ��Ķ˿ںͲ���

		//������ģ����д�����ʵҲ�Ǻܼ򵥵Ĵ���
	}

	//��ȱ���
	for (auto i = node->mvvu_Children.begin(); i != node->mvvu_Children.end(); ++i)
	{
		Deep_traversetree(*i);
	}

	//����
	return;
}

int compile_allocatememory::get_modulemodevalue(int moduleid)
{
	return mvl_program->getModuleModeValue(moduleid);
}



WinSwitcherType compile_allocatememory::get_porttype(int moduleid)
{
	return mvl_program->getModuleWinSwType(moduleid, get_modulemodevalue(moduleid));
}


ParaType  compile_allocatememory::get_paratype(int moduleid, int paraid)
{
	assert(mvl_program->searchModule(moduleid) != NULL);
	logic_BasicModule* lvl_tempmodule = mvl_program->searchModule(moduleid);
	vector<logic_BasicPara*> lvl_temppara = lvl_tempmodule->getCurParaList();
	auto i=lvl_temppara.begin();
	for (; i != lvl_temppara.end(); ++i)
	{
		if ((*i)->mvi_ParaID == paraid)
		{
			return (*i)->getParaType();
		}
	}

	assert(i != lvl_temppara.end());
}



string compile_allocatememory::get_portvalue(int moduleid)
{
	return mvl_program->getModuleWinSwValue(moduleid);
}



string compile_allocatememory::get_paravalue(int moduleid, int paraid)
{
	return mvl_program->getModulePara(get_paratype(moduleid, paraid), moduleid, paraid);
}

//�ж��Ƿ������ߣ����Ƿ������ߵ��ն�
bool compile_allocatememory::ispara_havastartlink(int moduleid, int paraid)
{
	assert(mvl_program->searchModule(moduleid) != NULL);
	logic_BasicModule* lvl_tempmodule = mvl_program->searchModule(moduleid);
	vector<logic_BasicPara*> lvl_temppara = lvl_tempmodule->getCurParaList();
	auto i = lvl_temppara.begin();
	for (; i != lvl_temppara.end(); ++i)
	{
		if ((*i)->mvi_ParaID == paraid)
		{
			if ((*i)->getParaIOType == PARA_OUT)
			{
				return false;
			}
			if (!(*i)->mvb_IsInport)
			{
				return false;
			}
			else
			{
				return true;
			}
		}//������һ��if
	}//����for

}

bool compile_allocatememory::ispara_havainitvalue(int moduleid, int paraid)
{
	if (get_paravalue(moduleid, paraid).size() == 0)
	{
		return true;
	}
	return false;
}

bool compile_allocatememory::isport_havestartlink(int moduleid)
{
	if (get_porttype(moduleid) == WS_INPORT)
	{
		return true;
	}
	return false;
}

bool compile_allocatememory::isport_haveinitvalue(int moduleid)
{
	if (get_portvalue(moduleid).size())
	{
		return true;
	}
	return false;
}

//һ������֪��������ʼ�˵�������øú���
whPort compile_allocatememory::get_linkstartinfo(int moduleid, int paraid)
{
	assert(ispara_havastartlink(moduleid, paraid));
	whPort lvw_tempendport;
	lvw_tempendport.moduleId = moduleid;
	lvw_tempendport.paraId = paraid;
	mvl_program->getLinkstartwhport(lvw_tempendport);
}


int compile_allocatememory::get_portsize(int moduleid)
{
	WinSwitcherType lvw_tempporttype = get_porttype(moduleid);
	int lvi_size = 0;
	switch (lvw_tempporttype)
	{
	case WS_NONE:
		lvi_size=0;
	    break;
	case WS_NAME:
		//����ģ����������ڴ��в�ʹ�ã���������䣬ֻ�����ϲ����ʹ��
		lvi_size = 0;
		break;
	case WS_PORT_SINGLE:
	    lvi_size=1;
		break;
	case WS_PORT_DOUBLE:
		lvi_size = 1;
		break;
	case WS_VALUE_TEXT:
		//�ı�������Դ���ͣ���Ҫ�����ֽڴ�����Դ�ĳ����ַ
		lvi_size = 2;
		break;
	case WS_VALUE_INT:
		lvi_size = 4;
		break;
	case WS_VALUE_DOUBLE:
		lvi_size = 4;
		break;
	case WS_VALUE_BOOL:
		lvi_size = 1;
		break;
	case WS_INPORT:
		//��Ҫ���⴦��
		break;
	default:
		lvi_size = 0;
		break;
	}

	return lvi_size;
}


int compile_allocatememory::get_parasize(int moduleid, int paraid)
{
	ParaType lvp_paratype = get_paratype(moduleid, paraid);
	int lvi_size;
	switch (lvp_paratype)
	{
	case PARA_LOGIC:
		lvi_size = 1;
		break;
	case PARA_NUM:
		lvi_size = 4;
		break;
	case PARA_TEXT:
		//������Դ
		lvi_size = 2;
		break;
	default:
		lvi_size = 0;
		break;
			
	}
	return lvi_size;
}

//�ص�
void compile_allocatememory::Allocate_memory()
{
	for (auto i = mvv_totalpara.begin(); i != mvv_totalpara.end(); i++)
	{
		int lvi_moduleid = (*i).mvi_moduleid;
		int lvi_paraid = (*i).mvi_paraid;
		addr_mapkeyinfo lva_tempkey=*i;
		WORD lvw_addr;
		//�Զ˿ڽ��д���
		if (lvi_paraid == 0)
		{
			lvw_addr = mvi_memoryinfo;
			mvi_memoryinfo += get_portsize(lvi_moduleid);
			mvm_modulepara_mapadddr[lva_tempkey] = lvw_addr;

		}
		//�Բ������д���
		else
		{
			lvw_addr = mvi_memoryinfo;
			mvi_memoryinfo += get_parasize(lvi_moduleid, lvi_paraid);
			mvm_modulepara_mapadddr[lva_tempkey] = lvw_addr;

		}
	}
}

WORD compile_allocatememory::get_modulepara_addr(int moduleid, int paraid)
{
	addr_mapkeyinfo lva_tempaddr;
	lva_tempaddr.mvi_moduleid = moduleid;
	lva_tempaddr.mvi_paraid = paraid;
	assert(mvm_modulepara_mapadddr.find(lva_tempaddr) != mvm_modulepara_mapadddr.end());
	return mvm_modulepara_mapadddr[lva_tempaddr];
}

WORD compile_allocatememory::get_module_linkparaaddr(int moduleid, int paraid)
{
	//���ģ�����û����ʼ�����ߣ�������ϵ�
	assert(ispara_havastartlink(moduleid, paraid));
	whPort lvw_whport = get_linkstartinfo(moduleid, paraid);
	int lvi_linkmoduleid = lvw_whport.moduleId;
	int lvi_linkparaid = lvw_whport.paraId;
	return get_modulepara_addr(lvi_linkmoduleid, lvi_linkparaid);
}