#pragma once

#include <stdlib.h>
#include <stdio.h>
#include "esp_system.h"

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
uint8_t max30102(int argc, char **argv);

