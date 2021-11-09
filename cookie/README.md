# 하드 디스크에 있는 쿠키를 통한 정보 노출

 - **에러를 적절하게 처리하지 않았을 때** 발생하는 보안 취약점
    - 에러 정보에 **과도하게 많은 정보를 포함**하면, 공격자가 이를 악용할 수 있기 때문에  보안취약점이 발생할 수 있습니다.

## 정의

- 웹 응용 프로그램에서의 쿠키는 `메모리`에 상주, 브라우저의 실행 종료 시 사라짐
    - 원하는 경우 **세션에 상관없이 지속적으로 저장**되도록 설정 가능 → `디스크`에 기록
    - 기록된 후, 다음 브라우저 세션이 시작되었을 때 `메모리`에 로드됨


개인 정보나 인증 정보 등이 `persistent cookie`에 저장된다면 시스템이 취약해짐

## 안전한 코딩 기법

- 쿠키의 **만료 시간**은 세션 지속 시간과 관련해 **최소한**으로 설정
- 영속적인 쿠키에는 사용자 권한 등급, 세션 ID 등이 **포함되지 않도록** 함

--- 

### 💖 [JAVA에서의 보안 강화](./javacookies.md)
- [쿠키 유효 기간 설정 시 사용자 미검증 취약점](https://github.com/yesyeseo/DevFest-2021-Ewha-Security-Archive/blob/cookie/cookie/javacookies.md#-%EC%99%B8%EB%B6%80-%EC%9E%85%EB%A0%A5%EC%9D%B4-%EC%BF%A0%ED%82%A4-%EC%9C%A0%ED%9A%A8-%EA%B8%B0%EA%B0%84-%EC%84%A4%EC%A0%95%EC%97%90-%EA%B7%B8%EB%8C%80%EB%A1%9C-%EC%82%AC%EC%9A%A9%EB%90%A8)
- [쿠키 유효 기간 상숫값 설정 취약점](https://github.com/yesyeseo/DevFest-2021-Ewha-Security-Archive/blob/cookie/cookie/javacookies.md#-%EC%BF%A0%ED%82%A4-%EC%9C%A0%ED%9A%A8-%EC%8B%9C%EA%B0%84-%EC%84%A4%EC%A0%95%EC%9D%B4-%EC%83%81%EC%88%AB%EA%B0%92%EC%9D%B4-%EB%90%A8)