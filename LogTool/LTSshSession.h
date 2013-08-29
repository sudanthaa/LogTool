#pragma once

#include <libssh2.h>

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

protected:
	LTEnv* p_Env;
	LIBSSH2_SESSION*	p_Session;
	LIBSSH2_CHANNEL*	p_Channel;
};
