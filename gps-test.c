#include <stdio.h>
#include <bcm2835.h>
#include <stdlib.h>
#include <string.h>

#define GPS_INPUT RPI_GPIO_P1_08

int main () {
        if(!bcm2835_init()) {
                printf("INITIALIZATION ERROR! \n");
                return 1;
        }
        bcm2835_gpio_fsel(GPS_INPUT, BCM2835_GPIO_FSEL_INPT);

        printf("Reading GPS-data...\n");

        while (1) {
            if (bcm2835_gpio_lev(GPS_INPUT) == LOW) {
                    printf("LOW\n");
            }
            bcm2835_delay(10);
            if (bcm2835_gpio_lev(GPS_INPUT) == HIGH) {
                    printf("HIGH\n");
            }
        }

        bcm2835_close();
        return 0;
}