#include <iostream>
#include <cstring>
#include "Memory.hpp"
#include "registerAddr.hpp"

Memory::Memory(void) {}

Memory::~Memory(void) {}

void			Memory::reset(void)
{
	memset(this->_m_wram, 0, 32768);
	memset(this->_m_vram, 0, 16384);
	memset(this->_m_oam, 0, 160);
	memset(this->_m_io, 0, 127);
	memset(this->_m_zp, 0, 127);
	this->_inBios = true;
}

htype			Memory::getRomType(void)
{
	return this->_rom.getHardware();
}

int				Memory::loadRom(const char *file, htype hardware)
{
	int		ret;

	ret = this->_rom.load(file);
	hardware = (hardware == AUTO) ? this->_rom.getHardware() : hardware;
	this->_codeBios = this->_bios.load(hardware);
	this->_typeBios = hardware;
	return ret;
}

uint8_t			Memory::read_byte(uint16_t addr)
{
	switch (addr & 0xF000){
		case 0x0000:
			if (this->_inBios && addr != 0x0100)
			{
				if (addr <= 0xFF && this->_typeBios == GB)
					return this->_codeBios[addr];
				else if (addr <= 0x900 && this->_typeBios == GBC)
					return this->_codeBios[addr];
				else
					return this->_rom.read(addr);
			}
			else
			{
				if (addr == 0x0100)			// TODO A verifier si marche pour GBC
					this->_inBios = false;	// TODO A verifier si marche pour GBC
				return this->_rom.read(addr);
			}
			break;
		case 0x1000:
		case 0x2000:
		case 0x3000:
		case 0x4000:
		case 0x5000:
		case 0x6000:
		case 0x7000:
			// ROM
			return this->_rom.read(addr);
			break;
		case 0x8000:
		case 0x9000:
			// VRAM
			return this->_m_wram[(this->_m_io[(VBK & 0xFF)] & 0x01)][(addr & 0x0FFF)];
			break;
		case 0xA000:
		case 0xB000:
			// ERAM
			return this->_rom.read(addr);
			break;
		case 0xC000:
		case 0xD000:
			// WRAM
			if ((addr & 0xF000) < 0xD000)
				return this->_m_wram[0][(addr & 0x0FFF)];
			else
				return this->_m_wram[(this->_m_io[(SVBK & 0xFF)] & 0x03)][(addr & 0x0FFF)];
			break;
		case 0xF000:
			switch (addr & 0x0F00){
				case 0x0E00:
					if ((addr & 0xFF) <= 0x9F)
					{
						// SPRITE
						return this->_m_oam[(addr & 0xFF)];
					}
					break;
				case 0x0F00:
					if ((addr & 0xFF) <= 0x7F)
					{
						// I/O
						return this->_m_io[(addr & 0xFF)];
					}
					else
					{
						// Zero page
						return this->_m_zp[(addr & 0xFF) - 0x7F];
					}
					break;
			}
			break;
	}
	return 0;
}

void			Memory::write_byte(uint16_t addr, uint8_t val, bool super)
{
	switch (addr & 0xF000){
		case 0x0000:
		case 0x1000:
		case 0x2000:
		case 0x3000:
		case 0x4000:
		case 0x5000:
		case 0x6000:
		case 0x7000:
			// ROM
			this->_rom.write(addr, val);
			break;
		case 0x8000:
		case 0x9000:
			// VRAM
			this->_m_wram[(this->_m_io[(VBK & 0xFF)] & 0x01)][(addr & 0x0FFF)] = val;
			break;
		case 0xA000:
		case 0xB000:
			// ERAM
			this->_rom.write(addr, val);
			break;
		case 0xC000:
		case 0xD000:
			// WRAM
			if ((addr & 0xF000) < 0xD000)
				this->_m_wram[0][(addr & 0x0FFF)] = val;
			else
				this->_m_wram[(this->_m_io[(SVBK & 0xFF)] & 0x03)][(addr & 0x0FFF)] = val;
			break;
		case 0xF000:
			switch (addr & 0x0F00){
				case 0x0E00:
					if ((addr & 0xFF) <= 0x9F)
					{
						// SPRITE
						this->_m_oam[(addr & 0xFF)] = val;
					}
					break;
				case 0x0F00:
					if (!super && addr == 0xFF00)
					{
						// P1
						this->_m_io[(addr & 0xFF)] = (val & 0xF0) | (this->_m_io[(addr & 0xFF)] & 0x0F);
					}
					else if (!super && addr == 0xFF04)
					{
						// DIV
						this->_m_io[(addr & 0xFF)] = 0x00;
					}
					else if ((addr & 0xFF) <= 0x7F)
					{
						// I/O
						this->_m_io[(addr & 0xFF)] = val;
					}
					else
					{
						// Zero page
						this->_m_zp[(addr & 0xFF) - 0x7F] = val;
					}
					break;
			}
			break;
	}
}

uint16_t		Memory::read_word(uint16_t addr)
{
	return this->read_byte(addr) + (this->read_byte(addr + 1) << 8);
}

void			Memory::write_word(uint16_t addr, uint16_t val, bool super)
{
	this->write_byte(addr, (val & 0xFF), super);
	this->write_byte(addr + 1, ((val & 0xFF00) >> 8), super);
}
