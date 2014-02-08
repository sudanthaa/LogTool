#include "LTPch.h"
#include "LTJiraCredentials.h"

LTJiraCredentials::LTJiraCredentials(void)
{
}

LTJiraCredentials::~LTJiraCredentials(void)
{
}

CString LTJiraCredentials::GetFullTicketID()
{
	CString sTicket;
	GetFullTicketID(sTicket);

	return sTicket;
}

void LTJiraCredentials::GetFullTicketID( CString& sOut )
{
	sOut.Format("%s-%s", s_Project, s_ID);
}

CString LTJiraCredentials::GetAuthCode()
{
	CString sAuthCode;
	GetAuthCode(sAuthCode);
	return sAuthCode;
}

void LTJiraCredentials::GetAuthCode( CString& sOut )
{
	sOut.Format("%s:%s", s_User, s_Password);
}
