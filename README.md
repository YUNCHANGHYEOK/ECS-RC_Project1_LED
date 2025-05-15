# Joystick 기반 RGB LED 제어 시스템 (Arduino + R9DS + 조종기)

> **조종기를 활용해 3색 LED 밝기 및 색상 변경, 전체 ON/OFF 제어 시스템**

---

## 📌 프로젝트 목적

이 프로젝트는 **조종기의 입력 신호를 수신기로 받아서 실시간으로 LED 색상 및 밝기를 제어**를 구현하는 것이 목적입니다.

- PWM 신호 처리 및 디지털 인터럽트 학습  
- HSV 색상 시스템과 RGB 변환
- ISR을 이용한 무선 조작 기반 실시간 제어 시스템 구현

---

## 📦 준비물

| 조종기 | 아두이노 UNO | R9DS 수신기 |
|--------|--------------|-------------|
| <img src="https://github.com/user-attachments/assets/fe86fd95-fcfd-40d0-899a-e01683a7d690" width="200px"> | <img src="https://upload.wikimedia.org/wikipedia/commons/3/38/Arduino_Uno_-_R3.jpg" width="200px"> | <img src="https://github.com/user-attachments/assets/0a791965-1cdd-47bd-a8e6-ab88660c91c9" width="200px"> |


---



## ⚙️ 시스템 개요

| 채널 | 제어 항목 | 설명 |
|------|------------|------|
| **CH2 (ELEV)** | 단색 LED 밝기 조절 | 오른쪽 조이스틱 위,아래 → 각기 다른 단색 LED 밝기 조정 |
| **CH3 (THRO)** | 3색 LED RGB 색상 변경 | 왼쪽 조이스틱 위,아래 → HSV 기반 RGB 색상 변화 |
| **CH6 (SWA)** | 전체 LED ON/OFF | 스위치 방식으로 LED 전체 제어 |

---

## 🧠 시스템 작동 원리

- 수신기(R9DS)는 조종기로부터 각 채널별 PWM 신호(1000~2000μs)를 출력  
- 아두이노는 `PinChangeInterrupt`를 통해 각 PWM 신호의 펄스 폭을 측정  
- 펄스 폭 → 제어 변수로 변환 (Hue, Brightness, Power ON/OFF)  
- HSV → RGB로 변환 후 3색 LED 및 단색 LED 출력  

---

## 🔄 작동 흐름도

[조종기 조작]  
↓  
[R9DS 수신기 PWM 출력]  
↓  
[Arduino 입력 핀 수신]  
↓  
[PinChangeInterrupt로 펄스 측정]  
↓  
[map(), constrain() 함수로 값 변환]  
↓  
[HSV → RGB 변환]  
↓  
[analogWrite()로 LED 제어]  

---

## 💡 주요 기능

| 기능 | 설명 |
|------|------|
| 🎨 **HSV 색상 변화** | CH3 조작 → 색상 변화 (Hue 조절) |
| 💡 **단색 LED 밝기 조절** | CH2 조작 → 단색 LED 밝기 세밀 제어 |
| 🔘 **ON/OFF 제어** | CH6 스위치 → 전체 LED 전원 상태 제어 |
| 🛠 **PWM 디코딩** | `PinChangeInterrupt` 라이브러리 사용으로 고정밀 PWM 측정 |

---

## 🔌 아두이노 핀 배치

| 핀 번호 | 기능 | 설명 |
|---------|------|------|
| D4 | CH6 | ON/OFF 제어 |
| D7 | CH3 | HSV 색상 제어 |
| D8 | CH2 | 단색 밝기 제어 |
| D9~D11 | 3색 LED RGB 핀 | PWM 출력 |
| D3/D5/D6 | 단색 LED 핀 | PWM 출력 |

---

## 📄 주요 코드 요약

```cpp
// CH3 (색상) 제어
float hue = map(ch3_pulse, 1000, 2000, 0, 270);

// CH2 (밝기) 제어
int brightness = map(ch2_pulse, 1200, 1800, 0, 255);

// CH6가 ON일 경우
if (ch6_pulse >= 1800) {
  analogWrite(redPin, r);
  analogWrite(greenPin, g);
  analogWrite(bluePin, b);
  analogWrite(yellowPin, brightness);
  analogWrite(green2Pin, brightness);
  analogWrite(blue2Pin, brightness);
} else {
  analogWrite(redPin, 0);
  analogWrite(greenPin, 0);
  analogWrite(bluePin, 0);
  analogWrite(yellowPin, 0);
  analogWrite(green2Pin, 0);
  analogWrite(blue2Pin, 0);
}
```
시리얼 출력 예시
아래 예시 이미지들은 시리얼 모니터에서 출력된 데이터를 시각화한 것입니다:

HSV 밝기 조절 예시
![HSV](https://github.com/user-attachments/assets/c9c89db6-565e-4d52-adde-b92925dbbd83)


3개의 LED 밝기 조절
![그림5](https://github.com/user-attachments/assets/40d29524-1405-4291-99c3-3500322ff692)



ON/OFF 상태 확인
![onoff](https://github.com/user-attachments/assets/d0229c99-99e6-4b97-9f5f-5e87155a02df)


🧪 테스트 결과 및 개선점
✅ PWM 수신 값 안정적 인식 확인 (1000~2000us 범위 정확히 매핑됨)

✅ 색상 전환 부드럽고 빠름, LED 반응 실시간 적용

⚠️ 간헐적 떨림 존재 → PWM 필터링 및 low-pass filter 필요 가능성

🔧 향후 개선 사항:
OLED 디스플레이로 색상 및 밝기 시각화

EEPROM 저장 기능 (전원 OFF 후 마지막 상태 유지)

BLE 모듈 연동으로 모바일 제어 확장

🧰 참고 라이브러리
PinChangeInterrupt: PWM 신호를 인터럽트 기반으로 정확히 읽기 위한 라이브러리

Adafruit_NeoPixel: 다양한 RGB LED 제어 시 확장 가능

FastLED: 고급 LED 제어용 라이브러리

🚀 실행 방법
조종기와 수신기 페어링

R9DS 수신기 PWM 핀을 아두이노에 연결

제공된 .ino 코드 업로드

시리얼 모니터를 통해 입력값 확인

조이스틱/스위치로 조작하며 LED 변화 확인

