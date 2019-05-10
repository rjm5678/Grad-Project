#include "sys.h"

/* RGB565 to RGB24, return 1 means ok*/
u8 RGB565toRGB24(u16 RGB565, u8* RGB24)
{
	u8 R, G, B;
	
	R = (RGB565 & 0xF800) >> 8;
	G = (RGB565 & 0x07E0) >> 3;
	B = (RGB565 & 0x001F) << 3;
	
	RGB24[0] = R;
	RGB24[1] = G;
	RGB24[2] = B;
	
	return 1;
}
