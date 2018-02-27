/* Game state demo test */

#include <gb/gb.h>

#include "alpha.c"

#define GS_MENU = 0x00;
#define GS_PLAY = 0x01;

void update_switches();
void init_device();
void input_process();

UBYTE i, j, k; // Define globals for loops as to avoid declaring variables onto the stack

/* Variables for player */
UBYTE player_loc[2]; // store x,y location of sprite
UBYTE health = 10;

/* Variables for other entities */
UBYTE entities[3][5]; // store x,y and status

/* graphics data variables */
UWORD idx;

/* 8x8 description of color, in this case all white/blank pixels */ 
const UBYTE blankTile[] ={ 
    0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00
};

/* 20 tile spaces pointing at tile address 0 */ 
const UBYTE blankLine[] ={
	0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00
};

const UBYTE helloWorld[] =
{
  0x19,0x16,0x1D,0x1D,0x20, //Hello
  0x28,0x20,0x23,0x1D,0x15  //World
};

void update_switches() {
	HIDE_WIN;
	SHOW_SPRITES;
	SHOW_BKG;
}

void init_device() {
	DISPLAY_ON;
	NR52_REG = 0x8F;
	NR51_REG = 0x11;
	NR50_REG = 0x77;
	// Wipe Nintendo logo tile locations back to default of 0
	set_bkg_tiles(0, 8, 20, 1, blankLine);
	set_bkg_tiles(0, 9, 20, 1, blankLine);

	set_bkg_data(1, 48, alpha);

}

void input_process() {
	if (joypad() & J_B) {
		set_bkg_tiles(5, 0, 10, 1, helloWorld); // Start at x = 5, y = 0, copy 10 tiles from helloWorld into a single line
	}
}

UBYTE rect_collision(UBYTE x1, UBYTE y1, UBYTE w1, UBYTE h1, // object 1
					UBYTE x2, UBYTE y2, UBYTE w2, UBYTE h2) { // object 2
	
	if ((x1 < (x2+w2)) && ((x1+w1) > x2) && (y1 < (h2+y2)) && ((y1+h1) > y2)) {
		return 1;
	}
	return 0;
}

void main()
{
    // Set up initial device settings
	init_device();

	while(1) {
		if ()
		input_process();
		update_switches();

		// Wait so we only process once a frame
		wait_vbl_done();
	}
}