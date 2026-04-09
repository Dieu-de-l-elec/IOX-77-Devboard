/*
 *   This is the final code (V1.0) for each CH32V003 of the IOX-77 devboard  | by C.G. | https://github.com/Dieu-de-l-elec/IOX-77-Devboard/
 *   
 *   After creating a new Mounriver Studio project and selecting the chip 'CH32V003F4U', the following main.c file can be replaced with the following code (in User>main.c)  
 *   as the CH32 doesn't comes with an external oscillator, the file 'system_ch32v00x.c has to be modified, changing the code as below:
 *            //#define SYSCLK_FREQ_8MHz_HSI    8000000
 *            //#define SYSCLK_FREQ_24MHZ_HSI   HSI_VALUE
 *            #define SYSCLK_FREQ_48MHZ_HSI   48000000               // This line has to be uncommented
 *            //#define SYSCLK_FREQ_8MHz_HSE    8000000
 *            //#define SYSCLK_FREQ_24MHz_HSE   HSE_VALUE
 *            //#define SYSCLK_FREQ_48MHz_HSE   48000000              Default line that should be commented
 */
 
#include "debug.h"

#define I2C_MODE 1

#define Size 8
#define RXAdderss 0x08             // CH32_A: 0x02; CH32_B: 0x06; CH32_C: 0x08; CH32_D: 0x0A; CH32_E: 0x0C
#define TxAdderss 0x08             // CH32_A: 0x02; CH32_B: 0x06; CH32_C: 0x08; CH32_D: 0x0A; CH32_E: 0x0C 

volatile uint8_t slave_state = 0;  // 0 = start | 0xff = end
volatile uint16_t slave_recv_len = 0;
volatile uint16_t slave_send_len = 0;
volatile uint8_t cmd_recv = 0;
volatile uint8_t cmd_ready = 0;

uint8_t toggle = 0;
uint32_t ms = 0;

uint16_t ADCBuffer[9];
uint16_t rankToPin[10] = {0xFF};

void SysTick_init (u32 counter);
void SysTick_Handler (void) __attribute__ ((interrupt ("WCH-Interrupt-fast")));

typedef struct {
    uint32_t PINSTATE;
    uint32_t PREV_MS;
    uint16_t INTERVAL;
    uint8_t ENABLE;
} digitalReadPinState;

digitalReadPinState digitalSampling[] = {
    {0x00000000, 0, 0, 0},
    {0x00000000, 0, 0, 0},
    {0x00000000, 0, 0, 0},
    {0x00000000, 0, 0, 0},
    {0x00000000, 0, 0, 0},
    {0x00000000, 0, 0, 0},
    {0x00000000, 0, 0, 0},
    {0x00000000, 0, 0, 0},
    {0x00000000, 0, 0, 0},
    {0x00000000, 0, 0, 0},
    {0x00000000, 0, 0, 0},
    {0x00000000, 0, 0, 0},
    {0x00000000, 0, 0, 0},
    {0x00000000, 0, 0, 0}
};

typedef struct {
    uint32_t PREV_MS;
    uint16_t INTERVAL;
    uint8_t ENABLE;
    uint8_t INDEX;
} analogReadPinState;

analogReadPinState analogSampling[] = {
    {0, 0, 0, 0}, //  0
    {0, 0, 0, 0}, //  1
    {0, 0, 0, 0}, //  4
    {0, 0, 0, 0}, //  9
    {0, 0, 0, 0}, //  10
    {0, 0, 0, 0}, //  11
    {0, 0, 0, 0}, //  12
    {0, 0, 0, 0}  //  13
};

uint16_t analogReadingHistory[8][16] = {0};

uint8_t analogPinMap[8] = {0, 1, 4, 9, 10, 11, 12, 13};

typedef struct {
    uint8_t CHANNEL;
    uint8_t ENABLE;
    uint8_t PIN;
} channelADC;

