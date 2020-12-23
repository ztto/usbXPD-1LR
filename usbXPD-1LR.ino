/*
 * XPD-1LR To USB
 * (Arduino Pro Micro)
 *
 *DSUB9P(Md)  --  pro micro  gpio
 *  1     --   d4(D0)     d4
 *  2     --   d5(D1)     c6
 *  3     --   d6(D2)     d7
 *  4     --   d7(D3)     e6
 *  5     --   Vcc(+5V)
 *  6     --   d2 (LH)    d1(int1)
 *  7     --   d3 (ACK)   d0(int0)
 *  8     --   d8 (REQ)   b4
 *  9     --   GND
 */
#include <Joystick.h>

Joystick_ Joystick = Joystick_(
  0x03,                    // reportid
  JOYSTICK_TYPE_GAMEPAD,  // type
  2,                      // button count
  0,                       // hat switch count
  true,                    // x axis enable
  true,                    // y axis enable
  false,                   // z axis enable
  true,                    // right x axis enable
  true,                    // right y axis enable
  false,                   // right z axis enable
  false,                   // rudder enable
  false,                   // throttle enable
  false,                   // accelerator enable
  false,                   // brake enable
  false                    // steering enable
);

// gpio
#define D0      4
#define D1      5
#define D2      6
#define D3      7
#define D4      2
#define D5      3
#define SEL     8

int PIN[3];
#define LOOP_INTERVAL 16  // 入力周期(msec)

// セットアップ
void setup() {
  
  // シリアル初期化
//  Serial.begin(115200);
//  while (!Serial);

  // pin初期設定
  pinMode(D0,INPUT_PULLUP);
  pinMode(D1,INPUT_PULLUP);
  pinMode(D2,INPUT_PULLUP);
  pinMode(D3,INPUT_PULLUP);
  pinMode(D4,INPUT_PULLUP);
  pinMode(D5,INPUT_PULLUP) ;
  pinMode(SEL,OUTPUT) ;
  // d8(SEL) HIGH
  PORTB |= 0b00010000 ;

  // ジョイスティック開始
  Joystick.begin();

  // ジョイスティック初期設定
  Joystick.setXAxisRange(0, 2);
  Joystick.setYAxisRange(0, 2);
  Joystick.setRxAxisRange(0, 2);
  Joystick.setRyAxisRange(0, 2);
}

uint8_t btn6;
// メインループ
void loop() {

  // d8(SEL) LOW
  PORTB &= 0b11101111;
  delayMicroseconds(20);

  // 上 下 左 右 a
  PIN[0] = PINC;
  PIN[1] = PIND;
  PIN[2] = PINE;

  // ボタン入力 ------------
  // 上
  if (!(PIN[1] & (1 << 4))) {
    Joystick.setYAxis(0);
  }
  // 下
  if (!(PIN[0] & (1 << 6))) {
    Joystick.setYAxis(2);
  }
  if (((PIN[1] & (1 << 4))) &&
      ((PIN[0] & (1 << 6)))) {
    Joystick.setYAxis(1);
  }
  // 左
  if (!(PIN[1] & (1 << 7))) {
    Joystick.setXAxis(0);
  }
  // 右
  if (!(PIN[2] & (1 << 6))) {
    Joystick.setXAxis(2);
  }
  if (((PIN[1] & (1 << 7))) &&
      ((PIN[2] & (1 << 6)))) {
    Joystick.setXAxis(1);
  }
  // a
  if (!(PIN[1] & (1 << 0))) {
    Joystick.setButton(0, 1);
  } else {
    Joystick.setButton(0, 0);
  }

  //-------------------------
//  for (int i = 0; i < 3; i++) {
//    Serial.print(i);
//    Serial.print(":");
//    Serial.print(PIN[i]);
//    Serial.print(",");
//  }
//  Serial.println("");
  //-------------------------

  // HID デバイスへの出力に反映させる
  Joystick.sendState();

  // d8(SEL) HIGH
  PORTB |= 0b00010000 ;
  delayMicroseconds(20);

  // 上 下 左 右 b
  PIN[0] = PINC;
  PIN[1] = PIND;
  PIN[2] = PINE;

  // ボタン入力 ------------
  // 上
  if (!(PIN[1] & (1 << 4))) {
    Joystick.setRyAxis(0);
  }
  // 下
  if (!(PIN[0] & (1 << 6))) {
    Joystick.setRyAxis(2);
  }
  if (((PIN[1] & (1 << 4))) &&
      ((PIN[0] & (1 << 6)))) {
    Joystick.setRyAxis(1);
  }
  // 左
  if (!(PIN[1] & (1 << 7))) {
    Joystick.setRxAxis(0);
  }
  // 右
  if (!(PIN[2] & (1 << 6))) {
    Joystick.setRxAxis(2);
  }
  if (((PIN[1] & (1 << 7))) &&
      ((PIN[2] & (1 << 6)))) {
    Joystick.setRxAxis(1);
  }
  // b
  if (!(PIN[1] & (1 << 1))) {
    Joystick.setButton(1, 1);
  } else {
    Joystick.setButton(1, 0);
  }

  //-------------------------
//  for (int i = 0; i < 3; i++) {
//    Serial.print(i);
//    Serial.print(":");
//    Serial.print(PIN[i]);
//    Serial.print(",");
//  }
//  Serial.println("");
  //-------------------------

  // HID デバイスへの出力に反映させる
  Joystick.sendState();
}
