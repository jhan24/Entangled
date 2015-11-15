#include "mylib.h"
#include <stdlib.h>
#include "font.h"
#include "spiderdance.h"

unsigned short *videoBuffer = (unsigned short *)0x6000000;

void setPixel4(int row, int col, u8 index)
{
	int pixel = OFFSET(row, col, 240);
	int shrt = pixel/2;
	if(col & 1)
	{
		// column is odd
		videoBuffer[shrt] = (videoBuffer[shrt] & 0x00FF) | (index<<8);
	}
	else
	{
		// column is even
		videoBuffer[shrt] = (videoBuffer[shrt] & 0xFF00) | (index);
	}
}

void drawRect4(int row, int col, int height, int width, unsigned char index)
{
	volatile unsigned short color = index | (index<<8);
	int r;
	if (height == 0 || width == 0) {

	} else {
		if ((width & 1) && (col & 1)) {
			for (r = 0; r < height; r++) {
				int shrt = OFFSET(row+r, col, 240) / 2;
				videoBuffer[shrt] = (videoBuffer[shrt] & 0x00FF) | (index << 8);
			}
			width--;
			col++;
		} else if (width & 1) {
			// width is odd
			for (r = 0; r < height; r++) {
				int shrtEnd = OFFSET(row+r, col+width, 240) / 2;
				videoBuffer[shrtEnd] = (videoBuffer[shrtEnd] & 0xFF00) | (index);
			}
			width--;
		} else if (col & 1) {
			// column is odd
			for (r=0; r<height; r++) {
				int shrt = OFFSET(row+r, col, 240) / 2;
				int shrtEnd = OFFSET(row+r, col+width, 240) / 2;
				videoBuffer[shrt] = (videoBuffer[shrt] & 0x00FF) | (index << 8);
				videoBuffer[shrtEnd] = (videoBuffer[shrtEnd] & 0xFF00) | (index);
			}
			col++;
			width--;
		}

		if (width > 1) {
			for(r=0; r<height; r++)
			{
				DMA[3].src = &color;
				DMA[3].dst = &videoBuffer[OFFSET(row+r, col, 240)/2];
				DMA[3].cnt = (width/2) | DMA_SOURCE_FIXED | DMA_ON; 
			}
		}
	}
}

void FlipPage()
{
	if(REG_DISPCTL & BUFFER1FLAG)
	{
		// We were displaying Buffer 1
		REG_DISPCTL = REG_DISPCTL & ~BUFFER1FLAG;
		videoBuffer = BUFFER1;
	}
	else
	{
		// We were displaying Buffer 0
		REG_DISPCTL = REG_DISPCTL | BUFFER1FLAG;
		videoBuffer = BUFFER0;
	}
}

void waitForVblank()
{
	while(SCANLINECOUNTER > 160);
	while(SCANLINECOUNTER < 160);
}

void drawImage4(int row, int col, int width, int height, const u16* dataarr) {
	// Attempted to fix the odd-column problem, but then realized I couldn't use DMA with it (because off-by-one)
	// if (col & 1) {
	// 	for (int r = 0; r < height; r++) {
	// 		int shrt = OFFSET(row+r, col, 240) / 2;
	// 		int shrtEnd = OFFSET(row+r, col+width, 240) / 2;
	// 		int img = OFFSET(r, 0, width) / 2;
	// 		int imgEnd = OFFSET(r, width - 1, width) / 2;
	// 		videoBuffer[shrt] = (videoBuffer[shrt] & 0x00FF) | (dataarr[img] << 8);
	// 		videoBuffer[shrtEnd] = (videoBuffer[shrtEnd] & 0xFF00) | (dataarr[imgEnd] >> 8);

	// 		DMA[3].src = &dataarr[OFFSET(r, 1, width) / 2];
	// 		DMA[3].dst = &videoBuffer[OFFSET(row+r, col+1, 240) / 2];
	// 		DMA[3].cnt = ((width / 2) - 1) | DMA_ON;
	// 	}
	// } else {
	// 	for (int r = 0; r < height; r++) {
	// 		DMA[3].src = &dataarr[OFFSET(r, 0, width) / 2];
	// 		DMA[3].dst = &videoBuffer[OFFSET(row+r, col, 240) / 2];
	// 		DMA[3].cnt = width / 2 | DMA_ON;
	// 	}
	// }

		for (int r = 0; r < height; r++) {
			DMA[3].src = &dataarr[OFFSET(r, 0, width) / 2];
			DMA[3].dst = &videoBuffer[OFFSET(row+r, col, 240) / 2];
			DMA[3].cnt = width / 2 | DMA_ON;
		}

}

