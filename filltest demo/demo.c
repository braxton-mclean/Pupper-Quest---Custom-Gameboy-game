/* Sample Program to demonstrate the drawing functions in GBDK */
/* Jon Fuge jonny@q-continuum.demon.co.uk */

#include <stdio.h>
#include <gb/gb.h>
#include <gb/font.h>
#include <gb/console.h>
#include <gb/drawing.h>

#define SCROLLING 0x01
#define FINISHED_SCROLLING 0x02
#define AA_SCROLL_TEST 0x01
#define AA_MAIN_MENU 0x02
#define GS_IDLE 0x01
#define GS_MENU 0x02

/* Dummy variables for loops */
UBYTE a, b, c, d, e;

UBYTE time;

UBYTE joypad_input;

/* Variables for scrolling */
UBYTE horizontal_line, vertical_line, scroll_status;

/* Area state of our game, used to define game screen presence (main menu, game level 1, etc.) */
UBYTE active_area;
/* State of our game, paused, menu, etc.) */
UBYTE game_state;

/* graphics data variables */
UWORD idx;
const UBYTE blankTile[] ={ 
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};

void clear_screen_inefficiently();
void clear_screen();
void init_filler_screen();
void init_scroll_screen();
void scroll_screen();
void init_scroll_test_menu();
void init_scroll_test_idle();
void gl_scroll_test();
void gl_main_menu();

/* The most inefficient method to clear the screen, looks cool though */ 
void clear_screen_inefficiently() {
    for (vertical_line=0; vertical_line!=143; vertical_line++) { /* vertical offset */
       for (horizontal_line=0; horizontal_line!=160; horizontal_line++) { /*horizontal offset */
            color(WHITE,WHITE,SOLID);
            plot_point(horizontal_line,vertical_line);
        }
    }
    line(0,143,159,143);
}

/* Clear the screen in a faster, smarter way, way less cool */
/* DONT USE Currently does not work consistently, am unsure why though... Seems to be an inconsistent nature of the register that handles the addresses for VRAM sheet starts, cant tell if its the emulator or my code*/
void clear_screen() {
    display_off;
    disable_interrupts();
    for(idx=16; idx != 0xFF; idx++) { //Changed to 16 for hope that maybe its the nintendo logo that has to stay around?
        set_bkg_data(idx, 0x01, blankTile);
        set_sprite_data(idx, 0x01, blankTile);
    }
    enable_interrupts();
    DISPLAY_ON;
}

void init_filler_screen() {
    c = 0;

    /* Some filler-text */
    for (a=0; a!=18; a++) {
        for (b=0; b!=16; b++) {
            gotogxy(b,a);
            d=a/4;
            e=b/4;
            if (d==e) {
              d=3-e;
            }
            color(d,e,SOLID);
            gprintf("%c",c++);
            delay(10);
        } 
    }
}

void init_scroll_screen() {
    vertical_line = 0;
    horizontal_line = 0;
    scroll_status = SCROLLING;
}

void scroll_screen() {
    if(vertical_line!=143) {
        for (horizontal_line=0; horizontal_line!=160; horizontal_line++) { /*horizontal offset */
            color(getpix(horizontal_line,vertical_line+1),WHITE,SOLID);
            plot_point(horizontal_line,vertical_line);
        }
        color(WHITE,WHITE,SOLID);
        vertical_line++;
    } else {
        line(0,143,159,143);
        scroll_status = FINISHED_SCROLLING;
    }
    /* Scroll the screen using the hardest method imaginable :) */
    //for (vertical_line=0; vertical_line<=142; vertical_line++) { /* vertical offset */
    //  for (horizontal_line=0; horizontal_line<=159; horizontal_line++) { /*horizontal offset */
    //        color(getpix(horizontal_line,vertical_line+1),WHITE,SOLID);
    //        plot_point(horizontal_line,vertical_line);
    //    }
    //    color(WHITE,WHITE,SOLID);
    //}
    //line(0,143,159,143);
}

void init_scroll_test_menu() {
    clear_screen_inefficiently();
    init_filler_screen();
    init_scroll_screen();
}

void init_scroll_test_idle() {
    clear_screen_inefficiently();

    /* Draw two circles, a line, and two boxes in different drawing modes */
    color(LTGREY,WHITE,SOLID);
    delay(500);
    circle(140,20,15,M_FILL);
    delay(500);
    color(BLACK,WHITE,SOLID);
    circle(140,20,10,M_NOFILL);
    delay(500);
    color(DKGREY,WHITE,XOR);
    circle(120,40,30,M_FILL);
    delay(500);
    line(0,0,159,143);
    color(BLACK,LTGREY,SOLID);
    delay(500);
    box(0,130,40,143,M_NOFILL);
    box(50,130,90,143,M_FILL);

    init_scroll_screen();
}

void init_main_menu_menu() {
    clear_screen_inefficiently();
    gotogxy(3,3);
    color(3,0,SOLID);
    gprintf("PUPPER  QUEST!");
    color(0,0,SOLID);
    line(0,143,159,143);
}

/* game logic for AA_SCROLL_TEST */
void gl_scroll_test(){
    if (joypad_input&J_SELECT) {
        if(game_state!=GS_MENU){
            game_state = GS_MENU;
            init_scroll_test_menu();
        }
    } else if (joypad_input&J_START) {
        if(active_area!=AA_MAIN_MENU) {
            active_area = AA_MAIN_MENU;
            game_state = GS_MENU;
            init_main_menu_menu();
        }
    }

    if(game_state&GS_IDLE) {
        if(scroll_status != FINISHED_SCROLLING) {
            scroll_screen();
        } else if((time&0x08)==0) {
            init_scroll_screen();
        }
    } else if (game_state&GS_MENU) {
        if(scroll_status != FINISHED_SCROLLING) {
            scroll_screen();
        } else if((time&0x08)==0) {
            init_scroll_screen();
        }
    }
}

/* game logic for AA_MAIN_MENU */
void gl_main_menu(){
    
}

void main()
{
    /* Ensure our gameboy is working */
    init_filler_screen();

    /* Base state logic setup for AA_SCROLL_TEST and GS_IDLE*/
    active_area = AA_SCROLL_TEST;
    game_state = GS_IDLE;
    init_scroll_test_idle();
    delay(1000);

    while(1) {
        time++;
        joypad_input = joypad();
        if(active_area&AA_SCROLL_TEST) {
            gl_scroll_test();
        } else if (active_area&AA_MAIN_MENU) {
            gl_main_menu();
        }
    }
}