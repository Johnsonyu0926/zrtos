/*
 * Copyright (c) 2024 Microchip
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZEPHYR_INCLUDE_DRIVERS_POWER_MICROCHIP_PIC32CXSG_SUPC_H_
#define ZEPHYR_INCLUDE_DRIVERS_POWER_MICROCHIP_PIC32CXSG_SUPC_H_

#define PIC32CXSG_DT_SUPC_CONTROLLER DEVICE_DT_GET(DT_NODELABEL(supc))

#define PIC32CXSG_DT_SUPC_WAKEUP_SOURCE_ID(node_id) \
	DT_PROP_BY_IDX(node_id, wakeup_source_id wakeup_source_id)

#define PIC32CXSG_DT_INST_SUPC_WAKEUP_SOURCE_ID(inst) \
	PIC32CXSG_DT_SUPC_WAKEUP_SOURCE_ID(DT_DRV_INST(inst))

#endif /* ZEPHYR_INCLUDE_DRIVERS_POWER_MICROCHIP_PIC32CXSG_SUPC_H_ */
