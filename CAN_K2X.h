/* Copyright (C) 2014

   Contributors:  Pedro Cevallos & Neil McNeight

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

Acknowledgements:
Fabian Greif for the initial libraries for MCP2515, SJA1000 and AT90CAN
  http://www.kreatives-chaos.com/artikel/universelle-can-bibliothek
  as well as his updates at https://github.com/dergraaf/avr-can-lib
David Harding for his version of the MCP2515 library
  http://forum.arduino.cc/index.php/topic,8730.0.html
Kyle Crockett CANduino library with 16Mhz oscillator
  http://code.google.com/p/canduino/
Nuno Alves for the help on Extended ID messaging
Stevenh for his work on library and all of the MCP research/work
  http://modelrail.otenko.com/arduino/arduino-controller-area-network-can
Collin Kidder (collin80) for his work on the Arduino Due CAN interface
  https://github.com/collin80/due_can
Daniel Kasamis (togglebit) both for his code at
  https://github.com/togglebit/ArduinoDUE_OBD_FreeRunningCAN as well as his
  DUE CANshield http://togglebit.net/product/arduino-due-can-shield/
Cory Fowler (coryjfowler) for 16 MHz bitrate timing information
  https://github.com/coryjfowler/MCP2515_lib
teachop for the FlexCAN library for the Teensy 3.1
  https://github.com/teachop/FlexCAN_Library

-------------------------------------------------------------------------------
Change Log

DATE		VER		WHO			WHAT
07/07/13  0.1   PC    Modified and merge all MCP2515 libraries found. Stripped
                        away most unused functions and corrected MCP2515 defs
09/12/13  0.2   PC    Added selectable CS SPI for CAN controller to use 1 IC
                        to control several mcp2515
02/05/14	0.3		PC		Added filter and mask controls
05/01/14  0.4   PC    Cleaned up functions, variables and added message
                        structures for J1939, CANopen and CAN.
05/07/14	1.0		PC		Released Library to the public through GitHub
06/18/14  1.9   NEM   Preparing a unified CAN library across three different
                        CAN controllers
-------------------------------------------------------------------------------

*/


#if defined(__MK20DX256__) // Teensy 3.1

#ifndef _CAN_K2X_H_
#define _CAN_K2X_H_

#include <Arduino.h>
#include "CAN.h"
#include <stdint.h>

/* Common bit definition */
#define BIT0             (0x00000001L)
#define BIT1             (0x00000002L)
#define BIT2             (0x00000004L)
#define BIT3             (0x00000008L)
#define BIT4             (0x00000010L)
#define BIT5             (0x00000020L)
#define BIT6             (0x00000040L)
#define BIT7             (0x00000080L)
#define BIT8             (0x00000100L)
#define BIT9             (0x00000200L)
#define BIT10            (0x00000400L)
#define BIT11            (0x00000800L)
#define BIT12            (0x00001000L)
#define BIT13            (0x00002000L)
#define BIT14            (0x00004000L)
#define BIT15            (0x00008000L)
#define BIT16            (0x00010000L)
#define BIT17            (0x00020000L)
#define BIT18            (0x00040000L)
#define BIT19            (0x00080000L)
#define BIT20            (0x00100000L)
#define BIT21            (0x00200000L)
#define BIT22            (0x00400000L)
#define BIT23            (0x00800000L)
#define BIT24            (0x01000000L)
#define BIT25            (0x02000000L)
#define BIT26            (0x04000000L)
#define BIT27            (0x08000000L)
#define BIT28            (0x10000000L)
#define BIT29            (0x20000000L)
#define BIT30            (0x40000000L)
#define BIT31            (0x80000000L)


/* FlexCAN module I/O Base Addresss */
#define FLEXCAN0_BASE			(0x40024000L)
#define FLEXCAN1_BASE			(0x400A4000L)

typedef volatile uint32_t vuint32_t;

/*********************************************************************
*
* FlexCAN0 (FLEXCAN0)
*
*********************************************************************/

/* Register read/write macros */
#define FLEXCAN0_MCR                   (*(vuint32_t*)(FLEXCAN0_BASE))
#define FLEXCAN0_CTRL1                 (*(vuint32_t*)(FLEXCAN0_BASE+4))
#define FLEXCAN0_TIMER                 (*(vuint32_t*)(FLEXCAN0_BASE+8))
#define FLEXCAN0_TCR                   (*(vuint32_t*)(FLEXCAN0_BASE+0x0C))
#define FLEXCAN0_RXMGMASK              (*(vuint32_t*)(FLEXCAN0_BASE+0x10))
#define FLEXCAN0_RX14MASK              (*(vuint32_t*)(FLEXCAN0_BASE+0x14))
#define FLEXCAN0_RX15MASK              (*(vuint32_t*)(FLEXCAN0_BASE+0x18))
#define FLEXCAN0_ECR                   (*(vuint32_t*)(FLEXCAN0_BASE+0x1C))
#define FLEXCAN0_ESR1                  (*(vuint32_t*)(FLEXCAN0_BASE+0x20))
#define FLEXCAN0_IMASK2                (*(vuint32_t*)(FLEXCAN0_BASE+0x24))
#define FLEXCAN0_IMASK1                (*(vuint32_t*)(FLEXCAN0_BASE+0x28))
#define FLEXCAN0_IFLAG2                (*(vuint32_t*)(FLEXCAN0_BASE+0x2C))
#define FLEXCAN0_IFLAG1                (*(vuint32_t*)(FLEXCAN0_BASE+0x30))
#define FLEXCAN0_CTRL2                 (*(vuint32_t*)(FLEXCAN0_BASE+0x34))
#define FLEXCAN0_ESR2                  (*(vuint32_t*)(FLEXCAN0_BASE+0x38))
#define FLEXCAN0_FUREQ                 (*(vuint32_t*)(FLEXCAN0_BASE+0x3C))
#define FLEXCAN0_FUACK                 (*(vuint32_t*)(FLEXCAN0_BASE+0x40))
#define FLEXCAN0_CRCR                  (*(vuint32_t*)(FLEXCAN0_BASE+0x44))
#define FLEXCAN0_RXFGMASK              (*(vuint32_t*)(FLEXCAN0_BASE+0x48))
#define FLEXCAN0_RXFIR                 (*(vuint32_t*)(FLEXCAN0_BASE+0x4C))
#define FLEXCAN0_DBG1                  (*(vuint32_t*)(FLEXCAN0_BASE+0x58))
#define FLEXCAN0_DBG2                  (*(vuint32_t*)(FLEXCAN0_BASE+0x5C))

#define FLEXCAN0_IMEUR                 FLEXCAN0_FUREQ
#define FLEXCAN0_LRFR                  FLEXCAN0_FUACK


/* Message Buffers */
#define FLEXCAN0_MB0_CS                (*(vuint32_t*)(FLEXCAN0_BASE+0x80))
#define FLEXCAN0_MB0_ID                (*(vuint32_t*)(FLEXCAN0_BASE+0x84))
#define FLEXCAN0_MB0_WORD0             (*(vuint32_t*)(FLEXCAN0_BASE+0x88))
#define FLEXCAN0_MB0_WORD1             (*(vuint32_t*)(FLEXCAN0_BASE+0x8C))

#define FLEXCAN0_MBn_CS(n)             (*(vuint32_t*)(FLEXCAN0_BASE+0x80+n*0x10))
#define FLEXCAN0_MBn_ID(n)             (*(vuint32_t*)(FLEXCAN0_BASE+0x84+n*0x10))
#define FLEXCAN0_MBn_WORD0(n)          (*(vuint32_t*)(FLEXCAN0_BASE+0x88+n*0x10))
#define FLEXCAN0_MBn_WORD1(n)          (*(vuint32_t*)(FLEXCAN0_BASE+0x8C+n*0x10))

/* Rx Individual Mask Registers */
#define FLEXCAN0_RXIMR0                (*(vuint32_t*)(FLEXCAN0_BASE+0x880))
#define FLEXCAN0_RXIMRn(n)             (*(vuint32_t*)(FLEXCAN0_BASE+0x880+n*4))

/* Rx FIFO ID Filter Table Element 0 to 127 */
#define FLEXCAN0_IDFLT_TAB0            (*(vuint32_t*)(FLEXCAN0_BASE+0xE0))
#define FLEXCAN0_IDFLT_TAB(n)          (*(vuint32_t*)(FLEXCAN0_BASE+0xE0+(n*4)))
//#define FLEXCAN0_IDFLT_TAB(n)		(*(vuint32_t*)(FLEXCAN0_BASE+0xE0+(n<<2)))

/* Memory Error Control Register */
#define FLEXCAN0_MECR					*(vuint32_t*)(FLEXCAN0_BASE+0x3B70))

/* Error Injection Address Register */
#define FLEXCAN0_ERRIAR					*(vuint32_t*)(FLEXCAN0_BASE+0x3B74))

/* Error Injection Data Pattern Register */
#define FLEXCAN0_ERRIDPR				*(vuint32_t*)(FLEXCAN0_BASE+0x3B78))

/* Error Injection Parity Pattern Register */
#define FLEXCAN0_ERRIPPR				*(vuint32_t*)(FLEXCAN0_BASE+0x3B7C))

/* Error Report Address Register */
#define FLEXCAN0_RERRAR					*(vuint32_t*)(FLEXCAN0_BASE+0x3B80))

/* Error Report Data Register */
#define FLEXCAN0_RERRDR					*(vuint32_t*)(FLEXCAN0_BASE+0x3B84))

/* Error Report Syndrome Register */
#define FLEXCAN0_RERRSYNR				*(vuint32_t*)(FLEXCAN0_BASE+0x3B88))

/* Error Status Register */
#define FLEXCAN0_ERRSR					*(vuint32_t*)(FLEXCAN0_BASE+0x3B8C))


