-- license:BSD-3-Clause
-- copyright-holders:MAMEdev Team

---------------------------------------------------------------------------
--
--   bus.lua
--
--   Rules for building bus cores
--
---------------------------------------------------------------------------


---------------------------------------------------
--
--@src/devices/bus/rc2014/rc2014.h,BUSES["RC2014"] = true
---------------------------------------------------

if (BUSES["RC2014"]~=null) then
	files {
		MAME_DIR .. "src/devices/bus/rc2014/rc2014.cpp",
		MAME_DIR .. "src/devices/bus/rc2014/rc2014.h",
		MAME_DIR .. "src/devices/bus/rc2014/cf.cpp",
		MAME_DIR .. "src/devices/bus/rc2014/cf.h",
		MAME_DIR .. "src/devices/bus/rc2014/clock.cpp",
		MAME_DIR .. "src/devices/bus/rc2014/clock.h",
		MAME_DIR .. "src/devices/bus/rc2014/edge.cpp",
		MAME_DIR .. "src/devices/bus/rc2014/edge.h",
		MAME_DIR .. "src/devices/bus/rc2014/fdc.cpp",
		MAME_DIR .. "src/devices/bus/rc2014/fdc.h",
		MAME_DIR .. "src/devices/bus/rc2014/ide.cpp",
		MAME_DIR .. "src/devices/bus/rc2014/ide.h",
		MAME_DIR .. "src/devices/bus/rc2014/micro.cpp",
		MAME_DIR .. "src/devices/bus/rc2014/micro.h",
		MAME_DIR .. "src/devices/bus/rc2014/modules.cpp",
		MAME_DIR .. "src/devices/bus/rc2014/modules.h",
		MAME_DIR .. "src/devices/bus/rc2014/ram.cpp",
		MAME_DIR .. "src/devices/bus/rc2014/ram.h",
		MAME_DIR .. "src/devices/bus/rc2014/rom.cpp",
		MAME_DIR .. "src/devices/bus/rc2014/rom.h",
		MAME_DIR .. "src/devices/bus/rc2014/romram.cpp",
		MAME_DIR .. "src/devices/bus/rc2014/romram.h",
		MAME_DIR .. "src/devices/bus/rc2014/rtc.cpp",
		MAME_DIR .. "src/devices/bus/rc2014/rtc.h",
		MAME_DIR .. "src/devices/bus/rc2014/serial.cpp",
		MAME_DIR .. "src/devices/bus/rc2014/serial.h",
		MAME_DIR .. "src/devices/bus/rc2014/sound.cpp",
		MAME_DIR .. "src/devices/bus/rc2014/sound.h",
		MAME_DIR .. "src/devices/bus/rc2014/z180cpu.cpp",
		MAME_DIR .. "src/devices/bus/rc2014/z180cpu.h",
		MAME_DIR .. "src/devices/bus/rc2014/z80cpu.cpp",
		MAME_DIR .. "src/devices/bus/rc2014/z80cpu.h",
	}
end

---------------------------------------------------
--
--@src/devices/bus/s100/s100.h,BUSES["S100"] = true
---------------------------------------------------

if (BUSES["S100"]~=null) then
	files {
		MAME_DIR .. "src/devices/bus/s100/s100.cpp",
		MAME_DIR .. "src/devices/bus/s100/s100.h",
		MAME_DIR .. "src/devices/bus/s100/am310.cpp",
		MAME_DIR .. "src/devices/bus/s100/am310.h",
		MAME_DIR .. "src/devices/bus/s100/ascsasi.cpp",
		MAME_DIR .. "src/devices/bus/s100/ascsasi.h",
		MAME_DIR .. "src/devices/bus/s100/dg640.cpp",
		MAME_DIR .. "src/devices/bus/s100/dg640.h",
		MAME_DIR .. "src/devices/bus/s100/dj2db.cpp",
		MAME_DIR .. "src/devices/bus/s100/dj2db.h",
		MAME_DIR .. "src/devices/bus/s100/djdma.cpp",
		MAME_DIR .. "src/devices/bus/s100/djdma.h",
		MAME_DIR .. "src/devices/bus/s100/mm65k16s.cpp",
		MAME_DIR .. "src/devices/bus/s100/mm65k16s.h",
		MAME_DIR .. "src/devices/bus/s100/nsmdsa.cpp",
		MAME_DIR .. "src/devices/bus/s100/nsmdsa.h",
		MAME_DIR .. "src/devices/bus/s100/nsmdsad.cpp",
		MAME_DIR .. "src/devices/bus/s100/nsmdsad.h",
		MAME_DIR .. "src/devices/bus/s100/poly16k.cpp",
		MAME_DIR .. "src/devices/bus/s100/poly16k.h",
		MAME_DIR .. "src/devices/bus/s100/polyfdc.cpp",
		MAME_DIR .. "src/devices/bus/s100/polyfdc.h",
		MAME_DIR .. "src/devices/bus/s100/polyvti.cpp",
		MAME_DIR .. "src/devices/bus/s100/polyvti.h",
		MAME_DIR .. "src/devices/bus/s100/seals8k.cpp",
		MAME_DIR .. "src/devices/bus/s100/seals8k.h",
		MAME_DIR .. "src/devices/bus/s100/vectordualmode.cpp",
		MAME_DIR .. "src/devices/bus/s100/vectordualmode.h",
		MAME_DIR .. "src/devices/bus/s100/wunderbus.cpp",
		MAME_DIR .. "src/devices/bus/s100/wunderbus.h",
	}
