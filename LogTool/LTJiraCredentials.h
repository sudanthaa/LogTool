#pragma once

class LTJiraCredentials
{
public:
	LTJiraCredentials(void);
	~LTJiraCredentials(void);

	CString GetFullTicketID();
	CString GetAuthCode();
	void	GetFullTicketID(CString& sOut);
	void	GetAuthCode(CString& sOut);

	CString s_User;
	CString s_URL;
	CString s_Project;
	CString s_Password;
	CString s_ID;
};
