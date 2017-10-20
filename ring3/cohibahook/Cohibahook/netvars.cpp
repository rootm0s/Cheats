#include "main.h"
CNetworkedVariableManager g_NetworkedVariableManager;

void CNetworkedVariableManager::Init(void)
{
	m_tables.clear();
	m_savedproxy.clear();

	ValveSDK::ClientClass *clientClass = g_Valve.pClient->GetAllClasses(); //g_InterfaceManager->Client()->GetOriginalMethod<GetAllClasses_t>( INDEX_GETALLCLASSES )( g_InterfaceManager->Client()->thisptr() );

	if (!clientClass)
	{
		Base::Debug::LOG("ClientClass was not found");
		return;
	}

	while (clientClass)
	{
		RecvTable *recvTable = clientClass->GetTable();

		m_tables.push_back(recvTable);

		clientClass = clientClass->NextClass();
	}
}

CNetworkedVariableManager::~CNetworkedVariableManager(void)
{
	for (unsigned int i = 0; i < m_savedproxy.size(); i++)
	{
		RecvProp *recvProp = 0;
		GetProp(m_savedproxy[i].szTableName, m_savedproxy[i].szPropName, &recvProp);

		if (!recvProp)
			return;

		recvProp->m_ProxyFn = m_savedproxy[i].SavedProxy;
	}
}

// calls GetProp wrapper to get the absolute offset of the prop
int CNetworkedVariableManager::GetOffset(const char *tableName, const char *propName)
{
	int offset = GetProp(tableName, propName);

	if (!offset)
	{
		printf("Failed to find offset for prop: %s from table: %s", propName, tableName);
		return 0;
	}

	return offset;
}


// calls GetProp wrapper to get prop and sets the proxy of the prop
bool CNetworkedVariableManager::HookProp(const char *tableName, const char *propName, RecvVarProxyFn function)
{
	RecvProp *recvProp = 0;
	GetProp(tableName, propName, &recvProp);

	if (!recvProp)
	{
		Base::Debug::LOG("HookProp failed");
		return false;
	}

	//kolonote:
	//make a list of hooked proxies, then when we eject dll we dont have to worry about proxies that point to a pointer where the pointee doesnt exist
	Oldproxy_t oldproxyinfo;

	strcpy(oldproxyinfo.szTableName, tableName);
	strcpy(oldproxyinfo.szPropName, propName);

	oldproxyinfo.SavedProxy = recvProp->m_ProxyFn;

	m_savedproxy.push_back(oldproxyinfo);

	//kolonote:
	//now we replace without worries
	recvProp->m_ProxyFn = function;

	return true;
}


// wrapper so we can use recursion without too much performance loss
int CNetworkedVariableManager::GetProp(const char *tableName, const char *propName, RecvProp **prop)
{
	RecvTable *recvTable = GetTable(tableName);

	if (!recvTable)
	{
		printf("Failed to find table: %s", tableName);
		return 0;
	}


	int offset = GetProp(recvTable, propName, prop);

	if (!offset)
	{
		printf("Failed to find prop: %s from table: %s", propName, tableName);
		return 0;
	}


	return offset;
}


// uses recursion to return a the relative offset to the given prop and sets the prop param
int CNetworkedVariableManager::GetProp(RecvTable *recvTable, const char *propName, RecvProp **prop)
{
	int extraOffset = 0;

	for (int i = 0; i < recvTable->m_nProps; ++i)
	{
		RecvProp *recvProp = &recvTable->m_pProps[i];


		RecvTable *child = recvProp->m_pDataTable;

		if (child
			&& (child->m_nProps > 0))
		{
			int tmp = GetProp(child, propName, prop);

			if (tmp)
			{
				extraOffset += (recvProp->m_Offset + tmp);
			}
		}


		if (_stricmp(recvProp->m_pVarName, propName))
		{
			continue;
		}


		if (prop)
		{
			*prop = recvProp;
		}

		return (recvProp->m_Offset + extraOffset);
	}

	return extraOffset;
}


RecvTable *CNetworkedVariableManager::GetTable(const char *tableName)
{
	if (m_tables.empty())
	{
		printf("Failed to find table: %s (m_tables is empty)", tableName);

		return 0;
	}


	for each (RecvTable *table in m_tables)
	{
		if (!table)
		{
			continue;
		}


		if (_stricmp(table->m_pNetTableName, tableName) == 0)
		{
			return table;
		}
	}

	return 0;
}
