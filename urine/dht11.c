#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

#define MOTOR_DEV "/dev/motor_dev"
#define MAXTIMINGS    85 // timing grows up to 84
#define DHTPIN        7

int dat[5] = {0, 0, 0, 0, 0 }; // data array for get humidity & temperature data

int get_humidity()
{
    uint8_t laststate = HIGH;	// save last gpio state (LOW or HIGH)
    uint8_t count = 0;		// counts time of HIGH signal 
    uint8_t j = 0;
    uint8_t i = 0;

    dat[0] = dat[1] = dat[2] = dat[3] = dat[4] = 0;

    pinMode( DHTPIN, OUTPUT );		// set pin mode to send start signal
    digitalWrite( DHTPIN, LOW );	// send start signal, LOW siganl for 18ms
    delay(18);
    digitalWrite( DHTPIN, HIGH );	// send HIGH signal for 40us
    delayMicroseconds( 40 );
    pinMode( DHTPIN, INPUT ); 		// set pin mode to receive response from DHT sensor

    for ( i = 0; i < MAXTIMINGS; i++ )
    {
	printf("i : %d\n", i);
        count = 0;
        while ( digitalRead( DHTPIN ) == laststate )
        {
	    // read data in bits. count HIGH signal time
	    // 26~28ms means 0, over 70 means 1
            count++;
            delayMicroseconds( 1 );
            if ( count == 255 )
                break;
        }
        laststate = digitalRead( DHTPIN );

        if ( count == 255 )
            break;

        if ( (i >= 4) && (i % 2 == 0) )
        {
            dat[j / 8] <<= 1;
            if ( count > 40 ) // 26~28ms means 0, over 70 means 1, so we set the boundary 
                dat[j / 8] |= 1; // if the read bit is 1, 
            j++; // increase bit index
        }
    }
    if ( (j >= 40) &&
         (dat[4] == ( (dat[0] + dat[1] + dat[2] + dat[3]) & 0xFF) ) )
    {
	// if all 5 bytes received, and checksum also correct
	return dat[0]; // only return integer part of humidity
    }else  {
	// if invalid data
	return 0;
    }
}

void clean_sheet(){
    if(fork() == 0){
	// execute servo motor on child process
        execvp("/home/pi/Desktop/sp/urine/servo3", NULL);
    }
    else{
	// wait child process
        wait(0);
    }
}

int main( void )
{
    int humidity = 0;

    if ( wiringPiSetup() == -1 )
        exit(1);

    while(1){
        humidity = get_humidity(); // get current humidity data from dht11
        if(humidity > 45){	   // if humidtiy exceeds 45, clean urine sheet with motor
	    printf("지렸다 : %d\n", humidity); 
            clean_sheet();	   // run motor & clean urine sheet
	    // upload log to server with 'curl' linux utility
	    system("curl -XPOST -d \"pet_idx=1&msg=시트를닦았다.\" http://challenge.ajou-whois.org/sp/index.php?page=addEvent");
	    sleep(20); // delay for next sensing
 }
	else
	    printf("안지렸다 : %d\n", humidity);
        delay( 1000 );
    }

    return(0);
}

