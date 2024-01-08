#include <bcm2835.h>

#define LIGHT_SERIAL RPI_V2_GPIO_P1_16
#define DELAY_MINI 250000

void dot () {
    bcm2835_gpio_write(LIGHT_SERIAL, HIGH);
    usleep(DELAY_MINI);
    bcm2835_gpio_write(LIGHT_SERIAL, LOW);
    usleep(DELAY_MINI);
    usleep(DELAY_MINI);
    usleep(DELAY_MINI);
    usleep(DELAY_MINI);
    return;
}

void dash () {
    bcm2835_gpio_write(LIGHT_SERIAL, HIGH);
    usleep(DELAY_MINI);
    usleep(DELAY_MINI);
    usleep(DELAY_MINI);
    bcm2835_gpio_write(LIGHT_SERIAL, LOW);
    usleep(DELAY_MINI);
    usleep(DELAY_MINI);
    usleep(DELAY_MINI);
    usleep(DELAY_MINI);
    return;
}

void letter_P () {
    dot();
    dash();
    dash();
    dot();
    return;
}

void letter_O () {
    dash();
    dash();
    dash();
    return;
}

void letter_H () {
    dot();
    dot();
    dot();
    dot();
    return;
}

void letter_U () {
    dot();
    dot();
    dash();
    return;
}

void letter_N () {
    dash();
    dot();
}

void letter_A () {
    dot();
    dash();
}

void letter_I () {
    dot();
    dot();
}

void letter_D () {
    dash();
    dot();
    dot();
}

void letter_Y () {
    dot();
    dash();
    dash();
    dash();
    return;
}
