// license:BSD-3-Clause
// copyright-holders:Jonathan Gevaryahu, Robbbert, Miodrag Milanovic
/******************************************************************************

  An emulation for sbc8080, by @vintagechips (Den'noh-Densetsu),
  URL: https://vintagechips.wordpress.com/2018/06/24/sbc8080-cpu%E3%83%AB%E3%83%BC%E3%82%BA%E3%82%AD%E3%83%83%E3%83%88/

  It consists of 8080+8224+8228, ROM, RAM, and 8251.  It is a UART with 
  interrupt driven.
******************************************************************************/

#include "emu.h"	// for offs_t declaration
#include "cpu/z80/z80.h"
#include "sbc8080.h"
#include "interface.h"
#include "tty.h"

#include <cstdio>
#include <cstdlib>

class sbc8080_state;
static class sbc8080_state *g_sbc8080;

class sbc8080_state : public driver_device
{
public:
	sbc8080_state(const machine_config &mconfig, device_type type, const char *tag) :
		driver_device(mconfig, type, tag),
		m_maincpu(*this, "maincpu"),
		m_main_ram(*this, "main_ram")
	{
		m_tty = new tty();
		g_sbc8080 = this;
		fprintf(stderr, "sbc8080_state: constructor\n");
	}

	uint8_t uart_creg_r();
	uint8_t uart_dreg_r();
	void uart_creg_w(uint8_t data);
	void uart_dreg_w(uint8_t data);
	void display_w(offs_t offset, uint8_t data);

	void z80_mem(address_map &map) ATTR_COLD;
	void io_map(address_map &map) ATTR_COLD;
	void sbc8080(machine_config &config);

	void int_line(int state);
	void update_tty_state(uint8_t state) { m_tty->device_update(state); };

private:
	required_device<z80_device> m_maincpu;
	required_shared_ptr<uint8_t> m_main_ram;
	uint8_t m_out_data; // byte written to 0xFFFF
	uint8_t m_out_req; // byte written to 0xFFFE
	uint8_t m_out_req_last; // old value at 0xFFFE before the most recent write
	uint8_t m_out_ack; // byte written to 0xFFFC
	std::string terminate_string;
	tty *m_tty;
	int m_tty_state;

	virtual void machine_reset() override ATTR_COLD;

};

static void irq_callback(offs_t offset, uint8_t value)
{
	static uint8_t prev_input = 0xff;
	if (offset == IRQ_INPUT_DEVICE) {
		if (prev_input != value) {
		    g_sbc8080->int_line(value ? ASSERT_LINE : CLEAR_LINE);
			prev_input = value;
		}
	}
}


/******************************************************************************
 Machine Start/Reset
******************************************************************************/

void sbc8080_state::machine_reset()
{
	// zerofill
	m_out_ack = 0;
	m_out_req = 0;
	m_out_req_last = 0;
	m_out_data = 0;
	terminate_string = "";

	// program is self-modifying, so need to refresh it on each run
	memcpy(m_main_ram, sbc8080_binary, sizeof sbc8080_binary);
	// serial reset
	m_tty->set_irq_cb(irq_callback);
	m_tty->device_reset();
	fprintf(stderr, "machine_reset\n");
}


/**************************************************************
  8251 emulation
 **************************************************************/

// Data regisger/Control register
#define I8251_DR 0
#define I8251_CR 1
#define I8251_SR 1

// Status Register
#define DSR_Bit (1<<7)
#define SYNDET_Bit  (1<<6)
#define FE_Bit  (1<<5)
#define OE_Bit  (1<<4)
#define PE_Bit  (1<<3)
#define TXEMPTY_Bit (1<<2)
#define RXRDY_Bit (1<<1)
#define TXRDY_Bit (1<<0)

#if 0
static inline uint32_t uart_fr_to_i8251_SR(uart_inst_t *uart)
{
    uint32_t uart_fr = uart_get_hw(uart)->fr;
    uint32_t data = 0;
    if ((uart_fr & UART_UARTFR_TXFF_BITS) == 0) {
        data |= TXRDY_Bit;
    }
    if ((uart_fr & UART_UARTFR_RXFE_BITS) == 0) {
        data |= RXRDY_Bit;
    }
    if((uart_fr & UART_UARTFR_BUSY_BITS) == 0) {
        data |= TXEMPTY_Bit;
    }
    return data;
}
#endif

/******************************************************************************
 I/O Handlers
******************************************************************************/

uint8_t sbc8080_state::uart_creg_r()
{
	uint8_t c = 0, cc = 0;

	//m_tty->device_update(0);
	cc = m_tty->input_device_status();
	cc |= 2;
	// i8251 status register emulation
	if (cc & 1) {
		c |= RXRDY_Bit;
	}
	if (cc & 2) {
		c |= (TXRDY_Bit|TXEMPTY_Bit);
	}
	return c;
}

void sbc8080_state::uart_creg_w(uint8_t data)
{
	//fprintf(stderr, "uart_creg_w: %02x\n", data);
}

std::uint8_t sbc8080_state::uart_dreg_r()
{
	std::uint8_t ch;
	ch = m_tty->input_device_read();
	//fprintf(stderr, "[%c]", ch);
	return ch;
}

void sbc8080_state::uart_dreg_w(uint8_t data)
{
	//fprintf(stderr, "(%c)", data);
	m_tty->output_device_write(data);
}

void sbc8080_state::display_w(offs_t offset, uint8_t data)
{
	int_line(CLEAR_LINE);
}

/******************************************************************************
 Address Maps
******************************************************************************/

void sbc8080_state::z80_mem(address_map &map)
{
	map(0x0000, 0xffff).ram().share("main_ram");
}

void sbc8080_state::io_map(address_map &map)
{
	map.unmap_value_high();
	map.global_mask(0xff);
	map(0x00, 0x00).rw(FUNC(sbc8080_state::uart_dreg_r),FUNC(sbc8080_state::uart_dreg_w));
	map(0x01, 0x01).rw(FUNC(sbc8080_state::uart_creg_r),FUNC(sbc8080_state::uart_creg_w));

}


/******************************************************************************
 Input Ports
******************************************************************************/

static INPUT_PORTS_START( sbc8080 )
INPUT_PORTS_END


/******************************************************************************
 Machine Drivers
******************************************************************************/

void sbc8080_state::sbc8080(machine_config &config)
{
	/* basic machine hardware */
	//Z80(config, m_maincpu, XTAL(3'579'545));
	Z80(config, m_maincpu, XTAL(40'000'000));
	m_maincpu->set_addrmap(AS_PROGRAM, &sbc8080_state::z80_mem);
	m_maincpu->set_addrmap(AS_IO, &sbc8080_state::io_map);
	// register a hook to z80 instruction execution loop
	m_maincpu->execute_run_cb().set(*this, FUNC(sbc8080_state::update_tty_state));
}

/*
 * int handler
 */

void sbc8080_state::int_line(int state)
{
	m_maincpu->set_input_line(INPUT_LINE_IRQ0, state);
}
/******************************************************************************
 ROM Definitions
******************************************************************************/

ROM_START(sbc8080)
	ROM_REGION(0x0, "maincpu", 0)
ROM_END


/******************************************************************************
 Drivers
******************************************************************************/

/*    YEAR  NAME      PARENT      COMPAT  MACHINE   INPUT   STATE         INIT        COMPANY                         FULLNAME                            FLAGS */
COMP( 2024, sbc8080,   0,          0,      sbc8080,   sbc8080, sbc8080_state, empty_init, "VintageChips", "sbc8080 (Z80 with PIC18F47Q53)", MACHINE_NO_SOUND_HW )
