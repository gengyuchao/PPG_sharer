
#include <stdlib.h>
#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include "esp_system.h"

#include "driver_max30102_register_test.h"
#include "driver_max30102_fifo_test.h"
#include "driver/gpio.h"
#include "hal/gpio_hal.h"

#define MAX30102_INTR_USE_GPIO_NUM GPIO_NUM_7
#define ESP_INTR_FLAG_DEFAULT 0

/**
 * @brief global var definition
 */
uint8_t (*g_gpio_irq)(void) = NULL;        /**< irq function address */

static QueueHandle_t gpio_evt_queue = NULL;

static void IRAM_ATTR gpio_isr_handler(void* arg)
{
    uint32_t gpio_num = (uint32_t) arg;
    xQueueSendFromISR(gpio_evt_queue, &gpio_num, NULL);
}

static void max30102_intr_task(void* arg)
{
    gpio_num_t io_num;
    for(;;) {
        if(xQueueReceive(gpio_evt_queue, &io_num, portMAX_DELAY)) {
            g_gpio_irq();
            // printf("GPIO[%d] intr, val: %d\n", io_num, gpio_get_level(io_num));
        }
    }
}

/**
 * @brief  gpio interrupt init
 * @return status code
 *         - 0 success
 *         - 1 init failed
 * @note   none
 */
uint8_t gpio_interrupt_init(void)
{
    //zero-initialize the config structure.
    gpio_config_t io_conf = {};
    //disable interrupt
    io_conf.intr_type = GPIO_INTR_NEGEDGE;
    //set as output mode
    io_conf.mode = GPIO_MODE_INPUT;
    //bit mask of the pins that you want to set,e.g.GPIO18/19
    io_conf.pin_bit_mask = 1 << MAX30102_INTR_USE_GPIO_NUM;
    //disable pull-down mode
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    //disable pull-up mode
    io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
    //configure GPIO with the given settings
    gpio_config(&io_conf);

    //create a queue to handle gpio event from isr
    gpio_evt_queue = xQueueCreate(10, sizeof(uint32_t));
    //start gpio task
    xTaskCreate(max30102_intr_task, "max30102_intr_task", 4096, NULL, 10, NULL);

    //install gpio isr service
    gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);
    //hook isr handler for specific gpio pin
    gpio_isr_handler_add(MAX30102_INTR_USE_GPIO_NUM, gpio_isr_handler, (void*) MAX30102_INTR_USE_GPIO_NUM);

    printf("Minimum free heap size: %d bytes\n", esp_get_minimum_free_heap_size());

    return 0;

}

/**
 * @brief  gpio interrupt deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t gpio_interrupt_deinit(void)
{
    //remove isr handler for gpio number.
    gpio_isr_handler_remove(MAX30102_INTR_USE_GPIO_NUM);
    gpio_uninstall_isr_service();
    return 0;
}

/**
 * @brief     max30102 full function
 * @param[in] argc is arg numbers
 * @param[in] **argv is the arg address
 * @return    status code
 *             - 0 success
 *             - 1 run failed
 *             - 5 param is invalid
 * @note      none
 */
