#include "ltag_ascetic.h"
#include "hitraw.h"
#include "clipin_snd.h"
#include "clipout_snd.h"
#include "lcd_driver.h"
#include "tm_driver.h"

int main (void) {
configuring_ports();

init_timer2();
init_int0();
init_tm();	//�������������� ����������� ���������
init_timer1();
init_timer0();

init_var();                		//����� �������� ����������

init_lcd(); 
lcd_clrscr();
init_joystick();
init_adc();




_delay_ms(15); 
lcd_puts("LTAscetic \n");
//lcd_gotoxy(0, 1);
lcd_puts("www.open-tag.ru");
//lcd_puts(int_to_str(4,3));
//lcd_clrscr();
//lcd_generate_batt_symbols();


/*
lcd_putc(0);

lcd_putc(1);
lcd_putc(2);
lcd_putc(3);
lcd_putc(4);
lcd_putc(5);
*/

sei(); 

life_leds_status[0] = ON;
timer1 = 0;
while (timer1 < 10000);

life_leds_status[1] = ON;
timer1 = 0;
while (timer1 < 10000);

//LIFE_LED3_ON;
life_leds_status[2] = ON;
timer1 = 0;
while (timer1 < 10000);

//LIFE_LED4_ON;
life_leds_status[3] = ON;
timer1 = 0;
while (timer1 < 10000);

//FIRE_LED_ON;
fire_led_status = ON;
timer1 = 0;
while (timer1 < 10000);

BULLETS_OUT_LED_ON;
timer1 = 0;
while (timer1 < 10000);

WOUND_LED_ON;
timer1 = 0;
while (timer1 < 10000);

FIRE_LED_ON;
timer1 = 0;
while (timer1 < 10000);



lcd_bl_on();
timer1 = 0;
while (timer1 < 10000);


lcd_bl_off();
timer1 = 0;
while (timer1 < 10000);

//LIFE_LED1_OFF;
life_leds_status[0] = OFF;
timer1 = 0;
while (timer1 < 10000);

//LIFE_LED2_OFF;
life_leds_status[1] = OFF;
timer1 = 0;
while (timer1 < 10000);

//LIFE_LED3_OFF;
life_leds_status[2] = OFF;
timer1 = 0;
while (timer1 < 10000);

//LIFE_LED4_OFF;
life_leds_status[3] = OFF;
timer1 = 0;
while (timer1 < 10000);

FIRE_LED_OFF;
timer1 = 0;
while (timer1 < 10000);

BULLETS_OUT_LED_OFF;
timer1 = 0;
while (timer1 < 10000);

WOUND_LED_OFF;
timer1 = 0;
while (timer1 < 10000);

timer1 = 0;
while (timer1 < 10000);
//life_led1_status = FQR_1HZ;
//life_led2_status = FQR_2HZ;
//life_led3_status = FQR_4HZ;
//life_led4_status = ON;

display_life(life);
beep(1000, 3, 128);

invite();
joystick_event=no_pressing;
display_status();

timer1 = 0;
while (timer1 < 10000);


while(1){

////volatile uint16_t adc_data;
////volatile uint16_t batt_voltage;
////adc_data=ReadADC(ADC_CHANNEL);
//adc_data=(adc_data/4)*7.5;
display_voltage_update();
//batt_voltage = (adc_data/4)*7.5;
//batt_voltage = batt_voltage/10;

if (chit_detected)
	{
		lcd_clrscr();
		lcd_puts("������ ��������,");
		lcd_gotoxy(0, 1);
		lcd_puts("������� �������!");
		while (chit_detected)
		{
			beep(1000, 3, 128);
			beep(500, 3, 128); //������������� ���� (�������, ������������, ���������)
		};
		keyboard_event=no_key_pressing;
		reload_key_event=no_key_pressing;
		joystick_event = no_pressing;
		display_status();
	}

switch(keyboard_event)
	{
	 	case no_key_pressing: break;
		case key_pressing:
		{
			if (bullets > 0)
			{
				bullets--;//��������� �� 1 ���������� ��������		
				last_simple = 0;//������������� ���� ��������
				send_ir_package();		//���������� "�������"
			//	display_bullets_update();
				
	 		}
			keyboard_event=no_key_pressing; 
		} 
        break;
  

		default:keyboard_event=no_key_pressing;	
	}


switch(reload_key_event)
	{
	 	case no_key_pressing: break;
		case key_pressing:
		{
			if (clips > 0)
			{
				playclipinsound();
				clips--;//��������� �� 1 ���������� ��������
				bullets = eeprom_read_byte(&eeprom_bullets_in_clip);
				display_clips_update();
				display_bullets_update();
				for (int i=0; i< eeprom_read_byte(&eeprom_reload_duration); i++){
				timer1 = 0;
				while (timer1 < 64000);
				}
				//while (timer1 < 10000);
				playclipoutsound();
				BULLETS_OUT_LED_OFF;
		//		last_simple = 0;//������������� ���� ��������
		//		send_ir_package();		//���������� "�������"
				
	 		}
			reload_key_event=no_key_pressing; 
		} 
        break;
  

		default:reload_key_event=no_key_pressing;	
	}


	{
		switch(rx_event)//�������, ����� ������ ������� ���������
					{
						case RX_COMPLETE: 	//������� �����
						{
						//	cli();
						/*********************************************************
							WOUND_LED_ON; //�������� ��������������� ���������
							timer1=0;
							while(timer1 < 35000);
							WOUND_LED_OFF;	//��������� ��������������� ���������
						************************************************************/
							
							rx_event = NOT_EVENT;	
							if(!get_buffer_bit(0)) //���� ���� ��� ����� 0, �� ��� ����� � ������� (�������)
							{
						
								
	//							uint8_t player_id;
								rx_packet = get_packet_value();
								volatile int gg;
								gg++;

								if (rx_packet.team_id != team_id())//"����" ��������� �� ������ ������, �� �����, �������
								{
									WOUND_LED_ON; //�������� ��������������� ���������						
									lcd_bl_on();
									display_hit_data();
									playhitsound();

									WOUND_LED_OFF;





								if (life_in_percent > rx_packet.damage) 
									{
										life_in_percent = life_in_percent-rx_packet.damage;
										life = (life_in_percent*10)/125;
										if ((life==0)&&(life_in_percent>0)) life=1;

									}
									else 
									{	
										life = 0;
										life_in_percent=0;
										WOUND_LED_ON;
										display_life(life);//��������� ������� ����� �� ������
										display_life_update();//��������� ������� ����� �� ���
										volatile uint8_t keypress_cntr; //������� ������, � ������� ������� ����� ��� �����
										keypress_cntr = 0;
										
										
										
										if ((eeprom_read_byte(&eeprom_tm_serial_num.device_code)!=0)&&(eeprom_read_byte(&eeprom_tm_serial_num.device_code)!=0xFF))

										/*���� �� ���� ��� ������ � ������*/
										{

											joystick_event=no_pressing;
											keyboard_event=no_key_pressing;
											tm_event=no_tm_event;
											uint8_t tm_valide;
											tm_valide=0;
											lcd_bl_off();
											
											
											while (!tm_valide)
											{//[while]
												lcd_clrscr();
												lcd_home();
												lcd_puts("��� ���������");
												lcd_gotoxy(0, 1);
												lcd_puts("��������� ����");
												while (tm_event == no_tm_event)
												{
													WOUND_LED_INVERT;
													timer1 = 0;
													while (timer1 < 10000);	
												
												};
												switch(tm_event)
												{//[switch]
													case no_tm_event: 
													{
															
													}
													break;
					
													case tm_crc_error: 
													{
														lcd_clrscr();
														lcd_home();
														lcd_puts("������ CRC");
														timer1 = 0;
														while (timer1 < 60000){};
														tm_event=no_tm_event;
													}
													break;

													case tm_crc_ok: 
													{
					
														if (tm_verification()) 	
														{ 	tm_valide=1;
															lcd_clrscr();
															lcd_home();
															lcd_puts("�����!");
															timer1 = 0;
															while (timer1 < 60000){};
				
															tm_event=no_tm_event;
															break;
														}	
														lcd_clrscr();
														lcd_home();
														lcd_puts("�� ��� ����");
														timer1 = 0;
														while (timer1 < 60000){};
													/*
														lcd_clrscr();
														lcd_home();
														lcd_puts("��� ���������");
														lcd_gotoxy(0, 1);
														lcd_puts("������� ����");
													*/
														tm_event=no_tm_event;
														
																		
													}

														break;
				
												}//[/switch]
											}//[while]
										
										}//end if
									
									else
									{
										
										lcd_bl_off();
										display_status();
										while(keypress_cntr < 20)
										{
											WOUND_LED_INVERT;
											timer1 = 0;
											while (timer1 < 10000);
											switch (FIRE_KEY_IN&FIRE_KEY_PIN) //���������, ����� �� �����
											{
												case 0:  keypress_cntr++ ; break;
												case FIRE_KEY_PIN: keypress_cntr = 0; break;
												default: keypress_cntr = 0;	
											}
						
										}

									}	
										
										
										WOUND_LED_OFF;
										init_var();//"�������" - �������� ����� ����
										joystick_event=no_pressing;
										keyboard_event=no_key_pressing;
										tm_event=no_tm_event;
									//	display_status();
									}
								
								display_life(life);//��������� ������� ����� �� ������
//								display_life_update();//��������� ������� ����� �� ���
								lcd_bl_off();
								display_status();
								}

									
								



	//							rx_player_id = rx_buffer[0];
	//							rx_damage = get_damage_from_rx_paket();

							

							}
							
						
						
						//	sei();
							break;
						}
						
						case RX_ERROR:		//������ ������
						{
						//	cli();
							BULLETS_OUT_LED_ON;
							timer1=0;
							while(timer1 < 35000);
							BULLETS_OUT_LED_OFF;
							rx_event = NOT_EVENT;	
						//	sei();
							break;
						}
						
						case NOT_EVENT:		//������ ������
						{
						//	cli();
						//	rx_event = NOT_EVENT;	
						//	sei();
							break;
						}



					}



	}

//	timer1=0;				//������� �����
//	while(timer1 < 65000);	//��������, ���� �� ���������� 65000 ���������� ������� (���� ������ �������)

	switch(joystick_event)
	{
		case key_up_pressing: 
			{
		//	lcd_clrscr();
	//		lcd_gotoxy(0, 0);
	
	//		if ((result+10)<=max_value) result=result+10;
	//		lcd_gotoxy(0, 1);
	//		lcd_puts(int_to_str(result,3));
		/*
			uint16_t adc_data;
			uint16_t batt_voltage;
			adc_data=ReadADC(ADC_CHANNEL);
			adc_data=(adc_data/4)*7.5;
			display_voltage_update(adc_data);
		*/
			switch(display_batt_mode)
			{
				case icon: display_batt_mode=digit;
				break;
				case digit: display_batt_mode=icon;
			
			}
			
			
			
			joystick_event = no_pressing;
			}
	
		break;
		case key_right_pressing: 
			{
			//lcd_clrscr();
			//lcd_home();
//			if ((result)<max_value) result++;
//			lcd_gotoxy(0, 1);
//			lcd_puts(int_to_str(pgm_read_byte(arr_adress+result),3));
			//lcd_puts("������ ������ \n");
			//lcd_puts("������");
			joystick_event = no_pressing;
			}
		break;
		case key_down_pressing: 
			{
	//		lcd_clrscr();
	//		lcd_gotoxy(0, 0);
	//		if((result-9)>0) result=result-10;
	//		lcd_gotoxy(0, 1);
	//		lcd_puts(int_to_str(result,3));
			joystick_event = no_pressing;
			}
		break;
		case key_left_pressing: 
			{
			//lcd_clrscr();
			//lcd_gotoxy(0, 0);
//			if ((result)>0) result--;
			//lcd_puts("������ ������ \n");
			//lcd_puts("�����");
//			lcd_gotoxy(0, 1);
//			lcd_puts(int_to_str(pgm_read_byte(arr_adress+result),3));
			joystick_event = no_pressing;
			}
		break;
		case key_central_pressing: 
			{
			joystick_event = no_pressing;
			}
		break;
		default: joystick_event = no_pressing;
	
	
		}
	
	if (display_bullets_update_now) 
		{
			display_bullets_update();
			display_bullets_update_now=false;
		}

	};
return 0;

}




