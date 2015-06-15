/*

   (C) MECT srl 2015

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>

 */

#include "chip.h"


/*****************************************************************************
 * DAC PWM section variables declaration
 ****************************************************************************/

#define SCT_PWM            LPC_SCT0 /* Use SCT0 for PWM */
#define SCT_PWM_PIN_OUT1    1        /* COUT1 Generate square wave */
#define SCT_PWM_PIN_OUT2    0        /* COUT0 [index 2] Controls LED */
#define SCT_PWM_OUT1        1        /* Index of OUT 1 PWM */
#define SCT_PWM_OUT2        2        /* Index of OUT 2 PWM */
#define SCT_PWM_RATE   1000000        /* PWM frequency 1 MHz */
#define OUT_STEP_CNT    17999        /* Change duty cycle every 100 us */
#define	HIEDGE_VOLTAGEIN	3400.
#define	LOWEDGE_VOLTAGEIN	675.
#define TICKRATE_HZ     100000        /* 1 ms Tick rate */
#define STEPS	52
/*****************************************************************************
 * Digital IO section variables declaration
 ****************************************************************************/
#define PORT_MASK       ( (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5) | (1 << 6) | (1 << 7) | (1 << 8) | (1 << 10) )
#define switchSinCh1	0
#define switchSquareCh1	1
#define switchSinCh2	2
#define switchSquareCh2	3

static volatile int ticks;
static bool sequence0Complete;



void Board_Init(void);
void PWM_setup_pin(void);
void InitGPIO(void);
void InitDAC(void);
void InitADC(void);
double calc_point(double val, double isi, double fsi, double isl, double fsl);
void UpdateValues();
