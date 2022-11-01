/*
 * SerBoMotor_0523.c
 *
 * Created: 2016-05-23 오후 7:11:47
 * Author : admin
 */ 
#include <avr/io.h>
#include <util/delay.h>

#define SERBO_DDR   DDRE
#define SERBO_PORT	PORTE /*PORTB를 써야함. 왜냐면 PORTB의 5번핀이 OC1A이므로. PWM OC1A핀의 펄스로 서보모터를 제어하는데,
							OCR1A의 값에 따라 TCNT1의 값이 매칭이 되는 순간 OC1A의 펄스가 1로 셋됨. */
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

void main (void)
{

	TCCR3A = 0x82; //FAST PWM mode 14번 설정, top값 ICR1으로 제어가능모드임, OC1A만 1 0을 설정.
	TCCR3B = 0x1B;	//oc1A 1 0 으로 설정했기 때문에, TCNT값이 top일때 OC1A는 0, OCR1A과 매칭이됬을때 OC1a는 1로 펄스를 설정!
	TCNT3L = 0x00; // low 비트 0으로 초기화 
	TCNT3H = 0x00; // hihg 비트 0으로 초기화
	ICR3=4999; //0부터 4999까지 세고 다시 0으로 초기화가 되므로 총 5000개;
	OCR3A=0;	//초기값 0으로 설정.
	SERBO_DDR   = 0x08; //PORTE 3번핀 OC3A핀만 출력으로 사용.
	while(1)
	{
		svangle(0);
		_delay_ms(1000);
		/*svangle(20);
		_delay_ms(1000);*/
		svangle(60);
		_delay_ms(1000);
		
	}
}
