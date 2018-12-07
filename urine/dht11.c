#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>

#define MAXTIMINGS    85
#define DHTPIN        7 //wPi pin. physical num 7

int dat[5] = {0, 0, 0, 0, 0 };

int get_humidity()
{
    uint8_t laststate    = HIGH;
    uint8_t counter        = 0;
    uint8_t j        = 0, i;
    float    f;

    dat[0] = dat[1] = dat[2] = dat[3] = dat[4] = 0;

    pinMode( DHTPIN, OUTPUT );
    digitalWrite( DHTPIN, LOW );
    delay(18);
    digitalWrite( DHTPIN, HIGH );
    delayMicroseconds( 40 );
    pinMode( DHTPIN, INPUT );

    for ( i = 0; i < MAXTIMINGS; i++ )
    {
        counter = 0;
        while ( digitalRead( DHTPIN ) == laststate )
        {
            counter++;
            delayMicroseconds( 1 );
            if ( counter == 255 )
            {
                break;
            }
        }
        laststate = digitalRead( DHTPIN );

        if ( counter == 255 )
            break;

        if ( (i >= 4) && (i % 2 == 0) )
        {
            dat[j / 8] <<= 1;
            if ( counter > 50 )
                dat[j / 8] |= 1;
            j++;
        }
    }
    if ( (j >= 40) &&
         (dat[4] == ( (dat[0] + dat[1] + dat[2] + dat[3]) & 0xFF) ) )
    {
	return dat[0]; // only return  decimal part of humidity
    }else  {
	return 0;
       //printf("Data not good, skip\n" );
    }
    //return 0;
}

#define MOTOR_DEV "/dev/motor_dev"


void clean_sheet(){
    fprintf(stderr, "clean sheet called!\n");
    int motor_fd = 0;
    char prev_value = 0;
    char next_value = 0;
    motor_fd = open(MOTOR_DEV, O_WRONLY);
    if(motor_fd < 0){
        fprintf(stderr, "[-] open error button : %d\n", motor_fd);
        exit(-1);
    }

    write(motor_fd, "13", 2);
    sleep(5);
    write(motor_fd, "14", 2);
    sleep(5);
    close(motor_fd);
    fprintf("closing motor!\n");
}

int main( void )
{
    printf( "Urine sheet cleaner.\n" );

    int humidity = 0;

    if ( wiringPiSetup() == -1 )
        exit( 1 );

    while ( 1 ){
        humidity = get_humidity();
        if(humidity > 30){
	    printf("지렸다 : %d\n", humidity);
            clean_sheet();
        }
	else
	    printf("안지렸다 : %d\n", humidity);
        delay( 1000 );
    }

    return(0);
}

