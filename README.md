# STM32F103-Coreboard

STM32测试核心板，设计用于硬件程序调试

### 硬件设计
芯片采用STM32F103C8T6，一个1.3inch OLED屏，六个用户按键，一个有源蜂鸣器，使用TP4056充电的1000mah充电电池，TYPE-C充电/数据端口。引出14个GPIO端口和一组SWD调试端口。

![](/image/altuim-3d-view.png)

![](/image/example-1.jpg)

![](/image/example-2.jpg)

![](/image/example-3.jpg)

PinMap：

| PB11 | PB10 | GND  | 3.3V | PB3  | PB1  | PB0  | PA7  | 3.3V | DIO  | CLK  | GND  |
| :--: | :--: | :--: | :--: | :--: | :--: | :--: | :--: | :--: | :--: | :--: | :--: |
| GND  | 3.3V | PB4  | PB5  | PB6  | PB7  | PB8  | PB9  | PA1  | PA2  | 3.3V | GND  |

TYPE-C端口可程序配置为虚拟串口、USB-HID等，用于数据传输。

使用PA0端口作为内置电池的电压采样输入端口，采样分压为1/2，用于指示电压状态。