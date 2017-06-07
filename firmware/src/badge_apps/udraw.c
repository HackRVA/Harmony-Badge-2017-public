
/*******************************************************************************
 * Author: Jonathan Lundquist / jonathan46000@gmail.com
 * Date: 5-24-17
 ******************************************************************************/

#include "app.h"
#include "colors.h"
#include "fb.h"
#include "assetList.h"
#include "buttons.h"
#include "utils.h"
#include "ir.h"

//ir bit masks
#define FIRST_PKT 0b101010101           //used to indicate a packet is first packet
#define BG_ID_MASK 0b011111111          //used to store data in pkt.p.badgeId
#define BG_ID_OUT 0b011111111           //used to retrieve data in pkt.p.badgeId
#define SET_HIGH_BIT 0b100000000        //set high bit on badge ID

//bit masks 32bit should change with pixel bit type
#define EMPTY_MASK 0b00000000000000000000000000000000 //black line
#define WHITE_MASK 0b11111111111111111111111111111111 //white line
#define MASK_1 0b00000000000000000000000000000001     //bit mask
#define MASK(INDEX) (MASK_1 << INDEX)                 //Indexing macro
#define DELETE_MASK(INDEX) (WHITE_MASK^MASK(INDEX))   //Indexing macro

//constants 
#define PIX_NUM 32                      //number of pixels high should match PIX_NUM_WIDE bit type
#define PIX_BYTES PIX_NUM/8             //number of bytes in a buffer line
#define SCREEN_WIDTH 128                //width of samsung display
#define PIX_WIDTH SCREEN_WIDTH/PIX_NUM  //width of each pixel

typedef uint32_t PIX_NUM_WIDE;          //number of writable pixels wide in bits

enum {                                  //drawing on or off enumerator
    DRAW_ON,
    DRAW_OFF,
};

enum {                                  //draw color control enumerator
    DRAW_BLACK,
    DRAW_WHITE,
    DRAW_RED,
    DRAW_GREEN,
    DRAW_BLUE,
    DRAW_YELLOW,
    DRAW_PURPLE,
    DRAW_CYAN,
};

enum {                                  //drop down menu control enum
    DRAW_DROP_DOWN_OFF,
    DRAW_DROP_DOWN_LEFT,
    DRAW_DROP_DOWN_RIGHT,
};

enum {
    DRAW_INIT,
    DRAW_RENDER,
    DRAW_GET_INPUT,
    DRAW_EXIT,
    DRAW_PROCESS,
};

//App IR send structs
struct short_halfs {                    //used to access halves of 16bits
    unsigned char upper;
    unsigned char lower;
};

union half_pkt {
    struct short_halfs bits8;           //union for breaking up packet data
    uint16_t bits16;
};

union line_byte_index {                 //union for breaking up image buffer data
    uint8_t bytes[4];
    uint32_t line;
};

//App image buffer struct
struct _image_buffer {                  //image buffer bit masks
    PIX_NUM_WIDE red[PIX_NUM];          //uint32 pixel wide red mask[32 element tall]
    PIX_NUM_WIDE green[PIX_NUM];        //uint32 pixel wide green mask[32 element tall]
    PIX_NUM_WIDE blue[PIX_NUM];         //uint32 pixel wide blue mask[32 element tall]
};

//App control structs
struct _cursor {                        //cursor position
    unsigned char x;
    unsigned char y;                                       
    unsigned char moved;                //cursor moved true or false
};

struct _draw_state {
    struct _cursor cursor;              //cursor pos struct
    unsigned char color;                //current draw color
    unsigned char on_off;               //drawing on or off true or false
    unsigned char state;                //current app state function
};

//public global vars
unsigned char image_received = 0;
unsigned char show_drop_down = DRAW_DROP_DOWN_OFF;
unsigned char show_ir_buff = 0;


//Game State Functions
void draw_init(void);
void draw_render(void);
void draw_get_input(void);
void draw_exit(void);
void draw_process(void);

//draw ir functions
unsigned char draw_get_first_line(struct _image_buffer img_buf);
unsigned char draw_get_last_line(struct _image_buffer img_buf);
unsigned char draw_is_empty(struct _image_buffer img_buf, unsigned char line);
void draw_send_packet(struct _image_buffer img_buf, unsigned char byte, unsigned char vert_index);
void draw_send_image(struct _image_buffer img_buf);
void draw_ir_udraw(struct IRpacket_t p);

