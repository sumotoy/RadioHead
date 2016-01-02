// RH_MRF89.h
//
// Definitions for Microchip MRF89XA family radios radios per:
// http://ww1.microchip.com/downloads/en/DeviceDoc/70622C.pdf
// http://ww1.microchip.com/downloads/en/DeviceDoc/75017B.pdf
//
// Author: Mike McCauley (mikem@airspayce.com)
// Copyright (C) 2015 Mike McCauley
// $Id: RH_MRF89.h,v 1.6 2015/12/17 10:58:13 mikem Exp mikem $
// 

#ifndef RH_RF95_h
#define RH_RF95_h

#include <RHNRFSPIDriver.h>

// This is the maximum number of interrupts the driver can support
// Most Arduinos can handle 2, Megas can handle more
#define RH_MRF89_NUM_INTERRUPTS 3

// Max number of octets the MRF89XA Rx/Tx FIFO can hold
#define RH_MRF89_FIFO_SIZE 64

// This is the maximum number of bytes that can be carried by the MRF89XA.
// We use some for headers, keeping fewer for RadioHead messages
#define RH_MRF89_MAX_PAYLOAD_LEN RH_MRF89_FIFO_SIZE

// The length of the headers we add.
// The headers are inside the MRF89XA payload
#define RH_MRF89_HEADER_LEN 4
    
// This is the maximum user message length that can be supported by this driver. 
// Can be pre-defined to a smaller size (to save SRAM) prior to including this header
// Here we allow for 4 bytes headers, user data. Message length and CRC are automatically encoded and decoded by 
// the MRF89XA
#ifndef RH_MRF89_MAX_MESSAGE_LEN
 #define RH_MRF89_MAX_MESSAGE_LEN (RH_MRF89_MAX_PAYLOAD_LEN - RH_MRF89_HEADER_LEN)
#endif

// Bits that must be set to do a SPI read
#define RH_MRF89_SPI_READ_MASK              0x40

// The MRF89XA crystal frequency in MHz
#define RH_MRF89_XTAL_FREQ                  12.8

#include "_registers/RH_REG_MRF89.h"

/////////////////////////////////////////////////////////////////////
/// \class RH_MRF89 RH_MRF89.h <RH_MRF89.h>
/// \brief Send and receive addressed, reliable, acknowledged datagrams by Microchip MRF89XA and compatible transceivers.
/// and modules.
///
/// The Microchip MRF89XA http://ww1.microchip.com/downloads/en/DeviceDoc/70622C.pdf is a low cost 900MHz
/// bancd transceiver chip.
/// It is commonly used on preassembled modules with supporting circcuits and antennas, such as 
/// the MRF89XAM9A http://www.microchip.com/wwwproducts/Devices.aspx?product=MRF89XAM9A
/// This class supports all such modules
///
/// This base class provides basic functions for sending and receiving unaddressed, unreliable datagrams
/// of arbitrary length to 59 octets per packet. Use one of the Manager classes to get addressing and 
/// acknowledgement reliability, routing, meshes etc.
///
/// Several MRF89XA modules can be connected to an Arduino, permitting the construction of translators
/// and frequency changers, etc. Each instance requires 2 chip select pins, and interrupt pin the standard 3 SPI pins.
/// 
/// Naturally, for any 2 radios to communicate that must be configured to use the same frequency and 
/// data rate, and with identical network addresses.
///
/// Example Arduino programs are included to show the main modes of use.
///
/// All messages sent and received by this class conform to this packet format:
///
/// - 3 octets PREAMBLE
/// - 2 to 4 octets NETWORK ADDRESS (also call Sync Word)
/// - 1 octet message length bits packet control field
/// - 4 to 63 octets PAYLOAD, consisting of:
///   - 1 octet TO header
///   - 1 octet FROM header
///   - 1 octet ID header
///   - 1 octet FLAGS header
///   - 0 to 59 octets of user message
/// - 2 octets CRC 
///
/// The payload is whitened. No Manchester encoding is used.
///
/// \par Connecting MRF89XA to Arduino
///
/// The electrical connection between the MRF89XA and the Arduino require 3.3V, the 3 x SPI pins (SCK, SDI, SDO), 
/// a 2 Chip Select pins (/CSCON and /CSDAT) and an interrupt.
///
/// Caution: the MRF89XA is a 3.3V part and is not tolerant of 5V inputs. Connecting MRF89XA directly to a 5V
/// MCU such as most Arduinos will damage the MRF89XA.
///
/// Connect the MRF89XA to most 3.3V Arduinos or Teensy 3.1 like this (use 3.3V not 5V).
/// \code
///                 Teensy       MRF89XAM9A
///               3.3V-----------VIN   (3.3V in)
///             pin D9-----------/CSDAT (data chip select in)
///          SS pin D10----------/CSCON (configuration chip select in)
///         SCK pin D13----------SCK   (SPI clock in)
///        MOSI pin D11----------SDI   (SPI Data in)
///        MISO pin D12----------SDO   (SPI data out)
///                 D2-----------IRQ1  (Interrupt 1 output)
///                              IRQ0  (Interrupt 0 output, not connected)
///                 GND----------GND   (ground in)
/// \endcode
/// You can use other pins for /CSDAT, /CSCON, IRQ1 by passing appropriate arguments to the constructor.
///
/// \par Example programs
///
/// Several example programs are provided.
///
class RH_MRF89 : public RHNRFSPIDriver
{
public:

