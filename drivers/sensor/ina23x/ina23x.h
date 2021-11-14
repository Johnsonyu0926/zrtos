/*
 * Copyright (c) 2021  The Chromium OS Authors
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZEPHYR_DRIVERS_SENSOR_INA23X_H_
#define ZEPHYR_DRIVERS_SENSOR_INA23X_H_

#include <drivers/gpio.h>
#include <drivers/i2c.h>

#ifdef CONFIG_INA23X_VARIANT_230
#define INA23X_REG_CONFIG     0x00
#define INA23X_REG_SHUNT_VOLT 0x01
#define INA23X_REG_BUS_VOLT   0x02
#define INA23X_REG_POWER      0x03
#define INA23X_REG_CURRENT    0x04
#define INA23X_REG_CALIB      0x05
#define INA23X_REG_MASK       0x06
#define INA23X_REG_ALERT      0x07
#else
#define INA23X_REG_CONFIG     0x00
#define INA23X_ADC_CONFIG     0x01
#define INA23X_REG_CALIB      0x02
#define INA23X_REG_SHUNT_VOLT 0x04
#define INA23X_REG_BUS_VOLT   0x05
#define INA23X_REG_MASK       0x06
#define INA23X_REG_CURRENT    0x07
#define INA23X_REG_POWER      0x08
#define INA23X_REG_ALERT      0x0B
#define INA23X_REG_SOVL       0x0C
#define INA23X_REG_SUVL       0x0D
#define INA23X_REG_BOVL       0x0E
#define INA23X_REG_BUVL       0x0F
#define INA23X_REG_TEMP_LIMIT 0x10
#define INA23X_REG_PWR_LIMIT  0x11
#endif

struct ina23x_data {
	const struct device *dev;
	uint16_t current;
	uint16_t bus_voltage;
#ifdef CONFIG_INA23X_VARIANT_230
	uint16_t power;
#else
	uint32_t power;
#endif
#ifdef CONFIG_INA23X_TRIGGER
	const struct device *gpio;
	struct gpio_callback gpio_cb;
	struct k_work work;
	sensor_trigger_handler_t handler_alert;
#endif  /* CONFIG_INA23X_TRIGGER */
};

struct ina23x_config {
	struct i2c_dt_spec i2c;
	uint16_t config;
	uint16_t adc_config;
	uint16_t current_lsb;
	uint16_t rshunt;
#ifdef CONFIG_INA23X_TRIGGER
	bool trig_enabled;
	uint16_t mask;
	const struct gpio_dt_spec gpio_alert;
	uint16_t alert_limit;
#endif  /* CONFIG_INA23X_TRIGGER */
};

int ina23x_trigger_mode_init(const struct device *dev);
int ina23x_trigger_set(const struct device *dev,
		       const struct sensor_trigger *trig,
		       sensor_trigger_handler_t handler);

#endif /* ZEPHYR_DRIVERS_SENSOR_INA23X_H_ */
