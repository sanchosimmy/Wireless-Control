/**
 * \file fan.c
 * \ingroup WiControl
 * \brief Driver for fan controlling.
 * \author Akshay Pampatwar
 * \version 0.0
 * \date 15th April 2020
 */

#include "fan.h"
#include "pinmap.h"
#include "commonheader.h"
#include "globalvariable.h"
#include "math.h"

#define ScrTriggerDi() GPIOIntDisable(GPIO_PORTF_BASE, ZCD)		//TimerDisable(TIMER1_BASE, TIMER_A)
#define ScrTriggerEn() GPIOIntEnable(GPIO_PORTF_BASE, ZCD)		//TimerEnable(TIMER1_BASE, TIMER_A)
#define SCR_TRIGGER_PW 2	//mS.
#define Vrms_Normal	230		//Volts

///Generates 2 mS pulse to trigger fan SCR and Starts ADC sampling.
void ScrPulseTr(void);
///Turns on GPIO o/p pin of fan SCR for full AC vtg cycle
void FanFullOn(void);
///Turns off fan if speed is 0
void Fan_Off_Speed(void);

/**
 * Turns on indicator LED depending on fan speed
 * @param FanSpeed: 0 to 5
 */
void FanSpeedIndUpdate(int8_t);

///Initializes gpio pin as interrupt for rising edge trigger. For detecting zero crossing of AC vtg.
void GpioZeroCrossDetInit(void);
///ISR for zero crossing.Update & Starts timer1 (After that time SCR will be triggered)
void PortFIntHandler(void);
/**
 * Correction of SCR trigger time depending on Line freq and AC rms voltage.
 * @param Trigger_angle: (0 to 180)
 * @return Scr Trigger delay: (mS) Corresponding to i/p trigger angle & AC vgt correction.
 */
float VtgSwingCorr(float);
/**
 * Convert ADC sample into AC rms vtg and saves in Vrms variable.
 * @param ADC Reading: (uint32_t)
 */
void AcVtgRms(uint32_t);

///Timer2B for pulse generation, SCR trigger pulse (2 mS)
void Timer2BInit(void);
///SCR pulse:ISR for Timer2B, Turns off SCR gpio pin.
void Timer2BIntHandler(void);
///Timer1 for Scr trigger delay i.e. from 0 to 10 mS
void Timer1Init(void);
///ISR for full Timer1
void Timer1IntHandler(void);
///ADC module 1 initialize
void ADC1_Init(void);
///Takes sampled adc value and calls AcVtgRms(uint32_t) to save AC rms vtg
void Adc1IntHandler(void);
///Zero crossing detection ISR: Triggers Timer1 for SCR trigger delay.
void PortFIntHandler(void);

float ScrTrgDelay = 5.0;	//mS
static float Vrms = 230.0;	//V
static float AcTimePeriod = 20.0;		//mS Ac 1/frequency
static bool Fan_On_Off = 0;
//static uint32_t ADCValue;

void Fan_Init()
{
//	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
//	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
//	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
//	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
//	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
//	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

	GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, Fan_State);
	GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, Fan_1 | Fan_2);
	GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, Fan_4 | Fan_Scr_Op);
	GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, Fan_3);
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, Fan_5);

	Timer2BInit();	//Used for pulse generation
	ADC1_Init();	//To initialize sensing of AC voltage
	GpioZeroCrossDetInit();	//Interrupt for detecting rising egde.
}

void Fan_On()
{
	Fan_On_Off = 1;

	if(Fan_Speed <= 0)
		Fan_Off();
	else if(Fan_Speed == MAX_FAN_SPEED)
		FanFullOn();
	else
		ScrTriggerEn();
	//Switch On Fan LED indicator
	GPIOPinWrite(GPIO_PORTA_BASE,Fan_State,Fan_State);
}

