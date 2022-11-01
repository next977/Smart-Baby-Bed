# Smart-Baby-Bed
경기산업기술교육센터] 개인프로젝트 3 - Smart Baby Bed

- **개발 필요성 :** 갓난 아기가 밤에 불안감과 울음으로 통해 아기와 부모님들의 고충을 덜어
주고자 개발 하였습니다.

- **개발 목표 :**

       ① 아이의 울음을 감지시 실시간으로 Step motor와 servo motor를 이용해 아이의 불안감 해소.
       ② 블루투스를 이용하여 실시간으로 Step motor와 servo motor를 작동시켜 아이에게 흥미를 줌.

- **개발 기간 :** 2016.  8. 3 ~ 2016. 9. 30 (약 2개월)
- **개발 언어 및 환경 :** C/C++ / Windows 7 (64bit) / Atmel Studio 7.0 / MCU(Atmega128)
- **개발 내용 :**

       1) **Analog Sound sensor (소리감지 센서) :** 아이의 울음소리 감지.

       2) **Step Motor (모빌) :** 아이의 흥미를 유발 시켜주는 센서.

       3) **Servo Motor (침대) :** 아이의 흥미를 유발 시켜주는 센서.

      4) **Blue Tooth :** MCU 인 Atmage 128 과 스마트폰 간 통신

- **구현 기술 :**

       1) **Analog Sound Sensor: 소리 감지 센서, ADC**

           □ 용도
                 ▶ 아날로그 소리감지 센서 값 측정
           □ 기술내용
                 ▶ 10비트 축차형 ADC를 이용한 아날로그 센서값 입력
                 ▶ ADC로 입력된 전압값을 소리(dB)값으로 변환
                 ▶ 소리 감지 센서와 MCU간 인터페이스

        2) **Blue Tooth: Blue Tooth, UART Interface**

           □ 용도
                ▶ 블루투스 모듈을 사용하여 Sound sensor가 측정한 소리(dB) 값을 스마트 폰으로 전송
           □ 기술내용
                ▶ 블루투스 모듈과 MCU간 인터페이스
                ▶ Interrupt를 이용한 양방향 UART 통신 프로그램 작성

        3) **Step Motor: Step Motor, CTC**

           □ 용도 
                ▶ CTC 제어를 이용한 Step Motor 속도 조절
           □ 기술내용
                ▶ MCU 타이머 CTC mode 인터럽트 사용법
                ▶ CTC을 이용한 모터 제어 기술

        4) **Servo Motor: Servo Motor, PWM**

           □ 용도
                ▶ PWM 제어를 이용한 Servo Motor 각도 조절
           □ 기술내용
                ▶ MCU 타이머 인터럽트 사용법
                ▶ PWM을 이용한 모터 제어 기술
