#include <stdio.h>
#include <bcm2835.h>
#include <stdlib.h>
#include <string.h>

#define GPS_SERIAL RPI_V2_GPIO_P1_10 // Это GPIO 14 на Raspberry Pi (TXD)
#define GPS_RX_BUFFER_SIZE 256

void setup() {
    if (!bcm2835_init()) {
        printf("Ошибка инициализации bcm2835. Убедитесь, что вы запускаете программу с правами суперпользователя (sudo).\n");
        exit(1);
    }

    bcm2835_gpio_fsel(GPS_SERIAL, BCM2835_GPIO_FSEL_OUTP); // Настройка пина на вывод
}

char* read_gps_data() {
    char* data = (char*)malloc(GPS_RX_BUFFER_SIZE);
    int i = 0;

    //bcm2835_gpio_write(GPS_SERIAL, HIGH); // Устанавливаем высокий уровень на пине TX

    while(1) {
        char c = getchar();
	printf("1");
        if (c == '$') {
            i = 0;
        }
        data[i++] = c;

        if (c == '\n') {
            data[i] = '\0';
            return data;
        }
    }
}

int main() {
    setup();

    while(1) {
        char* data = read_gps_data();
        if (strstr(data, "$GPGGA") != NULL) {
            char* token = strtok(data, ",");
            int count = 0;

            while (token != NULL) {
                if (count == 2) {
                    printf("Широта: %s\n", token);
                } else if (count == 4) {
                    printf("Долгота: %s\n", token);
                }
                token = strtok(NULL, ",");
                count++;
            }
        }

        free(data);
    }

    bcm2835_close();
    return 0;
}
