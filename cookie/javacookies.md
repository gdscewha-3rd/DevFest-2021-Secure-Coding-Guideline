# JAVA에서의 안전하지 않은 코드

## 😢 외부 입력이 쿠키 유효 기간 설정에 `그대로` 사용됨

`javax.servlet.http.Cookie.setMaxAge` 메소드 호출에 **외부의 입력**이 쿠키의 **유효 시한** 설정에 **그대로** 사용되어 프로그램의 취약점이 야기된다.

```java
public void makeCookie(ServletRequest request) {
    String maxAge = request.getParameter("maxAge");
    if (maxAge.matches("[0-9]+")) {
        String sessionID = request.getParameter("sessionID");
        if (sessionID.matches("[A-Z=0-9a-z]+")) {
            Cookie c = new Cookie("sessionID", sessionID);
            // 외부 입력이 쿠키 유효시한 설정에 그대로 사용됨!
            c.setMaxAge(Integer.parseInt(maxAge));
        }
    }
}
```

### 안전한 코드

사용자가 요청한 값으로 쿠키의 유효 기간을 설정하기 전에 **사용자 요청을 검증**하는 로직을 작성하여 **메소드 호출 전에** 호출한다.

```java
public void makeCookie(ServletRequest request) {
    String maxAge = request.getParameter("maxAge");

    if (maxAge == null || "".equals(maxAge)) return;
    // 무기한의 기한 설정 방지
    if (maxAge.matches("[0-9]+")) {
        String sessionID = request.getParameter("sessionID");
        if (maxAge == null || "".equals(maxAge)) return;
        // 무기한 기한 설정 방지
        if (sessionID.matches("[A-Z=0-9a-z]+")) {
            Cookie c = new Cookie("sessionID", sessionID);
            int t = Integer.parseInt(maxAge);
            if (t > 3600) { t = 3600; }
            // 3600초 초과하면 3600초로 설정
            c.setMaxAge(t);
        }
    }
}
```

위 코드에서는 앞선 코드와 달리 1. **무기한**일 경우에는 그냥 return을 해 준다거나, 2. **일정 기간 이상**(위 코드에서는 3600초)일 경우에는 자연스럽게 **사용자 지정 최댓값**으로 바꾸어 주며 취약점을 제거했다.

---

## 😢 쿠키 유효 시간 설정이 `상숫값`이 됨 

`javax.servlet.http.Cookie.setMaxAge` 메소드 호출에 쿠키의 **유효 기간 설정을 상숫값**으로 사용하여 프로그램의 취약점을 야기함.

```java
...
Cookie idCookie = new Cookie("id", userId);
Cookie passwordCookie = new Cookie("password", password);

idCookie.setMaxAge(60*60); // 넉넉하게 쿠키 지속 시간 설정
passwordCookie.setMaxAge(60*60);
...
```

### 안전한 코드

setMaxAge값을 `음수`로 설정하면 브라우저가 **떠 있는 동안에만 작동**한다.
이를 이용하여, 로그아웃 이외의 어떤 행동을 해도 **음수로 설정**되게 하였다.
로그아웃 시에는 즉시 쿠키가 삭제된다.

```java
...
if (command.equals(LOGIN_CMD)) {
    ...
    Cookie idCookie = new Cookie("id", userId);
    Cookie passwordCookie = new Cookie("password", password);

    idCookie.setMaxAge(-60 * 2); // 넉넉하게 쿠키 지속 시간 설정
    passwordCookie.setMaxAge(-60 * 2);
}

...

if (command.equals(LOGOUT_CMD)) {
    ...
    idCookie.setMaxAge(0); // 로그아웃 시 쿠키 바로 삭제
    passwordCookie.setMaxAge(0);
}

...

if (command.equals(OTHER_ACTION_CMD)) {
    ...
    idCookie.setMaxAge(-60 * 2); // 사용자가 계속 사용해야만 쿠키 갱신됨
    passwordCookie.setMaxAge(-60 * 2);
}