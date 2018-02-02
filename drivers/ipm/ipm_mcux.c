/*
 * Copyright (c) 2017, NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <errno.h>
#include <device.h>
#include <ipm.h>
#include <fsl_lpc_mailbox.h>
#include <fsl_clock.h>
#include <soc.h>

#define MCUX_IPM_DATA_REGS 1
#define MCUX_IPM_MAX_ID_VAL 0


struct mcux_mailbox_config {
	MAILBOX_Type *base;
	void (*irq_config_func)(struct device *dev);
};

struct mcux_mailbox_data {
	ipm_callback_t callback;
	void *callback_ctx;
};

static void mcux_mailbox_isr(void *arg)
{
	struct device *dev = arg;
	struct mcux_mailbox_data *data = dev->driver_data;
	mailbox_cpu_id_t cpu_id;
#if defined(__CM4_CMSIS_VERSION)
	cpu_id = kMAILBOX_CM4;
#else
	cpu_id = kMAILBOX_CM0Plus;
#endif

	volatile u32_t value = MAILBOX_GetValue(MAILBOX, cpu_id);

	__ASSERT(value, "spurious MAILBOX interrupt");

	/* Clear or the interrupt gets called intermittently */
	MAILBOX_ClearValueBits(MAILBOX, cpu_id, value);

	if (data->callback) {
		/* Only one MAILBOX, id is unused and set to 0 */
		data->callback(data->callback_ctx, 0, &value);
	}
	/* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F
	 * Store immediate overlapping exception return operation
	 * might vector to incorrect interrupt
	 */
#if defined __CORTEX_M && (__CORTEX_M == 4U)
	__DSB();
#endif
}


static int mcux_mailbox_ipm_send(struct device *d, int wait, u32_t id,
			const void *data, int size)
{
	u32_t data32[MCUX_IPM_DATA_REGS]; /* Until we change API
					   * to u32_t array
					   */
	int flags;
	int i;

	ARG_UNUSED(wait);

	if (id > MCUX_IPM_MAX_ID_VAL) {
		return -EINVAL;
	}

	if (size > MCUX_IPM_DATA_REGS * sizeof(u32_t)) {
		return -EMSGSIZE;
	}

	flags = irq_lock();

	/* Actual message is passing using 32 bits registers */
	memcpy(data32, data, size);

	for (i = 0; i < ARRAY_SIZE(data32); ++i) {
#if defined(__CM4_CMSIS_VERSION)
		MAILBOX_SetValueBits(MAILBOX, kMAILBOX_CM0Plus, data32[i]);
#else
		MAILBOX_SetValueBits(MAILBOX, kMAILBOX_CM4, data32[i]);
#endif
	}

	irq_unlock(flags);

	return 0;
}


static int mcux_mailbox_ipm_max_data_size_get(struct device *d)
{
	ARG_UNUSED(d);
	/* Only a single 32-bit register available */
	return MCUX_IPM_DATA_REGS*sizeof(u32_t);
}


static u32_t mcux_mailbox_ipm_max_id_val_get(struct device *d)
{
	ARG_UNUSED(d);
	/* Only a single instance of MAILBOX available for this platform */
	return MCUX_IPM_MAX_ID_VAL;
}

static void mcux_mailbox_ipm_register_callback(struct device *d,
					       ipm_callback_t cb,
					       void *context)
{
	struct mcux_mailbox_data *driver_data = d->driver_data;

	driver_data->callback = cb;
	driver_data->callback_ctx = context;
}


static int mcux_mailbox_ipm_set_enabled(struct device *d, int enable)
{
	/* For now: nothing to be done */
	return 0;
}


static int mcux_mailbox_init(struct device *dev)
{
	const struct mcux_mailbox_config *config = dev->config->config_info;

	MAILBOX_Init(config->base);
	config->irq_config_func(dev);
	return 0;
}

static const struct ipm_driver_api mcux_mailbox_driver_api = {
	.send = mcux_mailbox_ipm_send,
	.register_callback = mcux_mailbox_ipm_register_callback,
	.max_data_size_get = mcux_mailbox_ipm_max_data_size_get,
	.max_id_val_get = mcux_mailbox_ipm_max_id_val_get,
	.set_enabled = mcux_mailbox_ipm_set_enabled
};


/* Config MAILBOX 0 */

static void mcux_mailbox_config_func_0(struct device *dev);

static const struct mcux_mailbox_config mcux_mailbox_0_config = {
	.base = MAILBOX,
	.irq_config_func = mcux_mailbox_config_func_0,
};

static struct mcux_mailbox_data mcux_mailbox_0_data;

DEVICE_AND_API_INIT(mailbox_0, CONFIG_MAILBOX_MCUX_MAILBOX_0_NAME,
		    &mcux_mailbox_init,
		    &mcux_mailbox_0_data, &mcux_mailbox_0_config,
		    PRE_KERNEL_1, CONFIG_KERNEL_INIT_PRIORITY_DEFAULT,
		    &mcux_mailbox_driver_api);


static void mcux_mailbox_config_func_0(struct device *dev)
{
	IRQ_CONNECT(CONFIG_MAILBOX_MCUX_MAILBOX_0_IRQ,
		    CONFIG_MAILBOX_MCUX_MAILBOX_0_IRQ_PRI,
		    mcux_mailbox_isr, DEVICE_GET(mailbox_0), 0);

	irq_enable(CONFIG_MAILBOX_MCUX_MAILBOX_0_IRQ);
}

