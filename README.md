# stm32f411-uart

## 📌 프로젝트 개요
이 프로젝트는 **STM32F411 MCU**를 기반으로 한 UART 통신 예제입니다.  
PC ↔ STM32 간의 직렬 통신을 통해 데이터를 송수신하며, 디버깅 및 모듈 간 데이터 교환을 학습/검증하는 목적을 가집니다.

## 🛠 개발 환경
- IDE: [STM32CubeIDE](https://www.st.com/en/development-tools/stm32cubeide.html)
- MCU: STM32F411 (Nucleo / Discovery 보드 또는 커스텀 보드)
- HAL Library: STM32Cube HAL (UART, GPIO, TIM 등)
- 통신 모듈: USB-to-UART (예: CP2102, FT232, CH340) 또는 HC-05 Bluetooth
- Baudrate: 9600 (기본값, 필요 시 변경 가능)

## 📂 프로젝트 구조

stm32f411-uart/
├── Core/
│ ├── Inc/ # 헤더 파일
│ ├── Src/ # C 소스 코드
│ └── Startup/ # 스타트업 코드
├── Drivers/ # HAL 드라이버
├── .ioc # CubeMX 설정 파일
└── README.md # 프로젝트 설명


## 🚀 빌드 및 실행 방법
1. STM32CubeIDE에서 `.ioc` 파일 열기
2. 코드 자동 생성 후 빌드
3. 보드를 PC에 USB 연결
4. 터미널 프로그램(예: PuTTY, minicom, Tera Term) 실행
   - COM 포트 선택
   - Baudrate: 9600
   - Data bits: 8, Stop bits: 1, Parity: None
5. 송수신 테스트 진행

## 📖 주요 기능
- UART 초기화 및 기본 송수신
- UART 인터럽트 기반 수신 처리
- 수신된 명령에 따라 GPIO 제어 (예: LED 토글)
- 향후 확장 가능: 블루투스 모듈(HC-05) 통신, 센서 데이터 전송 등

## 📖 참고 자료
- [STM32F4 Reference Manual](https://www.st.com/resource/en/reference_manual/dm00031020-stm32f405-415-stm32f407-417-stm32f427-437-and-stm32f429-439-advanced-arm-based-32-bit-mcus-stmicroelectronics.pdf)
- [STM32Cube HAL API Reference](https://www.st.com/en/embedded-software/stm32cube-mcu-packages.html)
- [UART Tutorial - STM32](https://controllerstech.com/stm32-uart-tutorial/)

---
