#include <unistd.h>
#include "main.h"

int main(void)
{
    init();
    
    while(1) {
        requst_mode();
        process_request();
    }

    return EXIT_SUCCESS;
}
