typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

#define REG_DISPCTL *(unsigned short *)0x4000000

#define SCANLINECOUNTER (*(unsigned short *)0x4000006)

#define MODE3 3
#define MODE4 4
#define BUFFER0 (unsigned short *)0x6000000
#define BUFFER1 (unsigned short *)0x600A000
#define BUFFER1FLAG (1<<4)

#define PALETTE ((unsigned short *)0x5000000)

#define BG2_ENABLE (1<<10)
// Colors
#define RGB(r, g, b) ((r) | (g) << 5 | (b) << 10)
#define BLACK   RGB(0,0,0)
#define WHITE   RGB(31, 31, 31)
#define RED     RGB(31,0,0)
#define GREEN   RGB(0, 31, 0)
#define BLUE    RGB(0,0,31)
#define YELLOW  RGB(31,31,0)
#define CYAN    RGB(0,31,31)
#define MAGENTA RGB(31,0,31)
#define GRAY    RGB(25, 25, 25)
#define PURPLE RGB(15, 15, 31)
#define ORANGE RGB(31, 15, 0)
#define HELL RGB(10, 0, 10)
#define BARCOLOR RGB(19, 6, 31)
#define PLAYERCOLOR RGB(31, 13, 25)

#define OFFSET(r, c, rowlen) ((r)*(rowlen)+(c))

extern unsigned short *videoBuffer;

/* Buttons */

#define BUTTON_A      (1<<0)
#define BUTTON_B      (1<<1)
#define BUTTON_SELECT (1<<2)
#define BUTTON_START  (1<<3)
#define BUTTON_RIGHT  (1<<4)
#define BUTTON_LEFT   (1<<5)
#define BUTTON_UP     (1<<6)
#define BUTTON_DOWN   (1<<7)
#define BUTTON_R      (1<<8)
#define BUTTON_L      (1<<9)

#define BUTTONS (*(unsigned int *)0x4000130)

#define KEY_DOWN_NOW(key)  ((~BUTTONS) & (key))

/* DMA      NOTE!!! USES U32...don't forget to typedef it */

typedef struct 
{
	const volatile void *src;
	volatile void *dst;
	volatile u32 cnt;
} DMAREC;

#define DMA ((volatile DMAREC *)0x040000B0)

#define REG_DMA0SAD         *(u32*)0x40000B0  // source address
#define REG_DMA0DAD         *(u32*)0x40000B4  // destination address
#define REG_DMA0CNT         *(u32*)0x40000B8  // control register

/* DMA channel 1 register definitions */
#define REG_DMA1SAD         *(u32*)0x40000BC  // source address
#define REG_DMA1DAD         *(u32*)0x40000C0  // destination address
#define REG_DMA1CNT         *(u32*)0x40000C4  // control register

/* DMA channel 2 register definitions */
#define REG_DMA2SAD         *(u32*)0x40000C8  // source address
#define REG_DMA2DAD         *(u32*)0x40000CC  // destination address
#define REG_DMA2CNT         *(u32*)0x40000D0  // control register

/* DMA channel 3 register definitions */
#define REG_DMA3SAD         *(u32*)0x40000D4  // source address
#define REG_DMA3DAD         *(u32*)0x40000D8  // destination address
#define REG_DMA3CNT         *(u32*)0x40000DC  // control register

/* Defines */
#define DMA_CHANNEL_0 0
#define DMA_CHANNEL_1 1
#define DMA_CHANNEL_2 2
#define DMA_CHANNEL_3 3

#define DMA_DESTINATION_INCREMENT (0 << 21)
#define DMA_DESTINATION_DECREMENT (1 << 21)
#define DMA_DESTINATION_FIXED (2 << 21)
#define DMA_DESTINATION_RESET (3 << 21)

#define DMA_SOURCE_INCREMENT (0 << 23)
#define DMA_SOURCE_DECREMENT (1 << 23)
#define DMA_SOURCE_FIXED (2 << 23)

#define DMA_REPEAT (1 << 25)

#define DMA_16 (0 << 26)
#define DMA_32 (1 << 26)

#define DMA_NOW (0 << 28)
#define DMA_AT_VBLANK (1 << 28)
#define DMA_AT_HBLANK (2 << 28)
#define DMA_AT_REFRESH (3 << 28)

#define DMA_IRQ (1 << 30)
#define DMA_ON (1 << 31)

#define START_ON_FIFO_EMPTY 0x30000000