void configuring_ports(){
IR_LED_DDR |= IR_LED_PIN; //�����, �� ������� ��������� �� ���� ��������� � ����� "�����"
IR_LED_HIGH_POWER_DDR|=IR_LED_HIGH_POWER_PIN;//�����, �� ������� ��������� �� ���� ��������� � ����� "�����"
IR_LED_HIGH_POWER_OFF;
IR_LED_LOW_POWER_DDR|=IR_LED_LOW_POWER_PIN;//�����, �� ������� ��������� �� ���� ��������� � ����� "�����"
IR_LED_LOW_POWER_OFF;
LIFE_LEDS_DDR |= LIFE_LED1_PIN  //�����, �� �������
				| LIFE_LED2_PIN //���������� ����������,
				| LIFE_LED3_PIN //������������ ������� "�����"
				| LIFE_LED4_PIN;//���� ����������� ��� ������

SOUND_DDR |= SOUND_PIN; //����������� ����� ��� (���)
BULLETS_OUT_LED_DDR|=BULLETS_OUT_LED_PIN;
//DDRA |= (1 << 4)|(1<<5)|(1<<6)|(1<<7); // ������������� ���� PORTA.1 ��� �����                                                                                                                                                                    DDRB = 1<<DDB3;						//PB3 (OC0) set as output
//DDRD |= (1 << 7);//��������� �� �������
DDRB |= (1 << 2);// ���������� ���������
PORTB &=~(1 << 2);
//PORTD &= ~(1 << 7);//��������� ��������� �� �������
WOUND_LED_DDR |= WOUND_LED_PIN;
TSOP_DDR &=~TSOP_PIN; //�����, � �������� ��������� ��-������ ����������� ��� "����"
RELOAD_KEY_DDR &=~RELOAD_KEY_PIN; //�����, �� ������� ����� ������ "�����������" ��� "����"
RELOAD_KEY_PORT |= RELOAD_KEY_PIN;//�������� ������������� ��������

FIRE_KEY_DDR&=~FIRE_KEY_PIN;
FIRE_KEY_PORT|=FIRE_KEY_PIN;

FIRE_MODE_KEY_DDR&=~FIRE_MODE_KEY_PIN;
FIRE_MODE_KEY_PORT|=FIRE_MODE_KEY_PIN;

ADC_DDR&=~ADC_PIN;
ADC_PORT&=~ADC_PIN;
}


/**************************************************************************************
* ������� ��������� ��������� ������� timer2
* ����� ������ ������ - ��� (����� ��� ����������)
* ������������ - ��� ��������, � �������� ������
* ������ ��������� ����� ����� ����� ��������, ����� ����������
* �������������� � ��������� �������� ������� ��-��������� 
***************************************************************************************/


