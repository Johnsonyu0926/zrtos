/*
 * Copyright (c) 2024 Laczen
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @file
 * @brief Public API for storage area eeprom subsystem
 */

#ifndef ZEPHYR_INCLUDE_STORAGE_STORAGE_AREA_EEPROM_H_
#define ZEPHYR_INCLUDE_STORAGE_STORAGE_AREA_EEPROM_H_

#include <zephyr/drivers/eeprom.h>
#include <zephyr/storage/storage_area/storage_area.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Storage_area_eeprom interface
 * @defgroup storage_area_eeprom_interface Storage_area_eeprom interface
 * @ingroup storage_apis
 * @{
 */

struct storage_area_eeprom {
	const struct storage_area area;
	const struct device *dev;
	const off_t doffset;
};

extern const struct storage_area_api storage_area_eeprom_api;

#define STORAGE_AREA_EEPROM(_dev, _doffset, _ws, _es, _size, _props)            \
	{                                                                       \
		.area =                                                         \
			{                                                       \
				.api = ((_ws == 0) ||                           \
					((_ws & (_ws - 1)) != 0) ||             \
					((_es % _ws) != 0) ||                   \
					((_size % _es) != 0))                   \
					       ? NULL                           \
					       : &storage_area_eeprom_api,      \
				.write_size = _ws,                              \
				.erase_size = _es,                              \
				.erase_blocks = _size / _es,                    \
				.props = _props | STORAGE_AREA_PROP_FOVRWRITE,	\
			},                                                      \
		.dev = _dev, .doffset = _doffset,                               \
	}

#define STORAGE_AREA_EEPROM_DEFINE(_name, _dev, _doffset, _ws, _es, _size,	\
				   _props)					\
	BUILD_ASSERT(_ws != 0, "Invalid write size");				\
	BUILD_ASSERT((_ws & (_ws - 1)) == 0, "Invalid write size");		\
	BUILD_ASSERT((_es % _ws) == 0, "Invalid erase size");			\
	BUILD_ASSERT((_size % _ws) == 0, "Invalid size");			\
	const struct storage_area_eeprom _storage_area_##_name =		\
		STORAGE_AREA_EEPROM(_dev, _doffset, _ws, _es, _size, _props)

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_INCLUDE_STORAGE_STORAGE_AREA_EEPROM_H_ */
