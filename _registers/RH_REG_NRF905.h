#ifndef RH_REGISTER_NRF905_h
#define RH_REGISTER_NRF905_h


// Register names
#define RH_NRF905_REG_MASK                   0x0f
#define RH_NRF905_REG_W_CONFIG               0x00
#define RH_NRF905_REG_R_CONFIG               0x10
#define RH_NRF905_REG_W_TX_PAYLOAD           0x20
#define RH_NRF905_REG_R_TX_PAYLOAD           0x21
#define RH_NRF905_REG_W_TX_ADDRESS           0x22
#define RH_NRF905_REG_R_TX_ADDRESS           0x23
#define RH_NRF905_REG_R_RX_PAYLOAD           0x24
#define RH_NRF905_REG_CHANNEL_CONFIG         0x80

// Configuration register
#define RH_NRF905_CONFIG_0                    0x00
#define RH_NRF905_CONFIG_0_CH_NO              0xff

#define RH_NRF905_CONFIG_1                    0x01
#define RH_NRF905_CONFIG_1_AUTO_RETRAN        0x20
#define RH_NRF905_CONFIG_1_RX_RED_PWR         0x10
#define RH_NRF905_CONFIG_1_PA_PWR             0x0c
#define RH_NRF905_CONFIG_1_PA_PWR_N10DBM      0x00
#define RH_NRF905_CONFIG_1_PA_PWR_N2DBM       0x04
#define RH_NRF905_CONFIG_1_PA_PWR_6DBM        0x08
#define RH_NRF905_CONFIG_1_PA_PWR_10DBM       0x0c
#define RH_NRF905_CONFIG_1_HFREQ_PLL          0x02
#define RH_NRF905_CONFIG_1_CH_NO              0x01

#define RH_NRF905_CONFIG_2                    0x02
#define RH_NRF905_CONFIG_2_TX_AFW             0x70
#define RH_NRF905_CONFIG_2_RX_AFW             0x07

#define RH_NRF905_CONFIG_3                    0x03
#define RH_NRF905_CONFIG_3_RX_PW              0x3f

#define RH_NRF905_CONFIG_4                    0x04
#define RH_NRF905_CONFIG_4_TX_PW              0x3f

#define RH_NRF905_CONFIG_5                    0x05
#define RH_NRF905_CONFIG_5_RX_ADDRESS         0xff

#define RH_NRF905_CONFIG_6                    0x06
#define RH_NRF905_CONFIG_6_RX_ADDRESS         0xff

#define RH_NRF905_CONFIG_7                    0x07
#define RH_NRF905_CONFIG_7_RX_ADDRESS         0xff

#define RH_NRF905_CONFIG_8                    0x08
#define RH_NRF905_CONFIG_8_RX_ADDRESS         0xff

#define RH_NRF905_CONFIG_9                    0x09
#define RH_NRF905_CONFIG_9_CRC_MODE_16BIT     0x80
#define RH_NRF905_CONFIG_9_CRC_EN             0x40
#define RH_NRF905_CONFIG_9_XOF                0x38
#define RH_NRF905_CONFIG_9_XOF_4MHZ           0x00
#define RH_NRF905_CONFIG_9_XOF_8MHZ           0x08
#define RH_NRF905_CONFIG_9_XOF_12MHZ          0x10
#define RH_NRF905_CONFIG_9_XOF_16MHZ          0x18
#define RH_NRF905_CONFIG_9_XOF_20MHZ          0x20
#define RH_NRF905_CONFIG_9_UP_CLK_EN          0x04
#define RH_NRF905_CONFIG_9_UP_CLK_FREQ        0x03
#define RH_NRF905_CONFIG_9_UP_CLK_FREQ_4MHZ   0x00
#define RH_NRF905_CONFIG_9_UP_CLK_FREQ_2MHZ   0x01
#define RH_NRF905_CONFIG_9_UP_CLK_FREQ_1MHZ   0x02
#define RH_NRF905_CONFIG_9_UP_CLK_FREQ_500KHZ 0x03

// Status register is always read as first byte
#define RH_NRF905_STATUS_AM                   0x80
#define RH_NRF905_STATUS_DR                   0x20

#endif