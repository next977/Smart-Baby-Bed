/*
 * Text_lcd.c
 *
 * Created: 2016-03-30 오전 10:16:22
 * Author : admin
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#define SERBO_DDR   DDRE
#define SERBO_PORT	PORTE /*PORTB를 써야함. 왜냐면 PORTB의 5번핀이 OC1A이므로. PWM OC1A핀의 펄스로 서보모터를 제어하는데,
							OCR1A의 값에 따라 TCNT1의 값이 매칭이 되는 순간 OC1A의 펄스가 1로 셋됨. */
#define STEP_DDRA   	DDRA //PORTA3 - A , PORTA2 - /A , PORTA1 - B , PORTA0 - /B
#define STEP_PORTA		PORTA
volatile unsigned char step ;
volatile unsigned int sss ;
volatile unsigned int a ;
volatile unsigned char BUF;
unsigned int result;     //result 변수 사용 선언 ADCW(ADCL+ADCH)가 10비트 이므로!
unsigned char g_ucOverflowCnt =	0; //8비트로 설정, 타이머를 구동하기 위해 오버플로우 횟수를 저장하기 위한 변수를 선언
enum{off,on}; // 블루투스의 데이터를 받아 조건문을 실행할 변수 정수형 상수 선언 off-0, on-1
const unsigned int m_TimeTable[500]= {
	//[Accelator = 4]
	32612,  13508,  10365,   8738,   7698,   6960,   6400,   5957,   5595,   5292,
	5033,   4809,   4612,   4438,   4282,   4142,   4014,   3898,   3791,   3692,
	3601,   3516,   3437,   3363,   3294,   3229,   3167,   3109,   3054,   3002,
	2952,   2905,   2860,   2817,   2776,   2736,   2699,   2662,   2628,   2594,
	2562,   2531,   2501,   2472,   2444,   2417,   2391,   2365,   2341,   2317,
	2294,   2272,   2250,   2229,   2208,   2188,   2169,   2150,   2131,   2113,
	2096,   2079,   2062,   2046,   2030,   2014,   1999,   1984,   1970,   1955,
	1942,   1928,   1915,   1901,   1889,   1876,   1864,   1852,   1840,   1828,
	1817,   1806,   1795,   1784,   1773,   1763,   1753,   1743,   1733,   1723,
	1714,   1704,   1695,   1686,   1677,   1668,   1659,   1651,   1642,   1634,
	1626,   1618,   1610,   1602,   1595,   1587,   1580,   1572,   1565,   1558,
	1551,   1544,   1537,   1530,   1523,   1517,   1510,   1504,   1497,   1491,
	1485,   1479,   1473,   1467,   1461,   1455,   1449,   1444,   1438,   1432,
	1427,   1421,   1416,   1411,   1406,   1400,   1395,   1390,   1385,   1380,
	1375,   1370,   1365,   1361,   1356,   1351,   1347,   1342,   1338,   1333,
	1329,   1324,   1320,   1316,   1311,   1307,   1303,   1299,   1295,   1291,
	1287,   1283,   1279,   1275,   1271,   1267,   1263,   1259,   1256,   1252,
	1248,   1245,   1241,   1237,   1234,   1230,   1227,   1223,   1220,   1217,
	1213,   1210,   1207,   1203,   1200,   1197,   1194,   1190,   1187,   1184,
	1181,   1178,   1175,   1172,   1169,   1166,   1163,   1160,   1157,   1154,
	1151,   1148,   1145,   1143,   1140,   1137,   1134,   1131,   1129,   1126,
	1123,   1121,   1118,   1115,   1113,   1110,   1108,   1105,   1103,   1100,
	1098,   1095,   1093,   1090,   1088,   1085,   1083,   1081,   1078,   1076,
	1074,   1071,   1069,   1067,   1064,   1062,   1060,   1058,   1055,   1053,
	1051,   1049,   1047,   1044,   1042,   1040,   1038,   1036,   1034,   1032,
	1030,   1028,   1026,   1024,   1022,   1020,   1018,   1016,   1014,   1012,
	1010,   1008,   1006,   1004,   1002,   1000,    998,    996,    995,    993,
	991,    989,    987,    985,    984,    982,    980,    978,    977,    975,
	973,    971,    970,    968,    966,    965,    963,    961,    960,    958,
	956,    955,    953,    951,    950,    948,    946,    945,    943,    942,
	940,    939,    937,    935,    934,    932,    931,    929,    928,    926,
	925,    923,    922,    920,    919,    918,    916,    915,    913,    912,
	910,    909,    908,    906,    905,    903,    902,    901,    899,    898,
	896,    895,    894,    892,    891,    890,    888,    887,    886,    884,
	883,    882,    881,    879,    878,    877,    875,    874,    873,    872,
	870,    869,    868,    867,    866,    864,    863,    862,    861,    860,
	858,    857,    856,    855,    854,    852,    851,    850,    849,    848,
	847,    846,    844,    843,    842,    841,    840,    839,    838,    837,
	835,    834,    833,    832,    831,    830,    829,    828,    827,    826,
	825,    824,    823,    822,    820,    819,    818,    817,    816,    815,
	814,    813,    812,    811,    810,    809,    808,    807,    806,    805,
	804,    803,    802,    801,    800,    799,    798,    798,    797,    796,
	795,    794,    793,    792,    791,    790,    789,    788,    787,    786,
	785,    784,    784,    783,    782,    781,    780,    779,    778,    777,
	776,    776,    775,    774,    773,    772,    771,    770,    769,    769,
	768,    767,    766,    765,    764,    764,    763,    762,    761,    760,
	759,    759,    758,    757,    756,    755,    754,    754,    753,    752,
	751,    750,    750,    749,    748,    747,    746,    746,    745,    744,
	743,    743,    742,    741,    740,    740,    739,    738,    737,    737,
	736,    735,    734,    734,    733,    732,    731,    731,    730,    729,

};
unsigned char mot_tbl[] = {0x0a,0x06,0x05,0x09};// 스텝모터의 상의 방식 데이터 시트 참조 바람
ISR (TIMER0_OVF_vect)	/* Timer0 Overflow0 ISP		0.003초 마다 인터럽트에 걸리게 하여 카운터를 세고, 그 카운터가 100이 되었을때,
													즉 0.3초 마다 ADCL,ADCH의 데이터를 읽어 소리감지 센서의 데이터를 LED나 FND로 표현 */
{
	cli(); // 인터럽트 종료.
	
	TCNT0	=	0xff - 45;		// 0.003s 0.003s를 맞춰주기 위해 -45를 함.
	g_ucOverflowCnt++;
	
	if (g_ucOverflowCnt == 100)		/* 0.003s * 100 = 0.3s 0.3초 마다 ADCL과 ADCH를 변수에 받아들여 센서 값을 읽어옴
									 0.3초마다 ADC변환을 하는것*/
	{
		//while((ADCSRA & 0x40)==0);
		while ( (ADCSRA & 0x10)==0x00);
		result = ADCW;               // result 변수에 ADCW(ADCL+ADCH)값 대입
		g_ucOverflowCnt	=	0;
		
	}
	sei();
}
ISR(TIMER1_COMPA_vect)	/*CTC모드로 설정후 OCR1A에 TCNT1의 값이 매칭이 되었을때 TCNT1은 0으로 초기화되고 COMPA(OCR1A)의 인터럽트 루틴으로 빠진다.
							즉, CTC모드 OCR1A의 인터럽트루틴을 생성해준것이다!
							모터 2개를 동시에 돌리기 위해 4핀 4핀 총 8핀 1포트로 돌리는것!*/
{
								/* sss++, if문을 돌리고 step ++, if문을 돌리고 난후 OCR1A에 테이블 값을 넣어야 된다
									그래야 모터가 터지지도 않고, 인터럽트루틴으로 오면 테이블 sss는 인덱스 1부터 돌아간다!*/
	cli();
	STEP_PORTA			=	mot_tbl[step];	
	OCR1AH=(m_TimeTable[sss]>>8); //16비트중 상위 8비트의 값을 주기위해.
	OCR1AL=m_TimeTable[sss]&0xff; //16비트중 하위 8비트의 값을 주기위해.
	sss++;
	if(sss==300) sss=299;
	step ++;
	if(step==4) step=0;
	a ++;
	if(a==1000) // 스텝모터의 작동을 보기 위한 변수 사용. 한동안 스텝모터의 작동을 확인 하려고 a를 1000까지 세어줌.
	{
		TIMSK  &=  0x01; // 스텝모터의 인터럽트 허용불가
		TIFR   &=  0x01; // 스텝모터의 인터럽트 허용 불가
		sss  = 0;
		step = 0;
		a = 0;
	}
	sei();
}

