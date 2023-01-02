idf_component_register(SRCS "main.c" "led_strip_encoder.c" "mqtt.c" "wifi_connect.c"
                       INCLUDE_DIRS ".")
target_compile_options(${COMPONENT_LIB} PRIVATE "-Wno-format")