/*********************************************************************
*
* FlexCAN1 (FLEXCAN1)
*
*********************************************************************/
/* Register read/write macros */
#define FLEXCAN1_MCR                   (*(vuint32_t*)(FLEXCAN1_BASE))
#define FLEXCAN1_CTRL1                 (*(vuint32_t*)(FLEXCAN1_BASE+4))
#define FLEXCAN1_TIMER                 (*(vuint32_t*)(FLEXCAN1_BASE+8))
#define FLEXCAN1_TCR                   (*(vuint32_t*)(FLEXCAN1_BASE+0x0C))
#define FLEXCAN1_RXMGMASK              (*(vuint32_t*)(FLEXCAN1_BASE+0x10))
#define FLEXCAN1_RX14MASK              (*(vuint32_t*)(FLEXCAN1_BASE+0x14))
#define FLEXCAN1_RX15MASK              (*(vuint32_t*)(FLEXCAN1_BASE+0x18))
#define FLEXCAN1_ECR                   (*(vuint32_t*)(FLEXCAN1_BASE+0x1C))
#define FLEXCAN1_ESR1                  (*(vuint32_t*)(FLEXCAN1_BASE+0x20))
#define FLEXCAN1_IMASK2                (*(vuint32_t*)(FLEXCAN1_BASE+0x24))
#define FLEXCAN1_IMASK1                (*(vuint32_t*)(FLEXCAN1_BASE+0x28))
#define FLEXCAN1_IFLAG2                (*(vuint32_t*)(FLEXCAN1_BASE+0x2C))
#define FLEXCAN1_IFLAG1                (*(vuint32_t*)(FLEXCAN1_BASE+0x30))
#define FLEXCAN1_CTRL2                 (*(vuint32_t*)(FLEXCAN1_BASE+0x34))
#define FLEXCAN1_ESR2                  (*(vuint32_t*)(FLEXCAN1_BASE+0x38))
#define FLEXCAN1_FUREQ                 (*(vuint32_t*)(FLEXCAN1_BASE+0x3C))
#define FLEXCAN1_FUACK                 (*(vuint32_t*)(FLEXCAN1_BASE+0x40))
#define FLEXCAN1_CRCR                  (*(vuint32_t*)(FLEXCAN1_BASE+0x44))
#define FLEXCAN1_RXFGMASK              (*(vuint32_t*)(FLEXCAN1_BASE+0x48))
#define FLEXCAN1_RXFIR                 (*(vuint32_t*)(FLEXCAN1_BASE+0x4C))
#define FLEXCAN1_DBG1                  (*(vuint32_t*)(FLEXCAN1_BASE+0x58))
#define FLEXCAN1_DBG2                  (*(vuint32_t*)(FLEXCAN1_BASE+0x5C))

#define FLEXCAN1_IMEUR                 FLEXCAN1_FUREQ
#define FLEXCAN1_LRFR                  FLEXCAN1_FUACK

/* Message Buffers */
#define FLEXCAN1_MB0_CS                (*(vuint32_t*)(FLEXCAN1_BASE+0x80))
#define FLEXCAN1_MB0_ID                (*(vuint32_t*)(FLEXCAN1_BASE+0x84))
#define FLEXCAN1_MB0_WORD0             (*(vuint32_t*)(FLEXCAN1_BASE+0x88))
#define FLEXCAN1_MB0_WORD1             (*(vuint32_t*)(FLEXCAN1_BASE+0x8C))

#define FLEXCAN1_MBn_CS(n)		         (*(vuint32_t*)(FLEXCAN1_BASE+0x80+n*0x10))
#define FLEXCAN1_MBn_ID(n)		         (*(vuint32_t*)(FLEXCAN1_BASE+0x84+n*0x10))
#define FLEXCAN1_MBn_WORD0(n)		       (*(vuint32_t*)(FLEXCAN1_BASE+0x88+n*0x10))
#define FLEXCAN1_MBn_WORD1(n)		       (*(vuint32_t*)(FLEXCAN1_BASE+0x8C+n*0x10))

/* Rx Individual Mask Registers */
#define FLEXCAN1_RXIMR0                (*(vuint32_t*)(FLEXCAN1_BASE+0x880))
#define FLEXCAN1_RXIMRn(n)             (*(vuint32_t*)(FLEXCAN1_BASE+0x880+n*4))


/* Rx FIFO ID Filter Table Element 0 to 127 */
#define FLEXCAN1_IDFLT_TAB0		(*(vuint32_t*)(FLEXCAN1_BASE+0xE0))
#define FLEXCAN1_IDFLT_TAB(n)		(*(vuint32_t*)(FLEXCAN1_BASE+0xE0+(n<<2)))

/* Memory Error Control Register */
#define FLEXCAN1_MECR					*(vuint32_t*)(FLEXCAN1_BASE+0x7B70))

/* Error Injection Address Register */
#define FLEXCAN1_ERRIAR					*(vuint32_t*)(FLEXCAN1_BASE+0x3B74))

/* Error Injection Data Pattern Register */
#define FLEXCAN1_ERRIDPR				*(vuint32_t*)(FLEXCAN1_BASE+0x3B78))

/* Error Injection Parity Pattern Register */
#define FLEXCAN1_ERRIPPR				*(vuint32_t*)(FLEXCAN1_BASE+0x3B7C))

/* Error Report Address Register */
#define FLEXCAN1_RERRAR					*(vuint32_t*)(FLEXCAN1_BASE+0x3B80))

/* Error Report Data Register */
#define FLEXCAN1_RERRDR					*(vuint32_t*)(FLEXCAN1_BASE+0x3B84))

/* Error Report Syndrome Register */
#define FLEXCAN1_RERRSYNR				*(vuint32_t*)(FLEXCAN1_BASE+0x3B88))

/* Error Status Register */
#define FLEXCAN1_ERRSR					*(vuint32_t*)(FLEXCAN1_BASE+0x3B8C))

/* Bit definitions and macros for FLEXCAN_MCR */
#define FLEXCAN_MCR_MAXMB(x)           (((x)&0x0000007F)<<0)
#define FLEXCAN_MCR_IDAM(x)            (((x)&0x00000003)<<8)
#define FLEXCAN_MCR_MAXMB_MASK			(0x0000007F)
#define FLEXCAN_MCR_IDAM_MASK			(0x00000300)
#define FLEXCAN_MCR_IDAM_BIT_NO        (8)
#define FLEXCAN_MCR_AEN                (0x00001000)
#define FLEXCAN_MCR_LPRIO_EN           (0x00002000)
#define FLEXCAN_MCR_IRMQ               (0x00010000)
#define FLEXCAN_MCR_SRX_DIS            (0x00020000)
#define FLEXCAN_MCR_DOZE               (0x00040000)
#define FLEXCAN_MCR_WAK_SRC            (0x00080000)
#define FLEXCAN_MCR_LPM_ACK            (0x00100000)
#define FLEXCAN_MCR_WRN_EN             (0x00200000)
#define FLEXCAN_MCR_SLF_WAK            (0x00400000)
#define FLEXCAN_MCR_SUPV               (0x00800000)
#define FLEXCAN_MCR_FRZ_ACK            (0x01000000)
#define FLEXCAN_MCR_SOFT_RST           (0x02000000)
#define FLEXCAN_MCR_WAK_MSK            (0x04000000)
#define FLEXCAN_MCR_NOT_RDY            (0x08000000)
#define FLEXCAN_MCR_HALT               (0x10000000)
#define FLEXCAN_MCR_FEN                (0x20000000)
#define FLEXCAN_MCR_FRZ                (0x40000000)
#define FLEXCAN_MCR_MDIS               (0x80000000)

/* Bit definitions and macros for FLEXCAN_CTRL */
#define FLEXCAN_CTRL_PROPSEG(x)        (((x)&0x00000007L)<<0)
#define FLEXCAN_CTRL_LOM               (0x00000008)
#define FLEXCAN_CTRL_LBUF              (0x00000010)
#define FLEXCAN_CTRL_TSYNC             (0x00000020)
#define FLEXCAN_CTRL_BOFF_REC          (0x00000040)
#define FLEXCAN_CTRL_SMP               (0x00000080)
#define FLEXCAN_CTRL_RWRN_MSK          (0x00000400)
#define FLEXCAN_CTRL_TWRN_MSK          (0x00000800)
#define FLEXCAN_CTRL_LPB               (0x00001000L)
#define FLEXCAN_CTRL_CLK_SRC           (0x00002000)
#define FLEXCAN_CTRL_ERR_MSK           (0x00004000)
#define FLEXCAN_CTRL_BOFF_MSK          (0x00008000)
#define FLEXCAN_CTRL_PSEG2(x)          (((x)&0x00000007L)<<16)
#define FLEXCAN_CTRL_PSEG1(x)          (((x)&0x00000007L)<<19)
#define FLEXCAN_CTRL_RJW(x)            (((x)&0x00000003L)<<22)
#define FLEXCAN_CTRL_PRESDIV(x)        (((x)&0x000000FFL)<<24)

/* Bit definitions and macros for FLEXCAN_CTRL2 */
#define FLEXCAN_CTRL2_IMEUEN	        (BIT31)
#define FLEXCAN_CTRL2_RFFN             (0x0F000000L)
#define FLEXCAN_CTRL2_RFFN_BIT_NO      (24)
#define FLEXCAN_CTRL2_TASD             (0x00F80000L)
#define FLEXCAN_CTRL2_TASD_BIT_NO      (19)
#define FLEXCAN_CTRL2_MRP              (BIT18)
#define FLEXCAN_CTRL2_RRS              (BIT17)
#define FLEXCAN_CTRL2_EACEN            (BIT16)
#define FLEXCAN_CTRL2_MUMASK           (BIT1)
#define FLEXCAN_CTRL2_FUMASK           (BIT0)
#define FLEXCAN_CTRL2_LOSTRLMSK		   (BIT2)
#define FLEXCAN_CTRL2_LOSTRMMSK		   (BIT1)
#define FLEXCAN_CTRL2_IMEUMASK		   (BIT0)
#define FLEXCAN_set_rffn(ctrl2,rffn)	ctrl2 = ((ctrl2) & ~FLEXCAN_CTRL2_RFFN) | ((rffn & 0xF)<<FLEXCAN_CTRL2_RFFN_BIT_NO)


/* Bit definitions and macros for FLEXCAN_TIMER */
#define FLEXCAN_TIMER_TIMER(x)         (((x)&0x0000FFFF)<<0)

/* Bit definitions and macros for FLEXCAN_TCR */
#define FLEXCAN_TCR_DSCACK             (0x00000100)
#define FLEXCAN_TCR_BIT_CLS            (0x00000200)
#define FLEXCAN_TCR_TRD                (0x00000400)

