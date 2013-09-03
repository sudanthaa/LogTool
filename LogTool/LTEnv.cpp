#include "LTPch.h"
#include "LTEnv.h"

VEC_ENV LTEnv::vec_Env;

LTEnv::LTEnv(void)
{
}

LTEnv::~LTEnv(void)
{
}

LTEnv* LTEnv::CreateEnv(const char* zBaseDir, const char* zSubDir, const char* zFile)
{
	CString sName;
	sName = zFile;
	sName = sName.Left(strlen(zFile) - 4);

	LTEnv* pEnv = new LTEnv;
	pEnv->s_Folder = zSubDir;
	pEnv->s_Name = sName;
	pEnv->s_FullFile.Format("%s\\%s\\%s", zBaseDir, zSubDir, zFile);
	if (!pEnv->Load())
	{
		delete pEnv;
		return NULL;
	}

	return pEnv;
}

bool LTEnv::Load( const char* zFile /*= NULL*/ )
{
	CString sFile;
	if (zFile == NULL)
		sFile = s_FullFile;

	char zUserName[100];
	char zIP[100];
	char zPassword[100];
	GetPrivateProfileString("CONNECTION:AUTHENTICATION","UserName", "", zUserName, 100, sFile);
	GetPrivateProfileString("CONNECTION","Host", "", zIP, 100, sFile);
	GetPrivateProfileString("MIT","Password", "", zPassword, 100, sFile);

	s_EnvUser = zUserName;
	s_IP = zIP;
	s_Password = zPassword;

	return true;
}

bool LTEnv::Save()
{
	WritePrivateProfileString("CONNECTION:AUTHENTICATION", "UserName", s_EnvUser, s_FullFile);
	WritePrivateProfileString("CONNECTION", "Host", s_IP, s_FullFile);
	WritePrivateProfileString("MIT", "Password", s_Password, s_FullFile);

	return true;
}

LTEnv* LTEnv::FindEnv( const char* zName )
{
	for (unsigned int ui = 0; ui < vec_Env.size(); ui++)
	{
		LTEnv* pEnv = vec_Env[ui];
		if (pEnv->s_Name == zName)
			return pEnv;
	}

	return NULL;
}

void LTEnv::Remove( LTEnv* pEnv )
{
	VEC_ENV_ITR itr = vec_Env.begin();
	for (;itr != vec_Env.end(); itr++)
	{
		if (*itr == pEnv)
		{
			vec_Env.erase(itr);
			break;
		}
	}
	
	remove(pEnv->s_FullFile);
}
