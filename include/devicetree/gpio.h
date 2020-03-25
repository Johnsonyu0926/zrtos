/**
 * @file
 * @brief GPIO Devicetree macro public API header file.
 */

/*
 * Copyright (c) 2020, Linaro Ltd.
 * Copyright (c) 2020 Nordic Semiconductor
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZEPHYR_INCLUDE_DEVICETREE_GPIO_H_
#define ZEPHYR_INCLUDE_DEVICETREE_GPIO_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup devicetree-gpio Devicetree GPIO API
 * @{
 */

/**
 * @brief Get gpio controller "name" (label property) at an index
 *
 * It's an error if the GPIO controller referenced by the phandle
 * in property "gpio_pha" at index "idx" has no label property.
 *
 * Example devicetree fragment:
 *
 *     gpio1: gpio@... {
 *             label = "GPIO_1";
 *     };
 *
 *     gpio2: gpio@... {
 *             label = "GPIO_2";
 *     };
 *
 *     n: node {
 *             gpios = <&gpio1 10 20>, <&gpio2 30 40>;
 *     };
 *
 * Example usage:
 *
 *     DT_GPIO_LABEL_BY_IDX(DT_NODELABEL(n), gpios, 1) // "GPIO_2"
 *
 * @param node_id node identifier
 * @param gpio_pha lowercase-and-underscores GPIO property with
 *        type "phandle-array"
 * @param idx logical index into the property
 * @return the label property for the referenced node at index idx
 * @see DT_PHANDLE_BY_IDX()
 */
#define DT_GPIO_LABEL_BY_IDX(node_id, gpio_pha, idx) \
	DT_PROP_BY_PHANDLE_IDX(node_id, gpio_pha, idx, label)

/**
 * @brief Equivalent to DT_GPIO_LABEL_BY_IDX(node_id, gpio_pha, 0)
 * @param node_id node identifier
 * @param gpio_pha lowercase-and-underscores GPIO property with
 *        type "phandle-array"
 * @return the label property for the named specifier at index 0
 * @see DT_GPIO_LABEL_BY_IDX()
 */
#define DT_GPIO_LABEL(node_id, gpio_pha) \
	DT_GPIO_LABEL_BY_IDX(node_id, gpio_pha, 0)

/**
 * @brief Get gpio controller 'pin' at an index
 *
 * This macro only works for GPIO controllers that specify a 'pin'
 * field in the phandle-array specifier.  Refer to the specific GPIO
 * controller binding if needed.
 *
 * @param node_id node identifier
 * @param gpio_pha lowercase-and-underscores GPIO property with
 *        type "phandle-array"
 * @param idx logical index into the property
 * @return the pin value for the named specifier at index idx
 * @see DT_PHA_BY_IDX()
 * @see DT_PHA()
 */
#define DT_GPIO_PIN_BY_IDX(node_id, gpio_pha, idx) \
	DT_PHA_BY_IDX(node_id, gpio_pha, idx, pin)

/**
 * @brief Equivalent to DT_GPIO_PIN_BY_IDX(node_id, gpio_pha, 0)
 * @param node_id node identifier
 * @param gpio_pha lowercase-and-underscores GPIO property with
 *        type "phandle-array"
 * @return the pin value for the named specifier at index idx
 * @see DT_GPIO_PIN_BY_IDX()
 */
#define DT_GPIO_PIN(node_id, gpio_pha) \
	DT_GPIO_PIN_BY_IDX(node_id, gpio_pha, 0)

/**
 * @brief Get gpio controller 'flags' at an index
 *
 * This macro only works for GPIO controllers that specify a 'flags'
 * field in the phandle-array specifier.  Refer to the specific GPIO
 * controller binding if needed.
 *
 * @param node_id node identifier
 * @param gpio_pha lowercase-and-underscores GPIO property with
 *        type "phandle-array"
 * @param idx logical index into the property
 * @return the flags value for the named specifier at index idx
 * @see DT_PHA_BY_IDX()
 * @see DT_PHA()
 */
#define DT_GPIO_FLAGS_BY_IDX(node_id, gpio_pha, idx) \
	DT_PHA_BY_IDX(node_id, gpio_pha, idx, flags)

/**
 * @brief Equivalent to DT_GPIO_FLAGS_BY_IDX(node_id, gpio_pha, 0)
 * @param node_id node identifier
 * @param gpio_pha lowercase-and-underscores GPIO property with
 *        type "phandle-array"
 * @return the flags value for the named specifier at index idx
 * @see DT_GPIO_FLAGS_BY_IDX()
 */
#define DT_GPIO_FLAGS(node_id, gpio_pha) \
	DT_GPIO_FLAGS_BY_IDX(node_id, gpio_pha, 0)

/**
 * @brief Get gpio controller "name" at an index (see @ref DT_GPIO_LABEL_BY_IDX)
 * @param inst instance number
 * @param gpio_pha lowercase-and-underscores GPIO property with
 *        type "phandle-array"
 * @param idx logical index into the property
 * @return the label property for the named specifier at index idx
 */
#define DT_INST_GPIO_LABEL_BY_IDX(inst, gpio_pha, idx) \
	DT_GPIO_LABEL_BY_IDX(DT_DRV_INST(inst), gpio_pha, idx)

/**
 * @brief Equivalent to DT_INST_GPIO_LABEL_BY_IDX(inst, gpio_pha, 0)
 * @param inst instance number
 * @param gpio_pha lowercase-and-underscores GPIO property with
 *        type "phandle-array"
 * @return the label property for the named specifier at index 0
 */
