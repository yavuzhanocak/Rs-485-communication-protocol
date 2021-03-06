//
//    +---------------+--------------+----------+-------------+------+-----+
//    | Master adress | Slave adress | ACK/NACK | Data Length | Data | CRC |
//    +---------------+--------------+----------+-------------+------+-----+
//
//    Master adress:    1 byte            G?nderilen veri a??klamalar?:
//    Slave adress:     1 byte            D:istek data
//    ACK/NACK :        1 byte            E:y?ksek s?cakl?k
//    Data Length:      1 byte            N:normal s?cakl?k
//    Data :            1 byte            M:?ifre do?ru girdisi
//    CRC:              1 byte            Numpad giris:istenilen ?r?n bilgisi
//
#include <16f877.h>
#include <crc.c>
#fuses XT,NOWDT,NOPROTECT,NOBROWNOUT,NOLVP,NOPUT,NOWRT,NODEBUG,NOCPD
#use delay (clock=4000000) 
#use fast_io(b)
#use rs232 (baud=9600, xmit=pin_C6, rcv=pin_C7, parity=N, stop=1, TIMEOUT=10) 

#define use_portb_lcd TRUE
#include <lcd.c>

#define MASTER 0x01
#define SLAVE1 0x02
#define SLAVE2 0x03
#define SLAVE3 0x04
#define ACK 0x01
#define NACK 0xFF


#define gon pin_C4
char  data1=48,data2,crc,buffer[6],send_buffer[6];
const char pinnum[5][6]={"c","CANGA","ETi","OREO","PUF"};
int ekran=0,bilgi,x=0,y=0,z=0,t=0,a=0;

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
      buffer[5]=getc()+bilgi;
      
      if(buffer[1] == SLAVE1 && buffer[2]==ACK && buffer[5]==crc)
      {  ekran=1;data1=buffer[4]-48;}
      else if(buffer[1] == SLAVE2 && buffer[2]==ACK && buffer[5]==crc)
      { data2=buffer[4];ekran=2; }
      else if(buffer[5]!=crc)ekran=3;
   }

}
void lcd_yazi(){
   if(data1==0 && ekran!=2 && ekran!=3 ){
    printf(lcd_putc,"\f");
    lcd_gotoxy(1,1);  printf(lcd_putc,"Hos Geldiniz.");
    lcd_gotoxy(1,2);lcd_putc("lutfen urun sec");
   
   }
   else  if(ekran==1 && data1!=0 && buffer[4]!=42 && x!=2 && buffer[4]!=35){
    printf(lcd_putc,"\f");
    lcd_gotoxy(1,1);  printf(lcd_putc,"Seciminiz:");
    lcd_gotoxy(1,2);lcd_putc(pinnum[data1]);
    delay_ms(800);
  
   }
   else if(ekran==2){
   printf(lcd_putc,"\f");
   lcd_gotoxy(1,1);  printf(lcd_putc,"Dikkat:");
   lcd_gotoxy(1,2); printf(lcd_putc,"Sicaklik=%d",data2);

   }
   else if(ekran==3){
   printf(lcd_putc,"\f");
   lcd_gotoxy(1,1);  printf(lcd_putc,"HATA");
   lcd_gotoxy(1,2); printf(lcd_putc,"CRC uyusmuyor");

   }
      else if(ekran==4){
   printf(lcd_putc,"\f");
   lcd_gotoxy(1,1);  printf(lcd_putc,"HATA");
   lcd_gotoxy(1,2); printf(lcd_putc,"Sifre yanlis");

   }
}
void data_istek(){
      output_high(gon);
      putc(MASTER);
      putc(SLAVE1);
      putc('D'); 
      
      
      delay_ms(10);
      clear_interrupt(INT_RDA);
      output_low(gon);
      
      delay_ms(400);
    
      output_high(gon);
      
      delay_ms(1);
      
      putc(MASTER);
      putc(SLAVE2);
      putc('D');   
      
      
      delay_ms(10);
      clear_interrupt(INT_RDA);
      output_low(gon);
      
      delay_ms(400);

}
void data_gonder(char data_send){
     output_high(gon);
     send_buffer[0]=MASTER;
     send_buffer[1]=SLAVE3;
     send_buffer[2]=ACK;
     send_buffer[3]=1;
     send_buffer[4]=data_send;
     crc=generate_8bit_crc(send_buffer,5,0x55);
     send_buffer[5]=crc;
      delay_ms(1);
       for(int i=0;i<6;i++){
             putc(send_buffer[i]);
         }
         delay_ms(10);
      clear_interrupt(INT_RDA);
      output_low(gon);
      
   
}
void data_motor(){
      if(data1!=0 && buffer[4]!=42 && x!=2 && buffer[4]!=35){
         data_gonder(buffer[4]);
        
      }
      else  if(ekran==2){
      data_gonder('E');
      x=1;
      }
       
      else if(ekran!=2 && x==1){
     data_gonder('N');
      x=0;
  
      }
      else if(buffer[4]==42 || x==2){
      x=2;a=0;
              if(buffer[4]==49 || y==1){
              y=1;
               if(buffer[4]==50 || z==1){
                  z=1;
                     if(buffer[4]==51||t==1){
                      t=1;
                        if(buffer[4]==35){
                          data_gonder('M');
                          delay_ms(400);
                         x=0;y=0;z=0;t=0;a=1;
                        }
                     
                      }
                    }
                }
                if(buffer[4]==35 && a==0){
                ekran=4;
                         x=0;y=0;z=0;t=0;
                        }
          }
   
}

void main()
{
   enable_interrupts(GLOBAL);
   clear_interrupt(INT_RDA);
   enable_interrupts(INT_RDA);
   
  
   set_tris_b(0x00);
   Set_tris_d(0b00001111);
   lcd_init();
   lcd_gotoxy(1,1);              // point LCD cursor to col1 row1
  // lcd_putc("Kahve !");  // print on LCD 
   lcd_gotoxy(1,2);              // point LCD cursor to col1 row2
   
   while(TRUE)
   {
      lcd_yazi();
      bilgi = input(pin_d0);
      data_istek();
      data_motor();
   }
}
