/*
 * Copyright (c) 2018, NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr.h>
#include <sys/printk.h>
#include <device.h>
#include <drivers/ipm.h>

static void ipm_callback(const struct device *dev, void *context,
			 uint32_t id, volatile void *data)
{
	int i;
	int status;
	struct ipm_msg msg;
	uint32_t *data32 = (uint32_t *)data;

	printk("%s: id = %u, data = 0x", __func__, id);
	for (i = 0; i < (CONFIG_IPM_IMX_MAX_DATA_SIZE / 4); i++) {
		printk("%08x", data32[i]);
	}
	printk("\n");

	msg.data = (const void *) data;
	msg.size = CONFIG_IPM_IMX_MAX_DATA_SIZE;
	msg.id = id;

	status = ipm_send(dev, 1, &msg);
	if (status) {
		printk("ipm_send() failed: %d\n", status);
	}
}

void main(void)
{
	const struct device *ipm;

	ipm = DEVICE_DT_GET(DT_NODELABEL(mub));
	if (!device_is_ready(ipm)) {
		while (1) {
		}
	}
	ipm_register_callback(ipm, ipm_callback, NULL);
	ipm_set_enabled(ipm, 1);
	printk("IPM initialized, data size = %d\n",
		CONFIG_IPM_IMX_MAX_DATA_SIZE);

	while (1) {
	}
}