//drop down menu function
void draw_drop_down(void);

//draw cursor location
void draw_cursor(struct _cursor pos);

//image read write functions
void draw_write_pixel(struct _image_buffer * img_buf,struct _draw_state g_data);
unsigned short draw_read_pixel(struct _image_buffer img_buf,unsigned char x, unsigned char y);
void draw_draw_pixel(unsigned char x, unsigned char y, unsigned short color);

//buffer indexing functions
unsigned char draw_read_pixel_data(PIX_NUM_WIDE const vert_index,unsigned const char horz_index);
void draw_write_pixel_data(PIX_NUM_WIDE * vert_index,unsigned const char horz_index);
void draw_remove_pixel_data(PIX_NUM_WIDE * vert_index,unsigned const char horz_index);

//Game control variables
struct _draw_state draw_state = {{0,0},DRAW_BLACK, DRAW_OFF,DRAW_INIT};
struct _image_buffer image_buffer;
struct _image_buffer ir_in_buffer;

//app loop
void udraw_task(void *p_arg){
    static unsigned char init = 1;
    if(init) {
        draw_state.state = DRAW_INIT;
        init=0;
    }
    for(;;) {
        switch(draw_state.state) {
            case DRAW_INIT:
                draw_init();
                break;
            case DRAW_RENDER:
                draw_render();
                break;
            case DRAW_GET_INPUT:
                draw_get_input();
                break;
            case DRAW_EXIT:
                draw_exit();
                break;
            case DRAW_PROCESS:
                draw_process();
                break;
            default:
                draw_exit();
        }
    } 
}

//initialize empty screen as white and set defaults in control
void draw_init(void) {
    FbBackgroundColor(BLACK);
    FbClear();
    unsigned char j;
    for(j=0;j<PIX_NUM;j++) {
        //clear image buffer
        image_buffer.blue[j]=WHITE_MASK;
        image_buffer.green[j]=WHITE_MASK;
        image_buffer.red[j]=WHITE_MASK;
        //clear ir buffer
        ir_in_buffer.blue[j]=WHITE_MASK;
        ir_in_buffer.green[j]=WHITE_MASK;
        ir_in_buffer.red[j]=WHITE_MASK;
    }
    draw_state.state = DRAW_RENDER;
    draw_state.color = DRAW_BLACK;
    draw_state.cursor.x = 0;
    draw_state.cursor.y = 0;
    show_drop_down = DRAW_DROP_DOWN_OFF;
    show_ir_buff = 0;
}

//render all pixels and cursor to the display
void draw_render(void) {
    unsigned char j,k;
    //render image buffer or ir buffer
    for(j=0;j<PIX_NUM;j++) {
        for(k=0;k<PIX_NUM;k++) {
            draw_draw_pixel(j,k,
                    draw_read_pixel((show_ir_buff? ir_in_buffer : image_buffer),j,k));
        }
    }
    draw_cursor(draw_state.cursor);
    //render drop down menu
    if(show_drop_down != DRAW_DROP_DOWN_OFF) {
        draw_drop_down();
    }
    draw_state.state = DRAW_GET_INPUT;
    FbSwapBuffers();
}

//draws drop down menu to display
void draw_drop_down(void) {
    FbColor(BLACK);
    FbMove(0,0);
    FbFilledRectangle(SCREEN_WIDTH,SCREEN_WIDTH/8);
    FbMove(2,4);
    if(show_drop_down == DRAW_DROP_DOWN_LEFT) {
        FbColor(GREEN);
    }
    else {
        FbColor(WHITE);
    }
    FbWriteLine("SEND PIC");
    FbMove(SCREEN_WIDTH/2+6,4);
    if(show_drop_down == DRAW_DROP_DOWN_RIGHT) {
        FbColor(GREEN);
    }
    else {
        FbColor(WHITE);
    }
    FbWriteLine("GET PIC");
}

