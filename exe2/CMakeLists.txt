add_executable(exe1
        main.c
)

target_link_libraries(exe1 pico_stdlib hardware_i2c freertos)
pico_add_extra_outputs(exe1)
