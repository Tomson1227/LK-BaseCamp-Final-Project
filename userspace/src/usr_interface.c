#include "main.h"

int main(void)
{
    t_devices device; 
    init_device(&device);

    while(1) {
        requst_mode(&device);
        process_request(&device);
    }

    close_device(&device);
    
    return EXIT_SUCCESS;
}

static int init_hms5883l(t_hmc5883l *hmc5883l) 
{
    char buff[BUFF_SIZE];
    int rc = 0;

    hmc5883l->X_fd = open(HMC5883L_X_AXIS_FILE, O_RDONLY | O_NONBLOCK);
    hmc5883l->Y_fd = open(HMC5883L_Y_AXIS_FILE, O_RDONLY | O_NONBLOCK);
    hmc5883l->Z_fd = open(HMC5883L_Z_AXIS_FILE, O_RDONLY | O_NONBLOCK);

    if(hmc5883l->X_fd < 0 || hmc5883l->Y_fd < 0 || hmc5883l->Z_fd < 0) {
        if(hmc5883l->X_fd >= 0)
            close(hmc5883l->X_fd);

        if(hmc5883l->Y_fd >= 0)
            close(hmc5883l->Y_fd);

        if(hmc5883l->Z_fd >= 0)
            close(hmc5883l->Z_fd);

        printf("ERROR: can not open hms5883l files\n");

        return EXIT_FAILURE;
    }

    rc = read(hmc5883l->X_fd, buff, BUFF_SIZE);
    hmc5883l->X_axis = atoi(buff);
    
    if(rc <= 0) 
        return EXIT_FAILURE;

    rc = read(hmc5883l->Y_fd, buff, BUFF_SIZE);
    hmc5883l->Y_axis = atoi(buff);
    
    if(rc <= 0) 
        return EXIT_FAILURE;

    rc = read(hmc5883l->Z_fd, buff, BUFF_SIZE);
    hmc5883l->Z_axis = atoi(buff);

    if(rc <= 0) 
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
}

static void close_hms5883l(t_hmc5883l *hmc5883l)
{
    close(hmc5883l->X_fd);
    close(hmc5883l->Y_fd);
    close(hmc5883l->Z_fd);
}

static int init_stepper_motor(t_stepper *stepper)
{
    char buff[BUFF_SIZE];
    int rc;

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

    rc = read(stepper->motor_speed_fd, buff, BUFF_SIZE);
    stepper->speed = atoi(buff);

    if(rc <= 0) 
        return EXIT_FAILURE;

    rc = read(stepper->motor_busy_fd, buff, BUFF_SIZE);
    stepper->busy = atoi(buff);
    
    if(rc <= 0) 
        return EXIT_FAILURE;
    
    return EXIT_SUCCESS;
}

static void close_stepper_motor(t_stepper *stepper)
{
    close(stepper->motor_speed_fd);
    close(stepper->motor_steps_fd);
    close(stepper->motor_busy_fd);
}

static int init_hc_sr04(t_hc_sr04 *hc_sr04)
{
    char buff[BUFF_SIZE];
    int rc;

    hc_sr04->distance_fd = open(HC_SR04_DISTANCE_FILE, O_RDONLY | O_NONBLOCK);

    if(hc_sr04->distance_fd < 0)
        return EXIT_FAILURE;
    
    rc = read(hc_sr04->distance_fd, buff, BUFF_SIZE);
    hc_sr04->distance = atoi(buff);
    
    if(rc <= 0) 
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
}

static void close_hc_sr04(t_hc_sr04 *hc_sr04)
{
    close(hc_sr04->distance_fd);
}

static void init_device(t_devices *device)
{
    int rc = 0;

    rc = init_hms5883l(&device->hmc5883l);
    
    if(rc != EXIT_SUCCESS)
        goto INIT_FAIL_1;
    
    rc = init_stepper_motor(&device->stepper);
    
    if(rc != EXIT_SUCCESS)
        goto INIT_FAIL_2;
        
    rc = init_hc_sr04(&device->hc_sr04);

    if(rc != EXIT_SUCCESS)
        goto INIT_FAIL_3;

INIT_FAIL_3: close_hms5883l(&device->hmc5883l);
INIT_FAIL_2: close_stepper_motor(&device->stepper);
INIT_FAIL_1: exit(1);
}

static void close_device(t_devices *device)
{
    close_hms5883l(&device->hmc5883l);
    close_stepper_motor(&device->stepper);
    close_hc_sr04(&device->hc_sr04);
}

void requst_mode(t_devices *device)
{

}

void process_request(t_devices *device)
{

}