//get inputs and set data accordingly
void draw_get_input(void) {
    draw_state.state = DRAW_PROCESS;
    if(BUTTON_PRESSED_AND_CONSUME) {
        switch(show_drop_down) {
            case DRAW_DROP_DOWN_OFF:
                draw_state.state = DRAW_EXIT;
                break;
            case DRAW_DROP_DOWN_LEFT:
                draw_send_image(image_buffer);
                break;
            case DRAW_DROP_DOWN_RIGHT:
                show_ir_buff = !show_ir_buff;
                image_received = 0;
                break;
            default:
                draw_state.state = DRAW_EXIT;
                break;
        }
    }
    if(DOWN_BTN) {
        if((draw_state.cursor.y < PIX_NUM) && 
           (show_drop_down == DRAW_DROP_DOWN_OFF)) {
            draw_state.cursor.y++;
            draw_state.cursor.moved = 1;
        }
        if(show_drop_down != DRAW_DROP_DOWN_OFF) {
            show_drop_down = DRAW_DROP_DOWN_OFF;
        }
    }
    if(UP_BTN) {
        if(draw_state.cursor.y > 0) {
            draw_state.cursor.y--;
            draw_state.cursor.moved = 1;
        }
        else {
            show_drop_down = DRAW_DROP_DOWN_LEFT;
        }
    }
    if(LEFT_BTN) {
        if((draw_state.cursor.x > 0) && 
           (show_drop_down == DRAW_DROP_DOWN_OFF)) {
            draw_state.cursor.x--;
            draw_state.cursor.moved = 1;
        }
        if(show_drop_down == DRAW_DROP_DOWN_RIGHT) {
            show_drop_down = DRAW_DROP_DOWN_LEFT;
        }
    }
    if(RIGHT_BTN) {
        if((draw_state.cursor.x < PIX_NUM) &&
            (show_drop_down == DRAW_DROP_DOWN_OFF)) {
            draw_state.cursor.x++;
            draw_state.cursor.moved = 1;
        }
        if(show_drop_down == DRAW_DROP_DOWN_LEFT) {
            show_drop_down = DRAW_DROP_DOWN_RIGHT;
        }
    }
    if(DOWN_TOUCH_AND_CONSUME) {
        if(draw_state.color < DRAW_CYAN) {
            draw_state.color++;
        }
    }
    if(UP_TOUCH_AND_CONSUME) {
        if(draw_state.color > DRAW_BLACK) {
            draw_state.color--;
        }
    }
    if(MIDDLE_TOUCH_AND_CONSUME) {
        draw_state.on_off = !draw_state.on_off;
    }
}

//process data based on what changed in draw_get_input
void draw_process() {
    draw_state.state = DRAW_RENDER;
    if(draw_state.cursor.moved && (draw_state.on_off == DRAW_ON)) {
        draw_write_pixel(&image_buffer,draw_state);
    }
}

//sets function pointer and returns to menus
void draw_exit(void) {
    draw_state.state = DRAW_RENDER;
    returnToMenus();
}

