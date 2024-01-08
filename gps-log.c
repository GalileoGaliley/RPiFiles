#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

#define UART_DEVICE "/dev/ttyAMA0" // Путь к устройству UART на Raspberry Pi
#define GPS_BUFFER_SIZE 128

int setupUART() {
    int uart_fd = open(UART_DEVICE, O_RDWR);
    if (uart_fd == -1) {
        perror("Failed to open UART");
        return -1;
    }

    struct termios options;
    tcgetattr(uart_fd, &options);
    cfsetispeed(&options, B9600);
    cfsetospeed(&options, B9600);
    options.c_cflag |= (CLOCAL | CREAD);
    options.c_cflag &= ~PARENB;
    options.c_cflag &= ~CSTOPB;
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;
    tcsetattr(uart_fd, TCSANOW, &options);

    return uart_fd;
}

int main() {
    int uart_fd = setupUART();
    if (uart_fd == -1) {
        return 1;
    }

    char gpsData[GPS_BUFFER_SIZE];
    memset(gpsData, 0, GPS_BUFFER_SIZE);

    int i = 0;
    while (1) {
	i++;
        char c;
        if (read(uart_fd, &c, 1) == 1) {
            if (c == '\n') {
                printf("Received GPS data: %s\n", gpsData);
                if (i >= 7000) {
                system("clear");
                i = 0;
            }
		// Здесь можно добавить код для разбора данных GPS
                // Проанализируйте содержимое gpsData, чтобы извлечь нужную информацию
                memset(gpsData, 0, GPS_BUFFER_SIZE); // Очистим буфер
            } else {
                strncat(gpsData, &c, 1);
            }
        }
    }

    close(uart_fd);

    return 0;
}