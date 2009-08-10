/*                                                                  */
/* TLU_address_map_v0-2.h                                                          */
/*                                                                  */
/* Generated on Mon Aug  3 09:50:52 2009               */
/*                                                                  */
/*                                                                  */
/* Definitions for JRA1 TLU                                         */
/* Generated by script make_tlu_address_map.pl                      */
/* Do not edit by hand!                                             */
/*                                                                  */

#define TIMESTAMP_RESET_BIT   0
#define TRIGGER_COUNTER_RESET_BIT    1
#define BUFFER_POINTER_RESET_BIT   2
#define TRIGGER_FSM_RESET_BIT    3
#define BEAM_TRIGGER_FSM_RESET_BIT   4
#define DMA_CONTROLLER_RESET_BIT   5
#define TRIGGER_SCALERS_RESET_BIT   6
#define CLOCK_GEN_RESET_BIT 7


#define I2C_SDA_OUT_BIT  0
#define I2C_SDA_IN_BIT  1
#define I2C_SCL_OUT_BIT  2
#define I2C_SCL_IN_BIT  3


/* I2C bus numbers ( write to register to select ) */
#define I2C_BUS_MOTHERBOARD   3  
#define I2C_BUS_HDMI          2
#define I2C_BUS_LEMO          1
#define I2C_BUS_DISPLAY          0

/* List I2C PCA9555 devices. */
#define I2C_BUS_MOTHERBOARD_LED_IO  0
#define I2C_BUS_MOTHERBOARD_TRIGGER__ENABLE_IPSEL_IO  1
#define I2C_BUS_MOTHERBOARD_RESET_ENABLE_IO  2
#define I2C_BUS_MOTHERBOARD_FRONT_PANEL_IO  3
#define I2C_BUS_MOTHERBOARD_LCD_IO  4

#define I2C_BUS_LEMO_RELAY_IO 0

/* This is a bit of a cock-up. The PCA9555 attached to the LEDs changed address between version "b" ( = 1 ) and version "c" (= 0)  */
#define I2C_BUS_LEMO_LED_IO_VB 1
#define I2C_BUS_LEMO_LED_IO 0

#define I2C_BUS_LEMO_DAC 2

#define I2C_BUS_PMT_DAC 2

#define BUFFER_DEPTH 4096 
/* #define BUFFER_DEPTH 1024 */