void fillScreen4(unsigned char index)
{
	volatile unsigned short color = index | (index << 8);
	DMA[3].src = &color;
	DMA[3].dst = videoBuffer;
	DMA[3].cnt = 19200 | DMA_SOURCE_FIXED | DMA_ON;
}

// Limit Check for the Player Ship
int playerLimitCheck(int x, int width, int lowerLimit, int upperLimit, int *whichLimit) {
	if ((x + (width) > upperLimit)) {
		*whichLimit = 1;
		return 1;
	} else if ((x < lowerLimit)) {
		*whichLimit = 0;
		return 1;
	}
	*whichLimit = 2;
	return 0;
}

void playerMovement(PLAYER *player, int *upPressed, int *downPressed, BAR *topBar, BAR *middleBar, BAR *bottomBar) { // For Button Inputs
	// Button Inputs
	// Movement of player Left and Right, hold X to slow movement.
	if (KEY_DOWN_NOW(BUTTON_LEFT)) {
		if (KEY_DOWN_NOW(BUTTON_B)) {
			player->x = player->x - 2;
		} else {
			player->x = player->x - 5;
		}
	} else if (KEY_DOWN_NOW(BUTTON_RIGHT)){
		if (KEY_DOWN_NOW(BUTTON_B)) {
			player->x = player->x + 2;
		} else {
			player->x = player->x + 5;
		}
	}

	// Movement up and down between lines
	if (KEY_DOWN_NOW(BUTTON_UP) && *upPressed == 0) {
		if (player->y == middleBar->y - player->height / 2) {
			player->y = topBar->y - player->height / 2;
		} else if (player->y == bottomBar->y - player->height / 2) {
			player->y = middleBar->y - player->height / 2;
		}
		*upPressed = 1;
	} else if (KEY_DOWN_NOW(BUTTON_DOWN) && *downPressed == 0) {
		if (player->y == middleBar->y - player->height / 2) {
			player->y = bottomBar->y - player->height / 2;
		} else if (player->y == topBar->y - player->height / 2) {
			player->y = middleBar->y - player->height / 2;
		}
		*downPressed = 1;
	}

	if (!KEY_DOWN_NOW(BUTTON_UP)) {
		*upPressed = 0;
	}
	if (!KEY_DOWN_NOW(BUTTON_DOWN)) {
		*downPressed = 0;
	}

	// Checking if player is trying to move out of bounds. Can't let that happen!
	int whichPlayerLimit = 2;
	if (playerLimitCheck(player->x, player->width, 0, 240, &whichPlayerLimit)) {
		if (whichPlayerLimit == 0) {
			player->x = 0;
		} else {
			player->x = 232;
		}
	}
}

void drawBasics(PLAYER *player, BAR *topBar, BAR *middleBar, BAR *bottomBar, int *drawPlayer) {
	drawRect4(topBar->y, topBar->x, topBar->height, topBar->width, topBar->color);
	drawRect4(middleBar->y, middleBar->x, middleBar->height, middleBar->width, middleBar->color);
	drawRect4(bottomBar->y, bottomBar->x, bottomBar->height, bottomBar->width, bottomBar->color);
	if (*drawPlayer == 1) {
		drawRect4(player->y, player->x, player->height, player->width, player->color);
	}
}

void createSpider(SPIDER *temp, int type, int *nextEmptySpider, int direction, int speed, int x, int y, int accel, u8 color) {
	temp->type = type;
	temp->height = 10;
	temp->width = 10;
	if (direction == -1) {
		temp->direction = rand() % 2;
		if (temp->direction == 0) {
			temp->x = 0;
			temp->speed = 2;
		} else {
			temp->x = 240 - temp->width;
			temp->speed = -2;
		}
	} else {
		temp->direction = direction;
		temp->x = x;
		temp->speed = speed;
	}

	if (speed != -1) {
		if (temp->direction == 0) {
			temp->speed = speed;
		} else {
			temp->speed = -1 * speed;
		}
	}
	
	if (y == -1) {
		temp->y = rand() % 3;
		if (temp->y == 0) {
			temp->y = 50 - temp->width / 2;
		} else if (temp->y == 1) {
			temp->y = 80 - temp->width / 2;
		} else {
			temp->y = 110 - temp->width / 2;
		}
	} else {
		temp->y = y;
	}
	temp->accel = accel;
	temp->color = color;
	temp->arrayPosition = *nextEmptySpider;
}

