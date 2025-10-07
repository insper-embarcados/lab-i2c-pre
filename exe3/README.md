# exe3

O I2C permite conectar dois periféricos no mesmo barramento. No exemplo, conectamos o BMP280, um sensor de temperatura e pressão da Bosch. Você deve acessar o registrador ID e imprimir o valor lido na serial:

``` c
printf("BMP280 ID: 0x%X \n", buffer[0]);
```

Consulte o [manual do BMP280](https://cdn-shop.adafruit.com/datasheets/BST-BMP280-DS001-11.pdf) e extraia a informação do endereço padrão desse sensor e do endereço do registrador que possui o ID.
