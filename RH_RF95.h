// RH_RF95.h
//
// Definitions for HopeRF LoRa radios per:
// http://www.hoperf.com/upload/rf/RFM95_96_97_98W.pdf
// http://www.hoperf.cn/upload/rfchip/RF96_97_98.pdf
//
// Author: Mike McCauley (mikem@airspayce.com)
// Copyright (C) 2014 Mike McCauley
// $Id: RH_RF95.h,v 1.9 2015/12/16 04:55:33 mikem Exp $
// 

#ifndef RH_RF95_h
#define RH_RF95_h

#include <RHSPIDriver.h>

// This is the maximum number of interrupts the driver can support
// Most Arduinos can handle 2, Megas can handle more
#define RH_RF95_NUM_INTERRUPTS 3

// Max number of octets the LORA Rx/Tx FIFO can hold
#define RH_RF95_FIFO_SIZE 255

// This is the maximum number of bytes that can be carried by the LORA.
// We use some for headers, keeping fewer for RadioHead messages
#define RH_RF95_MAX_PAYLOAD_LEN RH_RF95_FIFO_SIZE

// The length of the headers we add.
// The headers are inside the LORA's payload
#define RH_RF95_HEADER_LEN 4

// This is the maximum message length that can be supported by this driver. 
// Can be pre-defined to a smaller size (to save SRAM) prior to including this header
// Here we allow for 1 byte message length, 4 bytes headers, user data and 2 bytes of FCS
#ifndef RH_RF95_MAX_MESSAGE_LEN
 #define RH_RF95_MAX_MESSAGE_LEN (RH_RF95_MAX_PAYLOAD_LEN - RH_RF95_HEADER_LEN)
#endif

// The crystal oscillator frequency of the module
#define RH_RF95_FXOSC 32000000.0

// The Frequency Synthesizer step = RH_RF95_FXOSC / 2^^19
#define RH_RF95_FSTEP  (RH_RF95_FXOSC / 524288)

#include "_registers/RH_REG_RF95.h"

