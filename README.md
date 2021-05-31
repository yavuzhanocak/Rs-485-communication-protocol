# Rs-485-communication-protocol
### Summary
The system consists of 1 master and 3 slaves. Running the communication framework I created on the RS485 line. Crc and ack/nack control is being done. Information is being transmitted via docklight for the administrator.<br/> 
**Used materials:**
* PIC 16f877A
* Numpad
* Servo motor
* Lm35
* Compim port
* Max487
* 2x16 Lcd <br/> 

**Package frame structure:**
```
//
//    +---------------+--------------+----------+-------------+------+-----+
//    | Master adress | Slave adress | ACK/NACK | Data Length | Data | CRC |
//    +---------------+--------------+----------+-------------+------+-----+
//
//    Master adress:    1 byte            Gönderilen veri açıklamaları:
//    Slave adress:     1 byte            D:istek data
//    ACK/NACK :        1 byte            E:yüksek sıcaklık
//    Data Length:      1 byte            N:normal sıcaklık
//    Data :            1 byte            M:Şifre doğru girdisi
//    CRC:              1 byte            Numpad giris:istenilen ürün bilgisi
//

```
**Address information:**
```

#define MASTER 0x01
#define SLAVE1 0x02
#define SLAVE2 0x03
#define SLAVE3 0x04
#define ACK 0x01
#define NACK 0xFF

```

**Package building structure:**
```
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
```
### Master:
  It constantly requests information from slave-1 and slave-2 within the data_istek() function. <br/> Data from Slave-1 enters Numpad classification process. If a product request is made, the input is sent to the slave-3 within the data-gonder() function and the product is printed on the LCD screen. If a password is entered from slave-1, the master detects password authentication. If the password is correct, the 'M' command is sent to the slave-3 within the data_gonder() function. If it is detected that the password entry is wrong, the 'Sifre yanlis' command is seen on the LCD screen. <br/>
  When the ambient temperature exceeds 40 degrees, the temperature information from the slave-2 is printed on the LCD together with the 'Attention:' information and the 'E' command is sent to the slave-3 within the data_gonder() function. <br/>
  For the simulation of the crc control used in the system, the crc information calculated in the master with the toggle pin connected to the D0 port is corrupted. In this way, the incoming crc information and the calculated crc information do not match. 'CRC HATASI' command is seen on the LCD.   
![Master_640x497](https://user-images.githubusercontent.com/62069736/120207285-e9b56800-c234-11eb-9a9c-fdfc29cc507c.jpg)
### Slave-1: <br/>
An input is requested from the user via the numpad connected to Slave-1. Numpad sends '0' character by default. The product code is transmitted to the master within the user login. If you want to login as administrator, enter   the starting'*' and '1', '2', '3' password and end with the '#' character and send the entered password to the master. 
![slave-1](https://user-images.githubusercontent.com/62069736/120206908-70b61080-c234-11eb-8c40-a8486c418038.PNG)
### Slave-2: <br/>
Slave-2 measures the ambient temperature momentarily via lm35. If the common temperature rises above 40 degrees, the temperature information is sent to the master. 

![slave-2](https://user-images.githubusercontent.com/62069736/120208075-cccd6480-c235-11eb-8e2f-53df6ef0d250.PNG)
### Slave-3: <br/>
The selected product provides motor control within the information coming from the slave-3 master. 4 servo motors are used for 4 products. DC motor is used to reduce the ambient temperature. Compim output is provided via slave-3. 
![slave-3](https://user-images.githubusercontent.com/62069736/120207411-0d78ae00-c235-11eb-9dbe-510ee76bf351.PNG)
### Proteus schematic drawing:  <br/>
![Otomat](https://user-images.githubusercontent.com/62069736/120207788-84ae4200-c235-11eb-8dd8-b8f2ec801f51.png)
### Docklight screen output: <br/>
![a](https://user-images.githubusercontent.com/62069736/120208492-35b4dc80-c236-11eb-8f8e-1cbe7f12e4b9.png)
