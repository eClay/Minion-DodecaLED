#include "MinionDodecaLED.h"

#include "hri/dmac.h"
#include "hri/evsys.h"
#include "hri/port.h"
#include "hri/tcc.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


uint8_t* frame_buffer[2];


// typedef struct {
// //  eclay_digital_pin_t   digital_pin,
// //  processor_timer_t     timer,
// //  processor_timer_pin_t timer_pin
// } minion_dodecaled_lane_pin_t;

uint8_t dmaPatternDisable __attribute__((aligned (4))) = 0x00;
uint8_t dmaBuffer[] __attribute__ ((aligned (4))) = {
    0x5A, 0xA5, 0x5A, 0xA5, 0x5A, 0xA5, 0x5A, 0xA5, 0x5A, 0xA5,
    0x5A, 0xA5, 0x5A, 0xA5, 0x5A, 0xA5, 0x5A, 0xA5, 0x5A, 0xA5,
    0x5A, 0xA5, 0x5A, 0xA5, 0x5A, 0xA5, 0x5A, 0xA5, 0x5A, 0xA5,
    0x5A, 0xA5, 0x5A, 0xA5, 0x5A, 0xA5, 0x5A, 0xA5, 0x5A, 0xA5,
    0x5A, 0xA5, 0x5A, 0xA5, 0x5A, 0xA5, 0x5A, 0xA5, 0x5A, 0xA5,
    0x5A, 0xA5, 0x5A, 0xA5, 0x5A, 0xA5, 0x5A, 0xA5, 0x5A, 0xA5,
    0x5A, 0xA5, 0x5A, 0xA5, 0x5A, 0xA5, 0x5A, 0xA5, 0x5A, 0xA5,
    0x5A, 0xA5, 0x5A, 0xA5, 0x5A, 0xA5, 0x5A, 0xA5, 0x5A, 0xA5,
    0x5A, 0xA5, 0x5A, 0xA5, 0x5A, 0xA5, 0x5A, 0xA5, 0x5A, 0xA5,
    0x5A, 0xA5, 0x5A, 0xA5, 0x5A, 0xA5, 0x5A, 0xA5, 0x5A, 0xA5,
  };