    /// \brief Defines register configuration values for a desired modulation
    ///
    /// Defines values for various configuration fields and registers to 
    /// achieve a desired modulation speed and frequency deviation.
    typedef struct
    {
	uint8_t MODSEL;    ///< Value for MODSEL in RH_MRF89_REG_01_DMODREG
	uint8_t FDVAL;     ///< Value for FDVAL in RH_MRF89_REG_02_FDEVREG
	uint8_t BRVAL;     ///< Value for BRVAL RH_MRF89_REG_03_BRSREG
	uint8_t FILCREG;   ///< Value for PASFILV | BUTFILV in RH_MRF89_REG_10_FILCREG
	uint8_t TXIPOLFV;  ///< Value for TXIPOLFV in RH_MRF89_REG_1A_TXCONREG
    } ModemConfig;

    /// Choices for setModemConfig() for a selected subset of common
    /// data rates and frequency deviations.
    /// Rb is the data rate in kbps. Fd is the FSK Frequency deviation in kHz.
    /// These are indexes into MODEM_CONFIG_TABLE. We strongly recommend you use these symbolic
    /// definitions and not their integer equivalents: its possible that new values will be
    /// introduced in later versions (though we will try to avoid it).
    /// OOK is not yet supported.
    /// Based on sample configs in MRF89XA.h from Microchip
    typedef enum
    {
	FSK_Rb2Fd33 = 0,   ///< FSK, No Manchester, Whitened, Rb = 2kbs,    Fd = 33kHz
	FSK_Rb5Fd33,	   ///< FSK, No Manchester, Whitened, Rb = 5kbs,    Fd = 33kHz
	FSK_Rb10Fd33,	   ///< FSK, No Manchester, Whitened, Rb = 10kbs,   Fd = 33kHz
	FSK_Rb20Fd40,	   ///< FSK, No Manchester, Whitened, Rb = 20kbs,   Fd = 40kHz
	FSK_Rb40Fd80,	   ///< FSK, No Manchester, Whitened, Rb = 40kbs,   Fd = 80kHz
	FSK_Rb50Fd100,	   ///< FSK, No Manchester, Whitened, Rb = 50kbs,   Fd = 100kHz
	FSK_Rb66Fd133,	   ///< FSK, No Manchester, Whitened, Rb = 66kbs,   Fd = 133kHz
	FSK_Rb100Fd200,	   ///< FSK, No Manchester, Whitened, Rb = 100kbs,  Fd = 200kHz
	FSK_Rb200Fd200	   ///< FSK, No Manchester, Whitened, Rb = 200kbs,  Fd = 200kHz
    } ModemConfigChoice;

    /// Constructor.
    /// Constructor. You can have multiple instances, but each instance must have its own
    /// interrupt and 2 slave select pins. After constructing, you must call init() to initialise the interface
    /// and the radio module. A maximum of 3 instances can co-exist on one processor, provided there are sufficient
    /// distinct interrupt lines, one for each instance.
    /// \param[in] csconPin the Arduino pin number connected to the CSCON pin of the MRF89XA.
    /// Defaults to the normal SS pin for your Arduino (D10 for Diecimila, Uno etc, D53 for Mega, D10 for Maple)
    /// \param[in] csdatPin the Arduino pin number connected to the CSDAT pin of the MRF89XA.
    /// Defaults to 9.
    /// \param[in] interruptPin The interrupt Pin number that is connected to the IRQ1 pin of the MRF89XA.
    /// Defaults to pin 2. (IRQ0 pin of the MRF89XA does not need to be connected).
    /// \param[in] spi Pointer to the SPI interface object to use. 
    ///                Defaults to the standard Arduino hardware SPI interface
    RH_MRF89(uint8_t csconPin = SS, uint8_t csdatPin = 9, uint8_t interruptPin = 2, RHGenericSPI& spi = hardware_spi);