end

---------------------------------------------------
--
--@src/devices/bus/rs232/rs232.h,BUSES["RS232"] = true
---------------------------------------------------

if (BUSES["RS232"]~=null) then
	files {
		MAME_DIR .. "src/devices/bus/rs232/exorterm.cpp",
		MAME_DIR .. "src/devices/bus/rs232/exorterm.h",
		MAME_DIR .. "src/devices/bus/rs232/hlemouse.cpp",
		MAME_DIR .. "src/devices/bus/rs232/hlemouse.h",
		MAME_DIR .. "src/devices/bus/rs232/ie15.cpp",
		MAME_DIR .. "src/devices/bus/rs232/ie15.h",
		MAME_DIR .. "src/devices/bus/rs232/keyboard.cpp",
		MAME_DIR .. "src/devices/bus/rs232/keyboard.h",
		MAME_DIR .. "src/devices/bus/rs232/loopback.cpp",
		MAME_DIR .. "src/devices/bus/rs232/loopback.h",
		MAME_DIR .. "src/devices/bus/rs232/mboardd.cpp",
		MAME_DIR .. "src/devices/bus/rs232/mboardd.h",
		MAME_DIR .. "src/devices/bus/rs232/nss_tvinterface.cpp",
		MAME_DIR .. "src/devices/bus/rs232/nss_tvinterface.h",
		MAME_DIR .. "src/devices/bus/rs232/null_modem.cpp",
		MAME_DIR .. "src/devices/bus/rs232/null_modem.h",
		MAME_DIR .. "src/devices/bus/rs232/patchbox.cpp",
		MAME_DIR .. "src/devices/bus/rs232/patchbox.h",
		MAME_DIR .. "src/devices/bus/rs232/printer.cpp",
		MAME_DIR .. "src/devices/bus/rs232/printer.h",
		MAME_DIR .. "src/devices/bus/rs232/pty.cpp",
		MAME_DIR .. "src/devices/bus/rs232/pty.h",
		MAME_DIR .. "src/devices/bus/rs232/rs232.cpp",
		MAME_DIR .. "src/devices/bus/rs232/rs232.h",
		MAME_DIR .. "src/devices/bus/rs232/rs232_sync_io.cpp",
		MAME_DIR .. "src/devices/bus/rs232/rs232_sync_io.h",
		MAME_DIR .. "src/devices/bus/rs232/scorpion.cpp",
		MAME_DIR .. "src/devices/bus/rs232/scorpion.h",
		MAME_DIR .. "src/devices/bus/rs232/sun_kbd.cpp",
		MAME_DIR .. "src/devices/bus/rs232/sun_kbd.h",
		MAME_DIR .. "src/devices/bus/rs232/swtpc8212.cpp",
		MAME_DIR .. "src/devices/bus/rs232/swtpc8212.h",
		MAME_DIR .. "src/devices/bus/rs232/teletex800.cpp",
		MAME_DIR .. "src/devices/bus/rs232/teletex800.h",
		MAME_DIR .. "src/devices/bus/rs232/terminal.cpp",
		MAME_DIR .. "src/devices/bus/rs232/terminal.h",
		MAME_DIR .. "src/devices/bus/rs232/xvd701.cpp",
		MAME_DIR .. "src/devices/bus/rs232/xvd701.h",
	}
end


---------------------------------------------------
--
--@src/devices/bus/sdk85/memexp.h,BUSES["SDK85"] = true
---------------------------------------------------

if (BUSES["SDK85"]~=null) then
	files {
		MAME_DIR .. "src/devices/bus/sdk85/memexp.cpp",
		MAME_DIR .. "src/devices/bus/sdk85/memexp.h",
		MAME_DIR .. "src/devices/bus/sdk85/i8755.cpp",
		MAME_DIR .. "src/devices/bus/sdk85/i8755.h",
	}