/* Bit definitions and macros for FLEXCAN_RXGMASK */
#define FLEXCAN_RXGMASK_MI0            (0x00000001)
#define FLEXCAN_RXGMASK_MI1            (0x00000002)
#define FLEXCAN_RXGMASK_MI2            (0x00000004)
#define FLEXCAN_RXGMASK_MI3            (0x00000008)
#define FLEXCAN_RXGMASK_MI4            (0x00000010)
#define FLEXCAN_RXGMASK_MI5            (0x00000020)
#define FLEXCAN_RXGMASK_MI6            (0x00000040)
#define FLEXCAN_RXGMASK_MI7            (0x00000080)
#define FLEXCAN_RXGMASK_MI8            (0x00000100)
#define FLEXCAN_RXGMASK_MI9            (0x00000200)
#define FLEXCAN_RXGMASK_MI10           (0x00000400)
#define FLEXCAN_RXGMASK_MI11           (0x00000800)
#define FLEXCAN_RXGMASK_MI12           (0x00001000)
#define FLEXCAN_RXGMASK_MI13           (0x00002000)
#define FLEXCAN_RXGMASK_MI14           (0x00004000)
#define FLEXCAN_RXGMASK_MI15           (0x00008000)
#define FLEXCAN_RXGMASK_MI16           (0x00010000)
#define FLEXCAN_RXGMASK_MI17           (0x00020000)
#define FLEXCAN_RXGMASK_MI18           (0x00040000)
#define FLEXCAN_RXGMASK_MI19           (0x00080000)
#define FLEXCAN_RXGMASK_MI20           (0x00100000)
#define FLEXCAN_RXGMASK_MI21           (0x00200000)
#define FLEXCAN_RXGMASK_MI22           (0x00400000)
#define FLEXCAN_RXGMASK_MI23           (0x00800000)
#define FLEXCAN_RXGMASK_MI24           (0x01000000)
#define FLEXCAN_RXGMASK_MI25           (0x02000000)
#define FLEXCAN_RXGMASK_MI26           (0x04000000)
#define FLEXCAN_RXGMASK_MI27           (0x08000000)
#define FLEXCAN_RXGMASK_MI28           (0x10000000)
#define FLEXCAN_RXGMASK_MI29           (0x20000000)
#define FLEXCAN_RXGMASK_MI30           (0x40000000)
#define FLEXCAN_RXGMASK_MI31           (0x80000000)

/* Bit definitions and macros for FLEXCAN_RX14MASK */
#define FLEXCAN_RX14MASK_MI0           (0x00000001)
#define FLEXCAN_RX14MASK_MI1           (0x00000002)
#define FLEXCAN_RX14MASK_MI2           (0x00000004)
#define FLEXCAN_RX14MASK_MI3           (0x00000008)
#define FLEXCAN_RX14MASK_MI4           (0x00000010)
#define FLEXCAN_RX14MASK_MI5           (0x00000020)
#define FLEXCAN_RX14MASK_MI6           (0x00000040)
#define FLEXCAN_RX14MASK_MI7           (0x00000080)
#define FLEXCAN_RX14MASK_MI8           (0x00000100)
#define FLEXCAN_RX14MASK_MI9           (0x00000200)
#define FLEXCAN_RX14MASK_MI10          (0x00000400)
#define FLEXCAN_RX14MASK_MI11          (0x00000800)
#define FLEXCAN_RX14MASK_MI12          (0x00001000)
#define FLEXCAN_RX14MASK_MI13          (0x00002000)
#define FLEXCAN_RX14MASK_MI14          (0x00004000)
#define FLEXCAN_RX14MASK_MI15          (0x00008000)
#define FLEXCAN_RX14MASK_MI16          (0x00010000)
#define FLEXCAN_RX14MASK_MI17          (0x00020000)
#define FLEXCAN_RX14MASK_MI18          (0x00040000)
#define FLEXCAN_RX14MASK_MI19          (0x00080000)
#define FLEXCAN_RX14MASK_MI20          (0x00100000)
#define FLEXCAN_RX14MASK_MI21          (0x00200000)
#define FLEXCAN_RX14MASK_MI22          (0x00400000)
#define FLEXCAN_RX14MASK_MI23          (0x00800000)
#define FLEXCAN_RX14MASK_MI24          (0x01000000)
#define FLEXCAN_RX14MASK_MI25          (0x02000000)
#define FLEXCAN_RX14MASK_MI26          (0x04000000)
#define FLEXCAN_RX14MASK_MI27          (0x08000000)
#define FLEXCAN_RX14MASK_MI28          (0x10000000)
#define FLEXCAN_RX14MASK_MI29          (0x20000000)
#define FLEXCAN_RX14MASK_MI30          (0x40000000)
#define FLEXCAN_RX14MASK_MI31          (0x80000000)

/* Bit definitions and macros for FLEXCAN_RX15MASK */
#define FLEXCAN_RX15MASK_MI0           (0x00000001)
#define FLEXCAN_RX15MASK_MI1           (0x00000002)
#define FLEXCAN_RX15MASK_MI2           (0x00000004)
#define FLEXCAN_RX15MASK_MI3           (0x00000008)
#define FLEXCAN_RX15MASK_MI4           (0x00000010)
#define FLEXCAN_RX15MASK_MI5           (0x00000020)
#define FLEXCAN_RX15MASK_MI6           (0x00000040)
#define FLEXCAN_RX15MASK_MI7           (0x00000080)
#define FLEXCAN_RX15MASK_MI8           (0x00000100)
#define FLEXCAN_RX15MASK_MI9           (0x00000200)
#define FLEXCAN_RX15MASK_MI10          (0x00000400)
#define FLEXCAN_RX15MASK_MI11          (0x00000800)
#define FLEXCAN_RX15MASK_MI12          (0x00001000)
#define FLEXCAN_RX15MASK_MI13          (0x00002000)
#define FLEXCAN_RX15MASK_MI14          (0x00004000)
#define FLEXCAN_RX15MASK_MI15          (0x00008000)
#define FLEXCAN_RX15MASK_MI16          (0x00010000)
#define FLEXCAN_RX15MASK_MI17          (0x00020000)
#define FLEXCAN_RX15MASK_MI18          (0x00040000)
#define FLEXCAN_RX15MASK_MI19          (0x00080000)
#define FLEXCAN_RX15MASK_MI20          (0x00100000)
#define FLEXCAN_RX15MASK_MI21          (0x00200000)
#define FLEXCAN_RX15MASK_MI22          (0x00400000)
#define FLEXCAN_RX15MASK_MI23          (0x00800000)
#define FLEXCAN_RX15MASK_MI24          (0x01000000)
#define FLEXCAN_RX15MASK_MI25          (0x02000000)
#define FLEXCAN_RX15MASK_MI26          (0x04000000)
#define FLEXCAN_RX15MASK_MI27          (0x08000000)
#define FLEXCAN_RX15MASK_MI28          (0x10000000)
#define FLEXCAN_RX15MASK_MI29          (0x20000000)
#define FLEXCAN_RX15MASK_MI30          (0x40000000)
#define FLEXCAN_RX15MASK_MI31          (0x80000000)

/* Bit definitions and macros for FLEXCAN_ECR */
#define FLEXCAN_ECR_TX_ERR_COUNTER(x)  (((x)&0x000000FF)<<0)
#define FLEXCAN_ECR_RX_ERR_COUNTER(x)  (((x)&0x000000FF)<<8)

/* Bit definitions and macros for FLEXCAN_ESR1 */
#define FLEXCAN_ESR_WAK_INT            (0x00000001)
#define FLEXCAN_ESR_ERR_INT            (0x00000002)
#define FLEXCAN_ESR_BOFF_INT           (0x00000004)
#define FLEXCAN_ESR_RX                 (0x00000008)
#define FLEXCAN_ESR_FLT_CONF(x)        (((x)&0x00000003)<<4)
#define FLEXCAN_ESR_FLT_CONF_MASK		(0x00000030)
#define FLEXCAN_ESR_TX                 (0x00000040)
#define FLEXCAN_ESR_IDLE               (0x00000080)
#define FLEXCAN_ESR_RX_WRN             (0x00000100)
#define FLEXCAN_ESR_TX_WRN             (0x00000200)
#define FLEXCAN_ESR_STF_ERR            (0x00000400)
#define FLEXCAN_ESR_FRM_ERR            (0x00000800)
#define FLEXCAN_ESR_CRC_ERR            (0x00001000)
#define FLEXCAN_ESR_ACK_ERR            (0x00002000)
#define FLEXCAN_ESR_BIT0_ERR           (0x00004000)
#define FLEXCAN_ESR_BIT1_ERR           (0x00008000)
#define FLEXCAN_ESR_RWRN_INT           (0x00010000)
#define FLEXCAN_ESR_TWRN_INT           (0x00020000)
#define FLEXCAN_ESR_get_fault_code(esr)	(((esr) & FLEXCAN_ESR_FLT_CONF_MASK)>>4)
#define CAN_ERROR_ACTIVE				0
#define CAN_ERROR_PASSIVE				1
#define CAN_ERROR_BUS_OFF				2

/* Bit definition for FLEXCAN_ESR2 */
#define FLEXCAN_ESR2_IMB            	(0x00002000)
#define FLEXCAN_ESR2_VPS            	(0x00004000)
#define FLEXCAN_ESR2_LTM            	(0x007F0000L)
#define FLEXCAN_ESR2_LTM_BIT_NO       (16)
#define FLEXCAN_ESR2_LOSTRLF			(0x00000004)
#define FLEXCAN_ESR2_LOSTRMF			(0x00000002)
#define FLEXCAN_ESR2_IMEUF				(0x00000001)
#define FLEXCAN_get_LTM(esr2_value)	(((esr2_value) & (FLEXCAN_ESR2_LTM))>>(FLEXCAN_ESR2_LTM_BIT_NO))

