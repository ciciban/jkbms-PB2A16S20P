/*
 * byAlen, OkatTech, 2024/09
 * https://libmodbus.org/reference/
 * 
 * mandatory parametres
 * -d device ttyUSBx
 * -i JK BMS ID 
 */

#define _XOPEN_SOURCE
#define _DEFAULT_SOURCE
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <libgen.h>
#include <modbus/modbus.h>

int main(int argc, char* argv[])
{
	if ( argc != 5)
	{
		printf("Usage: %s -i JK-BMS ID -d /dev/ttyUSBx\n", argv[0]);
		return -1;
	}

	int cc, idval;
	char *devval = NULL;

	while ((cc = getopt (argc, argv, "d:i:")) != -1) 
	{
		switch (cc) 
		{
			case 'd':
				devval = optarg;
				break;
			case 'i':
				idval = atoi(optarg);
				break;
			case '?':
				printf("Nekaj ne stima sa argumentima. Abort\n");
				abort();
			default:
				printf("Defitivno nekaj ne stima sa argumentima. Abort\n");
				abort();

		}
	}
	printf ("ID=%d, USBdev=%s, 115200, 8N1\n", idval, devval);

	// pocetak modbus-a
	modbus_t *ctx;
	int registerAddress;
	int numBytes;
	uint16_t buffer[512];  // buffer sa dosta rezerve, ne skodi
	
	ctx = modbus_new_rtu( devval, 115200, 'N', 8, 1 );
	if (ctx == NULL)
	{
		printf("Unable to create the libmodbus context\n" );
		return -1;
	}
	printf ("ID=%d, USBdev=%s, 115200, 8N1 ... libmodbus context OK\n", idval, devval);

	if (modbus_connect(ctx) == -1)
	{
		printf("Connection failed: %s\n", modbus_strerror( errno ) );
		modbus_free(ctx);
		return -1;
	}
	printf ("ID=%d, USBdev=%s, 115200, 8N1 ... Connected\n", idval, devval);

	if (modbus_set_slave(ctx,idval) == -1)
	{
		printf("Invalid slave ID = %d\n",idval);
		modbus_free(ctx);
		return -1;
	}	
	printf ("ID=%d, USBdev=%s, 115200, 8N1 ... Slave ID OK\n", idval, devval);

	// modbus_read_input_bits      = The function uses the Modbus function code 0x02 (read input status).
	// modbus_read_registers       = The function uses the Modbus function code 0x03 (read holding registers).
	// modbus_read_input_registers = The function uses the Modbus function code 0x04 (read input registers).
	// The holding registers and input registers have different historical meaning, but nowadays it's more common to use holding registers only.

	// int modbus_read_input_bits     (modbus_t *ctx, int addr, int nb, uint8_t *dest);
	// int modbus_read_input_registers(modbus_t *ctx, int addr, int nb, uint16_t *dest);
	// int modbus_read_registers      (modbus_t *ctx, int addr, int nb, uint16_t *dest);

	// get local system time
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	printf("Local System Time = %02d.%02d.%04d %02d:%02d:%02d\n", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec);
	char DeviceTime[20];
	sprintf(DeviceTime,"%04d-%02d-%02d %02d:%02d:%02d",tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

	// ukupno trebam iscitati decimanlo 268/2=134 U16 registri = 0x86 registir hexadecimalno
	// ne mogu i puca na ca 0x70..to many data
	// citam prvo samo napon celija
	registerAddress = 0x1200;  
	numBytes = 0x11;
	memset( buffer, '\0', sizeof buffer );
	if (modbus_read_registers( ctx, registerAddress, numBytes, buffer ) == -1)
        {
                printf("getRTC - Read failed: %s\n", modbus_strerror( errno ));
                modbus_close(ctx);
                modbus_free(ctx);
                return -1;
        }
	float BattCell00 = ((float) buffer[ 0x00 ]) / 1000.0;
	float BattCell01 = ((float) buffer[ 0x01 ]) / 1000.0;
	float BattCell02 = ((float) buffer[ 0x02 ]) / 1000.0;
	float BattCell03 = ((float) buffer[ 0x03 ]) / 1000.0;
	float BattCell04 = ((float) buffer[ 0x04 ]) / 1000.0;
	float BattCell05 = ((float) buffer[ 0x05 ]) / 1000.0;
	float BattCell06 = ((float) buffer[ 0x06 ]) / 1000.0;
	float BattCell07 = ((float) buffer[ 0x07 ]) / 1000.0;
	float BattCell08 = ((float) buffer[ 0x08 ]) / 1000.0;
	float BattCell09 = ((float) buffer[ 0x09 ]) / 1000.0;
	float BattCell10 = ((float) buffer[ 0x0A ]) / 1000.0;
	float BattCell11 = ((float) buffer[ 0x0B ]) / 1000.0;
	float BattCell12 = ((float) buffer[ 0x0C ]) / 1000.0;
	float BattCell13 = ((float) buffer[ 0x0D ]) / 1000.0;
	float BattCell14 = ((float) buffer[ 0x0E ]) / 1000.0;
	float BattCell15 = ((float) buffer[ 0x0F ]) / 1000.0;
	float BattCell16 = ((float) buffer[ 0x10 ]) / 1000.0;

	printf("\
BattCell00 = %.3f mV\n\
BattCell01 = %.3f mV\n\
BattCell02 = %.3f mV\n\
BattCell03 = %.3f mV\n\
BattCell04 = %.3f mV\n\
BattCell05 = %.3f mV\n\
BattCell06 = %.3f mV\n\
BattCell07 = %.3f mV\n\
BattCell08 = %.3f mV\n\
BattCell09 = %.3f mV\n\
BattCell10 = %.3f mV\n\
BattCell11 = %.3f mV\n\
BattCell12 = %.3f mV\n\
BattCell13 = %.3f mV\n\
BattCell14 = %.3f mV\n\
BattCell15 = %.3f mV\n\
BattCell16 = %.3f mV it is dummy\n",\
BattCell00,BattCell01,BattCell02,BattCell03,BattCell04,BattCell05,BattCell06,BattCell07,BattCell08,BattCell09,BattCell10,BattCell11,BattCell12,BattCell13,BattCell14,BattCell15,BattCell16);
	
	// adresa 0x1290	
	// baterija mV  UINT32
	// baterija mW  UINT32
	// baterija mA  UINT32
	// baterija temp1 0.1 Celsija  UINT16
	// baterija temp2 0.1 Celsija  UINT16
	registerAddress = 0x1290;  
	numBytes = 0x0A;
	memset( buffer, '\0', sizeof buffer );
	printf("Waiting 1s...\n");
	sleep(1); // inace time out !?
	if (modbus_read_registers( ctx, registerAddress, numBytes, buffer ) == -1)
        {
                printf("getRTC - Read failed: %s\n", modbus_strerror( errno ));
                modbus_close(ctx);
                modbus_free(ctx);
                return -1;
        }
	long BattmVolt = buffer[ 0x00 ] << 16;
    	BattmVolt |= buffer[ 0x01 ];
	float BattVolt = ((float) BattmVolt) / 1000.0;
	printf("BattVolt = %.3f V\n",BattVolt );

	long BattmWatt = buffer[ 0x02 ] << 16;
    	BattmWatt |= buffer[ 0x03 ];
	float BattWatt = ((float) BattmWatt) / 1000.0;
	printf("BattWatt = %.3f Watt\n",BattWatt );

	long BatmCurrent = buffer[ 0x04 ] << 16;
    	BatmCurrent |= buffer[ 0x05 ];
	float BatCurrent = ((float) BatmCurrent) / 1000.0;
	printf("BatCurrent = %.3f Amper\n",BatCurrent );
	
	float BatTemp1 = ((float) buffer[ 0x06 ]) / 10.0;
	float BatTemp2 = ((float) buffer[ 0x07 ]) / 10.0;
	printf("Temp1 = %.1f C\nTemp2 = %.1f C\n",BatTemp1, BatTemp2 );

	modbus_close(ctx);
	modbus_free(ctx);

	return 0;
}
