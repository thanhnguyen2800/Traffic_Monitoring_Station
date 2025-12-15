#include "lcd_i2c.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static i2c_master_bus_handle_t i2c_bus;
static i2c_master_dev_handle_t lcd_dev;

static uint8_t backlight = 0x08;

static void lcd_write(uint8_t data) {
    uint8_t buf = data | backlight;
    i2c_master_transmit(lcd_dev, &buf, 1, 100);
    esp_rom_delay_us(50);   // <<< MUST HAVE: prevent corrupted characters
}

static void lcd_pulse_enable(uint8_t data)
{
    lcd_write(data | 0x04);  // E = 1
    esp_rom_delay_us(1);         // >= 450ns
    lcd_write(data & ~0x04); // E = 0
    esp_rom_delay_us(50);        // >= 37us
}

static void lcd_send_cmd(uint8_t cmd)
{
    uint8_t hi = cmd & 0xF0;
    uint8_t lo = (cmd << 4) & 0xF0;

    lcd_pulse_enable(hi);
    lcd_pulse_enable(lo);
}

static void lcd_send_data(uint8_t data)
{
    uint8_t hi = data & 0xF0;
    uint8_t lo = (data << 4) & 0xF0;

    lcd_pulse_enable(hi | 0x01);
    lcd_pulse_enable(lo | 0x01);
}

void lcd_init() {
    i2c_master_bus_config_t bus_cfg = {
        .i2c_port = I2C_NUM_0,
        .sda_io_num = 21,
        .scl_io_num = 22,
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .glitch_ignore_cnt = 7,
        .flags.enable_internal_pullup = true
    };

    i2c_new_master_bus(&bus_cfg, &i2c_bus);

    i2c_device_config_t dev_cfg = {
        .device_address = LCD_ADDR,
        .scl_speed_hz = 100000   // 100kHz = stable
    };

    i2c_master_bus_add_device(i2c_bus, &dev_cfg, &lcd_dev);

    vTaskDelay(pdMS_TO_TICKS(20));

    lcd_send_cmd(0x33);
    lcd_send_cmd(0x32);
    lcd_send_cmd(0x28);
    lcd_send_cmd(0x0C);
    lcd_send_cmd(0x06);
    lcd_send_cmd(0x01);
    vTaskDelay(pdMS_TO_TICKS(3)); // Clear needs long delay
}

void lcd_clear() {
    lcd_send_cmd(0x01);
    vTaskDelay(pdMS_TO_TICKS(3));
}

void lcd_set_cursor(int col, int row)
{
    static uint8_t addr[] = {0x80, 0xC0};
    lcd_send_cmd(addr[row] + col);
}

void lcd_print(const char *str)
{
    while (*str) {
        lcd_send_data(*str++);
    }
}
