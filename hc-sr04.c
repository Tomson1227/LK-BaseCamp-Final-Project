#include <linux/version.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/gpio.h>
#include <linux/delay.h>

#include <linux/kernel.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/hrtimer.h>
#include <linux/ktime.h>
#include <linux/device.h>
#include <linux/kdev_t.h>
#include <linux/interrupt.h> 

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Oleksandr Povshenko");
MODULE_DESCRIPTION("Driver for HC-SR04 ultrasonic sensor");

#define AM335_GPIO(bank,line)  (32 * bank + line)
#define HCSR04_INPUT    AM335_GPIO(1, 15)       // PIN 47
#define HCSR04_OUTPUT   AM335_GPIO(0, 27)       // PIN 27

// adaptation for kernels >= 4.1.0
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,1,0)
    #define  IRQF_DISABLED 0
#endif

typedef struct module_s {
    ktime_t start;
    ktime_t end;
    volatile bool loop;
    double distance;
    int irq;
}   module_t;

module_t hc_sr04;

// Interrupt handler on ECHO signal
static irqreturn_t gpio_isr(int irq, void *data)
{
    ktime_t current_time;
    module_t *module = (module_t *)data;

    current_time = ktime_get();
    
    if (gpio_get_value(HCSR04_INPUT)) {
        module->start = current_time;
    } else {
        module->end = current_time;
        module->loop = 0;
    }

	return IRQ_HANDLED;
}

static int gpio_init(module_t *module)
{
    int err = 0;

    err = gpio_request(HCSR04_OUTPUT, "TRIGGER");
	err += gpio_request(HCSR04_INPUT, "ECHO");

    if(err)
        return 1;
    
	err = gpio_direction_output(HCSR04_OUTPUT, 0);
	err += gpio_direction_input(HCSR04_INPUT);
    
    if(err)
        return 2;


	module->irq = gpio_to_irq(HCSR04_INPUT);
	err = request_irq(module->irq , gpio_isr, IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING | IRQF_DISABLED , "hc-sr04.trigger", module);

    if(err)
        return 3;

    return 0;
}

// This function is called when you write something on /sys/class/hc_sr04/value
static ssize_t hc_sr04_value_write(struct class *class, struct class_attribute *attr, const char *buf, size_t len) {
	printk(KERN_INFO "Buffer len %d bytes\n", len);
	return len;
}

// This function is called when you read /sys/class/hc_sr04/value
static ssize_t hc_sr04_value_read(struct class *class, struct class_attribute *attr, char *buf) {
	gpio_set_value(HCSR04_OUTPUT, 1);
	udelay(10);
	gpio_set_value(HCSR04_OUTPUT, 0);

	hc_sr04.start = hc_sr04.end = 0;
    hc_sr04.loop = 1;

	while (hc_sr04.loop);
    
    hc_sr04.distance = ktime_to_us(ktime_sub(hc_sr04.end ,hc_sr04.start)) * 170; // [mm]

	return sprintf(buf, "Distance: %lf [mm]\n", hc_sr04.distance); 
}

// Sysfs definitions for hc_sr04 class
static struct class_attribute hc_sr04_class_attrs[] = {
// static struct attribute_group hc_sr04_class_attrs[] = {
	__ATTR(value,	S_IRUGO | S_IWUSR, hc_sr04_value_read, hc_sr04_value_write),
	__ATTR_NULL,
};

// Name of directory created in /sys/class
static struct class hc_sr04_class = {
	.name =			"hc_sr04",
	.owner =		THIS_MODULE,
	.class_attrs =	hc_sr04_class_attrs,
    // .class_groups =	hc_sr04_class_attrs
};

static int hc_sr04_init(void)
{	
	if (class_register(&hc_sr04_class) < 0 ||
        gpio_init(&hc_sr04)) 
        return -1;

	printk(KERN_INFO "HC-SR04 driver initialized.\n");

	return 0;
}
 
static void hc_sr04_exit(void)
{
    free_irq(hc_sr04.irq, NULL);
	gpio_free(HCSR04_OUTPUT);
	gpio_free(HCSR04_INPUT);
	class_unregister(&hc_sr04_class);
	printk(KERN_INFO "HC-SR04 disabled.\n");
}
 
module_init(hc_sr04_init);
module_exit(hc_sr04_exit);
