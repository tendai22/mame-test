// license:BSD-3-Clause
// copyright-holders:Joakim Larsson Edstrom
/*
 * vme.cpp
 *
 * The Versabus-E was standardized as the VME bus by VITA 1981 for Europe
 * in the single or double Euroboard form factor. Several standard revs has
 * been approved since then up until recently and the VME64 revision.
 *
 * This bus driver starts with Versabus and VME rev C.
 *   http://bitsavers.informatik.uni-stuttgart.de/pdf/motorola/versabus/M68KVBS_VERSAbus_Specification_Manual_Jul81.pdf
 *
 * Acronymes from the specification
 * ---------------------------------
 * BACKPLANE  - A printed circuit board which provides the interconnection path
                between other printed circuit cards.
   SLOT       - A single position at which a card may be inserted into the backplane.
                One slot may consist of more than one edge connector.
   BOARD/CARD - Interchangeable terms representing one printed circuit board capable
                of being inserted into the backplane and containing a collection of
                electronic components.
   MODULE     - A collection of electronic components with a single functional
                purpose. More than one module may exist on the same card, but one
                module should never be spread over multiple cards.
   MASTER     - A functional module capable of initiating data bus transfers.
   REQUESTER  - A functional module capable of requesting control of the data
                transfer bus.
   INTERRUPT  - A functional module capable of detecting interrupt requests
   HANDLER      and initiating appropriate responses.
   MASTER     - The combination of a MASTER, REQUESTER, INTERRUPT HANDLER, and
   SUB-SYSTEM   (optionally) an INTERRUPTER, which function together and which
                must be on the same card.

   NOTE! All MASTERS, REQUESTERS, and INTERRUPT HANDLERS must be pieces of a
         MASTER SUB-SYSTEM.

   SLAVE       - A functional module capable of responding to data transfer
                 operations generated by a MASTER.
   INTERRUPTER - A functional module capable of requesting service from a MASTER
                 SUB-SYSTEM by generating an interrupt request.
   SLAVE       - The combination of a SLAVE and INTERRUPTER which function together
   SUB-SYSTEM    and which must be on the same card.

   NOTE! All INTERRUPTERS must be part of either SLAVE SUB-SYSTEMS or MASTER
         SUB-SYSTEMS. However, SLAVES may exist as stand-alone elements.
         Such SLAVES will never be called SLAVE SUB-SYSTEMS.

   CONTROLLER  - The combination of modules used to provide utility and emergency
   SUB-SYSTEM    signals for the VERSAbus. There will always be one and only one
                 CONTROLLER SUB-SYSTEM. It can contain the following functional
                 modules:

                 a. Data Transfer Bus ARBITER
                 b. Emergency Data Transfer Bus REQUESTER
                 c. Power up/power down MASTER
                 d. System clock driver
                 e. System reset driver
                 f. System test controller
                 g. Power monitor (for AC clock and AC fail driver)

    In any VERSAbus system, only one each of the above functional modules will exist.
    The slot numbered Al is designated as the controller sub-system slot because the
    user will typically provide modules a through d on the board residing in this
    slot. System reset and the system test  controller are typically connected to
    an operator control panel and may be located elsewhere. The power monitor is
    interfaced to the incoming AC power source and may also be located remotely.
*/

/*
 * TODO:
 *  - bus arbitration
 *  - block, rmw and address-only cycles
 *  - utility bus (serial clock/data, acfail, sysreset, sysfail)
 *  - variants (16-bit backplanes, VME64)
 *  - location monitors
 */

#include "emu.h"
#include "vme.h"

#define VERBOSE 0
#include "logmacro.h"

DEFINE_DEVICE_TYPE(VME, vme_bus_device, "vme", "VME bus")
DEFINE_DEVICE_TYPE(VME_SLOT, vme_slot_device, "vme_slot", "VME slot")