#define FIRMWARE_ID 253
#define BASE_ADDRESS 8192
#define FIRMWARE_ID_ADDRESS  8192
#define DUT_BUSY_ADDRESS  8193
#define DUT_RESET_ADDRESS  8194
#define DUT_TRIGGER_ADDRESS  8195
#define DUT_MASK_ADDRESS  8196
#define TRIG_INHIBIT_ADDRESS  8197
#define RESET_REGISTER_ADDRESS  8198
#define INITIATE_READOUT_ADDRESS  8199
#define STATE_CAPTURE_ADDRESS  8200
#define TRIGGER_FSM_STATUS_ADDRESS  8201
#define BEAM_TRIGGER_FSM_STATUS_ADDRESS  8202
#define DMA_STATUS_ADDRESS  8203
#define REGISTERED_BUFFER_POINTER_ADDRESS_BASE  8204
#define REGISTERED_BUFFER_POINTER_ADDRESS_BYTES  2
#define REGISTERED_BUFFER_POINTER_ADDRESS_0  8204
#define REGISTERED_BUFFER_POINTER_ADDRESS_1  8205
#define REGISTERED_TIMESTAMP_ADDRESS_BASE  8206
#define REGISTERED_TIMESTAMP_ADDRESS_BYTES  8
#define REGISTERED_TIMESTAMP_ADDRESS_0  8206
#define REGISTERED_TIMESTAMP_ADDRESS_1  8207
#define REGISTERED_TIMESTAMP_ADDRESS_2  8208
#define REGISTERED_TIMESTAMP_ADDRESS_3  8209
#define REGISTERED_TIMESTAMP_ADDRESS_4  8210
#define REGISTERED_TIMESTAMP_ADDRESS_5  8211
#define REGISTERED_TIMESTAMP_ADDRESS_6  8212
#define REGISTERED_TIMESTAMP_ADDRESS_7  8213
#define REGISTERED_TRIGGER_COUNTER_ADDRESS_BASE  8214
#define REGISTERED_TRIGGER_COUNTER_ADDRESS_BYTES  4
#define REGISTERED_TRIGGER_COUNTER_ADDRESS_0  8214
#define REGISTERED_TRIGGER_COUNTER_ADDRESS_1  8215
#define REGISTERED_TRIGGER_COUNTER_ADDRESS_2  8216
#define REGISTERED_TRIGGER_COUNTER_ADDRESS_3  8217
#define BUFFER_POINTER_ADDRESS_BASE  8218
#define BUFFER_POINTER_ADDRESS_BYTES  2
#define BUFFER_POINTER_ADDRESS_0  8218
#define BUFFER_POINTER_ADDRESS_1  8219
#define TIMESTAMP_ADDRESS_BASE  8220
#define TIMESTAMP_ADDRESS_BYTES  8
#define TIMESTAMP_ADDRESS_0  8220
#define TIMESTAMP_ADDRESS_1  8221
#define TIMESTAMP_ADDRESS_2  8222
#define TIMESTAMP_ADDRESS_3  8223
#define TIMESTAMP_ADDRESS_4  8224
#define TIMESTAMP_ADDRESS_5  8225
#define TIMESTAMP_ADDRESS_6  8226
#define TIMESTAMP_ADDRESS_7  8227
#define TRIGGER_COUNTER_ADDRESS_BASE  8228
#define TRIGGER_COUNTER_ADDRESS_BYTES  4
#define TRIGGER_COUNTER_ADDRESS_0  8228
#define TRIGGER_COUNTER_ADDRESS_1  8229
#define TRIGGER_COUNTER_ADDRESS_2  8230
#define TRIGGER_COUNTER_ADDRESS_3  8231
#define BEAM_TRIGGER_AMASK_ADDRESS  8232
#define BEAM_TRIGGER_OMASK_ADDRESS  8233
#define BEAM_TRIGGER_VMASK_ADDRESS  8234
#define INTERNAL_TRIGGER_INTERVAL  8235
#define BEAM_TRIGGER_IN_ADDRESS  8236
#define DUT_RESET_DEBUG_ADDRESS  8237
#define DUT_DEBUG_TRIGGER_ADDRESS  8238
#define DUT_CLOCK_DEBUG_ADDRESS  8239
#define DUT_I2C_BUS_SELECT_ADDRESS  8240
#define DUT_I2C_BUS_DATA_ADDRESS  8241
#define CLOCK_SOURCE_SELECT_ADDRESS  8242
#define TRIGGER_IN0_COUNTER_BASE  8243
#define TRIGGER_IN0_COUNTER_BYTES  2
#define TRIGGER_IN0_COUNTER_0  8243
#define TRIGGER_IN0_COUNTER_1  8244
#define TRIGGER_IN1_COUNTER_BASE  8245
#define TRIGGER_IN1_COUNTER_BYTES  2
#define TRIGGER_IN1_COUNTER_0  8245
#define TRIGGER_IN1_COUNTER_1  8246
#define TRIGGER_IN2_COUNTER_BASE  8247
#define TRIGGER_IN2_COUNTER_BYTES  2
#define TRIGGER_IN2_COUNTER_0  8247
#define TRIGGER_IN2_COUNTER_1  8248
#define TRIGGER_IN3_COUNTER_BASE  8249
#define TRIGGER_IN3_COUNTER_BYTES  2
#define TRIGGER_IN3_COUNTER_0  8249
#define TRIGGER_IN3_COUNTER_1  8250
#define REGISTERED_PARTICLE_COUNTER_ADDRESS_BASE  8251
#define REGISTERED_PARTICLE_COUNTER_ADDRESS_BYTES  4
#define REGISTERED_PARTICLE_COUNTER_ADDRESS_0  8251
#define REGISTERED_PARTICLE_COUNTER_ADDRESS_1  8252
#define REGISTERED_PARTICLE_COUNTER_ADDRESS_2  8253
#define REGISTERED_PARTICLE_COUNTER_ADDRESS_3  8254
#define REGISTERED_AUX_COUNTER_ADDRESS_BASE  8255
#define REGISTERED_AUX_COUNTER_ADDRESS_BYTES  4
#define REGISTERED_AUX_COUNTER_ADDRESS_0  8255
#define REGISTERED_AUX_COUNTER_ADDRESS_1  8256
#define REGISTERED_AUX_COUNTER_ADDRESS_2  8257
#define REGISTERED_AUX_COUNTER_ADDRESS_3  8258
#define HANDSHAKE_MODE_ADDRESS  8259
#define BUFFER_STOP_MODE_ADDRESS  8260
#define WRITE_TRIGGER_BITS_MODE_ADDRESS  8261
#define TRIGGER_PATTERN_ADDRESS_BASE  8262
#define TRIGGER_PATTERN_ADDRESS_BYTES  2
#define TRIGGER_PATTERN_ADDRESS_0  8262
#define TRIGGER_PATTERN_ADDRESS_1  8263
#define AUX_PATTERN_ADDRESS_BASE  8264
#define AUX_PATTERN_ADDRESS_BYTES  2
#define AUX_PATTERN_ADDRESS_0  8264
#define AUX_PATTERN_ADDRESS_1  8265
#define STROBE_WIDTH_ADDRESS_BASE  8266
#define STROBE_WIDTH_ADDRESS_BYTES  4
#define STROBE_WIDTH_ADDRESS_0  8266
#define STROBE_WIDTH_ADDRESS_1  8267
#define STROBE_WIDTH_ADDRESS_2  8268
#define STROBE_WIDTH_ADDRESS_3  8269
#define STROBE_PERIOD_ADDRESS_BASE  8270
#define STROBE_PERIOD_ADDRESS_BYTES  4
#define STROBE_PERIOD_ADDRESS_0  8270
#define STROBE_PERIOD_ADDRESS_1  8271
#define STROBE_PERIOD_ADDRESS_2  8272
#define STROBE_PERIOD_ADDRESS_3  8273
#define STROBE_ENABLE_ADDRESS  8274
#define TRIGGER_FSM_STATUS_VALUE_ADDRESS_BASE  8275
#define TRIGGER_FSM_STATUS_VALUE_ADDRESS_BYTES  3
#define TRIGGER_FSM_STATUS_VALUE_ADDRESS_0  8275
#define TRIGGER_FSM_STATUS_VALUE_ADDRESS_1  8276
#define TRIGGER_FSM_STATUS_VALUE_ADDRESS_2  8277
#define ENABLE_DUT_VETO_ADDRESS  8278
#define ADDRESS_MAP_SIZE 87
