#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>

#define DEV_PATH "/dev/pir_km"

void execute_cam(){
	printf("calling capture function!\n");
	if(fork()==0){
		execvp("/home/pi/Desktop/sp/raspicam/build/main", NULL);
//		execvp("/home/pi/Desktop/pir/build/cam", NULL);
	}
	else{
		wait(0);
	}
}

int main(){
	int fd = 0;
	char buf[1024];
	int sense_count = 0;
	fd = open(DEV_PATH, O_RDONLY);
	if(fd == -1){
		fprintf(stderr,"fopen() error : %s\n",strerror(errno));
		exit(1);
	}
	while(1){
		read(fd,buf,1);
		if(buf[0] == '1'){
			printf("sensed---\n");
			execute_cam();
			sleep(15);
		}
		else{
			sense_count = 0;
			printf("---\n");
		}
		sleep(1);
	}
}
