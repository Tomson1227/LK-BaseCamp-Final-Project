#ifndef _MAIN_H_
#define _MAIN_H_

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define CLASS_DIR       "/sys/class/"

#define HMC5883L_DIR    CLASS_DIR "hmc5883l/"
#define HMC5883L_X_AXIS_FILE     HMC5883L_DIR "x_axis"
#define HMC5883L_Y_AXIS_FILE     HMC5883L_DIR "y_axis"
#define HMC5883L_Z_AXIS_FILE     HMC5883L_DIR "z_axis"

#define STEPPER_DIR     CLASS_DIR "stepper/"
#define STEPPER_SPEED_FILE  STEPPER_DIR "speed"
#define STEPPER_STEPS_FILE  STEPPER_DIR "steps"
#define STEPPER_BUSY_FILE   STEPPER_DIR "busy"

#define HC_SR04_DIR     CLASS_DIR "hc_sr04/"
#define HC_SR04_DISTANCE_FILE   HC_SR04_DIR "distance"

#define BUFF_SIZE 20

typedef enum e_mode {
    MODE_1 = 0,
    MODE_2,
    MODE_3,
    MODE_4,
    MODE_5,
    MODE_6,
    MODE_7
} t_mode;

typedef struct s_hmc5883l {
    int X_fd;
    int Y_fd;
    int Z_fd;
    int X_axis;
    int Y_axis;
    int Z_axis;
} t_hmc5883l;

typedef struct s_stepper
{
    int motor_speed_fd;
    int motor_steps_fd;
    int motor_busy_fd;
    int speed;
    int busy;
} t_stepper;

typedef struct s_hc_sr04
{
    int distance_fd;
    int distance;
} t_hc_sr04;


typedef struct s_devices{
    t_mode mode;
    t_hmc5883l  hmc5883l;
    t_stepper   stepper; 
    t_hc_sr04   hc_sr04; 
} t_devices;

static void init_device(t_devices *device);
static void close_device(t_devices *device);
void requst_mode(t_devices *device);
void process_request(t_devices *device);

#endif /* _MAIN_H_ */
