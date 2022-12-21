/*
 * Copyright (c) 2022 Georgij Cernysiov
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZEPHYR_INCLUDE_DT_BINDINGS_TIMER_SMARTBOND_TIMER_H_
#define ZEPHYR_INCLUDE_DT_BINDINGS_TIMER_SMARTBOND_TIMER_H_

/* Timer countermode values */
#define SMARTBOND_TIM_COUNTERMODE_UP             0x00000000U
#define SMARTBOND_TIM_COUNTERMODE_DOWN           0x00000010U
#define SMARTBOND_TIM_COUNTERMODE_CENTER_DOWN    0x00000020U
#define SMARTBOND_TIM_COUNTERMODE_CENTER_UP      0x00000040U
#define SMARTBOND_TIM_COUNTERMODE_CENTER_UP_DOWN 0x00000060U

#endif /* ZEPHYR_INCLUDE_DT_BINDINGS_TIMER_SMARTBOND_TIMER_H_ */
