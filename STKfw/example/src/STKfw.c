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
#define revision 1.00
#include "board.h"

/*****************************************************************************
 * lookup tables
 ****************************************************************************/

int sinx[55]={50,56,62,68,73,78,83,87,91,94,97,98,100,100,100,99,97,95,92,88,84,79,74,69,63,57,51,45,39,33,28,23,18,\
13,10,6,4,2,1,0,0,1,3,5,8,11,15,20,25,30,36,42,48,};
int square[55]={100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,0,0,0,0,0,0,0,\
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,};


uint32_t cnt1 = 0, cnt2 = 0;
float freq1=1.0,freq2=1.0,vout;
int main(){


	int  out_dp = 0;
	int  x=0,y=0,period=0;
	int switch1a,switch1b,switch1c,switch2a,switch2b,switch2c;

	SystemCoreClockUpdate();
	Board_Init();
	InitDAC();
	InitADC();
	InitGPIO();
	SysTick_Config(Chip_Clock_GetSysTickClockRate() / TICKRATE_HZ);
	SysTick_Config(SystemCoreClock / TICKRATE_HZ);

	while (1) {
/*
 *
 * 	check for dip switches state
 *
 */
		switch2a=Chip_GPIO_ReadPortBit(LPC_GPIO, 0, 14);
		switch2b=Chip_GPIO_ReadPortBit(LPC_GPIO, 0, 15);
		switch2c=Chip_GPIO_ReadPortBit(LPC_GPIO, 0, 16);

		switch1a=Chip_GPIO_ReadPortBit(LPC_GPIO, 0, 24);
		switch1b=Chip_GPIO_ReadPortBit(LPC_GPIO, 0, 25);
		switch1c=Chip_GPIO_ReadPortBit(LPC_GPIO, 0, 26);



		if(freq1>0.)
			period=(int)OUT_STEP_CNT/(freq1*10);
		if (cnt1 >= period) {
			cnt1 = 0;
/* increment lookup table index*/
			if (++x > STEPS-1) {
				x = 0;
			}
		}

		if(switch1a){								// DC signal
			out_dp=100;
			Chip_GPIO_WritePortBit(LPC_GPIO, 0, switchSquareCh1,false);
			Chip_GPIO_WritePortBit(LPC_GPIO, 0, switchSinCh1,false);
		}
		else if(switch1b){							// square wave
			out_dp=square[x];
			Chip_GPIO_WritePortBit(LPC_GPIO, 0, switchSquareCh1,true);
			Chip_GPIO_WritePortBit(LPC_GPIO, 0, switchSinCh1,false);
		}

		else if(switch1c){							// sinusoidal wave
			out_dp=(int)calc_point((double)cnt1, 0., (double)(period) , (double)sinx[x], (double)sinx[x+1]);
		}
		else {										// none
			out_dp=0;
			Chip_GPIO_WritePortBit(LPC_GPIO, 0, switchSquareCh1,false);
			Chip_GPIO_WritePortBit(LPC_GPIO, 0, switchSinCh1,false);
		}
/* update pwm output*/
		Chip_SCTPWM_SetDutyCycle(SCT_PWM, SCT_PWM_OUT1,
		Chip_SCTPWM_PercentageToTicks(SCT_PWM, out_dp));


		if(freq2>0.)
			period=(int)OUT_STEP_CNT/(freq2*10);
		if (cnt2 >= period) {
			cnt2 = 0;
/* increment lookup table index*/
			if (++y > STEPS-1) {
				y = 0;
			}
		}

		if(switch2a){								// DC signal
			out_dp=100;
			Chip_GPIO_WritePortBit(LPC_GPIO, 0, switchSquareCh2,false);
			Chip_GPIO_WritePortBit(LPC_GPIO, 0, switchSinCh2,false);
		}
		else if(switch2b){							// square wave
			out_dp=square[y];
			Chip_GPIO_WritePortBit(LPC_GPIO, 0, switchSquareCh2,true);
			Chip_GPIO_WritePortBit(LPC_GPIO, 0, switchSinCh2,false);
		}
		else if(switch2c){						// sinusoidal wave
			out_dp=(int)calc_point((double)cnt2, 0., (double)(period) , (double)sinx[y], (double)sinx[y+1]);
			Chip_GPIO_WritePortBit(LPC_GPIO, 0, switchSquareCh2,true);
			Chip_GPIO_WritePortBit(LPC_GPIO, 0, switchSinCh2,false);
		}
		else {
			out_dp=0;							// none
			Chip_GPIO_WritePortBit(LPC_GPIO, 0, switchSquareCh2,false);
			Chip_GPIO_WritePortBit(LPC_GPIO, 0, switchSinCh2,false);
		}
		/* update pwm output*/
		Chip_SCTPWM_SetDutyCycle(SCT_PWM, SCT_PWM_OUT2,
		Chip_SCTPWM_PercentageToTicks(SCT_PWM, out_dp));


		if(sequence0Complete){
			UpdateValues();
			sequence0Complete=0;
		}

		//__WFI();
	}

}
/**
 * @brief	read analog inputs values & Update bar led
 * @return	Nothing
 */
