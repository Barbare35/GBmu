#ifndef MEMORY_HPP
#define MEMORY_HPP

// Define I/O Register Address

// Port / Mode
#define P1		0xFF00
#define SB		0xFF01
#define SC		0xFF02
#define DIV		0xFF04
#define TIMA	0xFF05
#define TMA		0xFF06
#define TAC		0xFF07
#define KEY1	0xFF4D
#define RP		0xFF56

// Bank Control
#define VBK		0xFF4F
#define SVBK	0xFF70

//Interrupt Flags
#define IF		0xFF0F
#define IE		0xFFFF
//#define IME		0xFF??

// LCD Display
#define LCDC	0xFF40
#define STAT	0xFF41
#define SCY		0xFF42
#define SCX		0xFF43
#define LY		0xFF44
#define LYC		0xFF45
#define DMA		0xFF46
#define BGP		0xFF47
#define OBP0	0xFF48
#define OBP1	0xFF49
#define WY		0xFF4A
#define WX		0xFF4B
#define HDMA1	0xFF51
#define HDMA2	0xFF52
#define HDMA3	0xFF53
#define HDMA4	0xFF54
#define HDMA5	0xFF55
#define BCPS	0xFF68
#define BCPD	0xFF69
#define OCPS	0xFF6A
#define OCPD	0xFF6B


class Memory {

	public:
		static Memory		&Instance(void);
		void				reset(void);
		uint8_t				read_byte(uint16_t addr);
		void				write_byte(uint16_t addr, uint8_t val);
		uint16_t			read_word(uint16_t addr);
		void				write_word(uint16_t addr, uint16_t val);

	private:
		static Memory		_instance;
		uint8_t				_m_rom_bank0[16384];
		uint8_t				_m_rom_bank1[16384];
		uint8_t				_m_wram[8][4095];
		uint8_t				_m_vram[16384];
		uint8_t				_m_oam[160];
		uint8_t				_m_io[127];

		Memory(void);
		~Memory(void);
};

#endif