#define DT_INST_GPIO_LABEL(inst, gpio_pha) \
	DT_INST_GPIO_LABEL_BY_IDX(inst, gpio_pha, 0)

/**
 * @brief Get gpio controller "pin" at an index (see @ref DT_GPIO_PIN_BY_IDX)
 * @param inst instance number
 * @param gpio_pha lowercase-and-underscores GPIO property with
 *        type "phandle-array"
 * @param idx logical index into the property
 * @return the pin value for the named specifier at index idx
 */
#define DT_INST_GPIO_PIN_BY_IDX(inst, gpio_pha, idx) \
	DT_GPIO_PIN_BY_IDX(DT_DRV_INST(inst), gpio_pha, idx)

/**
 * @brief Equivalent to DT_INST_GPIO_PIN_BY_IDX(inst, gpio_pha, 0)
 * @param inst instance number
 * @param gpio_pha lowercase-and-underscores GPIO property with
 *        type "phandle-array"
 * @return the pin value for the named specifier at index 0
 * @see DT_INST_GPIO_PIN_BY_IDX()
 */
#define DT_INST_GPIO_PIN(inst, gpio_pha) \
	DT_INST_GPIO_PIN_BY_IDX(inst, gpio_pha, 0)

/**
 * @brief Get a devicetree property value (see @ref DT_GPIO_FLAGS_BY_IDX)
 * @param inst instance number
 * @param gpio_pha lowercase-and-underscores GPIO property with
 *        type "phandle-array"
 * @param idx logical index into the property
 * @return a representation of the property's value
 */
#define DT_INST_GPIO_FLAGS_BY_IDX(inst, gpio_pha, idx) \
	DT_GPIO_FLAGS_BY_IDX(DT_DRV_INST(inst), gpio_pha, idx)

/**
 * @brief Equivalent to DT_INST_GPIO_FLAGS_BY_IDX(inst, gpio_pha, 0)
 * @param inst instance number
 * @param gpio_pha lowercase-and-underscores GPIO property with
 *        type "phandle-array"
 * @return the flags value for the named specifier at index 0
 * @see DT_INST_GPIO_FLAGS_BY_IDX()
 */
#define DT_INST_GPIO_FLAGS(inst, gpio_pha) \
	DT_INST_GPIO_FLAGS_BY_IDX(inst, gpio_pha, 0)

/**
 * @brief Create a struct initializer for a "gpio" phandle-array element
 *
 * The struct initializer will be of the form:
 *
 * { label, pin, flags },
 *
 * Example devicetree fragment:
 *
 *     gpio1: gpio@... {
 *             label = "GPIO_1";
 *     };
 *
 *     gpio2: gpio@... {
 *             label = "GPIO_2";
 *     };
 *
 *     n: node {
 *             gpios = <&gpio1 10 20>, <&gpio2 30 40>;
 *     };
 *
 * Example usage:
 *
 *     DT_GPIO_ELEM(DT_NODELABEL(n), gpios, 1)
 *
 * Will produce the following struct initializer:
 *
 *     { "GPIO_2", 30, 40 },
 *
 * The idx element is placed first so that the macro will be usable by
 * UTIL_LISTIFY
 *
 * @param idx into the gpio_pha phandle-array
 * @param node_id node identifier
 * @param gpio_pha lowercase-and-underscores GPIO property with
 *        type "phandle-array"
 * @return a struct initializer for the give index "idx" into the
 *         "phandle-array" gpio_pha
 */
#define DT_GPIO_ELEM(idx, node_id, gpio_pha) \
	{ \
		DT_GPIO_LABEL_BY_IDX(node_id, gpio_pha, idx), \
		DT_GPIO_PIN_BY_IDX(node_id, gpio_pha, idx), \
		DT_GPIO_FLAGS_BY_IDX(node_id, gpio_pha, idx), \
	},

/**
 * @brief Create an array initializer for a "gpio" phandle-array
 *
 * The array initializer will be of the form:
 *
 * {
 *     { label[0], pin[0], flags[0] }
 *     ...
 *     { label[n-1], pin[n-1], flags[n-1] },
 * }
 *
 * Example devicetree fragment:
 *
 *     gpio1: gpio@... {
 *             label = "GPIO_1";
 *     };
 *
 *     gpio2: gpio@... {
 *             label = "GPIO_2";
 *     };
 *
 *     n: node {
 *             gpios = <&gpio1 10 20>, <&gpio2 30 40>;
 *     };
 *
 * Example usage:
 *
 *     DT_GPIO_LISTIFY(DT_NODELABEL(n), gpios)
 *
 * Will produce:
 *
 *     { { "GPIO_1", 10, 20 }, { "GPIO_2", 30, 40 }, }
 *
 * @param node_id node identifier
 * @param gpio_pha lowercase-and-underscores GPIO property with
 *        type "phandle-array"
 * @return an array initializer for the given "phandle-array" gpio_pha
 */
#define DT_GPIO_LISTIFY(node_id, gpio_pha) \
	{ UTIL_LISTIFY(DT_PROP_LEN(node_id, gpio_pha), DT_GPIO_ELEM, \
		       node_id, gpio_pha) }

/**
 * @}
 */

#ifdef __cplusplus
}
#endif


#endif  /* ZEPHYR_INCLUDE_DEVICETREE_GPIO_H_ */
