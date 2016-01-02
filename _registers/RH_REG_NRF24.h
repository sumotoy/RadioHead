#ifndef RH_REGISTER_NRF24_h
#define RH_REGISTER_NRF24_h

// SPI Command names
#define RH_NRF24_COMMAND_R_REGISTER                        0x00
#define RH_NRF24_COMMAND_W_REGISTER                        0x20
#define RH_NRF24_COMMAND_ACTIVATE                          0x50 // only on RFM73 ?
#define RH_NRF24_COMMAND_R_RX_PAYLOAD                      0x61
#define RH_NRF24_COMMAND_W_TX_PAYLOAD                      0xa0
#define RH_NRF24_COMMAND_FLUSH_TX                          0xe1
#define RH_NRF24_COMMAND_FLUSH_RX                          0xe2
#define RH_NRF24_COMMAND_REUSE_TX_PL                       0xe3
#define RH_NRF24_COMMAND_R_RX_PL_WID                       0x60
#define RH_NRF24_COMMAND_W_ACK_PAYLOAD(pipe)               (0xa8|(pipe&0x7))
#define RH_NRF24_COMMAND_W_TX_PAYLOAD_NOACK                0xb0
#define RH_NRF24_COMMAND_NOP                               0xff

// Register names
#define RH_NRF24_REGISTER_MASK                             0x1f
#define RH_NRF24_REG_00_CONFIG                             0x00
#define RH_NRF24_REG_01_EN_AA                              0x01
#define RH_NRF24_REG_02_EN_RXADDR                          0x02
#define RH_NRF24_REG_03_SETUP_AW                           0x03
#define RH_NRF24_REG_04_SETUP_RETR                         0x04
#define RH_NRF24_REG_05_RF_CH                              0x05
#define RH_NRF24_REG_06_RF_SETUP                           0x06
#define RH_NRF24_REG_07_STATUS                             0x07
#define RH_NRF24_REG_08_OBSERVE_TX                         0x08
#define RH_NRF24_REG_09_RPD                                0x09
#define RH_NRF24_REG_0A_RX_ADDR_P0                         0x0a
#define RH_NRF24_REG_0B_RX_ADDR_P1                         0x0b
#define RH_NRF24_REG_0C_RX_ADDR_P2                         0x0c
#define RH_NRF24_REG_0D_RX_ADDR_P3                         0x0d
#define RH_NRF24_REG_0E_RX_ADDR_P4                         0x0e
#define RH_NRF24_REG_0F_RX_ADDR_P5                         0x0f
#define RH_NRF24_REG_10_TX_ADDR                            0x10
#define RH_NRF24_REG_11_RX_PW_P0                           0x11
#define RH_NRF24_REG_12_RX_PW_P1                           0x12
#define RH_NRF24_REG_13_RX_PW_P2                           0x13
#define RH_NRF24_REG_14_RX_PW_P3                           0x14
#define RH_NRF24_REG_15_RX_PW_P4                           0x15
#define RH_NRF24_REG_16_RX_PW_P5                           0x16
#define RH_NRF24_REG_17_FIFO_STATUS                        0x17
#define RH_NRF24_REG_1C_DYNPD                              0x1c
#define RH_NRF24_REG_1D_FEATURE                            0x1d

// These register masks etc are named wherever possible
// corresponding to the bit and field names in the nRF24L01 Product Specification
// #define RH_NRF24_REG_00_CONFIG                             0x00
#define RH_NRF24_MASK_RX_DR                                0x40
#define RH_NRF24_MASK_TX_DS                                0x20
#define RH_NRF24_MASK_MAX_RT                               0x10
#define RH_NRF24_EN_CRC                                    0x08
#define RH_NRF24_CRCO                                      0x04
#define RH_NRF24_PWR_UP                                    0x02
#define RH_NRF24_PRIM_RX                                   0x01