end



---------------------------------------------------
--
--@src/devices/bus/compis/graphics.h,BUSES["COMPIS_GRAPHICS"] = true
---------------------------------------------------

if (BUSES["COMPIS_GRAPHICS"]~=null) then
	files {
		MAME_DIR .. "src/devices/bus/compis/graphics.cpp",
		MAME_DIR .. "src/devices/bus/compis/graphics.h",
		MAME_DIR .. "src/devices/bus/compis/hrg.cpp",
		MAME_DIR .. "src/devices/bus/compis/hrg.h",
	}
end

---------------------------------------------------
--
--@src/devices/bus/pc1512/mouse.h,BUSES["PC1512"] = true
---------------------------------------------------

if (BUSES["PC1512"]~=null) then
	files {
		MAME_DIR .. "src/devices/bus/pc1512/mouse.cpp",
		MAME_DIR .. "src/devices/bus/pc1512/mouse.h",
	}
end

---------------------------------------------------
--
--@src/devices/bus/cbus/pc9801_cbus.h,BUSES["CBUS"] = true
---------------------------------------------------

if (BUSES["CBUS"]~=null) then
	files {
		MAME_DIR .. "src/devices/bus/cbus/pc9801_26.cpp",
		MAME_DIR .. "src/devices/bus/cbus/pc9801_26.h",
		MAME_DIR .. "src/devices/bus/cbus/pc9801_55.cpp",
		MAME_DIR .. "src/devices/bus/cbus/pc9801_55.h",
		MAME_DIR .. "src/devices/bus/cbus/pc9801_86.cpp",
		MAME_DIR .. "src/devices/bus/cbus/pc9801_86.h",
		MAME_DIR .. "src/devices/bus/cbus/pc9801_118.cpp",
		MAME_DIR .. "src/devices/bus/cbus/pc9801_118.h",
		MAME_DIR .. "src/devices/bus/cbus/pc9801_amd98.cpp",
		MAME_DIR .. "src/devices/bus/cbus/pc9801_amd98.h",
		MAME_DIR .. "src/devices/bus/cbus/pc9801_snd.cpp",
		MAME_DIR .. "src/devices/bus/cbus/pc9801_snd.h",
		MAME_DIR .. "src/devices/bus/cbus/mpu_pc98.cpp",
		MAME_DIR .. "src/devices/bus/cbus/mpu_pc98.h",
		MAME_DIR .. "src/devices/bus/cbus/pc9801_cbus.cpp",
		MAME_DIR .. "src/devices/bus/cbus/pc9801_cbus.h",
	}
end

---------------------------------------------------
--
--@src/devices/bus/psi_kbd/psi_kbd.h,BUSES["PSI_KEYBOARD"] = true
---------------------------------------------------

if (BUSES["PSI_KEYBOARD"]~=null) then
	files {
		MAME_DIR .. "src/devices/bus/psi_kbd/psi_kbd.cpp",
		MAME_DIR .. "src/devices/bus/psi_kbd/psi_kbd.h",
		MAME_DIR .. "src/devices/bus/psi_kbd/ergoline.cpp",
		MAME_DIR .. "src/devices/bus/psi_kbd/ergoline.h",
		MAME_DIR .. "src/devices/bus/psi_kbd/hle.cpp",
		MAME_DIR .. "src/devices/bus/psi_kbd/hle.h",
	}
end

---------------------------------------------------
--
--@src/devices/bus/interpro/sr/sr.h,BUSES["INTERPRO_SR"] = true
---------------------------------------------------

if (BUSES["INTERPRO_SR"]~=null) then
	files {
		MAME_DIR .. "src/devices/bus/interpro/sr/sr.cpp",
		MAME_DIR .. "src/devices/bus/interpro/sr/sr.h",
		MAME_DIR .. "src/devices/bus/interpro/sr/sr_cards.cpp",
		MAME_DIR .. "src/devices/bus/interpro/sr/sr_cards.h",
		MAME_DIR .. "src/devices/bus/interpro/sr/gt.cpp",
		MAME_DIR .. "src/devices/bus/interpro/sr/gt.h",
		MAME_DIR .. "src/devices/bus/interpro/sr/edge.cpp",
		MAME_DIR .. "src/devices/bus/interpro/sr/edge.h",
	}
end

---------------------------------------------------
--
--@src/devices/bus/interpro/keyboard/keyboard.h,BUSES["INTERPRO_KEYBOARD"] = true
---------------------------------------------------

