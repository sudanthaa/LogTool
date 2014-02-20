#include "LTPch.h"
#include "LTConfig.h"
#include "LTUtils.h"
#include "LTEnv.h"

LTConfig LTConfig::o_Inst;

#define  LT_CFG_GROUP_CUSTOM_ACTION "CUSTOM_ACTION"
#define  LT_MAX_CUSTOM_ACTION_CMD_LEM	20000

LTConfig::LTConfig(void)
:o_JiraProjects("JIRA-PROJECT",10)
,o_DisplayFilterIncludes("DISPLAY-INCLUDE-FILTER", 10)
,o_DisplayFilterExcludes("DISPLAY-EXCLUDE-FILTER",10)
,o_SelectionFilters("SELECTION-FILTER",10)
,o_LogMacs("LOG-MACHINE",10)
{
	s_JiraURL = "https://jira.millenniumit.com";
	b_JiraDoComment = false;
	b_JiraCreateNew = false;
	b_SelectionSame = false;

	char zUserName[UNLEN + 1];
	DWORD dwUserLen = UNLEN + 1;

	GetUserName(zUserName, &dwUserLen);

	CString sUserAppFolder;
	LTUtils::GetUserAppFolder(sUserAppFolder);

	SHCreateDirectoryEx(NULL, sUserAppFolder, NULL);
	Load();
	OnPostLoad();
}

LTConfig::~LTConfig(void)
{
}

void LTConfig::Save()
{
	CString sIniFile = LTUtils::GetAppIniFile();

	o_JiraProjects.Save(sIniFile);
	o_DisplayFilterIncludes.Save(sIniFile);
	o_DisplayFilterExcludes.Save(sIniFile);
	o_SelectionFilters.Save(sIniFile);
	o_LogMacs.Save(sIniFile);

	WritePrivateProfileString("MAIN","JIRA-URL", s_JiraURL, sIniFile);
	WritePrivateProfileString("MAIN","JIRA-TICKET", s_JiraTicket, sIniFile);
	_SaveBool("MAIN","JIRA-NEW-TICKET", b_JiraCreateNew , sIniFile);
	_SaveBool("MAIN","JIRA-COMMENT", b_JiraDoComment, sIniFile);


	CString sVal;
	sVal.Format("%u", a_CustomActions.size());
	WritePrivateProfileString(LT_CFG_GROUP_CUSTOM_ACTION, "COUNT", sVal, sIniFile);
	for (UINT ui = 0; ui < a_CustomActions.size(); ui++)
		a_CustomActions[ui]->Save(sIniFile, ui);
}

void LTConfig::Load()
{
	CString sIniFile = LTUtils::GetAppIniFile();

	char zBuff[100];
	GetPrivateProfileString("MAIN","JIRA-URL", s_JiraURL, zBuff, 100, sIniFile); s_JiraURL = zBuff;
	GetPrivateProfileString("MAIN","JIRA-TICKET", s_JiraTicket, zBuff, 100, sIniFile); s_JiraTicket = zBuff;
	_LoadBool("MAIN","JIRA-NEW-TICKET", b_JiraCreateNew, sIniFile);
	_LoadBool("MAIN","JIRA-COMMENT", b_JiraDoComment, sIniFile);
	
	o_JiraProjects.Load(sIniFile);
	o_DisplayFilterIncludes.Load(sIniFile);
	o_DisplayFilterExcludes.Load(sIniFile);
	o_SelectionFilters.Load(sIniFile);
	o_LogMacs.Load(sIniFile);


	UINT uiCount = GetPrivateProfileInt(LT_CFG_GROUP_CUSTOM_ACTION, "COUNT", 0, sIniFile);
	for (UINT ui = 0; ui < uiCount; ui++)
	{
		CustomAction* pCustomAction = new CustomAction;
		pCustomAction->Load(sIniFile, ui);
		a_CustomActions.push_back(pCustomAction);
	}
}

void LTConfig::OnPostLoad()
{
	if (o_DisplayFilterIncludes.GetCount() == 0)
		o_DisplayFilterIncludes.Set("logs/*.log;corefiles/*");

	if (o_DisplayFilterExcludes.GetCount() == 0)
		o_DisplayFilterExcludes.Set("logs/SSM*");

	if (a_CustomActions.size() == 0)
	{
		a_CustomActions.push_back(new CustomAction("Nora","tar -zcvf nora.tar.gz; scp nora.tar.gz $logenvsshcon:$logenvpath"));
		a_CustomActions.push_back(new CustomAction("LastPageNora","cd nora1; lst=`ls -1 | sort -g | tail -1`; tar -zcvf lastnora.tar.gz nora?/$lst nora1/Configs; scp lastnora.tar.gz $logenvsshcon:$logenvpath"));
		a_CustomActions.push_back(new CustomAction("RptDump","rptdump c rptdump.tar.gz; scp rptdump.tar.gz $logenvsshcon:$logenvpath"));
	}
}

