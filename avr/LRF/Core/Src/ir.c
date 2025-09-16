#include "ir.h"
#include "stm32f4xx_hal.h"
#include <math.h>

extern ADC_HandleTypeDef hadc1;

volatile float ir_distance[IR_NUM] = {0};
volatile float ir0_distance=0, ir1_distance=0, ir2_distance=0, ir3_distance=0;

static uint8_t ir_dummy_mode=0;
volatile float ir_dummy_vals[IR_NUM]={10.0,20.0,30.0,40.0};

static uint8_t object_detected=0;
static uint32_t no_object_counter=0;
static uint32_t object_lost_timestamp=0;

static const uint32_t ir_channel[IR_NUM]={ADC_CHANNEL_0, ADC_CHANNEL_1, ADC_CHANNEL_4, ADC_CHANNEL_5};

static uint8_t ir_idx=0;
static uint8_t ir_reading_active=0;

#define IR_MIN_DISTANCE 10.0f
#define IR_MAX_DISTANCE 150.0f
#define IR_MAX_STEP 25.0f
#define DETECTION_THRESHOLD 100.0f
#define NO_OBJECT_CONFIRM_COUNT 10

void IR_Init(void)
{
    ir_idx=0;
    ir_reading_active=0;
    object_detected=0;
    no_object_counter=0;
    object_lost_timestamp=HAL_GetTick();

    for(int i=0;i<IR_NUM;i++) ir_distance[i]=IR_MAX_DISTANCE;
    ir0_distance=ir1_distance=ir2_distance=ir3_distance=IR_MAX_DISTANCE;
}

static void IR_StartADC(uint8_t idx)
{
    ADC_ChannelConfTypeDef sConfig={0};
    sConfig.Channel=ir_channel[idx];
    sConfig.Rank=1;
    sConfig.SamplingTime=ADC_SAMPLETIME_15CYCLES;
    HAL_ADC_ConfigChannel(&hadc1,&sConfig);
    HAL_ADC_Start_IT(&hadc1);
}

void IR_StartReadAll(void)
{
    if(!ir_dummy_mode && !ir_reading_active)
    {
        ir_reading_active=1;
        ir_idx=0;
        IR_StartADC(ir_idx);
    }
}

void IR_IRQHandler(void)
{
    float distance;
    if(ir_dummy_mode)
    {
        distance=ir_dummy_vals[ir_idx];
    }
    else
    {
        uint32_t adcValue=HAL_ADC_GetValue(&hadc1);
        float voltage=(adcValue/4095.0f)*3.3f;
        distance=(voltage>0.42f)?(27.86f/(voltage-0.42f)):IR_MAX_DISTANCE;

        if(distance<IR_MIN_DISTANCE) distance=IR_MIN_DISTANCE;
        if(distance>IR_MAX_DISTANCE) distance=IR_MAX_DISTANCE;

        float prev=ir_distance[ir_idx];
        float diff=distance-prev;
        if(diff>IR_MAX_STEP) distance=prev+IR_MAX_STEP;
        else if(diff<-IR_MAX_STEP) distance=prev-IR_MAX_STEP;

        distance=prev*0.5f+distance*0.5f;

        if(fabsf(distance-prev)<0.3f) distance=prev;
    }

    ir_distance[ir_idx]=distance;
    switch(ir_idx){case 0:ir0_distance=distance;break;case 1:ir1_distance=distance;break;case 2:ir2_distance=distance;break;case 3:ir3_distance=distance;break;}

    ir_idx++;
    if(ir_idx>=IR_NUM)
    {
        ir_idx=0;
        ir_reading_active=0;

        uint8_t any_object=0;
        for(int i=0;i<IR_NUM;i++) if(ir_distance[i]<DETECTION_THRESHOLD) any_object=1;

        if(any_object)
        {
            if(!object_detected){object_detected=1; no_object_counter=0;}
        }
        else
        {
            no_object_counter++;
            if(no_object_counter>=NO_OBJECT_CONFIRM_COUNT && object_detected)
            {
                object_detected=0;
                object_lost_timestamp=HAL_GetTick();
                no_object_counter=0;
            }
        }
    }
    else if(!ir_dummy_mode) IR_StartADC(ir_idx);
}

uint8_t IR_IsObjectDetected(void){return object_detected;}
uint32_t IR_GetObjectLostTime(void){return object_detected?0:HAL_GetTick()-object_lost_timestamp;}
float IR_GetMinDistance(void){float m=IR_MAX_DISTANCE;for(int i=0;i<IR_NUM;i++) if(ir_distance[i]<m) m=ir_distance[i]; return m;}
int8_t IR_GetObjectDirection(void){if(!object_detected)return -1; float m=IR_MAX_DISTANCE; int8_t d=-1; for(int i=0;i<IR_NUM;i++){if(ir_distance[i]<m && ir_distance[i]<DETECTION_THRESHOLD){m=ir_distance[i]; d=i;}} return d;}
void IR_SetDummyValue(uint8_t idx,float val){if(idx<IR_NUM) ir_dummy_vals[idx]=val;}
void IR_SetDummyMode(uint8_t enable){ir_dummy_mode=enable;}
void IR_ResetDetectionState(void){object_detected=0; no_object_counter=0; object_lost_timestamp=HAL_GetTick();}