if (BUSES["INTERPRO_KEYBOARD"]~=null) then
	files {
		MAME_DIR .. "src/devices/bus/interpro/keyboard/keyboard.cpp",
		MAME_DIR .. "src/devices/bus/interpro/keyboard/keyboard.h",
		MAME_DIR .. "src/devices/bus/interpro/keyboard/hle.cpp",
		MAME_DIR .. "src/devices/bus/interpro/keyboard/hle.h",
		MAME_DIR .. "src/devices/bus/interpro/keyboard/lle.cpp",
		MAME_DIR .. "src/devices/bus/interpro/keyboard/lle.h"
	}
end

---------------------------------------------------
--
--@src/devices/bus/interpro/mouse/mouse.h,BUSES["INTERPRO_MOUSE"] = true
---------------------------------------------------

if (BUSES["INTERPRO_MOUSE"]~=null) then
	files {
		MAME_DIR .. "src/devices/bus/interpro/mouse/mouse.cpp",
		MAME_DIR .. "src/devices/bus/interpro/mouse/mouse.h"
	}
end

---------------------------------------------------
--
--@src/devices/bus/einstein/pipe/pipe.h,BUSES["TATUNG_PIPE"] = true
---------------------------------------------------

if (BUSES["TATUNG_PIPE"]~=null) then
	files {
		MAME_DIR .. "src/devices/bus/einstein/pipe/pipe.cpp",
		MAME_DIR .. "src/devices/bus/einstein/pipe/pipe.h",
		MAME_DIR .. "src/devices/bus/einstein/pipe/silicon_disc.cpp",
		MAME_DIR .. "src/devices/bus/einstein/pipe/silicon_disc.h",
		MAME_DIR .. "src/devices/bus/einstein/pipe/speculator.cpp",
		MAME_DIR .. "src/devices/bus/einstein/pipe/speculator.h",
		MAME_DIR .. "src/devices/bus/einstein/pipe/tk02.cpp",
		MAME_DIR .. "src/devices/bus/einstein/pipe/tk02.h",
	}
end

---------------------------------------------------
--
--@src/devices/bus/einstein/userport/userport.h,BUSES["EINSTEIN_USERPORT"] = true
---------------------------------------------------

if (BUSES["EINSTEIN_USERPORT"]~=null) then
	files {
		MAME_DIR .. "src/devices/bus/einstein/userport/userport.cpp",
		MAME_DIR .. "src/devices/bus/einstein/userport/userport.h",
		MAME_DIR .. "src/devices/bus/einstein/userport/mouse.cpp",
		MAME_DIR .. "src/devices/bus/einstein/userport/mouse.h",
		MAME_DIR .. "src/devices/bus/einstein/userport/speech.cpp",
		MAME_DIR .. "src/devices/bus/einstein/userport/speech.h",
	}
end

---------------------------------------------------
--
--@src/devices/bus/tmc600/euro.h,BUSES["TMC600"] = true
---------------------------------------------------

if (BUSES["TMC600"]~=null) then
	files {
		MAME_DIR .. "src/devices/bus/tmc600/euro.cpp",
		MAME_DIR .. "src/devices/bus/tmc600/euro.h",
	}
end

---------------------------------------------------
--
--@src/devices/bus/multibus/multibus.h,BUSES["MULTIBUS"] = true
---------------------------------------------------

if (BUSES["MULTIBUS"]~=null) then
	files {
		MAME_DIR .. "src/devices/bus/multibus/multibus.cpp",
		MAME_DIR .. "src/devices/bus/multibus/multibus.h",
		MAME_DIR .. "src/devices/bus/multibus/isbc202.cpp",
		MAME_DIR .. "src/devices/bus/multibus/isbc202.h",
		MAME_DIR .. "src/devices/bus/multibus/isbc8024.cpp",
		MAME_DIR .. "src/devices/bus/multibus/isbc8024.h",
		MAME_DIR .. "src/devices/bus/multibus/cpuap.cpp",
		MAME_DIR .. "src/devices/bus/multibus/cpuap.h",
		MAME_DIR .. "src/devices/bus/multibus/serad.cpp",
		MAME_DIR .. "src/devices/bus/multibus/serad.h",
		MAME_DIR .. "src/devices/bus/multibus/labtam_3232.cpp",
		MAME_DIR .. "src/devices/bus/multibus/labtam_3232.h",
		MAME_DIR .. "src/devices/bus/multibus/labtam_vducom.cpp",
		MAME_DIR .. "src/devices/bus/multibus/labtam_vducom.h",
		MAME_DIR .. "src/devices/bus/multibus/labtam_z80sbc.cpp",
		MAME_DIR .. "src/devices/bus/multibus/labtam_z80sbc.h",
		MAME_DIR .. "src/devices/bus/multibus/robotron_k7070.cpp",
		MAME_DIR .. "src/devices/bus/multibus/robotron_k7070.h",
		MAME_DIR .. "src/devices/bus/multibus/robotron_k7071.cpp",
		MAME_DIR .. "src/devices/bus/multibus/robotron_k7071.h",
	}
