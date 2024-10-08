/*
 * Copyright (c) 2024, Andrew Featherstone
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef __RP2350B_PINCTRL_H__
#define __RP2350B_PINCTRL_H__

#include "rpi-pico-rp2350-pinctrl-common.h"

/* RP2350B is in a QFN-80 package, and extends the set of available pins
 * accordingly.
 */
#define SPI1_SCK_P30 RP2XXX_PINMUX(30, RP2_PINCTRL_GPIO_FUNC_SPI)
#define SPI1_TX_P31  RP2XXX_PINMUX(31, RP2_PINCTRL_GPIO_FUNC_SPI)
#define SPI0_RX_P32  RP2XXX_PINMUX(32, RP2_PINCTRL_GPIO_FUNC_SPI)
#define SPI0_CSN_P33 RP2XXX_PINMUX(33, RP2_PINCTRL_GPIO_FUNC_SPI)
#define SPI0_SCK_P34 RP2XXX_PINMUX(34, RP2_PINCTRL_GPIO_FUNC_SPI)
#define SPI0_TX_P35  RP2XXX_PINMUX(35, RP2_PINCTRL_GPIO_FUNC_SPI)
#define SPI0_RX_P36  RP2XXX_PINMUX(36, RP2_PINCTRL_GPIO_FUNC_SPI)
#define SPI0_CSN_P37 RP2XXX_PINMUX(37, RP2_PINCTRL_GPIO_FUNC_SPI)
#define SPI0_SCK_P38 RP2XXX_PINMUX(38, RP2_PINCTRL_GPIO_FUNC_SPI)
#define SPI0_TX_P39  RP2XXX_PINMUX(39, RP2_PINCTRL_GPIO_FUNC_SPI)
#define SPI1_RX_P40  RP2XXX_PINMUX(40, RP2_PINCTRL_GPIO_FUNC_SPI)
#define SPI1_CSN_P41 RP2XXX_PINMUX(41, RP2_PINCTRL_GPIO_FUNC_SPI)
#define SPI1_SCK_P42 RP2XXX_PINMUX(42, RP2_PINCTRL_GPIO_FUNC_SPI)
#define SPI1_TX_P43  RP2XXX_PINMUX(43, RP2_PINCTRL_GPIO_FUNC_SPI)
#define SPI1_RX_P44  RP2XXX_PINMUX(44, RP2_PINCTRL_GPIO_FUNC_SPI)
#define SPI1_CSN_P45 RP2XXX_PINMUX(45, RP2_PINCTRL_GPIO_FUNC_SPI)
#define SPI1_SCK_P46 RP2XXX_PINMUX(46, RP2_PINCTRL_GPIO_FUNC_SPI)
#define SPI1_TX_P47  RP2XXX_PINMUX(47, RP2_PINCTRL_GPIO_FUNC_SPI)

#define UART1_TX_P30 RP2XXX_PINMUX(30, RP2_PINCTRL_GPIO_FUNC_UART)
#define UART1_RX_P31 RP2XXX_PINMUX(31, RP2_PINCTRL_GPIO_FUNC_UART)
#define UART0_TX_P32 RP2XXX_PINMUX(32, RP2_PINCTRL_GPIO_FUNC_UART)
#define UART0_RX_P33 RP2XXX_PINMUX(33, RP2_PINCTRL_GPIO_FUNC_UART)
#define UART1_TX_P34 RP2XXX_PINMUX(34, RP2_PINCTRL_GPIO_FUNC_UART)
#define UART1_RX_P35 RP2XXX_PINMUX(35, RP2_PINCTRL_GPIO_FUNC_UART)
#define UART0_TX_P36 RP2XXX_PINMUX(36, RP2_PINCTRL_GPIO_FUNC_UART)
#define UART0_RX_P37 RP2XXX_PINMUX(37, RP2_PINCTRL_GPIO_FUNC_UART)
#define UART1_TX_P38 RP2XXX_PINMUX(38, RP2_PINCTRL_GPIO_FUNC_UART)
#define UART1_RX_P39 RP2XXX_PINMUX(39, RP2_PINCTRL_GPIO_FUNC_UART)
#define UART0_TX_P40 RP2XXX_PINMUX(40, RP2_PINCTRL_GPIO_FUNC_UART)
#define UART0_RX_P41 RP2XXX_PINMUX(41, RP2_PINCTRL_GPIO_FUNC_UART)
#define UART1_TX_P42 RP2XXX_PINMUX(42, RP2_PINCTRL_GPIO_FUNC_UART)
#define UART1_RX_P43 RP2XXX_PINMUX(43, RP2_PINCTRL_GPIO_FUNC_UART)
#define UART0_TX_P44 RP2XXX_PINMUX(44, RP2_PINCTRL_GPIO_FUNC_UART)
#define UART0_RX_P45 RP2XXX_PINMUX(45, RP2_PINCTRL_GPIO_FUNC_UART)
#define UART1_TX_P46 RP2XXX_PINMUX(46, RP2_PINCTRL_GPIO_FUNC_UART)
#define UART1_RX_P47 RP2XXX_PINMUX(47, RP2_PINCTRL_GPIO_FUNC_UART)

