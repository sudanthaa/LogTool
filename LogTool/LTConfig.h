#pragma once

#include <deque>
#include <vector>


class LTConfig
{
public:
	LTConfig(void);
	~LTConfig(void);

	class StringSet{
	public:
		StringSet(const char* zCfgName, int iMaxCount);

		void	Set(const char* zValue);
		CString	Get();
		void	Save(const char* zIniFile);
		void	Load(const char* zIniFile);

		int		GetCount();
		CString	GetAt(int iIndex);

	private:
		std::deque<CString>	dq_Strings;
		int		i_MaxCount;
		CString s_CfgName;
	};

	class CustomAction
	{
	public:
		CustomAction(const char* zName = "", const char* zCommand = "");
		~CustomAction();
	
		void	Save(const char* zIniFile, UINT uiIndex);
		void	Load(const char* zIniFile, UINT uiIndex);

		CString	s_Command;
		CString s_Name;
	};

	void	Save();
	void	Load();
	
	CString	GetExcludeFilter()	{	return o_DisplayFilterExcludes.Get(); };
	CString	GetIncludeFilter()	{	return o_DisplayFilterIncludes.Get(); };
	CString	GetSelectionFilter()	{	return o_SelectionFilters.Get(); };
	CString	GetLogMachine()		{	return o_LogMacs.Get(); };

	StringSet*	GetExcludeFilterSet()	{	return &o_DisplayFilterExcludes; };
	StringSet*	GetIncludeFilterSet()	{	return &o_DisplayFilterIncludes; };
	StringSet*	GetSelectionFilterSet()	{	return &o_SelectionFilters; };
	StringSet*	GetLogMacSet()			{	return &o_LogMacs; };
	StringSet*	GetJiraProjectSet()		{	return &o_JiraProjects; };

	UINT	GetCustomActionCount()	{	return a_CustomActions.size(); };
	CustomAction*	GetCustomActionAt(UINT ui)	{ return a_CustomActions[ui]; };
	void	AddCustomAction(CustomAction* pCustomAction);
	void	RemoveCustomAction(CustomAction* pCustomAction);
	CustomAction*	FindCustomAction(const char* zCustomAction);

	static LTConfig	o_Inst;

	CString	 s_JiraURL;
	CString	 s_JiraTicketID;

	bool	b_JiraDoComment;
	bool	b_JiraCreateNew;
	bool	b_SelectionSame;

protected:
	void	OnPostLoad();
	void	_LoadBool(const char* zGroup, const char* zParam, bool& bVar, const char* zIni);
	void	_SaveBool(const char* zGroup, const char* zParam, bool bVar, const char* zIni);

	StringSet	o_JiraProjects;
	StringSet	o_DisplayFilterIncludes;
	StringSet	o_DisplayFilterExcludes;
	StringSet	o_SelectionFilters;
	StringSet	o_LogMacs;
	std::vector<CustomAction*>	a_CustomActions;
};
