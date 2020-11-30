#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/io.h>
#include <linux/delay.h>

MODULE_AUTHOR("Sota Sakakibara");
MODULE_DESCRIPTION("driver for LED control");
MODULE_LICENSE("GPL");
MODULE_VERSION("0.0.1");

static dev_t dev;
static struct cdev cdv;
static struct class *cls = NULL;

static volatile u32 *gpio_base = NULL;

int o(void){//トン
	gpio_base[7] = 1 << 25;
	mdelay(300);
	gpio_base[10] = 1 << 25;
	mdelay(300);
	return 0;
}

int u(void){//ツー
	gpio_base[7] = 1 << 25;
	mdelay(700);
	gpio_base[10] = 1 << 25;
	mdelay(300);
	return 0;
}

static ssize_t led_write(struct file* filp, const char* buf, size_t count, loff_t* pos)
{
	char *c;
	char alp;
	int i = 0;

	if(copy_from_user(&c,buf,sizeof(char)))
	return -EFAULT;

	while(c[i] != '\0'){
		alp = c[i];
		switch(alp){
			case 'a':o();u();break;
			case 'b':u();o();o();o();break;
			case 'c':u();o();u();o();break;
			case 'd':u();o();o();break;
			case 'e':o();break;
			case 'f':o();o();u();o();break;
			case 'g':u();u();o();break;
			case 'h':o();o();o();o();break;
			case 'i':o();o();break;
			case 'j':o();u();u();u();break;
			case 'k':u();o();u();break;
			case 'l':o();u();o();o();break;
			case 'm':u();u();break;
			case 'n':u();o();break;
			case 'o':u();u();u();break;
			case 'p':o();u();u();o();break;
			case 'q':u();u();o();u();break;
			case 'r':o();u();o();break;
			case 's':o();o();o();break;
			case 't':u();break;
			case 'u':o();o();u();break;
			case 'v':o();o();o();u();break;
		        case 'w':o();u();u();break;
		 	case 'x':u();o();o();u();break;
			case 'y':u();o();u();u();break;
			case 'z':u();u();o();o();break;
			default :printk("半角小文字で入力してください\n");
		}
		mdelay(700);
		i++;
	}
	/*
	if(c == '0'){
		gpio_base[10] = 1 << 25;
	}else if(c == '1'){
		gpio_base[7] = 1 << 25;
	}else{
		for(i=0;i<=10;i++){
			gpio_base[7] = 1 << 25;
			mdelay(500);
			gpio_base[10] = 1 << 25;
			mdelay(500);
		}
	}*/
	return 1;
}

static struct file_operations led_fops = {
	.owner = THIS_MODULE,
	.write = led_write
};

static int __init init_mod(void)
{
	int retval;

	gpio_base = ioremap_nocache(0xfe200000, 0xA0);
	const u32 led = 25;
	const u32 index = led/10;
	const u32 shift = (led%10)*3;
	const u32 mask = ~(0x7 << shift);
	gpio_base[index] = (gpio_base[index] & mask) | (0x1 << shift);

	retval =  alloc_chrdev_region(&dev, 0, 1, "myled");
	if(retval < 0){
		printk(KERN_ERR "alloc_chrdev_region failed.\n");
		return retval;
	}
	printk(KERN_INFO "%s is loaded. major:%d\n",__FILE__,MAJOR(dev));

	cdev_init(&cdv, &led_fops);
	cdv.owner = THIS_MODULE;
	retval = cdev_add(&cdv, dev, 1);
	if(retval < 0){
		printk(KERN_ERR "cdev_add failed. major:%d, minor:%d",MAJOR(dev),MINOR(dev));
		return retval;
	}
	cls = class_create(THIS_MODULE,"myled");
	if(IS_ERR(cls)){
		printk(KERN_ERR "class_create failed.");
		return PTR_ERR(cls);
	}
	device_create(cls, NULL, dev, NULL, "myled%d",MINOR(dev));

	return 0;
}
static void __exit cleanup_mod(void)
{
	cdev_del(&cdv);
	device_destroy(cls, dev);
	class_destroy(cls);
	unregister_chrdev_region(dev, 1);
	printk(KERN_INFO "%s is unloaded. major:%d\n",__FILE__,MAJOR(dev));
	iounmap(gpio_base);
}

module_init(init_mod);
module_exit(cleanup_mod);