void LTConfig::_SaveBool( const char* zGroup, const char* zParam, bool bVar, const char* zIni )
{
	WritePrivateProfileString(zGroup, zParam, bVar ? "TRUE":"FALSE", zIni);
}

void LTConfig::_LoadBool( const char* zGroup, const char* zParam, bool& bVar, const char* zIni )
{
	char zBuff[100];
	GetPrivateProfileString(zGroup, zParam, bVar ? "TRUE":"FALSE", zBuff, 100, zIni); 
	bVar = (strcmp(zBuff, "TRUE") == 0);
}

void LTConfig::AddCustomAction( CustomAction* pCustomAction )
{
	a_CustomActions.push_back(pCustomAction);
}

void LTConfig::RemoveCustomAction( CustomAction* pCustomAction )
{
	std::vector<CustomAction*>::iterator itr = a_CustomActions.begin();
	for (;itr != a_CustomActions.end(); itr++)
	{
		CustomAction* pCustomActionEx = *itr;
		if (pCustomActionEx == pCustomAction)
		{
			a_CustomActions.erase(itr);
			delete pCustomAction;
			break;
		}
	}
}

void LTConfig::StringSet::Set( const char* zValue )
{	
	dq_Strings.push_back(zValue);
	if (dq_Strings.size() > (unsigned int) i_MaxCount)
		dq_Strings.pop_front();
}

CString LTConfig::StringSet::Get()
{
	if (dq_Strings.size() == 0)
		return "";

	return dq_Strings.front();
}

void LTConfig::StringSet::Save( const char* zIniFile )
{
	CString sCount;
	sCount.Format("%d", (int)dq_Strings.size());
	WritePrivateProfileString(s_CfgName, "COUNT", sCount, zIniFile);

	int iIndex = 0;
	std::deque<CString>::iterator itr = dq_Strings.begin();
	for (; itr != dq_Strings.end(); itr++)
	{
		CString sName;
		sName.Format("VALUE_%d", iIndex++);
		WritePrivateProfileString(s_CfgName, sName, *itr, zIniFile);
	}
}

void LTConfig::StringSet::Load( const char* zIniFile )
{
	char zBuff[100];
	GetPrivateProfileString(s_CfgName, "COUNT", "0", zBuff, 100, zIniFile);

	int iCount = atoi(zBuff);
	for (int i = 0; i < iCount; i++)
	{
		CString sName;
		sName.Format("VALUE_%d", i);
		GetPrivateProfileString(s_CfgName, sName,"",zBuff, 100, zIniFile);

		if (strlen(zBuff) > 0)
			Set(zBuff);
	}
}

int LTConfig::StringSet::GetCount()
{
	return (int)dq_Strings.size();
}

CString LTConfig::StringSet::GetAt( int iIndex )
{
	if (iIndex < (int)dq_Strings.size())
		return dq_Strings.at(iIndex);

	return "";
}

LTConfig::StringSet::StringSet(const char* zCfgName, int iMaxCount )
{
	s_CfgName = zCfgName;
	i_MaxCount = iMaxCount;
}

void LTConfig::CustomAction::Save( const char* zIniFile, UINT uiIndex )
{
	CString sName;
	CString sCommand;
	sName.Format("NAME_%u", uiIndex);
	sCommand.Format("COMMAND_%d", uiIndex);

	WritePrivateProfileString(LT_CFG_GROUP_CUSTOM_ACTION, sName, s_Name, zIniFile);
	WritePrivateProfileString(LT_CFG_GROUP_CUSTOM_ACTION, sCommand, s_Command, zIniFile);
}

void LTConfig::CustomAction::Load( const char* zIniFile, UINT uiIndex )
{
	CString sName;
	CString sCommand;
	sName.Format("NAME_%u", uiIndex);
	sCommand.Format("COMMAND_%d", uiIndex);

	int iCmdBuffLen = LT_MAX_CUSTOM_ACTION_CMD_LEM;
	char zNameBuff[200];
	char* pCmdBuff = new char[iCmdBuffLen];
	GetPrivateProfileString(LT_CFG_GROUP_CUSTOM_ACTION, sName, s_Name, zNameBuff, 200, zIniFile);
	GetPrivateProfileString(LT_CFG_GROUP_CUSTOM_ACTION, sCommand, s_Command, pCmdBuff, iCmdBuffLen, zIniFile); 
	s_Name = zNameBuff;
	s_Command = pCmdBuff;
	delete [] pCmdBuff;
	pCmdBuff = NULL;
}

LTConfig::CustomAction::CustomAction( const char* zName /*= ""*/, const char* zCommand /*= ""*/ )
{
	s_Name = zName;
	s_Command = zCommand;
}

LTConfig::CustomAction::~CustomAction()
{

}
