#include <linux/gpio.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/delay.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define GPIO_OUT 12
#define DEV_NAME "pir_km"
#define DEV_NUM 261

MODULE_LICENSE("GPL");

int pir_open(struct inode* pinode, struct file* pfile){
	printk(KERN_ALERT "OPEN pir_km\n");
	gpio_request(GPIO_OUT, "GPIO_PIR"); // gpio pin request
	gpio_direction_input(GPIO_OUT);
	return 0;
}

int pir_close(struct inode* pinode, struct file* pfile){
	printk(KERN_ALERT "RELEASE pir_km\n");
	gpio_free(GPIO_OUT);

	return 0;
}

ssize_t pir_read(struct file* pfile, char __user* buffer, size_t length, loff_t* offset){
	if(gpio_get_value(GPIO_OUT) == 1)	// if value read from gpio is 1
		copy_to_user(buffer,"1",1);	// it is detected and copy to user
	else					// else, it is not detected, copy 0 to user
		copy_to_user(buffer,"0",1);

	return 0;
}

struct file_operations fop = {
	.owner = THIS_MODULE,
	.open = pir_open,
	.read = pir_read,
	.release = pir_close,
};

int __init pir_init(void){
	printk(KERN_ALERT "INIT pir_km\nmajor 261\n");
	register_chrdev(DEV_NUM, DEV_NAME, &fop);
	return 0;
}

void __exit pir_exit(void){
	printk(KERN_ALERT "EXIT pir_km\n");
	unregister_chrdev(DEV_NUM, DEV_NAME);
}

module_init(pir_init);
module_exit(pir_exit);