ISR(USART1_RX_vect) //USART1 블루투스의 데이터를 받아 BUF의 변수에 저장
{
	
	while(!(UCSR1A & 0x80)); // 이전에 송신한 데이터가 아직 송신 중이면 대기
	BUF= UDR1;
	
}

void svangle(float angle)
{
	unsigned int pulse;
	pulse = angle*225/90+375+0.5;/* pulse 공식에 마지막+0.5 해도 상관없음. 0.5를 더하면 더 정밀하게 각도 제어가능. 왜냐면
								1도에 0.01ms이니까! 
								원래  OCR1A값을 구하는 공식은. 0도 1500usec(1.5ms), 40도 400usec(0.4ms)이므로 1도는
								0.01ms 이고, 이것으로 -90도는 0.6ms , 0도 1.5ms , +90도 2.4ms이다.
								(1+4999)ICR값 *2.4ms/20ms=600 (+90도)OCR1값
								0.1ms만큼의 폭을 알기위해서 600/24=25 ,이므로 0.6ms(-90도), 1.5ms(0도), 2.4ms(+90도)
								는 150,375,600이다. 이것들로 인한 공식은
								OCR1A = (600-375)*x/90 +375    =225*x/90 + 375 이다. x에 우리가 원하는 각을 쓰면된다.*/
	OCR3A=pulse;
}