void init_timer2(void){
OCR2 = F_CPU/IR_F0/2-1;       //���������� ����� �������������� � ��������, ����� �������, ��� ������� ������� 
TCCR2 = _BV(CS20)|_BV(WGM21); // ����� ������ ������ - ��� (����� ��� ����������)
                              // ������������ � �������� ���������� (7 372 800 ��)
TIMSK |= _BV(OCIE2);          // ��������� ���������� �� �������/���������
}




/**************************************************************************************
* ������� ��������� ��������� ������� ���������� ������ INT0
***************************************************************************************/
void init_int0(void){
DDRD &=~(1<<2); 				//����������� ����� INT0 ��� ����
MCUCR |=_BV(ISC01);				//���������� ����� �������������� 
MCUCR &=~_BV(ISC00);			//�� ����� ��������
GICR |=_BV(INT0); 				//��������� ������� ���������� �� INT0

}



void set_buffer_bit(uint8_t index, bool value){	//������ �������� ���� � ������ ��-���������
uint8_t byte_index;
uint8_t bit_index;
byte_index = index/8; //����������, � ����� ����� ��������� ������ ���
bit_index = index - (byte_index*8);//���������� ����� ���� � �����
if(value) 
		{
			rx_buffer[byte_index] |= (1<<(7-bit_index));
		}
else	{
			rx_buffer[byte_index] &= ~(1<<(7-bit_index));
		}
}






/*

inline trx_packet get_packet_value(){ //��������� ������ �� ����������� ������
trx_packet result;
uint8_t byte_tmp;

result.player_id = rx_buffer[0];
byte_tmp = rx_buffer[1];
byte_tmp = byte_tmp << 2; //����������� �� ��� ����� �������
byte_tmp = byte_tmp >> 4;
result.damage = pgm_read_byte(&(damage_value[byte_tmp]));
result.team_id = rx_buffer[1]>>6;

return result;
}
*/




/**************************************************************************************
* ������ ���������� "�������"
* ������������ ������ �� ������� ������ ����� ������ data_packet.data[0]
* � ��������� �������� ������
* ������� ���������� ����������  ������ ����� �������� ���� ������ 
***************************************************************************************/


void send_ir_package(void){ //���������� ����� ("��������")
FIRE_LED_ON;
cursor_position = 0; 		//������ - �� ������ ����� ������
ir_transmitter_on = true;	//��������� ��������
//while (ir_transmitter_on);	//����, ���� ����� �����������
}

/**************************************************************************************
* ��������� �������������� ������
* � �������� ��������� ������� ����������� ����������������� ����� ������ (�� 1 �� 127)
* � ���������� ���������� ������� � ���������� ���������� data_packet.player_id
* ����� ��������������� ������� ������������  data_packet.player_id.(bit_0 ... bit_7) 
***************************************************************************************/
void set_player_id(uint8_t ID){

uint8_t *p_id_bit; 							//��������� �� ���� ��������� player_id
p_id_bit = &data_packet.packet.player_id.bit_6; 	//��������� �� 6 "���" ��������� 
for (int i=0; i < 7; i++) { 				//���� ������ �������� 7 ������� ��� ID
ID = ID << 1; 								//�������� ����� �� ���� ���
if (ID&(1<<7)) 								//���� ������� ��� = 1
	{
		*p_id_bit++ = IR_ONE; 				//����������� ��������������� ��������  data_packet.player_id.bit_x
	}
else 
	{
		*p_id_bit++ = IR_ZERO; 
	}

}

data_packet.packet.player_id.bit_7 = IR_ZERO; //�������� ���������, ���� "���" ������ ���� ����� 0 

}



/**************************************************************************************
* ��������� �������������� (�����) �������
* � �������� ��������� ������� ����������� ����������������� ����� (����) ������� (�� 0 �� 3)
* � ���������� ���������� ������� � ���������� ���������� data_packet.team_id
* ����� ��������������� ������� ������������  data_packet.team_id.(bit_0 � bit_1) 
***************************************************************************************/



void set_team_color(tteam_color  color){
switch(color){

		case Red : { //�� ��������� 00 = Red
						data_packet.packet.team_id.bit_0 = IR_ZERO;
						data_packet.packet.team_id.bit_1 = IR_ZERO;
						break;	
					}
		case Blue: { //�� ��������� 01 = Blue
						data_packet.packet.team_id.bit_0 = IR_ONE;
						data_packet.packet.team_id.bit_1 = IR_ZERO;
						break;	
					}
		case Yellow: { //�� ��������� 10 = Yellow
						data_packet.packet.team_id.bit_0 = IR_ZERO;
						data_packet.packet.team_id.bit_1 = IR_ONE;
						break;	
					}
		case Green: { //�� ��������� 11 = Green
						data_packet.packet.team_id.bit_0 = IR_ONE;
						data_packet.packet.team_id.bit_1 = IR_ONE;
						break;	
					}


			}




}




/**************************************************************************************
* ��������� ��������� ��������� ������ ������ (��������� ����)
* � �������� ��������� ������� ����������� ��������� ����
* � ���������� ���������� ������� � ���������� ���������� data_packet.damage
* ����� ��������������� ������� ������������  data_packet.damage.(bit_0 � bit_3) 
***************************************************************************************/


