# 패스워드 평문 저장

> ## 정의
> **패스워드**를 비롯한 중요데이터를 **암호화되지 않은 평문 텍스트**의 형태로 저장되면 암호가 외부에 직접 드러날 수 있어
> **기밀성**이 보장되지 못하고 암호가 저장된 파일에 접근할 수 있는 사람이면 누구나 암호를 알아낼 수 있어 **무결성** 또한 보장되지 못한다. 
>  * 기밀성 : 비인가자들은 정보의 내용을 알 수 없게 해야 한다는 보안 원칙
>  * 무결성 : 허가되지 않은 자가(또는 허가되지 않은 방식으로) 정보를 변경, 삭제, 생성하는 것을 보호해야한다는 보안 원칙
<br>
<br>

> ## 안전한 코딩 기법
> 개인정보, 금융정보, 패스워드 등 중요정보를 저장할 때는 반드시 **암호화**하여 저장한다. 중요정보를 읽고 쓸 경우에는 허가된 사용자만이 사용정보에 접근하도록 **인증 과정**을 거쳐야 한다. 

<br>
<br>

## java 예제
### 패스워드 정보의 DB 저장
* 안전하지 못한 코드 : **인증을 통과한 사용자**의 패스워드 정보가 **평문**으로 DB에 저장된다. 
```java
String id = request.getParameter("id");
//외부값에 의해 패스워드 정보를 얻고 있다.
String pwd = request.getParameter("pwd");

// SQL문, ? : 변수가 들어갈 자리 
String sql = "insert into customer(id, pwd, name, ssn, zipcode, addr)"
+"values(?, ?, ?, ?, ?, ?)"
//지정된 SQL 명령어를 실행하는 PreparedStatement 인스턴스 생성
PreparedStatement stmt = con.prepareStatement(sql);

//SQL 문에서 ? 값에 대응되는 값을 지정함
stmt.setString(1,id);
stmt.setString(2, pwd);
....
//입력받은 패스워드가 평문으로 DB에 저장되어 안전하지 않다. 
stmt.executeUpdate();
```
<br>
<br>
  
* 안전한 코드 : 패스워드 등 중요 데이터를 **해쉬값**으로 **변환하여 저장**하고 있다. 
```java
String id = request.getParameter("id"); 
// 외부값에 의해 패스워드 정보를 얻고 있다.
String pwd = request.getParameter("pwd");

// 패스워드를 솔트값을 포함하여 SHA-256 해시로 변경하여 안전하게 저장한다. 
//SHA-256 형식으로 암호화할 객체 생성
MessageDigest md = MessageDigest.getInstance("SHA-256");
md.reset();
//salt : 해시함수에서 추가입력으로 사용되는 랜덤데이터  
md.update(salt);

//데이터 전처리
byte[] hashInBytes = md.digest(pwd.getBytes()); 
StringBuilder sb = new StringBuilder();
//전처리한 데이터(바이트)를 헥사값으로 변환
for (byte b : hashInBytes) {
sb.append(String.format("%02x", b)); }
pwd = sb.toString();
......

// SQL문, ? : 변수가 들어갈 자리
String sql = " insert into customer(id, pwd, name, ssn, zipcode, addr)"
+ " values (?, ?, ?, ?, ?, ?)";
//지정된 SQL 명령어를 실행하는 PreparedStatement 인스턴스 생성
PreparedStatement stmt = con.prepareStatement(sql); 
//SQL 문에서 ? 값에 대응되는 값을 지정함
stmt.setString(1, id);
stmt.setString(2, pwd);
......
//입력받은 패스워드가 SHA-256 알고리즘을 통해 해시값으로 저장되어서 안전하다. 
stmt.executeUpdate();
 ```
 <br>
<details>
<summary>
💚코드 추가 설명💚
	</summary>
<div markdown="1">       

* 클라이언트가 서버에게 무언가를 요청하면 서버는 요청에 해당하는 것을 응답해주는 구조임.
	
