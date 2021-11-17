#include <linux/module.h>
#include <linux/init.h>
#include <linux/gpio.h>
#include <linux/delay.h>
#include <linux/device/class.h>
#include <linux/kthread.h>
#include "pin_definition.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Oleksandr Povshenko");
MODULE_DESCRIPTION("Driver for stepper motor");

#define GPIO_NUMB 4

struct stepper_s {
    struct platform_device *pdev;
    struct task_struct *thread;
    u8 gpio[4];
    int speed;
    int steps;
    u8 busy;
};

static void small_steps(struct stepper_s *motor);

struct stepper_s motor;

static int stepper_probe(struct platform_device *pdev)
{
	int ret;
	struct device *dev = &pdev->dev;
	struct device_node *node = dev->of_node;

    motot.pdev = pdev;

	ret = of_property_read_u32_array(node, "pins", lcd_key_dev.pins, );
	if (!ret){
		printk(KERN_INFO "STEPPER: Used pins: %d %d %d %d\n", 
            motor.gpio[0], motor.gpio[1], motor.gpio[2], motor.gpio[3]);
	} else {
		printk(KERN_WARNING "STEPPER: Used default pins\n");
        motor.gpio[0] = STEPPER_OUT_0;
        motor.gpio[1] = STEPPER_OUT_1;
        motor.gpio[2] = STEPPER_OUT_2;
        motor.gpio[3] = STEPPER_OUT_3;
	}

	dev_info(&pdev->dev, "device probed\n");

	return 0;
}

static int stepper_remove(struct platform_device* pdev)
{
	printk(KERN_INFO "STEPPER: driver removed");
	return 0;
}

static const struct of_device_id stepper_keys[] = {
    { .compatible = "stepper", },
    {},
};
MODULE_DEVICE_TABLE(of, stepper_keys);

static struct platform_driver stepper_driver = {
    .probe = stepper_probe,
    .remove = stepper_remove,
    .driver = {
        .name = "lcd-key-drv",
        .of_match_table = of_match_ptr(lcd_key_of_match),
        .owner = THIS_MODULE,
    },
};

static int thread_func(void *data)
{
    struct stepper_s *motor = (struct stepper_s *) data;
    motor->busy = 1;
    small_steps(motor);
    motor->busy = 0;

	return 0;
}

static int gpio_init(struct stepper_s *motor)
{
    u8 err;
    u8 gpio_init = 0;
    char buffer[12];
    int index;
    
    motor->gpio[0] = STEPPER_OUT_0;
    motor->gpio[1] = STEPPER_OUT_1;
    motor->gpio[2] = STEPPER_OUT_2;
    motor->gpio[3] = STEPPER_OUT_3;
    motor->speed = 10000; // default
    motor->busy = 0;

    index = 0;
    while(index < GPIO_NUMB) {
        sprintf(buffer, "gpio_out_%d", index);
    	
        err = gpio_request(motor->gpio[index], buffer);
        err += gpio_direction_output(motor->gpio[index], 0); 
        
        gpio_init |= !err << index;
        ++index;
    }

    if(gpio_init ^ 0x0F) {
        index = 0;
        while(index < GPIO_NUMB) {
            if(gpio_init & BIT(index))
    	        gpio_free(motor->gpio[index]);
            
            ++index;
        }
        
        printk(KERN_WARNING "GPIO: init fail\n");
        return -ENODEV;
    }

    return 0;
}

static inline void gpio_deinit(struct stepper_s *motor)
{
    int index = 0;
    
    while(index < GPIO_NUMB) {
        gpio_free(motor->gpio[index]);
        ++index;
    }
}