/////////////////////////////////////////////////////////////////////
/// \class RH_RF95 RH_RF95.h <RH_RF95.h>
/// \brief Driver to send and receive unaddressed, unreliable datagrams via a LoRa 
/// capable radio transceiver.
///
/// For Semtech SX1276/77/78/79 and HopeRF RF95/96/97/98 and other similar LoRa capable radios.
/// Based on http://www.hoperf.com/upload/rf/RFM95_96_97_98W.pdf
/// and http://www.hoperf.cn/upload/rfchip/RF96_97_98.pdf
/// and http://www.semtech.com/images/datasheet/LoraDesignGuide_STD.pdf
/// and http://www.semtech.com/images/datasheet/sx1276.pdf
/// and http://www.semtech.com/images/datasheet/sx1276_77_78_79.pdf
/// FSK/GFSK/OOK modes are not (yet) supported.
///
/// Works with
/// - the excellent MiniWirelessLoRa from Anarduino http://www.anarduino.com/miniwireless
/// - The excellent Modtronix inAir4 http://modtronix.com/inair4.html 
/// and inAir9 modules http://modtronix.com/inair9.html.
///
/// \par Overview
///
/// This class provides basic functions for sending and receiving unaddressed, 
/// unreliable datagrams of arbitrary length to 251 octets per packet.
///
/// Manager classes may use this class to implement reliable, addressed datagrams and streams, 
/// mesh routers, repeaters, translators etc.
///
/// Naturally, for any 2 radios to communicate that must be configured to use the same frequency and 
/// modulation scheme.
///
/// This Driver provides an object-oriented interface for sending and receiving data messages with Hope-RF
/// RFM95/96/97/98(W), Semtech SX1276/77/78/79 and compatible radio modules in LoRa mode.
///
/// The Hope-RF (http://www.hoperf.com) RFM95/96/97/98(W) and Semtech SX1276/77/78/79 is a low-cost ISM transceiver
/// chip. It supports FSK, GFSK, OOK over a wide range of frequencies and
/// programmable data rates, and it also supports the proprietary LoRA (Long Range) mode, which
/// is the only mode supported in this RadioHead driver.
///
/// This Driver provides functions for sending and receiving messages of up
/// to 251 octets on any frequency supported by the radio, in a range of
/// predefined Bandwidths, Spreading Factors and Coding Rates.  Frequency can be set with
/// 61Hz precision to any frequency from 240.0MHz to 960.0MHz. Caution: most modules only support a more limited
/// range of frequencies due to antenna tuning.
///
/// Up to 2 modules can be connected to an Arduino (3 on a Mega),
/// permitting the construction of translators and frequency changers, etc.
///
/// Support for other features such as transmitter power control etc is
/// also provided.
///
/// Tested on MinWirelessLoRa with arduino-1.0.5
/// on OpenSuSE 13.1. 
/// Also tested with Teensy3.1, Modtronix inAir4 and Arduino 1.6.5 on OpenSuSE 13.1
///
/// \par Packet Format
///
/// All messages sent and received by this RH_RF95 Driver conform to this packet format:
///
/// - LoRa mode:
/// - 8 symbol PREAMBLE
/// - Explicit header with header CRC (handled internally by the radio)
/// - 4 octets HEADER: (TO, FROM, ID, FLAGS)
/// - 0 to 251 octets DATA 
/// - CRC (handled internally by the radio)
///
/// \par Connecting RFM95/96/97/98 and Semtech SX1276/77/78/79 to Arduino
///
/// We tested with Anarduino MiniWirelessLoRA, which is an Arduino Duemilanove compatible with a RFM96W
/// module on-board. Therefore it needs no connections other than the USB
/// programming connection and an antenna to make it work.
///
/// If you have a bare RFM95/96/97/98 that you want to connect to an Arduino, you
/// might use these connections (untested): CAUTION: you must use a 3.3V type
/// Arduino, otherwise you will also need voltage level shifters between the
/// Arduino and the RFM95.  CAUTION, you must also ensure you connect an
/// antenna.
/// 
/// \code
///                 Arduino      RFM95/96/97/98
///                 GND----------GND   (ground in)
///                 3V3----------3.3V  (3.3V in)
/// interrupt 0 pin D2-----------DIO0  (interrupt request out)
///          SS pin D10----------NSS   (CS chip select in)
///         SCK pin D13----------SCK   (SPI clock in)
///        MOSI pin D11----------MOSI  (SPI Data in)
///        MISO pin D12----------MISO  (SPI Data out)
/// \endcode
/// With these connections, you can then use the default constructor RH_RF95().
/// You can override the default settings for the SS pin and the interrupt in
/// the RH_RF95 constructor if you wish to connect the slave select SS to other
/// than the normal one for your Arduino (D10 for Diecimila, Uno etc and D53
/// for Mega) or the interrupt request to other than pin D2 (Caution,
/// different processors have different constraints as to the pins available
/// for interrupts).
///
/// You can connect a Modtronix inAir4 or inAir9 directly to a 3.3V part such as a Teensy 3.1 like
/// this (tested).
/// \code
///                 Teensy      inAir4 inAir9
///                 GND----------GND   (ground in)
///                 3V3----------3.3V  (3.3V in)
/// interrupt 0 pin D2-----------D00   (interrupt request out)
///          SS pin D10----------CS    (CS chip select in)
///         SCK pin D13----------CK    (SPI clock in)
///        MOSI pin D11----------SI    (SPI Data in)
///        MISO pin D12----------SO    (SPI Data out)
/// \endcode
/// With these connections, you can then use the default constructor RH_RF95().
/// you must also set the transmitter power with useRFO:
/// driver.setTxPower(13, true);
///
/// Note that if you are using Modtronix inAir4 or inAir9,or any other module which uses the
/// transmitter RFO pins and not the PA_BOOST pins
/// that you must configure the power transmitter power for -1 to 14 dBm and with useRFO true. 
/// Failure to do that will result in extremely low transmit powers.
///
/// If you have an Arduino Zero, you should note that you cannot use Pin 2 for the interrupt line 
/// (Pin 2 is for the NMI only), instead you can use any other pin (we use Pin 3) and initialise RH_RF69 like this:
/// \code
/// // Slave Select is pin 10, interrupt is Pin 3
/// RH_RF69 driver(10, 3);
/// \endcode
///
/// It is possible to have 2 or more radios connected to one Arduino, provided
/// each radio has its own SS and interrupt line (SCK, SDI and SDO are common
/// to all radios)
///
/// Caution: on some Arduinos such as the Mega 2560, if you set the slave
/// select pin to be other than the usual SS pin (D53 on Mega 2560), you may
/// need to set the usual SS pin to be an output to force the Arduino into SPI
/// master mode.
///
/// Caution: Power supply requirements of the RFM module may be relevant in some circumstances: 
/// RFM95/96/97/98 modules are capable of pulling 120mA+ at full power, where Arduino's 3.3V line can
/// give 50mA. You may need to make provision for alternate power supply for
/// the RFM module, especially if you wish to use full transmit power, and/or you have
/// other shields demanding power. Inadequate power for the RFM is likely to cause symptoms such as:
/// - reset's/bootups terminate with "init failed" messages
/// - random termination of communication after 5-30 packets sent/received
/// - "fake ok" state, where initialization passes fluently, but communication doesn't happen
/// - shields hang Arduino boards, especially during the flashing
///
/// \par Interrupts
///
/// The RH_RF95 driver uses interrupts to react to events in the RFM module,
/// such as the reception of a new packet, or the completion of transmission
/// of a packet.  The RH_RF95 driver interrupt service routine reads status from
/// and writes data to the the RFM module via the SPI interface. It is very
/// important therefore, that if you are using the RH_RF95 driver with another
/// SPI based deviced, that you disable interrupts while you transfer data to
/// and from that other device.  Use cli() to disable interrupts and sei() to
/// reenable them.
///
/// \par Memory
///
/// The RH_RF95 driver requires non-trivial amounts of memory. The sample
/// programs all compile to about 8kbytes each, which will fit in the
/// flash proram memory of most Arduinos. However, the RAM requirements are
/// more critical. Therefore, you should be vary sparing with RAM use in
/// programs that use the RH_RF95 driver.
///
/// It is often hard to accurately identify when you are hitting RAM limits on Arduino. 
/// The symptoms can include:
/// - Mysterious crashes and restarts
/// - Changes in behaviour when seemingly unrelated changes are made (such as adding print() statements)
/// - Hanging
/// - Output from Serial.print() not appearing
///
/// \par Range
///
/// We have made some simple range tests under the following conditions:
/// - rf95_client base station connected to a VHF discone antenna at 8m height above ground
/// - rf95_server mobile connected to 17.3cm 1/4 wavelength antenna at 1m height, no ground plane.
/// - Both configured for 13dBm, 434MHz, Bw = 125 kHz, Cr = 4/8, Sf = 4096chips/symbol, CRC on. Slow+long range
/// - Minimum reported RSSI seen for successful comms was about -91
/// - Range over flat ground through heavy trees and vegetation approx 2km.
/// - At 20dBm (100mW) otherwise identical conditions approx 3km.
/// - At 20dBm, along salt water flat sandy beach, 3.2km.
///
/// It should be noted that at this data rate, a 12 octet message takes 2 seconds to transmit.
///
/// At 20dBm (100mW) with Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on. 
/// (Default medium range) in the conditions described above.
/// - Range over flat ground through heavy trees and vegetation approx 2km.
/// 
/// \par Transmitter Power
///
/// You can control the transmitter power on the RF transceiver
/// with the RH_RF95::setTxPower() function. The argument can be any of
/// +5 to +23 (for modules that use PA_BOOST)
/// -1 to +14 (for modules that use RFO transmitter pin)
/// The default is 13. Eg:
/// \code
/// driver.setTxPower(10); // use PA_BOOST transmitter pin
/// driver.setTxPower(10, true); // use PA_RFO pin transmitter pin
/// \endcode
///
/// We have made some actual power measurements against
/// programmed power for Anarduino MiniWirelessLoRa (which has RFM96W-433Mhz installed)
/// - MiniWirelessLoRa RFM96W-433Mhz, USB power
/// - 30cm RG316 soldered direct to RFM96W module ANT and GND
/// - SMA connector
/// - 12db attenuator
/// - SMA connector
/// - MiniKits AD8307 HF/VHF Power Head (calibrated against Rohde&Schwartz 806.2020 test set)
/// - Tektronix TDS220 scope to measure the Vout from power head
/// \code
/// Program power           Measured Power
///    dBm                         dBm
///      5                           5
///      7                           7
///      9                           8
///     11                          11
///     13                          13
///     15                          15
///     17                          16
///     19                          18
///     20                          20 
///     21                          21 
///     22                          22 
///     23                          23 
/// \endcode
///
/// We have also measured the actual power output from a Modtronix inAir4 http://modtronix.com/inair4.html
/// connected to a Teensy 3.1:
/// Teensy 3.1 this is a 3.3V part, connected directly to:
/// Modtronix inAir4 with SMA antenna connector, connected as above:
/// 10cm SMA-SMA cable
/// - MiniKits AD8307 HF/VHF Power Head (calibrated against Rohde&Schwartz 806.2020 test set)
/// - Tektronix TDS220 scope to measure the Vout from power head
/// \code
/// Program power           Measured Power
///    dBm                         dBm
///      -1                         0
///      1                          2
///      3                          4
///      5                          7
///      7                         10
///      9                         13
///     11                         14.2
///     13                         15
///     14                         16
/// \endcode
/// (Caution: we dont claim laboratory accuracy for these power measurements)
/// You would not expect to get anywhere near these powers to air with a simple 1/4 wavelength wire antenna.
class RH_RF95 : public RHSPIDriver
{
public:
    /// \brief Defines register values for a set of modem configuration registers
    ///
    /// Defines register values for a set of modem configuration registers
    /// that can be passed to setModemRegisters() if none of the choices in
    /// ModemConfigChoice suit your need setModemRegisters() writes the
    /// register values from this structure to the appropriate registers
    /// to set the desired spreading factor, coding rate and bandwidth
    typedef struct
    {
	uint8_t    reg_1d;   ///< Value for register RH_RF95_REG_1D_MODEM_CONFIG1
	uint8_t    reg_1e;   ///< Value for register RH_RF95_REG_1E_MODEM_CONFIG2
	uint8_t    reg_26;   ///< Value for register RH_RF95_REG_26_MODEM_CONFIG3
    } ModemConfig;
  
