// license:BSD-3-Clause
// copyright-holders:AJR
/**********************************************************************

    This 8251-based RS232 compatible serial interface is described
    in the article "Cross-Pollinating the Apple II" by Richard
    Campbell, BYTE Volume 4 Number 4, April 1979, pages 20-25. It
    works more as a proof of concept than a full-featured peripheral.

    Rates are generated by the MM5307AA based on an 895 kHz division
    of the bus clock, since no onboard oscillator is provided. The
    resulting data rates are, as the author notes, “3 percent low,
    but in practice this is close enough.”

    Only TXD and RXD are connected to the 1488 driver and 1489
    receiver, though, as the author admits, they have enough spare
    pins that modem control signals could be easily added.

***********************************************************************

    List of integrated circuits (complete)

    No.     Type        Function
    ---     ----        --------
    IC1     74LS04      Invert read and reset signals from Apple bus
    IC2     8251        Serial-to-parallel/parallel-to-serial conversion
    IC3     74LS161     Divide 7 MHz Apple bus signal by 8
    IC4     MM5307AA    Data rate generation
    IC5     LM1488      TTL to RS232 level conversion (TXD)
    IC6     LM1489      RS232 to TTL level conversion (RXD)

**********************************************************************/

#include "emu.h"
#include "byte8251.h"

#include "bus/rs232/rs232.h"
#include "machine/i8251.h"
#include "machine/mm5307.h"


namespace {

class a2bus_byte8251_device : public device_t, public device_a2bus_card_interface
{
public:
	// construction/destruction
	a2bus_byte8251_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock);

	DECLARE_INPUT_CHANGED_MEMBER(rate_changed);

protected:
	// device-level overrides
	virtual void device_start() override ATTR_COLD;
	virtual void device_reset() override ATTR_COLD;
	virtual ioport_constructor device_input_ports() const override ATTR_COLD;
	virtual void device_add_mconfig(machine_config &config) override ATTR_COLD;

	// device_a2bus_card_interface overrides
	virtual u8 read_c0nx(u8 offset) override;
	virtual void write_c0nx(u8 offset, u8 data) override;
	virtual bool take_c800() override { return false; }

private:
	// object finders
	required_device<i8251_device> m_usart;
	required_device<mm5307_device> m_brg;
	required_ioport m_switches;
};

a2bus_byte8251_device::a2bus_byte8251_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock)
	: device_t(mconfig, A2BUS_BYTE8251, tag, owner, clock)
	, device_a2bus_card_interface(mconfig, *this)
	, m_usart(*this, "8251")
	, m_brg(*this, "brg")
	, m_switches(*this, "SWITCHES")
{
}

void a2bus_byte8251_device::device_start()
{
	// CTS grounded
	m_usart->write_cts(0);
}

void a2bus_byte8251_device::device_reset()
{
	// Update data rate
	m_brg->control_w(m_switches->read());
}

INPUT_CHANGED_MEMBER(a2bus_byte8251_device::rate_changed)
{
	m_brg->control_w(newval);
}

u8 a2bus_byte8251_device::read_c0nx(u8 offset)
{
	return m_usart->read(offset & 1);
}

void a2bus_byte8251_device::write_c0nx(u8 offset, u8 data)
{
	m_usart->write(offset & 1, data);
}

static INPUT_PORTS_START(byte8251)
	PORT_START("SWITCHES") // “A dual in line pin-type switch may be used”
	PORT_DIPNAME(0xf, 0xf, "Data Rate") PORT_DIPLOCATION("S:1,2,3,4") PORT_CHANGED_MEMBER(DEVICE_SELF, a2bus_byte8251_device, rate_changed, 0)
	PORT_DIPSETTING(0x1, "50 bps")
	PORT_DIPSETTING(0x2, "75 bps")
	PORT_DIPSETTING(0x3, "110 bps")
	PORT_DIPSETTING(0x4, "134.5 bps")
	PORT_DIPSETTING(0x5, "150 bps")
	PORT_DIPSETTING(0x6, "300 bps")
	PORT_DIPSETTING(0x7, "600 bps")
	PORT_DIPSETTING(0x8, "900 bps")
	PORT_DIPSETTING(0x9, "1200 bps")
	PORT_DIPSETTING(0xa, "1800 bps")
	PORT_DIPSETTING(0xb, "2400 bps")
	PORT_DIPSETTING(0xc, "3600 bps")
	PORT_DIPSETTING(0xd, "4800 bps")
	PORT_DIPSETTING(0xe, "7200 bps")
	PORT_DIPSETTING(0xf, "9600 bps")
INPUT_PORTS_END

ioport_constructor a2bus_byte8251_device::device_input_ports() const
{
	return INPUT_PORTS_NAME(byte8251);
}

void a2bus_byte8251_device::device_add_mconfig(machine_config &config)
{
	I8251(config, m_usart, 1021800); // CLK tied to ϕ1 signal from bus pin 38
	m_usart->txd_handler().set("rs232", FUNC(rs232_port_device::write_txd));

	MM5307AA(config, m_brg, A2BUS_7M_CLOCK / 8);
	m_brg->output_cb().set(m_usart, FUNC(i8251_device::write_txc));
	m_brg->output_cb().append(m_usart, FUNC(i8251_device::write_rxc));

	rs232_port_device &rs232(RS232_PORT(config, "rs232", default_rs232_devices, nullptr));
	rs232.rxd_handler().set(m_usart, FUNC(i8251_device::write_rxd));
}

} // anonymous namespace


// device type definition
DEFINE_DEVICE_TYPE_PRIVATE(A2BUS_BYTE8251, device_a2bus_card_interface, a2bus_byte8251_device, "a2bus_byte8251", "BYTE Serial Interface (8251 based)")