#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define DEV_NAME "/dev/led_dev"

int main(){
	int fd = open(DEV_NAME, O_WRONLY);
	char buf[1024] ="";
	if(fd == -1){
		fprintf(stderr,"open() error : %s\n",strerror(errno));
		exit(1);
	}
	while(1){
		write(fd,"1",1);
		sleep(2);
		write(fd,"0",1);

		sleep(2);
	}
}

