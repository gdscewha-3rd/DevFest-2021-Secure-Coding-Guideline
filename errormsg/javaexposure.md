# JAVA에서의 안전하지 않은 코드

--- 

## 😢 스택 트레이스 출력 → 프로그램 내부 정보 유출

```java
public staic void main(String[] args) {
    String urlString = args[0];
    try {
        URL url = new URL(urlString);
        URLConnection cmx = url.openConnection();
        cmx.connect();
    } catch (Exception e) {
        // error를 catch하면 Stack Trace를 출력한다.
        e.printStackTrace();
    }
}
```
위 코드에서는 Exception을 catch 했을 때 **Stack Trace**를 출력하기 때문에 프로그램 내부 정보가 유출될 수 있다.

![image](https://user-images.githubusercontent.com/69420512/140457922-745d4d31-ec86-45b3-b6ac-a6c01861a128.png)

(참고: https://www.educative.io/edpresso/what-is-the-printstacktrace-method-in-java)

### 안전한 코드

```java
public staic void main(String[] args) {
    String urlString = args[0];
    try {
        URL url = new URL(urlString);
        URLConnection cmx = url.openConnection();
        cmx.connect();
    } catch(Exception e) {
        System.out.println("연결 예외 발생");
    }
}
```

그래서 StackTrace를 print하는 것이 아닌, 위처럼 `System.out.println("연결 예외 발생");`처럼 정보를 유출하지 않는 문구를 출력해 준다.

## 😢 예외 내용 출력 → 시스템 정보 유출

```java
public void ReadConfiguration() {
    try {
        BufferedReader in = new BufferedReader(new FileReader("config.cfg"));
        configuration.option1 = in.readLine();
        configuration.option2 = in.readLine();
        configuration.option3 = in.readLine();
    } catch(Exception e) {
        System.out.println(e.toString);
    }
}
```

이 코드 또한 예외 발생 시 예외 내용을 출력하기 때문에 취약하다. **구체적인 예외 내용에 대해 전혀 알 수 없도록** 만드는 것이 가장 안전하다.

### 안전한 코드

```java
public void ReadConfiguration() {
    try {
        BufferedReader in = new BufferedReader(new FileReader("config.cfg"));
        configuration.option1 = in.readLine();
        configuration.option2 = in.readLine();
        configuration.option3 = in.readLine();
    } catch(Exception e) {
        System.out.println("환경 설정을 실패하였습니다");
    }
}
```

## 😢 에러 페이지로 redirect → 모두 같은 에러 페이지 보여 주기


에러가 발생하면 무조건 `에러 페이지로 강제 redirect` 시키는 것은 위험하다. 공격자에게 정보를 노출시키지 않기 위해서는 외부 서비스 시 보이는 에러에 대해 발생 **종류에 따른 차이를 제거**하고 **모두 같은 에러 페이지**만을 보여주는 것이 안전하다.

### 안전한 코드

```java
    } catch(NumberFormatException e) {
        RequestDispatcherdispatcher = request.get.RequestDispatcher("data-error.jsp"); // 에러 발생 처리 주소
        // 에러 발생 처리 주소
        dispatcher.forware(request, response);
        // 에러 주소 페이지 전환
    }
}
```