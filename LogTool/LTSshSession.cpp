#include "LTPch.h"
#include "LTSshSession.h"
#include "LTEnv.h"


static void*	lt_alloc(size_t count, void **abstract)		{	return malloc(count);}
static void		lt_free(void *ptr, void **abstract)			{	free(ptr);}
static void*	lt_realloc(void *ptr, size_t count, void **abstract){	return realloc(ptr, count);}
static char*	lt_strdup(const char* str, void **abstract)
					{   
						int len = strlen(str) + 1; 
						char* cp = (char*)lt_alloc(len, abstract);
						strcpy_s(cp, len, str);
						return cp;
					}


LTSshSession::LTSshSession(void)
{
	p_Session = NULL;
	p_Channel = NULL;
	p_Env = NULL;
}

LTSshSession::~LTSshSession(void)
{
}

void LTSshSession::KeyboadCallback( const char *name, int name_len, const char *instruction, 
								   int instruction_len, int num_prompts, 
								   const LIBSSH2_USERAUTH_KBDINT_PROMPT *prompts, 
								   LIBSSH2_USERAUTH_KBDINT_RESPONSE *responses, 
								   void **abstract )
{
	LTSshSession* pSession = (LTSshSession*) abstract;
	LTEnv* pEnv = pSession->GetEnv();

	if (num_prompts == 1)
	{
		responses[0].text = lt_strdup(pEnv->s_Password.GetBuffer(), abstract);
		responses[1].length = pEnv->s_Password.GetLength();
	}
}