void Fan_Off()
{
	Fan_On_Off = 0;

	ScrTriggerDi();
	GPIOPinWrite(GPIO_PORTC_BASE,Fan_Scr_Op,0);
	GPIOPinWrite(GPIO_PORTA_BASE,Fan_State,0);
}
void Fan_Off_Speed()
{
	ScrTriggerDi();
	GPIOPinWrite(GPIO_PORTC_BASE,Fan_Scr_Op,0);
	GPIOPinWrite(GPIO_PORTA_BASE,Fan_State,0);
}
void FanFullOn()
{
//	ScrTriggerEn=0;
	ScrTriggerDi();
	GPIOPinWrite(GPIO_PORTC_BASE,Fan_Scr_Op,Fan_Scr_Op);
}

void Fan_Speed_Update(int8_t FanSpeed)
{
//	int8_t FanSpeed = 4;
	float triggerAngle = 90.0; 	//Degrees

	if(FanSpeed <= 0)
	{
		FanSpeed = 0;
		Fan_Off_Speed();
	}
	else if(FanSpeed >= MAX_FAN_SPEED && Fan_On_Off == 1 )
	{
		FanSpeed = MAX_FAN_SPEED;
		FanFullOn();
	}
	else
	{
		if(Fan_On_Off == 1)
			ScrTriggerEn();

		triggerAngle = (float)((triggerAngle*MAX_FAN_SPEED)/ FanSpeed);
		ScrTrgDelay = VtgSwingCorr(triggerAngle);
	}
	FanSpeedIndUpdate(FanSpeed);
}

float VtgSwingCorr(float triggerAngle)
{
	float tdo = 0;

	tdo = (triggerAngle*AcTimePeriod)/360.00;
//triggerAngle * AcTimePeriod *Vrms/(180.0 * 230);
	return tdo + (AcTimePeriod -2*tdo + AcTimePeriod*sin((4*3.141*tdo)/AcTimePeriod))*(Vrms-Vrms_Normal)/(Vrms_Normal*(1-cos((4*3.141*tdo)/AcTimePeriod)));
}

void FanSpeedIndUpdate(int8_t FanSpeed)
{
//Switch on required LEDs
	switch (FanSpeed)
		{
			case 5:
				GPIOPinWrite(GPIO_PORTF_BASE, Fan_5,Fan_5);
			case 4:
				GPIOPinWrite(GPIO_PORTC_BASE, Fan_4,Fan_4);
			case 3:
				GPIOPinWrite(GPIO_PORTE_BASE, Fan_3,Fan_3);
			case 2:
				GPIOPinWrite(GPIO_PORTB_BASE, Fan_2, Fan_2);
			case 1:
				GPIOPinWrite(GPIO_PORTB_BASE, Fan_1,Fan_1);
				break;
			case 0:
				break;
			default: // code to be executed if n doesn't match any cases
				break;
		}
	//Switch Off not required status LEDs
	switch (FanSpeed + 1)
		{
		    case 1:
		    	GPIOPinWrite(GPIO_PORTB_BASE, Fan_1,~Fan_1);
		    case 2:
		    	GPIOPinWrite(GPIO_PORTB_BASE, Fan_2,~Fan_2);
		    case 3:
		    	GPIOPinWrite(GPIO_PORTE_BASE, Fan_3,~Fan_3);
		    case 4:
		    	GPIOPinWrite(GPIO_PORTC_BASE, Fan_4,~Fan_4);
		    case 5:
		    	GPIOPinWrite(GPIO_PORTF_BASE, Fan_5,~Fan_5);
		    	break;
		    default: // code to be executed if n doesn't match any cases
		    	break;
		}
}

void GpioZeroCrossDetInit()
{
	  GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, ZCD);
	  GPIOPadConfigSet(GPIO_PORTF_BASE,ZCD,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPD);
	  GPIOIntTypeSet(GPIO_PORTF_BASE,ZCD,GPIO_RISING_EDGE);
	  GPIOIntRegister(GPIO_PORTF_BASE,PortFIntHandler);
	  GPIOIntEnable(GPIO_PORTF_BASE, ZCD);
}
void ScrPulseTr()
{

	TimerEnable(TIMER2_BASE, TIMER_B);
	GPIOPinWrite(GPIO_PORTC_BASE,Fan_Scr_Op,Fan_Scr_Op);
	ADCProcessorTrigger(ADC0_BASE, 3);
}

