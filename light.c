#include <unistd.h>
#include <stdio.h>
#include <bcm2835.h>
#include <stdlib.h>
#include <string.h>
#include "light_letters.c"

#define LIGHT_SERIAL RPI_V2_GPIO_P1_16 // Это GPIO 14 на Raspberry Pi (TXD)
#define GPS_RX_BUFFER_SIZE 256
#define DELAY_MASTER 3000000
#define DELAY_MINI 250000
#define DELAY 1200000

void setup() {
    if (!bcm2835_init()) {
        printf("Ошибка инициализации bcm2835. Убедитесь, что вы запускаете программу с правами суперпользователя (sudo).\n");
        exit(1);
    }

    bcm2835_gpio_fsel(LIGHT_SERIAL, BCM2835_GPIO_FSEL_OUTP); // Настройка пина на вывод
    bcm2835_gpio_write(LIGHT_SERIAL, LOW);
}

int main() {
    setup();

    char i = 0;

    while (1) {

	usleep(DELAY_MASTER);
	letter_I();
	usleep(DELAY_MINI);
	letter_D();
	usleep(DELAY_MINI);
	letter_I();
	usleep(DELAY_MINI);
	usleep(DELAY_MINI);
	letter_N();
	usleep(DELAY_MINI);
	letter_A();
	usleep(DELAY_MINI);
	letter_H();
	usleep(DELAY_MINI);
	letter_U();
	usleep(DELAY_MINI);
	letter_Y();
    }

    bcm2835_close();
    return 0;
}