channelADC ADCChannelEnabled[] = {
    {ADC_Channel_1, 0, 0 },
    {ADC_Channel_0, 0, 1 },
    {ADC_Channel_2, 0, 4 },
    {ADC_Channel_3, 0, 9 },
    {ADC_Channel_4, 0, 10},
    {ADC_Channel_7, 0, 11},
    {ADC_Channel_5, 0, 12},
    {ADC_Channel_6, 0, 13}
};

u8 TxData[Size] = {0};
u8 RxData[Size] = {0};
u8 RxDataBuffer[Size];

void I2C1_EV_IRQHandler (void) __attribute__ ((interrupt ("WCH-Interrupt-fast")));
void I2C1_ER_IRQHandler (void) __attribute__ ((interrupt ("WCH-Interrupt-fast")));

uint8_t PWMCapablePins[] = {0, 2, 3, 4, 9, 10, 11};

typedef struct {
    GPIO_TypeDef *PORT;
    uint16_t PIN;
} PinSetup;

uint8_t sinMap[] = {0, 3, 6, 9, 12, 15, 18, 21, 25, 28, 31, 34, 37, 40, 43, 46, 49, 53, 56, 59, 62, 65, 68, 71, 74, 77, 80, 83, 86, 89, 92, 95, 97, 100, 103, 106, 109, 112, 115, 117, 120, 123, 126, 128, 131, 134, 136, 139, 142, 144, 147, 149, 152, 155, 157, 159, 162, 164, 167, 169, 171, 174, 176, 178, 181, 183, 185, 187, 189, 191, 193, 195, 197, 199, 201, 203, 205, 207, 209, 211, 212, 214, 216, 217, 219, 221, 222, 224, 225, 227, 228, 230, 231, 232, 234, 235, 236, 237, 238, 239, 241, 242, 243, 244, 244, 245, 246, 247, 248, 249, 249, 250, 251, 251, 252, 252, 253, 253, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255};

PinSetup pinMap[] = {
    {GPIOA, GPIO_Pin_1}, //  0  T1CH2
    {GPIOA, GPIO_Pin_2}, //  1
    {GPIOC, GPIO_Pin_0}, //  2  T2CH3
    {GPIOC, GPIO_Pin_3}, //  3  T1CH3
    {GPIOC, GPIO_Pin_4}, //  4  T1CH4
    {GPIOC, GPIO_Pin_5}, //  5
    {GPIOC, GPIO_Pin_6}, //  6
    {GPIOC, GPIO_Pin_7}, //  7
    {GPIOD, GPIO_Pin_0}, //  8
    {GPIOD, GPIO_Pin_2}, //  9  T1CH1
    {GPIOD, GPIO_Pin_3}, //  10 T2CH2
    {GPIOD, GPIO_Pin_4}, //  11 T2CH1
    {GPIOD, GPIO_Pin_5}, //  12
    {GPIOD, GPIO_Pin_6}  //  13
};

void SysTick_Handler (void) {
    ms++;
    SysTick->SR = 0;
}

void IOXpinMode (uint8_t pin_number, GPIOMode_TypeDef mode) {
    if (pin_number > 13)
        return;

    GPIO_InitTypeDef GPIO_InitStructure = {0};

    RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD, ENABLE);

    GPIO_InitStructure.GPIO_Pin = pinMap[pin_number].PIN;
    GPIO_InitStructure.GPIO_Mode = mode;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_30MHz;
    GPIO_Init (pinMap[pin_number].PORT, &GPIO_InitStructure);
}

void IOXdigitalWrite (uint8_t pin_number, uint8_t bit_state) {
    if (pin_number > 13)
        return;
    if (bit_state == 1) {
        GPIO_SetBits (pinMap[pin_number].PORT, pinMap[pin_number].PIN);
    } else {
        GPIO_ResetBits (pinMap[pin_number].PORT, pinMap[pin_number].PIN);
    }
}

uint8_t isPWM (uint8_t pin_number) {
    for (uint8_t p = 0; p < 7; p++) {
        if (pin_number == PWMCapablePins[p])
            return 1;
    }
    return 0;
}