// #define RH_NRF24_REG_01_EN_AA                              0x01
#define RH_NRF24_ENAA_P5                                   0x20
#define RH_NRF24_ENAA_P4                                   0x10
#define RH_NRF24_ENAA_P3                                   0x08
#define RH_NRF24_ENAA_P2                                   0x04
#define RH_NRF24_ENAA_P1                                   0x02
#define RH_NRF24_ENAA_P0                                   0x01

// #define RH_NRF24_REG_02_EN_RXADDR                          0x02
#define RH_NRF24_ERX_P5                                    0x20
#define RH_NRF24_ERX_P4                                    0x10
#define RH_NRF24_ERX_P3                                    0x08
#define RH_NRF24_ERX_P2                                    0x04
#define RH_NRF24_ERX_P1                                    0x02
#define RH_NRF24_ERX_P0                                    0x01

// #define RH_NRF24_REG_03_SETUP_AW                           0x03
#define RH_NRF24_AW_3_BYTES                                0x01
#define RH_NRF24_AW_4_BYTES                                0x02
#define RH_NRF24_AW_5_BYTES                                0x03

// #define RH_NRF24_REG_04_SETUP_RETR                         0x04
#define RH_NRF24_ARD                                       0xf0
#define RH_NRF24_ARC                                       0x0f

// #define RH_NRF24_REG_05_RF_CH                              0x05
#define RH_NRF24_RF_CH                                     0x7f

// #define RH_NRF24_REG_06_RF_SETUP                           0x06
#define RH_NRF24_CONT_WAVE                                 0x80
#define RH_NRF24_RF_DR_LOW                                 0x20
#define RH_NRF24_PLL_LOCK                                  0x10
#define RH_NRF24_RF_DR_HIGH                                0x08
#define RH_NRF24_PWR                                       0x06
#define RH_NRF24_PWR_m18dBm                                0x00
#define RH_NRF24_PWR_m12dBm                                0x02
#define RH_NRF24_PWR_m6dBm                                 0x04
#define RH_NRF24_PWR_0dBm                                  0x06
#define RH_NRF24_LNA_HCURR                                 0x01

// #define RH_NRF24_REG_07_STATUS                             0x07
#define RH_NRF24_RX_DR                                     0x40
#define RH_NRF24_TX_DS                                     0x20
#define RH_NRF24_MAX_RT                                    0x10
#define RH_NRF24_RX_P_NO                                   0x0e
#define RH_NRF24_STATUS_TX_FULL                            0x01

// #define RH_NRF24_REG_08_OBSERVE_TX                         0x08
#define RH_NRF24_PLOS_CNT                                  0xf0
#define RH_NRF24_ARC_CNT                                   0x0f

// #define RH_NRF24_REG_09_RPD                                0x09
#define RH_NRF24_RPD                                       0x01

// #define RH_NRF24_REG_17_FIFO_STATUS                        0x17
#define RH_NRF24_TX_REUSE                                  0x40
#define RH_NRF24_TX_FULL                                   0x20
#define RH_NRF24_TX_EMPTY                                  0x10
#define RH_NRF24_RX_FULL                                   0x02
#define RH_NRF24_RX_EMPTY                                  0x01

// #define RH_NRF24_REG_1C_DYNPD                              0x1c
#define RH_NRF24_DPL_ALL                                   0x3f
#define RH_NRF24_DPL_P5                                    0x20
#define RH_NRF24_DPL_P4                                    0x10
#define RH_NRF24_DPL_P3                                    0x08
#define RH_NRF24_DPL_P2                                    0x04
#define RH_NRF24_DPL_P1                                    0x02
#define RH_NRF24_DPL_P0                                    0x01

// #define RH_NRF24_REG_1D_FEATURE                            0x1d
#define RH_NRF24_EN_DPL                                    0x04
#define RH_NRF24_EN_ACK_PAY                                0x02
#define RH_NRF24_EN_DYN_ACK                                0x01

#endif