bool LTSshSession::Connect( LTEnv* pEnv, CString& sErr)
{
	p_Env = pEnv;

	unsigned long hostaddr;
    int sock, i, auth_pw = 0;
    struct sockaddr_in sin;
    const char *fingerprint;
    char *userauthlist;

	hostaddr = inet_addr(p_Env->s_IP);
  
    /* Ultra basic "connect to port 22 on localhost".  Your code is
     * responsible for creating the socket establishing the connection
     */ 
    sock = socket(AF_INET, SOCK_STREAM, 0);
 
    sin.sin_family = AF_INET;
    sin.sin_port = htons(22);
    sin.sin_addr.s_addr = hostaddr;
    if (connect(sock, (struct sockaddr*)(&sin),
                sizeof(struct sockaddr_in)) != 0) {
        TRACE( "failed to connect!\n");
        return false;
    }
 
    /* Create a session instance and start it up. This will trade welcome
     * banners, exchange keys, and setup crypto, compression, and MAC layers
     */ 
    p_Session = libssh2_session_init_ex(lt_alloc, lt_free, lt_realloc, this);

    if (libssh2_session_handshake(p_Session, sock)) 
	{
        TRACE( "Failure establishing SSH session\n");
        return false;
    }
 
    /* At this point we haven't authenticated. The first thing to do is check
     * the host-key's fingerprint against our known hosts Your app may have it
     * hard coded, may go to a file, may present it to the user, that's your
     * call
     */ 
    fingerprint = libssh2_hostkey_hash(p_Session, LIBSSH2_HOSTKEY_HASH_SHA1);

    TRACE( "Fingerprint: ");
    for (i = 0; i < 20; i++) {
        TRACE( "%02X ", (unsigned char)fingerprint[i]);
    }
    TRACE( "\n");
 
    /* check what authentication methods are available */ 
    userauthlist = libssh2_userauth_list(p_Session, p_Env->s_EnvUser, p_Env->s_EnvUser.GetLength());

    TRACE( "Authentication methods: %s\n", userauthlist);
    if (strstr(userauthlist, "password") != NULL) {
        auth_pw |= 1;
    }
    if (strstr(userauthlist, "keyboard-interactive") != NULL) {
        auth_pw |= 2;
    }
    if (strstr(userauthlist, "publickey") != NULL) {
        auth_pw |= 4;
    }
 

    if (auth_pw & 1) {
        /* We could authenticate via password */ 
        if (libssh2_userauth_password(p_Session, p_Env->s_EnvUser, p_Env->s_Password)) {

            TRACE( "\tAuthentication by password failed!\n");
            goto shutdown;
        } else {
            TRACE( "\tAuthentication by password succeeded.\n");
        }
    } else if (auth_pw & 2) {
        /* Or via keyboard-interactive */ 
        if (libssh2_userauth_keyboard_interactive(p_Session, p_Env->s_EnvUser, &KeyboadCallback) ) {
            TRACE(
                "\tAuthentication by keyboard-interactive failed!\n");
            goto shutdown;
        } else {
            TRACE(
                "\tAuthentication by keyboard-interactive succeeded.\n");
        }
    } else if (auth_pw & 4) {
        /* Or by public key */
		char zHomeDrive[100];
		size_t iret = 0;
		getenv_s(&iret, zHomeDrive, 100,  "HOMEDRIVE");

		char zHomePath[100];
		getenv_s(&iret, zHomePath, 100, "HOMEPATH");

		CString sPrivFile;
		CString sPubFile;

		sPrivFile.Format("%s%s.ssh\\id_rsa", zHomeDrive, zHomePath);
		sPubFile.Format("%s%s.ssh\\id_rsa.pub", zHomeDrive, zHomePath);

        if (libssh2_userauth_publickey_fromfile(p_Session, 
				p_Env->s_EnvUser, sPubFile, sPrivFile, "")) 
		{
            TRACE( "\tAuthentication by public key failed!\n");
            goto shutdown;
        } else {
            TRACE( "\tAuthentication by public key succeeded.\n");
        }
    } else {
        TRACE( "No supported authentication methods found!\n");
        goto shutdown;
    }
 
    /* Request a shell */ 
    if (!(p_Channel = libssh2_channel_open_session(p_Session))) {

        TRACE( "Unable to open a session\n");
        goto shutdown;
    }
 
    /* Some environment variables may be set,
     * It's up to the server which ones it'll allow though
     */ 
    libssh2_channel_setenv(p_Channel, "FOO", "bar");

 
    /* Request a terminal with 'vanilla' terminal emulation
     * See /etc/termcap for more options
     */ 
    if (libssh2_channel_request_pty(p_Channel, "vanilla")) {

        TRACE( "Failed requesting pty\n");
        goto skip_shell;
    }
 
    /* Open a SHELL on that pty */ 
    if (libssh2_channel_shell(p_Channel)) {

        TRACE( "Unable to request shell on allocated pty\n");
        goto shutdown;
    }

	return true;


	//libssh2_channel_exec(p_Channel, "ls -la");

 
    /* At this point the shell can be interacted with using
     * libssh2_channel_read()
     * libssh2_channel_read_stderr()
     * libssh2_channel_write()
     * libssh2_channel_write_stderr()
     *
     * Blocking mode may be (en|dis)abled with: libssh2_channel_set_blocking()
     * If the server send EOF, libssh2_channel_eof() will return non-0
     * To send EOF to the server use: libssh2_channel_send_eof()
     * A p_Channel can be closed with: libssh2_channel_close()
     * A p_Channel can be freed with: libssh2_channel_free()
     */ 
 
  skip_shell:
    if (p_Channel) {
        libssh2_channel_free(p_Channel);
        p_Channel = NULL;
    }
 
    /* Other p_Channel types are supported via:
     * libssh2_scp_send()
     * libssh2_scp_recv()
     * libssh2_channel_direct_tcpip()
     */ 
 
  shutdown:
 
    libssh2_session_disconnect(p_Session,

                               "Normal Shutdown, Thank you for playing");
    libssh2_session_free(p_Session);

 
#ifdef WIN32
    closesocket(sock);
#else
    close(sock);
#endif
    TRACE( "all done!\n");
    return 0;
}