//writes to image buffer based on current data in control struct
void draw_write_pixel(struct _image_buffer * img_buf,struct _draw_state g_data) {
    switch(g_data.color) {
        case DRAW_BLACK:
            draw_write_pixel_data(&img_buf->red[g_data.cursor.y],g_data.cursor.x);
            draw_write_pixel_data(&img_buf->green[g_data.cursor.y],g_data.cursor.x);
            draw_write_pixel_data(&img_buf->blue[g_data.cursor.y],g_data.cursor.x);
            break;
        case DRAW_WHITE:
            draw_remove_pixel_data(&img_buf->red[g_data.cursor.y],g_data.cursor.x);
            draw_remove_pixel_data(&img_buf->green[g_data.cursor.y],g_data.cursor.x);
            draw_remove_pixel_data(&img_buf->blue[g_data.cursor.y],g_data.cursor.x);
            break;
        case DRAW_RED:
            draw_write_pixel_data(&img_buf->red[g_data.cursor.y],g_data.cursor.x);
            draw_remove_pixel_data(&img_buf->green[g_data.cursor.y],g_data.cursor.x);
            draw_remove_pixel_data(&img_buf->blue[g_data.cursor.y],g_data.cursor.x);
            break;
        case DRAW_GREEN:
            draw_remove_pixel_data(&img_buf->red[g_data.cursor.y],g_data.cursor.x);
            draw_write_pixel_data(&img_buf->green[g_data.cursor.y],g_data.cursor.x);
            draw_remove_pixel_data(&img_buf->blue[g_data.cursor.y],g_data.cursor.x);
            break;
        case DRAW_BLUE:
            draw_remove_pixel_data(&img_buf->red[g_data.cursor.y],g_data.cursor.x);
            draw_remove_pixel_data(&img_buf->green[g_data.cursor.y],g_data.cursor.x);
            draw_write_pixel_data(&img_buf->blue[g_data.cursor.y],g_data.cursor.x);
            break;
        case DRAW_YELLOW:
            draw_write_pixel_data(&img_buf->red[g_data.cursor.y],g_data.cursor.x);
            draw_write_pixel_data(&img_buf->green[g_data.cursor.y],g_data.cursor.x);
            draw_remove_pixel_data(&img_buf->blue[g_data.cursor.y],g_data.cursor.x);
            break;
        case DRAW_PURPLE:
            draw_write_pixel_data(&img_buf->red[g_data.cursor.y],g_data.cursor.x);
            draw_remove_pixel_data(&img_buf->green[g_data.cursor.y],g_data.cursor.x);
            draw_write_pixel_data(&img_buf->blue[g_data.cursor.y],g_data.cursor.x);
            break;
        case DRAW_CYAN:
            draw_remove_pixel_data(&img_buf->red[g_data.cursor.y],g_data.cursor.x);
            draw_write_pixel_data(&img_buf->green[g_data.cursor.y],g_data.cursor.x);
            draw_write_pixel_data(&img_buf->blue[g_data.cursor.y],g_data.cursor.x);
            break;
    }
}

//returns a color to draw to display based on combination of rgb bits for pixel in image buffer
unsigned short draw_read_pixel(struct _image_buffer img_buf,unsigned char x, unsigned char y) {
    unsigned short draw_color = BLACK;
    if(draw_read_pixel_data(img_buf.blue[y],x)) {
        draw_color = draw_color | BLUE;
    }
    if(draw_read_pixel_data(img_buf.green[y],x)) {
        draw_color = draw_color | GREEN;
    }
    if(draw_read_pixel_data(img_buf.red[y],x)) {
        draw_color = draw_color | RED;
    }
    return draw_color;
}


//returns if a bit is set in an image buffer line based on horizontal index
unsigned char draw_read_pixel_data(PIX_NUM_WIDE const vert_index,unsigned const char horz_index) {
    return ((vert_index&MASK(horz_index)) ? 1 : 0);
}

//writes a pixel to an image buffer line based on a horizontal index
void draw_write_pixel_data(PIX_NUM_WIDE * vert_index,unsigned const char horz_index) {
    *vert_index = (*vert_index) | MASK(horz_index); 
}

//removes a pixel from an image buffer line based on a horizontal index
void draw_remove_pixel_data(PIX_NUM_WIDE * vert_index,unsigned const char horz_index) {
    *vert_index = (*vert_index) & DELETE_MASK(horz_index);
}

//draws a pixel on the display based on location and color
void draw_draw_pixel(unsigned char x, unsigned char y, unsigned short color) {
    FbColor(color);
    FbMove(x*PIX_WIDTH, y*PIX_WIDTH);
    FbFilledRectangle(PIX_WIDTH, PIX_WIDTH);
}

//draws cursor on the display based on _cursor struct data
void draw_cursor(struct _cursor pos) {
    FbColor(MAGENTA);
    FbMove(pos.x*PIX_WIDTH,pos.y*PIX_WIDTH);
    FbRectangle(PIX_WIDTH,PIX_WIDTH);
}

unsigned char draw_is_empty(struct _image_buffer img_buf, unsigned char line) {
    if((img_buf.red[line] == WHITE_MASK) &&
       (img_buf.green[line] == WHITE_MASK) &&
       (img_buf.blue[line] == WHITE_MASK)) {
        return 1;
    }
    else {
        return 0;
    }
}

unsigned char draw_get_first_line(struct _image_buffer img_buf) {
    unsigned char first_line = 0;
    while(draw_is_empty(img_buf,first_line)&&first_line<=PIX_NUM) {
        first_line++;
    }
    if(first_line > PIX_NUM) {
        first_line = 0;
    }
    return first_line;
}