    /// Choices for setModemConfig() for a selected subset of common
    /// data rates. If you need another configuration,
    /// determine the necessary settings and call setModemRegisters() with your
    /// desired settings. It might be helpful to use the LoRa calculator mentioned in 
    /// http://www.semtech.com/images/datasheet/LoraDesignGuide_STD.pdf
    /// These are indexes into MODEM_CONFIG_TABLE. We strongly recommend you use these symbolic
    /// definitions and not their integer equivalents: its possible that new values will be
    /// introduced in later versions (though we will try to avoid it).
    typedef enum
    {
	Bw125Cr45Sf128 = 0,	   ///< Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on. Default medium range
	Bw500Cr45Sf128,	           ///< Bw = 500 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on. Fast+short range
	Bw31_25Cr48Sf512,	   ///< Bw = 31.25 kHz, Cr = 4/8, Sf = 512chips/symbol, CRC on. Slow+long range
	Bw125Cr48Sf4096,           ///< Bw = 125 kHz, Cr = 4/8, Sf = 4096chips/symbol, CRC on. Slow+long range
    } ModemConfigChoice;

    /// Constructor. You can have multiple instances, but each instance must have its own
    /// interrupt and slave select pin. After constructing, you must call init() to initialise the interface
    /// and the radio module. A maximum of 3 instances can co-exist on one processor, provided there are sufficient
    /// distinct interrupt lines, one for each instance.
    /// \param[in] slaveSelectPin the Arduino pin number of the output to use to select the RH_RF22 before
    /// accessing it. Defaults to the normal SS pin for your Arduino (D10 for Diecimila, Uno etc, D53 for Mega, D10 for Maple)
    /// \param[in] interruptPin The interrupt Pin number that is connected to the RFM DIO0 interrupt line. 
    /// Defaults to pin 2, as required by Anarduino MinWirelessLoRa module.
    /// Caution: You must specify an interrupt capable pin.
    /// On many Arduino boards, there are limitations as to which pins may be used as interrupts.
    /// On Leonardo pins 0, 1, 2 or 3. On Mega2560 pins 2, 3, 18, 19, 20, 21. On Due and Teensy, any digital pin.
    /// On other Arduinos pins 2 or 3. 
    /// See http://arduino.cc/en/Reference/attachInterrupt for more details.
    /// On Chipkit Uno32, pins 38, 2, 7, 8, 35.
    /// On other boards, any digital pin may be used.
    /// \param[in] spi Pointer to the SPI interface object to use. 
    ///                Defaults to the standard Arduino hardware SPI interface
    RH_RF95(uint8_t slaveSelectPin = SS, uint8_t interruptPin = 2, RHGenericSPI& spi = hardware_spi);
  