vme_bus_device::vme_bus_device(machine_config const &mconfig, char const *tag, device_t *owner, u32 clock, u8 datawidth)
	: device_t(mconfig, VME, tag, owner, clock)
	, device_memory_interface(mconfig, *this)
	, m_asc
	{
		{ "iack",   ENDIANNESS_BIG, datawidth, 4, -2, address_map_constructor(FUNC(vme_bus_device::iack), this) },
		{}, {}, {}, {}, {}, {}, {}, {},
		{ "a32_09", ENDIANNESS_BIG, datawidth, 32, 0, address_map_constructor(FUNC(vme_bus_device::a32), this) },
		{ "a32_0a", ENDIANNESS_BIG, datawidth, 32, 0, address_map_constructor(FUNC(vme_bus_device::a32), this) },
		{ "a32_0b", ENDIANNESS_BIG, datawidth, 32, 0, address_map_constructor(FUNC(vme_bus_device::a32), this) },
		{},
		{ "a32_0d", ENDIANNESS_BIG, datawidth, 32, 0, address_map_constructor(FUNC(vme_bus_device::a32), this) },
		{ "a32_0e", ENDIANNESS_BIG, datawidth, 32, 0, address_map_constructor(FUNC(vme_bus_device::a32), this) },
		{ "a32_0f", ENDIANNESS_BIG, datawidth, 32, 0, address_map_constructor(FUNC(vme_bus_device::a32), this) },
		{}, {}, {}, {}, {}, {}, {}, {},
		{}, {}, {}, {}, {}, {}, {}, {},
		{}, {}, {}, {}, {}, {}, {}, {},
		{},
		{ "a16_29", ENDIANNESS_BIG, datawidth, 16, 0, address_map_constructor(FUNC(vme_bus_device::a16), this) },
		{}, {}, {},
		{ "a16_2d", ENDIANNESS_BIG, datawidth, 16, 0, address_map_constructor(FUNC(vme_bus_device::a16), this) },
		{}, {},
		{}, {}, {}, {}, {}, {}, {}, {},
		{},
		{ "a24_39", ENDIANNESS_BIG, datawidth, 24, 0, address_map_constructor(FUNC(vme_bus_device::a24), this) },
		{ "a24_3a", ENDIANNESS_BIG, datawidth, 24, 0, address_map_constructor(FUNC(vme_bus_device::a24), this) },
		{ "a24_3b", ENDIANNESS_BIG, datawidth, 24, 0, address_map_constructor(FUNC(vme_bus_device::a24), this) },
		{},
		{ "a24_3d", ENDIANNESS_BIG, datawidth, 24, 0, address_map_constructor(FUNC(vme_bus_device::a24), this) },
		{ "a24_3e", ENDIANNESS_BIG, datawidth, 24, 0, address_map_constructor(FUNC(vme_bus_device::a24), this) },
		{ "a24_3f", ENDIANNESS_BIG, datawidth, 24, 0, address_map_constructor(FUNC(vme_bus_device::a24), this) },
	}
	, m_irq(*this)
	, m_berr(*this)
	, m_irq_count{}
	, m_iack(false)
{
}

void vme_bus_device::device_start()
{
	save_item(NAME(m_irq_count));
	save_item(NAME(m_iack));
}

device_memory_interface::space_config_vector vme_bus_device::memory_space_config() const
{
	return space_config_vector{
		std::make_pair(vme::IACK,  &m_asc[vme::IACK]),

		std::make_pair(vme::AM_09, &m_asc[vme::AM_09]),
		std::make_pair(vme::AM_0a, &m_asc[vme::AM_0a]),
		std::make_pair(vme::AM_0b, &m_asc[vme::AM_0b]),
		std::make_pair(vme::AM_0d, &m_asc[vme::AM_0d]),
		std::make_pair(vme::AM_0e, &m_asc[vme::AM_0e]),
		std::make_pair(vme::AM_0f, &m_asc[vme::AM_0f]),

		std::make_pair(vme::AM_29, &m_asc[vme::AM_29]),
		std::make_pair(vme::AM_2d, &m_asc[vme::AM_2d]),

		std::make_pair(vme::AM_39, &m_asc[vme::AM_39]),
		std::make_pair(vme::AM_3a, &m_asc[vme::AM_3a]),
		std::make_pair(vme::AM_3b, &m_asc[vme::AM_3b]),
		std::make_pair(vme::AM_3d, &m_asc[vme::AM_3d]),
		std::make_pair(vme::AM_3e, &m_asc[vme::AM_3e]),
		std::make_pair(vme::AM_3f, &m_asc[vme::AM_3f]),
	};
}

