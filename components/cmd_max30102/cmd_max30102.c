/* Console example — various system commands

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include "esp_log.h"
#include "esp_console.h"
#include "argtable3/argtable3.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sdkconfig.h"
#include "max30102_func.h"

#ifdef CONFIG_FREERTOS_USE_STATS_FORMATTING_FUNCTIONS
#define WITH_TASKS_INFO 1
#endif

static const char *TAG = "max30102";

static void register_cmd_max30102(void);

void register_max30102(void)
{
    register_cmd_max30102();
}

/** 'deep_sleep' command puts the chip into deep sleep mode */

static struct {
    struct arg_str *info;
    struct arg_str *pin;
    struct arg_str *help;
    struct arg_str *test;
    struct arg_str *check;
    struct arg_end *end;
} cmd_max30102_args;

typedef uint8_t (*max30102_func_cb_t)(int argc, char **argv);

static int cmd_max30102(int argc, char **argv)
{
    // printf("argc:%d\n",argc);
    // for(int i = 0 ; i < argc ; i++) {
    //     printf("argv[%d]:%s\n",i,argv[i]);
    // }
    max30102(argc, argv);
    
    return 0;
}

static void register_cmd_max30102(void)
{
    int num_args = 3;
    cmd_max30102_args.info =
        arg_str0("i", "info", "null", "info of max30102");
    cmd_max30102_args.pin =
        arg_str0("p", "pin", "null", "pin of max30102");
    cmd_max30102_args.help =
        arg_str0("h", "help", "null", "help of max30102");
    cmd_max30102_args.test =
        arg_strn("t", "test", "<reg,fifo reg>", 0 , 2, "test of max30102");
    cmd_max30102_args.check =
        arg_strn("c", "check", "<fifo reg>", 0 , 2, "check of max30102");

    cmd_max30102_args.end = arg_end(num_args);

    const esp_console_cmd_t cmd = {
        .command = "max30102",
        .help = "max30102 useful cmd.",
        .hint = NULL,
        .func = &cmd_max30102,
        .argtable = &cmd_max30102_args
    };
    ESP_ERROR_CHECK( esp_console_cmd_register(&cmd) );
}
