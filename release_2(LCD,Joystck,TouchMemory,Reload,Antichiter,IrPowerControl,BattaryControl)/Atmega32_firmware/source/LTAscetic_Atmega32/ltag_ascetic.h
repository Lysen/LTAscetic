#include <avr/io.h>        // ���������� ����� ��� ������ �����-�����
#include <util/delay.h>    // ���� ����������� ������������ ��������
#include <avr/interrupt.h> // ����� ������������ ����������
#include <avr/pgmspace.h>  //����� ������� ��������� � ������ ��������
#include <avr/eeprom.h>

#include "definition_of_ports_atmega16.h"
#include "hal.h"
#include "miles_protocol.h"
#include "types.h"
//#include "shift_regist_driver.h"






//����������� ���� �������
//extern void init_shift_register(void);
void configuring_ports(void);		//������������ ������
void init_timer2(void); 			//��������� ������� timer2, ����� �������������� ��� ����� � �������� ��-�������
void init_int0(void);				//����������� ������� ���������� ������ INT0
void init_tm(void);				//����������� ������� ���������� ������ INT1
void set_buffer_bit(uint8_t, bool);	//������ �������� ���� � ������ ��-���������
void send_ir_package(void);			//���������� �������������� ����� (�������)
void set_player_id(uint8_t);		//������ ������ �������������
void set_team_color(tteam_color);	//������ ���� ����� �������
void set_gun_damage(tgun_damage);	//������ ��������� ������ ������ (����)
void init_var(void);				//����������� ���������� �������� ����������
bool get_buffer_bit(uint8_t);		//��������� �������� ���� � ������ ��-���������
inline trx_packet get_packet_value(void); //��������� ������ �� ����������� ������
tteam_color team_id(void);//���������� ���� ����� ������� 
tgun_damage gun_damage(void);//���������� ������� ����, ��������� ����� �����
void init_timer0(void); //����������� timer0 �� ����� �������� ���, ��� ������ 
void init_timer1(void); //����������� timer1 �� ������� ������� ����� -8 
void display_life(uint8_t life_value);//���������� ������� ����� �� ������������ 
inline  TKEYBOARD_STATUS get_keyboard_status(void); //���������, ����� �� �����
inline  TKEYBOARD_EVENT test_keyboard(void);//��������� ������� ����������
inline  TKEYBOARD_STATUS get_reload_key_status(void);//���������, ������ �� ������� "������������"
inline  TKEYBOARD_EVENT test_reload_key(void);//��������� ������� ������� "������������"
uint8_t bullets_limit(void);//���������� ����� ��������
TFIRE_MODE_STATUS fire_mode(void);//���������� ������� ����� ���� (���������/���������)
void beep(uint16_t, uint16_t, uint8_t); //������������� ���� (�������, ������������, ���������)
void damage_beep(void); // ������������� ���� ��� �������
void playhitsound(void); //������������� ���� ��� �������
void playclipinsound(void); //������������� ���� ��� ������������� �������
void playclipoutsound(void); //������������� ���� ��� ���������� �������
void write_team_id_to_eeprom(tteam_color);
void invite(void); //�����������  � ���� ��������
//inline trx_packet get_packet_value(void); //��������� ������ �� ����������� 
char* int_to_str(uint8_t x, uint8_t digits);//����������� ����� � ������
volatile void get_int_settings(char* text, uint8_t* var_adress, uint8_t max_value);//�������� �������� �������� � ������� ��������� � ���
void check_eeprom(void);//�������� ������������ �������� ����������, �������� � eeprom
volatile void get_enum_settings(char* text, uint8_t* var_adress, uint8_t* arr_adress, uint8_t max_value);
void display_status(void);//������� �� ������� ������� �������� �����, ��������, ��������� 
void display_life_update(void);//��������� �������� ����� �� �������
void display_bullets_update(void);//��������� ���������� �������� �� �������
void display_clips_update(void);//��������� ���������� ��������� �� �������
void init_adc(void);
uint16_t ReadADC(uint8_t ch);
void display_voltage_update(void);
void display_hit_data(void);//������� ��� � ��� ����� � ����� ���� ����
//void generate_batt_symbols(void);//������� � ������ ��� ������� ���������
void get_ir_power_settings(void);
void get_all_setings(void);
//���������� ����������

extern volatile uint16_t timer1; 

