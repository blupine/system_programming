#include <linux/gpio.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/delay.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/delay.h>

#define GPIO_LED 21

#define DEV_NAME "led_dev"
#define DEV_NUM 262

MODULE_LICENSE("GPL");

int led_open(struct inode* pinode, struct file* pfile){
	printk(KERN_ALERT "OPEN led_dev\n");
	gpio_request(GPIO_LED, "GPIO_LED");

	if(gpio_direction_output(GPIO_LED, 1) != 0)
		printk("err GPIO_LED\n");

	return 0;
}

int led_close(struct inode* pinode, struct file* pfile){
	printk(KERN_ALERT "RELEASE led_dev\n");
	gpio_free(GPIO_LED);
	return 0;
}

ssize_t led_write(struct file* pfile, const char __user* buffer, size_t length, loff_t* offset){
	char msg[2]=""; // msg buffer to read from user

	if(copy_from_user(msg, buffer, length) < 0){ // read from user, exception handling
		printk("led_dev write error\n");
		return -1;
	}
	// if the value read from user is 1, set led 0, else 1
	msg[0] == '1' ?	gpio_set_value(GPIO_LED, 0) : gpio_set_value(GPIO_LED, 1);

	return length;
}

struct file_operations fop = {
	.owner = THIS_MODULE,
	.open = led_open,
	.write = led_write,
	.release = led_close,
};

int __init led_init(void){
	printk(KERN_ALERT "INIT led_dev\n");
	register_chrdev(DEV_NUM, DEV_NAME, &fop);
	return 0;
}

void __exit led_exit(void){
	printk(KERN_ALERT "EXIT led_dev\n");
	unregister_chrdev(DEV_NUM, DEV_NAME);
}

module_init(led_init);
module_exit(led_exit);