uint8_t max30102(int argc, char **argv)
{
    if (argc == 1)
    {
        goto help;
    }
    else if (argc == 2)
    {
        if (strcmp("-i", argv[1]) == 0)
        {
            max30102_info_t info;
            
            /* print max30102 info */
            max30102_info(&info);
            max30102_interface_debug_print("max30102: chip is %s.\n", info.chip_name);
            max30102_interface_debug_print("max30102: manufacturer is %s.\n", info.manufacturer_name);
            max30102_interface_debug_print("max30102: interface is %s.\n", info.interface);
            max30102_interface_debug_print("max30102: driver version is %d.%d.\n", info.driver_version/1000, (info.driver_version%1000)/100);
            max30102_interface_debug_print("max30102: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
            max30102_interface_debug_print("max30102: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
            max30102_interface_debug_print("max30102: max current is %0.2fmA.\n", info.max_current_ma);
            max30102_interface_debug_print("max30102: max temperature is %0.1fC.\n", info.temperature_max);
            max30102_interface_debug_print("max30102: min temperature is %0.1fC.\n", info.temperature_min);
            
            return 0;
        }
        else if (strcmp("-p", argv[1]) == 0)
        {
            /* print pin connection */
            max30102_interface_debug_print("max30102: SCL connected to GPIO3(BCM).\n");
            max30102_interface_debug_print("max30102: SDA connected to GPIO2(BCM).\n");
            max30102_interface_debug_print("max30102: INT connected to GPIO17(BCM).\n");
            
            return 0;
        }
        else if (strcmp("-h", argv[1]) == 0)
        {
            /* show max30102 help */
            
            help:
            
            max30102_interface_debug_print("max30102 -i\n\tshow max30102 chip and driver information.\n");
            max30102_interface_debug_print("max30102 -h\n\tshow max30102 help.\n");
            max30102_interface_debug_print("max30102 -p\n\tshow max30102 pin connections of the current board.\n");
            max30102_interface_debug_print("max30102 -t reg\n\trun max30102 register test.\n");
            max30102_interface_debug_print("max30102 -t fifo <times>\n\trun max30102 fifo test.times means test times.\n");
            max30102_interface_debug_print("max30102 -r fifo <times>\n\trun max30102 fifo mytest.times means read times.\n");
            
            return 0;
        }
        else
        {
            return 5;
        }
    }
    else if (argc == 3)
    {
        /* run test */
        if (strcmp("-t", argv[1]) == 0)
        {
            /* reg test */
            if (strcmp("reg", argv[2]) == 0)
            {
                uint8_t res;
                
                res = max30102_register_test();
                if (res != 0)
                {
                    return 1;
                }
                else
                {
                    return 0;
                }
            }
            /* param is invalid */
            else
            {
                return 5;
            }
        }
        /* param is invalid */
        else
        {
            return 5;
        }
    }
    else if (argc == 4)
    {
        /* run test */
        if (strcmp("-t", argv[1]) == 0)
        {
            /* reg test */
            if (strcmp("fifo", argv[2]) == 0)
            {
                uint8_t res;
                
                /* set gpio */
                g_gpio_irq = max30102_fifo_test_irq_handler;
                res = gpio_interrupt_init();
                if (res != 0)
                {
                    g_gpio_irq = NULL;
                    
                    return 1;
                }
                
                res = max30102_fifo_test(atoi(argv[3]));
                if (res != 0)
                {
                    (void)gpio_interrupt_deinit();
                    g_gpio_irq = NULL;
                    
                    return 1;
                }
                else
                {
                    (void)gpio_interrupt_deinit();
                    g_gpio_irq = NULL;
                    
                    return 0;
                }
            }
            /* param is invalid */
            else
            {
                return 5;
            }
        }
        if (strcmp("-r", argv[1]) == 0)
        {
            /* reg test */
            if (strcmp("fifo", argv[2]) == 0)
            {
                uint8_t res;
                
                /* set gpio */
                g_gpio_irq = max30102_fifo_test_irq_handler;
                res = gpio_interrupt_init();
                if (res != 0)
                {
                    g_gpio_irq = NULL;
                    
                    return 1;
                }
                
                res = max30102_get_data_test(atoi(argv[3]));
                if (res != 0)
                {
                    (void)gpio_interrupt_deinit();
                    g_gpio_irq = NULL;
                    
                    return 1;
                }
                else
                {
                    (void)gpio_interrupt_deinit();
                    g_gpio_irq = NULL;
                    
                    return 0;
                }
            }
            /* param is invalid */
            else
            {
                return 5;
            }
        }
        /* param is invalid */
        else
        {
            return 5;
        }
    }
    /* param is invalid */
    else
    {
        return 5;
    }
}

