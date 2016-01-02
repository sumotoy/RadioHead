#ifndef RH_REGISTER_MRF89_h
#define RH_REGISTER_MRF89_h



// Register names from Figure 2-18
#define RH_MRF89_REG_00_GCONREG             0x00
#define RH_MRF89_REG_01_DMODREG             0x01
#define RH_MRF89_REG_02_FDEVREG             0x02
#define RH_MRF89_REG_03_BRSREG              0x03
#define RH_MRF89_REG_04_FLTHREG             0x04
#define RH_MRF89_REG_05_FIFOCREG            0x05
#define RH_MRF89_REG_06_R1CREG              0x06
#define RH_MRF89_REG_07_P1CREG              0x07
#define RH_MRF89_REG_08_S1CREG              0x08
#define RH_MRF89_REG_09_R2CREG              0x09
#define RH_MRF89_REG_0A_P2CREG              0x0a
#define RH_MRF89_REG_0B_S2CREG              0x0b
#define RH_MRF89_REG_0C_PACREG              0x0c
#define RH_MRF89_REG_0D_FTXRXIREG           0x0d
#define RH_MRF89_REG_0E_FTPRIREG            0x0e
#define RH_MRF89_REG_0F_RSTHIREG            0x0f
#define RH_MRF89_REG_10_FILCREG             0x10
#define RH_MRF89_REG_11_PFCREG              0x11
#define RH_MRF89_REG_12_SYNCREG             0x12
// Hmm the addresses of the next 2 is ambiguous in the docs
// this seems to agree with whats in the chip:
#define RH_MRF89_REG_13_RSVREG              0x13
#define RH_MRF89_REG_14_RSTSREG             0x14
#define RH_MRF89_REG_15_OOKCREG             0x15
#define RH_MRF89_REG_16_SYNCV31REG          0x16
#define RH_MRF89_REG_17_SYNCV23REG          0x17
#define RH_MRF89_REG_18_SYNCV15REG          0x18
#define RH_MRF89_REG_19_SYNCV07REG          0x19
#define RH_MRF89_REG_1A_TXCONREG            0x1a
#define RH_MRF89_REG_1B_CLKOREG             0x1b
#define RH_MRF89_REG_1C_PLOADREG            0x1c
#define RH_MRF89_REG_1D_NADDSREG            0x1d
#define RH_MRF89_REG_1E_PKTCREG             0x1e
#define RH_MRF89_REG_1F_FCRCREG             0x1f

// Register bitfield definitions
//#define RH_MRF89_REG_00_GCONREG             0x00
#define RH_MRF89_CMOD                       0xe0
#define RH_MRF89_CMOD_TRANSMIT              0x80
#define RH_MRF89_CMOD_RECEIVE               0x60
#define RH_MRF89_CMOD_FS                    0x40
#define RH_MRF89_CMOD_STANDBY               0x20
#define RH_MRF89_CMOD_SLEEP                 0x00

#define RH_MRF89_FBS                        0x18
#define RH_MRF89_FBS_950_960                0x10
#define RH_MRF89_FBS_915_928                0x08
#define RH_MRF89_FBS_902_915                0x00

#define RH_MRF89_VCOT                       0x06
#define RH_MRF89_VCOT_180MV                 0x06
#define RH_MRF89_VCOT_120MV                 0x04
#define RH_MRF89_VCOT_60MV                  0x02
#define RH_MRF89_VCOT_TANK                  0x00

#define RH_MRF89_RPS                        0x01

//#define RH_MRF89_REG_01_DMODREG             0x01
#define RH_MRF89_MODSEL                     0xc0
#define RH_MRF89_MODSEL_FSK                 0x80
#define RH_MRF89_MODSEL_OOK                 0x40

#define RH_MRF89_DMODE0                     0x20

#define RH_MRF89_OOKTYP                     0x18
#define RH_MRF89_OOKTYP_AVERAGE             0x10
#define RH_MRF89_OOKTYP_PEAK                0x08
#define RH_MRF89_OOKTYP_FIXED               0x00

#define RH_MRF89_DMODE1                     0x04

#define RH_MRF89_IFGAIN                     0x03
#define RH_MRF89_IFGAIN_M13P5               0x03
#define RH_MRF89_IFGAIN_M9                  0x02
#define RH_MRF89_IFGAIN_M4P5                0x01
#define RH_MRF89_IFGAIN_0                   0x00

// DMODE1 and DMODE1:
#define RH_MRF89_OPMODE_CONTINUOUS          0x00
#define RH_MRF89_OPMODE_BUFFER              RH_MRF89_DMODE0
#define RH_MRF89_OPMODE_PACKET              RH_MRF89_DMODE1

//#define RH_MRF89_REG_03_BRSREG              0x03
#define RH_MRF89_BRVAL                      0x7f

//#define RH_MRF89_REG_05_FIFOCREG            0x05
#define RH_MRF89_FSIZE                      0xc0
#define RH_MRF89_FSIZE_64                   0xc0
#define RH_MRF89_FSIZE_48                   0x80
#define RH_MRF89_FSIZE_32                   0x40
#define RH_MRF89_FSIZE_16                   0x00

