#include <Arduino.h>
#include <PinChangeInterrupt.h>

// HSV RGB LED 핀 (PWM 출력용)
const int redPin = 11;
const int greenPin = 10;
const int bluePin = 9;

// 단색 LED 핀 (PWM 출력용)
const int yellowPin = 6;
const int green2Pin = 5;
const int blue2Pin = 3;

// 수신기 입력 핀 (PWM 신호 입력용)
const int onOffPin = 4;       // CH6 → LED 전체 ON/OFF 제어
const int huePin = 7;         // CH3 → HSV 색상(Hue) 선택
const int brightnessPin = 8;  // CH2 → 단색 LED 밝기 조절

// PWM 펄스 측정용 변수
volatile unsigned long ch6_start = 0, ch6_pulse = 1500; // CH6: ON/OFF
volatile unsigned long ch3_start = 0, ch3_pulse = 1500; // CH3: 색상
volatile unsigned long ch2_start = 0, ch2_pulse = 1500; // CH2: 밝기

// 현재 선택된 색상 추적용 변수
enum LedColor { NONE, BLUE, GREEN, RED };
LedColor currentColor = NONE;

// HSV 값을 RGB로 변환하는 함수
void hsvToRgb(float h, float s, float v, int& r, int& g, int& b) {
  float c = v * s;
  float x = c * (1 - fabs(fmod(h / 60.0, 2) - 1));
  float m = v - c;
  float r1, g1, b1;

  if (h < 60)      { r1 = c; g1 = x; b1 = 0; }
  else if (h < 120){ r1 = x; g1 = c; b1 = 0; }
  else if (h < 180){ r1 = 0; g1 = c; b1 = x; }
  else if (h < 240){ r1 = 0; g1 = x; b1 = c; }
  else if (h < 300){ r1 = x; g1 = 0; b1 = c; }
  else             { r1 = c; g1 = 0; b1 = x; }

  r = (r1 + m) * 255;
  g = (g1 + m) * 255;
  b = (b1 + m) * 255;
}

// CH6: ON/OFF 제어용 인터럽트
void ch6ISR() {
  if (digitalRead(onOffPin) == HIGH)
    ch6_start = micros();
  else
    ch6_pulse = micros() - ch6_start;
}

// CH3: HSV 색상 제어용 인터럽트
void ch3ISR() {
  if (digitalRead(huePin) == HIGH)
    ch3_start = micros();
  else
    ch3_pulse = micros() - ch3_start;
}

// CH2: 단색 LED 밝기 제어용 인터럽트
void ch2ISR() {
  if (digitalRead(brightnessPin) == HIGH)
    ch2_start = micros();
  else
    ch2_pulse = micros() - ch2_start;
}

void setup() {
  Serial.begin(9600); // 디버깅용 시리얼 통신 시작

  // 입력 핀 설정
  pinMode(onOffPin, INPUT);
  pinMode(huePin, INPUT);
  pinMode(brightnessPin, INPUT);

  // 출력 핀 설정
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(yellowPin, OUTPUT);
  pinMode(green2Pin, OUTPUT);
  pinMode(blue2Pin, OUTPUT);

  // 인터럽트 등록 (PinChangeInterrupt 라이브러리 사용)
  attachPinChangeInterrupt(digitalPinToPCINT(onOffPin), ch6ISR, CHANGE);
  attachPinChangeInterrupt(digitalPinToPCINT(huePin), ch3ISR, CHANGE);
  attachPinChangeInterrupt(digitalPinToPCINT(brightnessPin), ch2ISR, CHANGE);
}

void loop() {
  // CH3 → 색상 값(Hue)을 0~360도로 변환
  float hue = map(ch3_pulse, 1000, 2000, 0, 360);
  hue = constrain(hue, 0, 360);

  int r = 0, g = 0, b = 0;
  hsvToRgb(hue, 1.0, 1.0, r, g, b);  // HSV 색상 → RGB 값 계산

  // CH2 → 단색 LED 밝기 계산
  // 맵핑 범위를 좁혀 조이스틱 변화에 더 민감하게 반응
  int brightness = map(ch2_pulse, 1200, 1800, 0, 255);
  brightness = constrain(brightness, 0, 255);

  // CH6가 ON 조건일 때만 LED 작동
  if (ch6_pulse >= 1800) {
    // HSV RGB LED 출력
    analogWrite(redPin, r);
    analogWrite(greenPin, g);
    analogWrite(bluePin, b);

    // 단색 LED들 동일 밝기로 출력
    analogWrite(yellowPin, brightness);
    analogWrite(green2Pin, brightness);
    analogWrite(blue2Pin, brightness);

    // 현재 색상 기록 (디버깅용)
    if (r > g && r > b) currentColor = RED;
    else if (g > r && g > b) currentColor = GREEN;
    else if (b > r && b > g) currentColor = BLUE;
    else currentColor = NONE;
  } else {
    // CH6가 OFF일 경우 모든 LED 끄기
    analogWrite(redPin, 0);
    analogWrite(greenPin, 0);
    analogWrite(bluePin, 0);
    analogWrite(yellowPin, 0);
    analogWrite(green2Pin, 0);
    analogWrite(blue2Pin, 0);
    currentColor = NONE;
  }

  // 시리얼 출력 (디버깅용)
  Serial.print("CH2 (solid bright): ");
  Serial.print(ch2_pulse);
  Serial.print("  CH3 (HSV hue): ");
  Serial.print(ch3_pulse);
  Serial.print("  CH6 (on/off): ");
  Serial.print(ch6_pulse);
  Serial.print("  Hue: ");
  Serial.print(hue);
  Serial.print("  Brightness: ");
  Serial.print(brightness);
  Serial.print("\n");

  delay(20); // 안정적인 출력 및 PWM 처리 타이밍 확보
}
