#include "LTPch.h"
#include "LTConfig.h"
#include "LTUtils.h"

LTConfig LTConfig::o_Inst;

LTConfig::LTConfig(void)
:o_JiraProjects("JIRA-PROJECT",10)
,o_DisplayFilterIncludes("DISPLAY-INCLUDE-FILTER", 10)
,o_DisplayFilterExcludes("DISPLAY-EXCLUDE-FILTER",10)
,o_SelectionFilters("SELECTION-FILTER",10)
,o_LogMacs("LOG-MACHINE",10)
{
	s_JiraURL = "https://jira.millenniumit.com";

	char zUserName[UNLEN + 1];
	DWORD dwUserLen = UNLEN + 1;

	GetUserName(zUserName, &dwUserLen);
	s_JiraUser = zUserName;

	CString sUserAppFolder;
	LTUtils::GetUserAppFolder(sUserAppFolder);

	SHCreateDirectoryEx(NULL, sUserAppFolder, NULL);
	Load();
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

	WritePrivateProfileString("MAIN","JIRA-USER", s_JiraUser, sIniFile);
	WritePrivateProfileString("MAIN","JIRA-URL", s_JiraURL, sIniFile);
}

void LTConfig::Load()
{
	CString sIniFile = LTUtils::GetAppIniFile();

	char zBuff[100];
	GetPrivateProfileString("MAIN","JIRA-USER", s_JiraUser, zBuff, 100, sIniFile); s_JiraUser = zBuff;
	GetPrivateProfileString("MAIN","JIRA-URL", s_JiraURL, zBuff, 100, sIniFile); s_JiraURL = zBuff;
	
	o_JiraProjects.Load(sIniFile);
	o_DisplayFilterIncludes.Load(sIniFile);
	o_DisplayFilterExcludes.Load(sIniFile);
	o_SelectionFilters.Load(sIniFile);
	o_LogMacs.Load(sIniFile);
}

void LTConfig::OnPostLoad()
{

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

	return "";
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
	GetPrivateProfileString(s_CfgName,"COUNT","0",zBuff,100, zIniFile);

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