void TIMER_Init (void)
{
	
	TCCR0	=	0x07;  // 0000 0111일때 111은 CSn2, CSn1, CSn0 의 값들이 111로 받아 본책 228페이지의 1024의 분주값.

	/* 0.01s에 오버플로우 발생, 1/(14745600Hz/1024)ⅹ45 = 0.03s  14745600hz는 cpu의 주파수 인데 이것은 14745600주기이다. 근데 이것의 초를 구하려면 1/14745600이고 
	X144를 해줘야 0.01초 를 만들어 줄 수 있다.*/ 
	// 오버플로우까지 카운트를 144로 설정
	
	TCNT0	=	0xff - 45; /*0xff이면 1111 1111이므로 255까지 카운터를 할 수 있는데, 여기서 256이면 오버풀러 이므로 256을 쓸수가 없어 144를 빼주어 111부터 144개를 카운터 하여
	                          111+144 즉 255를 만들어 주고 다음번 째 카운터에 256을 만들어줘 오버플로우 발생시켜 인터럽트가 걸리게끔 해주기위함임!.*/
	TIMSK |=	1 << TOIE0;	
	TIFR |=	1 << TOV0;	
	sei();
}

void ADC_Init()
{
	
	ADMUX	=	0xC1;     	/* ADC1 Channel, ADC멀티플랙서 선택 레지스터 0~4비트까지 mux0~mux4로 즉 1100 0001 0비트의 mux0을 선택
								7비트, 6비트의 REFS1, REFS0 을 11로 주어 2.56v를 써줌. , 5비트의 ADLAR에 0을 주어 우정렬을 함. */	
	ADCSRA	=	0xA6;	/* ADC제어/상태 레지스터로 1010 0110 7비트인 ADEN에 1을 주어 ADC를 허용 하고, ADFR에 1을 주어 free running 모드로 
							동작, ADPS0~2 까지 총 3비트에 110을 주어 64번으로 분주해 샘플링 .*/
	ADCSRA	|=	0x40;	/* ADC start conversion 0100 0000 or 해서 0100 0000 즉 ADSC비트에 1을 주어서 free running 모드에서 첫 번째 변환을 시작 하기
							위해 "1"로 설정 , 그다음은 자동으로 변환이 반복됨, free running 모드는 한번 시작하면 자동적으로 adc를 변환*/
}
void uart_set(void) // 블루투스의 설정 RX, TX모두다 허용
{
	
	UBRR1H = 0;
	UBRR1L = 103; //전송속도는 9600bps
	UCSR1A = 0x00;
	UCSR1B = 0x98; // receiver와 transmitter 허용
	UCSR1C = 0x06; // 8bit 단위 송수신
}

