# 사용자 중요 정보 평문 저장/전송

## 정의

- 데이터를 평문으로 통신 채널을 통해 송수신할 경우, 인증받지 않은 사용자에 의해 발생한 `스니핑`을 통해 보안과 관련된 중요한  데이터가 노출될 수 있다.
    - `스니핑` : 네트워크상에서 다른 상대방들의 패킷 교환을 엿듣는 행위. ex) 전화 도청
    
<br>

## 안전한 코딩기법

- 중요한 정보를 저장할 때에는 반드시 `암호화`하여 저장한다.
- 중요한 정보를 통신 채널을 통해 전송할 때에도 반드시 `암호화` 과정을 거쳐야 한다.
    - 필요할 경우 SSL 또는 HTTPS와 같은 보안 채널을 사용한다.
    - 보안 채널을 사용하거나 브라우저 쿠키에 중요 데이터를 저장하는 경우, `setSecure(true)` 메소드를 통해 쿠키 객체에 보안속성을 설정하여 중요정보 노출을 방지할 수 있다.
    - 보안속성이 설정된 쿠키는 HTTP로는 전송되지 않으므로, 중요 데이터를 저장한 쿠키를 HTTP로 전송하기 위해서는 보안 속성을 대신하여 반드시 암호화를 적용해야 한다.

<br>

## JAVA 예제 :
### 암호화하지 않은 패스워드 사용 ➡ AES 암호화 

```java
String password = getPassword();
o.write(password);
```

패스워드를 암호화하지 않고 평문으로 전송하고 있다. 이 경우에 패킷 스니핑을 통해 패스워드가 노출될 수 있다.

`안전하지 않은 코드`

```java
try {
	Socket s = new Socket("taranis", 4444);
	PrintWriter o = new PrintWriter(s.getOutputStream(), true);
	
	String password = getPassword();
	o.write(password);
} catch (FileNotFoundException e) {
……
```

---

```java
Cipher c = Cipher.getInstance("AES/CBC/PKCS5Padding");
	
String password = getPassword();
byte[] encPassword = c.update(password.getBytes());
```

패스워드를 네트워크를 통해 서버로 전송하기 전에 암호화하여 안전한 프로그램이다. 이 예제는 AES 암호화 알고리즘을 사용하였다.

`안전한 코드`

```java
try {
	Socket s = new Socket("taranis", 4444);
	PrintStream o = new PrintStream(s.getOutputStream(), true);
	
	Cipher c = Cipher.getInstance("AES/CBC/PKCS5Padding");
	
	String password = getPassword();
	byte[] encPassword = c.update(password.getBytes());
	o.write(encPassword, 0, encPassword.length);
} catch (FileNotFoundException e) {
……
```

<br>

## Android-JAVA 예제 :
### 일반 소켓 통신 사용해 데이터 노출 가능 ➡ 키를 사용해 암호화

```java
Socket socket = new Socket(hostname, port);
```

일반적인 소켓 통신을 사용하여 네트워크를 통하여 데이터를 외부에 전송하고 있다. 마찬가지로 이 경우도 패킷 스니핑을 통하여 데이터의 내용이 노출될 수 있다.

`안전하지 않은 코드`

```java
public void onCreate(Bundle savedInstanceState) {
	int port = 443;
	String hostname = "hostname";
	Socket socket = new Socket(hostname, port);
	InputStream in = socket.getInputStream();
	OutputStream out = socket.getOutputStream();
	// Read from in and write to out...
	inclose();
	out.close();
}
```

---

```java
SocketFactory socketFactory = SSLSocketFactory.getDefault();
Socket socket = socketFactory.createSocket(hostname, port);
```

민감한 정보를 네트워크를 통하여 서버에 전송하기 전에 최소한 128비트 길이의 키를 이용하여 암호화하는 것이 바람직하다.

`안전한 코드`

```java
public void onCreate(Bundle savedInstanceState) {
	int port = 443;
	String hostname = "hostname";
	SocketFactory socketFactory = SSLSocketFactory.getDefault();
	Socket socket = socketFactory.createSocket(hostname, port);
	InputStream in = socket.getInputStream();
	OutputStream out = socket.getOuputStream();
	// Read from in and write to out..
	in.close();
	out.close();
}
```

<br>

## C 예제 :
### 파일에서 읽은 패스워드 암호화 없이 직접 연결 ➡ 패스워드 검증 (AES-CBC 암호화)

```c
fgets(passwd, sizeof(passwd), fp);
```

패스워드를 파일에서 읽어 오고 있다.

```c
(SQLCHAR*) passwd,
```

파일에서 읽어온 패스워드를 암호화 없이 직접 연결하고 있다.

`안전하지 않은 코드`

