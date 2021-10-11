#ifndef PIN_DEFINITION_H
#define PIN_DEFINITION_H

#define AM335_GPIO(bank,line)  (32 * bank + line)

#define HCSR04_INPUT    AM335_GPIO(1, 15)       // PIN 47
#define HCSR04_OUTPUT   AM335_GPIO(0, 27)       // PIN 27

#define GPIO_OUT_0      AM335_GPIO(0, 0)       // PIN ?
#define GPIO_OUT_1      AM335_GPIO(0, 0)       // PIN ?
#define GPIO_OUT_2      AM335_GPIO(0, 0)       // PIN ?
#define GPIO_OUT_3      AM335_GPIO(0, 0)       // PIN ?

#endif /* PIN_DEFINITION_H */
