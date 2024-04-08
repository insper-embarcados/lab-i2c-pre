docker run -v $(pwd):$(pwd) -w $(pwd) rafaelcorsi/wokwi-custom-ci:latest sh -c "clang --target=wasm32-unknown-wasi --sysroot /opt/wasi-libc -nostartfiles -Wl,--import-memory -Wl,--export-table -Wl,--no-entry -Werror -o mpu6050.chip.wasm mpu6050.chip.c"
docker run -v $(pwd):$(pwd) -w $(pwd) rafaelcorsi/wokwi-custom-ci:latest sh -c "clang --target=wasm32-unknown-wasi --sysroot /opt/wasi-libc -nostartfiles -Wl,--import-memory -Wl,--export-table -Wl,--no-entry -Werror -o bmp280.chip.wasm bmp280.chip.c"