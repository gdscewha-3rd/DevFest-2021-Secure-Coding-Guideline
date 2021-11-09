# Xquery Injection

**가. 정의**

대표적인 마크업 언어인 XML 데이터에 대해 동적 쿼리문을 생성할 때 XQuery를 사용하곤 합니다. 이 때 외부 입력 값에 대해 적절한 검증 절차가 필요한데요. 만일 검증 절차가 없을 경우에는 공격자가 쿼리문의 구조를 임의로 조작할 수 있게 됩니다. 그래서 악의적인 쿼리가 실행되어 허가되지 않은 데이터를 조회하거나 인증 절차를 우회할 수 있게 되어 위협이 될 수 있습니다. 

이 공격을 바로 XQuery 삽입(XQuery Injection) 이라고 합니다.

XQuery Injection의 공격 원리를 정리해보면 다음과 같은 순서로 이루어집니다.

1. 공격자가 XQuery Injection 시도
2. XML 데이터에 대한 조작된 동적 쿼리문 생성
3. 조작된 XQuery문 실행
4. 서버는 실행된 결과 즉, 공격 결과를 공격자에게 전달

**나. 안전한 코딩기법**

XQuery Injection을 예방하기 위한 안전한 코딩 기법은 크게 2가지가 있습니다. 

1. XQuery에 사용되는 외부 입력값에 대하여 특수문자나 쿼리 예약어를 필터링합니다
2. XQuery를 사용한 쿼리문은 string을 연결하는 형태가 아닌 파라미터화된 쿼리문 사용합니다

아래 예제를 통해 조금 더 구체적으로 살펴보겠습니다.

**다. 예제1**

먼저 안전하지 않은 코드 전체부터 살펴보겠습니다.

이 예제는 외부로부터 입력받은 값(name)을 executeQuery를 사용한 질의 생성 문자열 인자 생성에 사용하는 과정의 코드입니다.

```java
......

	// 외부로부터 입력 값(name) 받음
	String name = props.getProperty("name");
	
	//
	Hashtable env = new Hashtable();
	env.put(Context.INITIAL_CONTEXT_FACTORY, "com.sun.jndi.ldap.LdapCtxFactory");
	env.put(Context.PROVIDER_URL, "ldap://localhost:389/o=rootDir");
	javax.naming.directory.DirContext ctx = new InitialDirContext(env);
	javax.xml.xquery.XQDataSource xqds = (javax.xml.xquery.XQDataSource) ctx.lookup("xqj/personnel");
	javax.xml.xquery.XQConnection conn = xqds.getConnection();

	//안전하지 않은 코드
	String es = "doc('users.xml')/userlist/user[uname='" + name + "']";
	
	// 입력값이 Xquery 인자로 사용
	XQPreparedExpression expr = conn.prepareExpression(es);
	XQResultSequence result = expr.executeQuery();
	
	while (result.next())
	{
				String str = result.getAtomicValue();
				if (str.indexOf('>') < 0)
				{
						System.out.println(str);
				}

......
```

이 때, 외부로부터 받은 입력 값을 받은 것을 바로 XQuery문에 포함시켜 실행하는 것을 볼 수 있습니다. 

```java
String es = "doc('users.xml')/userlist/user[uname='" + name + "']";
```

이 경우, 만일 외부 입력값이 XQuery 상의 쿼리 구조를 변경시키는 구문일 경우, XQuery Injection이 실행됩니다. 

name의 값을 `something' or '1'='1` 과 같이 전달하게 되면 `doc('users.xml')/userlist/user[uname='something' or '1'='1'` 와 같은 질의문을 수행하게 됩니다. 이로 인해 파일 내의 모든 값을 출력할 수 있게 되어 정보가 유출될 위험이 있습니다. 

따라서 이를 막기 위해 아래와 같이 인자(parameter) 형식으로 전달될 수 있도록 코드를 수정해야 합니다.

```java
String es = "doc('user.xml')/userlist/user[uname='$xpathname']";
expr.bindString(new QName("xpathname"), name, null); 
XQResultSequence result = expr.executeQuery();
```

preparedExpression을 사용하여 bindString, bindInt 등의 함수를 통해 인자 형식으로 전달하는 방식인데요. 이를 통해 외부에서 입력받은 값은 단순히 문자열로 취급되며 XQuery 상의 쿼리 문법으로 인식되지 않습니다. 따라서 외부 입력으로부터 쿼리 구조가 바뀌는 것을 막을 수 있습니다. 

안전한 코드의 전체는 아래와 같습니다.

```java
......

	// 외부로부터 입력 값(name) 받음
	String name = props.getProperty("name");
	Hashtable env = new Hashtable();
	env.put(Context.INITIAL_CONTEXT_FACTORY, "com.sun.jndi.ldap.LdapCtxFactory");
	env.put(Context.PROVIDER_URL, "ldap://localhost:389/o=rootDir");
	javax.naming.directory.DirContext ctx = new InitialDirContext(env);
	javax.xml.xquery.XQDataSource xqds = (javax.xml.xquery.XQDataSource) ctx.lookup("xqj/personnel");
	javax.xml.xquery.XQConnection conn = xqds.getConnection();

	//안전한 코드
	String es = "doc('users.xml')/userlist/user[uname='$xpathname']";
	
	// 입력값이 Xquery 인자로 사용
	XQPreparedExpression expr = conn.prepareExpression(es);
	expr.bindString(new QName("xpathname"), name, null);
	XQResultSequence result = expr.executeQuery();
	
	while (result.next())
	{
				String str = result.getAtomicValue();
				if (str.indexOf('>') < 0)
				{
						System.out.println(str);
				}
```

