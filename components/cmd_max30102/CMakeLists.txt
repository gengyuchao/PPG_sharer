idf_component_register(SRCS "cmd_max30102.c"
                            "functions/max30102_func.c"
                    INCLUDE_DIRS . functions
                    REQUIRES console max30102 driver)