#define I2C1_SDA_P30 RP2XXX_PINMUX(30, RP2_PINCTRL_GPIO_FUNC_I2C)
#define I2C1_SCL_P31 RP2XXX_PINMUX(31, RP2_PINCTRL_GPIO_FUNC_I2C)
#define I2C0_SDA_P32 RP2XXX_PINMUX(32, RP2_PINCTRL_GPIO_FUNC_I2C)
#define I2C0_SCL_P33 RP2XXX_PINMUX(33, RP2_PINCTRL_GPIO_FUNC_I2C)
#define I2C1_SDA_P34 RP2XXX_PINMUX(34, RP2_PINCTRL_GPIO_FUNC_I2C)
#define I2C1_SCL_P35 RP2XXX_PINMUX(35, RP2_PINCTRL_GPIO_FUNC_I2C)
#define I2C0_SDA_P36 RP2XXX_PINMUX(36, RP2_PINCTRL_GPIO_FUNC_I2C)
#define I2C0_SCL_P37 RP2XXX_PINMUX(37, RP2_PINCTRL_GPIO_FUNC_I2C)
#define I2C1_SDA_P38 RP2XXX_PINMUX(38, RP2_PINCTRL_GPIO_FUNC_I2C)
#define I2C1_SCL_P39 RP2XXX_PINMUX(39, RP2_PINCTRL_GPIO_FUNC_I2C)
#define I2C0_SDA_P40 RP2XXX_PINMUX(40, RP2_PINCTRL_GPIO_FUNC_I2C)
#define I2C0_SCL_P41 RP2XXX_PINMUX(41, RP2_PINCTRL_GPIO_FUNC_I2C)
#define I2C1_SDA_P42 RP2XXX_PINMUX(42, RP2_PINCTRL_GPIO_FUNC_I2C)
#define I2C1_SCL_P43 RP2XXX_PINMUX(43, RP2_PINCTRL_GPIO_FUNC_I2C)
#define I2C0_SDA_P44 RP2XXX_PINMUX(44, RP2_PINCTRL_GPIO_FUNC_I2C)
#define I2C0_SCL_P45 RP2XXX_PINMUX(45, RP2_PINCTRL_GPIO_FUNC_I2C)
#define I2C1_SDA_P46 RP2XXX_PINMUX(46, RP2_PINCTRL_GPIO_FUNC_I2C)
#define I2C1_SCL_P47 RP2XXX_PINMUX(47, RP2_PINCTRL_GPIO_FUNC_I2C)

