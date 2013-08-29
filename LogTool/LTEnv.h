#pragma once

#include <vector>

class LTEnv;
typedef std::vector<LTEnv*>  VEC_ENV;
typedef VEC_ENV::iterator	  VEC_ENV_ITR;

class LTEnv
{
public:
	LTEnv(void);
	~LTEnv(void);

	static LTEnv* CreateEnv(const char* zBaseDir, const char* zSubDir, const char* zFile);
	static LTEnv* FindEnv(const char* zUser);
	bool Load(const char* zFile = NULL);
	bool Save();
	bool IsPasswordSet(){ return s_Password.GetLength() > 0; };

	static VEC_ENV vec_Env;

	CString  s_IP;
	CString	 s_Password;
	CString	 s_Name;
	CString	 s_EnvUser;
	CString	 s_Folder;
	CString	 s_FullFile;
};
