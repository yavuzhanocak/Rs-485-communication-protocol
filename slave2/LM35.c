//
//    +---------------+--------------+----------+-------------+------+-----+
//    | Master adress | Slave adress | ACK/NACK | Data Length | Data | CRC |
//    +---------------+--------------+----------+-------------+------+-----+
//
//    Master adress:    1 byte
//    Slave adress:     1 byte
//    ACK/NACK :        1 byte
//    Data Length:      1 byte
//    Data :            1 byte
//    CRC:              1 byte
//
#include <16f877.h>
#device ADC=10
#include <crc.c>
#include <string.h>
#include <stdlib.h>
#fuses XT,NOWDT,NOPROTECT,NOBROWNOUT,NOLVP,NOPUT,NOWRT,NODEBUG,NOCPD
#use delay (clock=4000000) 
#use fast_io(a)
#use fast_io(b)
#define use_portb_lcd TRUE
#include <lcd.c>
#use rs232 (baud=9600, xmit=pin_C6, rcv=pin_C7, parity=N, stop=1) // RS232 protokol?n?n 9600 bit/sn baud h?z?nda olaca??n? ve

#define MASTER 0x01
#define SLAVE1 0x02
#define SLAVE2 0x03
#define SLAVE3 0x04
#define ACK 0x01
#define NASK 0xFF
#define BUFFER_S?ZE 64
#define enable pin_C4                                                                 

char i,buffer[6],num=48,crc;  // klavye isminde 80 ?yeli bir dizi tan?mlan?yor
unsigned long int bilgi;
float voltaj;
int sicaklik;

#int_RDA 
void seri()
{
   char t1,t2,t3;
   
   t1 = getc();
   if(t1==MASTER)
   {
      t2=getc();
      t3 = getc();
      if(t2==SLAVE2 && t3 == 'D')
      {
      buffer[0]=MASTER;
      buffer[1]=SLAVE2;
      buffer[2]=ACK;
      buffer[3]=1;
      buffer[4]=sicaklik;
      crc=generate_8bit_crc(buffer,5,0x55);
      buffer[5]=crc;
         if(sicaklik>40){
            delay_ms(10);
            output_high(enable);
            delay_ms(1);
            for(int i=0;i<6;i++){
            putc(buffer[i]);
            }
            delay_ms(10);
            output_low(enable);
         }
         
      }
   }
  
}
int32 sayi;
char sicaklik_oku(){
   bilgi=read_adc();
   voltaj=(0.0048828125*bilgi)*1000;
   sicaklik=(voltaj/10)+1;
   lcd_gotoxy(1,2);
   printf(lcd_putc,"%d",sicaklik);
 }
void main()
{
    set_tris_a(0x01);
    set_tris_b(0x00);
   
    setup_adc(adc_clock_div_32);
    setup_adc_ports(AN0);
   
    set_adc_channel(0);
   
    output_low(enable);
    enable_interrupts(GLOBAL);
    enable_interrupts(INT_RDA);
   
    lcd_init();                   
                
    lcd_gotoxy(1,1);             
    lcd_putc("Ortam sicakli?i:"); 
    lcd_gotoxy(1,2);  
   while(TRUE)
   {
   sicaklik_oku();
    
   }

}
