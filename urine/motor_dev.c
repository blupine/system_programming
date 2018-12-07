#include <linux/gpio.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/delay.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/kernel.h>
#include <linux/kthread.h>

#define SERVO 23
#define DEV_NAME "motor_dev"
#define DEV_NUM 260

MODULE_LICENSE("GPL");

static char *msg = NULL;
static struct task_struct *thread;
int mark = 0;
int range = 200;


int softPWMThread(void *p)
{
	int space = 0;
	while(!kthread_should_stop())
	{
		space = range - mark;
		if(mark != 0)
		{
			gpio_set_value(SERVO, 1);
		}
		udelay(mark * 100);
		if(space != 0)
		{
			gpio_set_value(SERVO, 0);
		}
		udelay(space * 100);
	}

}

int motor_open(struct inode* pinode, struct file* pfile){
	printk(KERN_ALERT "OPEN motor1_dev\n");
	gpio_request(SERVO, "GPIO_SERVO");
	gpio_direction_output(SERVO, 0);
	gpio_set_value(SERVO, 0);
	thread = kthread_create(softPWMThread, NULL, "softPWMThread") ;
	if(thread)
	{
		wake_up_process(thread);
	}
	return 0;
}

int motor_close(struct inode* pinode, struct file* pfile){
	printk(KERN_ALERT "RELEASE motor1_dev\n");
	gpio_free(SERVO);
	kthread_stop(thread);

	return 0;
}

ssize_t motor_write(struct file *pfile, const char __user* buffer, size_t length, loff_t* offset)
{
	int res = 0;
	int ret = 0;
	ret = kstrtoint_from_user(buffer, length, 10, &res);
	if(ret)
	{
		printk("kstrtol Error %d", ret);
		return ret;
	}
	else
	{
		mark = res;
		printk("mark : %d", mark);
	}
    	return length;	
}

struct file_operations fop = {
	.owner = THIS_MODULE,
	.open = motor_open,
	.write = motor_write,
	.release = motor_close,
};

int __init motor_init(void){
	printk(KERN_ALERT "INIT motor1_dev\nmajor 260\n");
	register_chrdev(DEV_NUM, DEV_NAME, &fop);
	msg = (char*)kmalloc(256, GFP_KERNEL);
    	if(msg != NULL)
    	{
	       	printk("malloc allocator address: 0x%p\n",msg);
    	}
	return 0;
}

void __exit motor_exit(void){
	printk(KERN_ALERT "EXIT motor1_dev\n");
	if(msg)
	{
		kfree(msg);
	}
	unregister_chrdev(DEV_NUM, DEV_NAME);
}

module_init(motor_init);
module_exit(motor_exit);