void updateSpiders(SPIDER spiders[30], int *playerHealth, PLAYER *player, int *playerInvincibility) {
	for (int i = 0; i < 30; i++) {
		if (spiders[i].arrayPosition != 987654) {
			spiders[i].x = spiders[i].x + spiders[i].speed;
			spiders[i].speed = spiders[i].speed + spiders[i].accel;
			if (spiders[i].x > (240 - spiders[i].width) || spiders[i].x < 0) {
				spiders[i].arrayPosition = 987654;
			} else {
				drawRect4(spiders[i].y, spiders[i].x, spiders[i].height, spiders[i].width, spiders[i].color);
				// Check for Collisions
				if (*playerInvincibility == 0 && spiders[i].x < player->x + player->width &&
					spiders[i].x + spiders[i].width > player->x &&
					spiders[i].y + spiders[i].height > player->y &&
					spiders[i].y < player->y + player->height) {
					*playerHealth = *playerHealth - 10;
					REG_SND4ENV = 0xF21F;
					REG_SND4FRQ = 0x8081;
					*playerInvincibility = 100;
				}
			}
		}
	}
}

void createBullet(BULLET *temp, int norm, int x, int y, int speedX, int speedY, int accelX, int accelY, int type, PLAYER *player) {
	if (type == 0) {
		temp->color = 254;
		temp->width = 6;
		temp->height = 6;
		temp->type = type;
		if (norm == 0) {
			norm = 1 + (rand() % 4);
		}		
			
		if (norm == 1) {
			temp->y = 0;
			temp->x = 5 + (rand() % (235 - temp->width));
			temp->speedY = 2 + (rand() % 2);
			if (rand() % 2) {
				temp->speedX = 0 + (rand() % 4);
			} else {
				temp->speedX = 0 - (rand() % 4);
			}
		} else if (norm == 2) {
			temp->y = 240 - temp->height;
			temp->x = 5 + (rand() % (235 - temp->width));
			if (rand() % 2) {
				temp->speedX = 0 + (rand() % 4);
			} else {
				temp->speedX = 0 - (rand() % 4);
			}
			temp->speedY = -2 - (rand() % 2);
		} else if (norm == 3) {
			temp->x = 0;
			temp->y = 5 + (rand() % (155 - temp->height));
			temp->speedX = 2 + (rand() % 2);
			if (rand() % 2) {
				temp->speedY = 1 + (rand() % 4);
			} else {
				temp->speedY = -1 - (rand() % 4);
			}
		} else if (norm == 4) {
			temp->x = 240 - temp->width;
			temp->y = 5 + (rand() % (155 - temp->height));
			temp->speedX = -2 - (rand() % 2);
			if (rand() % 2) {
				temp->speedY = 1 + (rand() % 4);
			} else {
				temp->speedY = -1 - (rand() % 4);
			}
		} else {
			temp->y = y;
			temp->x = x;
			temp->speedX = speedX;
			temp->speedY = speedY;
		}
	} else if (type == 1) {
		temp->color = 249;
		temp->width = 6;
		temp->height = 6;
		temp->type = type;
		if (norm == 0) {
			norm = 1 + (rand() % 4);
		}		
			
		if (norm == 1) {
			temp->y = 0;
			temp->x = 5 + (rand() % (235 - temp->width));
			temp->speedY = 2 + (rand() % 2);
			temp->speedX = (temp->x - player->x) / ((temp->y - player->y) / temp->speedY);
		} else if (norm == 2) {
			temp->y = 240 - temp->height;
			temp->x = 5 + (rand() % (235 - temp->width));
			temp->speedY = -2 - (rand() % 2);
			temp->speedX = (temp->x - player->x) / ((temp->y - player->y) / temp->speedY);
		} else if (norm == 3) {
			temp->x = 0;
			temp->y = 5 + (rand() % (155 - temp->height));
			temp->speedX = 2 + (rand() % 2);
			temp->speedY = (temp->y - player->y) / ((temp->x - player->x) / temp->speedX);
			if (temp->speedY == 0) {
				temp->speedY = 1;
			}
		} else if (norm == 4) {
			temp->x = 240 - temp->width;
			temp->y = 5 + (rand() % (155 - temp->height));
			temp->speedX = -2 - (rand() % 2);
			temp->speedY = (temp->y - player->y) / ((temp->x - player->x) / temp->speedX);
			if (temp->speedY == 0) {
				temp->speedY = -1;
			}
		} else {
			temp->y = y;
			temp->x = x;
			temp->speedX = speedX;
			temp->speedY = speedY;
		}
	}

	temp->accelX = accelX;
	temp->accelY = accelY;
}

