#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/ledc.h"
#include "esp_timer.h"
#include "lcd_i2c.h"

#define IR1_GPIO 34
#define IR2_GPIO 35
#define BUZZER   13

int flag1 = 0, flag2 = 0;
int64_t timer1 = 0, timer2 = 0;

float distance = 9; //9 cm -> 9m (coi như)
float speed = 0;

void buzzer_beep(int duration_ms)
{
    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 512);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
    vTaskDelay(pdMS_TO_TICKS(duration_ms));
    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 0);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
}

void buzzer_init()
{
    ledc_timer_config_t timer = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .timer_num = LEDC_TIMER_0,
        .duty_resolution = LEDC_TIMER_10_BIT,
        .freq_hz = 2000,
        .clk_cfg = LEDC_AUTO_CLK
    };
    ledc_timer_config(&timer);

    ledc_channel_config_t channel = {
        .gpio_num = BUZZER,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = LEDC_CHANNEL_0,
        .timer_sel = LEDC_TIMER_0,
        .duty = 0,
        .hpoint = 0
    };
    ledc_channel_config(&channel);
}


void app_main(void)
{
    lcd_init();
    lcd_clear();

    lcd_set_cursor(0,0);
    lcd_print(" Welcome To ");
    lcd_set_cursor(0,1);
    lcd_print(" our project ");
    vTaskDelay(2000 / portTICK_PERIOD_MS);
    lcd_clear();

    gpio_set_direction(IR1_GPIO, GPIO_MODE_INPUT);
    gpio_set_direction(IR2_GPIO, GPIO_MODE_INPUT);
    gpio_set_direction(BUZZER, GPIO_MODE_OUTPUT);
    buzzer_init();
    gpio_set_drive_capability(BUZZER, GPIO_DRIVE_CAP_0);

    while (1)
    {
        if (gpio_get_level(IR1_GPIO) == 0 && flag1 == 0) {
            timer1 = esp_timer_get_time(); // microseconds
            flag1 = 1;
        }

        if (gpio_get_level(IR2_GPIO) == 0 && flag2 == 0) {
            timer2 = esp_timer_get_time();
            flag2 = 1;
        }

        if (flag1 == 1 && flag2 == 1)
        {
            int64_t dt = llabs(timer2 - timer1);
            float Time = dt / 1e6; // convert to seconds

            speed = (distance / Time) * 3.6; // m/s → km/h
        }

        if (speed == 0)
        {
            lcd_set_cursor(0,1);
            if(flag1==0 && flag2==0) lcd_print("No car detected ");
            else lcd_print("Searching...    ");
        }
        else 
        {
            lcd_clear();
            lcd_set_cursor(0,0);
            char buf[20];
            sprintf(buf, "Speed: %.1fKm/h", speed);
            lcd_print(buf);

            lcd_set_cursor(0,1);
            if (speed > 50) {
                lcd_print(" Over Speeding ");
                buzzer_beep(500);
            } else {
                lcd_print(" Normal Speed  ");
            }

            vTaskDelay(3000 / portTICK_PERIOD_MS);

            speed = 0;
            flag1 = 0;
            flag2 = 0;
        }

        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}