/* Bit definitions and macros for FLEXCAN_IMASK1 */
#define FLEXCAN_IMASK1_BUF0M           (0x00000001)
#define FLEXCAN_IMASK1_BUF1M           (0x00000002)
#define FLEXCAN_IMASK1_BUF2M           (0x00000004)
#define FLEXCAN_IMASK1_BUF3M           (0x00000008)
#define FLEXCAN_IMASK1_BUF4M           (0x00000010)
#define FLEXCAN_IMASK1_BUF5M           (0x00000020)
#define FLEXCAN_IMASK1_BUF6M           (0x00000040)
#define FLEXCAN_IMASK1_BUF7M           (0x00000080)
#define FLEXCAN_IMASK1_BUF8M           (0x00000100)
#define FLEXCAN_IMASK1_BUF9M           (0x00000200)
#define FLEXCAN_IMASK1_BUF10M          (0x00000400)
#define FLEXCAN_IMASK1_BUF11M          (0x00000800)
#define FLEXCAN_IMASK1_BUF12M          (0x00001000)
#define FLEXCAN_IMASK1_BUF13M          (0x00002000)
#define FLEXCAN_IMASK1_BUF14M          (0x00004000)
#define FLEXCAN_IMASK1_BUF15M          (0x00008000)
#define FLEXCAN_IMASK1_BUF16M          (0x00010000)
#define FLEXCAN_IMASK1_BUF17M          (0x00020000)
#define FLEXCAN_IMASK1_BUF18M          (0x00040000)
#define FLEXCAN_IMASK1_BUF19M          (0x00080000)
#define FLEXCAN_IMASK1_BUF20M          (0x00100000)
#define FLEXCAN_IMASK1_BUF21M          (0x00200000)
#define FLEXCAN_IMASK1_BUF22M          (0x00400000)
#define FLEXCAN_IMASK1_BUF23M          (0x00800000)
#define FLEXCAN_IMASK1_BUF24M          (0x01000000)
#define FLEXCAN_IMASK1_BUF25M          (0x02000000)
#define FLEXCAN_IMASK1_BUF26M          (0x04000000)
#define FLEXCAN_IMASK1_BUF27M          (0x08000000)
#define FLEXCAN_IMASK1_BUF28M          (0x10000000)
#define FLEXCAN_IMASK1_BUF29M          (0x20000000)
#define FLEXCAN_IMASK1_BUF30M          (0x40000000)
#define FLEXCAN_IMASK1_BUF31M          (0x80000000)

/* Bit definitions and macros for FLEXCAN_IFLAG1 */
#define FLEXCAN_IFLAG1_BUF0I           (0x00000001)
#define FLEXCAN_IFLAG1_BUF1I           (0x00000002)
#define FLEXCAN_IFLAG1_BUF2I           (0x00000004)
#define FLEXCAN_IFLAG1_BUF3I           (0x00000008)
#define FLEXCAN_IFLAG1_BUF4I           (0x00000010)
#define FLEXCAN_IFLAG1_BUF5I           (0x00000020)
#define FLEXCAN_IFLAG1_BUF6I           (0x00000040)
#define FLEXCAN_IFLAG1_BUF7I           (0x00000080)
#define FLEXCAN_IFLAG1_BUF8I           (0x00000100)
#define FLEXCAN_IFLAG1_BUF9I           (0x00000200)
#define FLEXCAN_IFLAG1_BUF10I          (0x00000400)
#define FLEXCAN_IFLAG1_BUF11I          (0x00000800)
#define FLEXCAN_IFLAG1_BUF12I          (0x00001000)
#define FLEXCAN_IFLAG1_BUF13I          (0x00002000)
#define FLEXCAN_IFLAG1_BUF14I          (0x00004000)
#define FLEXCAN_IFLAG1_BUF15I          (0x00008000)
#define FLEXCAN_IFLAG1_BUF16I          (0x00010000)
#define FLEXCAN_IFLAG1_BUF17I          (0x00020000)
#define FLEXCAN_IFLAG1_BUF18I          (0x00040000)
#define FLEXCAN_IFLAG1_BUF19I          (0x00080000)
#define FLEXCAN_IFLAG1_BUF20I          (0x00100000)
#define FLEXCAN_IFLAG1_BUF21I          (0x00200000)
#define FLEXCAN_IFLAG1_BUF22I          (0x00400000)
#define FLEXCAN_IFLAG1_BUF23I          (0x00800000)
#define FLEXCAN_IFLAG1_BUF24I          (0x01000000)
#define FLEXCAN_IFLAG1_BUF25I          (0x02000000)
#define FLEXCAN_IFLAG1_BUF26I          (0x04000000)
#define FLEXCAN_IFLAG1_BUF27I          (0x08000000)
#define FLEXCAN_IFLAG1_BUF28I          (0x10000000)
#define FLEXCAN_IFLAG1_BUF29I          (0x20000000)
#define FLEXCAN_IFLAG1_BUF30I          (0x40000000)
#define FLEXCAN_IFLAG1_BUF31I          (0x80000000)

/* Bit definitions and macros for FLEXCAN_MB_CS */
#define FLEXCAN_MB_CS_TIMESTAMP(x)    (((x)&0x0000FFFF)<<0)
#define FLEXCAN_MB_CS_TIMESTAMP_MASK  (0x0000FFFFL)
#define FLEXCAN_MB_CS_LENGTH(x)       (((x)&0x0000000F)<<16)
#define FLEXCAN_MB_CS_RTR             (0x00100000)
#define FLEXCAN_MB_CS_IDE             (0x00200000)
#define FLEXCAN_MB_CS_SRR             (0x00400000)
#define FLEXCAN_MB_CS_CODE(x)         (((x)&0x0000000F)<<24)
#define FLEXCAN_MB_CS_CODE_MASK		(0x0F000000L)
#define FLEXCAN_MB_CS_DLC_MASK			(0x000F0000L)
#define FLEXCAN_MB_CODE_RX_INACTIVE	(0)
#define FLEXCAN_MB_CODE_RX_EMPTY		(4)
#define FLEXCAN_MB_CODE_RX_FULL		(2)
#define FLEXCAN_MB_CODE_RX_OVERRUN		(6)
#define FLEXCAN_MB_CODE_RX_BUSY		(1)

#define FLEXCAN_MB_CS_IDE_BIT_NO        (21)
#define FLEXCAN_MB_CS_RTR_BIT_NO        (20)
#define FLEXCAN_MB_CS_DLC_BIT_NO        (16)

#define FLEXCAN_MB_CODE_TX_INACTIVE	(8)
#define FLEXCAN_MB_CODE_TX_ABORT		(9)
#define FLEXCAN_MB_CODE_TX_ONCE			(0x0C)
#define FLEXCAN_MB_CODE_TX_RESPONSE		(0x0A)
#define FLEXCAN_MB_CODE_TX_RESPONSE_TEMPO	(0x0E)
#define FLEXCAN_get_code(cs)				(((cs) & FLEXCAN_MB_CS_CODE_MASK)>>24)
#define FLEXCAN_get_length(cs)             (((cs) & FLEXCAN_MB_CS_DLC_MASK)>>16)

/* Bit definitions and macros for FLEXCAN_MB_ID */
#define FLEXCAN_MB_ID_STD_MASK		   (0x1FFC0000L)
#define FLEXCAN_MB_ID_EXT_MASK		   (0x1FFFFFFFL)
#define FLEXCAN_MB_ID_IDEXT(x)        (((x)&0x0003FFFF)<<0)
#define FLEXCAN_MB_ID_IDSTD(x)        (((x)&0x000007FF)<<18)
#define FLEXCAN_MB_ID_PRIO(x)         (((x)&0x00000007)<<29)
#define FLEXCAN_MB_ID_PRIO_BIT_NO	(29)
#define FLEXCAN_MB_ID_STD_BIT_NO	(18)
#define FLEXCAN_MB_ID_EXT_BIT_NO	(0)


/* Bit definitions and macros for FLEXCAN_MB_WORD0 */
#define FLEXCAN_MB_WORD0_DATA3(x)     (((x)&0x000000FF)<<0)
#define FLEXCAN_MB_WORD0_DATA2(x)     (((x)&0x000000FF)<<8)
#define FLEXCAN_MB_WORD0_DATA1(x)     (((x)&0x000000FF)<<16)
#define FLEXCAN_MB_WORD0_DATA0(x)     (((x)&0x000000FF)<<24)

/* Bit definitions and macros for FLEXCAN_MB_WORD1 */
#define FLEXCAN_MB_WORD1_DATA7(x)     (((x)&0x000000FF)<<0)
#define FLEXCAN_MB_WORD1_DATA6(x)     (((x)&0x000000FF)<<8)
#define FLEXCAN_MB_WORD1_DATA5(x)     (((x)&0x000000FF)<<16)
#define FLEXCAN_MB_WORD1_DATA4(x)     (((x)&0x000000FF)<<24)

/* Bit definitions and macros for FLEXCAN_RXIMR0 */
#define FLEXCAN_RXIMR0_MI0             (0x00000001)
#define FLEXCAN_RXIMR0_MI1             (0x00000002)
#define FLEXCAN_RXIMR0_MI2             (0x00000004)
#define FLEXCAN_RXIMR0_MI3             (0x00000008)
#define FLEXCAN_RXIMR0_MI4             (0x00000010)
#define FLEXCAN_RXIMR0_MI5             (0x00000020)
#define FLEXCAN_RXIMR0_MI6             (0x00000040)
#define FLEXCAN_RXIMR0_MI7             (0x00000080)
#define FLEXCAN_RXIMR0_MI8             (0x00000100)
#define FLEXCAN_RXIMR0_MI9             (0x00000200)
#define FLEXCAN_RXIMR0_MI10            (0x00000400)
#define FLEXCAN_RXIMR0_MI11            (0x00000800)
#define FLEXCAN_RXIMR0_MI12            (0x00001000)
#define FLEXCAN_RXIMR0_MI13            (0x00002000)
#define FLEXCAN_RXIMR0_MI14            (0x00004000)
#define FLEXCAN_RXIMR0_MI15            (0x00008000)
#define FLEXCAN_RXIMR0_MI16            (0x00010000)
#define FLEXCAN_RXIMR0_MI17            (0x00020000)
#define FLEXCAN_RXIMR0_MI18            (0x00040000)
#define FLEXCAN_RXIMR0_MI19            (0x00080000)
#define FLEXCAN_RXIMR0_MI20            (0x00100000)
#define FLEXCAN_RXIMR0_MI21            (0x00200000)
#define FLEXCAN_RXIMR0_MI22            (0x00400000)
#define FLEXCAN_RXIMR0_MI23            (0x00800000)
#define FLEXCAN_RXIMR0_MI24            (0x01000000)
#define FLEXCAN_RXIMR0_MI25            (0x02000000)
#define FLEXCAN_RXIMR0_MI26            (0x04000000)
#define FLEXCAN_RXIMR0_MI27            (0x08000000)
#define FLEXCAN_RXIMR0_MI28            (0x10000000)
#define FLEXCAN_RXIMR0_MI29            (0x20000000)
#define FLEXCAN_RXIMR0_MI30            (0x40000000)
#define FLEXCAN_RXIMR0_MI31            (0x80000000)

