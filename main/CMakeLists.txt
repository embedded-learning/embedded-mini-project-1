idf_component_register(SRCS "main.c" "light_sensor_task.c" "relay_control_task.c"
                       PRIV_REQUIRES relay light_sensor)
