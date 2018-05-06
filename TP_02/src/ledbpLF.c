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


/** GLOBAL DEFINITIONS **/

static int LED = 2;
module_param(LED, int, 0);
MODULE_PARM_DESC(LED, "Nombre de led");

static int BUT = 1;
module_param(BUT, int, 0);
MODULE_PARM_DESC(BUT, "Nombre de button");

static const int led0[2] = {4, 17};
static const int but0[1] = {18};

static int major;

/** GPIO **/

//~ static void gpio_fsel(int pin, int fun)
//~ {
//~ uint32_t reg = pin / 10;
//~ uint32_t bit = (pin % 10) * 3;
//~ uint32_t mask = 0b111 << bit;
//~ gpio_regs->gpfsel[reg] = (gpio_regs->gpfsel[reg] & ~mask) | ((fun << bit) & mask);
//~ }

static void gpio_write(int pin, int val)
{
	if (val)
		gpio_regs->gpset[pin / 32] = (1 << (pin % 32));
	else
		gpio_regs->gpclr[pin / 32] = (1 << (pin % 32));
}

static int gpio_read(int pin)
{
	return (gpio_regs->gplev[pin/32] >> (pin % 32)) & 0x1;
}

/** LEDBP **/

static int 
open_ledbp(struct inode *inode, struct file *file) {
	printk(KERN_DEBUG "open()\n");
	return 0;
}

static ssize_t 
read_ledbp(struct file *file, char *buf, size_t count, loff_t *ppos) {

	int i;
	for (i = 0; i < 1; i++) {
		int nb = gpio_read(but0[i]);
		(nb == 0) ? (*(buf+i) = '1') : (*(buf+i) = '0');
		printk(KERN_DEBUG "read() %c\n", *(buf+i)) ;
	}

	return count;
}

static ssize_t 
write_ledbp(struct file *file, const char *buf, size_t count, loff_t *ppos) {
	int val;
	int i;

	for (i = 0; i < 2; i++) {
		(*(buf+i) == '1') ? (val = 1) : (val = 0);
		gpio_write(led0[i], val);
		printk(KERN_DEBUG "write() %c\n", *(buf+i));
	}

	return count;
}

static int 
release_ledbp(struct inode *inode, struct file *file) {
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
	printk(KERN_DEBUG "Hello World TOTO!\n");
	printk(KERN_DEBUG "TOTO LED=%d !\n", LED);
	printk(KERN_DEBUG "TOTO BUT=%d !\n", BUT);
	major = register_chrdev(0, "ledbpLF", &fops_ledbp); // 0 est le numéro majeur qu'on laisse choisir par linux
	return 0;
}

/** FONCTION EXIT **/
static void __exit mon_module_cleanup(void)
{
	printk(KERN_DEBUG "Goodbye World TOTO!\n");
	unregister_chrdev(major, "ledbp");
}

module_init(mon_module_init);
module_exit(mon_module_cleanup);