    /// Initialise the Driver transport hardware and software.
    /// Make sure the Driver is properly configured before calling init().
    /// \return true if initialisation succeeded.
    virtual bool    init();

    /// Prints the value of all chip registers
    /// to the Serial device if RH_HAVE_SERIAL is defined for the current platform
    /// For debugging purposes only.
    /// \return true on success
    bool printRegisters();

    /// Sets all the registered required to configure the data modem in the RF95/96/97/98, including the bandwidth, 
    /// spreading factor etc. You can use this to configure the modem with custom configurations if none of the 
    /// canned configurations in ModemConfigChoice suit you.
    /// \param[in] config A ModemConfig structure containing values for the modem configuration registers.
    void           setModemRegisters(const ModemConfig* config);

    /// Select one of the predefined modem configurations. If you need a modem configuration not provided 
    /// here, use setModemRegisters() with your own ModemConfig.
    /// \param[in] index The configuration choice.
    /// \return true if index is a valid choice.
    bool        setModemConfig(ModemConfigChoice index);

    /// Tests whether a new message is available
    /// from the Driver. 
    /// On most drivers, this will also put the Driver into RHModeRx mode until
    /// a message is actually received by the transport, when it wil be returned to RHModeIdle.
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

    /// Sets the length of the preamble
    /// in bytes. 
    /// Caution: this should be set to the same 
    /// value on all nodes in your network. Default is 8.
    /// Sets the message preamble length in RH_RF95_REG_??_PREAMBLE_?SB
    /// \param[in] bytes Preamble length in bytes.  
    void           setPreambleLength(uint16_t bytes);

