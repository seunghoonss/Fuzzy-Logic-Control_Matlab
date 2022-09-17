#define F_CPU 16000000L
#include <avr/io.h>
#include <stdio.h>
#include <string.h>
#include <util/delay.h>
#include <util/twi.h>
#include <avr/interrupt.h>
#define LCD_ADDR 0x27
#define RS_COMMAND 0
#define RS_DATA 1
#define BACKLIGHT (1 << 3)
#define PIN_EN (1 << 2)
#define SCL_FREQ  100   // kHz
#define CPU_CLOCK 16000 // kHz
//#define START_TRANSMITTED 0x08
//#define MT_SLA_ACK 0x18
//#define MT_DATA_ACK 0x28

// 사용한 함수 선언
void TWI_init(void);
void TWI_OneByteWrite(uint8_t SLA_address, uint8_t byte_data);
void LCD_write_4bit(uint8_t data, uint8_t rs_bit); //4bit data 전달 함수
void LCD_write_8bit(uint8_t data, uint8_t rs_bit);
void LCD_write_data(char data);
void LCD_write_command(uint8_t command);
void LCD_clear_display();
void LCD_return_home();
void LCD_entry_mode_set(uint8_t ID, uint8_t S);
void LCD_display_onoff_control(uint8_t D, uint8_t C, uint8_t B);
void LCD_cursor_or_display_shift(uint8_t SC, uint8_t RL);
void LCD_function_set(uint8_t DL, uint8_t N, uint8_t F);
void LCD_goto_XY(uint8_t row, uint8_t col);
void LCD_init();
void LCD_write_string(char* string);

