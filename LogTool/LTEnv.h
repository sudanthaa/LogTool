#pragma once

class LTEnv
{
public:
	LTEnv(void);
	~LTEnv(void);

	static LTEnv* CreateEnv(const char* zBaseDir, const char* zSubDir, const char* zFile);
	bool Load(const char* zFile = NULL);
	bool Save();
	bool IsPasswordSet(){ return s_Password.GetLength() > 0; };

	CString  s_IP;
	CString	 s_Password;
	CString	 s_Name;
	CString	 s_EnvUser;
	CString	 s_Folder;
	CString	 s_FullFile;
};
