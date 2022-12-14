
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#define STEP_DDRA   	DDRA
#define STEP_PORTA		PORTA
volatile unsigned char step ;
volatile unsigned int sss ;

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
unsigned char mot_tbl[] = {0x0a,0x06,0x05,0x09};// 2상 여자 1.8도 스텝모터 1.8도 스텝모터의 데이터 시트에있는 상에 데이터를 넣어구동하는방법
ISR(TIMER1_COMPA_vect)	/*CTC모드로 설정후 OCR1A에 TCNT1의 값이 매칭이 되었을때 TCNT1은 0으로 초기화되고 COMPA(OCR1A)의 인터럽트 루틴으로 빠진다.
							즉, CTC모드 OCR1A의 인터럽트루틴을 생성해준것이다!
							모터 2개를 동시에 돌리기 위해 4핀 4핀 총 8핀 1포트로 돌리는것!*/
{
	cli();							/* sss++, if문을 돌리고 step ++, if문을 돌리고 난후 OCR1A에 테이블 값을 넣어야 된다
									그래야 모터가 터지지도 않고, 인터럽트루틴으로 오면 테이블 sss는 인덱스 1부터 돌아간다!*/
	
	sss++;
	if(sss==500) sss=499;
	step ++;
	if(step==4) step=0;
	OCR1AH=(m_TimeTable[sss]>>8); //16비트중 상위 8비트의 값을 주기위해.
	OCR1AL=m_TimeTable[sss]&0xff; //16비트중 하위 8비트의 값을 주기위해.
	sei();
}


void main (void)
{

	TCCR1A = 0x54; //ctc모드 설정, ocA 토글으로 설정, 64분주 프리스케일러 설정 
	TCCR1B = 0x0B;	//ctc모드 설정, ocA 토글으로 설정, 64분주 프리스케일러 설정 
	TCNT1L = 0x00; // low 비트
	TCNT1H = 0x00; // hihg 비트
	TIMSK  = 0x10; //카이머 카운터 16비트 1번카운터 허용
	TIFR  = 0x10; // 타이머카운터 16비트 1번카운터 허용
	OCR1AH=(m_TimeTable[0]>>8);	//처음 OCR1A의 값을 주어서 처음 CTC모드의 매칭값을 주어 OCR1A의 인터럽트를 걸리게끔 초기값을 0번째 초기값을 줌!
	OCR1AL=m_TimeTable[0]&0xff; 
	sei();	//sei를 꼭 써주어야된다. enaible 인터럽트 (인터럽트 허용하겠다) 
	
	STEP_DDRA		=	0xFF; //스텝모터 출력으로 설정 데이터 값을 받아 동작!
	while(1)

	{
	
		
		STEP_PORTA			=	mot_tbl[step];	/*0번째 step의 상의 값을 스텝모터에 넣고, 인터럽트에 빠질때마다 다음 스텝의 값을 준다.
										스텝모터의 동작원리는 스텝모터의 1,2,3,4스텝을 주면 동작을 하나, 1,2,3,4스텝 사이의 시간을 주어야된다(펄스)
										그냥 아무런 시간을 주면 되는게 아니라, 1,2,3,4스텝 사이에 시간을 줄때 모터를 동작시키기위해 
										처음에는 시간을 점점 빠르게(시간간격이 점점 작아지게) 주고, 스텝모터가 일정하게 동작할때는
										시간을 보통으로(시간 간격이 일정하게) 주고, 스텝모터를 멈추게하기 위해 시간을 점점 느리게
										(시간간격이 점점 커지게) 주어야 된다.
										그러기 때문에 CTC모드를 이용하여 CTC모드일때 인터럽트 루틴으로 빠져 main에서 스텝을 주고 난후
										인터럽트에 빠져 다음 시간을 주고(시간을 주기위해 다음 카운터를 준다 (카운터 시간을 주는것) OCR1A의
										값을 주어 다음 인터럽트에 빠지기까지의 시간을 카운터로 주는 방식으로 스텝에 맞춰 시간을 조정할 수 있다!
										그러기떄문에 OCR1A의 값을 스텝모터의 가감속 테이블의 값의 배열로 조작한다.*/
	}
}
