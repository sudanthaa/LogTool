#pragma once

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
};
