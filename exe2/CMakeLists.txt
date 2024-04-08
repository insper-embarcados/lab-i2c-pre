add_executable(exe2
        main.c
)

target_link_libraries(exe2 pico_stdlib hardware_i2c freertos)
pico_add_extra_outputs(exe2)
