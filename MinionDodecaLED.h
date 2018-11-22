#ifndef ECLAY_MINION_DODECALED_H
#define ECLAY_MINION_DODECALED_H

// TODO: Change to eClay level when ready. Direct access to processor level for now.
#include "hri/dmac.h"
#include "hri/evsys.h"

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif


#define DEFINE_MINION_DODECALED_BUFFER(name, length, bpp) \
  COMPILER_ALINGED(4) \
  uint8_t* name[((length)*(bpp))];

#define MINION_DODECALED_NUM_BANKS            (2)
#define MINION_DODECALED_NUM_LANES_PER_BANK   (8)

typedef uint8_t   minion_dodecaled_bank_t;
typedef uint8_t   minion_dodecaled_lane_t;
typedef uint16_t  minion_dodecaled_position_t;
typedef uint32_t  minion_dodecaled_value_t;

typedef uint32_t  minion_dodecaled_bit_frequency_t;
typedef uint8_t   minion_dodecaled_low_dutycycle_t;
typedef uint8_t   minion_dodecaled_high_dutycycle_t;

typedef enum {
  MINION_DODECALED_LANE_MODE_OFF = 0,
  MINION_DODECALED_LANE_MODE_DATA,
  MINION_DODECALED_LANE_MODE_CLOCK,
} minion_dodecaled_lane_mode_t;

typedef enum {
  MINION_DODECALED_HIGH_DMA_CHANNEL_0  = 0x0,
  MINION_DODECALED_HIGH_DMA_CHANNEL_1  = 0x1,
  MINION_DODECALED_HIGH_DMA_CHANNEL_2  = 0x2,
  MINION_DODECALED_HIGH_DMA_CHANNEL_3  = 0x3,
  MINION_DODECALED_HIGH_DMA_CHANNEL_4  = 0x4,
  MINION_DODECALED_HIGH_DMA_CHANNEL_5  = 0x5,
  MINION_DODECALED_HIGH_DMA_CHANNEL_6  = 0x6,
  MINION_DODECALED_HIGH_DMA_CHANNEL_7  = 0x7,
  MINION_DODECALED_HIGH_DMA_CHANNEL_8  = 0x8,
  MINION_DODECALED_HIGH_DMA_CHANNEL_9  = 0x9,
  MINION_DODECALED_HIGH_DMA_CHANNEL_10 = 0xA,
  MINION_DODECALED_HIGH_DMA_CHANNEL_11 = 0xB
} minion_dodecaled_high_dma_channel_t;

// On SAMD21, only first 4 DMA channels are available for use with event system
typedef enum {
  MINION_DODECALED_VALUE_DMA_CHANNEL_0 = 0x0,
  MINION_DODECALED_VALUE_DMA_CHANNEL_1 = 0x1,
  MINION_DODECALED_VALUE_DMA_CHANNEL_2 = 0x2,
  MINION_DODECALED_VALUE_DMA_CHANNEL_3 = 0x3,
} minion_dodecaled_value_dma_channel_t;

typedef enum {
  MINION_DODECALED_LOW_DMA_CHANNEL_0  = 0x0,
  MINION_DODECALED_LOW_DMA_CHANNEL_1  = 0x1,
  MINION_DODECALED_LOW_DMA_CHANNEL_2  = 0x2,
  MINION_DODECALED_LOW_DMA_CHANNEL_3  = 0x3,
  MINION_DODECALED_LOW_DMA_CHANNEL_4  = 0x4,
  MINION_DODECALED_LOW_DMA_CHANNEL_5  = 0x5,
  MINION_DODECALED_LOW_DMA_CHANNEL_6  = 0x6,
  MINION_DODECALED_LOW_DMA_CHANNEL_7  = 0x7,
  MINION_DODECALED_LOW_DMA_CHANNEL_8  = 0x8,
  MINION_DODECALED_LOW_DMA_CHANNEL_9  = 0x9,
  MINION_DODECALED_LOW_DMA_CHANNEL_10 = 0xA,
  MINION_DODECALED_LOW_DMA_CHANNEL_11 = 0xB
} minion_dodecaled_low_dma_channel_t;

typedef enum {
  MINION_DODECALED_VALUE_EVENT_CHANNEL_0  = 0x0,
  MINION_DODECALED_VALUE_EVENT_CHANNEL_1  = 0x1,
  MINION_DODECALED_VALUE_EVENT_CHANNEL_2  = 0x2,
  MINION_DODECALED_VALUE_EVENT_CHANNEL_3  = 0x3,
  MINION_DODECALED_VALUE_EVENT_CHANNEL_4  = 0x4,
  MINION_DODECALED_VALUE_EVENT_CHANNEL_5  = 0x5,
  MINION_DODECALED_VALUE_EVENT_CHANNEL_6  = 0x6,
  MINION_DODECALED_VALUE_EVENT_CHANNEL_7  = 0x7,
  MINION_DODECALED_VALUE_EVENT_CHANNEL_8  = 0x8,
  MINION_DODECALED_VALUE_EVENT_CHANNEL_9  = 0x9,
  MINION_DODECALED_VALUE_EVENT_CHANNEL_10 = 0xA,
  MINION_DODECALED_VALUE_EVENT_CHANNEL_11 = 0xB
} minion_dodecaled_value_event_channel_t;


void MINION_DODECALED_Initialize( void );


void MINIOM_DODECALED_BANK_Buffer_Set( 
    minion_dodecaled_bank_t bank,
    uint8_t* buffer
  );

void MINION_DODECALED_BANK_DMAConfiguration_Set(
    minion_dodecaled_bank_t bank,
    minion_dodecaled_high_dma_channel_t     high_dma,
    minion_dodecaled_value_dma_channel_t    value_dma,
    minion_dodecaled_low_dma_channel_t      low_dma,
    minion_dodecaled_value_event_channel_t  value_event
  );

// void MINION_DODECALED_BANK_ShowDoneCallback_Set(
//     minion_dodecaled_bank_t bank,
//     minion_dodecaled_show_done_callback_t callback
//   );

void MINION_DODECALED_BANK_Show(
    minion_dodecaled_bank_t     bank
  );

bool MINION_DODECALED_BANK_isShowDone(
    minion_dodecaled_bank_t     bank
  );


void MINION_DODECALED_LANE_Mode_Set(
    minion_dodecaled_bank_t      bank,
    minion_dodecaled_lane_t      lane,
    minion_dodecaled_lane_mode_t mode
  );


// void MINION_DODECALED_PIXEL_Set(
//     minion_dodecaled_bank_t     bank,
//     minion_dodecaled_lane_t     lane,
//     minion_dodecaled_pixel_t    position,
//     minion_dodecaled_value_t    value
//   );

// minion_dodecaled_value_t MINION_DODECALED_PIXEL_Get(
//     minion_dodecaled_bank_t     bank,
//     minion_dodecaled_lane_t     lane,
//     minion_dodecaled_pixel_t    pixel
//   );


#ifdef __cplusplus
}
#endif

#endif
