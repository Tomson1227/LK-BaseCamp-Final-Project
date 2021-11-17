#ifndef _STEPPER_H_
#define _STEPPER_H_

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define CLASS_DIR       "/sys/class/"
#define STEPPER_DIR     CLASS_DIR "stepper/"
#define STEPPER_SPEED_FILE  STEPPER_DIR "speed"
#define STEPPER_STEPS_FILE  STEPPER_DIR "steps"
#define STEPPER_BUSY_FILE   STEPPER_DIR "busy"

#define BUFF_SIZE 20

typedef struct s_stepper
{
    int motor_speed_fd;
    int motor_steps_fd;
    int motor_busy_fd;
}              t_stepper;

int init_stepper_motor(t_stepper *stepper);
void close_stepper_motor(t_stepper *stepper);

int stepper_is_busy(t_stepper *stepper);
int stepper_read_speed(t_stepper *stepper);
int stepper_set_speed(t_stepper *stepper, int speed);
int stepper_rotate_steps(t_stepper *stepper, int steps);
int stepper_rotate_angle(t_stepper *stepper, int angle);

#endif /* _STEPPER_H_ */