void LCD_write_4bit(uint8_t data, uint8_t rs_bit) {
	uint8_t lcd_data, msd_data;
	msd_data = data & 0xF0;
	
	lcd_data = msd_data | BACKLIGHT | PIN_EN | rs_bit; // D7-4 BL EN RW RS
	
	TWI_OneByteWrite(LCD_ADDR, lcd_data);
	lcd_data &= (~PIN_EN);
	TWI_OneByteWrite(LCD_ADDR, lcd_data);

	_delay_us(40);
}
// 8비트 데이터 출력 함수
void LCD_write_8bit(uint8_t data, uint8_t rs_bit){
	uint8_t lcd_data, msd_data;
	
	msd_data = data & 0xF0;
	lcd_data = (data << 4) & 0xF0;
	
	LCD_write_4bit(msd_data, rs_bit);
	LCD_write_4bit(lcd_data, rs_bit);
}
// LCD 문자 데이터 출력 함수
void LCD_write_data(char data){
	LCD_write_8bit(data, RS_DATA);
}
// LCD 레지스터 설정 함수
void LCD_write_command(uint8_t command){
	LCD_write_8bit(command, RS_COMMAND);
}
// LCD 화면 지우고 커서 초기화 함수
void LCD_clear_display(){
	LCD_write_command(0x01);
	_delay_ms(2);
}
// LCD 커서 초기화 함수
void LCD_return_home(){
	LCD_write_command(0x02);
	_delay_ms(2);
}
// 데이터를 쓰거나 읽었을 때 LCD 화면 이동 및 커서위치 증감 설정 함수
void LCD_entry_mode_set(uint8_t ID, uint8_t S){
	LCD_write_command(0x04 | (ID<<1) | (S<<0));
	_delay_ms(40);
}
// LCD 화면,커서,커서깜빡임 설정 함수
void LCD_display_onoff_control(uint8_t D, uint8_t C, uint8_t B){
	LCD_write_command(0x08 | (D<<2) | (C<<1) | (B<<0));
	_delay_ms(40);
}
// LCD 화면or커서 이동 방향 설정 함수
void LCD_cursor_or_display_shift(uint8_t SC, uint8_t RL){
	LCD_write_command(0x10 | (SC<<3) | (RL<<2));
	_delay_ms(40);
}
// LCD 인터페이스 길이, 화면 표시 행수, 폰트 크기 설정 함수
void LCD_function_set(uint8_t DL, uint8_t N, uint8_t F){
	LCD_write_command(0x20 | (DL<<4) | (N<<3) | (F<<2));
	_delay_ms(40);
}
// LCD 커서 이동 함수
void LCD_goto_XY(uint8_t row, uint8_t col){
	col %= 16;
	row %= 2;
	
	uint8_t address = (0x40 * row) + col;
	uint8_t command = 0x80 + address;
	LCD_write_command(command);
}
// LCD 초기화 함수
void LCD_init(){
	_delay_ms(25);
	
	LCD_write_4bit(0x30, RS_COMMAND);
	_delay_ms(2);
	LCD_write_4bit(0x30, RS_COMMAND);
	_delay_ms(2);
	LCD_write_4bit(0x30, RS_COMMAND);
	_delay_ms(2);
	LCD_write_4bit(0x20, RS_COMMAND);
	_delay_ms(2);
	
	LCD_function_set(0, 1, 0);
	LCD_display_onoff_control(1, 0, 0);
	LCD_entry_mode_set(1, 0);
	_delay_ms(2);
	LCD_clear_display();
}
// LCD 문자열 출력 함수
void LCD_write_string(char* string){
	uint8_t i;
	for (i=0; string[i]; i++){
		LCD_write_data(string[i]);
	}
}
// I2C 초기화 함수
void TWI_init(void) {
	TWSR &= ~((1 << TWPS1) | (1 << TWPS0));
	TWBR = ((CPU_CLOCK /SCL_FREQ) - 16) / 2;
}
// I2C 함수
void TWI_OneByteWrite(uint8_t SLA_address, uint8_t byte_data) {
	uint8_t SLA_W;
	SLA_W = (SLA_address << 1);

	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
	TWDR = SLA_W;
	TWCR = (1<<TWINT) | (1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
	TWDR = byte_data;
	TWCR = (1<<TWINT) | (1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
	TWCR = (1<<TWINT)|(1<<TWEN)| (1<<TWSTO);
	_delay_us(10);
}


// 초기화, 송신, 수신 기능을 함수로 작성
void UART1_init(uint16_t baud){
	// RXEN1과 TXEN1을 1로 초기화
	UCSR1B |= (1<<RXEN1) | (1<<TXEN1);
	UBRR1H = baud >> 8;
	UBRR1L = baud & 0x00FF;
}
void UART1_transmit(char data){
	// 송신
	while(!(UCSR1A & (1<<UDRE1))); // 송신 버퍼가 비어 있을 때까지 무한 반복
	UDR1=data; // data값을 UDR1에 저장
}
uint8_t UART1_receive(void){
	// 수신
	while (!(UCSR1A & (1<<RXC1))); // 수신 버퍼에 읽을 문자가 있을 때까지 무한 반복
	return UDR1; // UDR1을 반환
}

// printf와 scanf 구현
FILE OUTPUT = FDEV_SETUP_STREAM(UART1_transmit, NULL, _FDEV_SETUP_WRITE);
FILE INPUT = FDEV_SETUP_STREAM(NULL, UART1_receive, _FDEV_SETUP_READ);

// 인터럽트 변수 선언
volatile uint8_t tmr1_cmpa_count = 0; // ISR에서 값을 변경하는 변수 tmr1_cmpa_count 선언
volatile uint8_t tmr1_one_second_flag = 0;
volatile uint16_t adc_value = 0;
volatile char AD[32];
volatile int tail = 0;
volatile int ad_len = 0;
volatile int SW = 0;
volatile int SW3 = 0;
volatile int UART_SW = 0;
volatile int ms = 0;
volatile int int6_SW = 0;
ISR(INT4_vect){  // 스위치1(이전 모드)
	SW = (SW+2) % 3;
}

ISR(INT5_vect){  // 스위치2(다음 모드)
	SW = (SW+1) % 3;
}

ISR(INT6_vect){  // 스위치3(모드별 기능)
	SW3++;
	int6_SW=1;
}

ISR(TIMER1_COMPA_vect) { // 1번 타이머/카운터 compare match 인터럽트
	tmr1_cmpa_count++;
	if (tmr1_cmpa_count == 5) {
		tmr1_cmpa_count = 0;
		tmr1_one_second_flag++;
	}
}

ISR(USART1_RX_vect){ // UART 수신 인터럽트

	TCCR1B = 0x0A; // CTC모드, 분주비 8
	ms = tmr1_one_second_flag;
	
	UART_SW = 1;
	AD[tail]=UDR1;
	tail++;
	ad_len = tail;
}

int main(void)
{
	stdout = &OUTPUT;
	stdin = &INPUT;
	UART1_init(103); // Boad rate 9600
	TWI_init();  // I2C 초기화
	LCD_init();   // LCD 초기화
	char asd[32];
	DDRE |= ~0x70;  // PE4, 5, 6 입력
	PORTE |= 0x70;  // 내부 풀업 저항 활성화
	EIMSK |= (1<< INT6)|(1<< INT5)|(1<< INT4);  // 외부 인터럽트 핀 지정
	EICRB |= (1<< ISC61)|(1<< ISC51)|(1<< ISC41);  // Falling edgy
	
	sei();  // 인터럽트 활성화
	
	// 처음 LCD표시
	// 	LCD_goto_XY(0,0);
	// 	LCD_write_string("Embedded coding");
	// 	LCD_goto_XY(1,0);
	// 	LCD_write_string("Team #5");
	
	while(1){
		if(SW==0){
			tmr1_one_second_flag = 0;  // 타이머 플레그 초기화
			UCSR1A = 0x00;  //
			UCSR1B |= (1<<RXCIE1) | (1<<RXEN1) | (1<<TXEN1);  //
			UCSR1C = 0x06;  //
			OCR1A = 4000;   // OCR값 4000
			TCCR1B = 0x00; // 중지
			TIMSK |= (1 << OCIE1A); // 1번 타이머/카운터 Compare match interrupt 설정
			char s[18];
			
			LCD_clear_display();
			TWI_init();
			LCD_init();
			
			if (UART_SW == 0){  // 통신을 하지 않았을 떄
				LCD_goto_XY(0,0);
				LCD_write_string("Embedded coding");
				LCD_goto_XY(1,0);
				LCD_write_string("Team #5");
			}
			else{  // 돌아왔을 때 광고 유지
				LCD_clear_display();
				LCD_goto_XY(0,0);
				strncpy(s,AD,(ad_len<17) ? ad_len : 16);
				s[ad_len]=NULL;
				LCD_write_string(s);

				if (ad_len > 16)
				{
					LCD_goto_XY(1,0);
					strncpy(s,AD+16,ad_len-16);
					s[ad_len]=NULL;
					LCD_write_string(s);
				}
			}
			
			while (SW == 0)  // MODE1이 선택되는 동안
			{
				if (tmr1_one_second_flag - ms > 25){  // 통신이 되고 0.25초가 지난 뒤
					tmr1_one_second_flag=0;  // 타이머 flag초기화
					TCCR1B = 0x00;

					LCD_clear_display();  // 화면 지움
					LCD_goto_XY(0,0);  // LCD 화면 (0, 0)커서 위치
					strncpy(s,AD,(tail<17) ? tail : 16); // 문자열 복사
					s[tail]=NULL;		  // 문자열 끝은 NULL
					LCD_write_string(s);  // 문자열 출력
					
					
					if (tail > 16) // 길이가 16보다 클 때
					{
						LCD_goto_XY(1,0);			// LCD 화면 (1,0)으로 커서 이동
						strncpy(s,AD+16,tail-16);  // AD[16:33] 문자열 복사
						s[tail-16]=NULL;		   // 문자열 끝은 NULL
						LCD_write_string(s);	   // 문자열 출력
					}
					tail = 0;					   // tail은 0으로 초기화
				}
				if(SW3 == 1){
					LCD_display_onoff_control(0,0,0);
				}
				if(SW3 >= 2){
					LCD_display_onoff_control(1,0,0);
					SW3=0;
				}
				
			}
		}
		if (SW==1){
			TWI_init();
			LCD_init();
			uint16_t seconds = 0; // seconds 변수
			TIMSK |= (1 << OCIE1A); // 1번 타이머/카운터 Compare match A Interrupt 설정
			TCCR1B = 0x0A;		    // 1번 타이머/카운터 분주비 8
			OCR1A = 0x0FA0;
			sei();
			
			LCD_goto_XY(0,0);				  //LCD화면 (0,0)으로 커서 이동
			LCD_write_string("00:00:00.00");  //초기값 출력
			
			int h = 0;  // 시
			int m = 0;  // 분
			int s = 0;  // 초
			//char asd[15];
			while(SW == 1){	   // MODE2
				if(SW3 % 3 == 1){
					if(tmr1_one_second_flag){  // 0.01초 흐름
						tmr1_one_second_flag = 0;
						
						
						if(seconds < 10)LCD_goto_XY(0,10);
						else LCD_goto_XY(0,9);
						sprintf(asd,"%d", seconds);
						LCD_write_string(asd);
						
						if(seconds >= 98){ // 1초 흐름
							s++;
							seconds=0;  // 인트
							if(s<10)LCD_goto_XY(0, 7);
							else LCD_goto_XY(0,6);
							
							sprintf(asd,"%d", s);
							LCD_write_string(asd);  // 스트링
							
						}
						if (s == 60) {// 1분 흐름
							LCD_goto_XY(0,6);
							LCD_write_string("0");
							m++;
							s=0;
							if(m<10)LCD_goto_XY(0,4);
							else LCD_goto_XY(0,3);
							sprintf(asd,"%d", m);
							LCD_write_string(asd);  // 스트링
						}
						
						if (m == 60){ // 1분 흐름
							LCD_goto_XY(0,3);
							LCD_write_string("0");
							h++;
							m=0;
							if(m<10)LCD_goto_XY(0,1);
							else LCD_goto_XY(0,0);
							sprintf(asd,"%d", h);
							LCD_write_string(asd);
						}
						printf("%d:%d:%d:%d\r\n", h, m, s, ++seconds);
						sprintf(asd,"%d:%d:%d:%d",h,m,s,++seconds);
						//LCD_write_string(asd);
					}
					

				}
				else if(SW3 % 3 == 2){ // SW3 2번 클릭 // 스탑워치가 멈춘다
					printf("%d:%d:%d:%d\r\n", h, m, s, seconds);
				}
				
				else if(SW3 == 3){ // SW3 3번 클릭 // 초기화 상태에서 다시 스탑워치 작동
					h = 0;
					m = 0;
					s = 0;
					seconds = 0;
					SW3 = 1;
					LCD_goto_XY(0,0);   // LCD 광고문 출력 모드
					LCD_write_string("00:00:00:00");
				}
			}
			
		}
		if(SW==2){
			TWI_init();		// I2C 초기화
			LCD_init();		// LCD 초기화
			
			

			ADMUX |= 0x5B;		 // 차등입력 AD0->양, AD1->음
			ADCSRA |= (1<<ADEN); // ADC 활성화
			ADCSRA |= (1<<ADFR); // 프리러닝모드
			ADCSRA |= (1<<ADSC); // AD변환시작
			ADCSRA |= 0x07;		 // 분주비 128
			
			//char asd[15];
			
			LCD_goto_XY(0, 0);	 // LCD화면 (0,0)으로 커서 이동
			LCD_write_string("Voltage: ");// Voltage: 문자열 출력
			
			float V1;
			float V2;
			float adc1 = 51;
			float adc2 = 87;
			sei();
			while (SW == 2) // MODE 3
			{
				adc_value = ADC;
				
				//V1 = (adc_value*(5.0/512.0)); // 차등입력
				V2 = (1/(adc2-adc1)*adc_value)+ ((adc2 - 2*adc1)/(adc2 - adc1)); //오차값
				printf("ADC : %d, voltage : %.2fV\r\n", adc_value, V2); // 값 출력
				_delay_ms(100);
				
				LCD_goto_XY(0,0);
				sprintf(asd,"Voltage:%.2fV", V2);
				LCD_write_string(asd);  // 스트링
				
				if(int6_SW){
					int6_SW=0;
					LCD_goto_XY(1,0);
					sprintf(asd,"Voltage:%.2fV", V2);
					LCD_write_string(asd);  // 스트링
				}
				_delay_ms(100); //0.1초 마다 출력
			}
		}
	}
}


