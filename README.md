# jkbms-PB2A16S20P
Read data from JiKong (JK) BMS PB2A16S20P over RS-485 and ModBus 

Using CH340 Serial converters
```
Bus 002 Device 002: ID 1a86:7523 QinHeng Electronics CH340 serial converter
```
Tested on JK-PB2A16S20P and it should work on

- JK-PB1A16S10P
- JK-PB1A16S15P
- JK-PB2A16S15P
- JK-PB2A16S20P

# Compile and build it on linux

Use apt or yum/dnf to install ModBus and others libs (as root or sudo):
```
apt install libmodbus-dev build-essential libc6-dev
```
Add your user to following groups (as root or sudo)
```
usermod -a -G tty,dialout Your_User
```
Compile and build
```
cc -I /usr/include/modbus JK16S-modbus-get-data.c -L /usr/lib/aarch64-linux-gnu/ -lmodbus -o JK16S-modbus-get-data
```
Run it
```
./JK16S-modbus-get-data -i 1 -d /dev/ttyUSB0
```
Output on stdout
```
ID=1, USBdev=/dev/ttyUSB0, 115200, 8N1
ID=1, USBdev=/dev/ttyUSB0, 115200, 8N1 ... libmodbus context OK
ID=1, USBdev=/dev/ttyUSB0, 115200, 8N1 ... Connected
ID=1, USBdev=/dev/ttyUSB0, 115200, 8N1 ... Slave ID OK
Local System Time = 26.09.2024 08:30:10
BattCell00 = 3.331 mV
BattCell01 = 3.330 mV
BattCell02 = 3.330 mV
BattCell03 = 3.331 mV
BattCell04 = 3.331 mV
BattCell05 = 3.333 mV
BattCell06 = 3.332 mV
BattCell07 = 3.335 mV
BattCell08 = 3.331 mV
BattCell09 = 3.331 mV
BattCell10 = 3.331 mV
BattCell11 = 3.332 mV
BattCell12 = 3.332 mV
BattCell13 = 3.331 mV
BattCell14 = 3.328 mV
BattCell15 = 3.332 mV
BattCell16 = 0.000 mV it is dummy
Waiting 1s...
BattVolt = 53.301 V
BattWatt = 0.000 Watt
BatCurrent = 0.000 Amper
Temp1 = 23.3 C
Temp2 = 23.3 C
```
# Documentation
All Register are in BMS.RS485.Modbus.V1.0.pdf (mostly chinese)
