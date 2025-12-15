idf_component_register(
    SRCS "main.c" "lcd_i2c.c"
    INCLUDE_DIRS "."
    REQUIRES driver
    PRIV_REQUIRES esp_driver_gpio esp_timer
)