/*
static void big_steps(struct stepper_s *motor)
{
    int dl = motor->speed / 4;

    if(motor->steps > 0) {
        while(motor->steps) {
            gpio_set_value(motor->gpio[0], 1);
            gpio_set_value(motor->gpio[1], 1);
            gpio_set_value(motor->gpio[2], 0);
            gpio_set_value(motor->gpio[3], 0);
            udelay(dl);
            gpio_set_value(motor->gpio[0], 0);
            gpio_set_value(motor->gpio[1], 1);
            gpio_set_value(motor->gpio[2], 1);
            gpio_set_value(motor->gpio[3], 0);
            udelay(dl);
            gpio_set_value(motor->gpio[0], 0);
            gpio_set_value(motor->gpio[1], 0);
            gpio_set_value(motor->gpio[2], 1);
            gpio_set_value(motor->gpio[3], 1);
            udelay(dl);
            gpio_set_value(motor->gpio[0], 1);
            gpio_set_value(motor->gpio[1], 0);
            gpio_set_value(motor->gpio[2], 0);
            gpio_set_value(motor->gpio[3], 1);
            udelay(dl);
            --motor->steps;
        }
    } else {
        while(motor->steps) {
            gpio_set_value(motor->gpio[0], 0);
            gpio_set_value(motor->gpio[1], 0);
            gpio_set_value(motor->gpio[2], 1);
            gpio_set_value(motor->gpio[3], 1);
            udelay(dl);
            gpio_set_value(motor->gpio[0], 0);
            gpio_set_value(motor->gpio[1], 1);
            gpio_set_value(motor->gpio[2], 1);
            gpio_set_value(motor->gpio[3], 0);
            udelay(dl);
            gpio_set_value(motor->gpio[0], 1);
            gpio_set_value(motor->gpio[1], 1);
            gpio_set_value(motor->gpio[2], 0);
            gpio_set_value(motor->gpio[3], 0);
            udelay(dl);
            gpio_set_value(motor->gpio[0], 1);
            gpio_set_value(motor->gpio[1], 0);
            gpio_set_value(motor->gpio[2], 0);
            gpio_set_value(motor->gpio[3], 1);
            udelay(dl);
            ++motor->steps;
        }
    }

    gpio_set_value(motor->gpio[0], 0);
    gpio_set_value(motor->gpio[1], 0);
    gpio_set_value(motor->gpio[2], 0);
    gpio_set_value(motor->gpio[3], 0);
    udelay(dl);
}
*/

static void small_steps(struct stepper_s *motor) 
{    
    int dl = motor->speed / 8;

    if(motor->steps > 0) {
        while(motor->steps) {
            gpio_set_value(motor->gpio[0], 1);
            gpio_set_value(motor->gpio[1], 0);
            gpio_set_value(motor->gpio[2], 0);
            gpio_set_value(motor->gpio[3], 0);
            udelay(dl);
            gpio_set_value(motor->gpio[0], 1);
            gpio_set_value(motor->gpio[1], 1);
            gpio_set_value(motor->gpio[2], 0);
            gpio_set_value(motor->gpio[3], 0);
            udelay(dl);
            gpio_set_value(motor->gpio[0], 0);
            gpio_set_value(motor->gpio[1], 1);
            gpio_set_value(motor->gpio[2], 0);
            gpio_set_value(motor->gpio[3], 0);
            udelay(dl);
            gpio_set_value(motor->gpio[0], 0);
            gpio_set_value(motor->gpio[1], 1);
            gpio_set_value(motor->gpio[2], 1);
            gpio_set_value(motor->gpio[3], 0);
            udelay(dl);
            gpio_set_value(motor->gpio[0], 0);
            gpio_set_value(motor->gpio[2], 0);
            gpio_set_value(motor->gpio[1], 1);
            gpio_set_value(motor->gpio[3], 0);
            udelay(dl);
            gpio_set_value(motor->gpio[2], 0);
            gpio_set_value(motor->gpio[3], 0);
            gpio_set_value(motor->gpio[1], 1);
            gpio_set_value(motor->gpio[0], 1);
            udelay(dl);
            gpio_set_value(motor->gpio[1], 0);
            gpio_set_value(motor->gpio[2], 0);
            gpio_set_value(motor->gpio[3], 0);
            gpio_set_value(motor->gpio[0], 1);
            udelay(dl);
            gpio_set_value(motor->gpio[0], 1);
            gpio_set_value(motor->gpio[1], 0);
            gpio_set_value(motor->gpio[2], 0);
            gpio_set_value(motor->gpio[3], 1);
            udelay(dl);
            --motor->steps;
        }
    } else {
        while(motor->steps) {
            gpio_set_value(motor->gpio[0], 0);
            gpio_set_value(motor->gpio[1], 0);
            gpio_set_value(motor->gpio[2], 0);
            gpio_set_value(motor->gpio[3], 1);
            udelay(dl);
            gpio_set_value(motor->gpio[0], 0);
            gpio_set_value(motor->gpio[1], 0);
            gpio_set_value(motor->gpio[2], 1);
            gpio_set_value(motor->gpio[3], 1);
            udelay(dl);
            gpio_set_value(motor->gpio[0], 0);
            gpio_set_value(motor->gpio[1], 0);
            gpio_set_value(motor->gpio[2], 1);
            gpio_set_value(motor->gpio[3], 0);
            udelay(dl);
            gpio_set_value(motor->gpio[0], 0);
            gpio_set_value(motor->gpio[1], 1);
            gpio_set_value(motor->gpio[2], 1);
            gpio_set_value(motor->gpio[3], 0);
            udelay(dl);
            gpio_set_value(motor->gpio[0], 0);
            gpio_set_value(motor->gpio[1], 1);
            gpio_set_value(motor->gpio[2], 0);
            gpio_set_value(motor->gpio[3], 0);
            udelay(dl);
            gpio_set_value(motor->gpio[0], 1);
            gpio_set_value(motor->gpio[1], 1);
            gpio_set_value(motor->gpio[2], 0);
            gpio_set_value(motor->gpio[3], 0);
            udelay(dl);
            gpio_set_value(motor->gpio[0], 1);
            gpio_set_value(motor->gpio[1], 0);
            gpio_set_value(motor->gpio[2], 0);
            gpio_set_value(motor->gpio[3], 0);
            udelay(dl);
            gpio_set_value(motor->gpio[0], 1);
            gpio_set_value(motor->gpio[1], 0);
            gpio_set_value(motor->gpio[2], 0);
            gpio_set_value(motor->gpio[3], 1);
            udelay(dl);
            ++motor->steps;
        }
    }

    gpio_set_value(motor->gpio[0], 0);
    gpio_set_value(motor->gpio[1], 0);
    gpio_set_value(motor->gpio[2], 0);
    gpio_set_value(motor->gpio[3], 0);
    udelay(dl);
}