    /// Initialise the Driver transport hardware and software.
    /// Make sure the Driver is properly configured before calling init().
    /// \return true if initialisation succeeded.
    virtual bool    init();

    /// Prints the value of all chip registers
    /// to the Serial device if RH_HAVE_SERIAL is defined for the current platform
    /// For debugging purposes only.
    /// \return true on success
    bool printRegisters();

    /// Sets the radio into low-power sleep mode.
    /// If successful, the transport will stay in sleep mode until woken by 
    /// changing mode to idle, transmit or receive (eg by calling send(), recv(), available() etc)
    /// Caution: there is a time penalty as the radio takes a finite time to wake from sleep mode.
    /// \return true if sleep mode was successfully entered.
    virtual bool    sleep();

    /// If current mode is Rx or Tx changes it to Idle. If the transmitter or receiver is running, 
    /// disables them.
    void           setModeIdle();

    /// If current mode is Tx or Idle, changes it to Rx. 
    /// Starts the receiver in the radio.
    // the next valid packet received will cause available() to be true.
    void           setModeRx();

    /// If current mode is Rx or Idle, changes it to Rx. F
    /// Starts the transmitter in the radio.
    void           setModeTx();

    /// Sets the transmitter power output level in register RH_MRF89_REG_1A_TXCONREG.
    /// Be a good neighbour and set the lowest power level you need.
    /// After init(), the power will be set to RH_MRF89_TXOPVAL_1DBM (1dBm)
    /// The highest power available is RH_MRF89_TXOPVAL_13DBM (13dBm)
    /// Caution: In some countries you may only select certain higher power levels if you
    /// are also using frequency hopping. Make sure you are aware of the legal
    /// limitations and regulations in your region.
    /// Caution: in some countries the maximum permitted power level may depend on the Bit rate
    /// \param[in] power Transmitter power level, one of RH_MRF89_TXOPVAL*
    void           setTxPower(uint8_t power);

    /// Select one of the predefined modem configurations. If you need a modem configuration not provided 
    /// here, use setModemRegisters() with your own ModemConfig.
    /// \param[in] index The configuration choice.
    /// \return true if index is a valid choice.
    bool        setModemConfig(ModemConfigChoice index);

    /// Tests whether a new message is available
    /// from the Driver. 
    /// On most drivers, this will also put the Driver into RHModeRx mode until
    /// a message is actually received by the transport, when it will be returned to RHModeIdle.
    /// This can be called multiple times in a timeout loop
    /// \return true if a new, complete, error-free uncollected message is available to be retreived by recv()
    virtual bool    available();

    /// Turns the receiver on if it not already on.
    /// If there is a valid message available, copy it to buf and return true
    /// else return false.
    /// If a message is copied, *len is set to the length (Caution, 0 length messages are permitted).
    /// You should be sure to call this function frequently enough to not miss any messages
    /// It is recommended that you call it in your main loop.
    /// \param[in] buf Location to copy the received message
    /// \param[in,out] len Pointer to available space in buf. Set to the actual number of octets copied.
    /// \return true if a valid message was copied to buf
    virtual bool    recv(uint8_t* buf, uint8_t* len);

    /// Waits until any previous transmit packet is finished being transmitted with waitPacketSent().
    /// Then loads a message into the transmitter and starts the transmitter. Note that a message length
    /// of 0 is permitted. 
    /// \param[in] data Array of data to be sent
    /// \param[in] len Number of bytes of data to send
    /// \return true if the message length was valid and it was correctly queued for transmit
    virtual bool    send(const uint8_t* data, uint8_t len);

    /// Returns the maximum message length 
    /// available in this Driver.
    /// \return The maximum legal message length
    virtual uint8_t maxMessageLength();

    /// Sets the centre frequency in MHz.
    /// Permitted ranges are: 902.0 to 928.0 and 950.0 to 960.0 (inclusive)
    /// Caution not all freqs are supported on all modules: check your module specifications
    /// Caution: not all available and supported frequencies are legal in every country: check 
    /// Regulatory Approval eg for MRF89XAM9A (in 75015B.pdf)
    /// Australia 915.0 to 928.0
    bool            setFrequency(float centre);

