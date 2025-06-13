referencias:
- banco de dados sequencia de dna:
https://www.ncbi.nlm.nih.gov/nuccore/NC_100739.1?report=fasta


- Explicações e códigos Z-algorithm:
https://medium.com/@sagar.sahil.nitp98/understanding-and-implementing-the-z-algorithm-for-string-pattern-matching-string-comparison-and-ac2d66fee675
https://www.geeksforgeeks.org/z-algorithm-linear-time-pattern-searching-algorithm/


- KMP
https://www.geeksforgeeks.org/kmp-algorithm-for-pattern-searching/

20:19:56 **** Incremental Build of configuration Debug for project UART ****
make -j4 all 
arm-none-eabi-gcc "../Core/Src/main.c" -mcpu=cortex-m0 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F030x8 -c -I../Core/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F0xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"Core/Src/main.d" -MT"Core/Src/main.o" --specs=nano.specs -mfloat-abi=soft -mthumb -o "Core/Src/main.o"
../Core/Src/main.c: In function 'main':
../Core/Src/main.c:215:36: warning: passing argument 2 of 'HAL_UART_Transmit' makes pointer from integer without a cast [-Wint-conversion]
  215 |         HAL_UART_Transmit(&huart2, *text, 10,1000);
      |                                    ^~~~~
      |                                    |
      |                                    char
In file included from ../Core/Inc/stm32f0xx_hal_conf.h:289,
                 from ../Drivers/STM32F0xx_HAL_Driver/Inc/stm32f0xx_hal.h:29,
                 from ../Core/Inc/main.h:30,
                 from ../Core/Src/main.c:20:
../Drivers/STM32F0xx_HAL_Driver/Inc/stm32f0xx_hal_uart.h:1568:79: note: expected 'const uint8_t *' {aka 'const unsigned char *'} but argument is of type 'char'
 1568 | HAL_StatusTypeDef HAL_UART_Transmit(UART_HandleTypeDef *huart, const uint8_t *pData, uint16_t Size, uint32_t Timeout);
      |                                                                ~~~~~~~~~~~~~~~^~~~~
arm-none-eabi-gcc -o "UART.elf" @"objects.list"   -mcpu=cortex-m0 -T"C:\Users\Alunos\STM32CubeIDE\workspace_1.18.1\UART\STM32F030R8TX_FLASH.ld" --specs=nosys.specs -Wl,-Map="UART.map" -Wl,--gc-sections -static --specs=nano.specs -mfloat-abi=soft -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
Finished building target: UART.elf
 
arm-none-eabi-size  UART.elf 
arm-none-eabi-objdump -h -S UART.elf  > "UART.list"
   text	   data	    bss	    dec	    hex	filename
  13712	    104	   2192	  16008	   3e88	UART.elf
Finished building: default.size.stdout
 
Finished building: UART.list
 

20:19:57 Build Finished. 0 errors, 1 warnings. (took 1s.445ms)


 warning: passing argument 2 of 'HAL_UART_Transmit' makes pointer from integer without a cast [-Wint-conversion]