static ssize_t steps_store(struct class *class, struct class_attribute *attr, const char *buf, size_t count) {
    motor.steps = simple_strtol(buf, NULL, 0);

    motor.thread = kthread_run(thread_func, (void *)&motor, "motor_thread");
    if(IS_ERR(motor.thread)) {
		pr_err("kthread_run() failed\n");
		return -ENODEV;
	}
    
	return count;
}

static ssize_t speed_store(struct class *class, struct class_attribute *attr, const char *buf, size_t count) 
{
    motor.speed = simple_strtol(buf, NULL, 0);
	return count;
}

static ssize_t speed_show(struct class *class, struct class_attribute *attr, char *buf)
{
    sprintf(buf, "Current speed: %d rpm\n", motor.speed);

	return strlen(buf);
}

static ssize_t busy_show(struct class *class, struct class_attribute *attr, char *buf)
{
    sprintf(buf, "Stepper motor: %s\n", motor.busy ? "busy" : "redy");

	return strlen(buf);
}

CLASS_ATTR_RW(speed);
CLASS_ATTR_WO(steps);
CLASS_ATTR_RO(busy);

static struct class *attr_class;

static int __init stepper_init(void)
{	
    int err;

    err = gpio_init(&motor); 
	if (err)  
        return -ENODEV;

	attr_class = class_create(THIS_MODULE, "stepper");
	if (IS_ERR(attr_class)) {
		err = PTR_ERR(attr_class);
		printk(KERN_ERR "stepper: failed to create sysfs class: %d\n", err);
		goto err_class_create;
	}

	err = class_create_file(attr_class, &class_attr_steps);
    if (err) {
		printk(KERN_ERR "stepper: failed to create sysfs class attribute steps: %d\n", err);
        goto err_steps_class_file;
	}

	err = class_create_file(attr_class, &class_attr_speed);
    if (err) {
		printk(KERN_ERR "stepper: failed to create sysfs class attribute speed: %d\n", err);
        goto err_speed_class_file;
	}

	err = class_create_file(attr_class, &class_attr_busy);
    if (err) {
		printk(KERN_ERR "stepper: failed to create sysfs class attribute busy: %d\n", err);
        goto err_speed_class_busy;
	}
    
    printk(KERN_INFO "stepper motor driver initialized.\n");

	return 0;

err_speed_class_busy:
    class_remove_file(attr_class, &class_attr_speed);
err_speed_class_file:
    class_remove_file(attr_class, &class_attr_steps);
err_steps_class_file:
    class_destroy(attr_class);
err_class_create:
    gpio_deinit(&motor);

    return -ENODEV;
}
 
static void __exit stepper_exit(void)
{
    if(motor.thread)
		kthread_stop(motor.thread);
    class_remove_file(attr_class, &class_attr_busy);
    class_remove_file(attr_class, &class_attr_steps);
    class_remove_file(attr_class, &class_attr_speed);
    class_destroy(attr_class);
    gpio_deinit(&motor);

	printk(KERN_INFO "stepper motor disabled.\n");
}
 
module_init(stepper_init);
module_exit(stepper_exit);
