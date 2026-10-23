# exe 2

O código fornecido faz a leitura do registrador 0x75:WHOAMI, que é um registrador contendo uma informação que confirma o ID do chip. Você deve ler o valor do INT_ENABLE e imprimir na serial usando o seguinte printf:

``` c
printf("INT_ENABLE: 0x%X \n", buffer[0]);
```