int main(void)
{  
	
	TIMER_Init();
	ADC_Init ();		// PORTF1 = ADC1
	uart_set();
	_delay_ms(5);
	TCCR3A = 0x82; //FAST PWM mode 14번 설정, top값 ICR1으로 제어가능모드임, OC1A만 1 0을 설정.
	TCCR3B = 0x1B;	//oc1A 1 0 으로 설정했기 때문에, TCNT값이 top일때 OC1A는 0, OCR1A과 매칭이됬을때 OC1a는 1로 펄스를 설정!
	TCNT3L = 0x00; // low 비트 0으로 초기화
	TCNT3H = 0x00; // hihg 비트 0으로 초기화
	ICR3=4999; //0부터 4999까지 세고 다시 0으로 초기화가 되므로 총 5000개;
	OCR3A=0;	//초기값 0으로 설정.

	DDRF = 0x00;             // PORTF.0핀 입력으로 사용
	SERBO_DDR   = 0x08; //PORTE 3번핀 OC3A핀만 출력으로 사용.
	STEP_DDRA		=	0xFF; //스텝모터 출력으로 설정 데이터 값을 받아 동작!	
	int i = 0;
	sei();	//sei를 꼭 써주어야된다. enaible 인터럽트 (인터럽트 허용하겠다)
	
	while(1)
	{
		if(result>=30)
		{
		
			TCCR1A = 0x54; //ctc모드 설정, ocA 토글으로 설정, 64분주 프리스케일러 설정
			TCCR1B = 0x0B;	//ctc모드 설정, ocA 토글으로 설정, 64분주 프리스케일러 설정
			TCNT1L = 0x00; // low 비트
			TCNT1H = 0x00; // hihg 비트
			OCR1AH=(m_TimeTable[0]>>8);	//처음 OCR1A의 값을 주어서 처음 CTC모드의 매칭값을 주어 OCR1A의 인터럽트를 걸리게끔 초기값을 0번째 초기값을 줌!
			OCR1AL=m_TimeTable[0]&0xff;
			TIMSK  |= 0x10; //카이머 카운터 16비트 1번카운터 허용
			TIFR   |= 0x10; // 타이머카운터 16비트 1번카운터 허용
			sei();
			for(i=0;i<5;i++)
			{
				svangle(40);
				_delay_ms(1000);
				svangle(10);
				_delay_ms(1000);
			}
		}
		switch(BUF)
		{
			
			case on:
						TCCR1A = 0x54; //ctc모드 설정, ocA 토글으로 설정, 64분주 프리스케일러 설정
						TCCR1B = 0x0B;	//ctc모드 설정, ocA 토글으로 설정, 64분주 프리스케일러 설정
						TCNT1L = 0x00; // low 비트
						TCNT1H = 0x00; // hihg 비트
						OCR1AH=(m_TimeTable[0]>>8);	//처음 OCR1A의 값을 주어서 처음 CTC모드의 매칭값을 주어 OCR1A의 인터럽트를 걸리게끔 초기값을 0번째 초기값을 줌!
						OCR1AL=m_TimeTable[0]&0xff;
						TIMSK  |= 0x10; //카이머 카운터 16비트 1번카운터 허용
						TIFR   |= 0x10; // 타이머카운터 16비트 1번카운터 허용
						sei();
						for(i=0;i<5;i++)
						{
							svangle(40);
							_delay_ms(1000);
							svangle(10);
							_delay_ms(1000);
						}
						break;
			case off:
						break;
		}
	}
}