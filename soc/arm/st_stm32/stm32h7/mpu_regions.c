/*
 * Copyright (c) 2020 Mario Jaun
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <devicetree.h>
#include "../../common/cortex_m/arm_mpu_mem_cfg.h"

static const struct arm_mpu_region mpu_regions[] = {
        MPU_REGION_ENTRY("FLASH", CONFIG_FLASH_BASE_ADDRESS,
                         REGION_FLASH_ATTR(REGION_FLASH_SIZE)),
        MPU_REGION_ENTRY("SRAM", CONFIG_SRAM_BASE_ADDRESS,
                         REGION_RAM_ATTR(REGION_SRAM_SIZE)),
#if DT_NODE_HAS_STATUS(DT_NODELABEL(sram2), okay)
        MPU_REGION_ENTRY("SRAM2_NOCACHE",
                         DT_REG_ADDR(DT_NODELABEL(sram2)),
                         REGION_RAM_NOCACHE_ATTR(REGION_16K)),
#endif
};

const struct arm_mpu_config mpu_config = {
        .num_regions = ARRAY_SIZE(mpu_regions),
        .mpu_regions = mpu_regions,
};