#include "LTPch.h"
#include "LTSshSession.h"
#include "LTEnv.h"
#include "LTOutputStringQueue.h"

#define  DEFAULT_SSH_PORT   22

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
	p_Env = NULL;
}

LTSshSession::~LTSshSession(void)
{
	if (p_Session)
	{
		libssh2_session_disconnect(p_Session, "Normal Shutdown, Thank you for playing");
		libssh2_session_free(p_Session);
		p_Session = NULL;
	}

	if (i_Socket != -1)
	{
#ifdef WIN32
		closesocket(i_Socket);
#else
		close(i_Socket);
#endif
		i_Socket = -1;
	}
}

void LTSshSession::KeyboadCallback( const char *name, int name_len, const char *instruction, 
								   int instruction_len, int num_prompts, 
								   const LIBSSH2_USERAUTH_KBDINT_PROMPT *prompts, 
								   LIBSSH2_USERAUTH_KBDINT_RESPONSE *responses, 
								   void **abstract )
{
	LTSshSession* pSession = (LTSshSession*) (*abstract);
	LTEnv* pEnv = pSession->GetEnv();

	if (num_prompts == 1)
	{
		responses[0].text = lt_strdup(pEnv->s_Password.GetBuffer(), abstract);
		responses[0].length = pEnv->s_Password.GetLength();
	}
}

bool LTSshSession::Connect( LTEnv* pEnv, CString& sErr)
{
	p_Env = pEnv;

	unsigned long hostaddr;
    int  i, auth_pw = 0;
    struct sockaddr_in sin;
    const char *fingerprint;
    char *userauthlist;

	hostaddr = inet_addr(p_Env->s_IP);
  
    /* Ultra basic "connect to port 22 on localhost".  Your code is
     * responsible for creating the socket establishing the connection
     */ 
    i_Socket = socket(AF_INET, SOCK_STREAM, 0);
 
    sin.sin_family = AF_INET;
    sin.sin_port = htons(DEFAULT_SSH_PORT);
    sin.sin_addr.s_addr = hostaddr;
    if (connect(i_Socket, (struct sockaddr*)(&sin),
                sizeof(struct sockaddr_in)) != 0) 
	{
		sErr.Format("Failed to connect. ip=%s", p_Env->s_IP);
        return false;
    }
 
    /* Create a session instance and start it up. This will trade welcome
     * banners, exchange keys, and setup crypto, compression, and MAC layers
     */ 
    p_Session = libssh2_session_init_ex(lt_alloc, lt_free, lt_realloc, this);

    if (libssh2_session_handshake(p_Session, i_Socket)) 
	{
		sErr = "Failure establishing SSH session";
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
        if (libssh2_userauth_password(p_Session, p_Env->s_EnvUser, p_Env->s_Password))
		{
            sErr = "Authentication by password failed!\n";
            goto shutdown;
        } else {
            TRACE( "\tAuthentication by password succeeded.\n");
        }
    } else if (auth_pw & 2) {
        /* Or via keyboard-interactive */ 
        if (libssh2_userauth_keyboard_interactive(p_Session, p_Env->s_EnvUser, &KeyboadCallback) ) 
		{
			sErr = "Authentication by keyboard-interactive failed!";
			goto shutdown;
        } else {
            TRACE("\tAuthentication by keyboard-interactive succeeded.\n");
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
			sErr = "Authentication by public key failed!";
            goto shutdown;
        } else {
            TRACE( "\tAuthentication by public key succeeded.\n");
        }
    } else {
        sErr = "No supported authentication methods found!";
        goto shutdown;
    }

	return true;
 
 
 shutdown:
    libssh2_session_disconnect(p_Session, "Normal Shutdown, Thank you for playing");
    libssh2_session_free(p_Session);
	p_Session = NULL;
 
#ifdef WIN32
    closesocket(i_Socket);
#else
    close(i_Socket);
#endif

	i_Socket = -1;
    return false;
}

static int waitsocket(int socket_fd, LIBSSH2_SESSION *session)
{
	struct timeval timeout;
	int rc;
	fd_set fd;
	fd_set *writefd = NULL;
	fd_set *readfd = NULL;
	int dir;

	timeout.tv_sec = 10;
	timeout.tv_usec = 0;

	FD_ZERO(&fd);

	FD_SET(socket_fd, &fd);

	/* now make sure we wait in the correct direction */ 
	dir = libssh2_session_block_directions(session);


	if (dir & LIBSSH2_SESSION_BLOCK_INBOUND)
		readfd = &fd;

	if (dir & LIBSSH2_SESSION_BLOCK_OUTBOUND)
		writefd = &fd;

	rc = select(socket_fd + 1, readfd, writefd, NULL, &timeout);

	return rc;
}

bool LTSshSession::Execute(const char* zCommand, std::list<CString>* plstOut, CString* pErr)
{
	int rc = 0;
	int bytecount = 0;

	LIBSSH2_CHANNEL*	pChannel;

	/* Request a shell */ 
	if (!(pChannel = libssh2_channel_open_session(p_Session))) 
	{
		if (pErr)
			pErr->Format("Unable to open a session");
		return false;
	}

	while ( (rc = libssh2_channel_exec(pChannel, zCommand)) == LIBSSH2_ERROR_EAGAIN )
    {
        waitsocket(i_Socket, p_Session);
    }
    
	if ( rc != 0 )
    {
		if (pErr)
			pErr->Format("Socket error.");
        return false;
    }

    for ( ;; )
    {
        /* loop until we block */ 
        int rc;
        do
        {
            char buffer[0x4000];
            rc = libssh2_channel_read( pChannel, buffer, sizeof(buffer) );

			int istart = 0;

            if ( rc > 0 )
            {
                int i;
                bytecount += rc;
               // TRACE( "We read:\n");
                for (i = 0; i < rc; ++i )
				{
					char c = buffer[i];
                    //TRACE( "%c" , c);
					if (c == '\n' || c == '\r')
					{
						buffer[i] = 0;
						CString sVal = buffer + istart;
						if (plstOut)
						{
							plstOut->push_back(sVal);
							
						}
						LTOutputStringQueue::PushString(sVal);
						buffer[i] = c;
						//TRACE( "%s - %d\n" , sVal, istart);
						istart = i + 1;
					}
				}
            }
            else {
                if ( rc != LIBSSH2_ERROR_EAGAIN )
                    /* no need to output this for the EAGAIN case */ 
                    TRACE( "libssh2_channel_read returned %d\n", rc);
            }
        }
        while( rc > 0 );
 
        /* this is due to blocking that would occur otherwise so we loop on
           this condition */ 
        if ( rc == LIBSSH2_ERROR_EAGAIN )
        {
            waitsocket(i_Socket, p_Session);
        }
        else
            break;
    }
	

	int exitcode = 127;
	char *exitsignal = (char *)"none";

	while( (rc = libssh2_channel_close(pChannel)) == LIBSSH2_ERROR_EAGAIN )
        waitsocket(i_Socket, p_Session);
 
    if( rc == 0 )
    {
        exitcode = libssh2_channel_get_exit_status( pChannel );
        libssh2_channel_get_exit_signal(pChannel, &exitsignal,
                                        NULL, NULL, NULL, NULL, NULL);
    }
 
    if (exitsignal)
        TRACE( "Got signal: %s\n", exitsignal);
    else
		TRACE( "EXIT: %d bytecount: %d  command:%s\n", exitcode, bytecount, zCommand);

	libssh2_channel_free(pChannel);
	pChannel = NULL;

	return true;
}
