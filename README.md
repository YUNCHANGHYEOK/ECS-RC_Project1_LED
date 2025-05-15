# Joystick 기반 RGB LED 제어 시스템 (Arduino + R9DS + 조종기)

> **조종기를 활용해 3색 LED 밝기 및 색상 변경, 전체 ON/OFF 제어 시스템**

---
## 📺 Youtube 시연 영상 (사진을 클릭하면 영상 재생)

<a href="https://youtu.be/5PipfmS6rMY">
  <img src="https://img.youtube.com/vi/5PipfmS6rMY/hqdefault.jpg" width="600px">
</a>

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

## 실물 회로 사진
---
<table>
  <tr>
    <td><img src="https://github.com/user-attachments/assets/b67f880a-3e50-420a-98b6-bfa86cda876b" width="200"/></td>
    <td><img src="https://github.com/user-attachments/assets/14b94bdf-4e68-4e8d-aaf5-43ee1cefa4f7" width="200"/></td>
    <td><img src="https://github.com/user-attachments/assets/ff36948a-9a96-4cd4-9f8b-faa148b2de80" width="200"/></td>
    <td><img src="https://github.com/user-attachments/assets/0726f9ef-0bd9-426c-bef6-f0210d75eb17" width="200"/></td>
  </tr>
</table>

# 짧은 시연영상
---
<table>
  <tr>
    <td>
      <a href="https://youtube.com/shorts/spuMrNXNdfk?feature=share">
        <img src="https://img.youtube.com/vi/spuMrNXNdfk/hqdefault.jpg" width="250px">
      </a>
    </td>
    <td>
      <a href="https://youtube.com/shorts/Rbt6Wvomx9I?feature=share">
        <img src="https://img.youtube.com/vi/Rbt6Wvomx9I/hqdefault.jpg" width="250px">
      </a>
    </td>
    <td>
      <a href="https://youtube.com/shorts/QOnhlJ7e9qI?feature=share">
        <img src="https://img.youtube.com/vi/QOnhlJ7e9qI/hqdefault.jpg" width="250px">
      </a>
    </td>
  </tr>
</table>

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

| 아두이노 핀 번호 | 연결된 RC 채널 | 기능                | 설명                                                  |
|------------------|----------------|---------------------|-------------------------------------------------------|
| D4               | CH6 (SWA)      | 전체 LED ON/OFF 제어 | 스위치 방식 제어 (위/아래)                             |
| D7               | CH3 (THRO)     | HSV 색상 제어        | 좌측 조이스틱 위/아래로 RGB 색상 변경                |
| D8               | CH2 (ELEV)     | 단색 LED 밝기 제어   | 우측 조이스틱 위/아래로 단색 LED 밝기 조절           |
| D9 ~ D11         | N/A            | RGB LED 출력         | `analogWrite()`로 PWM 제어 (RGB LED에 각각 연결)      |
| D3 / D5 / D6     | N/A            | 단색 LED 출력        | 각 단색 LED에 동일한 밝기 적용 (PWM 출력 핀 사용)    |

---

## 📄 주요 코드 요약

```cpp
// CH3 (색상) 제어
float hue = map(ch3_pulse, 1000, 2000, 0, 360);

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

## 🎛 시리얼 출력 예시

- **HSV 밝기 조절 예시**  
  <img src="https://github.com/user-attachments/assets/c9c89db6-565e-4d52-adde-b92925dbbd83" width="400px">

- **3개의 LED 밝기 조절**  
  <img src="https://github.com/user-attachments/assets/40d29524-1405-4291-99c3-3500322ff692" width="400px">

- **ON/OFF 상태 확인**  
  <img src="https://github.com/user-attachments/assets/d0229c99-99e6-4b97-9f5f-5e87155a02df" width="400px">

## 🌈 기존 RGB 방식 vs HSV 방식 비교 설명

색상을 프로그래밍으로 제어할 때 가장 많이 사용하는 두 가지 방식은 **RGB**와 **HSV**입니다.  
이 프로젝트에서는 **더 직관적인 HSV 방식을 채택**하여 사용자 조작을 부드럽고 예측 가능하게 개선하였습니다.

---

### ✅ 기존 RGB 방식: R, G, B 값을 각각 조정

- 색을 표현할 때 **빨강(R), 초록(G), 파랑(B)** 값을 각각 조절
- 예: **노란색 → (255, 255, 0)** 등 직접 혼합 비율 계산 필요
- 조이스틱 한 축(CH3)으로는 **3가지 값을 동시에 컨트롤하기 어려움**
- 색상 전환 시 예상치 못한 색이 나오거나 전환이 **불연속적이고 어색할 수 있음**

> ❌ **문제점**: 직관성이 낮고, 하나의 입력으로 다양한 색상 표현이 어려움

---

### ✅ HSV 방식: H(색상), S(채도), V(명도)로 분리 제어

#### 🔎 HSV란?

| 요소 | 설명 |
|------|------|
| **Hue (색상)** | 색의 종류, 0°~360° 범위 (예: 0°=빨강, 120°=초록, 240°=파랑) |
| **Saturation (채도)** | 색의 선명도, 0은 회색 / 1.0은 순색 |
| **Value (명도)** | 색의 밝기, 0은 검정 / 1.0은 최대 밝기 |

---

- 본 프로젝트에서는 **Hue만 가변 조절**, S와 V는 `1.0`으로 고정하여 **선명하고 밝은 색상**을 출력
- 조이스틱 하나(CH3)만으로도 **전체 색상환을 부드럽게 회전**하며 색상을 제어 가능

> ✅ **장점**: 자연스럽고 연속적인 색상 전환 + 직관적인 사용자 경험

---

### 🧮 주요 HSV 적용 코드

```cpp
// 주요 코드
float hue = map(ch3_pulse, 1000, 2000, 0, 360);  // 0~360도 색환 범위로 매핑
hsvToRgb(hue, 1.0, 1.0, r, g, b);                // HSV → RGB 변환 후 출력
```
🎯 HSV 방식 사용의 장점
| 항목 | RGB 방식 | HSV 방식 |
|------|----------|-----------|
| 색상 직관성 | 낮음 (R/G/B 조합으로 직접 계산 필요) | 높음 (색상환에서 각도로 지정 가능) |
| 구현 난이도 | 간단하지만 감도 조절 어려움 | 변환 알고리즘 필요하지만 제어 용이 |
| 색상 전환 부드러움 | 색상 점프 발생 가능성 | 색상 스펙트럼을 연속적으로 순회 가능 |


## 👥 개발자 및 기여 내용

이 프로젝트는 두 명이 협업하여 제작하였습니다:

- **윤창혁**  
  🎨 HSV 색상 기반 **3색 RGB LED 제어** 로직 및 `hsvToRgb()` 변환 함수, R9DS 송신기와 조종기 연결 구현

- **심창완**  
  💡 단색 **LED 3개 밝기 제어**, **ON/OFF 스위치 기능** 및 전체 통합 로직 구현

> 각자 담당한 기능을 나눠 개발하고, 최종적으로 하나의 시스템으로 통합하였습니다.  
> 하드웨어 구성은 2명이 협업하여 완성했습니다.




