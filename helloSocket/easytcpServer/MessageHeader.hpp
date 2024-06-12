enum CMD {
	CMD_LOGIN,
	CMD_LOGIN_RESULT,
	CMD_LOGINOUT,
	CMD_LOGINOUT_RESULT,
	CMD_NEW_USER_JOIN,
	CMD_ERROR
};

struct DataHeader {
	DataHeader() {
		dataLength = sizeof(DataHeader);
		cmd = CMD_ERROR;
	}
	short dataLength; // 数据长度
	short cmd; // 命令
};

// 登录以及其返回的结果
// 如果使用结构体来进行，不利于扩展，这里直接使用继承的方式
struct Login : public DataHeader {
	//int age;
	//char name[32];
	Login() {
		dataLength = sizeof(Login);
		cmd = CMD_LOGIN;
	}
	//DataHeader header;
	char userName[32];
	char passWord[32];
	char data[956];    // 1kb
};

struct LoginResult : public DataHeader {
	LoginResult() {
		dataLength = sizeof(LoginResult);
		cmd = CMD_LOGIN_RESULT;
		result = 0;   // 为0表示一切正常
	}
	int result;   // 登录的结果
	char data[1016];
};

// 登出以及其返回的结果
struct Loginout : public DataHeader {
	Loginout() {
		dataLength = sizeof(Loginout);
		cmd = CMD_LOGINOUT;
	}
	char userName[32];
};

struct LoginoutResult : public DataHeader {
	LoginoutResult() {
		dataLength = sizeof(LoginoutResult);
		cmd = CMD_LOGINOUT_RESULT;
		result = 0;
	}
	int result;   // 登录的结果
};

// 新的客户端的加入
struct NewUserJoin : public DataHeader {
	NewUserJoin() {
		dataLength = sizeof(NewUserJoin);
		cmd = CMD_NEW_USER_JOIN;
		sock = 0;
	}
	//int result;   // 登录的结果
	int sock;
};
