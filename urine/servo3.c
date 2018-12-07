#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <time.h>
#define MOTOR_DEV "/dev/motor_dev"

int main()
{
	int motor_fd = 0;
	char prev_value = 0;
	char next_value = 0;
	int status = 0;
	motor_fd = open(MOTOR_DEV, O_WRONLY);
	if(motor_fd < 0)
	{
		fprintf(stderr, "[-] open error button : %d\n",motor_fd);
		exit(-1);
	}

	write(motor_fd, "13", 1);
	sleep(5);

	write(motor_fd, "14", 2);
	sleep(5);
//   	write(motor_fd, "7.5", 2);
  //      sleep(1);

//	write(motor_fd, "3", 1);
//	sleep(5);
//	write(motor_fd, "0", 1);
	//write(motor_fd, "5", 1);
	close(motor_fd);
	return 0 ;
}