void updateBullets(BULLET spiders[30], int *playerHealth, PLAYER *player, int *playerInvincibility) {
	for (int i = 0; i < 30; i++) {
		if (spiders[i].arrayPosition != 987654) {
			spiders[i].x = spiders[i].x + spiders[i].speedX;
			spiders[i].y = spiders[i].y + spiders[i].speedY;
			spiders[i].speedX = spiders[i].speedX + spiders[i].accelX;
			spiders[i].speedY = spiders[i].speedY + spiders[i].accelY;
			if (spiders[i].x > (240 - spiders[i].width) || spiders[i].x < 0 || spiders[i].y < 0 || (spiders[i].y + spiders[i].height) > 160) {
				spiders[i].arrayPosition = 987654;
			} else {
				drawRect4(spiders[i].y, spiders[i].x, spiders[i].height, spiders[i].width, spiders[i].color);
				// Check for Collisions
				if (*playerInvincibility == 0 && spiders[i].x < player->x + player->width &&
					spiders[i].x + spiders[i].width > player->x &&
					spiders[i].y + spiders[i].height > player->y &&
					spiders[i].y < player->y + player->height) {
					*playerHealth = *playerHealth - 5;
					*playerInvincibility = 100;
				}
			}
		}
	}
}

void createLaser(LASER *temp, int x, int y, int size, int speed, int detonation, int fireLength, int type) {
	if (type == 0) {
		temp->x = x;
		temp->y = 0;
		temp->height = 160;
		temp->width = size;
	} else if (type == 1) {
		temp->x = 0;
		temp->y = y;
		temp->width = 240;
		temp->height = size;
	}
	if (temp->x < 0) {
		temp->x = 0;
	} else if (temp->x > 240 - temp->width) {
		temp->x = 240 - temp->width;
	}
	if (temp->y < 0) {
		temp->y = 20;
	} else if (temp->y > 160 - temp->height) {
		temp->y = 100;
	}
	temp->speed = speed;
	temp->detonation = detonation;
	temp->fireLength = fireLength;
	temp->type = type;
	temp->color = 248;
	if (size == 40) {
		temp->color = 241;
	} else if (size == 50) {
		temp->color = 245;
	} else if (size == 70) {
		temp->color = 240;
	}
}

void updateLasers(LASER spiders[5], int *playerHealth, PLAYER *player, int *playerInvincibility) {
	for (int i = 0; i < 5; i++) {
		if (spiders[i].arrayPosition != 987654) {
			if (spiders[i].type == 0) {
				spiders[i].x = spiders[i].x + spiders[i].speed;
				if (spiders[i].x < 0) {
					spiders[i].x = 0;
					spiders[i].speed = spiders[i].speed * -1;
				} else if (spiders[i].x > 240 - spiders[i].width) {
					spiders[i].x = 240 - spiders[i].width;
					spiders[i].speed = spiders[i].speed * -1;
				}
				if (spiders[i].detonation > 0) {
					if (spiders[i].detonation % 2) {
						drawRect4(spiders[i].y, spiders[i].x + (spiders[i].width / 2), spiders[i].height, 1, spiders[i].color);
					}
					spiders[i].detonation--;
				} else {
					if (spiders[i].fireLength > 0) {
						drawRect4(spiders[i].y, spiders[i].x, spiders[i].height, spiders[i].width, spiders[i].color);
						// Check for Collisions
						if (*playerInvincibility == 0 && spiders[i].x < player->x + player->width &&
							spiders[i].x + spiders[i].width > player->x &&
							spiders[i].y + spiders[i].height > player->y &&
							spiders[i].y < player->y + player->height) {
							*playerHealth = *playerHealth - 20;
							*playerInvincibility = 100;
						}
						spiders[i].fireLength--;
					} else {
						spiders[i].arrayPosition = 987654;
					}
				}
			} else {
				spiders[i].y = spiders[i].y + spiders[i].speed;
				if (spiders[i].y < 0) {
					spiders[i].y = 0;
					spiders[i].speed = spiders[i].speed * -1;
				} else if (spiders[i].y > 160 - spiders[i].height) {
					spiders[i].y = 160 - spiders[i].height;
					spiders[i].speed = spiders[i].speed * -1;
				}
				if (spiders[i].detonation > 0) {
					if (spiders[i].detonation % 2) {
						drawRect4(spiders[i].y + (spiders[i].height / 2), spiders[i].x, 1, spiders[i].width, spiders[i].color);
					}
					spiders[i].detonation--;
				} else {
					if (spiders[i].fireLength > 0) {
						drawRect4(spiders[i].y, spiders[i].x, spiders[i].height, spiders[i].width, spiders[i].color);
						// Check for Collisions
						if (*playerInvincibility == 0 && spiders[i].x < player->x + player->width &&
							spiders[i].x + spiders[i].width > player->x &&
							spiders[i].y + spiders[i].height > player->y &&
							spiders[i].y < player->y + player->height) {
							*playerHealth = *playerHealth - 20;
							*playerInvincibility = 100;
						}
						spiders[i].fireLength--;
					} else {
						spiders[i].arrayPosition = 987654;
					}
				}
			}
		}
	}
}