* 해당 코드는 Servlet 수준에서 사용자의 요청에 대해 파라미터를 통해 사용자 정보를 얻어서 DB에 쿼리를 날려 사용자 정보를 DB에 저장하고 있다. 
* Servlet : 자바 플랫폼에서 동적인 웹을 개발할 때 사용하는 클래스로, 클라이언트 요청을 처리하고 그 결과를 다시 클라이언트에세 전송하는 서버의 기능을 해주는 자바 프로그램

</div>
</details>
<br>

<details>
<summary>
💚사용된 클래스와 메소드💚
	</summary>
<div markdown="1">       

* request
    - getParameter(name) : http 요청에 대해 name이라는 파라미터 값을 얻기 위해 사용하는 것<br>
    [ServletRequest (Java(TM) EE 7 Specification APIs)](https://docs.oracle.com/javaee/7/api/javax/servlet/ServletRequest.html)
  
* PreparedStatement :  SQL 구문을 실행시키는 역할
    - executeUpdate() : 데이터베이스에서 데이터를 추가, 삭제, 수정하는 SQL 문을 실행<br>
    [PreparedStatement (Java SE 11 & JDK 11 )](https://docs.oracle.com/en/java/javase/11/docs/api/java.sql/java/sql/PreparedStatement.html)
 
* MessageDigest : 암호화를 위한 클래스<br>
    - [MessageDigest (Java Platform SE 7 )](https://docs.oracle.com/javase/7/docs/api/java/security/MessageDigest.html)
    

</div>
</details>

<br>
<details>
<summary>
💚SHA-256💚
	</summary>
<div markdown="1">       

* 암호화 해시 함수의 한 종류
	
* 해시 함수 : 임의의 길이의 정보를 입력으로 받아, 고정된 길이의 암호문(해시값)을 충력하는 암호기술
* 데이터 전처리, 초기값 설정, 해싱 연산 단계로 이루어져 있음 
	* 입력된 데이터를 전처리한다. 
	* 초기값을 설정한다. (초기값 : 알고리즘의 결과로 나올 해시값의 초기값)
	* 초기값에 입력된 데이터를 이용해 해싱 연산을 수행한다. 
* [SHA-256 코드](https://seed.kisa.or.kr/kisa/Board/21/detailView.do)

</div>
</details>
<br>

## C 예제
### 1. 파일에서 읽어온 패스워드와 DB 연결
* 안전하지 못한 코드 : 패스워드를 파일에서 읽어서 직접 DB 연결에 사용하고 있다. 이것은 패스워드를 암호화 하고 있지 않음을 의미한다. 
```c
int dbaccess()
{
  FILE * fp; char* server = "DBserver";
	char passwd[20];
	char user[20];
	SQLHENV henv;
	SQLHDBC hdbc;
	
	//config 파일을 열어서 해당 스트림에서 user, passwd 읽어옴
	fp = fopen("config", "r");
	fgets(user, sizeof(user), fp);
	fgets(passwd, sizeof(passwd), fp);
	fclose(fp);
	
	//DB와 연결
	SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv);
	SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc);
	SQLConnect(hdbc, (SQLCHAR*)server, (SQLSMALLINT)strlen(server), (SQLCHAR*)user, (SQLSMALLINT)strlen(user),
	/*파일로부터 패스워드를 평문으로 읽어들이고 있다*/
	(SQLCHAR*)passwd, 
	(SQLSMALLINT)strlen(passwd));
	return  0;
 }
 ```
 <br>
 <br>
 
 * 안전한 코드 : **외부에서 입력된 패스워드**는 **검증의 과정**을 거쳐서 사용하여야 한다.
 ```c
 int dbaccess()
{
	FILE * fp;
	char* server = "DBserver";
	char passwd[20];
	char user[20];
	char* verifiedPwd;
	SQLHENV henv;
	SQLHDBC hdbc;

	//config 파일을 열어서 해당 스트림에서 user, passwd 읽어옴
	fp = fopen("config", "r");
	fgets(user, sizeof(user), fp);
	fgets(passwd, sizeof(passwd), fp);
	fclose(fp);

	//패스워드 검증 과정
	verifiedPwd = verify(passwd);
	SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv);
	SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc);
	SQLConnect(hdbc, (SQLCHAR*)server, (SQLSMALLINT)strlen(server), (SQLCHAR*)user, (SQLSMALLINT)strlen(user),
	/*파일로부터 암호화된 패스워드를 읽어들이고 있다.*/ 
	(SQLCHAR*)verifiedPwd,(SQLSMALLINT)strlen(verifiedPwd)); 
	return  0;
}
 ```
 <br>
 <details>
<summary>💚사용된 함수💚</summary>
<div markdown="1">   
  
* [환경 핸들 - ODBC API Reference](https://docs.microsoft.com/ko-kr/sql/odbc/reference/develop-app/allocating-the-environment-handle?view=sql-server-ver15)

* [SQLConnect 함수 - ODBC API Reference](https://docs.microsoft.com/ko-kr/sql/odbc/reference/syntax/sqlconnect-function?view=sql-server-ver15)

* [SQLAllocHandle 함수 - ODBC API Reference](https://docs.microsoft.com/ko-kr/sql/odbc/reference/syntax/sqlallochandle-function?view=sql-server-ver15)
</div>
</details>
 <br>
 
 ### 2. 키보드로부터 읽어온 패스워드와 DB
 * 안전하지 못한 코드 : 사용자명과 패스워드를 키보드로부터 읽어서 문자열을 그대로 DB 연결에 사용하고 있다. 
 
 ```C
 const char  * GetParameter(const char  * queryString,  const char  * key);
int main(void)
{
	const  char* COMMAND_PARAM = "command";
	const  char* GET_USER_INFO_CMD = "get_user_info";
	
	char* queryStr;
	queryStr = getenv("QUERY_STRING");
	if (queryStr == NULL)
	{
		// Error 처리 
		...
	}
	char* command = GetParameter(queryStr, COMMAND_PARAM);
	if (!strcmp(command, GET_USER_INFO_CMD))
	{
		const char* USER_ID_PARAM = "user_id";
		const  char* PASSWORD_PARAM = "password";
		const char* userId = GetParameter(queryStr, USER_ID_PARAM);
		const char* password = GetParameter(queryStr, PASSWORD_PARAM);
		
		...
		char* dbUserId, * dbUserPassword;
		
		//암호화되지 않은 사용자 정보를 그대로 DB연결에 사용하고 있어서 안전하지 못하다. 
		loadDbUserInfo(&dbUserId, &dbUserPassword);
		SQLRETURN   retcode = SQLConnect(hdbc, (SQLCHAR*)"173.234.56.78", SQL_NTS, (SQLCHAR*)dbUserId, strlen(dbUserId), dbUserPassword, strlen(dbUserPassword));
		...
		free(userId);
		free(password);
	}
	return EXIT_SUCCESS; 
}
void  loadDbUserInfo(char** userId, char** password)    
{
	const int MAX_USER_LENGTH = 8;
	const int MAX_PASSWORD_LENGTH = 16;
	*userId = (char*)malloc(MAX_USER_LENGTH);
	*password = (char*)malloc(MAX_PASSWORD_LENGTH);
	FILE * fp = fopen("dbUserInfo.cfg", "r");
	fscanf(fp, "%s", *userId);
	fscanf(fp, "%s", *password);
}
 ```
<br>
<br>

* 안전한 코드 : 외부에서 입력된 사용자명과 패스워드를 loadDbUserInfo 함수에서 **OAEP패딩을 사용한 RSA 알고리즘**으로 **암호화**한 후 DB연결에 사용하고 있다. 

```c
const char  * GetParameter(const char  * queryString,  const char  * key);
const Key  decryptPassword="we37kor$^jkl315!#^!";  //private Key

int main(void)
{
	const  char  *  COMMAND_PARAM  =  "command";
	const  char *  GET_USER_INFO_CMD = "get_user_info";
	
	char * queryStr;
	queryStr = getenv("QUERY_STRING");
	if (queryStr == NULL) 
	{
		// Error  
		...
	}
	char * command = GetParameter(queryStr, COMMAND_PARAM);
	if (!strcmp(command, GET_USER_INFO_CMD)) 
	{
		const char * USER_ID_PARAM = "user_id";
		const  char  *  PASSWORD_PARAM  =  "password";
		const char * userId  =  GetParameter(queryStr, USER_ID_PARAM);
		const char * password = GetParameter(queryStr, PASSWORD_PARAM);
		...
		char * dbUserId, * dbUserPassword;
		
		//loadDbUserInfo를 통해 암호화된 사용자 정보를 DB연결에 사용하고 있다. 
		loadDbUserInfo(&dbUserId, &dbUserPassword);
		SQLRETURN   retcode    =    SQLConnect(hdbc,    (SQLCHAR*)    "173.234.56.78",    SQL_NTS, (SQLCHAR*) dbUserId, strlen(dbUserId), dbUserPassword, strlen(dbUserPassword));
		...
		free(userId);
		free(password);
	}
	return EXIT_SUCCESS; 32:     
}
void loadDbUserInfo(char  **  userId, char ** password)
{
	const int MAX_USER_LENGTH = 8;
	const int MAX_PASSWORD_LENGTH  = 16;
	*userId = (char *)malloc(MAX_USER_LENGTH);
	*password = (char *)malloc(MAX_PASSWORD_LENGTH);
	
	FILE * fp = fopen("dbUserInfo.cfg","r");
	fscanf(fp, "%s", *userId);
	fscanf(fp, "%s", *password);
        //RSA 알고리즘으로 암호화
	RSA_private_decrypt(strlen(decryptPassword),      decryptPassword,       *password, 
	RSA_PKCS1_OAEP_PADDDING);
}
```
<details>
<summary>💚사용된 함수💚</summary>
<div markdown="1">   
  
  * [RSA_public_encrypt](https://www.openssl.org/docs/man1.0.2/man3/RSA_private_decrypt.html)
</div>
</details>
<br>
<details>
<summary>💚RSA 암호화 알고리즘💚</summary>
<div markdown="1">   
  
* 암호화에 사용하는 키와 복호화에 사용하는 키가 다른 비대칭키 알고리즘
	
* 공개키와 개인키를 쌍으로 사용
	* 공개키 : 외부에 공개할 수 있는 키
	* 개인키 : 키의 소유자만 알고 있어야 하는 키
	* 본인 인증의 목적을 가질 때는 개인키로 암호화하고 공개키로 복호화한다.
	* 기밀성을 목적으로 할 때는 공개키로 암호화하고 개인키로 복호화한다. 

* 공개키와 개인키를 만드는 과정과 모듈러 연산을 통해 암호화, 복호화하는 과정이 필요하다.  
	
</div>
</details>
 <br>
 <br>
<details>
<summary>출처</summary>
<div markdown="1">
『전자정부 SW 개발, 운영자를 위한 C  시큐어 코딩 가이드』, 행정자치부, 한국 인터넷진흥원<br>
『전자정부 SW 개발, 운영자를 위한 소프트웨어 개발보안 가이드』, 행정안전부, 한국 인터넷진흥원<br>
https://docs.oracle.com/javaee/7/api/overview-summary.html<br>
https://docs.microsoft.com/ko-kr/documentation/<br>
https://seed.kisa.or.kr/kisa/Board/21/detailView.do<br>
https://gmlwjd9405.github.io/2018/10/27/webserver-vs-was.html<br>
https://m.blog.naver.com/ka28/221985380809
</div>
</details>
 <br>