// Sound
// Defines for the interrupt handler
#define REG_IE        *(volatile unsigned short*) 0x4000200
#define REG_IF        *(volatile unsigned short*) 0x4000202
#define REG_IME       *(volatile unsigned short*) 0x4000208
#define IRQ_ENABLE    1
#define IRQ_VBLANK    (1 << 0)
#define IRQ_HBLANK    (1 << 1)
#define IRQ_VCOUNT    (1 << 2)
#define IRQ_TIMER(n)  (1 << (3 + (n)))
#define IRQ_COM       (1 << 7)
#define IRQ_DMA(n)    (1 << (8 + (n)))
#define IRQ_KEYPAD    (1 << 12)
#define IRQ_CARTRIDGE (1 << 13)
typedef void (*irqptr)(void);
#define REG_ISR_MAIN *(irqptr*) 0x3007FFC

// Defines for the timer
#define REG_TMD(n)   *(volatile unsigned short*) (0x4000100 + ((n) << 2))
#define REG_TMCNT(n) *(volatile unsigned short*) (0x4000102 + ((n) << 2))
#define TM_FREQ_1    0
#define TM_FREQ_64   1
#define TM_FREQ_256  2
#define TM_FREQ_1024 3
#define TM_CASCADE   (1 << 2)
#define TM_IRQ       (1 << 6)
#define TM_ENABLE    (1 << 7)

// Defines for sound control
#define REG_SND1SWP   *(volatile unsigned short*) 0x4000060 
#define REG_SND1ENV   *(volatile unsigned short*) 0x4000062
#define REG_SND1FRQ   *(volatile unsigned short*) 0x4000064
#define REG_SND2ENV   *(volatile unsigned short*) 0x4000068
#define REG_SND2FRQ   *(volatile unsigned short*) 0x400006C
#define REG_SND4ENV   *(volatile unsigned short*) 0x4000078
#define REG_SND4FRQ   *(volatile unsigned short*) 0x400007C
#define REG_SNDDMGCNT *(volatile unsigned short*) 0x4000080
#define REG_SNDSTAT   *(volatile unsigned short*) 0x4000084
#define MASTER_SND_EN (1 << 7)
#define MASTER_VOL25  0
#define MASTER_VOL50  1
#define MASTER_VOL100 2
#define LEFT_VOL(n)   (n)
#define RIGHT_VOL(n)  ((n) << 4)
#define CHAN_EN_L(n)  (1 << ((n) + 7))
#define CHAN_EN_R(n)  (1 << ((n) + 11))

#define CLEAR DMA[3].src = gameplay_palette; \
			DMA[3].dst = PALETTE; \
			DMA[3].cnt = DMA_ON | 240; \
			drawImage4(0, 0, 240, 160, gameplay)

#define printText(wait) if (counter % (wait) == 0) { \
						if (*textPointer) { \
							printing[letters] = *textPointer++; \
							letters++; \
						} \
					}


// Structs
typedef struct player { // Player
	int x;
	int y;
	int height;
	int width;
	u8 color;
} PLAYER;

typedef struct bars { // The lines by which the player can move on
	int x;
	int y;
	int height;
	int width;
	int speedY;
	u8 color;
} BAR;

typedef struct spider { // Spider-type enemies - these will ONLY move on the same LINES that the player moves on.
	int x;
	int y;
	int speed;
	int accel;
	int direction;
	int type;
	int height;
	int width;
	int arrayPosition;
	u8 color;
} SPIDER;

typedef struct bullet { // Bullet-type enemies - these can move FREELY.
	int x;
	int y;
	int speedX;
	int speedY;
	int accelX;
	int accelY;
	int type;
	int height;
	int width;
	int arrayPosition;
	u8 color;
} BULLET;

typedef struct laser { // Laser-type enemies - these are like ELONGATED bullets.
	int x;
	int y;
	int speed;
	int detonation;
	int fireLength;
	int type;
	int height;
	int width;
	int arrayPosition;
	u8 color;
} LASER;

void setPixel4(int row, int col, u8 index);
void drawRect4(int row, int col, int height, int width, unsigned char index);
void FlipPage();
void waitForVblank();
void drawImage4(int, int, int, int, const u16*);
int playerLimitCheck(int, int, int, int, int*);
void fillScreen4(unsigned char index);
void playerMovement(PLAYER*, int*, int*, BAR*, BAR*, BAR*);
void drawBasics(PLAYER*, BAR*, BAR*, BAR*, int*);
void createSpider(SPIDER*, int, int*, int, int, int, int, int, u8);
void updateSpiders(SPIDER*, int*, PLAYER*, int*);
void createBullet(BULLET *temp, int norm, int x, int y, int speedX, int speedY, int accelX, int accelY, int type, PLAYER* player);
void updateBullets(BULLET*, int*, PLAYER*, int*);
void createLaser(LASER *temp, int x, int y, int size, int speed, int detonation, int fireLength, int type);
void updateLasers(LASER spiders[5], int *playerHealth, PLAYER *player, int *playerInvincibility);