    /// Returns the maximum message length 
    /// available in this Driver.
    /// \return The maximum legal message length
    virtual uint8_t maxMessageLength();

    /// Sets the transmitter and receiver 
    /// centre frequency.
    /// \param[in] centre Frequency in MHz. 137.0 to 1020.0. Caution: RFM95/96/97/98 comes in several
    /// different frequency ranges, and setting a frequency outside that range of your radio will probably not work
    /// \return true if the selected frquency centre is within range
    bool        setFrequency(float centre);

    /// If current mode is Rx or Tx changes it to Idle. If the transmitter or receiver is running, 
    /// disables them.
    void           setModeIdle();

    /// If current mode is Tx or Idle, changes it to Rx. 
    /// Starts the receiver in the RF95/96/97/98.
    void           setModeRx();

    /// If current mode is Rx or Idle, changes it to Rx. F
    /// Starts the transmitter in the RF95/96/97/98.
    void           setModeTx();

    /// Sets the transmitter power output level, and configures the transmitter pin.
    /// Be a good neighbour and set the lowest power level you need.
    /// Some SX1276/77/78/79 and compatible modules (such as RFM95/96/97/98) 
    /// use the PA_BOOST transmitter pin for high power output (and optionally the PA_DAC)
    /// while some (such as the Modtronix inAir4 and inAir9) 
    /// use the RFO transmitter pin for lower power but higher efficiency.
    /// You must set the appropriate power level and useRFO argument for your module.
    /// Check with your module manufacturer which transmtter pin is used on your module
    /// to ensure you are setting useRFO correctly. 
    /// Failure to do so will result in very low 
    /// transmitter power output.
    /// Caution: legal power limits may apply in certain countries.
    /// After init(), the power will be set to 13dBm, with useRFO false (ie PA_BOOST enabled).
    /// \param[in] power Transmitter power level in dBm. For RFM95/96/97/98 LORA with useRFO false, 
    /// valid values are from +5 to +23.
    /// For Modtronix inAir4 and inAir9 with useRFO true (ie RFO pins in use), 
    /// valid values are from -1 to 14.
    /// \param[in] useRFO If true, enables the use of the RFO transmitter pins instead of
    /// the PA_BOOST pin (false). Choose the correct setting for your module.
    void           setTxPower(int8_t power, bool useRFO = false);

    /// Sets the radio into low-power sleep mode.
    /// If successful, the transport will stay in sleep mode until woken by 
    /// changing mode it idle, transmit or receive (eg by calling send(), recv(), available() etc)
    /// Caution: there is a time penalty as the radio takes a finite time to wake from sleep mode.
    /// \return true if sleep mode was successfully entered.
    virtual bool    sleep();

protected:
    /// This is a low level function to handle the interrupts for one instance of RH_RF95.
    /// Called automatically by isr*()
    /// Should not need to be called by user code.
    void           handleInterrupt();

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
    static RH_RF95*     _deviceForInterrupt[];

    /// Index of next interrupt number to use in _deviceForInterrupt
    static uint8_t      _interruptCount;

    /// The configured interrupt pin connected to this instance
    uint8_t             _interruptPin;

    /// The index into _deviceForInterrupt[] for this device (if an interrupt is already allocated)
    /// else 0xff
    uint8_t             _myInterruptIndex;

    /// Number of octets in the buffer
    volatile uint8_t    _bufLen;
    
    /// The receiver/transmitter buffer
    uint8_t             _buf[RH_RF95_MAX_PAYLOAD_LEN];

    /// True when there is a valid message in the buffer
    volatile bool       _rxBufValid;
};

/// @example rf95_client.pde
/// @example rf95_server.pde
/// @example rf95_reliable_datagram_client.pde
/// @example rf95_reliable_datagram_server.pde

#endif

