#ifndef _BBB_REGS_H_
#define _BBB_REGS_H_

/*
 * =====================================================================================
 *
 *       Filename:  bb_regs
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  23/05/2018 17:47:17
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Francisco Helder (FHC), helderhdw@gmail.com
 *   Organization:  UFC-Quixadá
 *
 * =====================================================================================
 */

/* 
 * ================================= Macro definitions =================================
*/ 
/* The Input clock is selected as 24MHz. So for 1ms set the count to 0x5DC0. 
 *If the input clock is changed to different source this value has to be updated 
 *accordingly.
*/
#define TIMER_1MS_COUNT							0x5DC0
#define TIMER_OVERFLOW							0xFFFFFFFFu

#define DELAY_USE_INTERRUPT						0x0001

#define delay_opt_0								0x03E8
#define delay_opt_1								0x01F4
#define delay_opt_2								0x0190
#define delay_opt_3								0x00FA

#define TINT7									0x005F
#define GPIOINT1A								0x0062
#define GPIOINT1B								0x0063

//Bits 0-4 define which MIR_CLEAR register bit must be set
#define INT_CONFIG_BIT(irq_num)					(1<<(irq_num & 0x1F))

/**
 * \brief   This macro will check for write POSTED status
 *
 * \param   reg          Register whose status has to be checked
 *
 *    'reg' can take the following values \n
 *    DMTIMER_WRITE_POST_TCLR - Timer Control register \n
 *    DMTIMER_WRITE_POST_TCRR - Timer Counter register \n
 *    DMTIMER_WRITE_POST_TLDR - Timer Load register \n
 *    DMTIMER_WRITE_POST_TTGR - Timer Trigger register \n
 *    DMTIMER_WRITE_POST_TMAR - Timer Match register \n
 *
 **/
#define DMTimerWaitForWrite(reg)				\
												if(HWREG(DMTIMER_TSICR) & 0x4)\
												while((reg & HWREG(DMTIMER_TWPS)));

/* Interrupt */
#define INTC_BASE       						0x48200000
#define INTC_SIR_IRQ    						0x48200040
#define INTC_CONTROL    						0x48200048
#define INTC_ILR        						0x48200100
#define INTC_MIR_CLEAR2 						0x482000C8
#define INTC_MIR_CLEAR3 						0x482000E8
#define INTC_SYSCONFIG							0x48200010
#define INTC_SYSSTATUS							0x48200014
#define INTC_THRESHOLD							0x48200068

/* UART0 */
#define UART0_BASE 								0x44E09000
#define UART0_RHR  								0x44E09000
#define UART0_THR  								0x44E09000
#define UART0_LSR  								0x44E09014

/* Watch Dog Timer */
#define  WDT_BASE 								0x44E35000
#define  WDT_WSPR 								0x44E35048
#define  WDT_WWPS 								0x44E35034

/* Clock */
#define CM_PER_GPIO1_CLKCTRL 					0x44E000AC
#define CM_PER_RTC_RTC_CLKCTRL   				0x44E00800
#define CM_PER_RTC_CLKSTCTRL     				0x44E00804
#define CM_PER_TIMER7_CLKCTRL					0x44E0007C
#define SOC_CM_DPLL_CLKSEL_TIMER7_CLK          	0x44E00504

#define BASE_CONTROL_REGS						0x44E10000
/* Button pins */
#define CONF_GPMC_BEN1							0x44E10878			//gpio1_28
#define CONF_GPMC_A0							0x44E10840			//gpio1_16
/* External led pins */
#define CONF_GPMC_AD2							0x44E10808			//gpio1_2
#define CONF_GPMC_AD3							0x44E1080C			//gpio1_3
#define CONF_GPMC_AD7							0x44E1081C			//gpio1_7

/* GPIO1 registers */
#define GPIO1_IRQSTATUS_0						0x4804C02C
#define GPIO1_IRQSTATUS_1						0x4804C030
#define GPIO1_IRQSTATUS_SET_0					0x4804C034
#define GPIO1_IRQSTATUS_SET_1					0x4804C038
#define GPIO1_OE								0x4804C134
#define GPIO1_DATAIN							0x4804C138
#define GPIO1_RISINGDETECT						0x4804C148
#define GPIO1_DEBOUNCENABLE						0x4804C150
#define GPIO1_CLEARDATAOUT						0x4804C190
#define GPIO1_SETDATAOUT				 		0x4804C194



/* DMTIMER */
#define SOC_DMTIMER_7_REGS                   	0x4804A000
#define DMTIMER_TIDR   							0x4804A000
#define DMTIMER_TIOCP_CFG   					0x4804A010
#define DMTIMER_IRQ_EOI   						0x4804A020
#define DMTIMER_IRQSTATUS_RAW   				0x4804A024
#define DMTIMER_IRQSTATUS   					0x4804A028
#define DMTIMER_IRQENABLE_SET   				0x4804A02C
#define DMTIMER_IRQENABLE_CLR   				0x4804A030
#define DMTIMER_IRQWAKEEN  	 					0x4804A034
#define DMTIMER_TCLR   							0x4804A038
#define DMTIMER_TCRR   							0x4804A03C
#define DMTIMER_TLDR   							0x4804A040
#define DMTIMER_TTGR   							0x4804A044
#define DMTIMER_TWPS   							0x4804A048
#define DMTIMER_TMAR   							0x4804A04C
#define DMTIMER_TCAR(n)   						(0x50 + (((n) - 1) * 8))
#define DMTIMER_TSICR  		 					0x4804A054

/* RTC */
#define RTC_BASE           						0x44E3E000
#define SECONDS_REG        						0x44E3E000
#define MINUTES_REG        						0x44E3E004
#define HOURS_REG          						0x44E3E008
#define RTC_CTRL_REG       						0x44E3E040
#define RTC_STATUS_REG     						0x44E3E044
#define RTC_INTERRUPTS_REG 						0x44E3E048
#define RTC_OSC_REG        						0x44E3E054
#define KICK0R             						0x44E3E06C
#define KICK1R             						0x44E3E070


#endif //__BB_REGS_H__

