## SQL Injection : Persistence

### API Injection: Persistence 란?

J2EE Persistence API를 사용하는 응용프로그램에서 외부의 입력을 검증 없이 질의문을 사용하는 경우, 역시 프로그래머가 의도하지 않았던 임의의 query 명령어를 수행하는 공격을 말한다.

**➡️ 안전한 코딩 방법**

- 외부의 입력이 질의문의 구조를 변경할 수 없도록 한다. 즉, **인자화** 된 질의문 사용
- 질의문 생성 시 상수 문자열만을 사용하고,  javax.persistence.Query.setParameter() 메소드를 사용하여 인자값을 설정한다.
<br>


### 예제 1 - 입력값 검증

입력값을 id라고 할 때, 입력한 값은 다음 위치에 들어간다.

```java
SELECT col1 FROM MYTABLE WHERE name = '(입력값)'
```

만약 공격자가 입력값으로  `foo'; DROP MYTABLE; —` 을 입력하면 질의문은 다음과 같이 완성된다.

```java
SELECT col1 FROM MYTABLE WHERE name = 'foo'; DROP MYTABLE; --'
```

따라서 검증 없이 질의문을 그대로 입력하면, MYTABLE 을 삭제하는 명령어가 실행된다.

<br>

**안전하지 않은 코드**

```java
Properties props = new Properties();
String fileName = "conditions.txt";
FileInputStream in = new FileInputStream(fileName);
props.load(in);

// 외부로부터 입력 받기
String id = props.getProperty("id");
// 외부 입력값이 그대로 query의 인자로 사용된다.
Query query = 
em.createNativeQuery("SELECT OBJECT(i) FROM Item i WHERE i.itemID > " + id);
List<ItemListner> items = query.getResultList();
……
return r_type;
```

⇒ 따라서 인자를 받는 질의문을 생성 후, 인자 값을 설정하여 실행되도록 한다.

그렇게 하면 외부 입력값에 따라서 질의문 구조가 변경되는 것을 방지할 수 있다.

<br>

**안전한 코드**

```java
Properties props = new Properties();
String fileName = "conditions.txt";
FileInputStream in = new FileInputStream(fileName);
props.load(in);

// 외부 입력값 받기
String id = props.getProperty("id");
// 입력값을 검사한다
if (id == null || "".equals(id)) id = "itemid";
// Query을 작성한다
Query query = 
em.createNativeQuery("SELECT OBJECT(i) FROM Item i WHERE i.itemID > :id");
query.setParameter("id", id);
List<ItemListner> items = query.getResultList();
……
return r_type;
```
<br>

### 예제2 - 문자열 검증 함수

- 입력값에 userID와 passwd를 SQL 질의문에 그대로 넣는 경우

안전하게 SQL 구문 생성하기 위해, 외부로부터 입력되는 값을 질의문에 바로 사용하기 전에 안전한 형태로 `makeSecureString` 함수를 통해 변경한다.

이 함수로는 3가지 제한 조건을 사용하여 안전한 문자열을 생성 가능하다.

1. 인자에 SQL문에서 쓰이는 예약어를 제한한다.
2. 인자에 알파벳과 숫자를 제외한 문자를 사용하는 것을 제한한다. ( 특히 ' 싱글따옴표)
3. 위의 조건을 제한하기 위해 정규식을 사용한다.

<br>

➡️ 문자열을 더 안전하게 만들 수 있는 방법은 다음과 같다.

1. 문자열의 길이 제한을 낮춘다.
2. 정규식에 포함되는 단어의 개수를 높인다. (블랙리스트 개념으로 SQL procedure명이나 SQL 명령어들을 필터링할 정규식에 포함)
<br>

```java
private String makeSecureString(final String str, int maxLength) 
{
	String secureStr = str.substring(0, maxLength);
	Matcher matcher = unsecuredCharPattern.matcher(secureStr)
	return matcher.replaceAll("");
}
```

따라서 함수는 위와 같이 생성할 수 있다.
<br>


```java
String query = "SELECT * FROM members WHERE username= '" + makeSecureString(userId) + "' AND password = '" + makeSecureString(password) + "'";
Query query = manager.newQuery(queryStr);
```

그 후 입력값을 위의 함수의 인자로 전달 한 후, 생성된 문자열을 SQL 명령어에 넣어 실행하면 보다 안전한 코드를 작성할 수 있다.