/* Bit definitions and macros for FLEXCAN_RXIMR1 */
#define FLEXCAN_RXIMR1_MI0             (0x00000001)
#define FLEXCAN_RXIMR1_MI1             (0x00000002)
#define FLEXCAN_RXIMR1_MI2             (0x00000004)
#define FLEXCAN_RXIMR1_MI3             (0x00000008)
#define FLEXCAN_RXIMR1_MI4             (0x00000010)
#define FLEXCAN_RXIMR1_MI5             (0x00000020)
#define FLEXCAN_RXIMR1_MI6             (0x00000040)
#define FLEXCAN_RXIMR1_MI7             (0x00000080)
#define FLEXCAN_RXIMR1_MI8             (0x00000100)
#define FLEXCAN_RXIMR1_MI9             (0x00000200)
#define FLEXCAN_RXIMR1_MI10            (0x00000400)
#define FLEXCAN_RXIMR1_MI11            (0x00000800)
#define FLEXCAN_RXIMR1_MI12            (0x00001000)
#define FLEXCAN_RXIMR1_MI13            (0x00002000)
#define FLEXCAN_RXIMR1_MI14            (0x00004000)
#define FLEXCAN_RXIMR1_MI15            (0x00008000)
#define FLEXCAN_RXIMR1_MI16            (0x00010000)
#define FLEXCAN_RXIMR1_MI17            (0x00020000)
#define FLEXCAN_RXIMR1_MI18            (0x00040000)
#define FLEXCAN_RXIMR1_MI19            (0x00080000)
#define FLEXCAN_RXIMR1_MI20            (0x00100000)
#define FLEXCAN_RXIMR1_MI21            (0x00200000)
#define FLEXCAN_RXIMR1_MI22            (0x00400000)
#define FLEXCAN_RXIMR1_MI23            (0x00800000)
#define FLEXCAN_RXIMR1_MI24            (0x01000000)
#define FLEXCAN_RXIMR1_MI25            (0x02000000)
#define FLEXCAN_RXIMR1_MI26            (0x04000000)
#define FLEXCAN_RXIMR1_MI27            (0x08000000)
#define FLEXCAN_RXIMR1_MI28            (0x10000000)
#define FLEXCAN_RXIMR1_MI29            (0x20000000)
#define FLEXCAN_RXIMR1_MI30            (0x40000000)
#define FLEXCAN_RXIMR1_MI31            (0x80000000)

/* Bit definitions and macros for FLEXCAN_RXIMR2 */
#define FLEXCAN_RXIMR2_MI0             (0x00000001)
#define FLEXCAN_RXIMR2_MI1             (0x00000002)
#define FLEXCAN_RXIMR2_MI2             (0x00000004)
#define FLEXCAN_RXIMR2_MI3             (0x00000008)
#define FLEXCAN_RXIMR2_MI4             (0x00000010)
#define FLEXCAN_RXIMR2_MI5             (0x00000020)
#define FLEXCAN_RXIMR2_MI6             (0x00000040)
#define FLEXCAN_RXIMR2_MI7             (0x00000080)
#define FLEXCAN_RXIMR2_MI8             (0x00000100)
#define FLEXCAN_RXIMR2_MI9             (0x00000200)
#define FLEXCAN_RXIMR2_MI10            (0x00000400)
#define FLEXCAN_RXIMR2_MI11            (0x00000800)
#define FLEXCAN_RXIMR2_MI12            (0x00001000)
#define FLEXCAN_RXIMR2_MI13            (0x00002000)
#define FLEXCAN_RXIMR2_MI14            (0x00004000)
#define FLEXCAN_RXIMR2_MI15            (0x00008000)
#define FLEXCAN_RXIMR2_MI16            (0x00010000)
#define FLEXCAN_RXIMR2_MI17            (0x00020000)
#define FLEXCAN_RXIMR2_MI18            (0x00040000)
#define FLEXCAN_RXIMR2_MI19            (0x00080000)
#define FLEXCAN_RXIMR2_MI20            (0x00100000)
#define FLEXCAN_RXIMR2_MI21            (0x00200000)
#define FLEXCAN_RXIMR2_MI22            (0x00400000)
#define FLEXCAN_RXIMR2_MI23            (0x00800000)
#define FLEXCAN_RXIMR2_MI24            (0x01000000)
#define FLEXCAN_RXIMR2_MI25            (0x02000000)
#define FLEXCAN_RXIMR2_MI26            (0x04000000)
#define FLEXCAN_RXIMR2_MI27            (0x08000000)
#define FLEXCAN_RXIMR2_MI28            (0x10000000)
#define FLEXCAN_RXIMR2_MI29            (0x20000000)
#define FLEXCAN_RXIMR2_MI30            (0x40000000)
#define FLEXCAN_RXIMR2_MI31            (0x80000000)

/* Bit definitions and macros for FLEXCAN_RXIMR3 */
#define FLEXCAN_RXIMR3_MI0             (0x00000001)
#define FLEXCAN_RXIMR3_MI1             (0x00000002)
#define FLEXCAN_RXIMR3_MI2             (0x00000004)
#define FLEXCAN_RXIMR3_MI3             (0x00000008)
#define FLEXCAN_RXIMR3_MI4             (0x00000010)
#define FLEXCAN_RXIMR3_MI5             (0x00000020)
#define FLEXCAN_RXIMR3_MI6             (0x00000040)
#define FLEXCAN_RXIMR3_MI7             (0x00000080)
#define FLEXCAN_RXIMR3_MI8             (0x00000100)
#define FLEXCAN_RXIMR3_MI9             (0x00000200)
#define FLEXCAN_RXIMR3_MI10            (0x00000400)
#define FLEXCAN_RXIMR3_MI11            (0x00000800)
#define FLEXCAN_RXIMR3_MI12            (0x00001000)
#define FLEXCAN_RXIMR3_MI13            (0x00002000)
#define FLEXCAN_RXIMR3_MI14            (0x00004000)
#define FLEXCAN_RXIMR3_MI15            (0x00008000)
#define FLEXCAN_RXIMR3_MI16            (0x00010000)
#define FLEXCAN_RXIMR3_MI17            (0x00020000)
#define FLEXCAN_RXIMR3_MI18            (0x00040000)
#define FLEXCAN_RXIMR3_MI19            (0x00080000)
#define FLEXCAN_RXIMR3_MI20            (0x00100000)
#define FLEXCAN_RXIMR3_MI21            (0x00200000)
#define FLEXCAN_RXIMR3_MI22            (0x00400000)
#define FLEXCAN_RXIMR3_MI23            (0x00800000)
#define FLEXCAN_RXIMR3_MI24            (0x01000000)
#define FLEXCAN_RXIMR3_MI25            (0x02000000)
#define FLEXCAN_RXIMR3_MI26            (0x04000000)
#define FLEXCAN_RXIMR3_MI27            (0x08000000)
#define FLEXCAN_RXIMR3_MI28            (0x10000000)
#define FLEXCAN_RXIMR3_MI29            (0x20000000)
#define FLEXCAN_RXIMR3_MI30            (0x40000000)
#define FLEXCAN_RXIMR3_MI31            (0x80000000)

/* Bit definitions and macros for FLEXCAN_RXIMR4 */
#define FLEXCAN_RXIMR4_MI0             (0x00000001)
#define FLEXCAN_RXIMR4_MI1             (0x00000002)
#define FLEXCAN_RXIMR4_MI2             (0x00000004)
#define FLEXCAN_RXIMR4_MI3             (0x00000008)
#define FLEXCAN_RXIMR4_MI4             (0x00000010)
#define FLEXCAN_RXIMR4_MI5             (0x00000020)
#define FLEXCAN_RXIMR4_MI6             (0x00000040)
#define FLEXCAN_RXIMR4_MI7             (0x00000080)
#define FLEXCAN_RXIMR4_MI8             (0x00000100)
#define FLEXCAN_RXIMR4_MI9             (0x00000200)
#define FLEXCAN_RXIMR4_MI10            (0x00000400)
#define FLEXCAN_RXIMR4_MI11            (0x00000800)
#define FLEXCAN_RXIMR4_MI12            (0x00001000)
#define FLEXCAN_RXIMR4_MI13            (0x00002000)
#define FLEXCAN_RXIMR4_MI14            (0x00004000)
#define FLEXCAN_RXIMR4_MI15            (0x00008000)
#define FLEXCAN_RXIMR4_MI16            (0x00010000)
#define FLEXCAN_RXIMR4_MI17            (0x00020000)
#define FLEXCAN_RXIMR4_MI18            (0x00040000)
#define FLEXCAN_RXIMR4_MI19            (0x00080000)
#define FLEXCAN_RXIMR4_MI20            (0x00100000)
#define FLEXCAN_RXIMR4_MI21            (0x00200000)
#define FLEXCAN_RXIMR4_MI22            (0x00400000)
#define FLEXCAN_RXIMR4_MI23            (0x00800000)
#define FLEXCAN_RXIMR4_MI24            (0x01000000)
#define FLEXCAN_RXIMR4_MI25            (0x02000000)
#define FLEXCAN_RXIMR4_MI26            (0x04000000)
#define FLEXCAN_RXIMR4_MI27            (0x08000000)
#define FLEXCAN_RXIMR4_MI28            (0x10000000)
#define FLEXCAN_RXIMR4_MI29            (0x20000000)
#define FLEXCAN_RXIMR4_MI30            (0x40000000)
#define FLEXCAN_RXIMR4_MI31            (0x80000000)