```c
int dbaccess(){
	FILE *fp; char *server = "DBserver";
	char passwd[20];
	char user[20];
	SQLHENV henv;
	SQLHDBC hdbc;
	fp = fopen("config", "r");
	fgets(user, sizeof(user), fp);
	fgets(passwd, sizeof(passwd), fp);
	fclose(fp);
	SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv);
	SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc);
	SQLConnect(hdbc,
									(SQLCHAR*) server,
									(SQLSMALLINT) strlen(server),
									(SQLCHAR*) user,
									(SQLSMALLINT) strlen(user),
									(SQLCHAR*) passwd,
									(SQLSMALLINT) strlen(passwd) );
	return 0;
}
```

---

외부에서 입력된 패스워드는 검증의 과정을 거쳐서 사용해야 한다.

```c
char *key;
```

```c
HCkCrypt2 crypt = CkCrypt2_putCryptAlgorithm(crypt,”aes”);
CkCrypt2_putCipherMode(crypt,”cbc”);
```

AES-CBC로 암호화 모드를 설정한다.

```c
key = getenv(“encrypt_key”);
CkCrypt2_SetEncodedKey(crypt,key,”hex”);
```

외부에서 암호화 키를 불러와 설정한다.

```c
fgets(passwd, sizeof(passwd), fp);
```

패스워드를 파일에서 읽어온다.

```c
encPasswd = CkCrypt2_encryptStringENC(crypt, passwd);
```

패스워드 암호화를 진행한다.

```c
(SQLCHAR*) encPasswd,
```

암호화된 패스워드를 사용한다.

`안전한 코드`

```c
int dbaccess(){
	FILE *fp; char *server = "DBserver";
	char passwd[20];
	char user[20];
	char *encPasswd;
	char *key;
	SQLHENV henv;
	SQLHDBC hdbc;
	
	HCkCrypt2 crypt = CkCrypt2_putCryptAlgorithm(crypt,”aes”);
	CkCrypt2_putCipherMode(crypt,”cbc”);
	
	key = getenv(“encrypt_key”);
	CkCrypt2_SetEncodedKey(crypt,key,”hex”);
	fp = fopen("config", "r");
	fgets(user, sizeof(user), fp);
	
	fgets(passwd, sizeof(passwd), fp);
	fclose(fp);
	
	encPasswd = CkCrypt2_encryptStringENC(crypt, passwd);
	SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv);
	SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc);
	SQLConnect(hdbc,
									(SQLCHAR*) server,
									(SQLSMALLINT) strlen(server),
									(SQLCHAR*) user,
									(SQLSMALLINT) strlen(user),
									(SQLCHAR*) encPasswd,
									(SQLSMALLINT) strlen(verifiedPwd) );
	return 0;
}
```

<br>

## C# 예제 :
### 암호화하지 않은 패스워드 포함된 메시지 전송 ➡ 암호화 후 전송

```csharp
Message.Body = "Your password is: " + Server.HtmlEncode(password);
SmtpMail.Send(Message);
```

패스워드를 암호화하지 않고 패스워드가 포함된 메시지를 네트워크를 통하여 전송하고 있다. 이 경우 또한 패킷스니핑을 통하여 패스워드가 노출될 수 있다.

`안전하지 않은 코드`

```csharp
public void EmailPassword_OnClick(object sender, EventArgs args)
{
	MembershipUser u = Membership.GetUser(UsernameTextBox.Text, false);
	String password;

	if (u != null)
	{
	try
	{
		password = u.GetPassword(); // sensitive data created
	}
	catch (Exception e)
	{
		Msg.Text = "An exception occurred retrieving your password: " +
		Server.HtmlEncode(e.Message);
		return;
	}
	MailMessage Message = new MailMessage();
	Message.Body = "Your password is: " + Server.HtmlEncode(password);
	
	SmtpMail.Send(Message);
	Msg.Text = "Password sent via e-mail.";
	}
	else
	{
		Msg.Text = "User name is not valid. Please check the value and try again.";
	}
}
```

---

```csharp
byte[] data = System.Text.Encoding.ASCII.GetBytes(password);
data = new
System.Security.Cryptography.SHA256Managed().ComputeHash(data);
String hashedPassword = System.Text.Encoding.ASCII.GetString(data);
```

패스워드를 암호화하여 네트워크를 통해 전송한다.

`안전한 코드`

```csharp
public void EmailPassword_OnClick(object sender, EventArgs args)
{
	MembershipUser u = Membership.GetUser(UsernameTextBox.Text, false);
	String password;

	if (u != null)
	{
		try
		{
			password = u.GetPassword();
			byte[] data = System.Text.Encoding.ASCII.GetBytes(password);
			data = new
			System.Security.Cryptography.SHA256Managed().ComputeHash(data);
			String hashedPassword = System.Text.Encoding.ASCII.GetString(data);
		}
		catch (Exception e)
		{
			Msg.Text = "An exception occurred retrieving your password: " +
			Server.HtmlEncode(e.Message);
			return;
		}
		MailMessage Message = new MailMessage();
		Message.Body = "Your password is: " + Server.HtmlEncode(hasedPassword);
		SmtpMail.Send(Message);
		Msg.Text = "Password sent via e-mail.";
	}
	else
	{
		Msg.Text = "User name is not valid. Please check the value and try again.";
	}
}
```