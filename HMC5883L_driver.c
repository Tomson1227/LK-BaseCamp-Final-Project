#include <linux/module.h>
#include <linux/init.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Oleksandr Povshenko");
MODULE_DESCRIPTION("Driver for gyroscope module HMC5883L");

static int HMC5883L_init(void)
{	


	printk(KERN_INFO "HMC5883L driver initialized.\n");

	return 0;
}
 
static void HMC5883L_exit(void)
{
	printk(KERN_INFO "HMC5883L disabled.\n");
}
 
module_init(HMC5883L_init);
module_exit(HMC5883L_exit);