void UpdateValues(){
	double tmp;
	int states;
	static int blink=0;

	tmp=(0xfff0 & Chip_ADC_GetDataReg(LPC_ADC0,3 ))>>4;
	tmp=calc_point((double)tmp,LOWEDGE_VOLTAGEIN,HIEDGE_VOLTAGEIN,0,10.);
	vout=(vout*2.0+tmp)/3.0;

	tmp=(0xfff0 & Chip_ADC_GetDataReg(LPC_ADC0,1 ))>>4;
	tmp=calc_point((double)tmp,0.,4096.,0.1,10.);
	freq1=(freq1*9.0+tmp)/10.0;

	tmp=(0xfff0 & Chip_ADC_GetDataReg(LPC_ADC0,2 ))>>4;
	tmp=calc_point((double)tmp,0.,4096.,0.1,10.);
	freq2=(freq2*9.0+tmp)/10.0;



	states=0;
	switch ((int)vout){
	case 0:states=0x0; break;
	case 1: states= (1<<7); break;
	case 2: states= (1<<7) | (1<<8); break;
	case 3: states= (1<<7) | (1<<8) | (1<<10) ; break;
	case 4: states= (1<<7) | (1<<8) | (1<<10) | (1<<0) ; break;
	case 5: states= (1<<7) | (1<<8) | (1<<10) | (1<<0) | (1<<1) ; break;
	case 6: states= (1<<7) | (1<<8) | (1<<10) | (1<<0) | (1<<1) | (1<<2) ; break;
	case 7: states= (1<<7) | (1<<8) | (1<<10) | (1<<0) | (1<<1) | (1<<2) | (1<<3); break;
	case 8: states= (1<<7) | (1<<8) | (1<<10) | (1<<0) | (1<<1) | (1<<2) | (1<<3) | (1<<4); break;
	case 9: states= (1<<7) | (1<<8) | (1<<10) | (1<<0) | (1<<1) | (1<<2) | (1<<3) | (1<<4) | (1<<5); break;
	case 10: states=(1<<7) | (1<<8) | (1<<10) | (1<<0) | (1<<1) | (1<<2) | (1<<3) | (1<<4) | (1<<5) | (1<<6); break;
	default :
		if(vout > 11 ){
			if(blink>50 && blink<100)
				states=(1<<7) | (1<<8) | (1<<10) | (1<<0) | (1<<1) | (1<<2) | (1<<3) | (1<<4) | (1<<5) | (1<<6);
			else if(blink< 50)
				states=0x0;
			if(blink++ >100)
				blink=0;
		}

	}

	Chip_GPIO_SetMaskedPortValue(LPC_GPIO, 1, states);

}




/**
 * @brief	Handle interrupt from SysTick timer
 * @return	Nothing
 */
void SysTick_Handler(void)
{
	static uint32_t count;
	cnt1 ++;
	cnt2 ++;
	count++;
	if (count >= (TICKRATE_HZ / 100) && !sequence0Complete) {
		count = 0;
		/* Manual start for ADC0 conversion sequence A */
		Chip_ADC_StartSequencer(LPC_ADC0, ADC_SEQA_IDX);
	}
}

/**
 * @brief	Handle interrupt from ADC0 sequencer A
 * @return	Nothing
 */
void ADC0A_IRQHandler(void)
{
	uint32_t pending;

	/* Get pending interrupts */
	pending = Chip_ADC_GetFlags(LPC_ADC0);

	/* Sequence A completion interrupt */
	if (pending & ADC_FLAGS_SEQA_INT_MASK) {
		sequence0Complete = true;
	}

	/* Clear any pending interrupts */
	Chip_ADC_ClearFlags(LPC_ADC0, pending);


}


/**
 * @brief	linear interpolation
 * @return	double
 */
double calc_point(double val, double isi, double fsi, double isl, double fsl)
{

			return (val-isi)*(fsl-isl)/(fsi-isi)+isl;

}