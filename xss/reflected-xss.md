# 2) 반사 XSS (Reflected XSS)
## 2-1) 정의

검색 결과, 에러 메시지 등을 통해 서버가 외부에서 입력 받은 악성 스크립트가 포함된 URL 파라미터 값을 사용자 브라우저에서 응답할 때 발생한다. 공격 스크립트가 삽입된 URL을 사용자가 쉽게 확인할 수 없도록 변형하여, 이메일, 메신저, 파일등을 통해 실행을 유도하는 공격이다.

![image](https://user-images.githubusercontent.com/90726898/140514026-f992023d-88c3-44a5-a6da-00e583ee0097.png)
 
## 2-2) 예시 공격 코드
XSS 공격용 URL : `http://www.server.com/search/?q=<script>alert(document.cookie)</script>&x=0&y=0`

```html
<html> <br>
 <body> <br>
  <div id="pageTitleTxt"> <br>
  <h2><span class="highlight">Search Results</span><br /> <br>
  Search: "<script>alert(document.cookie)</script>"</h2> <br>
 </body> <br>
</html> //서버가 반사한 HTML 데이터 , 사용자가 URL 링크를 열면 브라우저에서 이 스크립트의 내용이 실행된다. 
```
