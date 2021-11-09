# 4. SQL 삽입 공격: mybatis Data Map

## 1) 정의

외부에서 입력된 값이 쿼리문의 **인자값**과 쿼리 **명령어에 연결되는 문자열**로 사용하는 경우 공격자 의도에서 벗어난 문자열 전달해 **쿼리문 의미 왜곡 또는 구조 변경**해 임의의 데이터베이스 명령어 수행 가능

## 2) 해결 방법

- 외부 입력에서 위험한 문자나 의도 벗어난 입력 제거하는 코드 프로그램 내에 추가

- mybatis Data Map 파일의 인자를 받는 쿼리 명령어 정의할 때 문자열 삽인 인자($...$) 사용 X <br>
  -> #<인자이름># 형태의 쿼리문 사용

## 3) 예제[1]

> mybatis Data Map에서 사용하는 질의문 설정파일(XML)

<br>

**안전하지 않은 코드(java)** : <br>

정의된 쿼리문 중 `delStudent` 명령어 선언에서 쿼리문 삽입 인자 중 `$name$`로 전달되는 문자열 그대로 연결해 질의문 생성<br>
`name`의 값으로 `' OR 'x'='x'` 전달시 `(DELETE STUDENTS WHERE NUM = #num# and Name = '' OR 'x'='x')` 퀴리문 수행되어 테이블의 모든 원소 삭제함

```java
<?xml version="1.0" encoding="UTF-8"?>

<!DOCTYPE sqlMap PUBLIC "-//iBATIS.com//DTD SQL Map 2.0//EN" "http://www.ibatis.com/dtd/sql-map-2.dtd">

<sqlMap namespace="Student">

	<resultMap id="StudentResult" class="Student">

		<result column="ID" property="id" />

		<result column="NAME" property="name" />

	</resultMap>

	<select id="listStudents" resultMap="StudentResult">

		SELECT NUM, NAME

		FROM STUDENTS

		ORDER BY NUM

	</select>

	<select id="nameStudent" parameterClass="Integer" resultClass="Student">

		SELECT NUM, NAME

		FROM STUDENTS

		WHERE NUM = #num#

	</select>

	<!-- dynamic SQL 사용 -->

	<delete id="delStudent" parameterClass="Student">

		DELETE STUDENTS

		WHERE NUM = #num# AND Name = '$name$'

	</delete>

</sqlMap>

```

<br>

**안전한 코드(java)** : <br>

`Name` 인자를 `#name#` 형태로 받도록 수정

```java
<?xml version="1.0" encoding="UTF-8"?>

<!DOCTYPE sqlMap PUBLIC "-//iBATIS.com//DTD SQL Map 2.0//EN" "http://www.ibatis.com/dtd/sql-map-2.dtd">

<sqlMap namespace="Student">

	<resultMap id="StudentResult" class="Student">

		<result column="ID" property="id" />

		<result column="NAME" property="name" />

	</resultMap>

	<select id="listStudents" resultMap="StudentResult">

		SELECT NUM, NAME

		FROM STUDENTS

		ORDER BY NUM

	</select>

	<select id="nameStudent" parameterClass="Integer" resultClass="Student">

		SELECT NUM, NAME

		FROM STUDENTS

		WHERE NUM = #num#

	</select>



	<!-- static SQL 사용 -->

	<delete id="delStudent" parameterClass="Student">

		DELETE STUDENTS

		WHERE NUM = #num# AND Name = '#name#'

	</delete>

</sqlMap>

```

## 4) 예제[2]

> 사용자가 소유하고 있는 item에 대한 정보 얻어 오고 있음

<br>

**안전하지 않은 코드(java)** : <br>
`$itemName$` 사용해 동적 질의문 생성<br>
`itemname`을 `name'; DROP items; --`로 주면 쿼리문 수행 결과는 ` SELECT * FROM items WHERE owner=``user name`` AND itemname= ``item name``; `과 `DROP items;` 실행 결과 동일<br>
-> DB의 items 테이블 정보 삭제됨

```java
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE sqlMap PUBLIC "-//iBATIS.com//DTD SQL Map 2.0//EN"
"http://www.ibatis.com/dtd/sql-map-2.dtd">
 <sqlMap namespace="UserItem">
 ...
 <select id="getItems" parameterClass="MyClass" resultClass="items">
 SELECT * FROM items WHERE owner = #userName# AND itemname = '$itemName$'
 </select>
 ...
 </sqlMap>
```

**안전한 코드(java)** : <br>
`$itemName$` 대신 `#itemName#`사용해 정적 질의문 생성<br>
`itemname`을 `name'; DROP items; -`하면 `itemname`의 이름이 `name'; DROP items; -`인 항목 찾기 때문에 쿼리문 의도에서 벗어난 동작 방지 가능

```java
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE sqlMap PUBLIC "-//iBATIS.com//DTD SQL Map 2.0//EN"
"http://www.ibatis.com/dtd/sql-map-2.dtd">
 <sqlMap namespace="UserItem">
 ...
 <select id="getItems" parameterClass="MyClass" resultClass="items">
 SELECT * FROM items WHERE owner = #userName# AND itemname = '#itemName#'
 </select>
 ...
 </sqlMap>
```
