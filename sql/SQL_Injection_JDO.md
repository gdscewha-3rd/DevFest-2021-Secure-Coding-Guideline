## SQL Injection : JDO

### JDO란?

Java Data Objects로, **적절한 검사 과정**을 거치지 않고 JDO API의 SQL 또는 JDOQL 질의문 생성을 위한 문자열로 사용하면 프로그래머가 의도하지 않았던 문자열을 전달해 **질의문 의미를 왜곡**시키거나 구조를 변경하여 임의의 질의 명령어를 수행하는 것을 말한다.

**➡️ 안전한 코딩 방법**

- JDO 질의문 생성 시 상수 문자열만을 사용하고, Query.execute 실행 시 인자값을 전달하는 Parameterize Query를 사용한다.

<BR>

### 예제1 - Parameterize Query

```sql
SELECT col1 FROM MYTABLE WHERE name=' 입력문 ';
```

위의 명령문에서 입력문 자리에  `name'; DROP MYTABLE; —`  를 입력값으로 전달한다면 다음과 같은 명령문이 실행된다.

```sql
SELECT col1 FROM MYTABLE WHERE name = 'name'; DROP MYTABLE;—'
```

따라서 검사과정 없이 입력문을 받으면 `Mytable` 이 삭제되는 결과가 발생한다.
  
<br>

**안전하지 않은 코드**

```java
try 
{
	Properties props = new Properties();
	String fileName = "contacts.txt";
	FileInputStream in = new FileInputStream(fileName);
	if( in != null ) { props.load(in); }
	in.close();

	// 외부로부터 입력 받기
	String name = props.getProperty("name");
	if( name != null ) 
	{
		query += " where name = '" + name + "'";
	}
	} 
catch (IOException e) { …… }

// 외부 입력값이 JDO 객체의 인자로 그대로 사용된다.
return (List<Contact>) pm.newQuery(query).execute()
```

위의 코드에서는 외부에서 입력한 값을 JDO 객체의 인자로 그대로 사용하기 때문에, 쿼리문이 수정될 수 있어 안전하지 않다.
  
외부 입력부분을 `?` 로 설정하고, 실행 시 해당 인자값이 전달되도록 수정하면 외부의 입력값이 쿼리문을 수정하는 것을 방지 할 수 있다.

  

<br>  

**안전한 코드**

```java
try 
{
	Properties props = new Properties();
	String fileName = "contacts.txt";
	FileInputStream in = new FileInputStream(fileName);
	props.load(in);
	// 외부로부터 입력값 받기
	name = props.getProperty("name");
	// 입력값을 점검한다
	if (name == null || "".equals(name)) return null;
	query += " where name = ?";
} 
catch (IOException e) { …… }

javax.jdo.Query q = pm.newQuery(query);
// Query API의 인자로 사용
return (List<Contact>) q.execute(name);
```

<br><br>

  
### **예제2 - Parameterize Query**

item에 대한 정보를 얻어올 때, itemname의 입력 값을  `name' OR 'a' = 'a'` 로 주게되면 `SELECT * FROM items`를 수행한 결과와 동일하게 된다.

즉, 다음 코드를 완성하게 된다.

```sql
SELECT * FROM items WHERE itemname='name' OR 'a' = 'a';
```

따라서 원래 의도와는 다르게 모든 사용자의 모든 item에 대한 정보를 얻을 수 있게 된다.
  
<br>


**안전하지 않은 코드**

```java
protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException 
{
	String command = request.getParameter(COMMAND_PARAM);
	...
	if (command.equals(GET_USER_INFO_CMD)) 
	{
		String userId = request.getParameter(USER_ID_PARM);
		String itemName = request.getParameter(ITEM_NAME_PARM);
		 
		PersistenceManager manager = factory.getPersistenceManager();
		
  		//query문에 입력값 바로 사용
		String sql = "SELECT * FROM items WHERE owner = '" + userName + "' AND itemname = '"
		+ itemName + "'";
		Query query = pm.newQuery(Query.SQL, sql);
		query.setClass(Person.class);
		List people = (List)query.execute();
	...
	}
}
```

위의 코드처럼 외부의 입력값 문자열을 바로 사용하지 않고, **실행 시 입력받은 문자열을 인자로 전달함**으로써 질의문 의도를 바꾸지 못하게 할 수 있다.
  
<br>
 

**안전한 코드 예시**

```java
 protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException 
{
	String command = request.getParameter(COMMAND_PARAM);
	...
	if (command.equals(GET_USER_INFO_CMD)) 
	{
		String userId = request.getParameter(USER_ID_PARM);
		String itemName = request.getParameter(ITEM_NAME_PARM);
		
		PersistenceManager manager = factory.getPersistenceManager();
		
		String sql = "SELECT * FROM items WHERE owner = ? AND itemname = ?";
		Query query = pm.newQuery(Query.SQL, sql);
		query.setClass(Person.class);
		 
		List people = (List)query.execute(userId, itemName);
		 ...
	}
}
```