end

---------------------------------------------------
--
--@src/devices/bus/rtpc/kbd_con.h,BUSES["RTPC_KBD"] = true
---------------------------------------------------

if (BUSES["RTPC_KBD"]~=null) then
	files {
		MAME_DIR .. "src/devices/bus/rtpc/kbd_con.cpp",
		MAME_DIR .. "src/devices/bus/rtpc/kbd_con.h",
		MAME_DIR .. "src/devices/bus/rtpc/kbd.cpp",
		MAME_DIR .. "src/devices/bus/rtpc/kbd.h",
	}
end

---------------------------------------------------
--
--@src/devices/bus/saitek_osa/expansion.h,BUSES["SAITEK_OSA"] = true
---------------------------------------------------

if (BUSES["SAITEK_OSA"]~=null) then
	files {
		MAME_DIR .. "src/devices/bus/saitek_osa/expansion.cpp",
		MAME_DIR .. "src/devices/bus/saitek_osa/expansion.h",
		MAME_DIR .. "src/devices/bus/saitek_osa/maestro.cpp",
		MAME_DIR .. "src/devices/bus/saitek_osa/maestro.h",
		MAME_DIR .. "src/devices/bus/saitek_osa/maestroa.cpp",
		MAME_DIR .. "src/devices/bus/saitek_osa/maestroa.h",
		MAME_DIR .. "src/devices/bus/saitek_osa/sparc.cpp",
		MAME_DIR .. "src/devices/bus/saitek_osa/sparc.h",
	}
end

---------------------------------------------------
--
--@src/devices/bus/samcoupe/drive/drive.h,BUSES["SAMCOUPE_DRIVE_PORT"] = true
---------------------------------------------------

if (BUSES["SAMCOUPE_DRIVE_PORT"]~=null) then
	files {
		MAME_DIR .. "src/devices/bus/samcoupe/drive/drive.cpp",
		MAME_DIR .. "src/devices/bus/samcoupe/drive/drive.h",
		MAME_DIR .. "src/devices/bus/samcoupe/drive/modules.cpp",
		MAME_DIR .. "src/devices/bus/samcoupe/drive/modules.h",
		MAME_DIR .. "src/devices/bus/samcoupe/drive/atom.cpp",
		MAME_DIR .. "src/devices/bus/samcoupe/drive/atom.h",
		MAME_DIR .. "src/devices/bus/samcoupe/drive/floppy.cpp",
		MAME_DIR .. "src/devices/bus/samcoupe/drive/floppy.h",
	}
end

---------------------------------------------------
--
--@src/devices/bus/samcoupe/expansion/expansion.h,BUSES["SAMCOUPE_EXPANSION"] = true
---------------------------------------------------

if (BUSES["SAMCOUPE_EXPANSION"]~=null) then
	files {
		MAME_DIR .. "src/devices/bus/samcoupe/expansion/expansion.cpp",
		MAME_DIR .. "src/devices/bus/samcoupe/expansion/expansion.h",
		MAME_DIR .. "src/devices/bus/samcoupe/expansion/modules.cpp",
		MAME_DIR .. "src/devices/bus/samcoupe/expansion/modules.h",
		MAME_DIR .. "src/devices/bus/samcoupe/expansion/blue_sampler.cpp",
		MAME_DIR .. "src/devices/bus/samcoupe/expansion/blue_sampler.h",
		MAME_DIR .. "src/devices/bus/samcoupe/expansion/dallas.cpp",
		MAME_DIR .. "src/devices/bus/samcoupe/expansion/dallas.h",
		MAME_DIR .. "src/devices/bus/samcoupe/expansion/onemeg.cpp",
		MAME_DIR .. "src/devices/bus/samcoupe/expansion/onemeg.h",
		MAME_DIR .. "src/devices/bus/samcoupe/expansion/sambus.cpp",
		MAME_DIR .. "src/devices/bus/samcoupe/expansion/sambus.h",
		MAME_DIR .. "src/devices/bus/samcoupe/expansion/sdide.cpp",
		MAME_DIR .. "src/devices/bus/samcoupe/expansion/sdide.h",
		MAME_DIR .. "src/devices/bus/samcoupe/expansion/sid.cpp",
		MAME_DIR .. "src/devices/bus/samcoupe/expansion/sid.h",
		MAME_DIR .. "src/devices/bus/samcoupe/expansion/spi.cpp",
		MAME_DIR .. "src/devices/bus/samcoupe/expansion/spi.h",
		MAME_DIR .. "src/devices/bus/samcoupe/expansion/voicebox.cpp",
		MAME_DIR .. "src/devices/bus/samcoupe/expansion/voicebox.h",
	}