#define RH_MRF89_FTINT                      0x3f

//#define RH_MRF89_REG_0C_PACREG              0x0c
#define RH_MRF89_PARC                       0x18
#define RH_MRF89_PARC_23                    0x18
#define RH_MRF89_PARC_15                    0x10
#define RH_MRF89_PARC_8P5                   0x08
#define RH_MRF89_PARC_3                     0x00

//#define RH_MRF89_REG_0D_FTXRXIREG           0x0d
#define RH_MRF89_IRQ0RXS                    0xc0
#define RH_MRF89_IRQ0RXS_CONT_RSSI          0x40
#define RH_MRF89_IRQ0RXS_CONT_SYNC          0x00
#define RH_MRF89_IRQ0RXS_BUFFER_SYNC        0xc0
#define RH_MRF89_IRQ0RXS_BUFFER_FIFOEMPTY   0x80
#define RH_MRF89_IRQ0RXS_BUFFER_WRITEBYTE   0x40
#define RH_MRF89_IRQ0RXS_BUFFER_NONE        0x00
#define RH_MRF89_IRQ0RXS_PACKET_SYNC        0xc0
#define RH_MRF89_IRQ0RXS_PACKET_FIFOEMPTY   0x80
#define RH_MRF89_IRQ0RXS_PACKET_WRITEBYTE   0x40
#define RH_MRF89_IRQ0RXS_PACKET_PLREADY     0x00

#define RH_MRF89_IRQ1RXS                    0x30
#define RH_MRF89_IRQ1RXS_CONT_DCLK          0x00
#define RH_MRF89_IRQ1RXS_BUFFER_FIFO_THRESH 0x30
#define RH_MRF89_IRQ1RXS_BUFFER_RSSI        0x20
#define RH_MRF89_IRQ1RXS_BUFFER_FIFOFULL    0x10
#define RH_MRF89_IRQ1RXS_BUFFER_NONE        0x00
#define RH_MRF89_IRQ1RXS_PACKET_FIFO_THRESH 0x30
#define RH_MRF89_IRQ1RXS_PACKET_RSSI        0x20
#define RH_MRF89_IRQ1RXS_PACKET_FIFOFULL    0x10
#define RH_MRF89_IRQ1RXS_PACKET_CRCOK       0x00

#define RH_MRF89_IRQ1TX                     0x08
#define RH_MRF89_FIFOFULL                   0x04
#define RH_MRF89_FIFOEMPTY                  0x02
#define RH_MRF89_FOVRUN                     0x01

//#define RH_MRF89_REG_0E_FTPRIREG            0x0e
#define RH_MRF89_FIFOFM                     0x80
#define RH_MRF89_FIFOFSC                    0x40
#define RH_MRF89_TXDONE                     0x20
#define RH_MRF89_IRQ0TXST                   0x10
#define RH_MRF89_RIRQS                      0x04
#define RH_MRF89_LSTSPLL                    0x02
#define RH_MRF89_LENPLL                     0x01

//#define RH_MRF89_REG_10_FILCREG             0x10
#define RH_MRF89_PASFILV                    0xf0
#define RH_MRF89_PASFILV_987KHZ             0xf0
#define RH_MRF89_PASFILV_676KHZ             0xe0
#define RH_MRF89_PASFILV_514KHZ             0xd0
#define RH_MRF89_PASFILV_458KHZ             0xc0
#define RH_MRF89_PASFILV_414KHZ             0xb0
#define RH_MRF89_PASFILV_378KHZ             0xa0
#define RH_MRF89_PASFILV_321KHZ             0x90
#define RH_MRF89_PASFILV_262KHZ             0x80
#define RH_MRF89_PASFILV_234KHZ             0x70
#define RH_MRF89_PASFILV_211KHZ             0x60
#define RH_MRF89_PASFILV_184KHZ             0x50
#define RH_MRF89_PASFILV_157KHZ             0x40
#define RH_MRF89_PASFILV_137KHZ             0x30
#define RH_MRF89_PASFILV_109KHZ             0x20
#define RH_MRF89_PASFILV_82KHZ              0x10
#define RH_MRF89_PASFILV_65KHZ              0x00

#define RH_MRF89_BUTFILV                    0x0f
#define RH_MRF89_BUTFILV_25KHZ              0x00
#define RH_MRF89_BUTFILV_50KHZ              0x01
#define RH_MRF89_BUTFILV_75KHZ              0x02
#define RH_MRF89_BUTFILV_100KHZ             0x03
#define RH_MRF89_BUTFILV_125KHZ             0x04
#define RH_MRF89_BUTFILV_150KHZ             0x05
#define RH_MRF89_BUTFILV_175KHZ             0x06
#define RH_MRF89_BUTFILV_200KHZ             0x07
#define RH_MRF89_BUTFILV_225KHZ             0x08
#define RH_MRF89_BUTFILV_250KHZ             0x09
#define RH_MRF89_BUTFILV_275KHZ             0x0a
#define RH_MRF89_BUTFILV_300KHZ             0x0b
#define RH_MRF89_BUTFILV_325KHZ             0x0c
#define RH_MRF89_BUTFILV_350KHZ             0x0d
#define RH_MRF89_BUTFILV_375KHZ             0x0e
#define RH_MRF89_BUTFILV_400KHZ             0x0f

