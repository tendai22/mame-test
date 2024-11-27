// license:BSD-3-Clause
// copyright-holders:Jonathan Gevaryahu, Robbbert, Miodrag Milanovic
/******************************************************************************

  An emulation for sbc8080, by @vintagechips (Den'noh-Densetsu),
  URL: https://vintagechips.wordpress.com/2018/06/24/sbc8080-cpu%E3%83%AB%E3%83%BC%E3%82%BA%E3%82%AD%E3%83%83%E3%83%88/

  It consists of 8080+8224+8228, ROM, RAM, and 8251.  It is a UART with 
  interrupt driven.
******************************************************************************/

#include "emu.h"
#include "cpu/z80/z80.h"
#include "sbc8080.h"
#include "interface.h"
#include "tty.h"

#include <cstdio>
#include <cstdlib>

class sbc8080_state : public driver_device
{
public:
	sbc8080_state(const machine_config &mconfig, device_type type, const char *tag) :
		driver_device(mconfig, type, tag),
		m_maincpu(*this, "maincpu"),
		m_main_ram(*this, "main_ram")
	{
		m_tty = new tty();
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
	void update_tty_state(uint8_t state) { m_tty->update_tty_status(state); };

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
	m_tty->input_device_reset();
	m_tty->output_device_reset();
	fprintf(stderr, "machine_reset\n");
	int_line(ASSERT_LINE);

}


/******************************************************************************
 I/O Handlers
******************************************************************************/

uint8_t sbc8080_state::uart_creg_r()
{
	// spit out the byte in out_byte if out_req is not equal to out_req_last
	uint8_t c;

	m_tty->output_device_update();
	m_tty->update_user_input();

	c = m_tty->input_device_status();
	c |= 2;
	//fprintf(stderr, "[%d]", c);
	return c;
}

void sbc8080_state::uart_creg_w(uint8_t data)
{
	fprintf(stderr, "uart_creg_w: %02x\n", data);
}

std::uint8_t sbc8080_state::uart_dreg_r()
{
	std::uint8_t ch;
	m_tty->output_device_update();
	m_tty->update_user_input();
	ch = m_tty->input_device_read();
	return ch;
}

void sbc8080_state::uart_dreg_w(uint8_t data)
{
	//if (data < 0x20) {
    //    fprintf(stderr, "[%02x]", data);
	//}
	m_tty->output_device_write(data);
	m_tty->output_device_update();
	m_tty->update_user_input();
}

void sbc8080_state::display_w(offs_t offset, uint8_t data)
{
	fprintf(stderr, "io_w: %04x %02x\n", offset, data);
	int_line(CLEAR_LINE);
}

/******************************************************************************
 Address Maps
******************************************************************************/

void sbc8080_state::z80_mem(address_map &map)
{
	map(0x0000, 0xdfff).ram().share("main_ram");
	//map(0xe000, 0xe000).rw(FUNC(sbc8080_state::uart_dreg_r), FUNC(sbc8080_state::uart_dreg_w));
	//map(0xe001, 0xe001).rw(FUNC(sbc8080_state::uart_creg_r), FUNC(sbc8080_state::uart_creg_w));
}

void sbc8080_state::io_map(address_map &map)
{
	map.unmap_value_high();
	map.global_mask(0xff);
	map(0x20, 0x25).w(FUNC(sbc8080_state::display_w));

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
	Z80(config, m_maincpu, XTAL(3'579'545));
	//Z80(config, m_maincpu, XTAL(40'000'000));
	m_maincpu->set_addrmap(AS_PROGRAM, &sbc8080_state::z80_mem);
	m_maincpu->set_addrmap(AS_IO, &sbc8080_state::io_map);
	// hook
	m_maincpu->execute_run_cb().set(*this, FUNC(sbc8080_state::update_tty_state));
}

/*
 * int handler
 */

void sbc8080_state::int_line(int state)
{
	fprintf(stderr, "(I%d)", state);
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