void IOXanalogWritePWM (uint8_t pin_number, uint16_t duty_cycle, uint32_t freqOUT) {

    if (isPWM (pin_number) == 0)
        return;

    if (freqOUT > 2000000)
        freqOUT = 2000000;

    if (duty_cycle > 1023)
        duty_cycle = 1023;

    uint32_t timer_clk = SystemCoreClock;  // 48MHz
    uint16_t PRSC = 0;
    uint16_t ARR = 0;
    uint16_t CCR = 0;

    PRSC = (timer_clk / (freqOUT * 65536)) + 1;
    ARR = (timer_clk / ((PRSC + 1) * freqOUT)) - 1;

    CCR = ((uint32_t)(ARR + 1) * duty_cycle) / 1023;


    if (pin_number == 2 || pin_number == 10 || pin_number == 11) {  // PC0, PD3 and PD4 are on timer 2

        RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD, ENABLE);
        RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM2, ENABLE);

        IOXpinMode (pin_number, GPIO_Mode_AF_PP);

        TIM_Cmd (TIM2, DISABLE);
        TIM_OCInitTypeDef TIM_OCInitStructure = {0};
        TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure = {0};

        TIM_TimeBaseInitStructure.TIM_Period = ARR;
        TIM_TimeBaseInitStructure.TIM_Prescaler = PRSC;
        TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
        TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
        TIM_TimeBaseInit (TIM2, &TIM_TimeBaseInitStructure);

        TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
        TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
        TIM_OCInitStructure.TIM_Pulse = CCR;
        TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

        switch (pin_number) {
        case 2:
            TIM_OC3Init (TIM2, &TIM_OCInitStructure);

            TIM_CtrlPWMOutputs (TIM2, ENABLE);
            TIM_OC3PreloadConfig (TIM2, TIM_OCPreload_Enable);
            break;
        case 10:
            TIM_OC2Init (TIM2, &TIM_OCInitStructure);

            TIM_CtrlPWMOutputs (TIM2, ENABLE);
            TIM_OC2PreloadConfig (TIM2, TIM_OCPreload_Enable);
            break;
        case 11:
            TIM_OC1Init (TIM2, &TIM_OCInitStructure);

            TIM_CtrlPWMOutputs (TIM2, ENABLE);
            TIM_OC1PreloadConfig (TIM2, TIM_OCPreload_Enable);
            break;
        }
        TIM_ARRPreloadConfig (TIM2, ENABLE);
        TIM_Cmd (TIM2, ENABLE);

    } else {

        RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD | RCC_APB2Periph_TIM1, ENABLE);

        IOXpinMode (pin_number, GPIO_Mode_AF_PP);

        TIM_Cmd (TIM1, DISABLE);
        TIM_OCInitTypeDef TIM_OCInitStructure = {0};
        TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure = {0};

        TIM_TimeBaseInitStructure.TIM_Period = ARR;
        TIM_TimeBaseInitStructure.TIM_Prescaler = PRSC;
        TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
        TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
        TIM_TimeBaseInit (TIM1, &TIM_TimeBaseInitStructure);

        TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
        TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
        TIM_OCInitStructure.TIM_Pulse = CCR;
        TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;


        switch (pin_number) {
        case 0:
            TIM_OC2Init (TIM1, &TIM_OCInitStructure);
            TIM_CtrlPWMOutputs (TIM1, ENABLE);
            TIM_OC2PreloadConfig (TIM1, TIM_OCPreload_Enable);
            break;
        case 3:
            TIM_OC3Init (TIM1, &TIM_OCInitStructure);
            TIM_CtrlPWMOutputs (TIM1, ENABLE);
            TIM_OC3PreloadConfig (TIM1, TIM_OCPreload_Enable);
            break;
        case 4:
            TIM_OC4Init (TIM1, &TIM_OCInitStructure);
            TIM_CtrlPWMOutputs (TIM1, ENABLE);
            TIM_OC4PreloadConfig (TIM1, TIM_OCPreload_Enable);
            break;
        case 9:
            TIM_OC1Init (TIM1, &TIM_OCInitStructure);
            TIM_CtrlPWMOutputs (TIM1, ENABLE);
            TIM_OC1PreloadConfig (TIM1, TIM_OCPreload_Enable);
            break;
        }
        TIM_ARRPreloadConfig (TIM1, ENABLE);
        TIM_Cmd (TIM1, ENABLE);
    }
}