void set_gun_damage(tgun_damage damage){

switch(damage){
		case Damage_1:{  //�� ��������� 0000 = 1
						data_packet.packet.damage.bit_0 = IR_ZERO;
						data_packet.packet.damage.bit_1 = IR_ZERO;
						data_packet.packet.damage.bit_2 = IR_ZERO;
						data_packet.packet.damage.bit_3 = IR_ZERO;
						break;
						}
		case Damage_2:{  //�� ��������� 0001 = 2
						data_packet.packet.damage.bit_0 = IR_ONE;
						data_packet.packet.damage.bit_1 = IR_ZERO;
						data_packet.packet.damage.bit_2 = IR_ZERO;
						data_packet.packet.damage.bit_3 = IR_ZERO;
						break;
						}

		case Damage_4:{  //�� ��������� 0010 = 4
						data_packet.packet.damage.bit_0 = IR_ZERO;
						data_packet.packet.damage.bit_1 = IR_ONE;
						data_packet.packet.damage.bit_2 = IR_ZERO;
						data_packet.packet.damage.bit_3 = IR_ZERO;
						break;
						}

		case Damage_5:{  //�� ��������� 0011 = 5
						data_packet.packet.damage.bit_0 = IR_ONE;
						data_packet.packet.damage.bit_1 = IR_ONE;
						data_packet.packet.damage.bit_2 = IR_ZERO;
						data_packet.packet.damage.bit_3 = IR_ZERO;
						break;
						}
		case Damage_7:{  //�� ��������� 0100 = 7
						data_packet.packet.damage.bit_0 = IR_ZERO;
						data_packet.packet.damage.bit_1 = IR_ZERO;
						data_packet.packet.damage.bit_2 = IR_ONE;
						data_packet.packet.damage.bit_3 = IR_ZERO;
						break;
						}
		case Damage_10:{  //�� ��������� 0101 = 10
						data_packet.packet.damage.bit_0 = IR_ONE;
						data_packet.packet.damage.bit_1 = IR_ZERO;
						data_packet.packet.damage.bit_2 = IR_ONE;
						data_packet.packet.damage.bit_3 = IR_ZERO;
						break;
						}
		case Damage_15:{  //�� ��������� 0110 = 15
						data_packet.packet.damage.bit_0 = IR_ZERO;
						data_packet.packet.damage.bit_1 = IR_ONE;
						data_packet.packet.damage.bit_2 = IR_ONE;
						data_packet.packet.damage.bit_3 = IR_ZERO;
						break;
						}
		case Damage_17:{  //�� ��������� 0111 = 17
						data_packet.packet.damage.bit_0 = IR_ONE;
						data_packet.packet.damage.bit_1 = IR_ONE;
						data_packet.packet.damage.bit_2 = IR_ONE;
						data_packet.packet.damage.bit_3 = IR_ZERO;
						break;
						}
		case Damage_20:{  //�� ��������� 1000 = 20
						data_packet.packet.damage.bit_0 = IR_ZERO;
						data_packet.packet.damage.bit_1 = IR_ZERO;
						data_packet.packet.damage.bit_2 = IR_ZERO;
						data_packet.packet.damage.bit_3 = IR_ONE;
						break;
						}

		case Damage_25:{  //�� ��������� 1001 = 25
						data_packet.packet.damage.bit_0 = IR_ONE;
						data_packet.packet.damage.bit_1 = IR_ZERO;
						data_packet.packet.damage.bit_2 = IR_ZERO;
						data_packet.packet.damage.bit_3 = IR_ONE;
						break;
						}

		case Damage_30:{  //�� ��������� 1010 = 30
						data_packet.packet.damage.bit_0 = IR_ZERO;
						data_packet.packet.damage.bit_1 = IR_ONE;
						data_packet.packet.damage.bit_2 = IR_ZERO;
						data_packet.packet.damage.bit_3 = IR_ONE;
						break;
						}
		case Damage_35:{  //�� ��������� 1011 = 35
						data_packet.packet.damage.bit_0 = IR_ONE;
						data_packet.packet.damage.bit_1 = IR_ONE;
						data_packet.packet.damage.bit_2 = IR_ZERO;
						data_packet.packet.damage.bit_3 = IR_ONE;
						break;
						}

		case Damage_40:{  //�� ��������� 1100 = 40
						data_packet.packet.damage.bit_0 = IR_ZERO;
						data_packet.packet.damage.bit_1 = IR_ZERO;
						data_packet.packet.damage.bit_2 = IR_ONE;
						data_packet.packet.damage.bit_3 = IR_ONE;
						break;
						}

		case Damage_50:{  //�� ��������� 1101 = 50
						data_packet.packet.damage.bit_0 = IR_ONE;
						data_packet.packet.damage.bit_1 = IR_ZERO;
						data_packet.packet.damage.bit_2 = IR_ONE;
						data_packet.packet.damage.bit_3 = IR_ONE;
						break;
						}
		case Damage_75:{  //�� ��������� 1110 = 75
						data_packet.packet.damage.bit_0 = IR_ZERO;
						data_packet.packet.damage.bit_1 = IR_ONE;
						data_packet.packet.damage.bit_2 = IR_ONE;
						data_packet.packet.damage.bit_3 = IR_ONE;
						break;
						}

		case Damage_100:{  //�� ��������� 1111 = 100
						data_packet.packet.damage.bit_0 = IR_ONE;
						data_packet.packet.damage.bit_1 = IR_ONE;
						data_packet.packet.damage.bit_2 = IR_ONE;
						data_packet.packet.damage.bit_3 = IR_ONE;
						break;
						}



			}



}




void init_var(void){            //����� �������� ����������
check_eeprom();
last_simple = 0xFFFF; //����� ����� ���� �������� ��� ������
snd_adress.curr_adress = (uint8_t*)0xFFFF; //����� ������ ������� ������������� ����
ir_transmitter_on=false; 	//�������� ���� �������� ������ (��������� ������ ��� �� �������������)
set_player_id(eeprom_read_byte(&eeprom_player_id));	//������������� ������������� ������
set_team_color(team_id());	//������������� ������������� (����) �������
set_gun_damage(gun_damage());		//������������� ��������� ������ (����)

clips=eeprom_read_byte(&eeprom_clips);//������������� ���������� �����
bullets=0; //eeprom_read_byte(&eeprom_bullets_in_clip);//������������� ���������� ��������
data_packet.packet.header = IR_START;		//�������������  ��������� (����� ����) ����������� ������������
data_packet.packet.end_of_data = 0;		//0 - ��� �������� �����������, ��� ������ ��� �������� ������ ���
cursor_position = 0; //������ - �� ������ ����� ������
start_bit_received = false;//����� ��� ��� �� ������
bit_in_rx_buff = 0;//����� ��������� ����
rx_event = NOT_EVENT;//���������� ������� ���������
//reset_clock(); //�������� ����
life = 8;//�������� -100% ������� �� �����
life_in_percent = 100;//��� ������� �� �������
key_pressing_duration.key_1    =0;//�������� �������� 
						  //������������
						  //������������ ������� ������
key_pressing_duration.key_1_inc=1;//��������� ������ ������������
key_pressing_duration.key_2    =0;//�������� �������� 
						  //������������
						  //������������ ������� ������
key_pressing_duration.key_2_inc=1;//��������� ������ ������������
chit_detected_counter=0;
chit_detected = false;
display_bullets_update_now = false;
display_batt_mode = icon;
curr_ir_pin = eeprom_read_byte(&eeprom_curr_ir_pin);//������������� �������� �� ���������

}




bool get_buffer_bit(uint8_t index){		//��������� �������� ���� � ������ ��-���������
uint8_t byte_index;
uint8_t bit_index;
byte_index = index/8; //����������, � ����� ����� ��������� ������ ���
bit_index = index - (byte_index*8);//���������� ����� ���� � �����
if(rx_buffer[byte_index]&(1<<(7-bit_index))) return true;
else return false;


}


inline trx_packet get_packet_value(){ //��������� ������ �� ����������� ������
trx_packet result;
uint8_t byte_tmp;

result.player_id = rx_buffer[0];
byte_tmp = rx_buffer[1];
byte_tmp = byte_tmp << 2; //����������� �� ��� ����� �������
byte_tmp = byte_tmp >> 4;
result.damage = pgm_read_byte(&(damage_value[byte_tmp]));
result.team_id = rx_buffer[1]>>6;

return result;
}








tteam_color team_id()//���������� ���� ����� ������� 
{


tteam_color result;
/*
	switch (SW_TEAM_IN&SW_TEAM_MASK) //�������� ��������� ������������� "DAMAGE"
	{
		case SW_TEAM_KEY1_PIN: //1-� ���� � ��������� OFF (���������), � ������ ������� (ON)
		{
			result = Blue;
			//return result;
			break;
		}
		case SW_TEAM_KEY2_PIN://2-� ���� � ��������� OFF (���������), � ������ ������� (ON)
		{
			result = Yellow;
			//return result;
			break;
		}
		
		case SW_TEAM_KEY1_PIN|SW_TEAM_KEY2_PIN: //��� ����� � ��������� OFF
		{
			result = Red;
			//return result;
			break;
		}

		case 0: //��� ����� � ��������� ON
		{
			result = Green;
			//return result;
			break;
		}
		default: result = Red;

	}
*/
result = eeprom_read_byte(&eeprom_team_id);
return result;
}



void write_team_id_to_eeprom(tteam_color color){
eeprom_write_byte(&eeprom_team_id, color);

}


/**************************************************************************************
* ������� ��������� ��������� ������� timer0
* ����� ������ ������ - ��� (����� ��� ����������)
* ������������ - � ������������� �� 1024
* ������ ��������� ����� ����� ����� ��������, ����� ����������
* �������������� 100 ���������� � ������� 
***************************************************************************************/

