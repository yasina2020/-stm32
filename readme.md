## 串口下载程序原理

![image-20240228123821624](readme.assets/image-20240228123821624.png)![image-20240228124138343](readme.assets/image-20240228124138343.png)

程序运行在Flash中，也就是往0x0800 0000中写什么程序，开机后就运行什么程序。如何实现更新呢，会有一个小的辅助程序bootloader存储在0x1FFF F000中，在bootloader模式下，就会启动这个辅助程序，他负责从串口读数据，然后写入到Flash中，再次上电，退出bootloader模式，就会运行新的程序了。

## RTC时钟

### unix时间戳：

![image-20240229231959142](readme.assets/image-20240229231959142.png)

![image-20240229235911459](readme.assets/image-20240229235911459.png)



### BKP：备份寄存器

bkp数据需要vbat引脚接电池

![image-20240301093343935](readme.assets/image-20240301093343935.png)

### RTC：实时时钟

-   可以独立计时，为系统提供时钟和日历功能。
-   RTC也接VBAT
-   32为可编程计数器：秒时间戳
-   20位的可编程与分频器，配置不同频率的输入时钟，将其分频为1hz，保证1s计数器加1
-   可选择三种RTC时钟源：
-   -   HSE/128  8MHz/128
    -   **LES 32.768KHz**
    -   LSI  40KHz

![image-20240301094953400](readme.assets/image-20240301094953400.png)

![image-20240301095011356](readme.assets/image-20240301095011356.png)