end

---------------------------------------------------
--
--@src/devices/bus/samcoupe/mouse/mouse.h,BUSES["SAMCOUPE_MOUSE_PORT"] = true
---------------------------------------------------

if (BUSES["SAMCOUPE_MOUSE_PORT"]~=null) then
	files {
		MAME_DIR .. "src/devices/bus/samcoupe/mouse/mouseport.cpp",
		MAME_DIR .. "src/devices/bus/samcoupe/mouse/mouseport.h",
		MAME_DIR .. "src/devices/bus/samcoupe/mouse/modules.cpp",
		MAME_DIR .. "src/devices/bus/samcoupe/mouse/modules.h",
		MAME_DIR .. "src/devices/bus/samcoupe/mouse/mouse.cpp",
		MAME_DIR .. "src/devices/bus/samcoupe/mouse/mouse.h",
	}
end

---------------------------------------------------
--
--@src/devices/bus/thomson/extension.h,BUSES["THOMSON"] = true
---------------------------------------------------

if (BUSES["THOMSON"]~=null) then
	files {
		MAME_DIR .. "src/devices/bus/thomson/extension.cpp",
		MAME_DIR .. "src/devices/bus/thomson/extension.h",
		MAME_DIR .. "src/devices/bus/thomson/cc90_232.cpp",
		MAME_DIR .. "src/devices/bus/thomson/cc90_232.h",
		MAME_DIR .. "src/devices/bus/thomson/cd90_015.cpp",
		MAME_DIR .. "src/devices/bus/thomson/cd90_015.h",
		MAME_DIR .. "src/devices/bus/thomson/cq90_028.cpp",
		MAME_DIR .. "src/devices/bus/thomson/cq90_028.h",
		MAME_DIR .. "src/devices/bus/thomson/cd90_351.cpp",
		MAME_DIR .. "src/devices/bus/thomson/cd90_351.h",
		MAME_DIR .. "src/devices/bus/thomson/cd90_640.cpp",
		MAME_DIR .. "src/devices/bus/thomson/cd90_640.h",
		MAME_DIR .. "src/devices/bus/thomson/md90_120.cpp",
		MAME_DIR .. "src/devices/bus/thomson/md90_120.h",
		MAME_DIR .. "src/devices/bus/thomson/midipak.cpp",
		MAME_DIR .. "src/devices/bus/thomson/midipak.h",
		MAME_DIR .. "src/devices/bus/thomson/nanoreseau.cpp",
		MAME_DIR .. "src/devices/bus/thomson/nanoreseau.h",
		MAME_DIR .. "src/devices/bus/thomson/rf57_932.cpp",
		MAME_DIR .. "src/devices/bus/thomson/rf57_932.h",
		MAME_DIR .. "src/devices/bus/thomson/speech.cpp",
		MAME_DIR .. "src/devices/bus/thomson/speech.h",
	}
end

---------------------------------------------------
--
--@src/devices/bus/pc8801/pc8801_exp.h,BUSES["PC8801"] = true
---------------------------------------------------

if (BUSES["PC8801"]~=null) then
	files {
		MAME_DIR .. "src/devices/bus/pc8801/gsx8800.cpp",
		MAME_DIR .. "src/devices/bus/pc8801/gsx8800.h",
		MAME_DIR .. "src/devices/bus/pc8801/pc8801_23.cpp",
		MAME_DIR .. "src/devices/bus/pc8801/pc8801_23.h",
		MAME_DIR .. "src/devices/bus/pc8801/pc8801_31.cpp",
		MAME_DIR .. "src/devices/bus/pc8801/pc8801_31.h",
		MAME_DIR .. "src/devices/bus/pc8801/pc8801_exp.cpp",
		MAME_DIR .. "src/devices/bus/pc8801/pc8801_exp.h",
		MAME_DIR .. "src/devices/bus/pc8801/pcg8100.cpp",
		MAME_DIR .. "src/devices/bus/pc8801/pcg8100.h",
		MAME_DIR .. "src/devices/bus/pc8801/jmbx1.cpp",
		MAME_DIR .. "src/devices/bus/pc8801/jmbx1.h",
		MAME_DIR .. "src/devices/bus/pc8801/hmb20.cpp",
		MAME_DIR .. "src/devices/bus/pc8801/hmb20.h",
	}
