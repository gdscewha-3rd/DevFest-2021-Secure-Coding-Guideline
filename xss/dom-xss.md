
# 3) DOM기반 XSS

## **3-1) 정의**

외부에서 입력 받은 악성 스크립트가 포함된 URL 파라미터 값이 서버를 거치지 않고, DOM 생성의 일부로 실행되면서 공격한다. Reflected XSS 및 Stored XSS 공격은 서버 애플리케이션 취약점으로 인해, 응답 페이지에 악성 스크립트가 포함되어 브라우저로 전달되면서 공격하는 것인 반면, DOM기반 XSS는 서버와 관계없이 발생하는 것이 차이점이다.  일반적으로 DOM 기반 XSS 취약점 있는 브라우저를 대상으로 조작된 URL을 이메일을 통해 사용자에게 전송하면, 피해자는 이 URL 링크를 클릭하는 순간 공격 피해를 입게 된다. 
 
 ![image](https://user-images.githubusercontent.com/90726898/140514222-4143991c-7bfc-44ed-8fe7-36680fec4784.png)

## **3-2) 예시 공격 코드**
DOM 스크립트 (정상적으로 동작) 
```html
<HTML> <br>
 <TITLE>Welcome!</TITLE> <br>
 Hi <br>
 <script> <br>
  var pos=document.URL.indexOf("name=")+5; <br>
  document.write(document.URL.substring(pos,document.URL.length)); <br>
 </script> <br>
 <br>
 Welcome to our system <br>
 This demo borrowed from http://www.webappsec.org/projects/articles/071105.shtml <br>
</HTML>
```

DOM 기반 XSS 공격 URL, 사용자가 URL 링크를 열면 브라우저에서 이 스크립트의 내용이 실행된다.

`http://www.server.com/page.html?name=<script>alert(document.cookie)</script>` 
