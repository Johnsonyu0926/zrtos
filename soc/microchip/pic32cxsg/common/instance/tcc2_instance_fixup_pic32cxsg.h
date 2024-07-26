/*
 * Copyright (c) 2024 Microchip
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _MICROCHIP_PIC32CXSG_TCC2_INSTANCE_FIXUP_H_
#define _MICROCHIP_PIC32CXSG_TCC2_INSTANCE_FIXUP_H_

/* ========== Register definition for TCC2 peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#define REG_TCC2_CTRLA             (0x42000C00) /**< \brief (TCC2) Control A */
#define REG_TCC2_CTRLBCLR          (0x42000C04) /**< \brief (TCC2) Control B Clear */
#define REG_TCC2_CTRLBSET          (0x42000C05) /**< \brief (TCC2) Control B Set */
#define REG_TCC2_SYNCBUSY          (0x42000C08) /**< \brief (TCC2) Synchronization Busy */
#define REG_TCC2_FCTRLA            (0x42000C0C) /**< \brief (TCC2) Recoverable Fault A Configuration */
#define REG_TCC2_FCTRLB            (0x42000C10) /**< \brief (TCC2) Recoverable Fault B Configuration */
#define REG_TCC2_WEXCTRL           (0x42000C14) /**< \brief (TCC2) Waveform Extension Configuration */
#define REG_TCC2_DRVCTRL           (0x42000C18) /**< \brief (TCC2) Driver Control */
#define REG_TCC2_DBGCTRL           (0x42000C1E) /**< \brief (TCC2) Debug Control */
#define REG_TCC2_EVCTRL            (0x42000C20) /**< \brief (TCC2) Event Control */
#define REG_TCC2_INTENCLR          (0x42000C24) /**< \brief (TCC2) Interrupt Enable Clear */
#define REG_TCC2_INTENSET          (0x42000C28) /**< \brief (TCC2) Interrupt Enable Set */
#define REG_TCC2_INTFLAG           (0x42000C2C) /**< \brief (TCC2) Interrupt Flag Status and Clear */
#define REG_TCC2_STATUS            (0x42000C30) /**< \brief (TCC2) Status */
#define REG_TCC2_COUNT             (0x42000C34) /**< \brief (TCC2) Count */
#define REG_TCC2_WAVE              (0x42000C3C) /**< \brief (TCC2) Waveform Control */
#define REG_TCC2_PER               (0x42000C40) /**< \brief (TCC2) Period */
#define REG_TCC2_CC0               (0x42000C44) /**< \brief (TCC2) Compare and Capture 0 */
#define REG_TCC2_CC1               (0x42000C48) /**< \brief (TCC2) Compare and Capture 1 */
#define REG_TCC2_CC2               (0x42000C4C) /**< \brief (TCC2) Compare and Capture 2 */
#define REG_TCC2_PERBUF            (0x42000C6C) /**< \brief (TCC2) Period Buffer */
#define REG_TCC2_CCBUF0            (0x42000C70) /**< \brief (TCC2) Compare and Capture Buffer 0 */
#define REG_TCC2_CCBUF1            (0x42000C74) /**< \brief (TCC2) Compare and Capture Buffer 1 */
#define REG_TCC2_CCBUF2            (0x42000C78) /**< \brief (TCC2) Compare and Capture Buffer 2 */
#else
#define REG_TCC2_CTRLA             (*(RwReg  *)0x42000C00UL) /**< \brief (TCC2) Control A */
#define REG_TCC2_CTRLBCLR          (*(RwReg8 *)0x42000C04UL) /**< \brief (TCC2) Control B Clear */
#define REG_TCC2_CTRLBSET          (*(RwReg8 *)0x42000C05UL) /**< \brief (TCC2) Control B Set */
#define REG_TCC2_SYNCBUSY          (*(RoReg  *)0x42000C08UL) /**< \brief (TCC2) Synchronization Busy */
#define REG_TCC2_FCTRLA            (*(RwReg  *)0x42000C0CUL) /**< \brief (TCC2) Recoverable Fault A Configuration */
#define REG_TCC2_FCTRLB            (*(RwReg  *)0x42000C10UL) /**< \brief (TCC2) Recoverable Fault B Configuration */
#define REG_TCC2_WEXCTRL           (*(RwReg  *)0x42000C14UL) /**< \brief (TCC2) Waveform Extension Configuration */
#define REG_TCC2_DRVCTRL           (*(RwReg  *)0x42000C18UL) /**< \brief (TCC2) Driver Control */
#define REG_TCC2_DBGCTRL           (*(RwReg8 *)0x42000C1EUL) /**< \brief (TCC2) Debug Control */
#define REG_TCC2_EVCTRL            (*(RwReg  *)0x42000C20UL) /**< \brief (TCC2) Event Control */
#define REG_TCC2_INTENCLR          (*(RwReg  *)0x42000C24UL) /**< \brief (TCC2) Interrupt Enable Clear */
#define REG_TCC2_INTENSET          (*(RwReg  *)0x42000C28UL) /**< \brief (TCC2) Interrupt Enable Set */
#define REG_TCC2_INTFLAG           (*(RwReg  *)0x42000C2CUL) /**< \brief (TCC2) Interrupt Flag Status and Clear */
#define REG_TCC2_STATUS            (*(RwReg  *)0x42000C30UL) /**< \brief (TCC2) Status */
#define REG_TCC2_COUNT             (*(RwReg  *)0x42000C34UL) /**< \brief (TCC2) Count */
#define REG_TCC2_WAVE              (*(RwReg  *)0x42000C3CUL) /**< \brief (TCC2) Waveform Control */
#define REG_TCC2_PER               (*(RwReg  *)0x42000C40UL) /**< \brief (TCC2) Period */
#define REG_TCC2_CC0               (*(RwReg  *)0x42000C44UL) /**< \brief (TCC2) Compare and Capture 0 */
#define REG_TCC2_CC1               (*(RwReg  *)0x42000C48UL) /**< \brief (TCC2) Compare and Capture 1 */
#define REG_TCC2_CC2               (*(RwReg  *)0x42000C4CUL) /**< \brief (TCC2) Compare and Capture 2 */
#define REG_TCC2_PERBUF            (*(RwReg  *)0x42000C6CUL) /**< \brief (TCC2) Period Buffer */
#define REG_TCC2_CCBUF0            (*(RwReg  *)0x42000C70UL) /**< \brief (TCC2) Compare and Capture Buffer 0 */
#define REG_TCC2_CCBUF1            (*(RwReg  *)0x42000C74UL) /**< \brief (TCC2) Compare and Capture Buffer 1 */
#define REG_TCC2_CCBUF2            (*(RwReg  *)0x42000C78UL) /**< \brief (TCC2) Compare and Capture Buffer 2 */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _MICROCHIP_PIC32CXSG_TCC2_INSTANCE_FIXUP_H_ */