#define PWM_7A_P30  RP2XXX_PINMUX(30, RP2_PINCTRL_GPIO_FUNC_PWM)
#define PWM_7B_P31  RP2XXX_PINMUX(31, RP2_PINCTRL_GPIO_FUNC_PWM)
#define PWM_8A_P32  RP2XXX_PINMUX(32, RP2_PINCTRL_GPIO_FUNC_PWM)
#define PWM_8B_P33  RP2XXX_PINMUX(33, RP2_PINCTRL_GPIO_FUNC_PWM)
#define PWM_9A_P34  RP2XXX_PINMUX(34, RP2_PINCTRL_GPIO_FUNC_PWM)
#define PWM_9B_P35  RP2XXX_PINMUX(35, RP2_PINCTRL_GPIO_FUNC_PWM)
#define PWM_10A_P36 RP2XXX_PINMUX(36, RP2_PINCTRL_GPIO_FUNC_PWM)
#define PWM_10B_P37 RP2XXX_PINMUX(37, RP2_PINCTRL_GPIO_FUNC_PWM)
#define PWM_11A_P38 RP2XXX_PINMUX(38, RP2_PINCTRL_GPIO_FUNC_PWM)
#define PWM_11B_P39 RP2XXX_PINMUX(39, RP2_PINCTRL_GPIO_FUNC_PWM)
#define PWM_12A_P40 RP2XXX_PINMUX(40, RP2_PINCTRL_GPIO_FUNC_PWM)
#define PWM_12B_P41 RP2XXX_PINMUX(41, RP2_PINCTRL_GPIO_FUNC_PWM)
#define PWM_13A_P42 RP2XXX_PINMUX(42, RP2_PINCTRL_GPIO_FUNC_PWM)
#define PWM_13B_P43 RP2XXX_PINMUX(43, RP2_PINCTRL_GPIO_FUNC_PWM)
#define PWM_14A_P44 RP2XXX_PINMUX(44, RP2_PINCTRL_GPIO_FUNC_PWM)
#define PWM_14B_P45 RP2XXX_PINMUX(45, RP2_PINCTRL_GPIO_FUNC_PWM)
#define PWM_15A_P46 RP2XXX_PINMUX(46, RP2_PINCTRL_GPIO_FUNC_PWM)
#define PWM_15B_P47 RP2XXX_PINMUX(47, RP2_PINCTRL_GPIO_FUNC_PWM)

#define PIO0_P30 RP2XXX_PINMUX(30, RP2_PINCTRL_GPIO_FUNC_PIO0)
#define PIO0_P31 RP2XXX_PINMUX(31, RP2_PINCTRL_GPIO_FUNC_PIO0)
#define PIO0_P32 RP2XXX_PINMUX(32, RP2_PINCTRL_GPIO_FUNC_PIO0)
#define PIO0_P33 RP2XXX_PINMUX(33, RP2_PINCTRL_GPIO_FUNC_PIO0)
#define PIO0_P34 RP2XXX_PINMUX(34, RP2_PINCTRL_GPIO_FUNC_PIO0)
#define PIO0_P35 RP2XXX_PINMUX(35, RP2_PINCTRL_GPIO_FUNC_PIO0)
#define PIO0_P36 RP2XXX_PINMUX(36, RP2_PINCTRL_GPIO_FUNC_PIO0)
#define PIO0_P37 RP2XXX_PINMUX(37, RP2_PINCTRL_GPIO_FUNC_PIO0)
#define PIO0_P38 RP2XXX_PINMUX(38, RP2_PINCTRL_GPIO_FUNC_PIO0)
#define PIO0_P39 RP2XXX_PINMUX(39, RP2_PINCTRL_GPIO_FUNC_PIO0)
#define PIO0_P40 RP2XXX_PINMUX(40, RP2_PINCTRL_GPIO_FUNC_PIO0)
#define PIO0_P41 RP2XXX_PINMUX(41, RP2_PINCTRL_GPIO_FUNC_PIO0)
#define PIO0_P42 RP2XXX_PINMUX(42, RP2_PINCTRL_GPIO_FUNC_PIO0)
#define PIO0_P43 RP2XXX_PINMUX(43, RP2_PINCTRL_GPIO_FUNC_PIO0)
#define PIO0_P44 RP2XXX_PINMUX(44, RP2_PINCTRL_GPIO_FUNC_PIO0)
#define PIO0_P45 RP2XXX_PINMUX(45, RP2_PINCTRL_GPIO_FUNC_PIO0)
#define PIO0_P46 RP2XXX_PINMUX(46, RP2_PINCTRL_GPIO_FUNC_PIO0)
#define PIO0_P47 RP2XXX_PINMUX(47, RP2_PINCTRL_GPIO_FUNC_PIO0)

