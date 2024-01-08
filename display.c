#include <bcm2835.h>
#include <stdio.h>
#include <wchar.h>
#include <stdint.h>
#include <unistd.h>

#include "alphabet.c"

// Пины для управления дисплеем
#define DC_PIN RPI_GPIO_P1_22
#define RST_PIN RPI_GPIO_P1_18
#define BACK_PIN RPI_GPIO_P1_12

// Команды для контроллера ST7735
#define ST7735_NOP     0x00
#define ST7735_SWRESET 0x01
#define ST7735_RDDID   0x04
#define ST7735_RDDST   0x09

#define ST7735_SLPIN   0x10
#define ST7735_SLPOUT  0x11
#define ST7735_PTLON   0x12
#define ST7735_NORON   0x13

#define ST7735_INVOFF  0x20
#define ST7735_INVON   0x21
#define ST7735_DISPOFF 0x28
#define ST7735_DISPON  0x29
#define ST7735_CASET   0x2A
#define ST7735_RASET   0x2B
#define ST7735_RAMWR   0x2C
#define ST7735_RAMRD   0x2E

#define ST7735_COLMOD  0x3A


#define WHITE    0xffff
#define RED      0xf800
#define BLACK    0x0000

static const uint8_t screen_width = 128; // размеры дисплея
static const uint8_t screen_height = 160;

char diff = screen_height / screen_width;

char size_font = 1;

void st7735_write_command(uint8_t cmd) {
    bcm2835_gpio_write(DC_PIN, LOW);
    bcm2835_spi_transfer(cmd);
}

void st7735_write_data(uint8_t data) {
    bcm2835_gpio_write(DC_PIN, HIGH);
    bcm2835_spi_transfer(data);
}

void st7735_set_address_window(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1) {
    st7735_write_command(ST7735_CASET);
    st7735_write_data(ST7735_NOP);
    st7735_write_data(x0);
    st7735_write_data(ST7735_NOP);
    st7735_write_data(x1);

    st7735_write_command(ST7735_RASET);
    st7735_write_data(ST7735_NOP);
    st7735_write_data(y0);
    st7735_write_data(ST7735_NOP);
    st7735_write_data(y1);

    st7735_write_command(ST7735_RAMWR);
}

void st7735_init() {
    bcm2835_init();
    bcm2835_spi_begin();
    bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);
    bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);
    bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_4); // Делитель дисплея
    bcm2835_gpio_fsel(RPI_GPIO_P1_22, BCM2835_GPIO_FSEL_OUTP); // DC
    bcm2835_gpio_fsel(RPI_GPIO_P1_18, BCM2835_GPIO_FSEL_OUTP); // RST
    bcm2835_gpio_fsel(RPI_GPIO_P1_12, BCM2835_GPIO_FSEL_OUTP); // RST

    bcm2835_gpio_write(RST_PIN, HIGH);
    bcm2835_gpio_write(BACK_PIN, HIGH);
    bcm2835_delay(10);
    bcm2835_gpio_write(RST_PIN, LOW);
    bcm2835_delay(10);
    bcm2835_gpio_write(RST_PIN, HIGH);
    bcm2835_delay(10);

    st7735_write_command(ST7735_SWRESET);
    bcm2835_delay(150);

    st7735_write_command(ST7735_SLPOUT);
    bcm2835_delay(500);

    st7735_write_command(ST7735_COLMOD);
    st7735_write_data(0x05);

    st7735_write_command(ST7735_DISPON);
}

void st7735_set_pixel(uint8_t x, uint8_t y, uint16_t color) {
    st7735_set_address_window(x, y , x + 2, y + 2);
    st7735_write_data(color >> 8);
    st7735_write_data(color & 0xFF);
    st7735_write_data(color >> 8);
    st7735_write_data(color & 0xFF);
}

void draw_square(uint8_t pos_x, uint8_t pos_y, uint16_t color, uint8_t square_size) {
    for (uint8_t i = 0; i < square_size; i++) {
        for (uint8_t j = 0; j < square_size; j++) {
            st7735_set_pixel(pos_x + square_size + i, pos_y + j, color);
        }
    }
}

void draw_сhar(uint16_t x, uint16_t y, int16_t character, uint16_t color) {
    int16_t size = 1000;
    
    char charIndex;
    if (character > 1000){
        charIndex = character - 944;
    } else {
        charIndex = character - 32;
    }
    
    if (charIndex < 0 || charIndex >= 140) {
        printf("Нет такого символа\n");
        return;
    }

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 7; j++) {
            if ((font5x7[charIndex][i] >> j) & 0x01) {
                draw_square((x) + i * size_font, (y) + j * size_font, color, size_font);
            }
        }
    }
}

void draw_text(uint16_t x, uint16_t y, const uint32_t *text, uint16_t color) {
    int xOffset = 0;
    while (*text) {
        draw_сhar(x + xOffset, y, *text, color);
        xOffset += 6 * size_font;
        text++;
    }
}

void fill_screen(uint16_t color) {
    st7735_set_address_window(0, 0, screen_width - 1, screen_height - 1);
    for (uint8_t i = 0; i < screen_height; i++) {
        for (uint8_t j = 0; j < screen_width; j++) {
            st7735_write_data(color >> 8);
            st7735_write_data(color & 0xFF);
        }
    }
}

int main() {
    if (!bcm2835_init()) {
        printf("Ошибка инициализации bcm2835.\n");
        return 1;
    }

    st7735_init();

    fill_screen(BLACK);

    char x_direct = 0;
    char y_direct = 0;
    char x_move = 0;
    char y_move = 0;
    const wchar_t* text = L"NIGGA";
    unsigned int text_len = wcslen(text);

    while (1){
        if (x_move + text_len * 6 * size_font >= screen_width){
            x_direct = 1;
        }
        if (x_move == 0){
            x_direct = 0;
        }
        if (y_move + 7 * size_font >= screen_height){
            y_direct = 1;
        }
        if (y_move == 0){
            y_direct = 0;
        }

        if(!x_direct) {
            x_move+=2;
        } else {
            x_move-=2;
        }

        if(!y_direct) {
            y_move+=1;
        } else {
            y_move-=1;
        }

        draw_text(x_move, y_move, text, WHITE);
        bcm2835_delay(80);
        draw_text(x_move, y_move, text, BLACK);

    }
    
    bcm2835_delay(2000);
    bcm2835_spi_end();
    bcm2835_close();

    return 0;
}