void MINION_DODECALED_Initialize( void )
{
  HRI_TCC_Initialize( HRI_TCC_INSTANCE_TCC0, HRI_GCLK_GENERATOR_0 );
  HRI_TCC_Prescaler_Set( HRI_TCC_INSTANCE_TCC0, HRI_TCC_PRESCALE_DIV1024 );
  HRI_TCC_Waveform_Set( HRI_TCC_INSTANCE_TCC0, HRI_TCC_WAVEFORM_NORMAL_PWM );
  HRI_TCC_Period_Set( HRI_TCC_INSTANCE_TCC0, 46875 );
  HRI_TCC_Compare_Set( HRI_TCC_INSTANCE_TCC0, HRI_TCC_CHANNEL_MC0, 36875 );
  HRI_TCC_Compare_Set( HRI_TCC_INSTANCE_TCC0, HRI_TCC_CHANNEL_MC1, 10000 );
  HRI_TCC_EventOutput_Enable( HRI_TCC_INSTANCE_TCC0, HRI_TCC_EVENT_OUTPUT_MC1 );
  TCC0->WEXCTRL.bit.OTMX = 0x2;

  hri_dmac_descriptor_t* descriptor;

  HRI_DMAC_Initialize();
  HRI_DMAC_PriorityLevel_Enable( HRI_DMAC_PRIORITY_LEVEL_0 );

  HRI_DMAC_CHANNEL_TriggerAction_Set( 4, HRI_DMAC_TRIGGER_ACTION_BEAT );
  HRI_DMAC_CHANNEL_TriggerSource_Set( 4, HRI_DMAC_TRIGGER_SOURCE_TCC0_OVF );
  HRI_DMAC_CHANNEL_ArbitrationLevel_Set( 4, HRI_DMAC_PRIORITY_LEVEL_0 );
  HRI_DMAC_CHANNEL_EventOutput_Disable( 4 );
  HRI_DMAC_CHANNEL_EventInput_Disable( 4 );

  descriptor = HRI_DMAC_CHANNEL_Descriptor_Get( 4 );
  HRI_DMAC_DESCRIPTOR_SourceAddress_Set( descriptor, &dmaPatternDisable );
  HRI_DMAC_DESCRIPTOR_DestinationAddress_Set( descriptor, HRI_TCC_PatternRegisterEnable(HRI_TCC_INSTANCE_TCC0) );
  HRI_DMAC_DESCRIPTOR_SourceIncrement_Disable( descriptor );
  HRI_DMAC_DESCRIPTOR_DestinationIncrement_Disable( descriptor ); 
  HRI_DMAC_DESCRIPTOR_IncrementStepSize_Set( descriptor, HRI_DMAC_STEP_SELECTION_DESTINATION, HRI_DMAC_ADDR_INC_STEP_SIZE_X1 ); 
  HRI_DMAC_DESCRIPTOR_BeatSize_Set( descriptor, HRI_DMAC_BEAT_SIZE_1BYTE ); 
  HRI_DMAC_DESCRIPTOR_BlockAction_Set( descriptor, HRI_DMAC_BLOCK_ACTION_NONE ); 
  HRI_DMAC_DESCRIPTOR_EventOutput_Set( descriptor, HRI_DMAC_EVENT_OUTPUT_DISABLE );
  HRI_DMAC_DESCRIPTOR_BlockSize_Set( descriptor, 100 );  
  HRI_DMAC_DESCRIPTOR_NextDescriptor_Set( descriptor, NULL );
  HRI_DMAC_DESCRIPTOR_DescriptorValid_Set( descriptor );

  HRI_DMAC_CHANNEL_Enable( 4 );

  HRI_DMAC_CHANNEL_TriggerAction_Set( 0, HRI_DMAC_TRIGGER_ACTION_BEAT );
  HRI_DMAC_CHANNEL_TriggerSource_Set( 0, HRI_DMAC_TRIGGER_SOURCE_DISABLE );
  HRI_DMAC_CHANNEL_ArbitrationLevel_Set( 0, HRI_DMAC_PRIORITY_LEVEL_0 );
  HRI_DMAC_CHANNEL_EventOutput_Disable( 0 );
  HRI_DMAC_CHANNEL_EventInput_Enable( 0 );
  HRI_DMAC_CHANNEL_EventInputAction_Set( 0, HRI_DMAC_EVENT_INPUT_ACTION_TRIG );

  descriptor = HRI_DMAC_CHANNEL_Descriptor_Get( 0 );
  HRI_DMAC_DESCRIPTOR_SourceAddress_Set( descriptor, &dmaBuffer );
  HRI_DMAC_DESCRIPTOR_DestinationAddress_Set( descriptor, HRI_TCC_PatternRegisterEnable(HRI_TCC_INSTANCE_TCC0) );
  HRI_DMAC_DESCRIPTOR_SourceIncrement_Enable( descriptor );
  HRI_DMAC_DESCRIPTOR_DestinationIncrement_Disable( descriptor ); 
  HRI_DMAC_DESCRIPTOR_IncrementStepSize_Set( descriptor, HRI_DMAC_STEP_SELECTION_DESTINATION, HRI_DMAC_ADDR_INC_STEP_SIZE_X1 ); 
  HRI_DMAC_DESCRIPTOR_BeatSize_Set( descriptor, HRI_DMAC_BEAT_SIZE_1BYTE ); 
  HRI_DMAC_DESCRIPTOR_BlockAction_Set( descriptor, HRI_DMAC_BLOCK_ACTION_NONE ); 
  HRI_DMAC_DESCRIPTOR_EventOutput_Set( descriptor, HRI_DMAC_EVENT_OUTPUT_DISABLE );
  HRI_DMAC_DESCRIPTOR_BlockSize_Set( descriptor, 100 );  
  HRI_DMAC_DESCRIPTOR_NextDescriptor_Set( descriptor, NULL );
  HRI_DMAC_DESCRIPTOR_DescriptorValid_Set( descriptor );

  HRI_DMAC_CHANNEL_Enable( 0 );

  HRI_GCLK_PeripheralClock_Enable( HRI_GCLK_GENERATOR_0, HRI_GCLK_PERIPHERAL_EVSYS_CHANNEL_0, false );
  HRI_EVSYS_Initialize();
  HRI_EVSYS_User_Set( HRI_EVSYS_USER_DMAC_CH0, HRI_EVSYS_USER_CHANNEL_0 );
  HRI_EVSYS_Generator_Set( HRI_EVSYS_GENERATOR_TCC0_MCX1, 0, HRI_EVSYS_EDGE_RISING, HRI_EVSYS_PATH_RESYNCHRONIZED );

  HRI_PORT_PinMuxSet( HRI_PORT_GROUP_PA, 4, HRI_PORT_PINMUX_E );    // TCC0/WO[0]
  HRI_PORT_PinMuxSet( HRI_PORT_GROUP_PA, 5, HRI_PORT_PINMUX_E );    // TCC0/WO[1]
  HRI_PORT_PinMuxSet( HRI_PORT_GROUP_PA, 18, HRI_PORT_PINMUX_F );   // TCC0/WO[2]
  HRI_PORT_PinMuxSet( HRI_PORT_GROUP_PA, 19, HRI_PORT_PINMUX_F );   // TCC0/WO[3]
  HRI_PORT_PinMuxSet( HRI_PORT_GROUP_PA, 14, HRI_PORT_PINMUX_F );   // TCC0/WO[4]
  HRI_PORT_PinMuxSet( HRI_PORT_GROUP_PA, 15, HRI_PORT_PINMUX_F );   // TCC0/WO[5]
  HRI_PORT_PinMuxSet( HRI_PORT_GROUP_PA, 12, HRI_PORT_PINMUX_F );   // TCC0/WO[6]
  HRI_PORT_PinMuxSet( HRI_PORT_GROUP_PA, 13, HRI_PORT_PINMUX_F );   // TCC0/WO[7]

  HRI_PORT_PinMuxSet( HRI_PORT_GROUP_PA, 6, HRI_PORT_PINMUX_E );    // TCC1/WO[0]
  HRI_PORT_PinMuxSet( HRI_PORT_GROUP_PA, 7, HRI_PORT_PINMUX_E );    // TCC1/WO[1]
  HRI_PORT_PinMuxSet( HRI_PORT_GROUP_PA, 8, HRI_PORT_PINMUX_F );    // TCC1/WO[2]
  HRI_PORT_PinMuxSet( HRI_PORT_GROUP_PA, 9, HRI_PORT_PINMUX_F );    // TCC1/WO[3]

  HRI_TCC_Pattern_Set( HRI_TCC_INSTANCE_TCC0, 0x00, 0x00);

  HRI_TCC_Enable( HRI_TCC_INSTANCE_TCC0 );
}


