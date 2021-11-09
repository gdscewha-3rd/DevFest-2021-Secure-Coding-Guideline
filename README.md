# DevFest 2021 : Secure Coding Guideline

 <p align="center">
    <img src="https://user-images.githubusercontent.com/69420512/140852566-fdd9ac1f-94c5-4854-9173-ebe535458dd5.png" width = 500>
 </p>


GDSC EWHA | 보안과 함께 하는 개발, 시큐어코딩 하루만에 배워 보기

<br>

# Table of Contents

- [SQL Injection: SQL 삽입](https://github.com/gdscewha-3rd/DevFest-2021-Secure-Coding-Guideline#%EF%B8%8F-sql-injection--sql-%EC%82%BD%EC%9E%85)
- [SQL Injection: JDO](https://github.com/gdscewha-3rd/DevFest-2021-Secure-Coding-Guideline#%EF%B8%8F-sql-injection--jdo)
- [SQL Injection: Persistence](https://github.com/gdscewha-3rd/DevFest-2021-Secure-Coding-Guideline#%EF%B8%8F-sql-injection--persistence)
- [SQL Injection: mybatis Data Map](https://github.com/gdscewha-3rd/DevFest-2021-Secure-Coding-Guideline#%EF%B8%8F-sql-injection--mybatis-data-map)
- [XSS Attack(크로스 사이드 스크립트)](https://github.com/gdscewha-3rd/DevFest-2021-Secure-Coding-Guideline#%EF%B8%8F-xss-%EA%B3%B5%EA%B2%A9-%ED%81%AC%EB%A1%9C%EC%8A%A4-%EC%82%AC%EC%9D%B4%EB%93%9C-%EC%8A%A4%ED%81%AC%EB%A6%BD%ED%8A%B8)
- [Xquery 삽입](https://github.com/gdscewha-3rd/DevFest-2021-Secure-Coding-Guideline#%EF%B8%8F-xquery-%EC%82%BD%EC%9E%85)
- [사용자 중요 정보 평문 저장](https://github.com/gdscewha-3rd/DevFest-2021-Secure-Coding-Guideline#%EF%B8%8F-%EC%82%AC%EC%9A%A9%EC%9E%90-%EC%A4%91%EC%9A%94-%EC%A0%95%EB%B3%B4-%ED%8F%89%EB%AC%B8-%EC%A0%80%EC%9E%A5)
- [패스워드 평문 저장](https://github.com/gdscewha-3rd/DevFest-2021-Secure-Coding-Guideline#%EF%B8%8F-%ED%8C%A8%EC%8A%A4%EC%9B%8C%EB%93%9C-%ED%8F%89%EB%AC%B8-%EC%A0%80%EC%9E%A5)
- [오류 메시지를 통한 정보 도출 방지](https://github.com/gdscewha-3rd/DevFest-2021-Secure-Coding-Guideline#%EF%B8%8F-%EC%98%A4%EB%A5%98-%EB%A9%94%EC%8B%9C%EC%A7%80%EB%A5%BC-%ED%86%B5%ED%95%9C-%EC%A0%95%EB%B3%B4-%EB%8F%84%EC%B6%9C-%EB%B0%A9%EC%A7%80)
- [쿠키를 통한 정보 노출 방지](https://github.com/gdscewha-3rd/DevFest-2021-Secure-Coding-Guideline#%EF%B8%8F-%EC%BF%A0%ED%82%A4%EB%A5%BC-%ED%86%B5%ED%95%9C-%EC%A0%95%EB%B3%B4-%EB%85%B8%EC%B6%9C)

---

## ✔️ **SQL Injection : `SQL 삽입`**

가장 **빈번**하게 일어나는 웹 해킹 공격 <br> 웹 어플리케이션 **사용자 입력값**에 **필터링**과 **이스케이프**가 제대로 적용돼 있지 않을 때 발생<br>
공격자가 **조작된 SQL 질의문을 삽입**해 웹 서버 **DB 정보 열람/유출/조작함**
[🚀 Learn More](./sql/SQL_Injection.md)
|JAVA|C|
|:---:|:---:|
| [📖](./sql/SQL%20Injection.md#1-%EC%A0%95%EC%9D%98) | [📖](./sql/SQL%20Injection.m#1-%EC%A0%95%EC%9D%98) |
| [외부 입력 받아 쿼리 생성](./sql/SQL%20Injection.md#%EC%95%88%EC%A0%84%ED%95%98%EC%A7%80-%EC%95%8A%EC%9D%80-%EC%BD%94%EB%93%9Cjava--tablename%EA%B3%BC-name%EC%97%90-%EB%8C%80%ED%95%9C-%EA%B2%80%EC%A6%9D-%EC%88%98%ED%96%89-x) | [외부 입력 받아 쿼리 생성](./sql/SQL%20Injection.md#%EC%95%88%EC%A0%84%ED%95%98%EC%A7%80-%EC%95%8A%EC%9D%80-%EC%BD%94%EB%93%9Cc--%EC%99%B8%EB%B6%80-%EC%9E%85%EB%A0%A5%EC%9D%B4-sql-%ED%80%B4%EB%A6%AC%EC%97%90-%EC%96%B4%EB%96%A0%ED%95%9C-%EC%B2%98%EB%A6%AC-%EC%97%86%EC%9D%B4-%EC%82%BD%EC%9E%85%EB%90%A8)|
| [ID와 Password 추출해 쿼리 생성](./sql/SQL%20Injection.md#%EC%95%88%EC%A0%84%ED%95%98%EC%A7%80-%EC%95%8A%EC%9D%80-%EC%BD%94%EB%93%9Cjava) | [ID와 Password 추출해 쿼리 생성](./sql/SQL%20Injection.md#%EC%95%88%EC%A0%84%ED%95%98%EC%A7%80-%EC%95%8A%EC%9D%80-%EC%BD%94%EB%93%9Cc-querystr%EC%9D%98-%EC%99%B8%EB%B6%80-%EC%9E%85%EB%A0%A5%EC%97%90%EC%84%9C-user_id%EC%99%80-password%EC%9D%98-%EA%B0%92%EC%9D%84-%EC%9E%98%EB%9D%BC-%EA%B7%B8%EB%8C%80%EB%A1%9C-sql%EB%AC%B8-%EC%9D%B8%EC%9E%90-%EA%B0%92%EC%9C%BC%EB%A1%9C-%EC%82%AC%EC%9A%A9) |
| [ Blind SQL injection 공격 구문](./sql/SQL%20Injection.md#6-%EC%98%88%EC%A0%9C-3--blind-sql-injection-%EA%B3%B5%EA%B2%A9-%EA%B5%AC%EB%AC%B8) | . |

---

## ✔️ **SQL Injection : `JDO`**

**Java Data Objects**로, **적절한 검사 과정 없이** **의도에 벗어난 질의문 생성**을 위한 문자열을 전달해 질의문 **의미 왜곡**시키거나 **구조 변경**해 임의의 질의 명령어 수행
[🚀 Learn More](./sql/SQL_Injection_JDO.md)
|JAVA|
|:---:|
| [📖](./sql/SQL_Injection_JDO.md#jdo%EB%9E%80) |
| [Parameterize Query 1](./sql/SQL_Injection_JDO.md#%EC%98%88%EC%A0%9C1---parameterize-query) |
|[Parameterize Query 2](./sql/SQL_Injection_JDO.md#%EC%98%88%EC%A0%9C2---parameterize-query)|

## ✔️ **SQL Injection : `Persistence`**

**J2EE Persistence API** 사용하는 응용프로그램에서 외부의 **입력을 검증 없이 질의문**으로 사용하는 경우, 역시 프로그래머가 의도하지 않았던 **임의의 query 명령어** 수행하는 공격
[🚀 Learn More](./sql/SQL_Injection_Persistence.md)

|                                                                      JAVA                                                                       |
| :---------------------------------------------------------------------------------------------------------------------------------------------: |
|                                  [📖](./sql/SQL_Injection_Persistence.md#api-injection-persistence-%EB%9E%80)                                  |
|            [입력값 검증](./sql/SQL_Injection_Persistence.md#%EC%98%88%EC%A0%9C-1---%EC%9E%85%EB%A0%A5%EA%B0%92-%EA%B2%80%EC%A6%9D)             |
| [문자열 검증 함수](./sql/SQL_Injection_Persistence.md#%EC%98%88%EC%A0%9C2---%EB%AC%B8%EC%9E%90%EC%97%B4-%EA%B2%80%EC%A6%9D-%ED%95%A8%EC%88%98) |

## ✔️ **SQL Injection : `mybatis Data Map`**

외부에서 **입력된 값**이 **쿼리문의 인자값과 쿼리 명령어에 연결되는 문자열**로 사용하는 경우 공격자 **의도에서 벗어난 문자열 전달**해 쿼리문 **의미 왜곡** 또는 **구조 변경**해 임의의 데이터베이스 명령어 수행 가능
[🚀 Learn More](./sql/SQL_Injection_mybatis_Data_Map.md)
|JAVA|
|:---:|
| [📖](./sql/SQL_Injection_mybatis_Data_Map.md#1-%EC%A0%95%EC%9D%98) |
| [질의문 설정 파일 (XML)](./sql/SQL_Injection_mybatis_Data_Map.md#3-%EC%98%88%EC%A0%9C-%EC%BD%94%EB%93%9C1) |
|[사용자 소유 정보 가져오기](./sql/SQL_Injection_mybatis_Data_Map.md#4-%EC%98%88%EC%A0%9C2)|

---

## ✔️ **XSS 공격 (크로스 사이드 스크립트)**

웹페이지에서 **임의의 스크립트를 실행**시켜 사용자 세션을 가로채거나 웹 사이트 변조, 악의적인 컨텐츠 삽입, 피싱 등을 시도하는 공격 [🚀 Learn More](../xss/README.md)

|              JAVA               |                C                |
| :-----------------------------: | :-----------------------------: |
| [📖](./xss/JAVA) | [📖](./xss/C) |
|      [안전하지 않은 Java 코드](./xss/JAVA/non-secure-code.java)      |       [안전하지 않은 C 코드](./xss/C/non-secure-code.c)        |
|      [Java 시큐어 코드](./xss/JAVA/secure-code.java)     |       [C 시큐어 코드](./xss/C/secure-code.c)        |

---

## ✔️ **Xquery 삽입**

악의적인 쿼리가 실행되어 허가되지 않은 데이터를 조회하거나 인증 절차를 우회할 수 있게 되어 위협이 될 수 있다. [🚀 Learn More](./xquery/xquery.md)

|JAVA|
|:---:|
| [📖](./xquery/xquery.md#가-정의) |
|[외부 입력값을 executeQuery 사용 질의 생성 문자열 인자 생성](./xquery/xquery.md#다-예제1)|
|[특정 아이템 가격 가져오기](./xquery/xquery.md#다-예제2)|

---

## ✔️ 사용자 `중요 정보` `평문` 저장

데이터를 **암호화하지 않은 평문**으로 통신 채널을 통해 송수신할 경우, 인증받지 않은 사용자에 의해 발생한 **스니핑**을 통해 보안과 관련된 중요한 **데이터가 노출**될 수 있습니다. [🚀 Learn More](./plaintext/plaintext.md)

|                          JAVA                           |                          ANDROID Java                           |                          C                           |                           C#                           |
| :-----------------------------------------------------: | :-------------------------------------------------------------: | :--------------------------------------------------: | :----------------------------------------------------: |
| [📖](./plaintext/plaintext.md#java-%EC%98%88%EC%A0%9C) | [📖](./plaintext/plaintext.md#android-java-%EC%98%88%EC%A0%9C) | [📖](./plaintext/plaintext.md#c-%EC%98%88%EC%A0%9C) | [📖](./plaintext/plaintext.md#c-%EC%98%88%EC%A0%9C-1) |



## ✔️ `패스워드` `평문` 저장

패스워드를 비롯한 **중요 데이터**가 **암호화되지 않은 평문 텍스트**의 형태로 저장되면 암호가 _외부에 직접_ 드러날 수 있어 **기밀성**이 보장되지 못하고 암호가 저장된 파일에 *접근할 수 있는 사람이면 누구*나 암호를 알아낼 수 있어 **무결성** 또한 보장되지 못합니다. [🚀 Learn More](./plaintext/password.md)

|                          JAVA                          |                           C                            |
| :----------------------------------------------------: | :----------------------------------------------------: |
| [📖](./plaintext/password.md#java-%EC%98%88%EC%A0%9C) | [📖](./plaintext/password.md#c-%EC%98%88%EC%A0%9C) |
|         패스워드 정보 평문으로 DB 저장 취약점          |      패스워드를 파일에서 읽어 직접 DB 연결 취약점      |
|                                                        |              패스워드 입력 미검증 취약점               |

---

## ✔️ `오류 메시지`를 통한 정보 도출 방지

**과도하게 많은 정보**나 민감한 정보를 포함하는 **에러 메시지를 출력**하면, **공격자가 이를 악용**할 수 있기 때문에 보안 취약점이 발생할 수 있습니다. [🚀 Learn More](./errormsg/readme.md)

|                                                                                                                         JAVA                                                                                                                          |                                                                                                                         C                                                                                                                         |
| :---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------: | :-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------: |
|                                                                                                           [📖](./errormsg/javaexposure.md)                                                                                                           |                                                                                                        [📖](./error_message/cexposure.md)                                                                                                        |
|    [스택 트레이스 출력 취약점](./errormsg/javaexposure.md#-%ED%99%98%EA%B2%BD-%EB%B3%80%EC%88%98-%EC%A0%95%EB%B3%B4-%EC%B6%9C%EB%A0%A5--%EC%A0%95%EC%9D%98%EB%90%9C-%EB%AC%B8%EC%A0%9C-%ED%8C%8C%EC%9D%BC-%EB%85%B8%EC%B6%9C-%EA%B0%80%EB%8A%A5)     |   [환경 변수 정보 출력 취약점](./errormsg/cexposure.md#-%ED%99%98%EA%B2%BD-%EB%B3%80%EC%88%98-%EC%A0%95%EB%B3%B4-%EC%B6%9C%EB%A0%A5--%EC%A0%95%EC%9D%98%EB%90%9C-%EB%AC%B8%EC%A0%9C-%ED%8C%8C%EC%9D%BC-%EB%85%B8%EC%B6%9C-%EA%B0%80%EB%8A%A5)    |
|      [예외 내용 출력 취약점](./errormsg/javaexposure.md#-%ED%99%98%EA%B2%BD-%EB%B3%80%EC%88%98-%EC%A0%95%EB%B3%B4-%EC%B6%9C%EB%A0%A5--%EC%A0%95%EC%9D%98%EB%90%9C-%EB%AC%B8%EC%A0%9C-%ED%8C%8C%EC%9D%BC-%EB%85%B8%EC%B6%9C-%EA%B0%80%EB%8A%A5)       | [발생 위치 정보 출력 취약점](./error_message/cexposure.md#-%ED%99%98%EA%B2%BD-%EB%B3%80%EC%88%98-%EC%A0%95%EB%B3%B4-%EC%B6%9C%EB%A0%A5--%EC%A0%95%EC%9D%98%EB%90%9C-%EB%AC%B8%EC%A0%9C-%ED%8C%8C%EC%9D%BC-%EB%85%B8%EC%B6%9C-%EA%B0%80%EB%8A%A5) |
| [에러 페이지 redirect 취약점](./error_message/javaexposure.md#-%ED%99%98%EA%B2%BD-%EB%B3%80%EC%88%98-%EC%A0%95%EB%B3%B4-%EC%B6%9C%EB%A0%A5--%EC%A0%95%EC%9D%98%EB%90%9C-%EB%AC%B8%EC%A0%9C-%ED%8C%8C%EC%9D%BC-%EB%85%B8%EC%B6%9C-%EA%B0%80%EB%8A%A5) |                                                                                                                                                                                                                                                   |

---

## ✔️ `쿠키`를 통한 `정보 노출`

개인 정보나 인증 정보 등이 **하드 디스크의 영속 쿠키**에 저장된다면 개인 **정보 유출** 등의 문제로 시스템이 취약해지는 문제가 발생합니다. [🚀 Learn More](./cookie/readme.md)

|                                                                                                                                 JAVA                                                                                                                                 |  
| :------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------: | 
|                                                                                                                    [📖](./cookie/javacookies.md)                                                                                                                    |     |
| [쿠키 유효 기간 설정 시 사용자 미검증 취약점](./cookie/javacookies.md#-%EC%99%B8%EB%B6%80-%EC%9E%85%EB%A0%A5%EC%9D%B4-%EC%BF%A0%ED%82%A4-%EC%9C%A0%ED%9A%A8-%EA%B8%B0%EA%B0%84-%EC%84%A4%EC%A0%95%EC%97%90-%EA%B7%B8%EB%8C%80%EB%A1%9C-%EC%82%AC%EC%9A%A9%EB%90%A8) |     |
|                                  [쿠키 유효 기간 상숫값 설정 취약점](./cookie/javacookies.md#-%EC%BF%A0%ED%82%A4-%EC%9C%A0%ED%9A%A8-%EC%8B%9C%EA%B0%84-%EC%84%A4%EC%A0%95%EC%9D%B4-%EC%83%81%EC%88%AB%EA%B0%92%EC%9D%B4-%EB%90%A8)                                  |     |


---
## GDSC EWHA TIMELINE

|     시간      |                                   주제                                    |                           강연자                           |
| :-----------: | :-----------------------------------------------------------------------: | :--------------------------------------------------------: |
| 19:00~19:10 |                                  오프닝 & 개발에 바로 활용할 수 있는 Secure Coding Guideline 소개                                    | GDSC EWHA                                                           |
| 19:10~19:45 |            실 사례를 통해 알아보는 보안 개발의 필요성과 해결책            |                 라도훈<br>스타트업 SERENDI                 |
| 19:40~20:20 |                          Secure Coding Guideline                          | 이채영, 전세연<br>2020 소프트웨어 개발보안 경진대회 대상팀 |
|  20:20~21:00  | 안전하고 빠른 코드를 얻기 위한 우리의 몸부림(TDD, Code Converage Profile) |              최원혁<br> (주)누리랩 대표 이사               |

---

### Organizer

[**GDSC EWHA**](https://github.com/gdscewha-3rd) | [구민정](https://github.com/mj982) [김소미](https://github.com/rlathal009) [김윤서](https://github.com/ottl-seo)
[배수현](https://github.com/SoohyeonB)
[윤하은](https://github.com/Haeun-Y) [이은빈](https://github.com/binable43)
[장아연](https://github.com/JangAyeon) [장예서](https://github.com/yesyeseo)
[하수민](https://github.com/minha62)

---


### Reference

행정안전부, 전자정부 SW 개발·운영자를 위한 소프트웨어 개발보안 가이드, 2019. 11

행정안전부, 전자정부 SW 개발·운영자를 위한 Java 시큐어코딩 가이드, 2012. 09

안전행정부, Android-Java 시큐어 코딩 가이드, 2011. 09

XSS 공격과 대응방안, 디지털정책연구 = The Journal of digital policy & management v.11 no.12, 2013년, pp.327 – 332, 홍성혁 (백석대학교, 정보통신학부 정보보호 전공)

크로스 사이트 스크립팅(XSS) 취약점에 대한 공격과 방어, 디지털융복합연구 = Journal of digital convergence v.13 no.2 , 2015년, pp.177 – 183, 최은정 (서울여자대학교 정보보호학과 )
