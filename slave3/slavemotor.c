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
#include <crc.c>
#fuses XT,NOLVP,NOWDT,NOPROTECT 
#use delay(clock=4000000) 
#define use_portb_lcd TRUE
#include <lcd.c>
#use rs232 (baud=9600, xmit=pin_C6, rcv=pin_C7, parity=N, stop=1) 


#define MASTER 0x01
#define SLAVE1 0x02
#define SLAVE2 0x03
#define SLAVE3 0x04
#define ACK 0x01
#define NACK 0xFF
#define BUFFER_S�ZE 12

#define gon pin_C4

int sayac=0,duty=5,gecikme=0,x=0;   
char  data1=0,data2,crc,buffer[6],crcdata,y;
const int pinnum[]={ PIN_D0, PIN_D1, PIN_D2, PIN_D3, PIN_D4};
const char urun[4][5]={"canga","eti","oreo","puf"};
const int size[]={5,3,4,3};
int miktar[]={0,0,0,0,0};
void com_set(){
            delay_ms(10);
            output_high(gon);
            delay_ms(1);
            printf("\nSatilan urun miktari:");
            for(int i=0;i<4;i++){
            y=size[i]; printf("\n");
               for(int t=0;t<y;t++){
                 putc(urun[i][t]);
                 }
                    printf("=%d_adet",miktar[i+1]);
              }
            delay_ms(10);
            output_low(gon);
 
 printf("\nLutfen sifre giriniz.\n");
 
 }
#int_RDA 
void seri()
{output_low(gon);
   buffer[0] = getc();
    
   if(buffer[0]==MASTER)
   {
      buffer[1]=getc();
      buffer[2]=getc();
      buffer[3]=getc();
      buffer[4]=getc();
      crc=generate_8bit_crc(buffer,5,0x55);
      buffer[5]=getc();
      
      if(buffer[1] ==  SLAVE3 && buffer[5]==crc )
      {  
       if(buffer[4] == 'E')x=1;
      else if(buffer[4] == 'N')x=0;
      else if(buffer[4] == 'M')com_set();
      else {data1=buffer[4]-48;
            duty=5;
            miktar[data1]=miktar[data1]+1;
            }}
}
}
#int_timer0
void kesme(){
   set_timer0(232);

   if(sayac==0)
      output_high(pinnum[data1]);
   sayac++;
   if(sayac==duty)
      output_low(pinnum[data1]);
   if(sayac==100){
      sayac=0;
      gecikme++;}
   if(gecikme==50){
      duty=10;
      gecikme=0;
     }
 }

void main() 
{ 
   setup_psp(PSP_DISABLED);
   setup_timer_1(T1_DISABLED);
   setup_timer_2(T2_DISABLED,0,1);
   setup_adc_ports(NO_ANALOGS);
   setup_adc(ADC_OFF);
   setup_CCP1(CCP_OFF);
   setup_CCP2(CCP_OFF);
  
   set_tris_d(0x00);
 
   setup_timer_0(RTCC_INTERNAL | RTCC_DIV_8);
   set_timer0(232);
   
    output_low(gon);
    enable_interrupts(GLOBAL);
    enable_interrupts(INT_RDA);
    
   enable_interrupts(INT_timer0);
   enable_interrupts(GLOBAL);
 
   printf("\nLutfen sifre girin\n");
   while(1) // infinite loop 
   { 
   if(x==1)output_high(pin_d7);
   else if(x==0)output_low(pin_d7);
   }
}
   
