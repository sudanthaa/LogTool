#include "LTPch.h"
#include "LTEnv.h"

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
	GetPrivateProfileString("CONNECTION:AUTHENTICATION","UserName", "", zUserName, 100, sFile);
	GetPrivateProfileString("CONNECTION","Host", "", zIP, 100, sFile);

	s_EnvUser = zUserName;
	s_IP = zIP;

	return true;
}

bool LTEnv::Save()
{
	return true;
}
