#include "LTPch.h"
#include "LTConfig.h"

LTConfig LTConfig::o_Inst;

LTConfig::LTConfig(void)
:o_JiraProjects(10)
,o_DisplayFilterIncludes(10)
,o_DisplayFilterExcludes(10)
,o_SelectionFilters(10)
,o_LogMacs(10)
{
	s_JiraURL = "https://jira.millenniumit.com";

	char zUserName[UNLEN + 1];
	DWORD dwUserLen = UNLEN + 1;

	GetUserName(zUserName, &dwUserLen);
	s_JiraUser = zUserName;
}

LTConfig::~LTConfig(void)
{
}

void LTConfig::Save()
{

}

void LTConfig::Load()
{

}

void LTConfig::OnPostLoad()
{

}

void LTConfig::StringSet::Set( const char* zValue )
{

}

CString LTConfig::StringSet::Get()
{
	if (a_Strings.size() == 0)
		return "";

	return "";
}

void LTConfig::StringSet::Save( const char* zIniFile )
{

}

void LTConfig::StringSet::Load( const char* zIniFile )
{

}

int LTConfig::StringSet::GetCount()
{
	return (int)a_Strings.size();
}

CString LTConfig::StringSet::GetAt( int iIndex )
{
	if (iIndex < (int)a_Strings.size())
		return "";

	return "";
}

LTConfig::StringSet::StringSet( int iMaxCount )
{

}
