#pragma once

#include <libssh2.h>
#include <list>

class LTEnv;

class LTSshSession
{
public:
	LTSshSession(void);
	~LTSshSession(void);

	bool	Connect(LTEnv* pEnv, CString& sErr);
	LTEnv*	GetEnv(){ return p_Env; };

	static  void KeyboadCallback(const char *name, int name_len,
					const char *instruction, int instruction_len,
					int num_prompts,
					const LIBSSH2_USERAUTH_KBDINT_PROMPT *prompts,
					LIBSSH2_USERAUTH_KBDINT_RESPONSE *responses,
					void **abstract);

	bool	Execute(const char* zCommand, std::list<CString>* plstOut = NULL, CString* pErr = NULL);

protected:
	LTEnv* p_Env;
	int		i_Socket;
	LIBSSH2_SESSION*	p_Session;
};