/* Bit definitions and macros for FLEXCAN_RXIMR5 */
#define FLEXCAN_RXIMR5_MI0             (0x00000001)
#define FLEXCAN_RXIMR5_MI1             (0x00000002)
#define FLEXCAN_RXIMR5_MI2             (0x00000004)
#define FLEXCAN_RXIMR5_MI3             (0x00000008)
#define FLEXCAN_RXIMR5_MI4             (0x00000010)
#define FLEXCAN_RXIMR5_MI5             (0x00000020)
#define FLEXCAN_RXIMR5_MI6             (0x00000040)
#define FLEXCAN_RXIMR5_MI7             (0x00000080)
#define FLEXCAN_RXIMR5_MI8             (0x00000100)
#define FLEXCAN_RXIMR5_MI9             (0x00000200)
#define FLEXCAN_RXIMR5_MI10            (0x00000400)
#define FLEXCAN_RXIMR5_MI11            (0x00000800)
#define FLEXCAN_RXIMR5_MI12            (0x00001000)
#define FLEXCAN_RXIMR5_MI13            (0x00002000)
#define FLEXCAN_RXIMR5_MI14            (0x00004000)
#define FLEXCAN_RXIMR5_MI15            (0x00008000)
#define FLEXCAN_RXIMR5_MI16            (0x00010000)
#define FLEXCAN_RXIMR5_MI17            (0x00020000)
#define FLEXCAN_RXIMR5_MI18            (0x00040000)
#define FLEXCAN_RXIMR5_MI19            (0x00080000)
#define FLEXCAN_RXIMR5_MI20            (0x00100000)
#define FLEXCAN_RXIMR5_MI21            (0x00200000)
#define FLEXCAN_RXIMR5_MI22            (0x00400000)
#define FLEXCAN_RXIMR5_MI23            (0x00800000)
#define FLEXCAN_RXIMR5_MI24            (0x01000000)
#define FLEXCAN_RXIMR5_MI25            (0x02000000)
#define FLEXCAN_RXIMR5_MI26            (0x04000000)
#define FLEXCAN_RXIMR5_MI27            (0x08000000)
#define FLEXCAN_RXIMR5_MI28            (0x10000000)
#define FLEXCAN_RXIMR5_MI29            (0x20000000)
#define FLEXCAN_RXIMR5_MI30            (0x40000000)
#define FLEXCAN_RXIMR5_MI31            (0x80000000)

/* Bit definitions and macros for FLEXCAN_RXIMR6 */
#define FLEXCAN_RXIMR6_MI0             (0x00000001)
#define FLEXCAN_RXIMR6_MI1             (0x00000002)
#define FLEXCAN_RXIMR6_MI2             (0x00000004)
#define FLEXCAN_RXIMR6_MI3             (0x00000008)
#define FLEXCAN_RXIMR6_MI4             (0x00000010)
#define FLEXCAN_RXIMR6_MI5             (0x00000020)
#define FLEXCAN_RXIMR6_MI6             (0x00000040)
#define FLEXCAN_RXIMR6_MI7             (0x00000080)
#define FLEXCAN_RXIMR6_MI8             (0x00000100)
#define FLEXCAN_RXIMR6_MI9             (0x00000200)
#define FLEXCAN_RXIMR6_MI10            (0x00000400)
#define FLEXCAN_RXIMR6_MI11            (0x00000800)
#define FLEXCAN_RXIMR6_MI12            (0x00001000)
#define FLEXCAN_RXIMR6_MI13            (0x00002000)
#define FLEXCAN_RXIMR6_MI14            (0x00004000)
#define FLEXCAN_RXIMR6_MI15            (0x00008000)
#define FLEXCAN_RXIMR6_MI16            (0x00010000)
#define FLEXCAN_RXIMR6_MI17            (0x00020000)
#define FLEXCAN_RXIMR6_MI18            (0x00040000)
#define FLEXCAN_RXIMR6_MI19            (0x00080000)
#define FLEXCAN_RXIMR6_MI20            (0x00100000)
#define FLEXCAN_RXIMR6_MI21            (0x00200000)
#define FLEXCAN_RXIMR6_MI22            (0x00400000)
#define FLEXCAN_RXIMR6_MI23            (0x00800000)
#define FLEXCAN_RXIMR6_MI24            (0x01000000)
#define FLEXCAN_RXIMR6_MI25            (0x02000000)
#define FLEXCAN_RXIMR6_MI26            (0x04000000)
#define FLEXCAN_RXIMR6_MI27            (0x08000000)
#define FLEXCAN_RXIMR6_MI28            (0x10000000)
#define FLEXCAN_RXIMR6_MI29            (0x20000000)
#define FLEXCAN_RXIMR6_MI30            (0x40000000)
#define FLEXCAN_RXIMR6_MI31            (0x80000000)

/* Bit definitions and macros for FLEXCAN_RXIMR7 */
#define FLEXCAN_RXIMR7_MI0             (0x00000001)
#define FLEXCAN_RXIMR7_MI1             (0x00000002)
#define FLEXCAN_RXIMR7_MI2             (0x00000004)
#define FLEXCAN_RXIMR7_MI3             (0x00000008)
#define FLEXCAN_RXIMR7_MI4             (0x00000010)
#define FLEXCAN_RXIMR7_MI5             (0x00000020)
#define FLEXCAN_RXIMR7_MI6             (0x00000040)
#define FLEXCAN_RXIMR7_MI7             (0x00000080)
#define FLEXCAN_RXIMR7_MI8             (0x00000100)
#define FLEXCAN_RXIMR7_MI9             (0x00000200)
#define FLEXCAN_RXIMR7_MI10            (0x00000400)
#define FLEXCAN_RXIMR7_MI11            (0x00000800)
#define FLEXCAN_RXIMR7_MI12            (0x00001000)
#define FLEXCAN_RXIMR7_MI13            (0x00002000)
#define FLEXCAN_RXIMR7_MI14            (0x00004000)
#define FLEXCAN_RXIMR7_MI15            (0x00008000)
#define FLEXCAN_RXIMR7_MI16            (0x00010000)
#define FLEXCAN_RXIMR7_MI17            (0x00020000)
#define FLEXCAN_RXIMR7_MI18            (0x00040000)
#define FLEXCAN_RXIMR7_MI19            (0x00080000)
#define FLEXCAN_RXIMR7_MI20            (0x00100000)
#define FLEXCAN_RXIMR7_MI21            (0x00200000)
#define FLEXCAN_RXIMR7_MI22            (0x00400000)
#define FLEXCAN_RXIMR7_MI23            (0x00800000)
#define FLEXCAN_RXIMR7_MI24            (0x01000000)
#define FLEXCAN_RXIMR7_MI25            (0x02000000)
#define FLEXCAN_RXIMR7_MI26            (0x04000000)
#define FLEXCAN_RXIMR7_MI27            (0x08000000)
#define FLEXCAN_RXIMR7_MI28            (0x10000000)
#define FLEXCAN_RXIMR7_MI29            (0x20000000)
#define FLEXCAN_RXIMR7_MI30            (0x40000000)
#define FLEXCAN_RXIMR7_MI31            (0x80000000)

/* Bit definitions and macros for FLEXCAN_RXIMR8 */
#define FLEXCAN_RXIMR8_MI0             (0x00000001)
#define FLEXCAN_RXIMR8_MI1             (0x00000002)
#define FLEXCAN_RXIMR8_MI2             (0x00000004)
#define FLEXCAN_RXIMR8_MI3             (0x00000008)
#define FLEXCAN_RXIMR8_MI4             (0x00000010)
#define FLEXCAN_RXIMR8_MI5             (0x00000020)
#define FLEXCAN_RXIMR8_MI6             (0x00000040)
#define FLEXCAN_RXIMR8_MI7             (0x00000080)
#define FLEXCAN_RXIMR8_MI8             (0x00000100)
#define FLEXCAN_RXIMR8_MI9             (0x00000200)
#define FLEXCAN_RXIMR8_MI10            (0x00000400)
#define FLEXCAN_RXIMR8_MI11            (0x00000800)
#define FLEXCAN_RXIMR8_MI12            (0x00001000)
#define FLEXCAN_RXIMR8_MI13            (0x00002000)
#define FLEXCAN_RXIMR8_MI14            (0x00004000)
#define FLEXCAN_RXIMR8_MI15            (0x00008000)
#define FLEXCAN_RXIMR8_MI16            (0x00010000)
#define FLEXCAN_RXIMR8_MI17            (0x00020000)
#define FLEXCAN_RXIMR8_MI18            (0x00040000)
#define FLEXCAN_RXIMR8_MI19            (0x00080000)
#define FLEXCAN_RXIMR8_MI20            (0x00100000)
#define FLEXCAN_RXIMR8_MI21            (0x00200000)
#define FLEXCAN_RXIMR8_MI22            (0x00400000)
#define FLEXCAN_RXIMR8_MI23            (0x00800000)
#define FLEXCAN_RXIMR8_MI24            (0x01000000)
#define FLEXCAN_RXIMR8_MI25            (0x02000000)
#define FLEXCAN_RXIMR8_MI26            (0x04000000)
#define FLEXCAN_RXIMR8_MI27            (0x08000000)
#define FLEXCAN_RXIMR8_MI28            (0x10000000)
#define FLEXCAN_RXIMR8_MI29            (0x20000000)
#define FLEXCAN_RXIMR8_MI30            (0x40000000)
#define FLEXCAN_RXIMR8_MI31            (0x80000000)

/* Bit definitions and macros for FLEXCAN_RXIMR9 */
#define FLEXCAN_RXIMR9_MI0             (0x00000001)
#define FLEXCAN_RXIMR9_MI1             (0x00000002)
#define FLEXCAN_RXIMR9_MI2             (0x00000004)
#define FLEXCAN_RXIMR9_MI3             (0x00000008)
#define FLEXCAN_RXIMR9_MI4             (0x00000010)
#define FLEXCAN_RXIMR9_MI5             (0x00000020)
#define FLEXCAN_RXIMR9_MI6             (0x00000040)
#define FLEXCAN_RXIMR9_MI7             (0x00000080)
#define FLEXCAN_RXIMR9_MI8             (0x00000100)
#define FLEXCAN_RXIMR9_MI9             (0x00000200)
#define FLEXCAN_RXIMR9_MI10            (0x00000400)
#define FLEXCAN_RXIMR9_MI11            (0x00000800)
#define FLEXCAN_RXIMR9_MI12            (0x00001000)
#define FLEXCAN_RXIMR9_MI13            (0x00002000)
#define FLEXCAN_RXIMR9_MI14            (0x00004000)
#define FLEXCAN_RXIMR9_MI15            (0x00008000)
#define FLEXCAN_RXIMR9_MI16            (0x00010000)
#define FLEXCAN_RXIMR9_MI17            (0x00020000)
#define FLEXCAN_RXIMR9_MI18            (0x00040000)
#define FLEXCAN_RXIMR9_MI19            (0x00080000)
#define FLEXCAN_RXIMR9_MI20            (0x00100000)
#define FLEXCAN_RXIMR9_MI21            (0x00200000)
#define FLEXCAN_RXIMR9_MI22            (0x00400000)
#define FLEXCAN_RXIMR9_MI23            (0x00800000)
#define FLEXCAN_RXIMR9_MI24            (0x01000000)
#define FLEXCAN_RXIMR9_MI25            (0x02000000)
#define FLEXCAN_RXIMR9_MI26            (0x04000000)
#define FLEXCAN_RXIMR9_MI27            (0x08000000)
#define FLEXCAN_RXIMR9_MI28            (0x10000000)
#define FLEXCAN_RXIMR9_MI29            (0x20000000)
#define FLEXCAN_RXIMR9_MI30            (0x40000000)
#define FLEXCAN_RXIMR9_MI31            (0x80000000)