    /// Sets the length of the preamble
    /// in bytes.
    /// Caution: this should be set to the same 
    /// value on all nodes in your network. Default is 4.
    /// Sets the message preamble length in RH_MRF89_REG_1E_PKTCREG
    /// \param[in] bytes Preamble length in bytes of 8 bits each.  
    void           setPreambleLength(uint8_t bytes);

    /// Sets the sync words for transmit and receive in registers RH_MRF89_REG_16_SYNCV31REG
    /// et seq.
    /// Caution: SyncWords should be set to the same 
    /// value on all nodes in your network. Nodes with different SyncWords set will never receive
    /// each others messages, so different SyncWords can be used to isolate different
    /// networks from each other. Default is { 0x69, 0x81, 0x7e, 0x96 }.
    /// Caution, sync words of 2 bytes and less do not work well with this chip.
    /// \param[in] syncWords Array of sync words, 1 to 4 octets long
    /// \param[in] len Number of sync words to set, 1 to 4.
    void            setSyncWords(const uint8_t* syncWords = NULL, uint8_t len = 0);

protected:

    /// Called automatically when a CRCOK or TXDONE interrupt occurs.
    /// Handles the interrupt.
    void handleInterrupt();

    /// Reads a single register from the MRF89XA
    /// \param[in] reg Register number, one of RH_MRF89_REG
    /// \return The value of the register
    uint8_t spiReadRegister(uint8_t reg);

    /// Writes to a single single register on the MRF89XA
    /// \param[in] reg Register number, one of RH_MRF89_REG_*
    /// \param[in] val The value to write
    /// \return the current value of RH_MRF89_REG_00_GCONREG (read while the command is sent)
    uint8_t spiWriteRegister(uint8_t reg, uint8_t val);

    /// Writes a single byte to the MRF89XA data FIFO.
    /// \param[in] data The data value to write
    /// \return 0
    uint8_t spiWriteData(uint8_t data);

    /// Write a number of bytes from a buffer to the MRF89XA data FIFO.
    /// \param[in] data Pointer to a buffer containing the len bytes to be written
    /// \param[in] len The number of bytes to write to teh FIFO
    /// \return 0;
    uint8_t spiWriteData(const uint8_t* data, uint8_t len);

    /// Reads a single byte from the MRF89XA data FIFO.
    /// \return The next data byte in the FIFO
    uint8_t spiReadData();

    /// Sets the operating mode in the CMOD bits in RH_MRF89_REG_00_GCONREG
    /// which controls what mode the MRF89XA is running in
    /// \param[in] mode One of RH_MRF89_CMOD_*
    void    setOpMode(uint8_t mode);

    /// Verifies that the MRF89XA PLL has locked on the slected frequency.
    /// This needs to be called if the frequency is changed
    bool    verifyPLLLock();

    /// Examine the revceive buffer to determine whether the message is for this node
    void validateRxBuf();

    /// Clear our local receive buffer
    void clearRxBuf();


private:
    /// Low level interrupt service routine for device connected to interrupt 0
    static void         isr0();

    /// Low level interrupt service routine for device connected to interrupt 1
    static void         isr1();

    /// Low level interrupt service routine for device connected to interrupt 1
    static void         isr2();

    /// Array of instances connected to interrupts 0 and 1
    static RH_MRF89*     _deviceForInterrupt[];

    /// Index of next interrupt number to use in _deviceForInterrupt
    static uint8_t      _interruptCount;

    // Sigh: this chip has 2 differnt chip selects.
    // We have to set one or the other as the SPI slave select pin depending
    // on which block of registers we are accessing
    uint8_t             _csconPin;
    uint8_t             _csdatPin;

    /// The configured interrupt pin connected to this instance
    uint8_t             _interruptPin;

    /// The index into _deviceForInterrupt[] for this device (if an interrupt is already allocated)
    /// else 0xff
    uint8_t             _myInterruptIndex;

    /// Number of octets in the buffer
    volatile uint8_t    _bufLen;
    
    /// The receiver/transmitter buffer
    uint8_t             _buf[RH_MRF89_MAX_PAYLOAD_LEN];

    /// True when there is a valid message in the buffer
    volatile bool       _rxBufValid;

};

/// @example mrf89_client.pde
/// @example mrf89_server.pde

#endif