u32 vme_bus_device::read_iack(address_space &space, offs_t offset, u32 mem_mask)
{
	if (!device().machine().side_effects_disabled())
	{
		LOG("read_iack 0x%08x mem_mask 0x%08x (%s)\n", offset, mem_mask, machine().describe_context());

		// enable interrupt acknowledge daisy chain
		m_iack = true;
	}

	return space.unmap();
}

// generate bus errors on unmapped read cycles
u32 vme_bus_device::read_berr(address_space &space, offs_t offset, u32 mem_mask)
{
	if (!device().machine().side_effects_disabled())
	{
		LOG("read_berr 0x%08x mem_mask 0x%08x (%s)\n", offset << 2, mem_mask, machine().describe_context());

		m_berr(0);
	}

	return space.unmap();
}

// generate bus errors on unmapped write cycles
void vme_bus_device::write_berr(offs_t offset, u32 data, u32 mem_mask)
{
	if (!device().machine().side_effects_disabled())
	{
		LOG("write_berr 0x%08x data 0x%08x mem_mask 0x%08x (%s)\n", offset << 2, data, mem_mask, machine().describe_context());

		m_berr(0);
	}
}

void vme_bus_device::iack(address_map &map)
{
	map(0x0, 0xf).r(FUNC(vme_bus_device::read_iack));
}

void vme_bus_device::a16(address_map &map)
{
	map(0x0000, 0xffff).rw(FUNC(vme_bus_device::read_berr), FUNC(vme_bus_device::write_berr));
}

void vme_bus_device::a24(address_map &map)
{
	map(0x00'0000, 0xff'ffff).rw(FUNC(vme_bus_device::read_berr), FUNC(vme_bus_device::write_berr));
}

void vme_bus_device::a32(address_map &map)
{
	map(0x0000'0000, 0xffff'ffff).rw(FUNC(vme_bus_device::read_berr), FUNC(vme_bus_device::write_berr));
}

template <unsigned I> void vme_bus_device::irq_w(int state)
{
	bool const irq_active = m_irq_count[I - 1];

	// count assertions
	if (!state)
		m_irq_count[I - 1]++;
	else if (irq_active)
		m_irq_count[I - 1]--;

	// update line state
	if (irq_active ^ bool(m_irq_count[I - 1]))
	{
		LOG("irq %d state %d\n", I, state);
		m_irq[I - 1](state);
	}
}

template void vme_bus_device::irq_w<1>(int state);
template void vme_bus_device::irq_w<2>(int state);
template void vme_bus_device::irq_w<3>(int state);
template void vme_bus_device::irq_w<4>(int state);
template void vme_bus_device::irq_w<5>(int state);
template void vme_bus_device::irq_w<6>(int state);
template void vme_bus_device::irq_w<7>(int state);

vme_slot_device::vme_slot_device(machine_config const &mconfig, char const *tag, device_t *owner, u32 clock)
	: device_t(mconfig, VME_SLOT, tag, owner, clock)
	, device_slot_interface(mconfig, *this)
	, m_bus(*this, owner->tag())
{
}

void vme_slot_device::device_start()
{
}

#undef LOG_OUTPUT_FUNC
#define LOG_OUTPUT_FUNC device().logerror