/* Bit definitions and macros for FLEXCAN_RXIMR10 */
#define FLEXCAN_RXIMR10_MI0            (0x00000001)
#define FLEXCAN_RXIMR10_MI1            (0x00000002)
#define FLEXCAN_RXIMR10_MI2            (0x00000004)
#define FLEXCAN_RXIMR10_MI3            (0x00000008)
#define FLEXCAN_RXIMR10_MI4            (0x00000010)
#define FLEXCAN_RXIMR10_MI5            (0x00000020)
#define FLEXCAN_RXIMR10_MI6            (0x00000040)
#define FLEXCAN_RXIMR10_MI7            (0x00000080)
#define FLEXCAN_RXIMR10_MI8            (0x00000100)
#define FLEXCAN_RXIMR10_MI9            (0x00000200)
#define FLEXCAN_RXIMR10_MI10           (0x00000400)
#define FLEXCAN_RXIMR10_MI11           (0x00000800)
#define FLEXCAN_RXIMR10_MI12           (0x00001000)
#define FLEXCAN_RXIMR10_MI13           (0x00002000)
#define FLEXCAN_RXIMR10_MI14           (0x00004000)
#define FLEXCAN_RXIMR10_MI15           (0x00008000)
#define FLEXCAN_RXIMR10_MI16           (0x00010000)
#define FLEXCAN_RXIMR10_MI17           (0x00020000)
#define FLEXCAN_RXIMR10_MI18           (0x00040000)
#define FLEXCAN_RXIMR10_MI19           (0x00080000)
#define FLEXCAN_RXIMR10_MI20           (0x00100000)
#define FLEXCAN_RXIMR10_MI21           (0x00200000)
#define FLEXCAN_RXIMR10_MI22           (0x00400000)
#define FLEXCAN_RXIMR10_MI23           (0x00800000)
#define FLEXCAN_RXIMR10_MI24           (0x01000000)
#define FLEXCAN_RXIMR10_MI25           (0x02000000)
#define FLEXCAN_RXIMR10_MI26           (0x04000000)
#define FLEXCAN_RXIMR10_MI27           (0x08000000)
#define FLEXCAN_RXIMR10_MI28           (0x10000000)
#define FLEXCAN_RXIMR10_MI29           (0x20000000)
#define FLEXCAN_RXIMR10_MI30           (0x40000000)
#define FLEXCAN_RXIMR10_MI31           (0x80000000)

/* Bit definitions and macros for FLEXCAN_RXIMR11 */
#define FLEXCAN_RXIMR11_MI0            (0x00000001)
#define FLEXCAN_RXIMR11_MI1            (0x00000002)
#define FLEXCAN_RXIMR11_MI2            (0x00000004)
#define FLEXCAN_RXIMR11_MI3            (0x00000008)
#define FLEXCAN_RXIMR11_MI4            (0x00000010)
#define FLEXCAN_RXIMR11_MI5            (0x00000020)
#define FLEXCAN_RXIMR11_MI6            (0x00000040)
#define FLEXCAN_RXIMR11_MI7            (0x00000080)
#define FLEXCAN_RXIMR11_MI8            (0x00000100)
#define FLEXCAN_RXIMR11_MI9            (0x00000200)
#define FLEXCAN_RXIMR11_MI10           (0x00000400)
#define FLEXCAN_RXIMR11_MI11           (0x00000800)
#define FLEXCAN_RXIMR11_MI12           (0x00001000)
#define FLEXCAN_RXIMR11_MI13           (0x00002000)
#define FLEXCAN_RXIMR11_MI14           (0x00004000)
#define FLEXCAN_RXIMR11_MI15           (0x00008000)
#define FLEXCAN_RXIMR11_MI16           (0x00010000)
#define FLEXCAN_RXIMR11_MI17           (0x00020000)
#define FLEXCAN_RXIMR11_MI18           (0x00040000)
#define FLEXCAN_RXIMR11_MI19           (0x00080000)
#define FLEXCAN_RXIMR11_MI20           (0x00100000)
#define FLEXCAN_RXIMR11_MI21           (0x00200000)
#define FLEXCAN_RXIMR11_MI22           (0x00400000)
#define FLEXCAN_RXIMR11_MI23           (0x00800000)
#define FLEXCAN_RXIMR11_MI24           (0x01000000)
#define FLEXCAN_RXIMR11_MI25           (0x02000000)
#define FLEXCAN_RXIMR11_MI26           (0x04000000)
#define FLEXCAN_RXIMR11_MI27           (0x08000000)
#define FLEXCAN_RXIMR11_MI28           (0x10000000)
#define FLEXCAN_RXIMR11_MI29           (0x20000000)
#define FLEXCAN_RXIMR11_MI30           (0x40000000)
#define FLEXCAN_RXIMR11_MI31           (0x80000000)

/* Bit definitions and macros for FLEXCAN_RXIMR12 */
#define FLEXCAN_RXIMR12_MI0            (0x00000001)
#define FLEXCAN_RXIMR12_MI1            (0x00000002)
#define FLEXCAN_RXIMR12_MI2            (0x00000004)
#define FLEXCAN_RXIMR12_MI3            (0x00000008)
#define FLEXCAN_RXIMR12_MI4            (0x00000010)
#define FLEXCAN_RXIMR12_MI5            (0x00000020)
#define FLEXCAN_RXIMR12_MI6            (0x00000040)
#define FLEXCAN_RXIMR12_MI7            (0x00000080)
#define FLEXCAN_RXIMR12_MI8            (0x00000100)
#define FLEXCAN_RXIMR12_MI9            (0x00000200)
#define FLEXCAN_RXIMR12_MI10           (0x00000400)
#define FLEXCAN_RXIMR12_MI11           (0x00000800)
#define FLEXCAN_RXIMR12_MI12           (0x00001000)
#define FLEXCAN_RXIMR12_MI13           (0x00002000)
#define FLEXCAN_RXIMR12_MI14           (0x00004000)
#define FLEXCAN_RXIMR12_MI15           (0x00008000)
#define FLEXCAN_RXIMR12_MI16           (0x00010000)
#define FLEXCAN_RXIMR12_MI17           (0x00020000)
#define FLEXCAN_RXIMR12_MI18           (0x00040000)
#define FLEXCAN_RXIMR12_MI19           (0x00080000)
#define FLEXCAN_RXIMR12_MI20           (0x00100000)
#define FLEXCAN_RXIMR12_MI21           (0x00200000)
#define FLEXCAN_RXIMR12_MI22           (0x00400000)
#define FLEXCAN_RXIMR12_MI23           (0x00800000)
#define FLEXCAN_RXIMR12_MI24           (0x01000000)
#define FLEXCAN_RXIMR12_MI25           (0x02000000)
#define FLEXCAN_RXIMR12_MI26           (0x04000000)
#define FLEXCAN_RXIMR12_MI27           (0x08000000)
#define FLEXCAN_RXIMR12_MI28           (0x10000000)
#define FLEXCAN_RXIMR12_MI29           (0x20000000)
#define FLEXCAN_RXIMR12_MI30           (0x40000000)
#define FLEXCAN_RXIMR12_MI31           (0x80000000)

/* Bit definitions and macros for FLEXCAN_RXIMR13 */
#define FLEXCAN_RXIMR13_MI0            (0x00000001)
#define FLEXCAN_RXIMR13_MI1            (0x00000002)
#define FLEXCAN_RXIMR13_MI2            (0x00000004)
#define FLEXCAN_RXIMR13_MI3            (0x00000008)
#define FLEXCAN_RXIMR13_MI4            (0x00000010)
#define FLEXCAN_RXIMR13_MI5            (0x00000020)
#define FLEXCAN_RXIMR13_MI6            (0x00000040)
#define FLEXCAN_RXIMR13_MI7            (0x00000080)
#define FLEXCAN_RXIMR13_MI8            (0x00000100)
#define FLEXCAN_RXIMR13_MI9            (0x00000200)
#define FLEXCAN_RXIMR13_MI10           (0x00000400)
#define FLEXCAN_RXIMR13_MI11           (0x00000800)
#define FLEXCAN_RXIMR13_MI12           (0x00001000)
#define FLEXCAN_RXIMR13_MI13           (0x00002000)
#define FLEXCAN_RXIMR13_MI14           (0x00004000)
#define FLEXCAN_RXIMR13_MI15           (0x00008000)
#define FLEXCAN_RXIMR13_MI16           (0x00010000)
#define FLEXCAN_RXIMR13_MI17           (0x00020000)
#define FLEXCAN_RXIMR13_MI18           (0x00040000)
#define FLEXCAN_RXIMR13_MI19           (0x00080000)
#define FLEXCAN_RXIMR13_MI20           (0x00100000)
#define FLEXCAN_RXIMR13_MI21           (0x00200000)
#define FLEXCAN_RXIMR13_MI22           (0x00400000)
#define FLEXCAN_RXIMR13_MI23           (0x00800000)
#define FLEXCAN_RXIMR13_MI24           (0x01000000)
#define FLEXCAN_RXIMR13_MI25           (0x02000000)
#define FLEXCAN_RXIMR13_MI26           (0x04000000)
#define FLEXCAN_RXIMR13_MI27           (0x08000000)
#define FLEXCAN_RXIMR13_MI28           (0x10000000)
#define FLEXCAN_RXIMR13_MI29           (0x20000000)
#define FLEXCAN_RXIMR13_MI30           (0x40000000)
#define FLEXCAN_RXIMR13_MI31           (0x80000000)

