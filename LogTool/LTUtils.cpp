#include "LTPch.h"
#include "LTUtils.h"
#include <Shlwapi.h>

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

CString LTUtils::GetVersionInfo(HMODULE hLib, CString csEntry)
{
	CString csRet;

	if (hLib == NULL)
		hLib = AfxGetResourceHandle();

	HRSRC hVersion = FindResource( hLib, 
		MAKEINTRESOURCE(VS_VERSION_INFO), RT_VERSION );
	if (hVersion != NULL)
	{
		HGLOBAL hGlobal = LoadResource( hLib, hVersion ); 
		if ( hGlobal != NULL)  
		{  

			LPVOID versionInfo  = LockResource(hGlobal);  
			if (versionInfo != NULL)
			{
				DWORD vLen,langD;
				BOOL retVal;    

				LPVOID retbuf=NULL;

				static char fileEntry[256];

				sprintf_s(fileEntry, 256,"\\VarFileInfo\\Translation");
				retVal = VerQueryValue(versionInfo,fileEntry,&retbuf,(UINT *)&vLen);
				if (retVal && vLen==4) 
				{
					memcpy(&langD,retbuf,4);            
					sprintf_s(fileEntry, 256, "\\StringFileInfo\\%02X%02X%02X%02X\\%s",
						(langD & 0xff00)>>8,langD & 0xff,(langD & 0xff000000)>>24, 
						(langD & 0xff0000)>>16, csEntry);            
				}
				else 
					sprintf_s(fileEntry, 256, "\\StringFileInfo\\%04X04B0\\%s", 
					GetUserDefaultLangID(), csEntry);

				if (VerQueryValue(versionInfo,fileEntry,&retbuf,(UINT *)&vLen)) 
					csRet = (char*)retbuf;
			}
		}

		UnlockResource( hGlobal );  
		FreeResource( hGlobal );  
	}

	return csRet;
}

CString LTUtils::FormatVersion(CString cs)
{
	CString csRet;
	if (!cs.IsEmpty())
	{
		cs.TrimRight();
		int iPos = cs.Find(',');
		if (iPos == -1)
			return "";
		cs.TrimLeft();
		cs.TrimRight();
		csRet.Format("%s.", cs.Left(iPos));

		while (1)
		{
			cs = cs.Mid(iPos + 1);
			cs.TrimLeft();
			iPos = cs.Find(',');
			if (iPos == -1)
			{
				csRet +=cs;
				break;
			}
			csRet += cs.Left(iPos);
		}
	}

	return csRet;
}



// Loads "FileVersion" from resource formats it and keeps it in mind
CString LTUtils::GetFileVersionX()
{
	CString csVersion = FormatVersion(GetVersionInfo(NULL, "FileVersion"));
	CString sFileVersion;
	sFileVersion.Format("Version %s (Build %s)", 
		csVersion, GetVersionInfo(NULL, "SpecialBuild"));

	return sFileVersion;
}

// Loads "ProductVersion" from resource formats it and keeps it in mind
CString LTUtils::GetProductVersionX()
{
	return GetVersionInfo(NULL, "ProductVersion");
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

	char szDocuments[MAX_PATH];
	if (! SUCCEEDED( SHGetFolderPath( NULL, CSIDL_MYDOCUMENTS, NULL, 0, szDocuments ) ) )
		return false;

	CString xShellSessionsDir5;
	xShellSessionsDir5.Format("%s\\NetSarang\\Xshell\\Sessions", szDocuments);
	
	if (PathIsDirectory(xShellSessionsDir5) != FALSE)
	{
		sFolder = xShellSessionsDir5;
		return true;
	}

	CString xShellSessionsDir;
	xShellSessionsDir.Format("%s\\NetSarang\\Xshell\\Sessions", szAppData);
	if (PathIsDirectory(xShellSessionsDir) != FALSE)
	{
		sFolder = xShellSessionsDir;
		return true;
	}

	return false;
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

CString LTUtils::GetTempPath()
{
	char zBuff[300];
	char zBuffEx[300];
	::GetTempPath(300, zBuff);
	::GetLongPathName(zBuff, zBuffEx, 300);

	return zBuffEx;
}