void init_timer0(void){

//OCR0 = F_CPU/1024/100-1;		// ���������� ������ �������������� � �������� 100 ��

OCR0 = 128; //���������� = 0,5
TCCR0 = _BV(WGM01)| _BV(WGM00);	// ����� ������ ������ - fast PWM (������� ���)
TCCR0 |=  _BV(CS00);            // ������������ � �������� ���������� 8 ���
TCCR0 |=  _BV(COM01);    		//����������������� ����� ���






//TIMSK |= _BV(OCIE0);          // ��������� ���������� �� �������/���������
		                      // ��������� ���������� ���������

}


tgun_damage gun_damage()//���������� ������� ����, ��������� ����� �����
{
/*tgun_damage result;
	switch (SW_DAMAGE_IN&SW_DAMAGE_MASK) //�������� ��������� ������������� "DAMAGE"
	{
		case SW_DAMAGE_KEY1_PIN: //1-� ���� � ��������� OFF (���������), � ������ ������� (ON)
		{
			result = Damage_50;
			//return result;
			break;
		}
		case SW_DAMAGE_KEY2_PIN://2-� ���� � ��������� OFF (���������), � ������ ������� (ON)
		{
			result = Damage_25;
			//return result;
			break;
		}
		
		case SW_DAMAGE_KEY1_PIN|SW_DAMAGE_KEY2_PIN: //��� ����� � ��������� OFF
		{
			result = Damage_10;
			//return result;
			break;
		}

		case 0: //��� ����� � ��������� ON
		{
			result = Damage_100;
			//return result;
			break;
		}
		default: result = Damage_25;

	}

return result;
*/
return eeprom_read_byte(&eeprom_damage);
}



void init_timer1(void){ //����������� timer1 �� ������� ������� ����� -8 
TCCR1A &=~_BV(WGM10); //����� ������ ������� - CTC (����� ��� ����������)
TCCR1A &=~_BV(WGM11);
TCCR1B |=_BV(WGM12); 
TCCR1B &=~_BV(WGM13); 
TCCR1A &=~_BV(COM1A0);//��������� ������ �� ������ OC1A
TCCR1A &=~_BV(COM1A1);
TCCR1B &=~_BV(COM1B0);//��������� ������ �� ������ OC1B
TCCR1B &=~_BV(COM1B1);

TCCR1B &=~_BV(CS10); //�������� = 8
TCCR1B |=_BV(CS11);
TCCR1B &=~_BV(CS12); 
//OCR1AL=60;
//OCR1AL=124;
OCR1AL=(F_CPU/8000)/8-1; // ����������� �� ������� ������� ����� - 8 ���
//OCR1AL=248; 
//OCR1AH=0x27;
//OCR1AL=0x0F;


TIMSK |= _BV(OCIE1A);  
//TIMSK |= _BV(OCIE1B);  

}



void display_life(uint8_t life_value) //���������� ������� ����� �� ������������ �������
{

uint8_t integer_part;
	for (int i=0; i<4; i++)
	{life_leds_status[i] = OFF;}

integer_part = life_value/2;
for (int i=0; i<integer_part; i++)
{
	life_leds_status[i] = ON;
}


if ((life_value-integer_part*2)>0) 
 life_leds_status[integer_part] = FQR_2HZ;
 
}


uint8_t bullets_limit(void)//���������� ����� ��������
{

uint16_t result;
	switch (SW_BULLETS_LIMIT_IN&SW_BULLETS_LIMIT_MASK) //�������� ��������� ������������� "BULLETS_LIMIT"
	{
		case SW_BULLETS_LIMIT_KEY1_PIN: //1-� ���� � ��������� OFF (���������), � ������ ������� (ON)
		{
			result = 64;
			//return result;
			break;
		}
		case SW_BULLETS_LIMIT_KEY2_PIN://2-� ���� � ��������� OFF (���������), � ������ ������� (ON)
		{
			result = 32;
			//return result;
			break;
		}
		
		case SW_BULLETS_LIMIT_KEY1_PIN|SW_BULLETS_LIMIT_KEY2_PIN: //��� ����� � ��������� OFF
		{
			result = 16;
			//return result;
			break;
		}

		case 0: //��� ����� � ��������� ON
		{
			result = 128;
			//return result;
			break;
		}
		default: result = 16;

	}

return result;


}



TFIRE_MODE_STATUS fire_mode()//���������� ������� ����� ���� (���������/���������)
{
TFIRE_MODE_STATUS result;
if (FIRE_MODE_KEY_IN&FIRE_MODE_KEY_PIN) result = single;
else  result = queues;
return result;

}




void beep(uint16_t fqr, uint16_t count, uint8_t value) //������������� ���� (�������, ������������, ���������)
{
uint16_t last_simple_tmp;
uint8_t devider; //��������, ����� ����� ��������, �����������, �� ������� ����� �������� ������� 8 ��� ����� �������� ������ (fqr)
uint16_t beep_counter; //������������ ����� � ������ (���� ���� ����� ������� ���������)

if (fqr > 4000) return; //���� ������������� ������� ���� 4 ��� �� �� � ������������� �� ������, �������

last_simple_tmp = last_simple; //�������� �������� ���������� ������ ����� �������� (����� ���� ��������������� � ��� �����)
last_simple = 0xFFFF; //����� ����� ���� ��������


devider = IR_F0/fqr; 
if (count > 160) count = 160; //��������� ����� ��������������� 16 ���������
beep_counter = (fqr/10)*count; //���������� ������ (�������� ���������)

for (uint16_t i=0; i < beep_counter; i++)
	{
		OCR0 = value;
		timer1=0;
		while (timer1 < devider);
		OCR0 = 0;
		timer1=0;
		while (timer1 < devider);

	}


last_simple = last_simple_tmp;
}

inline void damage_beep(void) // ������������� ���� ��� �������
{
WOUND_LED_ON; //�������� ��������������� ���������

beep(1000, 3, 128);
beep(500, 3, 128);
WOUND_LED_OFF;

}


void playhitsound(void) //������������� ���� ��� �������

{

snd_adress.start_adress = &pSnd_hit[0];

snd_adress.end_adress = &pSnd_hit[sizeof(pSnd_hit)-1];

snd_adress.curr_adress = &pSnd_hit[0];

play_hit_snd = true; //��������� ��������������� �����

while (play_hit_snd);// ���� ��������� ��������������� �����
}


void playclipinsound(void) //������������� ���� ��� ������������� �������
{

snd_adress.start_adress = &clipinSnd[0];

snd_adress.end_adress = &clipinSnd[sizeof(clipinSnd)-1];

snd_adress.curr_adress = &clipinSnd[0];

play_hit_snd = true; //��������� ��������������� �����

while (play_hit_snd);// ���� ��������� ��������������� �����
}


void playclipoutsound(void) //������������� ���� ��� ���������� �������
{

snd_adress.start_adress = &clipoutSnd[0];

snd_adress.end_adress = &clipoutSnd[sizeof(clipinSnd)-1];

snd_adress.curr_adress = &clipoutSnd[0];

play_hit_snd = true; //��������� ��������������� �����

while (play_hit_snd);// ���� ��������� ��������������� �����
}




