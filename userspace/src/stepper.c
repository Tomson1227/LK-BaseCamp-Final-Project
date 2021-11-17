#include "stepper.h"

int init_stepper_motor(t_stepper *stepper)
{
    stepper->motor_speed_fd = open(STEPPER_SPEED_FILE, O_RDONLY | O_NONBLOCK);
    stepper->motor_steps_fd = open(STEPPER_STEPS_FILE, O_RDONLY | O_NONBLOCK);
    stepper->motor_busy_fd =  open(STEPPER_BUSY_FILE, O_RDONLY | O_NONBLOCK);

    if (stepper->motor_speed_fd < 0 || 
        stepper->motor_steps_fd < 0 || 
        stepper->motor_busy_fd < 0) {
        
        if (stepper->motor_speed_fd >= 0)
            close(stepper->motor_speed_fd);

        if (stepper->motor_steps_fd >= 0)
            close(stepper->motor_steps_fd);

        if (stepper->motor_busy_fd >= 0)
            close(stepper->motor_busy_fd);

        printf("ERROR: can not open stepper motor files\n");

        return EXIT_FAILURE;
    }

    if(stepper_read_speed(stepper) > 0) 
        return EXIT_FAILURE;

    if(stepper_is_busy(stepper)) 
        return EXIT_FAILURE;
    
    return EXIT_SUCCESS;
}

void close_stepper_motor(t_stepper *stepper)
{
    close(stepper->motor_speed_fd);
    close(stepper->motor_steps_fd);
    close(stepper->motor_busy_fd);
}

int stepper_read_speed(t_stepper *stepper)
{
    char buff[BUFF_SIZE];
    int speed, rc;

    rc = read(stepper->motor_speed_fd, buff, BUFF_SIZE);
    speed = atoi(buff);

    if(rc <= 0) 
        return -EXIT_FAILURE;
    
    return speed;
}

/* speed limits [1 ; 640] */
int stepper_set_speed(t_stepper *stepper, int speed)
{
    char buff[BUFF_SIZE];
    int rc;

    if(speed > 640)
        speed = 640;
    else if (speed < 1)
        speed = 1;

    sprintf(buff, "%d", speed);
    rc = write(stepper->motor_speed_fd, buff, strlen(buff));

    if(rc <= 0) 
        return -EXIT_FAILURE;
    
    return EXIT_SUCCESS;
}

int stepper_is_busy(t_stepper *stepper)
{
    char buff[BUFF_SIZE];
    int busy, rc;

    rc = read(stepper->motor_busy_fd, buff, BUFF_SIZE);
    busy = atoi(buff);

    if(rc <= 0) 
        return -EXIT_FAILURE;
    
    return busy;
}

int stepper_rotate_steps(t_stepper *stepper, int steps)
{
    char buff[BUFF_SIZE];
    int rc;

    sprintf(buff, "%d", steps);
    rc = write(stepper->motor_steps_fd, buff, strlen(buff));

    if(rc <= 0)
        return -EXIT_FAILURE;
    
    return EXIT_SUCCESS;
}

int stepper_rotate_angle(t_stepper *stepper, int angle)
{
    int steps;

    steps = angle * 64000 / 5625;
    if(stepper_rotate_steps(stepper, steps))
        return -EXIT_FAILURE;
    
    return EXIT_SUCCESS;
}