/* Bit definitions and macros for FLEXCAN_RXIMR14 */
#define FLEXCAN_RXIMR14_MI0            (0x00000001)
#define FLEXCAN_RXIMR14_MI1            (0x00000002)
#define FLEXCAN_RXIMR14_MI2            (0x00000004)
#define FLEXCAN_RXIMR14_MI3            (0x00000008)
#define FLEXCAN_RXIMR14_MI4            (0x00000010)
#define FLEXCAN_RXIMR14_MI5            (0x00000020)
#define FLEXCAN_RXIMR14_MI6            (0x00000040)
#define FLEXCAN_RXIMR14_MI7            (0x00000080)
#define FLEXCAN_RXIMR14_MI8            (0x00000100)
#define FLEXCAN_RXIMR14_MI9            (0x00000200)
#define FLEXCAN_RXIMR14_MI10           (0x00000400)
#define FLEXCAN_RXIMR14_MI11           (0x00000800)
#define FLEXCAN_RXIMR14_MI12           (0x00001000)
#define FLEXCAN_RXIMR14_MI13           (0x00002000)
#define FLEXCAN_RXIMR14_MI14           (0x00004000)
#define FLEXCAN_RXIMR14_MI15           (0x00008000)
#define FLEXCAN_RXIMR14_MI16           (0x00010000)
#define FLEXCAN_RXIMR14_MI17           (0x00020000)
#define FLEXCAN_RXIMR14_MI18           (0x00040000)
#define FLEXCAN_RXIMR14_MI19           (0x00080000)
#define FLEXCAN_RXIMR14_MI20           (0x00100000)
#define FLEXCAN_RXIMR14_MI21           (0x00200000)
#define FLEXCAN_RXIMR14_MI22           (0x00400000)
#define FLEXCAN_RXIMR14_MI23           (0x00800000)
#define FLEXCAN_RXIMR14_MI24           (0x01000000)
#define FLEXCAN_RXIMR14_MI25           (0x02000000)
#define FLEXCAN_RXIMR14_MI26           (0x04000000)
#define FLEXCAN_RXIMR14_MI27           (0x08000000)
#define FLEXCAN_RXIMR14_MI28           (0x10000000)
#define FLEXCAN_RXIMR14_MI29           (0x20000000)
#define FLEXCAN_RXIMR14_MI30           (0x40000000)
#define FLEXCAN_RXIMR14_MI31           (0x80000000)

/* Bit definitions and macros for FLEXCAN_RXIMR15 */
#define FLEXCAN_RXIMR15_MI0            (0x00000001)
#define FLEXCAN_RXIMR15_MI1            (0x00000002)
#define FLEXCAN_RXIMR15_MI2            (0x00000004)
#define FLEXCAN_RXIMR15_MI3            (0x00000008)
#define FLEXCAN_RXIMR15_MI4            (0x00000010)
#define FLEXCAN_RXIMR15_MI5            (0x00000020)
#define FLEXCAN_RXIMR15_MI6            (0x00000040)
#define FLEXCAN_RXIMR15_MI7            (0x00000080)
#define FLEXCAN_RXIMR15_MI8            (0x00000100)
#define FLEXCAN_RXIMR15_MI9            (0x00000200)
#define FLEXCAN_RXIMR15_MI10           (0x00000400)
#define FLEXCAN_RXIMR15_MI11           (0x00000800)
#define FLEXCAN_RXIMR15_MI12           (0x00001000)
#define FLEXCAN_RXIMR15_MI13           (0x00002000)
#define FLEXCAN_RXIMR15_MI14           (0x00004000)
#define FLEXCAN_RXIMR15_MI15           (0x00008000)
#define FLEXCAN_RXIMR15_MI16           (0x00010000)
#define FLEXCAN_RXIMR15_MI17           (0x00020000)
#define FLEXCAN_RXIMR15_MI18           (0x00040000)
#define FLEXCAN_RXIMR15_MI19           (0x00080000)
#define FLEXCAN_RXIMR15_MI20           (0x00100000)
#define FLEXCAN_RXIMR15_MI21           (0x00200000)
#define FLEXCAN_RXIMR15_MI22           (0x00400000)
#define FLEXCAN_RXIMR15_MI23           (0x00800000)
#define FLEXCAN_RXIMR15_MI24           (0x01000000)
#define FLEXCAN_RXIMR15_MI25           (0x02000000)
#define FLEXCAN_RXIMR15_MI26           (0x04000000)
#define FLEXCAN_RXIMR15_MI27           (0x08000000)
#define FLEXCAN_RXIMR15_MI28           (0x10000000)
#define FLEXCAN_RXIMR15_MI29           (0x20000000)
#define FLEXCAN_RXIMR15_MI30           (0x40000000)
#define FLEXCAN_RXIMR15_MI31           (0x80000000)

/* Bit definitions for CRC register */
#define FLEXCAN_CRCR_MBCRC_BIT_NO		(16)
#define FLEXCAN_CRCR_MBCRC_MASK			(0x007F0000)
#define FLEXCAN_CRCR_CRC_BIT_NO			(0)
#define FLEXCAN_CRCR_CRC_MASK			(0x00007FFF)

/* Bit definition for Individual Matching Elements Update Register (IMEUR) */
#define FLEXCAN_IMEUR_IMEUP_MASK	(0x0000007F)
#define FLEXCAN_IMEUR_IMEUP_BIT_NO	(0)
#define FLEXCAN_IMEUR_IMEUREQ_MASK	(0x00000100)
#define FLEXCAN_IMEUR_IMEUACK_MASK	(0x00000200)
#define FLEXCAN_Set_IMEUP(imeur,imeup)	imeur = (imeur & ~(FLEXCAN_IMEUR_IMEUP_MASK)) | (imeup & FLEXCAN_IMEUR_IMEUP_MASK)
#define FLEXCAN_Get_IMEUP(imeur)	(imeur & FLEXCAN_IMEUR_IMEUP_MASK)

/* Bit definition for Lost Rx Frames Register (LRFR)
 */
#define FLEXCAN_LRFR_LOSTRLP_MASK	(0x007F0000)
#define FLEXCAN_LRFR_LFIFOMTC_MASK	(0x00008000)
#define FLEXCAN_LRFR_LOSTRMP_MASK	(0x000001FF)
#define FLEXCAN_LRFR_LOSTRLP_BIT_NO	(16)
#define FLEXCAN_LRFR_LFIFOMTC_BIT_NO	(15)
#define FLEXCAN_LRFR_LOSTRMP_BIT_NO		(0)
#define FLEXCAN_Get_LostMBLocked(lrfr)	((lrfr & FLEXCAN_LRFR_LOSTRLP_MASK)>>(FLEXCAN_LRFR_LOSTRLP_BIT_NO))
#define FLEXCAN_Get_LostMBUpdated(lrfr)	((lrfr & FLEXCAN_LRFR_LOSTRMP_MASK))

/* Bit definition for Memory Error Control Register */
#define FLEXCAN_MECR_NCEFAFRZ_MASK	(0x00000080)
#define FLEXCAN_MECR_RERRDIS_MASK	(0x00000100)
#define FLEXCAN_MECR_EXTERRIE_MAKS	(0x00002000)
#define FLEXCAN_MECR_FAERRIE_MAKS	(0x00004000)
#define FLEXCAN_MECR_HAERRIE_MAKS	(0x00008000)
#define FLEXCAN_MECR_CEI_MSK_MAKS	(0x00010000)
#define FLEXCAN_MECR_FANCEI_MSK_MAKS	(0x00040000)
#define FLEXCAN_MECR_HANCEI_MSK_MAKS	(0x00080000)
#define FLEXCAN_MECR_ECRWRDIS_MSK_MAKS	(0x80000000)

/* Bit definition for Error Report Address Register (RERRAR) */
#define FLEXCAN_RERRAR_NCE_MASK		(0x01000000)
#define FLEXCAN_RERRAR_SAID_MASK	(0x00070000)
#define FLEXCAN_ERRADDR_MASK		(0x00003FFF)

/* Bit definition for Error Report Syndrome Register (RERRSYNR) */
#define FLEXCAN_RERRSYNR_BE3_MASK	(0x80000000)
#define FLEXCAN_RERRSYNR_SYND3_MASK	(0x1F000000)
#define FLEXCAN_RERRSYNR_SYND3_BIT_NO	(24)
#define FLEXCAN_RERRSYNR_BE2_MASK	(0x00800000)
#define FLEXCAN_RERRSYNR_SYND2_MASK	(0x001F0000)
#define FLEXCAN_RERRSYNR_SYND2_BIT_NO	(16)
#define FLEXCAN_RERRSYNR_BE1_MASK	(0x00008000)
#define FLEXCAN_RERRSYNR_SYND1_MASK	(0x00001F00)
#define FLEXCAN_RERRSYNR_SYND1_BIT_NO	(8)
#define FLEXCAN_RERRSYNR_BE0_MASK	(0x00000080)
#define FLEXCAN_RERRSYNR_SYND0_MASK	(0x0000001F)
#define FLEXCAN_RERRSYNR_SYND0_BIT_NO	(0)

#define FLEXCAN_RERRSYNR_check_BEn_Bit(errsynr,n) (errsynr & FLEXCAN_RERRSYNR_BE##n##_MASK)
#define FLEXCAN_RERRSYNR_get_SYNDn(errsynr,n) (errsynr & FLEXCAN_RERRSYNR_SYND##n##_MASK)
#define FLEXCAN_RERRSYNR_check_SYNDn_Bit(errsynr,n) ((errsynr & FLEXCAN_RERRSYNR_SYND##n##_MASK)>>FLEXCAN_RERRSYNR_SYND##n##_BIT_NO)

/* Bit definition for Error Status Register (ERRSR) */
#define FLEXCAN_ERRSR_CEIOF_MASK	(0x00000001)
#define FLEXCAN_ERRSR_FANCEIOF_MASK	(0x00000004)
#define FLEXCAN_ERRSR_HANCEIOF_MASK	(0x00000008)
#define FLEXCAN_ERRSR_CEIF_MASK		(0x00010000)
#define FLEXCAN_ERRSR_FANCEIF_MASK	(0x00040000)
#define FLEXCAN_ERRSR_HANCEIF_MASK	(0x00080000)

// -------------------------------------------------------------
class CAN_K2X : public CANClass
{
  public:
    CAN_K2X();
    void begin(uint32_t bitrate);
    void end(void);
    uint8_t available(void);
    CAN_Frame read();
    void flush();
    uint8_t write(const CAN_Frame &msg);

  private:
    CAN_Filter defaultMask;
    void setFilter(const CAN_Filter &filter, uint8_t n);

};

extern CAN_K2X CAN;

#endif // _CAN_K2X_H_

#endif // defined(__MK20DX256__) // Teensy 3.1