void invite(){ //�����������  � ���� ��������

volatile uint8_t countdown = 5; //������� ��������� �������

lcd_clrscr();
lcd_home();
if ((eeprom_read_byte(&eeprom_tm_serial_num.device_code)==0)||(eeprom_read_byte(&eeprom_tm_serial_num.device_code)==0xFF))
	/* ���� ���� ��� �� �������*/
	{//[if]
		joystick_event = no_pressing;
		lcd_puts("������ ����� ��");
		lcd_gotoxy(0, 1);
		lcd_puts("�����.��.-������");
		//timer1 = 0;
		while ((joystick_event!=key_central_pressing)&&(eeprom_read_byte(&eeprom_tm_serial_num.device_code)==0)||(eeprom_read_byte(&eeprom_tm_serial_num.device_code)==0xFF)) //���� �� ������ ����������� ������ ��� �� ������� ����
		{//[while]
				
				while ((joystick_event==no_pressing)&&(tm_event == no_tm_event)){};
				switch(tm_event)
				{//[switch]
					case no_tm_event: 
					{
					
					}
					break;
					
					case tm_crc_error: 
					{
						lcd_clrscr();
						lcd_home();
						lcd_puts("������ CRC");
						timer1 = 0;
						while (timer1 < 60000){};
						lcd_clrscr();
						lcd_home();
						lcd_puts("������ ����� ��");
						lcd_gotoxy(0, 1);
						lcd_puts("�����.��.-������");
						tm_event=no_tm_event;
					}
					break;

					case tm_crc_ok: 
					{
					
						
						eeprom_write_byte(&eeprom_tm_serial_num.device_code,device_code);
						for (int i = 0; i<6; i++ )
						{
							eeprom_write_byte(&eeprom_tm_serial_num.serial[i],tm_code[i]);
						}
						lcd_clrscr();
						lcd_home();
						lcd_puts("���� �� �������!");
						timer1 = 0;
						while (timer1 < 60000){};
						tm_event=no_tm_event;
					
					}

					break;
				
				}//[/switch]
				if (joystick_event==key_central_pressing) 
				break;	

				if (joystick_event!=key_central_pressing) joystick_event = no_pressing;
		
			};//[/while]
		
if (joystick_event==key_central_pressing)
		/*���� ����� �� ������� ����������� ������*/
	{
		joystick_event = no_pressing;
		lcd_clrscr();
		lcd_home();

		lcd_puts("��� �������� ���\n�����. ������ 5");
		//lcd_puts("������");
		while ((countdown > 0)&&(joystick_event==no_pressing))//���� �� ��������� �������� ������ ��� �� ������ ������ ���������
		{
			timer1 = 0;
			while ((timer1 < 60000)&&(joystick_event==no_pressing)){};
			if (joystick_event!=no_pressing) break; //���� ������ ������, ������� �� ����� 
			lcd_gotoxy(14, 1);
			countdown--;
			lcd_puts(int_to_str(countdown,0));
		}

		if (joystick_event==key_central_pressing) 
		{
		
			get_all_setings();
			/*
			get_int_settings("�����. ������:", &eeprom_player_id, 127); //������ ����������� ������
			set_player_id(eeprom_read_byte(&eeprom_player_id));	//������������� ������������� ������
			get_int_settings("�����. �������:", &eeprom_team_id, 3); //������ ����������� ������
			set_team_color(team_id());	//������������� ������������� (����) �������
			get_enum_settings("��������� ����:", &eeprom_damage, &damage_value, Damage_100);
			set_gun_damage(gun_damage());		//������������� ��������� ������ (����)
			get_int_settings("������� ��������:", &eeprom_bullets_in_clip, 90); //������ ����������� ������ 
			get_int_settings("���������:", &eeprom_clips, 90);
			get_int_settings("����� ����������:", &eeprom_reload_duration, 8);
			*/
	//	return;
		}
	}


//	bullets = eeprom_read_byte(&eeprom_bullets_in_clip);
//	BULLETS_OUT_LED_OFF;
	bullets = 0;
	BULLETS_OUT_LED_ON;
	
	clips = eeprom_read_byte(&eeprom_clips);
	joystick_event=no_pressing;
	keyboard_event=no_key_pressing;
	tm_event=no_tm_event;




	}//[/if]


if ((eeprom_read_byte(&eeprom_tm_serial_num.device_code)!=0)&&(eeprom_read_byte(&eeprom_tm_serial_num.device_code)!=0xFF))

/*���� �� ���� ��� ������ � ������*/

	{//[if]
		
		volatile uint8_t tm_valide=0;
		while (!tm_valide)
		{//[while]
		lcd_clrscr();
		lcd_home();
		lcd_puts("��� ���������\n������� ����");
		//lcd_gotoxy(0, 1);
		//lcd_puts("��������� ����");
		while (tm_event == no_tm_event){};
		switch(tm_event)
				{//[switch]
					case no_tm_event: 
					{

					}
					break;
					
					case tm_crc_error: 
					{
						lcd_clrscr();
						lcd_home();
						lcd_puts("������ CRC");
						timer1 = 0;
						while (timer1 < 60000){};
						tm_event=no_tm_event;
					}
					break;

					case tm_crc_ok: 
					{
					
						if (tm_verification()) 	
						{
							tm_valide=1;				
							lcd_clrscr();
							lcd_home();
							lcd_puts("�����!");
							timer1 = 0;
							while (timer1 < 60000){};
							tm_event=no_tm_event;
							break;
						}
						lcd_clrscr();
						lcd_home();
						lcd_puts("�� ��� ����");
						timer1 = 0;
						while (timer1 < 60000){};
						tm_event=no_tm_event;

					}

					break;
				
				}//[/switch]
			}//[while]
		lcd_clrscr();
		lcd_home();
		joystick_event=no_pressing;
		keyboard_event=no_key_pressing;
		tm_event=no_tm_event;
lcd_puts("��� ��������\n������� ����  5");
//lcd_puts("������");
while ((countdown > 0)&&(tm_event == no_tm_event)&&(joystick_event==no_pressing))//���� �� ��������� �������� ������ ��� �� ������ ������ ���������
	{
		timer1 = 0;
		while ((timer1 < 60000)&&(joystick_event==no_pressing)){};
		if (joystick_event!=no_pressing) break; //���� ������ ������, ������� �� ����� 
		lcd_gotoxy(14, 1);
		countdown--;
		lcd_puts(int_to_str(countdown,0));
	}


switch(tm_event)
				{//[switch]
					case no_tm_event: 
					{

					}
					break;
					
					case tm_crc_error: 
					{
						lcd_clrscr();
						lcd_home();
						lcd_puts("������ CRC");
						timer1 = 0;
						while (timer1 < 60000){};
						lcd_clrscr();
						lcd_home();
						lcd_puts("��� ��������");
						lcd_gotoxy(0, 1);
						lcd_puts("������� ����");
						tm_event=no_tm_event;
					}
					break;

					case tm_crc_ok: 
					{
					
						if (tm_verification()) 	//���� ������
						{
							get_all_setings();
							tm_event=no_tm_event;
							break;
						}
						//����� ����
						lcd_clrscr();
						lcd_home();
						lcd_puts("�� ��� ����");
						timer1 = 0;
						while (timer1 < 60000){};
						lcd_clrscr();
						lcd_home();
						lcd_puts("��� ��������");
						lcd_gotoxy(0, 1);
						lcd_puts("������� ����");
						tm_event=no_tm_event;

					
					}

					break;
				
				}//[/switch]

/*
if (joystick_event==key_central_pressing) 
	{
		
		get_int_settings("�����. ������:", &eeprom_player_id, 127); //������ ����������� ������
		get_int_settings("�����. �������:", &eeprom_team_id, 3); //������ ����������� ������
		get_enum_settings("��������� ����:", &eeprom_damage, &damage_value, Damage_100);
		get_int_settings("������� ��������:", &eeprom_bullets_in_clip, 90); //������ ����������� ������ 
		get_int_settings("���������:", &eeprom_clips, 90);
		get_int_settings("����� ����������:", &eeprom_reload_duration, 8);
	//	return;
	}

*/


//	bullets = eeprom_read_byte(&eeprom_bullets_in_clip);
//	BULLETS_OUT_LED_OFF;

	bullets = 0;
	BULLETS_OUT_LED_ON;


	clips = eeprom_read_byte(&eeprom_clips);
	joystick_event=no_pressing;
	keyboard_event=no_key_pressing;
	}






}


