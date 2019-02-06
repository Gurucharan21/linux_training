#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>


#define BOARD_GPIO_IOCTL_MAGIC 0xBF
#define RED_LED_ON _IOWR(BOARD_GPIO_IOCTL_MAGIC, 1, unsigned long*)
#define YELLOW_LED_ON _IOWR(BOARD_GPIO_IOCTL_MAGIC, 2, unsigned long*)
#define BLUE_LED_ON _IOWR(BOARD_GPIO_IOCTL_MAGIC, 3, unsigned long*)
#define READ_VALUE _IOR(BOARD_GPIO_IOCTL_MAGIC, 4, unsigned long*)

int flag = 0;

struct my_gpio{
    	int pin;
    	int value;
};
static int major;


static irqreturn_t irq_handler(int irq, void *dev_id)
{
		
//	gpio_set_debounce(91,200);
	printk(KERN_INFO "invoked irq handler\n");
	flag++;
	return 0;

}
	
static int my_open(struct inode *inode, struct file *file)
{
//		printk(KERN_INFO "Inside Open function\n");
		
    	return 0;
}

static int my_close(struct inode *inode, struct file *file)
{
//        printk(KERN_INFO "Inside Close function\n");
        return 0;
}

static ssize_t my_read(struct file *file, char __user *user_buf, size_t count, loff_t *ppos)
{
    //unsigned long tmp = write_count;
        printk(KERN_INFO "Inside Read function\n");
    return 0; 
}

static ssize_t my_write(struct file *file, const char __user *user_buf, size_t count, loff_t *ppos)
{
        printk(KERN_INFO "Inside Write function\n");
     //   copy_from_user(kbuf, user_buf, count);
    //    printk(KERN_INFO "data:%s\n",kbuf);
    //    if (request_irq(gpio_to_irq(103), irq_handler, IRQF_TRIGGER_HIGH, "switch-detect", "/dev/guru"))
	//		printk(KERN_INFO "Interrupt called\n");
		return 0;
}


static long my_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
//        struct my_gpio *mygp = (struct my_gpio *)arg;
     //   printk(KERN_INFO "Inside Unlocked_ioctl function\n");
       int value = -1;

        switch(cmd)
        {
        	case RED_LED_ON:
            			gpio_set_value(24,1);
						gpio_set_value(200,0);
						gpio_set_value(90,0);
            			break;
        	case YELLOW_LED_ON:
            			gpio_set_value(24,0);
						gpio_set_value(200,1);
						gpio_set_value(90,0);
						break;
			case BLUE_LED_ON:
						gpio_set_value(24,0);
						gpio_set_value(200,0);
						gpio_set_value(90,1);
						break;
			case READ_VALUE:
						value = gpio_get_value(91);
//						printk(KERN_INFO "button\n");
						return value;
//						if (flag == 0)
//							return 1;
//						else 
//							return 0;
			default:
            	return -EINVAL;
        }
        return 0;
}


static const struct file_operations my_fops = {
    .open           = my_open,
    .release        = my_close,
    .read           = my_read,
    .write          = my_write,
    .unlocked_ioctl = my_ioctl,
};


static int __init open_init(void)
{
    major = register_chrdev(0, "guru",&my_fops);
    printk(KERN_INFO "Hello, Module is registered..! \n");


    gpio_direction_output(24,0);
    gpio_direction_output(200,0);
    gpio_direction_output(90,0);
	gpio_direction_input(91);
//	gpio_set_debounce(91,200);

	if (request_irq(gpio_to_irq(91), irq_handler, IRQF_TRIGGER_FALLING, "switch-detect", NULL))
	{	
            printk(KERN_INFO "Interrupt called\n");
	}

    return 0;
}


static void __exit close_exit(void)
{
    unregister_chrdev(major, "guru");
	free_irq(gpio_to_irq(91), NULL);
    printk(KERN_INFO "Exit..Module is Unregistered..!\n");
}

module_init(open_init);
module_exit(close_exit);

MODULE_LICENSE("GPL");

