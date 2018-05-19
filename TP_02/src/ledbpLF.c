#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <asm/io.h>
#include <mach/platform.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Lee Fabre, 2018");
MODULE_DESCRIPTION("Module, BLABLABLABLABLA");

struct gpio_s
{
	uint32_t gpfsel[7];
	uint32_t gpset[3];
	uint32_t gpclr[3];
	uint32_t gplev[3];
	uint32_t gpeds[3];
	uint32_t gpren[3];
	uint32_t gpfen[3];
	uint32_t gphen[3];
	uint32_t gplen[3];
	uint32_t gparen[3];
	uint32_t gpafen[3];
	uint32_t gppud[1];
	uint32_t gppudclk[3];
	uint32_t test[1];
}
*gpio_regs = (struct gpio_s *)__io_address(GPIO_BASE);


static const int LED0 = 4;
static const int LED1 = 17;
static const int BUTTON0 =18 ;

struct gpio_s
{
	uint32_t gpfsel[7];
	uint32_t gpset[3];
	uint32_t gpclr[3];
	uint32_t gplev[3];
	uint32_t gpeds[3];
	uint32_t gpren[3];
	uint32_t gpfen[3];
	uint32_t gphen[3];
	uint32_t gplen[3];
	uint32_t gparen[3];
	uint32_t gpafen[3];
	uint32_t gppud[1];
	uint32_t gppudclk[3];
	uint32_t test[1];
}

*gpio_regs = (struct gpio_s *)__io_address(GPIO_BASE);

static void gpio_fsel(int pin, int fun)
{
	uint32_t reg = pin / 10;
	uint32_t bit = (pin % 10) * 3;
	uint32_t mask = 0b111 << bit;
	gpio_regs->gpfsel[reg] = (gpio_regs->gpfsel[reg] & ~mask) | ((fun << bit) & mask);
}

static void gpio_write(int pin, bool val)
{
	if (val)
		gpio_regs->gpset[pin / 32] = (1 << (pin % 32));
	else
		gpio_regs->gpclr[pin / 32] = (1 << (pin % 32));
}

static int gpio_read(int pin)
{
	uint32_t reg = gpio_regs->gpset[pin / 32];
	reg  << (32 - pin);
	return reg >> 31;

}

static int 
open_ledbp(struct inode *inode, struct file *file)
{
	printk(KERN_DEBUG "open()\n");
	return 0;
}

static ssize_t 
read_ledbp(struct file *file, char *buf, size_t count, loff_t *ppos)
{
	int i = 0;

	printk(KERN_DEBUG "read()\n");

	if (count > 0) {
		buf[0] = gpio_read(LED0);
		i++;
		if (count>1){
			buf[1] = gpio_read(LED1);
			i++;
		}
		if (count>2){
			buf[2] = gpio_read(BUTTON0);
			i++;
		}
	}

	return i;
}

static ssize_t 
write_ledbp(struct file *file, const char *buf, size_t count, loff_t *ppos) 
{
	int i = 0;

	printk(KERN_DEBUG "write() of size %d\n",count);

	if (count > 0){
		gpio_write(LED0,buf[0]);
		i++;
		if (count > 1){
			gpio_write(LED1,buf[1]);
			i++;
		}
		if (count > 2) {
			gpio_write(BUTTON0,buf[2]);
			i++;
		}
	return i;
}

static int release_ledbp(struct inode *inode, struct file *file)
{
	printk(KERN_DEBUG "close()\n");
	return 0;
}

struct file_operations fops_ledbp =
{
	.open       = open_ledbp,
	.read       = read_ledbp,
	.write      = write_ledbp,
	.release    = release_ledbp 
};

static int __init mon_module_init(void)
{ 
	major = register_chrdev(0, "ledbpLF", &fops_ledbp); 
	return 0;
}

static void __exit mon_module_cleanup(void)
{
	unregister_chrdev(major, "ledbpLF");
}

module_init(mon_module_init);
module_exit(mon_module_cleanup);
