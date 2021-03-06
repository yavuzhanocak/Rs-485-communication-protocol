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

#include <16f877.h>
#include <crc.c>
#fuses XT,NOWDT,NOPROTECT,NOBROWNOUT,NOLVP,NOPUT,NOWRT,NODEBUG,NOCPD
#use delay (clock=4000000) 
#use rs232 (baud=9600, xmit=pin_C6, rcv=pin_C7, parity=N, stop=1) // RS232 protokol?n?n 9600 bit/sn baud h?z?nda olaca??n? ve
#include <kbd.c>   // TX,RX u?lar?n?n hangi pinler olaca??n? tan?ml?yor

#define MASTER 0x01
#define SLAVE1 0x02
#define SLAVE2 0x03
#define SLAVE3 0x04
#define ACK 0x01
#define NACK 0xFF
#define BUFFER_S?ZE 64

#define enable pin_C4                                                                 

char   i,buffer[6];  // klavye isminde 80 ?yeli bir dizi tan?mlan?yor
char num=48,k,crc;
int a=4;
#int_RDA 
void seri()
{
   char t1,t2,t3;
   
   t1 = getc();
   if(t1==MASTER)
   {
      t2=getc();
      t3 = getc();
 
      if(t2==SLAVE1 && t3 == 'D')
      {
      buffer[0]=MASTER;
      buffer[1]=SLAVE1;
      buffer[2]=ACK;
      buffer[3]=1;
      buffer[4]=num;
      crc=generate_8bit_crc(buffer,5,0x55);
      buffer[5]=crc;
            delay_ms(10);
            output_high(enable);
            delay_ms(1);
            for(int i=0;i<6;i++){
             putc(buffer[i]);
            }
            delay_ms(10);
            output_low(enable);
            num=48;
         }
   }
}

void main()
{
   set_tris_d(0xF0);
   output_b(0);
   
   output_low(enable);
   enable_interrupts(GLOBAL);
   enable_interrupts(INT_RDA);
   while(TRUE)
   {
    k = kbd_getc();  
    if(k!=0){
      output_toggle(pin_b2);
      num=k;
      k=0;
    }
   }

}