end

---------------------------------------------------
--
--@src/devices/bus/mtu130/board.h,BUSES["MTU130"] = true
---------------------------------------------------

if (BUSES["MTU130"]~=null) then
	files {
		MAME_DIR .. "src/devices/bus/mtu130/board.cpp",
		MAME_DIR .. "src/devices/bus/mtu130/board.h",
		MAME_DIR .. "src/devices/bus/mtu130/datamover.cpp",
		MAME_DIR .. "src/devices/bus/mtu130/datamover.h",
	}
 end

---------------------------------------------------
--
--@src/devices/bus/nabupc/option.h,BUSES["NABU"] = true
---------------------------------------------------
if (BUSES["NABU"]~=null) then
	files {
		MAME_DIR .. "src/devices/bus/nabupc/adapter.cpp",
		MAME_DIR .. "src/devices/bus/nabupc/adapter.h",
		MAME_DIR .. "src/devices/bus/nabupc/fdc.cpp",
		MAME_DIR .. "src/devices/bus/nabupc/fdc.h",
		MAME_DIR .. "src/devices/bus/nabupc/hdd.cpp",
		MAME_DIR .. "src/devices/bus/nabupc/hdd.h",
		MAME_DIR .. "src/devices/bus/nabupc/option.cpp",
		MAME_DIR .. "src/devices/bus/nabupc/option.h",
		MAME_DIR .. "src/devices/bus/nabupc/rs232.cpp",
		MAME_DIR .. "src/devices/bus/nabupc/rs232.h",
	}
end

---------------------------------------------------
--
--@src/devices/bus/mc68000/sysbus.h,BUSES["MC68000_SYSBUS"] = true
---------------------------------------------------

if (BUSES["MC68000_SYSBUS"]~=null) then
	files {
		MAME_DIR .. "src/devices/bus/mc68000/sysbus.cpp",
		MAME_DIR .. "src/devices/bus/mc68000/sysbus.h",
		MAME_DIR .. "src/devices/bus/mc68000/cards.cpp",
		MAME_DIR .. "src/devices/bus/mc68000/cards.h",
		MAME_DIR .. "src/devices/bus/mc68000/floppy.cpp",
		MAME_DIR .. "src/devices/bus/mc68000/floppy.h",
		MAME_DIR .. "src/devices/bus/mc68000/ram.cpp",
		MAME_DIR .. "src/devices/bus/mc68000/ram.h",
	}
end

---------------------------------------------------
--
--@src/devices/bus/waveblaster/waveblaster.h,BUSES["WAVEBLASTER"] = true
---------------------------------------------------

if (BUSES["WAVEBLASTER"]~=null) then
	files {
		MAME_DIR .. "src/devices/bus/waveblaster/waveblaster.cpp",
		MAME_DIR .. "src/devices/bus/waveblaster/waveblaster.h",
		MAME_DIR .. "src/devices/bus/waveblaster/omniwave.cpp",
		MAME_DIR .. "src/devices/bus/waveblaster/omniwave.h",
		MAME_DIR .. "src/devices/bus/waveblaster/db50xg.cpp",
		MAME_DIR .. "src/devices/bus/waveblaster/db50xg.h",
		MAME_DIR .. "src/devices/bus/waveblaster/db60xg.cpp",
		MAME_DIR .. "src/devices/bus/waveblaster/db60xg.h",
		MAME_DIR .. "src/devices/bus/waveblaster/wg130.cpp",
		MAME_DIR .. "src/devices/bus/waveblaster/wg130.h",
	}
end

---------------------------------------------------
--
--@src/devices/bus/pci/pci_slot.h,BUSES["PCI"] = true
---------------------------------------------------