extern volatile bool start_bit_received;				//���� ���� ��������������, ���� ������ �����-���
extern volatile uint16_t high_level_counter; 			//������� ������������ ������� �������� ������ �� ������ ��-���������										//������� ����� �������� ���������
extern volatile uint16_t bit_in_rx_buff; 				//���������� ���, �������� ��-���������� 
extern volatile trx_event rx_event; 					//������� ��-���������
extern volatile uint16_t low_level_counter; 			//������� ������������ ������� ������� ������ �� ������ ��-���������										
extern volatile uint8_t rx_buffer[RX_BUFFER_SIZE]; 	//������ ��-���������
extern volatile bool ir_transmitter_on;				//����, ����������� (true) ��� ����������� �������� ������ ����� ��-���� 														//������� ����� �������� ���������
extern volatile int ir_pulse_counter; 					//�������� ������� "�������" ��-�����
extern volatile int ir_space_counter; 					//�������� ������� ������������ ������������ ��������� ��-����� (����� ����� ������) 
//extern volatile trx_packet rx_packet;

extern volatile union data_packet_union  data_packet; 	//� ���� ���������� ����� ����������� ����� ������ ��� �������� ����� IR
extern volatile uint8_t cursor_position;				//��� ���������� ����� ������� ����� �������� ������� ������ data_packet.data[x]
extern volatile union data_packet_union  data_packet; 	//� ���� ���������� ����� ����������� ����� ������ ��� �������� ����� IR
extern uint8_t damage_value [] PROGMEM;
extern volatile trx_packet rx_packet;
extern volatile uint8_t life; //������� ����� (��������)
extern volatile uint8_t life_leds_status[4]; //���� ������ ����� ������� ��������� ����������� ���������� ��������
extern volatile TKEYBOARD_EVENT  keyboard_event; //������� ���������� 
extern volatile TKEYBOARD_EVENT  reload_key_event; //������� ������� "������������"
extern volatile TJOYSTICK_EVENT  joystick_event; //������� ��������� 
extern volatile struct pressing_duration key_pressing_duration;//��������� ������ �������� ������������ ������� ������
extern volatile struct joystick_pressing_duration joystick_key_pressing_duration;
extern volatile uint8_t fire_led_status; //������ ���������� ������� �������� 
extern volatile uint16_t bullets;  //���������� �������� � ����
extern volatile uint16_t last_simple;					//���������� ����� ��������� ������� �����
extern volatile bool play_hit_snd; //���� ���� ��������� (true) ��� ��������� (false) ��������������� �������������� ������
extern volatile tsnd_adress snd_adress; //� ���� ���������� ����� ������� ����� ��������� �������, ������� ����� �������������
//extern volatile uint8_t shift_register_buffer; //� ���� ���������� ����� ������� ������ ��� ���������� �������� ����� ���������
//extern const unsigned char pSnd_hit[];

extern volatile EEMEM tteam_color eeprom_team_id;
extern volatile EEMEM uint8_t eeprom_player_id;
extern volatile EEMEM tgun_damage eeprom_damage;
extern volatile EEMEM uint8_t eeprom_bullets_in_clip; // ���������� �������� � ������
extern volatile EEMEM uint8_t eeprom_clips; // ���������� �����
extern volatile EEMEM uint8_t eeprom_reload_duration; // ������������ ����������� (� ��������)
extern volatile uint8_t clips;//���-�� ���������� �����
extern volatile life_in_percent;// ������� "�����" � ��������
extern volatile uint16_t chit_detected_counter; // ������� ������������ ���������� �������
extern volatile bool chit_detected; // ����, ������� �������� true, ���� ������������� ���������� �������
extern volatile bool tm_connect; //����, ������� �������� true, ���� ������������� ���������� ����� ��������� � �����������
//extern volatile TTM_EVENT tm_event; //������� ����������� TouchMemory
extern volatile EEMEM uint16_t eeprom_batt_full_voltage; // ���������� ��������� ���������� ������� (�������� ���)
extern volatile EEMEM uint16_t eeprom_batt_low_voltage; // ���������� ��������� ����������� ������� (�������� ���)
extern volatile bool display_bullets_update_now; //����, �����������, ��� ���� �������� ��������� ���������� ��������
extern volatile TDISPLAY_BATT_MODE display_batt_mode; //����� ����������� ���������� ������� (������/�����) 
extern volatile EEMEM uint8_t eeprom_curr_ir_pin; //���������� �������� �� ���������
extern volatile uint8_t curr_ir_pin; //������� �����, ������� ����� "�������" ��� ��������, ���������� �������� �� ���������
extern volatile EEMEM ttm_serial_num eeprom_tm_serial_num;
