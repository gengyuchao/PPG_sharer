/**
 * @file main.cpp
 * @author Geng Yuchao (gengyuchao11@163.com)
 * @brief 
 * @version 0.1
 * @date 2022-06-26
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"


#ifdef __cplusplus
}
#endif


typedef uint8_t (*max30102_func_cb_t)(int argc, char **argv);

extern max30102_func_cb_t p_func_max30102_func;

extern "C" void app_main(void)
{
    printf("Hello world!\n");

    void console_main(void);
    console_main();
    
    while(1) {
        vTaskDelay(10);
    }
}

