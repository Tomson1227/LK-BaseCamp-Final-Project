#include "hc-sr04.h"

int init_hc_sr04(t_hc_sr04 *hc_sr04)
{
    hc_sr04->distance_fd = open(HC_SR04_DISTANCE_FILE, O_RDONLY | O_NONBLOCK);

    if(hc_sr04->distance_fd < 0)
        return EXIT_FAILURE;
    
    if(read_distance(hc_sr04) != EXIT_SUCCESS) 
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
}

void close_hc_sr04(t_hc_sr04 *hc_sr04)
{
    close(hc_sr04->distance_fd);
}

int read_distance(t_hc_sr04 *hc_sr04)
{
    int rc;
    char buff[BUFF_SIZE];

    rc = read(hc_sr04->distance_fd, buff, BUFF_SIZE);
    hc_sr04->distance = atoi(buff);
    
    if(rc <= 0) 
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
}