if (BUSES["PCI"]~=null) then
	files {
		MAME_DIR .. "src/devices/bus/pci/pci_slot.cpp",
		MAME_DIR .. "src/devices/bus/pci/pci_slot.h",
		MAME_DIR .. "src/devices/bus/pci/aha2940au.cpp",
		MAME_DIR .. "src/devices/bus/pci/aha2940au.h",
		MAME_DIR .. "src/devices/bus/pci/audiowerk2.cpp",
		MAME_DIR .. "src/devices/bus/pci/audiowerk2.h",
		MAME_DIR .. "src/devices/bus/pci/clgd5446.cpp",
		MAME_DIR .. "src/devices/bus/pci/clgd5446.h",
		MAME_DIR .. "src/devices/bus/pci/clgd546x_laguna.cpp",
		MAME_DIR .. "src/devices/bus/pci/clgd546x_laguna.h",
		MAME_DIR .. "src/devices/bus/pci/ds2416.cpp",
		MAME_DIR .. "src/devices/bus/pci/ds2416.h",
		MAME_DIR .. "src/devices/bus/pci/ess_maestro.cpp",
		MAME_DIR .. "src/devices/bus/pci/ess_maestro.h",
		MAME_DIR .. "src/devices/bus/pci/geforce.cpp",
		MAME_DIR .. "src/devices/bus/pci/geforce.h",
		MAME_DIR .. "src/devices/bus/pci/mga2064w.cpp",
		MAME_DIR .. "src/devices/bus/pci/mga2064w.h",
		MAME_DIR .. "src/devices/bus/pci/ncr53c825.cpp",
		MAME_DIR .. "src/devices/bus/pci/ncr53c825.h",
		MAME_DIR .. "src/devices/bus/pci/neon250.cpp",
		MAME_DIR .. "src/devices/bus/pci/neon250.h",
		MAME_DIR .. "src/devices/bus/pci/oti_spitfire.cpp",
		MAME_DIR .. "src/devices/bus/pci/oti_spitfire.h",
		MAME_DIR .. "src/devices/bus/pci/opti82c861.cpp",
		MAME_DIR .. "src/devices/bus/pci/opti82c861.h",
		MAME_DIR .. "src/devices/bus/pci/pdc20262.cpp",
		MAME_DIR .. "src/devices/bus/pci/pdc20262.h",
		MAME_DIR .. "src/devices/bus/pci/promotion.cpp",
		MAME_DIR .. "src/devices/bus/pci/promotion.h",
		MAME_DIR .. "src/devices/bus/pci/riva128.cpp",
		MAME_DIR .. "src/devices/bus/pci/riva128.h",
		MAME_DIR .. "src/devices/bus/pci/rivatnt.cpp",
		MAME_DIR .. "src/devices/bus/pci/rivatnt.h",
		MAME_DIR .. "src/devices/bus/pci/rtl8029as_pci.cpp",
		MAME_DIR .. "src/devices/bus/pci/rtl8029as_pci.h",
		MAME_DIR .. "src/devices/bus/pci/rtl8139_pci.cpp",
		MAME_DIR .. "src/devices/bus/pci/rtl8139_pci.h",
		MAME_DIR .. "src/devices/bus/pci/sis6326.cpp",
		MAME_DIR .. "src/devices/bus/pci/sis6326.h",
		MAME_DIR .. "src/devices/bus/pci/sonicvibes.cpp",
		MAME_DIR .. "src/devices/bus/pci/sonicvibes.h",
		MAME_DIR .. "src/devices/bus/pci/sw1000xg.cpp",
		MAME_DIR .. "src/devices/bus/pci/sw1000xg.h",
		MAME_DIR .. "src/devices/bus/pci/virge_pci.cpp",
		MAME_DIR .. "src/devices/bus/pci/virge_pci.h",
		MAME_DIR .. "src/devices/bus/pci/vision.cpp",
		MAME_DIR .. "src/devices/bus/pci/vision.h",
		MAME_DIR .. "src/devices/bus/pci/vt6306.cpp",
		MAME_DIR .. "src/devices/bus/pci/vt6306.h",
		MAME_DIR .. "src/devices/bus/pci/wd9710_pci.cpp",
		MAME_DIR .. "src/devices/bus/pci/wd9710_pci.h",
		MAME_DIR .. "src/devices/bus/pci/ymp21.cpp",
		MAME_DIR .. "src/devices/bus/pci/ymp21.h",
		MAME_DIR .. "src/devices/bus/pci/zr36057.cpp",
		MAME_DIR .. "src/devices/bus/pci/zr36057.h",
	}
end


---------------------------------------------------
--
--@src/devices/bus/plg1x0/plg1x0.h,BUSES["PLG1X0"] = true
---------------------------------------------------

if (BUSES["PLG1X0"]~=null) then
	files {
		MAME_DIR .. "src/devices/bus/plg1x0/plg1x0.cpp",
		MAME_DIR .. "src/devices/bus/plg1x0/plg1x0.h",
		MAME_DIR .. "src/devices/bus/plg1x0/plg100-vl.cpp",
		MAME_DIR .. "src/devices/bus/plg1x0/plg100-vl.h",
		MAME_DIR .. "src/devices/bus/plg1x0/plg150-ap.cpp",
		MAME_DIR .. "src/devices/bus/plg1x0/plg150-ap.h",
	}
end
