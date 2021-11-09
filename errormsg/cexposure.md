# C에서의 안전하지 않은 코드

--- 

## 😢 환경 변수 정보 출력 → 정의된 문제 파일 노출 가능

아래 코드처럼 환경 변수의 정보를 출력하면 공격자가 환경 변수에 정의된 문제 파일을 알 수 있다.

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]) {
    char* path = getenv("MYPATH");
    fprintf(stderr, path);
    return 0;
}
```

### 안전한 코드

오류 정보에 대한 공개는 사용자가 유추할 수 없도록 **최대한 간단하게** 사용하는 것이 안전한다.
아래 코드에서는 fprintf 구문을 제거해서 오류 정보를 공개하지 않도록 했다.

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]) {
    char* path = getenv("MYPATH");
    return 0;
}
```


## 😢 발생 위치 정보 출력 → 시스템 환경 정보 노출

발생 위치에 대한 정보와 세부 내용을 출력하면 공격자가 **시스템 환경에 대한 정보**를 수집할 수 있게 된다.


```c
public void ReadConfiguration() {
    char buffer[BUFFER_SIZE];
    FILE * fp = fopen("config.cfg", "r");
    
    if(fp == NULL) {
        printf("config.cfg를 찾을 수/열 수 없습니다.");
        return;
    }
    
    if(fgets(buffer, BUFFER_SIZE, fp) == NULL) {
        printf("config.cfg에서 option2를 읽을 수 없습니다.");
    }
    strlcpy(configuration.option2, buffer);
    
    if(fgets(buffer, BUFFER_SIZE, fp) == NULL) {
        printf("config.cfg에서 option1을 읽을 수 없습니다.");
    }
    strlcpy(configuration.option1, buffer);
        
    if(fgets(buffer, BUFFER_SIZE, fp) == NULL) {
        printf("config.cfg에서 option3을 읽을 수 없습니다.");
    }
    strlcpy(configuration.option3, buffer);
}
```

위 코드는 **옵션에 따라 나타나는 메시지가 다르기 때문에** 공격자가 유추 가능하다.

### 안전한 코드

어떠한 종류의 에러가 발생하든 공격자는 **한 가지 메시지**밖에 볼 수 없게 되어 있다.

```c
public int _ReadConfiguration(Configuration* configuration) {
    char buffer[BUFFER_SIZE];
    FILE* fp = fopen("config.cfg", "r");
    if(fp == NULL) { return False; }
    
    if(fgets(buffer, BUFFER_SIZE, fp) == NULL) { return FALSE; }
    strlcpy(configration->option1, buffer);
    
    if(fgets(buffer, BUFFER_SIZE, fp) == NULL) { return FALSE; }
    strlcpy(configration->option2, buffer);
    
    if(fgets(buffer, BUFFER_SIZE, fp) == NULL) { return FALSE; }
    strlcpy(configration->option3, buffer);
    
    return TRUE;
}

public void ReadConfiguration(Configuration* configuration) {
    if (_ReadConfiguration(configuration) == FALSE) { printf("환경 설정에 실패하였습니다."); }
}
```