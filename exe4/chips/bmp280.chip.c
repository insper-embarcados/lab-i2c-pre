// Wokwi Custom Chip - For information and examples see:
// https://link.wokwi.com/custom-chips-alpha
//
// SPDX-License-Identifier: MIT
// Copyright (C) 2022 Uri Shaked / wokwi.com

#include "wokwi-api.h"
#include <stdio.h>
#include <stdlib.h>
#include "mpu6050.h"

uint8_t mpu6050_write_register(uint8_t reg, uint8_t data);

typedef struct {
    pin_t pin_int;
    uint8_t write_value;
} chip_state_t;

static bool on_i2c_connect(void *user_data, uint32_t address, bool connect);
static uint8_t on_i2c_read(void *user_data);
static bool on_i2c_write(void *user_data, uint8_t data);
static void on_i2c_disconnect(void *user_data);

pin_t pin_test;
uint8_t mpu6050_register[400];
uint8_t mpu6050_connected = 0;
uint8_t mpu6050_access_reg = 0;
uint8_t mpu6050_write_counter = 0;
uint8_t mpu6050_read_counter = 0;

void chip_init() {
    chip_state_t *chip = malloc(sizeof(chip_state_t));

    pin_test = pin_init("TEST", OUTPUT_LOW);
    pin_mode(pin_test, OUTPUT_LOW);

    mpu6050_register[0xD0] = 0x58;

    const i2c_config_t i2c_config = {
        .user_data = chip,
        .address = 0x76,
        .scl = pin_init("SCL", INPUT),
        .sda = pin_init("SDA", INPUT),
        .connect = on_i2c_connect,
        .read = on_i2c_read,
        .write = on_i2c_write,
        .disconnect = on_i2c_disconnect, // Optional
    };
    i2c_init(&i2c_config);

    printf("Hello from custom bmp!\n");
}

uint8_t mpu6050_write_register(uint8_t reg, uint8_t data) {
    mpu6050_register[reg] = data;
    printf("bmmp write reg[0x%X]=%d \n", reg, mpu6050_register[reg]);
    return 0;
}

uint8_t mpu6050_read_register(uint8_t reg) {
    printf("bmp read reg[0x%X]=%d \n", reg, mpu6050_register[reg]);
    return mpu6050_register[reg];
}

bool on_i2c_connect(void *user_data, uint32_t address, bool connect) {
    printf("connected \n");
    mpu6050_connected = true;
    return true; /* Ack */
}

uint8_t on_i2c_read(void *user_data) {
    chip_state_t *chip = user_data;

    if (mpu6050_connected && mpu6050_write_counter == 1)
        return mpu6050_read_register(mpu6050_access_reg + mpu6050_read_counter++);
    else
        return 0;
}

bool on_i2c_write(void *user_data, uint8_t data) {
    chip_state_t *chip = user_data;
    if (mpu6050_write_counter == 0) {
        mpu6050_access_reg = data;
    } else
        mpu6050_write_register(mpu6050_access_reg, data);

    // teste exe 1
    if (mpu6050_access_reg == 0x1C && data == 0x08) {
        pin_mode(pin_test, OUTPUT_HIGH);
        printf("Test ok \n");
    }

    mpu6050_write_counter++;
    return true; // Ack
}

void on_i2c_disconnect(void *user_data) {
    printf("disconnected \n");
    mpu6050_connected = false;
    mpu6050_write_counter = 0;
    mpu6050_read_counter = 0;
    // Do nothing
}
