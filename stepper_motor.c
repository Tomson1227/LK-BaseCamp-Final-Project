#include <linux/module.h>
#include <linux/gpio.h>
#include <linux/delay.h>
#include <linux/kthread.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <asm/io.h>
#include "pin_definition.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Oleksandr Povshenko");
MODULE_DESCRIPTION("Driver for stepper motor");

#define GPIO_USED 4

typedef struct stepper_s {
    int gpio[4];
}   stepper_t;

stepper_t motor;

static int gpio_init(stepper_t *motor)
{
    int err = 0;
    char buffer[20];

    motor->gpio[0] = GPIO_OUT_0;
    motor->gpio[1] = GPIO_OUT_1;
    motor->gpio[2] = GPIO_OUT_2;
    motor->gpio[3] = GPIO_OUT_3;

    for (int index = 0; index < GPIO_USED; ++index) {
        sprintf(buffer, "gpio_out_%d", index);
    	
        if (gpio_request(motor->gpio[index], buffer)) {
            printk(KERN_ERROR "ERROR: request %s fail", buffer);
            return 1;
        }

	    if (gpio_direction_output(motor->gpio[index], 0)) {
            printk(KERN_ERROR "ERROR: %s set direction fail", buffer);
            return 2;
        }
    }

    return 0;
}

static int stepper_init(void)
{	
	if (class_register(&hc_sr04_class) < 0 ||
        gpio_init(&hc_sr04)) 
        return -1;

	printk(KERN_INFO "stepper motor driver initialized.\n");

	return 0;
}
 
static void stepper_exit(void)
{
    for(int index = 0; index < GPIO_USED; ++index) {
	    gpio_free(motor.gpio[index]);
    }

	printk(KERN_INFO "stepper motor disabled.\n");
}
 
module_init(hc_sr04_init);
module_exit(hc_sr04_exit);