**다. 예제2**

특정 아이템의 판매가격을 알아오는 코드 중 안전하지 않은 코드의 예시를 한번 볼까요?

### 안전하지 않은 코드

데이터에 대한 코드가 다음과 같이 주어져있습니다.  ⬇️

```xml
<?xml version="1.0"?>
 <items>
	 ...
	 <price>
	 <selling_price>
	 <iem001>100</iem001>
	 <iem002>150</iem001>
	...
	 </selling_price>
	 <buying_price>
	 <iem001>50</iem001>
	 <iem002>140</iem001>
	 ...
	 </buying_price>
	 </price>
	 ...
</items>
```

여기에서 특정 아이템의 판매가격을 알아와봅시다.

string을 연결하는 형태로 구성된 쿼리문으로 작성을 했습니다.  ⬇️

```java
String itemId = request.getParameter(ITEM_ID);
			
			 XQExpression xqe = conn.createExpression();
			 String xqueryString = "doc('items.xml')/items/price/selling_price/" + itemId;
```

이렇게 되면 적절한 검증 절차를 거치지 않고도 특정 아이템의 구매가격을 불러올 수 있습니다. 때문에 원하지 않는 정보가 노출될 위험이 있습니다.

<aside>
💡 그럼 어떻게 안전한 코드를 짤 수 있을까요?

</aside>

이전의 안전하지 않은 코드에서는 동적 쿼리문을 생성할 때 사용하는 외부 입력 값에 대해 적절한 검증 과정을 거치지 않았습니다. 이와 달리 이번에는 동적 생성을 하는 대신에 질의문을 PrepareExpression으로 준비한 후, 입력값을 인자로 전달하여 안전성을 높여주었습니다. 

```java
	 String xqueryString = "doc('items.xml')/items/price/selling_price/'$itemId';
		 //보안성을 위해 prepareExpression으로 질의문 준비
		 xqe = conn.prepareExpression(xqueryString);
```

이런 방식으로 코드를 짜면, 질의문에 대한 구문 분석이 더이상 동적으로 이루어지지 않게 됩니다. 따라서 입력 값에 질의문의 구조가 바뀌는 것을 막아 원하지 않는 정보가 노출되는 현상을 방지합니다.

```java
		 if (command.equals(GET_ITEM_PRICE))
		 {
			 String itemId = request.getParameter(ITEM_ID);
			
			 xqe.bindString(new QName("itemId"), itemId, null);
			 XQResultSequence rs = xqe.executeQuery();
			 ...
		 }
```

안전한 코드와 안전하지 않은 예시의 전체 코드입니다. 

 ⬇️ 전체 코드

**안전하지 않은 코드**

```java

 public class Service extends HttpServlet
 {
	 private final String COMMAND_PARAM = "command";
	
	 // Command 관련정의
	 private final String GET_ITEM_PRICE = "get_item_price";
	
	 private final String ITEM_ID = "item_id";
	
	 private XQDataSource xqs;
	 private XQConnection conn;
	
	 public Service()
	 {
		 ...
		 xqs = new SednaXQDataSource();
		 xqs.setProperty("serverName", "localhost");
		 xqs.setProperty("databaseName", "test");
		 conn = xqs.getConnection("SYSTEM", "MANAGER");
		 ...
	 }
	 ...
	 protected void doPost(HttpServletRequest request, HttpServletResponse response) throws
	ServletException, IOException
	 {
			 String command = request.getParameter(COMMAND_PARAM);
		 ...
		if (command.equals(GET_ITEM_PRICE))
		 {
			 String itemId = request.getParameter(ITEM_ID);
			
			 XQExpression xqe = conn.createExpression();
			 String xqueryString = "doc('items.xml')/items/price/selling_price/" + itemId;
			
			 XQResultSequence rs = xqe.executeQuery(xqueryString);
			 ...
		 }
		 ...
	 }
	 ...
}
```

**안전한 코드**

```java
public class Service extends HttpServlet
 {
	 private final String COMMAND_PARAM = "command";
	
	 // Command 관련정의
	 private final String GET_ITEM_PRICE = "get_item_price";
	
	 private final String ITEM_ID = "item_id";
	
	 private XQDataSource xqs;
	 private XQConnection conn;
	 private XQPrepareExpression xqe;
	
		public Service()
		{
			...
		 xqs = new SednaXQDataSource();
		 xqs.setProperty("serverName", "localhost");
		 xqs.setProperty("databaseName", "test");
		 conn = xqs.getConnection("SYSTEM", "MANAGER");
		 String xqueryString = "doc('items.xml')/items/price/selling_price/'$itemId';
		 //보안성을 위해 prepareExpression으로 질의문 준비
		 xqe = conn.prepareExpression(xqueryString);
		 ...
	 }
	 ...
	 protected void doPost(HttpServletRequest request, HttpServletResponse response) throws
	ServletException, IOException
	 {
		 String command = request.getParameter(COMMAND_PARAM)
		...
		 if (command.equals(GET_ITEM_PRICE))
		 {
			 String itemId = request.getParameter(ITEM_ID);
			
			 xqe.bindString(new QName("itemId"), itemId, null);
			 XQResultSequence rs = xqe.executeQuery();
			 ...
		 }
		 ...
	 }
	 ...
 }
```


라. 출처

행정안전부_JAVA 시큐어코딩 가이드

행정안전부_C 시큐어코딩 가이드
