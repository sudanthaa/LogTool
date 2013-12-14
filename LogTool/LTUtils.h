#pragma once



#define GetRValue(rgb)      (LOBYTE(rgb))
#define GetGValue(rgb)      (LOBYTE(((WORD)(rgb)) >> 8))
#define GetBValue(rgb)      (LOBYTE((rgb)>>16))

#define  RGB_PARAMS(c)	GetRValue(c),GetGValue(c),GetBValue(c)

class LTUtils
{
public:
	LTUtils(void);
	~LTUtils(void);

	static CString GetAppPath();
	static CString GetUserLocal();
	static bool GetXShellSeesionFolder(CString& sFolder);
	static bool GetUserAppFolder(CString& sFolder);
	static CString GetAppIniFile();
	static bool DecodePathString(const char* zConnStr, CString& sUser, CString& sIP, CString& sPath);
	static bool DecodePathStringEx(const char* zConnStr, CString& sEnvName, CString& sPath);
	static CString GetVersionInfo(HMODULE hLib, CString csEntry);
	static CString FormatVersion(CString cs);


	static CString GetProductVersionX();
	static CString GetFileVersionX();

};
