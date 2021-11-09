# [에러 처리] 오류 메시지를 통한 정보 노출

 - **에러를 적절하게 처리하지 않았을 때** 발생하는 보안 취약점
    - 에러 정보에 **과도하게 많은 정보를 포함**하면, 공격자가 이를 악용할 수 있기 때문에  보안취약점이 발생할 수 있습니다.

## 정의

- `오류 메시지`에 포함된 **정보가 노출**될 때 발생
	- 민감한 정보를 포함하는 오류 메시지 생성하여 외부에 제공하는 경우, 공격자의 악성 행위를 도와줄 수 있음
	- 예외 발생 시 예외 이름이나 스택트레이스를 출력하는 경우 프로그램 내부 구조를 쉽게 파악 가능함


## 안전한 코딩 기법

- 오류 메시지는 **정해진** 사용자에게 **유용한** **최소한의** 정보만 포함하도록 해야 한다.
- 소스 코드에서 **예외 사항**은 **내부적**으로 처리
- 사용자에게 민감한 정보를 **포함하는** 오류를 출력하지 **않도록** 설정
- 적절한 **환경설정**을 통해 에러 정보를 **노출하지 않고,** 미리 **정의된 페이지**를 제공하도록 설정


--- 

### 💖 [JAVA에서의 보안 강화](./javaexposure.md)
- [스택 트레이스 출력 취약점](https://github.com/yesyeseo/DevFest-2021-Ewha-Security-Archive/blob/errormsg/errormsg/cexposure.md#-%ED%99%98%EA%B2%BD-%EB%B3%80%EC%88%98-%EC%A0%95%EB%B3%B4-%EC%B6%9C%EB%A0%A5--%EC%A0%95%EC%9D%98%EB%90%9C-%EB%AC%B8%EC%A0%9C-%ED%8C%8C%EC%9D%BC-%EB%85%B8%EC%B6%9C-%EA%B0%80%EB%8A%A5)
- [예외 내용 출력 취약점](https://github.com/yesyeseo/DevFest-2021-Ewha-Security-Archive/blob/errormsg/errormsg/cexposure.md#-%ED%99%98%EA%B2%BD-%EB%B3%80%EC%88%98-%EC%A0%95%EB%B3%B4-%EC%B6%9C%EB%A0%A5--%EC%A0%95%EC%9D%98%EB%90%9C-%EB%AC%B8%EC%A0%9C-%ED%8C%8C%EC%9D%BC-%EB%85%B8%EC%B6%9C-%EA%B0%80%EB%8A%A5)
- [에러 페이지 redirect 취약점](https://github.com/yesyeseo/DevFest-2021-Ewha-Security-Archive/blob/errormsg/errormsg/cexposure.md#-%ED%99%98%EA%B2%BD-%EB%B3%80%EC%88%98-%EC%A0%95%EB%B3%B4-%EC%B6%9C%EB%A0%A5--%EC%A0%95%EC%9D%98%EB%90%9C-%EB%AC%B8%EC%A0%9C-%ED%8C%8C%EC%9D%BC-%EB%85%B8%EC%B6%9C-%EA%B0%80%EB%8A%A5)


### 💖 [C에서의 보안 강화](./cexposure.md)
- [환경 변수 정보 출력 취약점](https://github.com/yesyeseo/DevFest-2021-Ewha-Security-Archive/blob/errormsg/errormsg/cexposure.md#-%ED%99%98%EA%B2%BD-%EB%B3%80%EC%88%98-%EC%A0%95%EB%B3%B4-%EC%B6%9C%EB%A0%A5--%EC%A0%95%EC%9D%98%EB%90%9C-%EB%AC%B8%EC%A0%9C-%ED%8C%8C%EC%9D%BC-%EB%85%B8%EC%B6%9C-%EA%B0%80%EB%8A%A5)
- [발생 위치 정보 출력 취약점](https://github.com/yesyeseo/DevFest-2021-Ewha-Security-Archive/blob/errormsg/errormsg/cexposure.md#-%ED%99%98%EA%B2%BD-%EB%B3%80%EC%88%98-%EC%A0%95%EB%B3%B4-%EC%B6%9C%EB%A0%A5--%EC%A0%95%EC%9D%98%EB%90%9C-%EB%AC%B8%EC%A0%9C-%ED%8C%8C%EC%9D%BC-%EB%85%B8%EC%B6%9C-%EA%B0%80%EB%8A%A5)
