#include "adc.h"
#include "stm32f0xx_gpio.h"
#include "stm32f0xx_adc.h"
#include "stm32f0xx_rcc.h"
#include "FreeRTOS.h"
#include "task.h"

Adc adc;

Adc::Adc()
{
    
}

void Adc::init(void)
{
    RCC_ADCCLKConfig(RCC_ADCCLK_HSI14);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
        
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    ADC_InitTypeDef ADC_InitStructure;
    ADC_DeInit(ADC1);
    ADC_InitStructure.ADC_Resolution           = ADC_Resolution_12b;
    ADC_InitStructure.ADC_ContinuousConvMode   = DISABLE;
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
    ADC_InitStructure.ADC_ExternalTrigConv     = ADC_ExternalTrigConv_T2_TRGO;
    ADC_InitStructure.ADC_DataAlign            = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_ScanDirection        = ADC_ScanDirection_Upward;
    ADC_Init(ADC1, &ADC_InitStructure);
    ADC_VrefintCmd(ENABLE);
    
    ADC_ChannelConfig(ADC1, ADC_Channel_1 , ADC_SampleTime_55_5Cycles);    
    ADC_ChannelConfig(ADC1, ADC_Channel_Vrefint, ADC_SampleTime_55_5Cycles);
    
    ADC_GetCalibrationFactor(ADC1);
    ADC_Cmd(ADC1, ENABLE);
    while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_ADRDY));
    
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = ADC1_COMP_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    ADC_StartOfConversion(ADC1);
    ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);
     
    
}

void Adc::vAdcTask( void *pvParameters )
{        
    for( ;; )
    {	
        ADC_StartOfConversion(ADC1); 
        vTaskDelay(10);
        taskYIELD();
    }
}

uint16_t adcVal[2];
uint8_t ch = 0;
extern "C" void ADC1_COMP_IRQHandler()
{    
    if(ADC_GetITStatus(ADC1, ADC_IT_EOC) != RESET)
    {
        ADC_ClearITPendingBit(ADC1, ADC_IT_EOC);        
        adcVal[ch] = ADC_GetConversionValue(ADC1);
        ch++;
        if(ch > 1) ch = 0;           
    }
}