uint8_t IOXdigitalRead (uint8_t pin_number) {
    if (pin_number > 13)
        return 0;
    return GPIO_ReadInputDataBit (pinMap[pin_number].PORT, pinMap[pin_number].PIN);
}

void DMA_ADC_Init (DMA_Channel_TypeDef *DMA_CHx, uint32_t peripheralAddress, uint32_t memoryAddress, uint16_t bufferSize) {
    DMA_InitTypeDef DMA_InitStructure = {0};

    RCC_AHBPeriphClockCmd (RCC_AHBPeriph_DMA1, ENABLE);

    DMA_DeInit (DMA_CHx);
    DMA_InitStructure.DMA_PeripheralBaseAddr = peripheralAddress;
    DMA_InitStructure.DMA_MemoryBaseAddr = memoryAddress;
    DMA_InitStructure.DMA_BufferSize = bufferSize;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;  // 16 bits
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;          // 16 bits
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init (DMA_CHx, &DMA_InitStructure);
}

void ADC_config() {
    uint8_t Nb = 0;
    for (int c = 0; c < 8; c++) {
        if (ADCChannelEnabled[c].ENABLE == 1) {
            Nb++;
        }
    }

    ADC_InitTypeDef ADC_InitStructure = {0};

    RCC_APB2PeriphClockCmd (RCC_APB2Periph_ADC1, ENABLE);
    RCC_ADCCLKConfig (RCC_PCLK2_Div8);

    for (int c = 0; c < 8; c++) {
        if (ADCChannelEnabled[c].ENABLE == 1) {
            IOXpinMode (ADCChannelEnabled[c].PIN, GPIO_Mode_AIN);
        }
    }

    ADC_DeInit (ADC1);
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;                         // multi-channel acquisition
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;                   // Continuous sampling
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;  // No external trigger
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel = Nb + 1;                         // Number of channels + Vref
    ADC_Init (ADC1, &ADC_InitStructure);

    ADC_RegularChannelConfig (ADC1, ADC_Channel_Vrefint, 1, ADC_SampleTime_241Cycles);

    uint8_t rank = 2;
    for (int r = 1; r < 10; r++) {
        rankToPin[r] = 0xFF;
    }
    for (int c = 0; c < 8; c++) {
        if (ADCChannelEnabled[c].ENABLE == 1) {
            ADC_RegularChannelConfig (ADC1, ADCChannelEnabled[c].CHANNEL, rank, ADC_SampleTime_241Cycles);
            rankToPin[rank - 1] = ADCChannelEnabled[c].PIN;
            rank++;
        }
    }

    ADC_DMACmd (ADC1, ENABLE);
    ADC_Cmd (ADC1, ENABLE);
    ADC_ResetCalibration (ADC1);
    while (ADC_GetResetCalibrationStatus (ADC1));
    ADC_StartCalibration (ADC1);
    while (ADC_GetCalibrationStatus (ADC1));
    DMA_ADC_Init (DMA1_Channel1, (uint32_t)&ADC1->RDATAR, (uint32_t)ADCBuffer, Nb + 1);
    DMA_Cmd (DMA1_Channel1, ENABLE);
    ADC_SoftwareStartConvCmd (ADC1, ENABLE);
}

uint8_t giveRankADC (uint8_t pin) {  // from the ADC pin, gives the rank in the buffer
    for (int r = 1; r < 9; r++) {
        if (rankToPin[r] == pin) {
            return r;
        }
    }
    return 0xFF;  // error (not found)
}

uint16_t IOXanalogRead (uint8_t pin_number) {
    return ADCBuffer[giveRankADC (pin_number)];
}

