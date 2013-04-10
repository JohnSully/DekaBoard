/* The following 4K block of ROM is pre-computed bit-bang values for
	JTAG programming.  This saves considerable time when configuring the FPGA*/

#if 0
	// Code snippet to compute this table
	for (ib = 0; ib <= 0xff; ib++)
	{
		for (it = 0; it < 16; it++)
		{
			unsigned char *pb;
			pb = (jtag_bitvals_ram + (ib * 16) + it);
			if ((ib >> (it/2)) & 1)
				*pb = TDI_MASK;
			else
				*pb = 0;
	
			if (it & 1)
				*pb |= TCK_MASK;
			*pb |= LED_GREEN_OFF;
		}
	}
#endif
	
rom const unsigned char jtag_bitvals[0x100][16] = {
{0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1}, // 0x0
{2, 3, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1}, // 0x1
{0, 1, 2, 3, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1}, // 0x2
{2, 3, 2, 3, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1}, // 0x3
{0, 1, 0, 1, 2, 3, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1}, // 0x4
{2, 3, 0, 1, 2, 3, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1}, // 0x5
{0, 1, 2, 3, 2, 3, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1}, // 0x6
{2, 3, 2, 3, 2, 3, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1}, // 0x7
{0, 1, 0, 1, 0, 1, 2, 3, 0, 1, 0, 1, 0, 1, 0, 1}, // 0x8
{2, 3, 0, 1, 0, 1, 2, 3, 0, 1, 0, 1, 0, 1, 0, 1}, // 0x9
{0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 0, 1, 0, 1, 0, 1}, // 0xA
{2, 3, 2, 3, 0, 1, 2, 3, 0, 1, 0, 1, 0, 1, 0, 1}, // 0xB
{0, 1, 0, 1, 2, 3, 2, 3, 0, 1, 0, 1, 0, 1, 0, 1}, // 0xC
{2, 3, 0, 1, 2, 3, 2, 3, 0, 1, 0, 1, 0, 1, 0, 1}, // 0xD
{0, 1, 2, 3, 2, 3, 2, 3, 0, 1, 0, 1, 0, 1, 0, 1}, // 0xE
{2, 3, 2, 3, 2, 3, 2, 3, 0, 1, 0, 1, 0, 1, 0, 1}, // 0xF
{0, 1, 0, 1, 0, 1, 0, 1, 2, 3, 0, 1, 0, 1, 0, 1}, // 0x10
{2, 3, 0, 1, 0, 1, 0, 1, 2, 3, 0, 1, 0, 1, 0, 1}, // 0x11
{0, 1, 2, 3, 0, 1, 0, 1, 2, 3, 0, 1, 0, 1, 0, 1}, // 0x12
{2, 3, 2, 3, 0, 1, 0, 1, 2, 3, 0, 1, 0, 1, 0, 1}, // 0x13
{0, 1, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 0, 1, 0, 1}, // 0x14
{2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 0, 1, 0, 1}, // 0x15
{0, 1, 2, 3, 2, 3, 0, 1, 2, 3, 0, 1, 0, 1, 0, 1}, // 0x16
{2, 3, 2, 3, 2, 3, 0, 1, 2, 3, 0, 1, 0, 1, 0, 1}, // 0x17
{0, 1, 0, 1, 0, 1, 2, 3, 2, 3, 0, 1, 0, 1, 0, 1}, // 0x18
{2, 3, 0, 1, 0, 1, 2, 3, 2, 3, 0, 1, 0, 1, 0, 1}, // 0x19
{0, 1, 2, 3, 0, 1, 2, 3, 2, 3, 0, 1, 0, 1, 0, 1}, // 0x1A
{2, 3, 2, 3, 0, 1, 2, 3, 2, 3, 0, 1, 0, 1, 0, 1}, // 0x1B
{0, 1, 0, 1, 2, 3, 2, 3, 2, 3, 0, 1, 0, 1, 0, 1}, // 0x1C
{2, 3, 0, 1, 2, 3, 2, 3, 2, 3, 0, 1, 0, 1, 0, 1}, // 0x1D
{0, 1, 2, 3, 2, 3, 2, 3, 2, 3, 0, 1, 0, 1, 0, 1}, // 0x1E
{2, 3, 2, 3, 2, 3, 2, 3, 2, 3, 0, 1, 0, 1, 0, 1}, // 0x1F
{0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 2, 3, 0, 1, 0, 1}, // 0x20
{2, 3, 0, 1, 0, 1, 0, 1, 0, 1, 2, 3, 0, 1, 0, 1}, // 0x21
{0, 1, 2, 3, 0, 1, 0, 1, 0, 1, 2, 3, 0, 1, 0, 1}, // 0x22
{2, 3, 2, 3, 0, 1, 0, 1, 0, 1, 2, 3, 0, 1, 0, 1}, // 0x23
{0, 1, 0, 1, 2, 3, 0, 1, 0, 1, 2, 3, 0, 1, 0, 1}, // 0x24
{2, 3, 0, 1, 2, 3, 0, 1, 0, 1, 2, 3, 0, 1, 0, 1}, // 0x25
{0, 1, 2, 3, 2, 3, 0, 1, 0, 1, 2, 3, 0, 1, 0, 1}, // 0x26
{2, 3, 2, 3, 2, 3, 0, 1, 0, 1, 2, 3, 0, 1, 0, 1}, // 0x27
{0, 1, 0, 1, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 0, 1}, // 0x28
{2, 3, 0, 1, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 0, 1}, // 0x29
{0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 0, 1}, // 0x2A
{2, 3, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 0, 1}, // 0x2B
{0, 1, 0, 1, 2, 3, 2, 3, 0, 1, 2, 3, 0, 1, 0, 1}, // 0x2C
{2, 3, 0, 1, 2, 3, 2, 3, 0, 1, 2, 3, 0, 1, 0, 1}, // 0x2D
{0, 1, 2, 3, 2, 3, 2, 3, 0, 1, 2, 3, 0, 1, 0, 1}, // 0x2E
{2, 3, 2, 3, 2, 3, 2, 3, 0, 1, 2, 3, 0, 1, 0, 1}, // 0x2F
{0, 1, 0, 1, 0, 1, 0, 1, 2, 3, 2, 3, 0, 1, 0, 1}, // 0x30
{2, 3, 0, 1, 0, 1, 0, 1, 2, 3, 2, 3, 0, 1, 0, 1}, // 0x31
{0, 1, 2, 3, 0, 1, 0, 1, 2, 3, 2, 3, 0, 1, 0, 1}, // 0x32
{2, 3, 2, 3, 0, 1, 0, 1, 2, 3, 2, 3, 0, 1, 0, 1}, // 0x33
{0, 1, 0, 1, 2, 3, 0, 1, 2, 3, 2, 3, 0, 1, 0, 1}, // 0x34
{2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 2, 3, 0, 1, 0, 1}, // 0x35
{0, 1, 2, 3, 2, 3, 0, 1, 2, 3, 2, 3, 0, 1, 0, 1}, // 0x36
{2, 3, 2, 3, 2, 3, 0, 1, 2, 3, 2, 3, 0, 1, 0, 1}, // 0x37
{0, 1, 0, 1, 0, 1, 2, 3, 2, 3, 2, 3, 0, 1, 0, 1}, // 0x38
{2, 3, 0, 1, 0, 1, 2, 3, 2, 3, 2, 3, 0, 1, 0, 1}, // 0x39
{0, 1, 2, 3, 0, 1, 2, 3, 2, 3, 2, 3, 0, 1, 0, 1}, // 0x3A
{2, 3, 2, 3, 0, 1, 2, 3, 2, 3, 2, 3, 0, 1, 0, 1}, // 0x3B
{0, 1, 0, 1, 2, 3, 2, 3, 2, 3, 2, 3, 0, 1, 0, 1}, // 0x3C
{2, 3, 0, 1, 2, 3, 2, 3, 2, 3, 2, 3, 0, 1, 0, 1}, // 0x3D
{0, 1, 2, 3, 2, 3, 2, 3, 2, 3, 2, 3, 0, 1, 0, 1}, // 0x3E
{2, 3, 2, 3, 2, 3, 2, 3, 2, 3, 2, 3, 0, 1, 0, 1}, // 0x3F
{0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 2, 3, 0, 1}, // 0x40
{2, 3, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 2, 3, 0, 1}, // 0x41
{0, 1, 2, 3, 0, 1, 0, 1, 0, 1, 0, 1, 2, 3, 0, 1}, // 0x42
{2, 3, 2, 3, 0, 1, 0, 1, 0, 1, 0, 1, 2, 3, 0, 1}, // 0x43
{0, 1, 0, 1, 2, 3, 0, 1, 0, 1, 0, 1, 2, 3, 0, 1}, // 0x44
{2, 3, 0, 1, 2, 3, 0, 1, 0, 1, 0, 1, 2, 3, 0, 1}, // 0x45
{0, 1, 2, 3, 2, 3, 0, 1, 0, 1, 0, 1, 2, 3, 0, 1}, // 0x46
{2, 3, 2, 3, 2, 3, 0, 1, 0, 1, 0, 1, 2, 3, 0, 1}, // 0x47
{0, 1, 0, 1, 0, 1, 2, 3, 0, 1, 0, 1, 2, 3, 0, 1}, // 0x48
{2, 3, 0, 1, 0, 1, 2, 3, 0, 1, 0, 1, 2, 3, 0, 1}, // 0x49
{0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 0, 1, 2, 3, 0, 1}, // 0x4A
{2, 3, 2, 3, 0, 1, 2, 3, 0, 1, 0, 1, 2, 3, 0, 1}, // 0x4B
{0, 1, 0, 1, 2, 3, 2, 3, 0, 1, 0, 1, 2, 3, 0, 1}, // 0x4C
{2, 3, 0, 1, 2, 3, 2, 3, 0, 1, 0, 1, 2, 3, 0, 1}, // 0x4D
{0, 1, 2, 3, 2, 3, 2, 3, 0, 1, 0, 1, 2, 3, 0, 1}, // 0x4E
{2, 3, 2, 3, 2, 3, 2, 3, 0, 1, 0, 1, 2, 3, 0, 1}, // 0x4F
{0, 1, 0, 1, 0, 1, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1}, // 0x50
{2, 3, 0, 1, 0, 1, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1}, // 0x51
{0, 1, 2, 3, 0, 1, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1}, // 0x52
{2, 3, 2, 3, 0, 1, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1}, // 0x53
{0, 1, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1}, // 0x54
{2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1}, // 0x55
{0, 1, 2, 3, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1}, // 0x56
{2, 3, 2, 3, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1}, // 0x57
{0, 1, 0, 1, 0, 1, 2, 3, 2, 3, 0, 1, 2, 3, 0, 1}, // 0x58
{2, 3, 0, 1, 0, 1, 2, 3, 2, 3, 0, 1, 2, 3, 0, 1}, // 0x59
{0, 1, 2, 3, 0, 1, 2, 3, 2, 3, 0, 1, 2, 3, 0, 1}, // 0x5A
{2, 3, 2, 3, 0, 1, 2, 3, 2, 3, 0, 1, 2, 3, 0, 1}, // 0x5B
{0, 1, 0, 1, 2, 3, 2, 3, 2, 3, 0, 1, 2, 3, 0, 1}, // 0x5C
{2, 3, 0, 1, 2, 3, 2, 3, 2, 3, 0, 1, 2, 3, 0, 1}, // 0x5D
{0, 1, 2, 3, 2, 3, 2, 3, 2, 3, 0, 1, 2, 3, 0, 1}, // 0x5E
{2, 3, 2, 3, 2, 3, 2, 3, 2, 3, 0, 1, 2, 3, 0, 1}, // 0x5F
{0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 2, 3, 2, 3, 0, 1}, // 0x60
{2, 3, 0, 1, 0, 1, 0, 1, 0, 1, 2, 3, 2, 3, 0, 1}, // 0x61
{0, 1, 2, 3, 0, 1, 0, 1, 0, 1, 2, 3, 2, 3, 0, 1}, // 0x62
{2, 3, 2, 3, 0, 1, 0, 1, 0, 1, 2, 3, 2, 3, 0, 1}, // 0x63
{0, 1, 0, 1, 2, 3, 0, 1, 0, 1, 2, 3, 2, 3, 0, 1}, // 0x64
{2, 3, 0, 1, 2, 3, 0, 1, 0, 1, 2, 3, 2, 3, 0, 1}, // 0x65
{0, 1, 2, 3, 2, 3, 0, 1, 0, 1, 2, 3, 2, 3, 0, 1}, // 0x66
{2, 3, 2, 3, 2, 3, 0, 1, 0, 1, 2, 3, 2, 3, 0, 1}, // 0x67
{0, 1, 0, 1, 0, 1, 2, 3, 0, 1, 2, 3, 2, 3, 0, 1}, // 0x68
{2, 3, 0, 1, 0, 1, 2, 3, 0, 1, 2, 3, 2, 3, 0, 1}, // 0x69
{0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 2, 3, 0, 1}, // 0x6A
{2, 3, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 2, 3, 0, 1}, // 0x6B
{0, 1, 0, 1, 2, 3, 2, 3, 0, 1, 2, 3, 2, 3, 0, 1}, // 0x6C
{2, 3, 0, 1, 2, 3, 2, 3, 0, 1, 2, 3, 2, 3, 0, 1}, // 0x6D
{0, 1, 2, 3, 2, 3, 2, 3, 0, 1, 2, 3, 2, 3, 0, 1}, // 0x6E
{2, 3, 2, 3, 2, 3, 2, 3, 0, 1, 2, 3, 2, 3, 0, 1}, // 0x6F
{0, 1, 0, 1, 0, 1, 0, 1, 2, 3, 2, 3, 2, 3, 0, 1}, // 0x70
{2, 3, 0, 1, 0, 1, 0, 1, 2, 3, 2, 3, 2, 3, 0, 1}, // 0x71
{0, 1, 2, 3, 0, 1, 0, 1, 2, 3, 2, 3, 2, 3, 0, 1}, // 0x72
{2, 3, 2, 3, 0, 1, 0, 1, 2, 3, 2, 3, 2, 3, 0, 1}, // 0x73
{0, 1, 0, 1, 2, 3, 0, 1, 2, 3, 2, 3, 2, 3, 0, 1}, // 0x74
{2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 2, 3, 2, 3, 0, 1}, // 0x75
{0, 1, 2, 3, 2, 3, 0, 1, 2, 3, 2, 3, 2, 3, 0, 1}, // 0x76
{2, 3, 2, 3, 2, 3, 0, 1, 2, 3, 2, 3, 2, 3, 0, 1}, // 0x77
{0, 1, 0, 1, 0, 1, 2, 3, 2, 3, 2, 3, 2, 3, 0, 1}, // 0x78
{2, 3, 0, 1, 0, 1, 2, 3, 2, 3, 2, 3, 2, 3, 0, 1}, // 0x79
{0, 1, 2, 3, 0, 1, 2, 3, 2, 3, 2, 3, 2, 3, 0, 1}, // 0x7A
{2, 3, 2, 3, 0, 1, 2, 3, 2, 3, 2, 3, 2, 3, 0, 1}, // 0x7B
{0, 1, 0, 1, 2, 3, 2, 3, 2, 3, 2, 3, 2, 3, 0, 1}, // 0x7C
{2, 3, 0, 1, 2, 3, 2, 3, 2, 3, 2, 3, 2, 3, 0, 1}, // 0x7D
{0, 1, 2, 3, 2, 3, 2, 3, 2, 3, 2, 3, 2, 3, 0, 1}, // 0x7E
{2, 3, 2, 3, 2, 3, 2, 3, 2, 3, 2, 3, 2, 3, 0, 1}, // 0x7F
{0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 2, 3}, // 0x80
{2, 3, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 2, 3}, // 0x81
{0, 1, 2, 3, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 2, 3}, // 0x82
{2, 3, 2, 3, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 2, 3}, // 0x83
{0, 1, 0, 1, 2, 3, 0, 1, 0, 1, 0, 1, 0, 1, 2, 3}, // 0x84
{2, 3, 0, 1, 2, 3, 0, 1, 0, 1, 0, 1, 0, 1, 2, 3}, // 0x85
{0, 1, 2, 3, 2, 3, 0, 1, 0, 1, 0, 1, 0, 1, 2, 3}, // 0x86
{2, 3, 2, 3, 2, 3, 0, 1, 0, 1, 0, 1, 0, 1, 2, 3}, // 0x87
{0, 1, 0, 1, 0, 1, 2, 3, 0, 1, 0, 1, 0, 1, 2, 3}, // 0x88
{2, 3, 0, 1, 0, 1, 2, 3, 0, 1, 0, 1, 0, 1, 2, 3}, // 0x89
{0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 0, 1, 0, 1, 2, 3}, // 0x8A
{2, 3, 2, 3, 0, 1, 2, 3, 0, 1, 0, 1, 0, 1, 2, 3}, // 0x8B
{0, 1, 0, 1, 2, 3, 2, 3, 0, 1, 0, 1, 0, 1, 2, 3}, // 0x8C
{2, 3, 0, 1, 2, 3, 2, 3, 0, 1, 0, 1, 0, 1, 2, 3}, // 0x8D
{0, 1, 2, 3, 2, 3, 2, 3, 0, 1, 0, 1, 0, 1, 2, 3}, // 0x8E
{2, 3, 2, 3, 2, 3, 2, 3, 0, 1, 0, 1, 0, 1, 2, 3}, // 0x8F
{0, 1, 0, 1, 0, 1, 0, 1, 2, 3, 0, 1, 0, 1, 2, 3}, // 0x90
{2, 3, 0, 1, 0, 1, 0, 1, 2, 3, 0, 1, 0, 1, 2, 3}, // 0x91
{0, 1, 2, 3, 0, 1, 0, 1, 2, 3, 0, 1, 0, 1, 2, 3}, // 0x92
{2, 3, 2, 3, 0, 1, 0, 1, 2, 3, 0, 1, 0, 1, 2, 3}, // 0x93
{0, 1, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 0, 1, 2, 3}, // 0x94
{2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 0, 1, 2, 3}, // 0x95
{0, 1, 2, 3, 2, 3, 0, 1, 2, 3, 0, 1, 0, 1, 2, 3}, // 0x96
{2, 3, 2, 3, 2, 3, 0, 1, 2, 3, 0, 1, 0, 1, 2, 3}, // 0x97
{0, 1, 0, 1, 0, 1, 2, 3, 2, 3, 0, 1, 0, 1, 2, 3}, // 0x98
{2, 3, 0, 1, 0, 1, 2, 3, 2, 3, 0, 1, 0, 1, 2, 3}, // 0x99
{0, 1, 2, 3, 0, 1, 2, 3, 2, 3, 0, 1, 0, 1, 2, 3}, // 0x9A
{2, 3, 2, 3, 0, 1, 2, 3, 2, 3, 0, 1, 0, 1, 2, 3}, // 0x9B
{0, 1, 0, 1, 2, 3, 2, 3, 2, 3, 0, 1, 0, 1, 2, 3}, // 0x9C
{2, 3, 0, 1, 2, 3, 2, 3, 2, 3, 0, 1, 0, 1, 2, 3}, // 0x9D
{0, 1, 2, 3, 2, 3, 2, 3, 2, 3, 0, 1, 0, 1, 2, 3}, // 0x9E
{2, 3, 2, 3, 2, 3, 2, 3, 2, 3, 0, 1, 0, 1, 2, 3}, // 0x9F
{0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 2, 3, 0, 1, 2, 3}, // 0xA0
{2, 3, 0, 1, 0, 1, 0, 1, 0, 1, 2, 3, 0, 1, 2, 3}, // 0xA1
{0, 1, 2, 3, 0, 1, 0, 1, 0, 1, 2, 3, 0, 1, 2, 3}, // 0xA2
{2, 3, 2, 3, 0, 1, 0, 1, 0, 1, 2, 3, 0, 1, 2, 3}, // 0xA3
{0, 1, 0, 1, 2, 3, 0, 1, 0, 1, 2, 3, 0, 1, 2, 3}, // 0xA4
{2, 3, 0, 1, 2, 3, 0, 1, 0, 1, 2, 3, 0, 1, 2, 3}, // 0xA5
{0, 1, 2, 3, 2, 3, 0, 1, 0, 1, 2, 3, 0, 1, 2, 3}, // 0xA6
{2, 3, 2, 3, 2, 3, 0, 1, 0, 1, 2, 3, 0, 1, 2, 3}, // 0xA7
{0, 1, 0, 1, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3}, // 0xA8
{2, 3, 0, 1, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3}, // 0xA9
{0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3}, // 0xAA
{2, 3, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3}, // 0xAB
{0, 1, 0, 1, 2, 3, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3}, // 0xAC
{2, 3, 0, 1, 2, 3, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3}, // 0xAD
{0, 1, 2, 3, 2, 3, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3}, // 0xAE
{2, 3, 2, 3, 2, 3, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3}, // 0xAF
{0, 1, 0, 1, 0, 1, 0, 1, 2, 3, 2, 3, 0, 1, 2, 3}, // 0xB0
{2, 3, 0, 1, 0, 1, 0, 1, 2, 3, 2, 3, 0, 1, 2, 3}, // 0xB1
{0, 1, 2, 3, 0, 1, 0, 1, 2, 3, 2, 3, 0, 1, 2, 3}, // 0xB2
{2, 3, 2, 3, 0, 1, 0, 1, 2, 3, 2, 3, 0, 1, 2, 3}, // 0xB3
{0, 1, 0, 1, 2, 3, 0, 1, 2, 3, 2, 3, 0, 1, 2, 3}, // 0xB4
{2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 2, 3, 0, 1, 2, 3}, // 0xB5
{0, 1, 2, 3, 2, 3, 0, 1, 2, 3, 2, 3, 0, 1, 2, 3}, // 0xB6
{2, 3, 2, 3, 2, 3, 0, 1, 2, 3, 2, 3, 0, 1, 2, 3}, // 0xB7
{0, 1, 0, 1, 0, 1, 2, 3, 2, 3, 2, 3, 0, 1, 2, 3}, // 0xB8
{2, 3, 0, 1, 0, 1, 2, 3, 2, 3, 2, 3, 0, 1, 2, 3}, // 0xB9
{0, 1, 2, 3, 0, 1, 2, 3, 2, 3, 2, 3, 0, 1, 2, 3}, // 0xBA
{2, 3, 2, 3, 0, 1, 2, 3, 2, 3, 2, 3, 0, 1, 2, 3}, // 0xBB
{0, 1, 0, 1, 2, 3, 2, 3, 2, 3, 2, 3, 0, 1, 2, 3}, // 0xBC
{2, 3, 0, 1, 2, 3, 2, 3, 2, 3, 2, 3, 0, 1, 2, 3}, // 0xBD
{0, 1, 2, 3, 2, 3, 2, 3, 2, 3, 2, 3, 0, 1, 2, 3}, // 0xBE
{2, 3, 2, 3, 2, 3, 2, 3, 2, 3, 2, 3, 0, 1, 2, 3}, // 0xBF
{0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 2, 3, 2, 3}, // 0xC0
{2, 3, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 2, 3, 2, 3}, // 0xC1
{0, 1, 2, 3, 0, 1, 0, 1, 0, 1, 0, 1, 2, 3, 2, 3}, // 0xC2
{2, 3, 2, 3, 0, 1, 0, 1, 0, 1, 0, 1, 2, 3, 2, 3}, // 0xC3
{0, 1, 0, 1, 2, 3, 0, 1, 0, 1, 0, 1, 2, 3, 2, 3}, // 0xC4
{2, 3, 0, 1, 2, 3, 0, 1, 0, 1, 0, 1, 2, 3, 2, 3}, // 0xC5
{0, 1, 2, 3, 2, 3, 0, 1, 0, 1, 0, 1, 2, 3, 2, 3}, // 0xC6
{2, 3, 2, 3, 2, 3, 0, 1, 0, 1, 0, 1, 2, 3, 2, 3}, // 0xC7
{0, 1, 0, 1, 0, 1, 2, 3, 0, 1, 0, 1, 2, 3, 2, 3}, // 0xC8
{2, 3, 0, 1, 0, 1, 2, 3, 0, 1, 0, 1, 2, 3, 2, 3}, // 0xC9
{0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 0, 1, 2, 3, 2, 3}, // 0xCA
{2, 3, 2, 3, 0, 1, 2, 3, 0, 1, 0, 1, 2, 3, 2, 3}, // 0xCB
{0, 1, 0, 1, 2, 3, 2, 3, 0, 1, 0, 1, 2, 3, 2, 3}, // 0xCC
{2, 3, 0, 1, 2, 3, 2, 3, 0, 1, 0, 1, 2, 3, 2, 3}, // 0xCD
{0, 1, 2, 3, 2, 3, 2, 3, 0, 1, 0, 1, 2, 3, 2, 3}, // 0xCE
{2, 3, 2, 3, 2, 3, 2, 3, 0, 1, 0, 1, 2, 3, 2, 3}, // 0xCF
{0, 1, 0, 1, 0, 1, 0, 1, 2, 3, 0, 1, 2, 3, 2, 3}, // 0xD0
{2, 3, 0, 1, 0, 1, 0, 1, 2, 3, 0, 1, 2, 3, 2, 3}, // 0xD1
{0, 1, 2, 3, 0, 1, 0, 1, 2, 3, 0, 1, 2, 3, 2, 3}, // 0xD2
{2, 3, 2, 3, 0, 1, 0, 1, 2, 3, 0, 1, 2, 3, 2, 3}, // 0xD3
{0, 1, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 2, 3}, // 0xD4
{2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 2, 3}, // 0xD5
{0, 1, 2, 3, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 2, 3}, // 0xD6
{2, 3, 2, 3, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 2, 3}, // 0xD7
{0, 1, 0, 1, 0, 1, 2, 3, 2, 3, 0, 1, 2, 3, 2, 3}, // 0xD8
{2, 3, 0, 1, 0, 1, 2, 3, 2, 3, 0, 1, 2, 3, 2, 3}, // 0xD9
{0, 1, 2, 3, 0, 1, 2, 3, 2, 3, 0, 1, 2, 3, 2, 3}, // 0xDA
{2, 3, 2, 3, 0, 1, 2, 3, 2, 3, 0, 1, 2, 3, 2, 3}, // 0xDB
{0, 1, 0, 1, 2, 3, 2, 3, 2, 3, 0, 1, 2, 3, 2, 3}, // 0xDC
{2, 3, 0, 1, 2, 3, 2, 3, 2, 3, 0, 1, 2, 3, 2, 3}, // 0xDD
{0, 1, 2, 3, 2, 3, 2, 3, 2, 3, 0, 1, 2, 3, 2, 3}, // 0xDE
{2, 3, 2, 3, 2, 3, 2, 3, 2, 3, 0, 1, 2, 3, 2, 3}, // 0xDF
{0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 2, 3, 2, 3, 2, 3}, // 0xE0
{2, 3, 0, 1, 0, 1, 0, 1, 0, 1, 2, 3, 2, 3, 2, 3}, // 0xE1
{0, 1, 2, 3, 0, 1, 0, 1, 0, 1, 2, 3, 2, 3, 2, 3}, // 0xE2
{2, 3, 2, 3, 0, 1, 0, 1, 0, 1, 2, 3, 2, 3, 2, 3}, // 0xE3
{0, 1, 0, 1, 2, 3, 0, 1, 0, 1, 2, 3, 2, 3, 2, 3}, // 0xE4
{2, 3, 0, 1, 2, 3, 0, 1, 0, 1, 2, 3, 2, 3, 2, 3}, // 0xE5
{0, 1, 2, 3, 2, 3, 0, 1, 0, 1, 2, 3, 2, 3, 2, 3}, // 0xE6
{2, 3, 2, 3, 2, 3, 0, 1, 0, 1, 2, 3, 2, 3, 2, 3}, // 0xE7
{0, 1, 0, 1, 0, 1, 2, 3, 0, 1, 2, 3, 2, 3, 2, 3}, // 0xE8
{2, 3, 0, 1, 0, 1, 2, 3, 0, 1, 2, 3, 2, 3, 2, 3}, // 0xE9
{0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 2, 3, 2, 3}, // 0xEA
{2, 3, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 2, 3, 2, 3}, // 0xEB
{0, 1, 0, 1, 2, 3, 2, 3, 0, 1, 2, 3, 2, 3, 2, 3}, // 0xEC
{2, 3, 0, 1, 2, 3, 2, 3, 0, 1, 2, 3, 2, 3, 2, 3}, // 0xED
{0, 1, 2, 3, 2, 3, 2, 3, 0, 1, 2, 3, 2, 3, 2, 3}, // 0xEE
{2, 3, 2, 3, 2, 3, 2, 3, 0, 1, 2, 3, 2, 3, 2, 3}, // 0xEF
{0, 1, 0, 1, 0, 1, 0, 1, 2, 3, 2, 3, 2, 3, 2, 3}, // 0xF0
{2, 3, 0, 1, 0, 1, 0, 1, 2, 3, 2, 3, 2, 3, 2, 3}, // 0xF1
{0, 1, 2, 3, 0, 1, 0, 1, 2, 3, 2, 3, 2, 3, 2, 3}, // 0xF2
{2, 3, 2, 3, 0, 1, 0, 1, 2, 3, 2, 3, 2, 3, 2, 3}, // 0xF3
{0, 1, 0, 1, 2, 3, 0, 1, 2, 3, 2, 3, 2, 3, 2, 3}, // 0xF4
{2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 2, 3, 2, 3, 2, 3}, // 0xF5
{0, 1, 2, 3, 2, 3, 0, 1, 2, 3, 2, 3, 2, 3, 2, 3}, // 0xF6
{2, 3, 2, 3, 2, 3, 0, 1, 2, 3, 2, 3, 2, 3, 2, 3}, // 0xF7
{0, 1, 0, 1, 0, 1, 2, 3, 2, 3, 2, 3, 2, 3, 2, 3}, // 0xF8
{2, 3, 0, 1, 0, 1, 2, 3, 2, 3, 2, 3, 2, 3, 2, 3}, // 0xF9
{0, 1, 2, 3, 0, 1, 2, 3, 2, 3, 2, 3, 2, 3, 2, 3}, // 0xFA
{2, 3, 2, 3, 0, 1, 2, 3, 2, 3, 2, 3, 2, 3, 2, 3}, // 0xFB
{0, 1, 0, 1, 2, 3, 2, 3, 2, 3, 2, 3, 2, 3, 2, 3}, // 0xFC
{2, 3, 0, 1, 2, 3, 2, 3, 2, 3, 2, 3, 2, 3, 2, 3}, // 0xFD
{0, 1, 2, 3, 2, 3, 2, 3, 2, 3, 2, 3, 2, 3, 2, 3}, // 0xFE
{2, 3, 2, 3, 2, 3, 2, 3, 2, 3, 2, 3, 2, 3, 2, 3} // 0xFF
};