unsigned char draw_get_last_line(struct _image_buffer img_buf) {
    unsigned char last_line = PIX_NUM;
    while(draw_is_empty(img_buf,last_line)&&last_line>0){
        last_line--;
    }
    return last_line;
}

union half_pkt draw_send_first_pkt(struct _image_buffer img_buf) {
    union half_pkt hpkt;
    union IRpacket_u draw_pkt;
    hpkt.bits8.lower = draw_get_first_line(img_buf);
    hpkt.bits8.upper = draw_get_last_line(img_buf);
    draw_pkt.p.badgeId = FIRST_PKT;
    draw_pkt.p.command = IR_WRITE;
    draw_pkt.p.address = IR_UDRAW;
    draw_pkt.p.data = hpkt.bits16;
    IRqueueSend(draw_pkt);
    //draw_ir_udraw(draw_pkt.p);
    return hpkt;
}

void draw_send_packet(struct _image_buffer img_buf, unsigned char byte, unsigned char vert_index) {
    union IRpacket_u draw_pkt;
    union half_pkt hpkt;
    union line_byte_index line;
    line.line = img_buf.red[vert_index];
    hpkt.bits8.lower = line.bytes[byte];
    line.line = img_buf.green[vert_index];
    hpkt.bits8.upper = line.bytes[byte];
    line.line = img_buf.blue[vert_index];
    draw_pkt.p.data = hpkt.bits16;
    draw_pkt.p.badgeId = (BG_ID_MASK)&(line.bytes[byte]);
    draw_pkt.p.badgeId |= SET_HIGH_BIT;
    draw_pkt.p.command = IR_WRITE;
    draw_pkt.p.address = IR_UDRAW;
    IRqueueSend(draw_pkt);
    //draw_ir_udraw(draw_pkt.p);
}

void draw_send_image(struct _image_buffer img_buf) {
    const TickType_t draw_tick_rate = 60 / portTICK_PERIOD_MS;
    union half_pkt hpkt;
    hpkt = draw_send_first_pkt(img_buf);
    vTaskDelay(draw_tick_rate);
    unsigned char byte_index;
    if(hpkt.bits16 == 0) {
        return;
    }
    for(hpkt.bits8.lower;hpkt.bits8.lower<=hpkt.bits8.upper;hpkt.bits8.lower++) {
        for(byte_index=0;byte_index<PIX_BYTES;byte_index++) {
            draw_send_packet(img_buf,byte_index,hpkt.bits8.lower);
            vTaskDelay(draw_tick_rate);
            setNote(109, 800);
        }
    }
}

void draw_ir_udraw(struct IRpacket_t p) {
    
    static union half_pkt first_last;
    static unsigned char byteIndx = 0;
    union half_pkt hpkt;
    union line_byte_index line;
    line.line = EMPTY_MASK;
    if(p.badgeId == FIRST_PKT) {
        unsigned char j;
        for(j=0;j<PIX_NUM;j++) {
            //clear ir buffer
            ir_in_buffer.blue[j]=EMPTY_MASK;
            ir_in_buffer.green[j]=EMPTY_MASK;
            ir_in_buffer.red[j]=EMPTY_MASK;
        }
        first_last.bits16 = p.data;
        setNote(109, 800);
        blue(0b11111111);
    }

    else {

        hpkt.bits16 = p.data;
        line.bytes[byteIndx] = hpkt.bits8.lower;
        ir_in_buffer.red[first_last.bits8.lower] |= line.line;
        line.bytes[byteIndx] = hpkt.bits8.upper;
        ir_in_buffer.green[first_last.bits8.lower] |= line.line;
        line.bytes[byteIndx] = p.badgeId & BG_ID_OUT;
        ir_in_buffer.blue[first_last.bits8.lower] |= line.line;
     
        if(byteIndx == (PIX_BYTES-1)) {
            byteIndx = 0;
            first_last.bits8.lower++;
        }
        else {
            byteIndx++;       
        }
        
        if(first_last.bits8.lower == first_last.bits8.upper) {
            image_received = 1;
            first_last.bits16 = 0;
        }
    }
}