void EnterSleepMode (void) {
    RCC_APB2PeriphClockCmd (RCC_APB2Periph_TIM1, DISABLE);
    RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM2, DISABLE);
    RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOA, DISABLE);
    RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOD, DISABLE);
    RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOC, DISABLE);

    PWR_EnterSTANDBYMode (PWR_STANDBYEntry_WFI);
}

void IIC_Init (u32 bound, u16 address) {
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    I2C_InitTypeDef I2C_InitTSturcture = {0};
    NVIC_InitTypeDef NVIC_InitStructure = {0};

    RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB1PeriphClockCmd (RCC_APB1Periph_I2C1, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init (GPIOC, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init (GPIOC, &GPIO_InitStructure);

    I2C_InitTSturcture.I2C_ClockSpeed = bound;
    I2C_InitTSturcture.I2C_Mode = I2C_Mode_I2C;
    I2C_InitTSturcture.I2C_DutyCycle = I2C_DutyCycle_16_9;
    I2C_InitTSturcture.I2C_OwnAddress1 = address;
    I2C_InitTSturcture.I2C_Ack = I2C_Ack_Enable;
    I2C_InitTSturcture.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_Init (I2C1, &I2C_InitTSturcture);

    I2C_Cmd (I2C1, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel = I2C1_EV_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init (&NVIC_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = I2C1_ER_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init (&NVIC_InitStructure);

    I2C_ITConfig (I2C1, I2C_IT_BUF, ENABLE);
    I2C_ITConfig (I2C1, I2C_IT_EVT, ENABLE);
    I2C_ITConfig (I2C1, I2C_IT_ERR, ENABLE);
}

uint16_t averageAnalogReading (uint8_t pin_number) {
    uint8_t pinIndex = 0;
    for (uint8_t p = 0; p < 8; p++) {
        if (analogPinMap[p] == RxDataBuffer[1]) {
            pinIndex = p;
        }
    }
    uint32_t sum = 0;
    for (uint8_t r = 0; r < 16; r++) {
        sum += analogReadingHistory[pinIndex][r];
    }
    return (sum / 16);
}

void handleI2C() {
    if (cmd_ready == 1) {
        cmd_ready = 0;

        switch (RxDataBuffer[0]) {
        case 0x10: {  // IOX_digitalWrite()
            IOXpinMode (RxDataBuffer[1], GPIO_Mode_Out_PP);
            IOXdigitalWrite (RxDataBuffer[1], RxDataBuffer[2]);
            break;
        }
        case 0x11: {  // IOXanalogWritePWM()
            uint16_t duty_cycle = (uint16_t)RxDataBuffer[2] << 8 | (uint16_t)RxDataBuffer[3];
            uint32_t freqOUT = (uint32_t)RxDataBuffer[4] << 24 | (uint32_t)RxDataBuffer[5] << 16 | (uint32_t)RxDataBuffer[6] << 8 | (uint32_t)RxDataBuffer[7];
            IOXanalogWritePWM (RxDataBuffer[1], duty_cycle, freqOUT);
            break;
        }
        case 0x12: {
            switch (RxDataBuffer[2]) {
            case 0x01:
                IOXpinMode (RxDataBuffer[1], GPIO_Mode_IN_FLOATING);
                break;
            case 0x05:
                IOXpinMode (RxDataBuffer[1], GPIO_Mode_IPU);
                break;
            case 0x09:
                IOXpinMode (RxDataBuffer[1], GPIO_Mode_IPD);
                break;
            case 0x03:
                IOXpinMode (RxDataBuffer[1], GPIO_Mode_Out_PP);
                break;
            case 0x13:
                IOXpinMode (RxDataBuffer[1], GPIO_Mode_Out_OD);
                break;
            case 0x06:
                IOXpinMode (RxDataBuffer[1], GPIO_Mode_AF_OD);
                break;
            case 0x07:
                IOXpinMode (RxDataBuffer[1], GPIO_Mode_AF_PP);
                break;
            case 0xC0:
                for (uint8_t c = 0; c < 8; c++) {
                    if (ADCChannelEnabled[c].PIN == RxDataBuffer[1]) {
                        ADCChannelEnabled[c].ENABLE = 1;
                    }
                }
                ADC_config();
                break;
            }
            break;
        }
        case 0x13: {
            uint16_t interval = (uint16_t)RxDataBuffer[2] << 8 | (uint16_t)RxDataBuffer[3];
            digitalSampling[RxDataBuffer[1]].ENABLE = 1;
            digitalSampling[RxDataBuffer[1]].PREV_MS = ms;
            digitalSampling[RxDataBuffer[1]].INTERVAL = interval;
            break;
        }
        case 0x14: {
            uint8_t byte1 = (digitalSampling[RxDataBuffer[1]].PINSTATE >> 24) & 0xFF;
            uint8_t byte2 = (digitalSampling[RxDataBuffer[1]].PINSTATE >> 16) & 0xFF;
            uint8_t byte3 = (digitalSampling[RxDataBuffer[1]].PINSTATE >> 8) & 0xFF;
            uint8_t byte4 = (digitalSampling[RxDataBuffer[1]].PINSTATE) & 0xFF;

            TxData[0] = 0x14;             // CMD
            TxData[1] = RxDataBuffer[1];  // PIN
            TxData[2] = byte1;            // Data
            TxData[3] = byte2;            //
            TxData[4] = byte3;            //
            TxData[5] = byte4;            //
            break;
        }

        case 0x15: {
            uint16_t reading = IOXanalogRead (RxDataBuffer[1]);
            uint8_t byte1 = (reading >> 8) & 0xFF;
            uint8_t byte2 = reading & 0xFF;

            TxData[0] = 0x15;             // CMD
            TxData[1] = RxDataBuffer[1];  // PIN
            TxData[2] = byte1;            // Data
            TxData[3] = byte2;            //
            break;
        }
        case 0x16: {
            uint16_t interval = (uint16_t)RxDataBuffer[2] << 8 | (uint16_t)RxDataBuffer[3];
            uint8_t pinIndex = 0;
            for (uint8_t p = 0; p < 8; p++) {
                if (analogPinMap[p] == RxDataBuffer[1]) {
                    pinIndex = p;
                }
            }
            analogSampling[pinIndex].ENABLE = 1;
            analogSampling[pinIndex].PREV_MS = ms;
            analogSampling[pinIndex].INTERVAL = interval;
            break;
        }
        case 0x17: {
            uint16_t reading = averageAnalogReading (RxDataBuffer[1]);
            uint8_t byte1 = (reading >> 8) & 0xFF;
            uint8_t byte2 = reading & 0xFF;

            TxData[0] = 0x17;             // CMD
            TxData[1] = RxDataBuffer[1];  // PIN
            TxData[2] = byte1;            // Data
            TxData[3] = byte2;            //
            break;
        }

        case 0x18: {
            digitalSampling[RxDataBuffer[1]].ENABLE = 0;
            digitalSampling[RxDataBuffer[1]].PREV_MS = 0;
            digitalSampling[RxDataBuffer[1]].INTERVAL = 0;
            break;
        }

        case 0x19: {
            uint8_t pinIndex = 0;
            for (uint8_t p = 0; p < 8; p++) {
                if (analogPinMap[p] == RxDataBuffer[1]) {
                    pinIndex = p;
                }
            }
            analogSampling[pinIndex].ENABLE = 0;
            analogSampling[pinIndex].PREV_MS = 0;
            analogSampling[pinIndex].INTERVAL = 0;
            break;
        }

        case 0x20: {
            for (uint8_t c = 0; c < 8; c++) {
                if (ADCChannelEnabled[c].PIN == RxDataBuffer[1]) {
                    ADCChannelEnabled[c].ENABLE = 0;
                }
            }
            ADC_config();
        }

        case 0xAA: {
            EnterSleepMode();
            break;
        }
        }
    }
}

void IOXSinPulseWrite (uint8_t pin_number, uint32_t freqOUT, uint8_t amplitude, uint32_t pulse_duration) {
    uint32_t delay = pulse_duration * 1000 / 256;
    for (uint8_t i = 0; i < 128; i++) {
        IOXanalogWritePWM (pin_number, ((uint16_t)sinMap[i] * 4 * amplitude) / 255, freqOUT);
        Delay_Us (delay);
        handleI2C();
    }
    for (uint8_t i = 0; i < 128; i++) {
        IOXanalogWritePWM (pin_number, ((uint16_t)sinMap[127 - i] * 4 * amplitude) / 255, freqOUT);
        Delay_Us (delay);
        handleI2C();
    }
}

void digitalRead_sample() {
    for (uint8_t p = 0; p <= 13; p++) {
        if (digitalSampling[p].ENABLE == 1 && (ms - digitalSampling[p].PREV_MS) >= digitalSampling[p].INTERVAL) {
            digitalSampling[p].PREV_MS += digitalSampling[p].INTERVAL;
            uint8_t newBit = IOXdigitalRead (p);
            digitalSampling[p].PINSTATE = digitalSampling[p].PINSTATE << 1;
            digitalSampling[p].PINSTATE = digitalSampling[p].PINSTATE | newBit;
        }
    }
}

void analogRead_sample() {
    for (uint8_t p = 0; p < 8; p++) {
        if (analogSampling[p].ENABLE == 1 && (ms - analogSampling[p].PREV_MS) >= analogSampling[p].INTERVAL) {
            analogSampling[p].PREV_MS += analogSampling[p].INTERVAL;
            analogReadingHistory[p][analogSampling[p].INDEX] = IOXanalogRead (analogPinMap[p]);
            analogSampling[p].INDEX = (analogSampling[p].INDEX + 1) % 16;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main (void) {
    SystemCoreClockUpdate();
    Delay_Init();
    USART_Printf_Init (115200);
    SysTick_init (48000);
    ADC_config();
    IIC_Init (400000, RXAdderss);

    while (1) {
        uint32_t initTime = ms;
        for (uint8_t i = 0; i < 255; i++) {
            handleI2C();
            digitalRead_sample();
            analogRead_sample();
        }
        uint32_t endTime = ms;
        printf ("t: %d \n ", endTime - initTime);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void I2C1_EV_IRQHandler (void) {
    if (I2C_GetITStatus (I2C1, I2C_IT_ADDR) != RESET) {
        if (I2C_GetFlagStatus (I2C1, I2C_FLAG_TRA)) {
            slave_send_len = 0;
        } else {
            ((void)I2C_ReadRegister (I2C1, I2C_Register_STAR2));
        }
    }

    else if (I2C_GetITStatus (I2C1, I2C_IT_RXNE) != RESET) {
        RxData[slave_recv_len] = I2C_ReceiveData (I2C1);
        slave_recv_len++;
        cmd_recv = 1;
    }

    else if (I2C_GetITStatus (I2C1, I2C_IT_STOPF) != RESET) {
        I2C1->CTLR1 &= I2C1->CTLR1;
        ((void)(I2C1->STAR1));
        for (int i = 0; i < Size; i++) {
            RxDataBuffer[i] = RxData[i];
        }
        cmd_ready = 1;
        cmd_recv = 0;
        slave_recv_len = 0;
        slave_send_len = 0;
    }

    else if (I2C_GetITStatus (I2C1, I2C_IT_TXE) != RESET) {
        if (slave_send_len < Size) {
            I2C_SendData (I2C1, TxData[slave_send_len]);
            slave_send_len++;
        } else {
            I2C_SendData (I2C1, TxData[Size - 1]);
        }
    }

    else {
        // err
    }
}

void I2C1_ER_IRQHandler (void) {
    if (I2C_GetITStatus (I2C1, I2C_IT_AF)) {
        I2C_ClearITPendingBit (I2C1, I2C_IT_AF);
        slave_state = 0xff;
        slave_send_len = 0;
    } else {
        // err
    }
}

void SysTick_init (u32 counter) {
    NVIC_EnableIRQ (SysTicK_IRQn);
    SysTick->SR &= ~(1 << 0);
    SysTick->CMP = (counter - 1);
    SysTick->CNT = 0;
    SysTick->CTLR = 0x000F;
}
