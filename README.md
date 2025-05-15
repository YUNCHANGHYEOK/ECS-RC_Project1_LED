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

## 🌈 기존 RGB 방식 vs HSV 방식 비교 설명
색상을 프로그래밍으로 제어할 때, 가장 많이 사용하는 두 가지 방식은 RGB와 HSV입니다.   
이 프로젝트에서는 더 직관적인 HSV 방식을 선택하여 사용자 조작이 부드럽고 예측 가능하도록 개선했습니다.

### ✅ 기존 RGB 방식: R, G, B 값을 각각 조정
색을 표현할 때, 빨강(R), 초록(G), 파랑(B) 세 가지 값을 별도로 제어

- 예를 들어, 노란색을 만들기 위해서는 (255, 255, 0)처럼 직접 혼합 비율을 계산

- 조이스틱의 단일 축(CH3)으로는 3가지 값을 동시에 컨트롤하기 어려움

- 색상 전환 시 예상치 못한 색이나 어색한 변화가 발생하기 쉬움

- ❌ 문제점: 직관성이 낮고, 하나의 입력으로 다채로운 색상 표현이 어렵다

### ✅ HSV 방식: H(색상), S(채도), V(명도)로 분리 제어
- 색의 종류(Hue) 를 하나의 값(도수, 예: 0°=빨강, 120°=초록, 240°=파랑)으로 제어

- 채도와 밝기를 고정한 채, 하나의 슬라이더로 색상환을 회전하며 색상을 전환 가능

- 조이스틱 한 축(CH3)으로 전체 색상 스펙트럼을 부드럽게 순환 가능

```cpp
float hue = map(ch3_pulse, 1000, 2000, 0, 270);     // Hue만 조절
hsvToRgb(hue, 1.0, 1.0, r, g, b);                   // 변환 후 RGB 출력
```

- ✅ 장점: 색상 전환이 자연스럽고 부드러우며, 직관적인 제어 가능
