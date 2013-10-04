#include "LTPch.h"
#include "LTUtils.h"

#define  LT_INI_FILE_NAME "LogTool.ini"

LTUtils::LTUtils(void)
{
}

LTUtils::~LTUtils(void)
{
}

CString LTUtils::GetAppPath()
{
	TCHAR zModule[_MAX_PATH];
	GetModuleFileName(NULL, zModule, _MAX_PATH);

	CString sExeName = zModule;
	int iPos = sExeName.ReverseFind('\\');

	CString sAppPath = sExeName.Left(iPos); 
	return sAppPath;
}

CString LTUtils::GetUserLocal()
{
	return "";
}

bool LTUtils::GetXShellSeesionFolder(CString& sFolder)
{
	char szAppData[MAX_PATH];
	if (! SUCCEEDED( SHGetFolderPath( NULL, CSIDL_APPDATA, NULL, 0, szAppData ) ) )
		return false;

	//"NetSarang\Xshell\Sessions"
	CString sGlobalIniFile;
	sGlobalIniFile.Format("%s\\NetSarang\\Xshell\\Sessions", szAppData);

	sFolder = sGlobalIniFile;
	return true;
}

bool LTUtils::GetUserAppFolder( CString& sFolder )
{
	char szAppData[MAX_PATH];
	if (! SUCCEEDED( SHGetFolderPath( NULL, CSIDL_APPDATA, NULL, 0, szAppData ) ) )
		return false;

	sFolder.Format("%s\\MillenniumIT\\LogTool", szAppData);

	return true;
}

CString LTUtils::GetAppIniFile()
{
	CString sUserAppFolder;
	GetUserAppFolder(sUserAppFolder);

	sUserAppFolder += "\\"LT_INI_FILE_NAME;

	return sUserAppFolder;
}

bool LTUtils::DecodePathString(const char* zConnStr, CString& sUser, CString& sIP, CString& sPath )
{
	CString sEnvStr = zConnStr;
	int iPosEUEnd = sEnvStr.Find('@');
	int iPosIPEnd = sEnvStr.Find(':');
	sUser = sEnvStr.Left(iPosEUEnd);

	if (iPosIPEnd == -1)
		sIP = sEnvStr.Right(sEnvStr.GetLength() - iPosEUEnd - 1);
	else 
	{
		sIP = sEnvStr.Mid(iPosEUEnd + 1, iPosIPEnd - iPosEUEnd - 1);
		sPath = sEnvStr.Right(sEnvStr.GetLength() - iPosIPEnd - 1);
	}

	return true;
}

bool LTUtils::DecodePathStringEx( const char* zConnStr, CString& sEnvName, CString& sPath )
{
	CString sEnvStr = zConnStr;
	int iPosIPEnd = sEnvStr.Find(':');

	if (iPosIPEnd == -1)
		sEnvName = sEnvStr;
	else 
	{
		sEnvName = sEnvStr.Left(iPosIPEnd);
		sPath = sEnvStr.Right(sEnvStr.GetLength() - iPosIPEnd - 1);
	}

	return true;
}