//#define RH_MRF89_REG_11_PFCREG              0x11
#define RH_MRF89_POLCFV                     0xf0

//#define RH_MRF89_REG_12_SYNCREG             0x12
#define RH_MRF89_POLFILEN                   0x80
#define RH_MRF89_BSYNCEN                    0x40
#define RH_MRF89_SYNCREN                    0x20
#define RH_MRF89_SYNCWSZ                    0x18
#define RH_MRF89_SYNCWSZ_32                 0x18
#define RH_MRF89_SYNCWSZ_24                 0x10
#define RH_MRF89_SYNCWSZ_16                 0x08
#define RH_MRF89_SYNCWSZ_8                  0x00
#define RH_MRF89_SYNCTEN                    0x06
#define RH_MRF89_SYNCTEN_3                  0x06
#define RH_MRF89_SYNCTEN_2                  0x04
#define RH_MRF89_SYNCTEN_1                  0x02
#define RH_MRF89_SYNCTEN_0                  0x00

//#define RH_MRF89_REG_15_OOKCREG             0x15
#define RH_MRF89_OOTHSV                     0xe0
#define RH_MRF89_OOTHSV_6P0DB               0xe0
#define RH_MRF89_OOTHSV_5P0DB               0xc0
#define RH_MRF89_OOTHSV_4P0DB               0xa0
#define RH_MRF89_OOTHSV_3P0DB               0x80
#define RH_MRF89_OOTHSV_2P0DB               0x60
#define RH_MRF89_OOTHSV_1P5DB               0x40
#define RH_MRF89_OOTHSV_1P0DB               0x20
#define RH_MRF89_OOTHSV_0P5DB               0x00

#define RH_MRF89_OOKTHPV                    0x1c
#define RH_MRF89_OOKTHPV_16                 0x1c
#define RH_MRF89_OOKTHPV_8                  0x18
#define RH_MRF89_OOKTHPV_4                  0x14
#define RH_MRF89_OOKTHPV_2                  0x10
#define RH_MRF89_OOKTHPV_1_IN_8             0x0c
#define RH_MRF89_OOKTHPV_1_IN_4             0x08
#define RH_MRF89_OOKTHPV_1_IN_2             0x04
#define RH_MRF89_OOKTHPV_1_IN_1             0x00

#define RH_MRF89_OOKATHC                    0x03
#define RH_MRF89_OOKATHC_32PI               0x03
#define RH_MRF89_OOKATHC_8PI                0x00

//#define RH_MRF89_REG_1A_TXCONREG            0x1a
#define RH_MRF89_TXIPOLFV                   0xf0

#define RH_MRF89_TXOPVAL                    0x0e
#define RH_MRF89_TXOPVAL_M8DBM              0x0e
#define RH_MRF89_TXOPVAL_M5DBM              0x0c
#define RH_MRF89_TXOPVAL_M2DBM              0x0a
#define RH_MRF89_TXOPVAL_1DBM               0x08
#define RH_MRF89_TXOPVAL_4DBM               0x06
#define RH_MRF89_TXOPVAL_7DBM               0x04
#define RH_MRF89_TXOPVAL_10DBM              0x02
#define RH_MRF89_TXOPVAL_13DBM              0x00

//#define RH_MRF89_REG_1B_CLKOREG             0x1b
#define RH_MRF89_CLKOCNTRL                  0x80
#define RH_MRF89_CLKOFREQ                   0x7c

//#define RH_MRF89_REG_1C_PLOADREG            0x1c
#define RH_MRF89_MCHSTREN                   0x80
#define RH_MRF89_PLDPLEN                    0x7f

//#define RH_MRF89_REG_1E_PKTCREG             0x1e
#define RH_MRF89_PKTLENF                    0x80

#define RH_MRF89_PRESIZE                    0x60
#define RH_MRF89_PRESIZE_4                  0x60
#define RH_MRF89_PRESIZE_3                  0x40
#define RH_MRF89_PRESIZE_2                  0x20
#define RH_MRF89_PRESIZE_1                  0x00

#define RH_MRF89_WHITEON                    0x10
#define RH_MRF89_CHKCRCEN                   0x08

#define RH_MRF89_ADDFIL                     0x06
#define RH_MRF89_ADDFIL_NODEADDR_00_FF      0x06
#define RH_MRF89_ADDFIL_NODEADDR_00         0x04
#define RH_MRF89_ADDFIL_NODEADDR            0x02
#define RH_MRF89_ADDFIL_OFF                 0x00

#define RH_MRF89_STSCRCEN                   0x01

//#define RH_MRF89_REG_1F_FCRCREG             0x1f
#define RH_MRF89_ACFCRC                     0x80
#define RH_MRF89_FRWAXS                     0x40


#endif