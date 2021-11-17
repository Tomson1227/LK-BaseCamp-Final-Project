#include "main.h"

int init_device(t_devices *device)
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

    return EXIT_SUCCESS;

INIT_FAIL_3: close_hms5883l(&device->hmc5883l);
INIT_FAIL_2: close_stepper_motor(&device->stepper);
INIT_FAIL_1: return -EXIT_FAILURE;
}

void close_device(t_devices *device)
{
    close_hms5883l(&device->hmc5883l);
    close_stepper_motor(&device->stepper);
    close_hc_sr04(&device->hc_sr04);
}