#define PIO1_P30 RP2XXX_PINMUX(30, RP2_PINCTRL_GPIO_FUNC_PIO1)
#define PIO1_P31 RP2XXX_PINMUX(31, RP2_PINCTRL_GPIO_FUNC_PIO1)
#define PIO1_P32 RP2XXX_PINMUX(32, RP2_PINCTRL_GPIO_FUNC_PIO1)
#define PIO1_P33 RP2XXX_PINMUX(33, RP2_PINCTRL_GPIO_FUNC_PIO1)
#define PIO1_P34 RP2XXX_PINMUX(34, RP2_PINCTRL_GPIO_FUNC_PIO1)
#define PIO1_P35 RP2XXX_PINMUX(35, RP2_PINCTRL_GPIO_FUNC_PIO1)
#define PIO1_P36 RP2XXX_PINMUX(36, RP2_PINCTRL_GPIO_FUNC_PIO1)
#define PIO1_P37 RP2XXX_PINMUX(37, RP2_PINCTRL_GPIO_FUNC_PIO1)
#define PIO1_P38 RP2XXX_PINMUX(38, RP2_PINCTRL_GPIO_FUNC_PIO1)
#define PIO1_P39 RP2XXX_PINMUX(39, RP2_PINCTRL_GPIO_FUNC_PIO1)
#define PIO1_P40 RP2XXX_PINMUX(40, RP2_PINCTRL_GPIO_FUNC_PIO1)
#define PIO1_P41 RP2XXX_PINMUX(41, RP2_PINCTRL_GPIO_FUNC_PIO1)
#define PIO1_P42 RP2XXX_PINMUX(42, RP2_PINCTRL_GPIO_FUNC_PIO1)
#define PIO1_P43 RP2XXX_PINMUX(43, RP2_PINCTRL_GPIO_FUNC_PIO1)
#define PIO1_P44 RP2XXX_PINMUX(44, RP2_PINCTRL_GPIO_FUNC_PIO1)
#define PIO1_P45 RP2XXX_PINMUX(45, RP2_PINCTRL_GPIO_FUNC_PIO1)
#define PIO1_P46 RP2XXX_PINMUX(46, RP2_PINCTRL_GPIO_FUNC_PIO1)
#define PIO1_P47 RP2XXX_PINMUX(47, RP2_PINCTRL_GPIO_FUNC_PIO1)

#define PIO2_P30 RP2XXX_PINMUX(30, RP2_PINCTRL_GPIO_FUNC_PIO2)
#define PIO2_P31 RP2XXX_PINMUX(31, RP2_PINCTRL_GPIO_FUNC_PIO2)
#define PIO2_P32 RP2XXX_PINMUX(32, RP2_PINCTRL_GPIO_FUNC_PIO2)
#define PIO2_P33 RP2XXX_PINMUX(33, RP2_PINCTRL_GPIO_FUNC_PIO2)
#define PIO2_P34 RP2XXX_PINMUX(34, RP2_PINCTRL_GPIO_FUNC_PIO2)
#define PIO2_P35 RP2XXX_PINMUX(35, RP2_PINCTRL_GPIO_FUNC_PIO2)
#define PIO2_P36 RP2XXX_PINMUX(36, RP2_PINCTRL_GPIO_FUNC_PIO2)
#define PIO2_P37 RP2XXX_PINMUX(37, RP2_PINCTRL_GPIO_FUNC_PIO2)
#define PIO2_P38 RP2XXX_PINMUX(38, RP2_PINCTRL_GPIO_FUNC_PIO2)
#define PIO2_P39 RP2XXX_PINMUX(39, RP2_PINCTRL_GPIO_FUNC_PIO2)
#define PIO2_P40 RP2XXX_PINMUX(40, RP2_PINCTRL_GPIO_FUNC_PIO2)
#define PIO2_P41 RP2XXX_PINMUX(41, RP2_PINCTRL_GPIO_FUNC_PIO2)
#define PIO2_P42 RP2XXX_PINMUX(42, RP2_PINCTRL_GPIO_FUNC_PIO2)
#define PIO2_P43 RP2XXX_PINMUX(43, RP2_PINCTRL_GPIO_FUNC_PIO2)
#define PIO2_P44 RP2XXX_PINMUX(44, RP2_PINCTRL_GPIO_FUNC_PIO2)
#define PIO2_P45 RP2XXX_PINMUX(45, RP2_PINCTRL_GPIO_FUNC_PIO2)
#define PIO2_P46 RP2XXX_PINMUX(46, RP2_PINCTRL_GPIO_FUNC_PIO2)
#define PIO2_P47 RP2XXX_PINMUX(47, RP2_PINCTRL_GPIO_FUNC_PIO2)