char numbers[] PROGMEM={"0123456789"};
char* int_to_str(uint8_t x, uint8_t digits){
//const char numbers[10]="0123456789";

static volatile char str[6];



volatile uint8_t celoe, ostatok;
celoe=x;
//tmp = celoe/100;
//str[0]=pgm_read_byte(&numbers[tmp]);
//tmp = celoe - t100;
int digits_tmp;
digits_tmp=digits;
if (digits == 0) digits_tmp=3;
      for (int i=(digits_tmp-1); i>=0; i--)
      {   
      //volatile long int tmp;
	 // tmp = celoe;
      ostatok= celoe%10;
	  celoe  = celoe/10;
	  //ostatok= tmp - celoe*10;   
      str[i]= pgm_read_byte(&numbers[ostatok]);
      }
      str[digits_tmp]='\0';
	  
	  
	  
if (digits == 0)	
{
        while ((str[0]=='0')&str[1] !='\0') for (int i=0; i < 6; i++) str[i]=str[i+1];
}

//str[0]=numbers[0];
//str[0]=pgm_read_byte(&numbers[0]);
//str[1]=numbers[1];//pgm_read_byte(&(numbers[1]));
//str[2]=numbers[2];//pgm_read_byte(&(numbers[2]));
//str[3]='\0';

      return &str;      

}






volatile void get_int_settings(char* text, uint8_t* var_adress, uint8_t max_value){//�������� �������� �������� � ������� ��������� � ���
uint8_t result;
joystick_event=no_pressing;
result = eeprom_read_byte(var_adress);
if (result>max_value) result = max_value;

lcd_clrscr();
lcd_puts(text);
lcd_gotoxy(0, 1);
lcd_puts(int_to_str(result,3));
lcd_puts(" �����.��.-OK");

while  (joystick_event!=key_central_pressing)
{
	while  (joystick_event==no_pressing){};
	switch(joystick_event){
	case key_up_pressing: 
		{
		//	lcd_clrscr();
	//		lcd_gotoxy(0, 0);
	
			if ((result+10)<=max_value) result=result+10;
			lcd_gotoxy(0, 1);
			lcd_puts(int_to_str(result,3));
			joystick_event = no_pressing;
		}
	
		break;
		case key_right_pressing: 
		{
			//lcd_clrscr();
			//lcd_home();
			if ((result)<max_value) result++;
			lcd_gotoxy(0, 1);
			lcd_puts(int_to_str(result,3));
			//lcd_puts("������ ������ \n");
			//lcd_puts("������");
			joystick_event = no_pressing;
		}
		break;
		case key_down_pressing: 
		{
	//		lcd_clrscr();
	//		lcd_gotoxy(0, 0);
			if((result-9)>0) result=result-10;
			lcd_gotoxy(0, 1);
			lcd_puts(int_to_str(result,3));
			joystick_event = no_pressing;
		}
		break;
		case key_left_pressing: 
		{
			//lcd_clrscr();
			//lcd_gotoxy(0, 0);
			if ((result)>0) result--;
			//lcd_puts("������ ������ \n");
			//lcd_puts("�����");
			lcd_gotoxy(0, 1);
			lcd_puts(int_to_str(result,3));
			joystick_event = no_pressing;
		}
		break;
		case key_central_pressing: 
		{
			
		}
		break;
		default: joystick_event = no_pressing;
	
	
	}

}

if (result != eeprom_read_byte(var_adress))
	{
	
	lcd_clrscr();
	lcd_puts("����������...");
	eeprom_write_byte(var_adress, result);	
	}
}





volatile void get_enum_settings(char* text, uint8_t* var_adress, uint8_t* arr_adress, uint8_t max_value)
{
uint8_t result;
uint8_t value;
joystick_event=no_pressing;
result = eeprom_read_byte(var_adress);
value = pgm_read_byte(arr_adress+result);

lcd_clrscr();
lcd_puts(text);
lcd_gotoxy(0, 1);
lcd_puts(int_to_str(value,3));
lcd_puts(" �����.��.-OK");

while  (joystick_event!=key_central_pressing)
{
	while  (joystick_event==no_pressing){};
	switch(joystick_event){
	case key_up_pressing: 
		{
		//	lcd_clrscr();
	//		lcd_gotoxy(0, 0);
	
	//		if ((result+10)<=max_value) result=result+10;
	//		lcd_gotoxy(0, 1);
	//		lcd_puts(int_to_str(result,3));
			joystick_event = no_pressing;
		}
	
		break;
		case key_right_pressing: 
		{
			//lcd_clrscr();
			//lcd_home();
			if ((result)<max_value) result++;
			lcd_gotoxy(0, 1);
			lcd_puts(int_to_str(pgm_read_byte(arr_adress+result),3));
			//lcd_puts("������ ������ \n");
			//lcd_puts("������");
			joystick_event = no_pressing;
		}
		break;
		case key_down_pressing: 
		{
	//		lcd_clrscr();
	//		lcd_gotoxy(0, 0);
	//		if((result-9)>0) result=result-10;
	//		lcd_gotoxy(0, 1);
	//		lcd_puts(int_to_str(result,3));
			joystick_event = no_pressing;
		}
		break;
		case key_left_pressing: 
		{
			//lcd_clrscr();
			//lcd_gotoxy(0, 0);
			if ((result)>0) result--;
			//lcd_puts("������ ������ \n");
			//lcd_puts("�����");
			lcd_gotoxy(0, 1);
			lcd_puts(int_to_str(pgm_read_byte(arr_adress+result),3));
			joystick_event = no_pressing;
		}
		break;
		case key_central_pressing: 
		{
			
		}
		break;
		default: joystick_event = no_pressing;
	
	
	}
}

if (result != eeprom_read_byte(var_adress))
	{
	
	lcd_clrscr();
	lcd_puts("����������...");
	eeprom_write_byte(var_adress, result);	
	}

}



void check_eeprom(){

if (eeprom_read_byte(&eeprom_player_id)>127) eeprom_write_byte(&eeprom_player_id,0); 
if (eeprom_read_byte(&eeprom_team_id)>3) eeprom_write_byte(&eeprom_team_id,0); 
if (eeprom_read_byte(&eeprom_damage)>Damage_100) eeprom_write_byte(&eeprom_damage,Damage_10); 
if (eeprom_read_byte(&eeprom_bullets_in_clip)>90) eeprom_write_byte(&eeprom_bullets_in_clip,30); 
if (eeprom_read_byte(&eeprom_clips)>90) eeprom_write_byte(&eeprom_clips,15); 
if (eeprom_read_byte(&eeprom_reload_duration)>8) eeprom_write_byte(&eeprom_reload_duration,1); 
if (eeprom_read_word(&eeprom_batt_full_voltage)>1334) eeprom_write_word(&eeprom_batt_full_voltage,((DEFAULT_BATT_FULL_VOLTAGE*400)/7.5)); 
if (eeprom_read_word(&eeprom_batt_low_voltage)>1334) eeprom_write_word(&eeprom_batt_low_voltage,((DEFAULT_BATT_LOW_VOLTAGE*400)/7.5)); 
if ((eeprom_read_byte(&eeprom_curr_ir_pin)!=IR_LED_HIGH_POWER_PIN)&&(eeprom_read_byte(&eeprom_curr_ir_pin)!=IR_LED_LOW_POWER_PIN)) eeprom_write_byte(&eeprom_curr_ir_pin,IR_LED_HIGH_POWER_PIN); 

}