void MINIOM_DODECALED_BANK_Buffer_Set( 
    minion_dodecaled_bank_t bank,
    uint8_t* buffer
  )
{
  if( bank >= 2 ) return;

  frame_buffer[bank] = buffer;
}

void MINION_DODECALED_BANK_Enable(
    minion_dodecaled_bank_t bank,
    minion_dodecaled_high_dma_channel_t     high_dma,
    minion_dodecaled_value_dma_channel_t    value_dma,
    minion_dodecaled_low_dma_channel_t      low_dma,
    minion_dodecaled_value_event_channel_t  value_event
  )
{

}

// void MINION_DODECALED_BANK_ShowDoneCallback_Set(
//     minion_dodecaled_bank_t bank,
//     minion_dodecaled_show_done_callback_t callback
//   )
// {

// }

void MINION_DODECALED_BANK_Show(
    minion_dodecaled_bank_t     bank
  )
{
  
}

bool MINION_DODECALED_BANK_isShowDone(
    minion_dodecaled_bank_t     bank
  )
{

}


void MINION_DODECALED_LANE_Mode_Set(
    minion_dodecaled_bank_t      bank,
    minion_dodecaled_lane_t      lane,
    minion_dodecaled_lane_mode_t mode
  )
{
  if( (bank >= MINION_DODECALED_NUM_BANKS) ||
      (lane >= MINION_DODECALED_NUM_LANES_PER_BANK) )
  {
    return;
  }

  // switch( bank )
  // {
  //   case 0:

  // }

  switch( mode )
  {
    case MINION_DODECALED_LANE_MODE_OFF:
//      PROCESSOR_PORT_PinMuxSet( , PROCESSOR_PORT_PINMUX_DISABLED )
      break;

    case MINION_DODECALED_LANE_MODE_DATA:
//      PROCESSOR_PORT_PinMuxSet( , PROCESSOR_PORT_PINMUX_DISABLED )
      break;

    case MINION_DODECALED_LANE_MODE_CLOCK:
//      PROCESSOR_PORT_PinMuxSet( , PROCESSOR_PORT_PINMUX_DISABLED )
      break;

    default:
      break;

  }
}


// void MINION_DODECALED_PIXEL_Set(
//     minion_dodecaled_bank_t     bank,
//     minion_dodecaled_lane_t     lane,
//     minion_dodecaled_pixel_t    position,
//     minion_dodecaled_value_t    value
//   )
// {

// }

// minion_dodecaled_value_t MINION_DODECALED_PIXEL_Get(
//     minion_dodecaled_bank_t     bank,
//     minion_dodecaled_lane_t     lane,
//     minion_dodecaled_pixel_t    pixel
//   )
// {
//   return 0;
// }


// HRI_TCC_INTERRUPT_HANDLER_BEGIN(TCC0)
// HRI_TCC_INTERRUPT_HANDLER_OVERFLOW(TCC0,tcc0_callback)
// HRI_TCC_INTERRUPT_HANDLER_MC0(TCC0,tcc0_callback)
// HRI_TCC_INTERRUPT_HANDLER_MC1(TCC0,tcc0_callback)
// HRI_TCC_INTERRUPT_HANDLER_END()

// HRI_TCC_INTERRUPT_HANDLER_BEGIN(TCC1)
// HRI_TCC_INTERRUPT_HANDLER_OVERFLOW(TCC1,tcc1_callback)
// HRI_TCC_INTERRUPT_HANDLER_MC0(TCC1,tcc1_callback)
// HRI_TCC_INTERRUPT_HANDLER_MC1(TCC1,tcc1_callback)
// HRI_TCC_INTERRUPT_HANDLER_END()

