#include "main.h"

void requst_mode(t_devices *device);
void process_request(t_devices *device);

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

void requst_mode(t_devices *device)
{

}

void process_request(t_devices *device)
{

}