#define GPIN0_P12  RP2XXX_PINMUX(12, RP2_PINCTRL_GPIO_FUNC_GPCK)
#define GPIN1_P14  RP2XXX_PINMUX(14, RP2_PINCTRL_GPIO_FUNC_GPCK)
#define GPIN0_P20  RP2XXX_PINMUX(20, RP2_PINCTRL_GPIO_FUNC_GPCK)
#define GPIN1_P22  RP2XXX_PINMUX(22, RP2_PINCTRL_GPIO_FUNC_GPCK)
#define GPOUT0_P13 RP2XXX_PINMUX(13, RP2_PINCTRL_GPIO_FUNC_GPCK)
#define GPOUT1_P15 RP2XXX_PINMUX(15, RP2_PINCTRL_GPIO_FUNC_GPCK)
#define GPOUT0_P21 RP2XXX_PINMUX(21, RP2_PINCTRL_GPIO_FUNC_GPCK)
#define GPOUT1_P23 RP2XXX_PINMUX(23, RP2_PINCTRL_GPIO_FUNC_GPCK)
#define GPOUT2_P24 RP2XXX_PINMUX(24, RP2_PINCTRL_GPIO_FUNC_GPCK)
#define GPOUT3_P25 RP2XXX_PINMUX(25, RP2_PINCTRL_GPIO_FUNC_GPCK)

#define UART0_TX_P30 RP2XXX_PINMUX(30, RP2_PINCTRL_GPIO_FUNC_UART_ALT)
#define UART0_RX_P31 RP2XXX_PINMUX(31, RP2_PINCTRL_GPIO_FUNC_UART_ALT)
#define UART0_TX_P34 RP2XXX_PINMUX(34, RP2_PINCTRL_GPIO_FUNC_UART_ALT)
#define UART0_RX_P35 RP2XXX_PINMUX(35, RP2_PINCTRL_GPIO_FUNC_UART_ALT)
#define UART1_TX_P38 RP2XXX_PINMUX(38, RP2_PINCTRL_GPIO_FUNC_UART_ALT)
#define UART1_RX_P39 RP2XXX_PINMUX(39, RP2_PINCTRL_GPIO_FUNC_UART_ALT)
#define UART1_TX_P42 RP2XXX_PINMUX(42, RP2_PINCTRL_GPIO_FUNC_UART_ALT)
#define UART1_RX_P43 RP2XXX_PINMUX(43, RP2_PINCTRL_GPIO_FUNC_UART_ALT)
#define UART0_TX_P46 RP2XXX_PINMUX(46, RP2_PINCTRL_GPIO_FUNC_UART_ALT)
#define UART0_RX_P47 RP2XXX_PINMUX(47, RP2_PINCTRL_GPIO_FUNC_UART_ALT)

/* ADC channel allocations differ between the RP2350A and RP2350B.
 * Refer to Table 1116 in the datasheet.
 */
#define ADC_CH0_P40 RP2XXX_PINMUX(40, RP2_PINCTRL_GPIO_FUNC_NULL)
#define ADC_CH1_P41 RP2XXX_PINMUX(41, RP2_PINCTRL_GPIO_FUNC_NULL)
#define ADC_CH2_P42 RP2XXX_PINMUX(42, RP2_PINCTRL_GPIO_FUNC_NULL)
#define ADC_CH3_P43 RP2XXX_PINMUX(43, RP2_PINCTRL_GPIO_FUNC_NULL)
#define ADC_CH4_P44 RP2XXX_PINMUX(44, RP2_PINCTRL_GPIO_FUNC_NULL)
#define ADC_CH5_P45 RP2XXX_PINMUX(45, RP2_PINCTRL_GPIO_FUNC_NULL)
#define ADC_CH6_P46 RP2XXX_PINMUX(46, RP2_PINCTRL_GPIO_FUNC_NULL)
#define ADC_CH7_P47 RP2XXX_PINMUX(47, RP2_PINCTRL_GPIO_FUNC_NULL)

#endif /* __RP2350B_PINCTRL_H__ */
