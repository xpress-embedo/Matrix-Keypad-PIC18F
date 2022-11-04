# Matrix Keypad Interfacing with PIC18F4550
[Blog Link](http://embeddedlaboratory.blogspot.com/2016/08/matrix-keypad-interfacing-with.html)  
In this post i will show, how to interface Matrix Keypad and 16x2 LCD with one of the most popular Microchip PIC18F4550 micro-controller.  
I am using MPLABX Integrated Develoment Environment along with MPLAB XC8 Compiler for compiling the code.  
The beauty of this post is that, i will use the old existing keypad and 16x2 lcd library written for Cortex-M3 micro-controller, in this post.  

The following image shows the connection of PIC micro-controller with 16x2 LCD and Matrix Keypad.  

![Schematic Diagram](https://4.bp.blogspot.com/-_RPji6y9UBc/V6bmYHMZX9I/AAAAAAAAAB8/4ZqsczYHxY01j7eRlqzEhzwVzxU2lGo2gCLcB/s1600/matrix%2Bkeypad.png)  

The LCD and Matrix Keypad Library are written in generic format and can be ported for any other micro-controller.  
LCD Library is handled to minimize the update time by checking the busy flag, but one can use the delay feature as well. To use delay feature comment the following line in the `lcd_16x2.h` header file.  

```C
#define USE_LCD_BUSY_FLAG             /**< Use Busy Bit instead of Delay.*/
```
Coming to the keypad part, the specialty of the keypad library is that, it works for pins which are not connected on single port and not even in sequence. Apart from this one very good feature of the this library is that, on pressing and holding a particular key for 2 seconds, the repeater mode is activated and key values are returned every 100 milli-second. Have a look at the following video, to know what this repeater mode is and how it can be useful for you.  
![Key-1 Pressed Simulation](https://4.bp.blogspot.com/-izrha86YFXE/V6bpPLwT3kI/AAAAAAAAACI/2MRcZHEijZIjwo0636LzVGpfijblK1mZgCLcB/s1600/Matrix%2BKeypad%2BSimulation.png)  

## Project Description
Keypad has 16 keys, whenever a key is pressed its counter is increments by 1 and key-press with counter value is displayed on 16x2 LCD. Keypad Hold feature is added in algorithm which enables the repeat mode, which will increment counter speedily, when pressing a key for more than 2 seconds.
