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


void vTaskAlarmHandler(void *param)
{
    int run_seconds = 0;
    while (1)
    {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        run_seconds++;
        // 定时每十分钟提醒一次，进行PPG信号采样
        if (run_seconds % 600 == 0)
        {
            printf("Alarm! %d seconds\n", run_seconds);
        }
    }
    
}



// 蓝牙应用
// 初始化蓝牙，并新建一个广播，对打包好的 PPG 信号进行广播




extern "C" void app_main(void)
{
    printf("Hello world!\n");

    // 创建一个定时器，每十分钟提醒一次，进行PPG信号采样
    xTaskCreate(vTaskAlarmHandler, "vTaskAlarmHandler", 2048, NULL, 5, NULL);

    void ble_app_main(void);
    ble_app_main();
    void console_main(void);
    console_main();
    
    while(1) {
        vTaskDelay(10);
    }
}