void display_status()//������� �� ������� ������� �������� �����, ��������, ��������� 
{
	lcd_clrscr();
	lcd_puts("�����: ");
	lcd_puts(int_to_str(life_in_percent,0));
	lcd_puts("% ");
	lcd_gotoxy(0, 1);
	lcd_puts("����: ");
	lcd_puts(int_to_str(bullets,0));
	lcd_gotoxy(10, 1);
	lcd_puts("�: ");
	lcd_puts(int_to_str(clips,0));
	lcd_puts(" ");



}


void display_life_update(){//��������� �������� ����� �� �������
lcd_gotoxy(7, 0);
lcd_puts(int_to_str(life_in_percent,0));
lcd_puts("%   ");
}



void display_bullets_update(){//��������� �������� ����� �� �������
lcd_gotoxy(6, 1);
lcd_puts(int_to_str(bullets,0));
lcd_puts(" ");
}

void display_clips_update(){//��������� �������� ����� �� �������
lcd_gotoxy(13, 1);
lcd_puts(int_to_str(clips,0));
lcd_puts(" ");
}


void display_voltage_update(){//��������� �������� ����� �� �������

volatile uint16_t adc_data;
//volatile uint16_t batt_voltage;
adc_data=ReadADC(ADC_CHANNEL);


uint16_t delta; //������� ����� ���������� ��� ��� ��������� ���������� � ��������� ����������� �������
uint8_t curr_batt_level; //������� ������ ��������� ������� (1 �� 6 ���������)
delta=eeprom_read_word(&eeprom_batt_full_voltage)-eeprom_read_word(&eeprom_batt_low_voltage);
lcd_gotoxy(12, 0);

switch(display_batt_mode)
{
	case icon:
	{
		lcd_puts("   ");
	//	lcd_gotoxy(15, 0);
		if (adc_data < eeprom_read_word(&eeprom_batt_low_voltage)) 
		{
//		curr_batt_level=0;
			lcd_putc(0);
			return;
		}
		if (adc_data > eeprom_read_word(&eeprom_batt_full_voltage))
		{
			lcd_putc(5);
			return;
		}
		curr_batt_level = (6*(adc_data - eeprom_read_word(&eeprom_batt_low_voltage)))/delta;
		lcd_putc(curr_batt_level);
		return;


	}
	break;
	case digit:
	{
		adc_data=(adc_data*15)/8;
		lcd_puts(int_to_str(adc_data/100,0));
		lcd_puts(",");
		lcd_puts(int_to_str((adc_data%100)/10,0));
		lcd_puts(" ");
		return;
	
	}
	break;

}




/*
lcd_gotoxy(12, 0);
lcd_puts(int_to_str(viltage/100,0));
lcd_puts(",");
lcd_puts(int_to_str(viltage%100,0));
lcd_puts(" ");
*/




}



void init_adc(void)
{

ADMUX=((1<<REFS0)|(1<<REFS1));//�������� ���������� �������� �������� ����������
ADCSRA=(1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0); //Rrescalar div factor =128


}


uint16_t ReadADC(uint8_t ch)
{
   //Select ADC Channel ch must be 0-7
   ch=ch&0b00000111;
   ADMUX|=ch;

   //Start Single conversion
   ADCSRA|=(1<<ADSC);

   //Wait for conversion to complete
   while(!(ADCSRA & (1<<ADIF)));

   //Clear ADIF by writing one to it
   //Note you may be wondering why we have write one to clear it
   //This is standard way of clearing bits in io as said in datasheets.
   //The code writes '1' but it result in setting bit to '0' !!!

   ADCSRA|=(1<<ADIF);

   return(ADC);
}


/********************************************
*������� ��� � ��� ����� � ����� ���� ����
********************************************/

void display_hit_data(void)
{
lcd_clrscr();
lcd_home();
lcd_puts("���� ");
lcd_puts(int_to_str(rx_packet.damage,0));
lcd_puts("% �����");
lcd_gotoxy(0, 1);
lcd_puts("��. ");
lcd_puts(int_to_str(rx_packet.player_id,0));
lcd_puts(" ���. ");
lcd_puts(int_to_str(rx_packet.team_id,0));
}





void get_ir_power_settings(void)
{
uint8_t result;
uint8_t value;
joystick_event=no_pressing;
result = eeprom_read_byte(&eeprom_curr_ir_pin);
lcd_clrscr();
lcd_puts("�������� �� ���");
lcd_gotoxy(0, 1);
if (result==IR_LED_HIGH_POWER_PIN) lcd_puts("�����");
if (result==IR_LED_LOW_POWER_PIN) lcd_puts("�����");

lcd_puts(" ���.��.-OK");

while  (joystick_event!=key_central_pressing)
{
	while  (joystick_event==no_pressing){};
	switch(joystick_event){
	case key_up_pressing: 
		{
		//	lcd_clrscr();
	//		lcd_gotoxy(0, 0);
	
	//		if ((result+10)<=max_value) result=result+10;
	//		lcd_gotoxy(0, 1);
	//		lcd_puts(int_to_str(result,3));
			joystick_event = no_pressing;
		}
	
		break;
		case key_right_pressing: 
		{
			//lcd_clrscr();
			//lcd_home();
		
		//	if ((result)<max_value) result++;
			if (result==IR_LED_LOW_POWER_PIN)
			{
				result=IR_LED_HIGH_POWER_PIN;
				lcd_gotoxy(0, 1);
				lcd_puts("�����");
			}
			
			joystick_event = no_pressing;
		}
		break;
		case key_down_pressing: 
		{
	//		lcd_clrscr();
	//		lcd_gotoxy(0, 0);
	//		if((result-9)>0) result=result-10;
	//		lcd_gotoxy(0, 1);
	//		lcd_puts(int_to_str(result,3));
			joystick_event = no_pressing;
		}
		break;
		case key_left_pressing: 
		{
			//lcd_clrscr();
			//lcd_gotoxy(0, 0);
			
			if (result==IR_LED_HIGH_POWER_PIN) 
			{
				result=IR_LED_LOW_POWER_PIN;
				lcd_gotoxy(0, 1);
				lcd_puts("�����");
			}
			
			joystick_event = no_pressing;
		}
		break;
		case key_central_pressing: 
		{
			
		}
		break;
		default: joystick_event = no_pressing;
	
	
	}
}

if (result != eeprom_read_byte(&eeprom_curr_ir_pin))
	{
	
	lcd_clrscr();
	lcd_puts("����������...");
	eeprom_write_byte(&eeprom_curr_ir_pin, result);	
	}

}




void get_all_setings(void)
{
	get_int_settings("�����. ������:", &eeprom_player_id, 127); //������ ����������� ������
	set_player_id(eeprom_read_byte(&eeprom_player_id));	//������������� ������������� ������
	get_int_settings("�����. �������:", &eeprom_team_id, 3); //������ ����������� ������
	set_team_color(team_id());	//������������� ������������� (����) �������
	get_enum_settings("��������� ����:", &eeprom_damage, &damage_value, Damage_100);
	set_gun_damage(gun_damage());		//������������� ��������� ������ (����)
	get_int_settings("������� ��������:", &eeprom_bullets_in_clip, 90); //������ ����������� ������ 
	get_int_settings("���������:", &eeprom_clips, 90);
	get_int_settings("����� ����������:", &eeprom_reload_duration, 8);
	get_ir_power_settings();
	curr_ir_pin=eeprom_read_byte(&eeprom_curr_ir_pin);

}
