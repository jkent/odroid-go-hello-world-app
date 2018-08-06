#include <stdint.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_ota_ops.h"
#include "esp_partition.h"
#include "esp_system.h"
#include "driver/gpio.h"

#include "backlight.h"
#include "display.h"
#include "keypad.h"

#include "graphics.h"
#include "tf.h"
#include "OpenSans_Regular_11X12.h"


void app_main(void)
{
    display_init();
    backlight_init();
    keypad_init();

    struct tf_t *tf = tf_new(&font_OpenSans_Regular_11X12, 0, TF_ALIGN_CENTER);

    char s[32];
    strcpy(s, "Hello app world!");
    struct tf_metrics_t m = tf_get_str_metrics(tf, s);
    struct point_t p = {
        .x = DISPLAY_WIDTH/2 - m.width/2,
        .y = DISPLAY_HEIGHT/2 - m.height/2,
    };
    tf_draw_str(fb, tf, s, p);
    display_update();

    uint16_t keys = 0, changes = 0, pressed = 0;
    do {
        vTaskDelay(10 / portTICK_PERIOD_MS);
        keys = keypad_debounce(keypad_sample(), &pressed);
        pressed = keys & changes;
    } while (!(pressed & KEYPAD_MENU));

    const esp_partition_t *part = esp_partition_find_first(
        ESP_PARTITION_TYPE_APP, ESP_PARTITION_SUBTYPE_APP_OTA_0, NULL);
    esp_ota_set_boot_partition(part);
    esp_restart();
}