device_vme_card_interface::device_vme_card_interface(machine_config const &mconfig, device_t &device)
	: device_interface(device, "vme")
	, m_slot(dynamic_cast<vme_slot_device *>(device.owner()))
	, m_berr(*this)
	, m_iack(*this, 0)
	, m_irq_active(0)
	, m_master(false)
{
}

void device_vme_card_interface::interface_config_complete()
{
	// route bus errors to the card when it is the bus master
	if (m_slot) // HACK: avoid breaking listxml until proper fix can be identified
	m_slot->berr().append(
		[this](int state)
		{
			if (m_master)
			{
				LOG("vme berr %d\n", state);
				m_berr(state);
			}
		});
}

void device_vme_card_interface::interface_post_start()
{
	device().save_item(NAME(m_irq_active));
	device().save_item(NAME(m_master));

	/*
	 * Install a read tap into the bus interrupt acknowledge address space,
	 * simulating a daisy chain and giving each card an opportunity to respond
	 * to the interrupt acknowledge cycle in slot sequence. Empty slots do not
	 * respond, acting as if they have /IACKIN jumpered to /IACKOUT.
	 */
	vme_space(vme::IACK).install_read_tap(0x0, 0xf, "iack",
		[this](offs_t offset, u32 &data, u32 mem_mask)
		{
			// decode the interrupt number from address lines A03-A01
			unsigned const irq = BIT(offset, 1, 3);

			/*
			 * An interrupt acknowledge cycle is being asserted. This device
			 * will only respond if the upstream interrupt acknowledge daisy
			 * chain input (/IACKIN) is asserted and this device is asserting
			 * the interrupt number being acknowledged.
			 */
			if (m_slot->iackin_r() && BIT(m_irq_active, irq))
			{
				// deassert interrupt acknowledge daisy chain output
				m_slot->iackout_w(1);

				// read and return the interrupting device status/ID
				data = m_iack(irq);

				LOG("vme iack irq %d status 0x%08x (%s)\n", irq, data, device().machine().describe_context());
			}
		});
}

template <unsigned I> void device_vme_card_interface::vme_irq_w(int state)
{
	// check whether the interrupt state has changed
	if (BIT(m_irq_active, I) == state)
	{
		LOG("vme irq %d state %d\n", I, state);

		// record device interrupt state
		if (!state)
			m_irq_active |= 1U << I;
		else
			m_irq_active &= ~(1U << I);

		// update the bus irq state
		m_slot->irq_w<I>(state);
	}
}

template void device_vme_card_interface::vme_irq_w<1>(int state);
template void device_vme_card_interface::vme_irq_w<2>(int state);
template void device_vme_card_interface::vme_irq_w<3>(int state);
template void device_vme_card_interface::vme_irq_w<4>(int state);
template void device_vme_card_interface::vme_irq_w<5>(int state);
template void device_vme_card_interface::vme_irq_w<6>(int state);
template void device_vme_card_interface::vme_irq_w<7>(int state);

//--------------- P2 connector below--------------------------
/*
The VME P2 connector only specifies the mid row B of the connector
and leaves row A and C to be system specific. This has resulted in
a number of variants that has been more or less standardized:

- VMXbus was available on the first VME boards but not standardized hence
an almost compatible variant was developed by Motorola called MVMX32bus.
- VSBbus replaced VMX and MVMX32and was approved by IEEE in 1988
- SCSA is a P2 standardization for telephony voice and facsimile applications
- SkyChannel is packet switched P2 architecture from Sky Computers and standardized
through VITA/VSO.
- RACEway is a 40Mhz P2 bus allowing 480MBps throughput from Mercusry Computers and
standardized through VITA/VSO.
- VME64 adds two more rows, called 'z' and 'd', of user defined pins to the P2 connector
- P2CI adds a PCI bus onto a VME64 P2 connector

URL:s
http://rab.ict.pwr.wroc.pl/dydaktyka/supwa/vme/secbuses.html
http://www.interfacebus.com/Design_Connector_VME_P2_Buses.html

TODO: Figure out a good way to let all these variants coexist and interconnect in a VME system

*/