#include "hmc5883l.h"

int init_hms5883l(t_hmc5883l *hmc5883l)
{
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

    if(read_axis(hmc5883l) == EXIT_FAILURE) {
        close_hms5883l(hmc5883l);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

void close_hms5883l(t_hmc5883l *hmc5883l)
{
    close(hmc5883l->X_fd);
    close(hmc5883l->Y_fd);
    close(hmc5883l->Z_fd);
}

int read_axis(t_hmc5883l *hmc5883l)
{
    char buff[BUFF_SIZE];
    int rc = 0;
    
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
