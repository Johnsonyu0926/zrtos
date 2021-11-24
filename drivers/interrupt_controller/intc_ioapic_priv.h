/* ioapic_priv.h - private IOAPIC APIs */

/*
 * Copyright (c) 2012-2015 Wind River Systems, Inc.
 * Copyright (c) 2015 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZEPHYR_DRIVERS_INTERRUPT_CONTROLLER_INTC_IOAPIC_PRIV_H_
#define ZEPHYR_DRIVERS_INTERRUPT_CONTROLLER_INTC_IOAPIC_PRIV_H_

/* IO APIC direct register offsets */

#define IOAPIC_IND 0x00U   /* Index Register */
#define IOAPIC_DATA 0x10U  /* IO window (data) - pc.h */
#define IOAPIC_IRQPA 0x20U /* IRQ Pin Assertion Register */
#define IOAPIC_EOI 0x40U   /* EOI Register */

/* IO APIC indirect register offset */

#define IOAPIC_ID 0x00U     /* IOAPIC ID */
#define IOAPIC_VERS 0x01U   /* IOAPIC Version */
#define IOAPIC_ARB 0x02U    /* IOAPIC Arbitration ID */
#define IOAPIC_BOOT 0x03U   /* IOAPIC Boot Configuration */
#define IOAPIC_REDTBL 0x10U /* Redirection Table (24 * 64bit) */

/* Interrupt delivery type */

#define IOAPIC_DT_APIC 0x0 /* APIC serial bus */
#define IOAPIC_DT_FS 0x1   /* Front side bus message*/

/* Version register bits */

#define IOAPIC_MRE_MASK 0x00ff0000U /* Max Red. entry mask */
#define IOAPIC_MRE_POS 16
#define IOAPIC_PRQ 0x00008000U      /* this has IRQ reg */
#define IOAPIC_VERSION 0x000000ffU  /* version number */

/* Redirection table entry bits: upper 32 bit */

#define IOAPIC_DESTINATION 0xff000000U

/* Redirection table entry bits: lower 32 bit */

#define IOAPIC_VEC_MASK 0x000000ffU

#endif /* ZEPHYR_DRIVERS_INTERRUPT_CONTROLLER_INTC_IOAPIC_PRIV_H_ */
