# Rs-485-communication-protocol
## Summary
The system consists of 1 master and 3 slaves. Running the communication framework I created on the RS485 line. 
### Master:
  It constantly requests information from slave-1 and slave-2 within the data_istek() function. <br/> Data from Slave-1 enters Numpad classification process. If a product request is made, the input is sent to the slave-3 within the data-gonder() function and the product is printed on the LCD screen. If a password is entered from slave-1, the master detects password authentication. If the password is correct, the 'M' command is sent to the slave-3 within the data_gonder() function. If it is detected that the password entry is wrong, the 'Sifre yanlis' command is seen on the LCD screen. <br/>
  When the ambient temperature exceeds 40 degrees, the temperature information from the slave-2 is printed on the LCD together with the 'Attention:' information and the 'E' command is sent to the slave-3 within the data_gonder() function. <br/>
  For the simulation of the crc control used in the system, the crc information calculated in the master with the toggle pin connected to the D0 port is corrupted. In this way, the incoming crc information and the calculated crc information do not match. 'CRC HATASI' command is seen on the LCD.   
![Master](https://user-images.githubusercontent.com/62069736/120198695-12386480-c22b-11eb-8284-4eba4ac5e4ee.png)
