// user app for pir kernel module
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>

#define DEV_PATH "/dev/pir_km"	// pir sensor device file
#define LED_PATH "/dev/led_dev" // led module device file

void execute_cam(){
	printf("calling capture function!\n");
	if(fork()==0){
		// execute camera on child process
		execvp("/home/pi/Desktop/sp/raspicam/build/main", NULL);
	}
	else{
		wait(0); // wait child process
	}
}


void light_up(){ // light on led
	int fd = open(LED_PATH, O_WRONLY); // open led device file
	if(fd < 0){
		printf("file open error : device led\n");
	}
	else{
		write(fd, "1", 1); // light on led
		sleep(1.5);
		write(fd, "0", 1); // light off led 
		sleep(1.5);
		write(fd, "1", 1);
	}
	return;
}

int main(){
	int fd = 0;
	char buf[1024];
	fd = open(DEV_PATH, O_RDONLY);
	if(fd == -1){
		fprintf(stderr,"fopen() error : %s\n",strerror(errno));
		exit(1);
	}
	while(1){
		read(fd,buf,1); // read from pir sensor
		if(buf[0] == '1'){ // if motion detected
			printf("motion sensed. taking a picture...\n");
			light_up();   // light on led
			execute_cam();// execute pi camera
			sleep(15); // sleep for delay
		}
		else{ // if no motion detected
			printf("-----------\n");
		}
		sleep(1);
	}
}