void AcVtgRms(uint32_t ADCValue)
{
	Vrms=	ADCValue;
}
void ADC1_Init()
{
    //
    // The ADC0 peripheral must be enabled for use.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC1);
    SysCtlDelay(3);
    GPIOPinTypeADC(GPIO_PORTE_BASE, AC_IN);

    ADCHardwareOversampleConfigure(ADC1_BASE, 64);

    ADCSequenceConfigure(ADC1_BASE, 3, ADC_TRIGGER_PROCESSOR, 0);
    ADCSequenceEnable(ADC1_BASE, 3);

    ADCIntEnable(ADC1_BASE, 3);
    ADCIntEnableEx(ADC1_BASE, ADC_INT_SS3);

    ADCIntRegister(ADC1_BASE, 3, Adc1IntHandler);

    ADCIntClear(ADC1_BASE, 3);
}
void Adc1IntHandler()
{
	uint32_t ADCValue;
	ADCIntClear(ADC1_BASE, 3);
    ADCSequenceDataGet(ADC0_BASE, 3,&ADCValue);

    AcVtgRms(ADCValue);
//    AnalogValue=ADCValue*3300 / 4096.0;
}
void Timer2BInit()
{
	// The Timer0 peripheral must be enabled for use.
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER2);
	// The Timer0 peripheral must be enabled for use.
	TimerConfigure(TIMER2_BASE, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_B_ONE_SHOT);
    //Load Timer value
	TimerLoadSet(TIMER2_BASE, TIMER_B, SysCtlClockGet()*SCR_TRIGGER_PW/1000);
	// Enable processor interrupts.
    IntMasterEnable();
    // Configure the Timer0A interrupt for timer timeout.
    TimerIntEnable(TIMER2_BASE, TIMER_TIMB_TIMEOUT);
    //Link ISR fn call to timer 0 interrupt
    TimerIntRegister(TIMER2_BASE,TIMER_B, Timer2BIntHandler);
    // Enable the Timer2B interrupt on the processor (NVIC).
    IntEnable(INT_TIMER2B);
    // Enable Timer0B.
    //TimerEnable(TIMER0_BASE, TIMER_B);
}

void Timer1Init()
{
	// The Timer0 peripheral must be enabled for use.
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
	// The Timer0 peripheral must be enabled for use.
	TimerConfigure(TIMER1_BASE, TIMER_CFG_ONE_SHOT);
    //Load Timer value
	TimerLoadSet(TIMER1_BASE, TIMER_A, SysCtlClockGet()*ScrTrgDelay/1000);
	// Enable processor interrupts.
    IntMasterEnable();
    // Configure the Timer0A interrupt for timer timeout.
    TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
    //Link ISR fn call to timer 0 interrupt
    TimerIntRegister(TIMER1_BASE,TIMER_A, Timer1IntHandler);
    // Enable the Timer0A interrupt on the processor (NVIC).
    IntEnable(INT_TIMER1A);
    // Enable Timer0B.
    //TimerEnable(TIMER1_BASE, TIMER_A);
}

void Timer2BIntHandler()
{
	TimerIntClear(TIMER2_BASE, TIMER_TIMB_TIMEOUT);
	TimerDisable(TIMER2_BASE, TIMER_B);
    //Load Timer value
	TimerLoadSet(TIMER2_BASE, TIMER_B, SysCtlClockGet()*SCR_TRIGGER_PW/1000);
	GPIOPinWrite(GPIO_PORTC_BASE,Fan_Scr_Op,0);
}

void Timer1IntHandler()
{
	TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
	TimerDisable(TIMER1_BASE, TIMER_A);

	ScrPulseTr();
}
void PortFIntHandler()
{
	uint32_t status=0;

	  status = GPIOIntStatus(GPIO_PORTF_BASE,true);

	  GPIOIntClear(GPIO_PORTF_BASE,status);

	  if(status & ZCD == ZCD)
	  {
		//Load Timer value
		TimerDisable(TIMER1_BASE, TIMER_A);
		TimerLoadSet(TIMER1_BASE, TIMER_A, SysCtlClockGet() *ScrTrgDelay/1000);
		TimerEnable(TIMER1_BASE, TIMER_A);
	  }
}

