// Written by Jinsong Han

#include "mylib.h"
#include "playerlarge.h"
#include "title.h"
#include "gameplay.h"
#include "gameover.h"
#include "victory.h"
#include "smaller.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "font.h"
#include "countryside.h"
#include "spiderdance.h"
#include "glamour.h"
#include "meteor.h"
#include "story.h"


int main() {
	REG_DISPCTL = MODE4 | BG2_ENABLE;

	PALETTE[240] = RED;
	PALETTE[241] = GREEN;
	PALETTE[242] = BLUE;
	PALETTE[243] = BLACK;
	PALETTE[244] = WHITE;
	PALETTE[245] = YELLOW;
	PALETTE[246] = CYAN;
	PALETTE[247] = MAGENTA;
	PALETTE[248] = GRAY;
	PALETTE[249] = PURPLE;
	PALETTE[250] = ORANGE;
	PALETTE[251] = HELL;
	PALETTE[252] = BARCOLOR;
	PALETTE[253] = PLAYERCOLOR;
	PALETTE[254] = RGB(13, 31, 22);
	// Default to first state
	int state = 0;

	// For the last level
	int playerDamage = 0;
	//int selected = 0;

	// Create the three bar structs
	BAR topBar;
	BAR middleBar;
	BAR bottomBar;
	topBar.x = 0;
	middleBar.x = 0;
	bottomBar.x = 0;
	topBar.y = 50;
	middleBar.y = 80;
	bottomBar.y = 110;
	topBar.height = 1;
	middleBar.height = 1;
	bottomBar.height = 1;
	topBar.width = 240;
	middleBar.width = 240;
	bottomBar.width = 240;
	topBar.speedY = 0;
	middleBar.speedY = 0;
	bottomBar.speedY = 0;
	topBar.color = 252;
	middleBar.color = 252;
	bottomBar.color = 252;

	// Create the player
	PLAYER player;
	player.color = 253;
	player.height = 7;
	player.width = 8;
	player.x = 116;
	player.y = 77;
	int playerHealth = 100;
	int seededState = 0;
	while(1) {
		if (state == 999) { // Game Over
			spiderdance_stop();
			glamour_stop();
			meteor_play();
			int keepLooping = 1;
			DMA[3].src = gameover_palette;
			DMA[3].dst = PALETTE;
			DMA[3].cnt = DMA_ON | 240;
			while (keepLooping) {
				drawImage4(0, 0, 240, 160, gameover);
				if (KEY_DOWN_NOW(BUTTON_SELECT)) {
					state = 0;
					keepLooping = 0;
					meteor_stop();
				}
				waitForVblank();
				FlipPage();
			}
		} else if (state == 100) { // Victory
			int keepLooping = 1;
			glamour_stop();
			DMA[3].src = victory_palette;
			DMA[3].dst = PALETTE;
			DMA[3].cnt = DMA_ON | 240;
			int outerCounter = 0;
			int counter = 0;
			countryside_play();
			while (keepLooping) {
				drawImage4(0, 0, 240, 160, victory_frames[counter]);
				drawString4(145, 100, "The End", 244);
				if (KEY_DOWN_NOW(BUTTON_SELECT)) {
					state = 0;
					keepLooping = 0;
					countryside_stop();
				}
				outerCounter++;
				if (outerCounter % 5 == 0) {
					counter++;
					if (counter == 18) {
						counter = 0;
					}
				}
				
				waitForVblank();
				FlipPage();
			}
		} else if (state == 200) { // Animation
			glamour_play();
			int keepLooping = 1;
			DMA[3].src = smaller_palette;
			DMA[3].dst = PALETTE;
			DMA[3].cnt = DMA_ON | 223;
			DMA[3].src = story_palette;
			DMA[3].dst = &PALETTE[223];
			DMA[3].cnt = DMA_ON | 10;
			int outerCounter = 0;
			int counter = 0;
			while (keepLooping) {
				drawImage4(0, 0, 240, 160, smaller_frames[counter]);
				drawImage4(0, 0, STORY_WIDTH, STORY_HEIGHT, story);
				if (KEY_DOWN_NOW(BUTTON_SELECT)) {
					state = 0;
					keepLooping = 0;
					glamour_stop();
				}
				outerCounter++;
				if (outerCounter % 3 == 0) {
					counter++;
					if (counter == 31) {
						counter = 0;
					}
				}
				waitForVblank();
				FlipPage();
			}
		} else if (state == 0) { // Title Screen
			spiderdance_stop();
			glamour_stop();
			DMA[3].src = title_palette;
			DMA[3].dst = PALETTE;
			DMA[3].cnt = DMA_ON | 240;
			drawImage4(0, 0, 240, 160, title);
			int keepLooping = 1;
			while (keepLooping) {
				drawImage4(0, 0, 240, 160, title);
				if (KEY_DOWN_NOW(BUTTON_L)) {
					state = 200;
					keepLooping = 0;
				} else if (KEY_DOWN_NOW(BUTTON_R)) {
					if (KEY_DOWN_NOW(BUTTON_START)) {
						keepLooping = 0;
						state = 1;
						seededState = 20;
					}
				} else if (KEY_DOWN_NOW(BUTTON_B)) {
					if (KEY_DOWN_NOW(BUTTON_START)) {
						keepLooping = 0;
						state = 1;
						seededState = 40;
					}
				} else if (KEY_DOWN_NOW(BUTTON_UP)) {
					if (KEY_DOWN_NOW(BUTTON_START)) {
						keepLooping = 0;
						state = 1;
						seededState = 60;
					}
				} else if (KEY_DOWN_NOW(BUTTON_RIGHT)) {
					if (KEY_DOWN_NOW(BUTTON_START)) {
						keepLooping = 0;
						state = 1;
						seededState = 80;
					}
				} else if (KEY_DOWN_NOW(BUTTON_DOWN)) {
					if (KEY_DOWN_NOW(BUTTON_START)) {
						keepLooping = 0;
						state = 1;
						seededState = 82;
					}
				} else if (KEY_DOWN_NOW(BUTTON_START)) {
					state = 1; // 
					seededState = 2;
					keepLooping = 0;
					playerHealth = 100;
					player.x = 116;
					player.y = 77;
					player.color = 253;
				}
				waitForVblank();
				FlipPage();
			}
		} else if (state == 1) { // Fade out title screen
			// Add fade-out animation of title screen here in the future
			playerHealth = 100;
			playerDamage = 0;
			state = seededState; // Change this to debug stages!
			if (state < 44) {
				spiderdance_play();
			} else if (state >= 46) {
				glamour_play();
			}
		} else if (state == 2) { // Story 1
			int keepLooping = 1;
			int counter = 0;
			int upPressed = 0;
			int downPressed = 0;
			int drawPlayer = 1;
			char text[50];
			char printing[50];
			char *textPointer = &text[0];
			int letters = 0;
			while (keepLooping) {
				CLEAR;

				// STORY TEXT HERE
				if (counter < 50) {
					memset(printing, 0, 50);
				}
				if (counter >= 50 && counter < 200) {
					sprintf(text, "Oh dear me!");
					printText(2);
					if (counter == 199) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 200 && counter < 350) {
					sprintf(text, "We've got an uninvited guest!");
					printText(2);
					if (counter == 349) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 350 && counter < 500) {
					sprintf(text, "I guess I'll take this chance...");
					printText(2);
					if (counter == 499) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 500 && counter < 650) {
					sprintf(text, "...and have some fun.");
					printText(2);
					if (counter == 649) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 650 && counter < 800) {
					sprintf(text, "If you want to survive,");
					printText(2);
					if (counter == 799) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 800 && counter < 950) {
					sprintf(text, "You'd better avoid everything.");
					printText(2);
					if (counter == 949) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 950) {
					keepLooping = 0;
					state = 3;
				}
				drawString4(20, 10, printing, 244);
				drawBasics(&player, &topBar, &middleBar, &bottomBar, &drawPlayer);
				playerMovement(&player, &upPressed, &downPressed, &topBar, &middleBar, &bottomBar);

				if (KEY_DOWN_NOW(BUTTON_SELECT)) {
					keepLooping = 0;
					state = 0;
				}

				// Increments counter of the loop, this is basically the timer
				counter++;
				if (counter > 987654) {
					counter = 0;
				}

				// Displays HP
				char hpDisplay[20];
				sprintf(hpDisplay, "Health: %d", playerHealth);
				drawString4(150, 3, hpDisplay, 250);
				drawRect4(151, 75, 6, playerHealth, 246);
				drawRect4(151, 75 + playerHealth, 6, 100 - playerHealth, 247);

				// MODE 4
				waitForVblank();
				FlipPage();
			}
		} else if (state == 3) { // Stage 1
			int keepLooping = 1;
			int counter = 0;
			int upPressed = 0;
			int downPressed = 0;
			int playerInvincibility = 0;
			int cooldown = 0;
			int drawPlayer = 1;
			int generation = 1;
			// Create SPIDER enemies for this stage
			SPIDER spiders[30];
			int nextEmptySpider = 0;
			for (int i = 0; i < 30; i++) { // Initialize them
				spiders[i].arrayPosition = 987654;
			}

			while (keepLooping) {
				CLEAR;
				if (counter % (20 +  (rand() % 60)) == 0 && cooldown == 0 && generation == 1) {
					SPIDER temp;
					createSpider(&temp, 1, &nextEmptySpider, 0, 1, 0, -1, 0, 244);
					temp.x = 0;
					temp.speed = 1;
					spiders[nextEmptySpider] = temp;
					nextEmptySpider++;
					if (nextEmptySpider >= 30) {
						nextEmptySpider = 0;
					}
					cooldown = 25;
				}

				if (playerHealth <= 0) {
					keepLooping = 0;
					state = 999;
				}
				// Drawing

				// If player is invulnerable, flash the player to give player notice
				if (playerInvincibility > 0) {
					if (drawPlayer == 1) {
						drawPlayer = 0;
					} else {
						drawPlayer = 1;
					}
				} else {
					drawPlayer = 1;
				}

				// Drawing the objects
				drawBasics(&player, &topBar, &middleBar, &bottomBar, &drawPlayer);
				updateSpiders(spiders, &playerHealth, &player, &playerInvincibility);
				playerMovement(&player, &upPressed, &downPressed, &topBar, &middleBar, &bottomBar);

				if (KEY_DOWN_NOW(BUTTON_SELECT)) {
					keepLooping = 0;
					state = 0;
				}

				// Cooldown to prevent walls of spiders from appearing
				if (cooldown != 0) {
					cooldown--;
				}

				// If player gets hit, they get invincibility frames that last 100 loops. This decreases the remaining frames.
				if (playerInvincibility != 0) {
					playerInvincibility--;
				}

				// Displays HP
				char hpDisplay[20];
				sprintf(hpDisplay, "Health: %d", playerHealth);
				drawString4(150, 3, hpDisplay, 250);
				drawRect4(151, 75, 6, playerHealth, 246);
				drawRect4(151, 75 + playerHealth, 6, 100 - playerHealth, 247);

				// MODE 4
				waitForVblank();
				FlipPage();

				counter++;
				if (counter > 1000 && counter < 1100) {
					generation = 0;
				} else if (counter > 1250) {
					keepLooping = 0;
					state = 4;
				}
			}
		} else if (state == 4) { // Story 2
			int keepLooping = 1;
			int counter = 0;
			int upPressed = 0;
			int downPressed = 0;
			int drawPlayer = 1;
			char text[50];
			char printing[50];
			char *textPointer = &text[0];
			int letters = 0;
			while (keepLooping) {
				CLEAR;

				// STORY TEXT HERE
				if (counter < 50) {
					memset(printing, 0, 50);
				}
				if (counter >= 50 && counter < 200) {
					sprintf(text, "Not bad at all.");
					printText(2);
					if (counter == 199) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 200 && counter < 350) {
					sprintf(text, "You're pretty skilled, aren't ya?");
					printText(2);
					if (counter == 349) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 350 && counter < 500) {
					sprintf(text, "Why don't we kick it up a notch?");
					printText(2);
					if (counter == 499) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 500) {
					keepLooping = 0;
					state = 5;
				}
				drawString4(20, 10, printing, 244);
				drawBasics(&player, &topBar, &middleBar, &bottomBar, &drawPlayer);
				playerMovement(&player, &upPressed, &downPressed, &topBar, &middleBar, &bottomBar);

				if (KEY_DOWN_NOW(BUTTON_SELECT)) {
					keepLooping = 0;
					state = 0;
				}

				// Increments counter of the loop, this is basically the timer
				counter++;
				if (counter > 987654) {
					counter = 0;
				}

				// Displays HP
				char hpDisplay[20];
				sprintf(hpDisplay, "Health: %d", playerHealth);
				drawString4(150, 3, hpDisplay, 250);
				drawRect4(151, 75, 6, playerHealth, 246);
				drawRect4(151, 75 + playerHealth, 6, 100 - playerHealth, 247);

				// MODE 4
				waitForVblank();
				FlipPage();
			}
		} else if (state == 5) { // Stage 2
			int keepLooping = 1;
			int counter = 0;
			int upPressed = 0;
			int downPressed = 0;
			int playerInvincibility = 0;
			int cooldown = 0;
			int drawPlayer = 1;
			int generation = 1;
			// Create SPIDER enemies for this stage
			SPIDER spiders[30];
			int nextEmptySpider = 0;
			for (int i = 0; i < 30; i++) { // Initialize them
				spiders[i].arrayPosition = 987654;
			}

			while (keepLooping) {
				CLEAR;
				if (counter % (20 +  (rand() % 60)) == 0 && cooldown == 0 && generation == 1) {
					SPIDER temp;
					createSpider(&temp, 1, &nextEmptySpider, 0, 2, 0, -1, 0, 244);
					spiders[nextEmptySpider] = temp;
					nextEmptySpider++;
					if (nextEmptySpider >= 30) {
						nextEmptySpider = 0;
					}
					cooldown = 20;
				}

				if (playerHealth <= 0) {
					keepLooping = 0;
					state = 999;
				}

				// Drawing

				// If player is invulnerable, flash the player to give player notice
				if (playerInvincibility > 0) {
					if (drawPlayer == 1) {
						drawPlayer = 0;
					} else {
						drawPlayer = 1;
					}
				} else {
					drawPlayer = 1;
				}

				// Drawing the objects
				drawBasics(&player, &topBar, &middleBar, &bottomBar, &drawPlayer);
				updateSpiders(spiders, &playerHealth, &player, &playerInvincibility);
				playerMovement(&player, &upPressed, &downPressed, &topBar, &middleBar, &bottomBar);

				if (KEY_DOWN_NOW(BUTTON_SELECT)) {
					keepLooping = 0;
					state = 0;
				}

				// Cooldown to prevent walls of spiders from appearing
				if (cooldown != 0) {
					cooldown--;
				}

				// If player gets hit, they get invincibility frames that last 100 loops. This decreases the remaining frames.
				if (playerInvincibility != 0) {
					playerInvincibility--;
				}

				// Displays HP
				char hpDisplay[20];
				sprintf(hpDisplay, "Health: %d", playerHealth);
				drawString4(150, 3, hpDisplay, 250);
				drawRect4(151, 75, 6, playerHealth, 246);
				drawRect4(151, 75 + playerHealth, 6, 100 - playerHealth, 247);

				// MODE 4
				waitForVblank();
				FlipPage();

				counter++;
				if (counter > 1000 && counter < 1100) {
					generation = 0;
				} else if (counter > 1200) {
					keepLooping = 0;
					state = 6;
				}
			}
		} else if (state == 6) { // Story 3
			int keepLooping = 1;
			int counter = 0;
			int upPressed = 0;
			int downPressed = 0;
			int drawPlayer = 1;
			char text[50];
			char printing[50];
			char *textPointer = &text[0];
			int letters = 0;
			while (keepLooping) {
				CLEAR;

				// STORY TEXT HERE
				if (counter < 50) {
					memset(printing, 0, 50);
				}
				if (counter >= 50 && counter < 200) {
					sprintf(text, "I guess this is too easy.");
					printText(2);
					if (counter == 199) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 200 && counter < 350) {
					sprintf(text, "Don't worry!");
					printText(2);
					if (counter == 349) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 350 && counter < 500) {
					sprintf(text, "Since I'm such a great host,");
					printText(2);
					if (counter == 499) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 500 && counter < 750) {
					sprintf(text, "I'll keep you entertained.");
					printText(2);
					if (counter == 749) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 750) {
					keepLooping = 0;
					state = 7;
				}
				drawString4(20, 10, printing, 244);
				drawBasics(&player, &topBar, &middleBar, &bottomBar, &drawPlayer);
				playerMovement(&player, &upPressed, &downPressed, &topBar, &middleBar, &bottomBar);

				if (KEY_DOWN_NOW(BUTTON_SELECT)) {
					keepLooping = 0;
					state = 0;
				}

				// Increments counter of the loop, this is basically the timer
				counter++;
				if (counter > 987654) {
					counter = 0;
				}

				// Displays HP
				char hpDisplay[20];
				sprintf(hpDisplay, "Health: %d", playerHealth);
				drawString4(150, 3, hpDisplay, 250);
				drawRect4(151, 75, 6, playerHealth, 246);
				drawRect4(151, 75 + playerHealth, 6, 100 - playerHealth, 247);

				// MODE 4
				waitForVblank();
				FlipPage();
			}
		} else if (state == 7) { // Stage 3
			int keepLooping = 1;
			int counter = 0;
			int upPressed = 0;
			int downPressed = 0;
			int playerInvincibility = 0;
			int cooldown = 0;
			int drawPlayer = 1;
			int generation = 1;
			// Create SPIDER enemies for this stage
			SPIDER spiders[30];
			int nextEmptySpider = 0;
			for (int i = 0; i < 30; i++) { // Initialize them
				spiders[i].arrayPosition = 987654;
			}

			while (keepLooping) {
				CLEAR;
				if (counter % (10 +  (rand() % 40)) == 0 && cooldown == 0 && generation == 1) {
					SPIDER temp;
					createSpider(&temp, 1, &nextEmptySpider, -1, -1, -1, -1, 0, 244);
					spiders[nextEmptySpider] = temp;
					nextEmptySpider++;
					if (nextEmptySpider >= 30) {
						nextEmptySpider = 0;
					}
					cooldown = 20;
				}

				if (playerHealth <= 0) {
					keepLooping = 0;
					state = 999;
				}

				// Drawing

				// If player is invulnerable, flash the player to give player notice
				if (playerInvincibility > 0) {
					if (drawPlayer == 1) {
						drawPlayer = 0;
					} else {
						drawPlayer = 1;
					}
				} else {
					drawPlayer = 1;
				}

				// Drawing the objects
				drawBasics(&player, &topBar, &middleBar, &bottomBar, &drawPlayer);
				updateSpiders(spiders, &playerHealth, &player, &playerInvincibility);
				playerMovement(&player, &upPressed, &downPressed, &topBar, &middleBar, &bottomBar);

				if (KEY_DOWN_NOW(BUTTON_SELECT)) {
					keepLooping = 0;
					state = 0;
				}

				// Cooldown to prevent walls of spiders from appearing
				if (cooldown != 0) {
					cooldown--;
				}

				// If player gets hit, they get invincibility frames that last 100 loops. This decreases the remaining frames.
				if (playerInvincibility != 0) {
					playerInvincibility--;
				}

				// Displays HP
				char hpDisplay[20];
				sprintf(hpDisplay, "Health: %d", playerHealth);
				drawString4(150, 3, hpDisplay, 250);
				drawRect4(151, 75, 6, playerHealth, 246);
				drawRect4(151, 75 + playerHealth, 6, 100 - playerHealth, 247);

				// MODE 4
				waitForVblank();
				FlipPage();

				counter++;
				if (counter > 1000 && counter < 1100) {
					generation = 0;
				} else if (counter > 1200) {
					keepLooping = 0;
					state = 8;
				}
			}
		} else if (state == 8) { // Story 4
			int keepLooping = 1;
			int counter = 0;
			int upPressed = 0;
			int downPressed = 0;
			int drawPlayer = 1;
			char text[50];
			char printing[50];
			char *textPointer = &text[0];
			int letters = 0;
			while (keepLooping) {
				CLEAR;

				// STORY TEXT HERE
				if (counter < 50) {
					memset(printing, 0, 50);
				}
				if (counter >= 50 && counter < 200) {
					sprintf(text, "What?");
					printText(2);
					if (counter == 199) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 200 && counter < 350) {
					sprintf(text, "You didn't think those spiders");
					printText(2);
					if (counter == 349) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 350 && counter < 500) {
					sprintf(text, "could come from the other side?");
					printText(2);
					if (counter == 499) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 500 && counter < 650) {
					sprintf(text, "You're far too naive.");
					printText(2);
					if (counter == 649) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 650 && counter < 800) {
					sprintf(text, "You wouldn't survive anywhere.");
					printText(2);
					if (counter == 799) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 800 && counter < 950) {
					sprintf(text, "So you might as well die now.");
					printText(2);
					if (counter == 949) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 950) {
					keepLooping = 0;
					state = 9;
				}
				drawString4(20, 10, printing, 244);
				drawBasics(&player, &topBar, &middleBar, &bottomBar, &drawPlayer);
				playerMovement(&player, &upPressed, &downPressed, &topBar, &middleBar, &bottomBar);

				if (KEY_DOWN_NOW(BUTTON_SELECT)) {
					keepLooping = 0;
					state = 0;
				}

				// Increments counter of the loop, this is basically the timer
				counter++;
				if (counter > 987654) {
					counter = 0;
				}

				// Displays HP
				char hpDisplay[20];
				sprintf(hpDisplay, "Health: %d", playerHealth);
				drawString4(150, 3, hpDisplay, 250);
				drawRect4(151, 75, 6, playerHealth, 246);
				drawRect4(151, 75 + playerHealth, 6, 100 - playerHealth, 247);

				// MODE 4
				waitForVblank();
				FlipPage();
			}
		} else if (state == 9) { // Stage 4
			int keepLooping = 1;
			int counter = 0;
			int upPressed = 0;
			int downPressed = 0;
			int playerInvincibility = 0;
			int cooldown = 0;
			int drawPlayer = 1;
			int generation = 1;
			// Create SPIDER enemies for this stage
			SPIDER spiders[30];
			int nextEmptySpider = 0;
			for (int i = 0; i < 30; i++) { // Initialize them
				spiders[i].arrayPosition = 987654;
			}

			while (keepLooping) {
				CLEAR;
				if (counter % (10 +  (rand() % 30)) == 0 && cooldown == 0 && generation == 1) {
					SPIDER temp;
					createSpider(&temp, 1, &nextEmptySpider, -1, -1, -1, -1, 0, 244);
					spiders[nextEmptySpider] = temp;
					nextEmptySpider++;
					if (nextEmptySpider >= 30) {
						nextEmptySpider = 0;
					}
					cooldown = 12;
				}

				if (playerHealth <= 0) {
					keepLooping = 0;
					state = 999;
				}

				// Drawing

				// If player is invulnerable, flash the player to give player notice
				if (playerInvincibility > 0) {
					if (drawPlayer == 1) {
						drawPlayer = 0;
					} else {
						drawPlayer = 1;
					}
				} else {
					drawPlayer = 1;
				}

				// Drawing the objects
				drawBasics(&player, &topBar, &middleBar, &bottomBar, &drawPlayer);
				updateSpiders(spiders, &playerHealth, &player, &playerInvincibility);
				playerMovement(&player, &upPressed, &downPressed, &topBar, &middleBar, &bottomBar);

				if (KEY_DOWN_NOW(BUTTON_SELECT)) {
					keepLooping = 0;
					state = 0;
				}

				// Cooldown to prevent walls of spiders from appearing
				if (cooldown != 0) {
					cooldown--;
				}

				// If player gets hit, they get invincibility frames that last 100 loops. This decreases the remaining frames.
				if (playerInvincibility != 0) {
					playerInvincibility--;
				}

				// Displays HP
				char hpDisplay[20];
				sprintf(hpDisplay, "Health: %d", playerHealth);
				drawString4(150, 3, hpDisplay, 250);
				drawRect4(151, 75, 6, playerHealth, 246);
				drawRect4(151, 75 + playerHealth, 6, 100 - playerHealth, 247);

				// MODE 4
				waitForVblank();
				FlipPage();

				counter++;
				if (counter > 1000 && counter < 1100) {
					generation = 0;
				} else if (counter > 1200) {
					keepLooping = 0;
					state = 10;
				}
			}
		} else if (state == 10) { // Story 5
			int keepLooping = 1;
			int counter = 0;
			int upPressed = 0;
			int downPressed = 0;
			int drawPlayer = 1;
			char text[50];
			char printing[50];
			char *textPointer = &text[0];
			int letters = 0;
			while (keepLooping) {
				CLEAR;

				// STORY TEXT HERE
				if (counter < 50) {
					memset(printing, 0, 50);
				}
				if (counter >= 50 && counter < 200) {
					sprintf(text, "There's no kindness in this world.");
					printText(2);
					if (counter == 199) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 200 && counter < 350) {
					sprintf(text, "There's only darkness.");
					printText(2);
					if (counter == 349) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 350 && counter < 500) {
					sprintf(text, "Violence, Lies, and Ruin.");
					printText(2);
					if (counter == 499) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 500 && counter < 650) {
					sprintf(text, "Is that what you want?");
					printText(2);
					if (counter == 649) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 650) {
					keepLooping = 0;
					state = 11;
				}
				drawString4(20, 10, printing, 244);
				drawBasics(&player, &topBar, &middleBar, &bottomBar, &drawPlayer);
				playerMovement(&player, &upPressed, &downPressed, &topBar, &middleBar, &bottomBar);

				if (KEY_DOWN_NOW(BUTTON_SELECT)) {
					keepLooping = 0;
					state = 0;
				}

				// Increments counter of the loop, this is basically the timer
				counter++;
				if (counter > 987654) {
					counter = 0;
				}

				// Displays HP
				char hpDisplay[20];
				sprintf(hpDisplay, "Health: %d", playerHealth);
				drawString4(150, 3, hpDisplay, 250);
				drawRect4(151, 75, 6, playerHealth, 246);
				drawRect4(151, 75 + playerHealth, 6, 100 - playerHealth, 247);

				// MODE 4
				waitForVblank();
				FlipPage();
			}
		} else if (state == 11) { // Stage 5
			int keepLooping = 1;
			int counter = 0;
			int upPressed = 0;
			int downPressed = 0;
			int playerInvincibility = 0;
			int cooldown = 0;
			int drawPlayer = 1;
			int generation = 1;
			// Create SPIDER enemies for this stage
			SPIDER spiders[30];
			int nextEmptySpider = 0;
			for (int i = 0; i < 30; i++) { // Initialize them
				spiders[i].arrayPosition = 987654;
			}

			while (keepLooping) {
				CLEAR;
				if (counter % (5 +  (rand() % 25)) == 0 && cooldown == 0 && generation == 1) {
					SPIDER temp;
					createSpider(&temp, 1, &nextEmptySpider, -1, -1, -1, -1, 0, 244);
					spiders[nextEmptySpider] = temp;
					nextEmptySpider++;
					if (nextEmptySpider >= 30) {
						nextEmptySpider = 0;
					}
					cooldown = 15;
				}

				if (playerHealth <= 0) {
					keepLooping = 0;
					state = 999;
				}

				// Drawing

				// If player is invulnerable, flash the player to give player notice
				if (playerInvincibility > 0) {
					if (drawPlayer == 1) {
						drawPlayer = 0;
					} else {
						drawPlayer = 1;
					}
				} else {
					drawPlayer = 1;
				}

				// Drawing the objects
				drawBasics(&player, &topBar, &middleBar, &bottomBar, &drawPlayer);
				updateSpiders(spiders, &playerHealth, &player, &playerInvincibility);
				playerMovement(&player, &upPressed, &downPressed, &topBar, &middleBar, &bottomBar);

				if (KEY_DOWN_NOW(BUTTON_SELECT)) {
					keepLooping = 0;
					state = 0;
				}

				// Cooldown to prevent walls of spiders from appearing
				if (cooldown != 0) {
					cooldown--;
				}

				// If player gets hit, they get invincibility frames that last 100 loops. This decreases the remaining frames.
				if (playerInvincibility != 0) {
					playerInvincibility--;
				}

				// Displays HP
				char hpDisplay[20];
				sprintf(hpDisplay, "Health: %d", playerHealth);
				drawString4(150, 3, hpDisplay, 250);
				drawRect4(151, 75, 6, playerHealth, 246);
				drawRect4(151, 75 + playerHealth, 6, 100 - playerHealth, 247);

				// MODE 4
				waitForVblank();
				FlipPage();

				counter++;
				if (counter > 1000 && counter < 1100) {
					generation = 0;
				} else if (counter > 1200) {
					keepLooping = 0;
					state = 12;
				}
			}
		} else if (state == 12) { // Story 6
			int keepLooping = 1;
			int counter = 0;
			int upPressed = 0;
			int downPressed = 0;
			int drawPlayer = 1;
			char text[50];
			char printing[50];
			char *textPointer = &text[0];
			int letters = 0;
			while (keepLooping) {
				CLEAR;

				// STORY TEXT HERE
				if (counter < 50) {
					memset(printing, 0, 50);
				}
				if (counter >= 50 && counter < 200) {
					sprintf(text, "Quite impressive.");
					printText(2);
					if (counter == 199) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 200 && counter < 350) {
					sprintf(text, "I guess you deserve a reward.");
					printText(2);
					if (counter == 349) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 350 && counter < 500) {
					sprintf(text, "You want it, right?");
					printText(2);
					if (counter == 499) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 500 && counter < 650) {
					sprintf(text, "Hah.");
					printText(2);
					if (counter == 649) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 650) {
					keepLooping = 0;
					state = 13;
				}
				drawString4(20, 10, printing, 244);
				drawBasics(&player, &topBar, &middleBar, &bottomBar, &drawPlayer);
				playerMovement(&player, &upPressed, &downPressed, &topBar, &middleBar, &bottomBar);

				if (KEY_DOWN_NOW(BUTTON_SELECT)) {
					keepLooping = 0;
					state = 0;
				}
				if (counter >= 500 && counter < 520) {
					playerHealth--;
					if (playerHealth <= 1) {
						playerHealth = 1;
					}
				}
				// Increments counter of the loop, this is basically the timer
				counter++;
				if (counter > 987654) {
					counter = 0;
				}

				// Displays HP
				char hpDisplay[20];
				sprintf(hpDisplay, "Health: %d", playerHealth);
				drawString4(150, 3, hpDisplay, 250);
				drawRect4(151, 75, 6, playerHealth, 246);
				drawRect4(151, 75 + playerHealth, 6, 100 - playerHealth, 247);

				// MODE 4
				waitForVblank();
				FlipPage();
			}
		} else if (state == 13) { // Stage 6
			int keepLooping = 1;
			int counter = 0;
			int upPressed = 0;
			int downPressed = 0;
			int playerInvincibility = 0;
			int cooldown = 0;
			int drawPlayer = 1;
			int generation = 1;
			// Create SPIDER enemies for this stage
			SPIDER spiders[30];
			int nextEmptySpider = 0;
			for (int i = 0; i < 30; i++) { // Initialize them
				spiders[i].arrayPosition = 987654;
			}

			while (keepLooping) {
				CLEAR;
				if (counter % (5 +  (rand() % 25)) == 0 && cooldown == 0 && generation == 1) {
					SPIDER temp;
					createSpider(&temp, 1, &nextEmptySpider, -1, -1, -1, -1, 0, 244);
					spiders[nextEmptySpider] = temp;
					nextEmptySpider++;
					if (nextEmptySpider >= 30) {
						nextEmptySpider = 0;
					}
					cooldown = 15;
				}

				if (playerHealth <= 0) {
					keepLooping = 0;
					state = 999;
				}

				// Drawing

				// If player is invulnerable, flash the player to give player notice
				if (playerInvincibility > 0) {
					if (drawPlayer == 1) {
						drawPlayer = 0;
					} else {
						drawPlayer = 1;
					}
				} else {
					drawPlayer = 1;
				}

				// Drawing the objects
				drawBasics(&player, &topBar, &middleBar, &bottomBar, &drawPlayer);
				updateSpiders(spiders, &playerHealth, &player, &playerInvincibility);
				playerMovement(&player, &upPressed, &downPressed, &topBar, &middleBar, &bottomBar);

				if (KEY_DOWN_NOW(BUTTON_SELECT)) {
					keepLooping = 0;
					state = 0;
				}

				// Cooldown to prevent walls of spiders from appearing
				if (cooldown != 0) {
					cooldown--;
				}

				// If player gets hit, they get invincibility frames that last 100 loops. This decreases the remaining frames.
				if (playerInvincibility != 0) {
					playerInvincibility--;
				}

				// Displays HP
				char hpDisplay[20];
				sprintf(hpDisplay, "Health: %d", playerHealth);
				drawString4(150, 3, hpDisplay, 250);
				drawRect4(151, 75, 6, playerHealth, 246);
				drawRect4(151, 75 + playerHealth, 6, 100 - playerHealth, 247);

				// MODE 4
				waitForVblank();
				FlipPage();

				counter++;
				if (counter > 1000 && counter < 1100) {
					generation = 0;
				} else if (counter > 1200) {
					keepLooping = 0;
					state = 14;
				}
			}
		} else if (state == 14) { // Story 7
			int keepLooping = 1;
			int counter = 0;
			int upPressed = 0;
			int downPressed = 0;
			int drawPlayer = 1;
			char text[50];
			char printing[50];
			char *textPointer = &text[0];
			int letters = 0;
			while (keepLooping) {
				CLEAR;

				// STORY TEXT HERE
				if (counter < 50) {
					memset(printing, 0, 50);
				}
				if (counter >= 50 && counter < 200) {
					sprintf(text, "You must be mad.");
					printText(2);
					if (counter == 199) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 200 && counter < 350) {
					sprintf(text, "I was only trying to help.");
					printText(2);
					if (counter == 349) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 350 && counter < 500) {
					sprintf(text, "Don't you do better with pressure?");
					printText(2);
					if (counter == 499) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 500 && counter < 650) {
					sprintf(text, "Well, whatever.");
					printText(2);
					if (counter == 649) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 650 && counter < 800) {
					sprintf(text, "I'll heal you.");
					printText(2);
					if (counter == 799) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 800 && counter < 950) {
					sprintf(text, "Only so I can play with you");
					printText(2);
					if (counter == 949) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 950 && counter < 1100) {
					sprintf(text, "a little bit longer.");
					printText(2);
					if (counter == 1099) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 1100) {
					keepLooping = 0;
					state = 15;
				}

				if (counter >= 650 && counter < 700) {
					playerHealth = playerHealth + 1;
					if (playerHealth > 100) {
						playerHealth = 100;
					}
				}
				drawString4(20, 10, printing, 244);
				drawBasics(&player, &topBar, &middleBar, &bottomBar, &drawPlayer);
				playerMovement(&player, &upPressed, &downPressed, &topBar, &middleBar, &bottomBar);

				if (KEY_DOWN_NOW(BUTTON_SELECT)) {
					keepLooping = 0;
					state = 0;
				}

				// Increments counter of the loop, this is basically the timer
				counter++;
				if (counter > 987654) {
					counter = 0;
				}

				// Displays HP
				char hpDisplay[20];
				sprintf(hpDisplay, "Health: %d", playerHealth);
				drawString4(150, 3, hpDisplay, 250);
				drawRect4(151, 75, 6, playerHealth, 246);
				drawRect4(151, 75 + playerHealth, 6, 100 - playerHealth, 247);

				// MODE 4
				waitForVblank();
				FlipPage();
			}
		} else if (state == 15) { // Stage 7
			int keepLooping = 1;
			int counter = 0;
			int upPressed = 0;
			int downPressed = 0;
			int playerInvincibility = 0;
			int cooldown = 0;
			int drawPlayer = 1;
			int generation = 1;
			// Create SPIDER enemies for this stage
			SPIDER spiders[30];
			int nextEmptySpider = 0;
			for (int i = 0; i < 30; i++) { // Initialize them
				spiders[i].arrayPosition = 987654;
			}

			while (keepLooping) {
				CLEAR;
				if (counter % (5 +  (rand() % 25)) == 0 && cooldown == 0 && generation == 1) {
					SPIDER temp;
					createSpider(&temp, 1, &nextEmptySpider, -1, 3, -1, -1, 0, 244);
					spiders[nextEmptySpider] = temp;
					nextEmptySpider++;
					if (nextEmptySpider >= 30) {
						nextEmptySpider = 0;
					}
					cooldown = 15;
				}

				if (playerHealth <= 0) {
					keepLooping = 0;
					state = 999;
				}

				// Drawing

				// If player is invulnerable, flash the player to give player notice
				if (playerInvincibility > 0) {
					if (drawPlayer == 1) {
						drawPlayer = 0;
					} else {
						drawPlayer = 1;
					}
				} else {
					drawPlayer = 1;
				}

				// Drawing the objects
				drawBasics(&player, &topBar, &middleBar, &bottomBar, &drawPlayer);
				updateSpiders(spiders, &playerHealth, &player, &playerInvincibility);
				playerMovement(&player, &upPressed, &downPressed, &topBar, &middleBar, &bottomBar);

				if (KEY_DOWN_NOW(BUTTON_SELECT)) {
					keepLooping = 0;
					state = 0;
				}

				// Cooldown to prevent walls of spiders from appearing
				if (cooldown != 0) {
					cooldown--;
				}

				// If player gets hit, they get invincibility frames that last 100 loops. This decreases the remaining frames.
				if (playerInvincibility != 0) {
					playerInvincibility--;
				}

				// Displays HP
				char hpDisplay[20];
				sprintf(hpDisplay, "Health: %d", playerHealth);
				drawString4(150, 3, hpDisplay, 250);
				drawRect4(151, 75, 6, playerHealth, 246);
				drawRect4(151, 75 + playerHealth, 6, 100 - playerHealth, 247);

				// MODE 4
				waitForVblank();
				FlipPage();

				counter++;
				if (counter > 1000 && counter < 1100) {
					generation = 0;
				} else if (counter > 1200) {
					keepLooping = 0;
					state = 16;
				}
			}
		} else if (state == 16) { // Story 8
			int keepLooping = 1;
			int counter = 0;
			int upPressed = 0;
			int downPressed = 0;
			int drawPlayer = 1;
			char text[50];
			char printing[50];
			char *textPointer = &text[0];
			int letters = 0;
			while (keepLooping) {
				CLEAR;

				// STORY TEXT HERE
				if (counter < 50) {
					memset(printing, 0, 50);
				}
				if (counter >= 50 && counter < 200) {
					sprintf(text, "Random spawns can only be so fun.");
					printText(2);
					if (counter == 199) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 200 && counter < 350) {
					sprintf(text, "Maybe I'll do something...");
					printText(2);
					if (counter == 349) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 350 && counter < 500) {
					sprintf(text, "...more interesting.");
					printText(2);
					if (counter == 499) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 500) {
					keepLooping = 0;
					state = 17;
				}

				drawString4(20, 10, printing, 244);
				drawBasics(&player, &topBar, &middleBar, &bottomBar, &drawPlayer);
				playerMovement(&player, &upPressed, &downPressed, &topBar, &middleBar, &bottomBar);

				if (KEY_DOWN_NOW(BUTTON_SELECT)) {
					keepLooping = 0;
					state = 0;
				}

				// Increments counter of the loop, this is basically the timer
				counter++;
				if (counter > 987654) {
					counter = 0;
				}

				// Displays HP
				char hpDisplay[20];
				sprintf(hpDisplay, "Health: %d", playerHealth);
				drawString4(150, 3, hpDisplay, 250);
				drawRect4(151, 75, 6, playerHealth, 246);
				drawRect4(151, 75 + playerHealth, 6, 100 - playerHealth, 247);

				// MODE 4
				waitForVblank();
				FlipPage();
			}
		} else if (state == 17) { // Stage 8
			int keepLooping = 1;
			int counter = 0;
			int upPressed = 0;
			int downPressed = 0;
			int playerInvincibility = 0;
			int cooldown = 0;
			int drawPlayer = 1;
			int generation = 1;
			int blockCount = 0;
			int generate = 0;
			int generateGap = 50;
			// Create SPIDER enemies for this stage
			SPIDER spiders[30];
			int nextEmptySpider = 0;
			for (int i = 0; i < 30; i++) { // Initialize them
				spiders[i].arrayPosition = 987654;
			}

			while (keepLooping) {
				CLEAR;
				if (playerHealth <= 0) {
					keepLooping = 0;
					state = 999;
				}
				if (counter == generate && generation == 1) {
					SPIDER temp;
					if (blockCount == 0) {
						createSpider(&temp, 1, &nextEmptySpider, 0, 3, 0, 45, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 0, 3, 0, 75, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						blockCount = 1;
					} else {
						createSpider(&temp, 1, &nextEmptySpider, 0, 3, 0, 75, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 0, 3, 0, 105, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						blockCount = 0;
					}
					generate = generate + generateGap;
					if (generateGap >= 25) {
						generateGap = generateGap - 2;
					}
				}

				if (playerHealth <= 0) {
					keepLooping = 0;
					state = 999;
				}

				// Drawing

				// If player is invulnerable, flash the player to give player notice
				if (playerInvincibility > 0) {
					if (drawPlayer == 1) {
						drawPlayer = 0;
					} else {
						drawPlayer = 1;
					}
				} else {
					drawPlayer = 1;
				}

				// Drawing the objects
				drawBasics(&player, &topBar, &middleBar, &bottomBar, &drawPlayer);
				updateSpiders(spiders, &playerHealth, &player, &playerInvincibility);
				playerMovement(&player, &upPressed, &downPressed, &topBar, &middleBar, &bottomBar);

				if (KEY_DOWN_NOW(BUTTON_SELECT)) {
					keepLooping = 0;
					state = 0;
				}

				// Cooldown to prevent walls of spiders from appearing
				if (cooldown != 0) {
					cooldown--;
				}

				// If player gets hit, they get invincibility frames that last 100 loops. This decreases the remaining frames.
				if (playerInvincibility != 0) {
					playerInvincibility--;
				}

				// Displays HP
				char hpDisplay[20];
				sprintf(hpDisplay, "Health: %d", playerHealth);
				drawString4(150, 3, hpDisplay, 250);
				drawRect4(151, 75, 6, playerHealth, 246);
				drawRect4(151, 75 + playerHealth, 6, 100 - playerHealth, 247);

				// MODE 4
				waitForVblank();
				FlipPage();

				counter++;
				if (counter > 700 && counter < 800) {
					generation = 0;
				} else if (counter > 800) {
					keepLooping = 0;
					state = 18;
				}
			}
		} else if (state == 18) { // Story 9
			int keepLooping = 1;
			int counter = 0;
			int upPressed = 0;
			int downPressed = 0;
			int drawPlayer = 1;
			char text[50];
			char printing[50];
			char *textPointer = &text[0];
			int letters = 0;
			while (keepLooping) {
				CLEAR;

				// STORY TEXT HERE
				if (counter < 50) {
					memset(printing, 0, 50);
				}
				if (counter >= 50 && counter < 200) {
					sprintf(text, "Isn't this fun?");
					printText(2);
					if (counter == 199) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 200 && counter < 350) {
					sprintf(text, "Why don't we mix them both?");
					printText(2);
					if (counter == 349) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 350) {
					keepLooping = 0;
					state = 19;
				}

				drawString4(20, 10, printing, 244);
				drawBasics(&player, &topBar, &middleBar, &bottomBar, &drawPlayer);
				playerMovement(&player, &upPressed, &downPressed, &topBar, &middleBar, &bottomBar);

				if (KEY_DOWN_NOW(BUTTON_SELECT)) {
					keepLooping = 0;
					state = 0;
				}

				// Increments counter of the loop, this is basically the timer
				counter++;
				if (counter > 987654) {
					counter = 0;
				}

				// Displays HP
				char hpDisplay[20];
				sprintf(hpDisplay, "Health: %d", playerHealth);
				drawString4(150, 3, hpDisplay, 250);
				drawRect4(151, 75, 6, playerHealth, 246);
				drawRect4(151, 75 + playerHealth, 6, 100 - playerHealth, 247);

				// MODE 4
				waitForVblank();
				FlipPage();
			}
		} else if (state == 19) { // Stage 9
			int keepLooping = 1;
			int counter = 0;
			int upPressed = 0;
			int downPressed = 0;
			int playerInvincibility = 0;
			int cooldown = 0;
			int drawPlayer = 1;
			int generation = 1;
			int blockCount = 0;
			int generate = 0;
			int generateGap = 40;
			// Create SPIDER enemies for this stage
			SPIDER spiders[30];
			int nextEmptySpider = 0;
			for (int i = 0; i < 30; i++) { // Initialize them
				spiders[i].arrayPosition = 987654;
			}

			while (keepLooping) {
				CLEAR;
				if (counter % (10 +  (rand() % 40)) == 0 && cooldown == 0 && generation == 1) {
					SPIDER temp;
					createSpider(&temp, 1, &nextEmptySpider, 1, 2, 230, -1, 0, 244);
					spiders[nextEmptySpider] = temp;
					nextEmptySpider++;
					if (nextEmptySpider >= 30) {
						nextEmptySpider = 0;
					}
					cooldown = 25;
				}
				if (counter == generate && generation == 1) {
					SPIDER temp;
					if (blockCount == 0) {
						createSpider(&temp, 1, &nextEmptySpider, 0, 2, 0, 45, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 0, 2, 0, 75, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						blockCount = 1;
					} else {
						createSpider(&temp, 1, &nextEmptySpider, 0, 2, 0, 75, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 0, 2, 0, 105, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						blockCount = 0;
					}
					generate = generate + generateGap;
				}

				if (playerHealth <= 0) {
					keepLooping = 0;
					state = 999;
				}
				// Drawing

				// If player is invulnerable, flash the player to give player notice
				if (playerInvincibility > 0) {
					if (drawPlayer == 1) {
						drawPlayer = 0;
					} else {
						drawPlayer = 1;
					}
				} else {
					drawPlayer = 1;
				}

				// Drawing the objects
				drawBasics(&player, &topBar, &middleBar, &bottomBar, &drawPlayer);
				updateSpiders(spiders, &playerHealth, &player, &playerInvincibility);
				playerMovement(&player, &upPressed, &downPressed, &topBar, &middleBar, &bottomBar);

				if (KEY_DOWN_NOW(BUTTON_SELECT)) {
					keepLooping = 0;
					state = 0;
				}

				// Cooldown to prevent walls of spiders from appearing
				if (cooldown != 0) {
					cooldown--;
				}

				// If player gets hit, they get invincibility frames that last 100 loops. This decreases the remaining frames.
				if (playerInvincibility != 0) {
					playerInvincibility--;
				}

				// Displays HP
				char hpDisplay[20];
				sprintf(hpDisplay, "Health: %d", playerHealth);
				drawString4(150, 3, hpDisplay, 250);
				drawRect4(151, 75, 6, playerHealth, 246);
				drawRect4(151, 75 + playerHealth, 6, 100 - playerHealth, 247);

				// MODE 4
				waitForVblank();
				FlipPage();

				counter++;
				if (counter > 700 && counter < 800) {
					generation = 0;
				} else if (counter > 800) {
					keepLooping = 0;
					state = 20;
				}
			}
		} else if (state == 20) { // Story 10
			int keepLooping = 1;
			int counter = 0;
			int upPressed = 0;
			int downPressed = 0;
			int drawPlayer = 1;
			char text[50];
			char printing[50];
			char *textPointer = &text[0];
			int letters = 0;
			while (keepLooping) {
				CLEAR;

				// STORY TEXT HERE
				if (counter < 50) {
					memset(printing, 0, 50);
				}
				if (counter >= 50 && counter < 200) {
					sprintf(text, "Perhaps it's getting difficult?");
					printText(2);
					if (counter == 199) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 200 && counter < 350) {
					sprintf(text, "Well, you should be fine.");
					printText(2);
					if (counter == 349) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 350 && counter < 500) {
					sprintf(text, "After all...");
					printText(2);
					if (counter == 499) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 500 && counter < 650) {
					sprintf(text, "You want to survive out there...");
					printText(2);
					if (counter == 649) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 650 && counter < 800) {
					sprintf(text, "Don't you?");
					printText(2);
					if (counter == 799) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 800 && counter < 950) {
					sprintf(text, "This is nothing in comparison.");
					printText(2);
					if (counter == 949) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 950) {
					keepLooping = 0;
					state = 21;
				}

				drawString4(20, 10, printing, 244);
				drawBasics(&player, &topBar, &middleBar, &bottomBar, &drawPlayer);
				playerMovement(&player, &upPressed, &downPressed, &topBar, &middleBar, &bottomBar);

				if (KEY_DOWN_NOW(BUTTON_SELECT)) {
					keepLooping = 0;
					state = 0;
				}

				// Increments counter of the loop, this is basically the timer
				counter++;
				if (counter > 987654) {
					counter = 0;
				}

				// Displays HP
				char hpDisplay[20];
				sprintf(hpDisplay, "Health: %d", playerHealth);
				drawString4(150, 3, hpDisplay, 250);
				drawRect4(151, 75, 6, playerHealth, 246);
				drawRect4(151, 75 + playerHealth, 6, 100 - playerHealth, 247);

				// MODE 4
				waitForVblank();
				FlipPage();
			}
		} else if (state == 21) { // Stage 10
			int keepLooping = 1;
			int counter = 0;
			int upPressed = 0;
			int downPressed = 0;
			int playerInvincibility = 0;
			int cooldown = 0;
			int drawPlayer = 1;
			int generation = 1;
			int blockCount = 0;
			int generate = 0;
			int generateGap = 40;
			// Create SPIDER enemies for this stage
			SPIDER spiders[30];
			int nextEmptySpider = 0;
			for (int i = 0; i < 30; i++) { // Initialize them
				spiders[i].arrayPosition = 987654;
			}

			while (keepLooping) {
				CLEAR;
				if (counter % (10 +  (rand() % 35)) == 0 && cooldown == 0 && generation == 1) {
					SPIDER temp;
					createSpider(&temp, 1, &nextEmptySpider, -1, 3, -1, -1, 0, 244);
					spiders[nextEmptySpider] = temp;
					nextEmptySpider++;
					if (nextEmptySpider >= 30) {
						nextEmptySpider = 0;
					}
					cooldown = 30;
				}
				if (counter == generate && generation == 1) {
					SPIDER temp;
					if (blockCount == 0) {
						createSpider(&temp, 1, &nextEmptySpider, 0, 2, 0, 45, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 0, 2, 0, 75, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						blockCount = 1;
					} else {
						createSpider(&temp, 1, &nextEmptySpider, 0, 2, 0, 75, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 0, 2, 0, 105, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						blockCount = 0;
					}
					generate = generate + generateGap;
				}

				if (playerHealth <= 0) {
					keepLooping = 0;
					state = 999;
				}

				// Drawing

				// If player is invulnerable, flash the player to give player notice
				if (playerInvincibility > 0) {
					if (drawPlayer == 1) {
						drawPlayer = 0;
					} else {
						drawPlayer = 1;
					}
				} else {
					drawPlayer = 1;
				}

				// Drawing the objects
				drawBasics(&player, &topBar, &middleBar, &bottomBar, &drawPlayer);
				updateSpiders(spiders, &playerHealth, &player, &playerInvincibility);
				playerMovement(&player, &upPressed, &downPressed, &topBar, &middleBar, &bottomBar);

				if (KEY_DOWN_NOW(BUTTON_SELECT)) {
					keepLooping = 0;
					state = 0;
				}

				// Cooldown to prevent walls of spiders from appearing
				if (cooldown != 0) {
					cooldown--;
				}

				// If player gets hit, they get invincibility frames that last 100 loops. This decreases the remaining frames.
				if (playerInvincibility != 0) {
					playerInvincibility--;
				}

				// Displays HP
				char hpDisplay[20];
				sprintf(hpDisplay, "Health: %d", playerHealth);
				drawString4(150, 3, hpDisplay, 250);
				drawRect4(151, 75, 6, playerHealth, 246);
				drawRect4(151, 75 + playerHealth, 6, 100 - playerHealth, 247);

				// MODE 4
				waitForVblank();
				FlipPage();

				counter++;
				if (counter > 700 && counter < 800) {
					generation = 0;
				} else if (counter > 800) {
					keepLooping = 0;
					state = 22;
				}
			}
		} else if (state == 22) { // Story 11
			int keepLooping = 1;
			int counter = 0;
			int upPressed = 0;
			int downPressed = 0;
			int drawPlayer = 1;
			char text[50];
			char printing[50];
			char *textPointer = &text[0];
			int letters = 0;
			while (keepLooping) {
				CLEAR;

				// STORY TEXT HERE
				if (counter < 50) {
					memset(printing, 0, 50);
				}
				if (counter >= 50 && counter < 200) {
					sprintf(text, "That sure looked difficult.");
					printText(2);
					if (counter == 199) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 200 && counter < 350) {
					sprintf(text, "You seem to be doing fine.");
					printText(2);
					if (counter == 349) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 350 && counter < 500) {
					sprintf(text, "Well, after I healed you that is.");
					printText(2);
					if (counter == 349) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 499) {
					keepLooping = 0;
					state = 23;
				}

				if (counter >= 200 && counter < 250) {
					playerHealth = playerHealth + 1;
					if (playerHealth >= 100) {
						playerHealth = 100;
					}
				}
				drawString4(20, 10, printing, 244);
				drawBasics(&player, &topBar, &middleBar, &bottomBar, &drawPlayer);
				playerMovement(&player, &upPressed, &downPressed, &topBar, &middleBar, &bottomBar);

				if (KEY_DOWN_NOW(BUTTON_SELECT)) {
					keepLooping = 0;
					state = 0;
				}

				// Increments counter of the loop, this is basically the timer
				counter++;
				if (counter > 987654) {
					counter = 0;
				}

				// Displays HP
				char hpDisplay[20];
				sprintf(hpDisplay, "Health: %d", playerHealth);
				drawString4(150, 3, hpDisplay, 250);
				drawRect4(151, 75, 6, playerHealth, 246);
				drawRect4(151, 75 + playerHealth, 6, 100 - playerHealth, 247);

				// MODE 4
				waitForVblank();
				FlipPage();
			}
		} else if (state == 23) { // Stage 11
			int keepLooping = 1;
			int counter = 0;
			int upPressed = 0;
			int downPressed = 0;
			int playerInvincibility = 0;
			int cooldown = 0;
			int drawPlayer = 1;
			int generation = 1;
			int generate = 0;
			int generateGap = 100;
			// Create SPIDER enemies for this stage
			SPIDER spiders[30];
			int nextEmptySpider = 0;
			for (int i = 0; i < 30; i++) { // Initialize them
				spiders[i].arrayPosition = 987654;
			}

			while (keepLooping) {
				CLEAR;
				if (counter == generate && generation == 1) {
					SPIDER temp;
					createSpider(&temp, 1, &nextEmptySpider, 0, 2, 0, 45, 0, 250);
					spiders[nextEmptySpider] = temp;
					nextEmptySpider++;
					if (nextEmptySpider >= 30) {
						nextEmptySpider = 0;
					}
					createSpider(&temp, 1, &nextEmptySpider, 0, 2, 0, 75, 0, 250);
					spiders[nextEmptySpider] = temp;
					nextEmptySpider++;
					if (nextEmptySpider >= 30) {
						nextEmptySpider = 0;
					}
					createSpider(&temp, 1, &nextEmptySpider, 1, 2, 230, 105, 0, 250);
					spiders[nextEmptySpider] = temp;
					nextEmptySpider++;
					if (nextEmptySpider >= 30) {
						nextEmptySpider = 0;
					}
					generate = generate + generateGap;
					if (generateGap > 30) {
						generateGap = generateGap - 3;
					}
				}

				if (playerHealth <= 0) {
					keepLooping = 0;
					state = 999;
				}

				// Drawing

				// If player is invulnerable, flash the player to give player notice
				if (playerInvincibility > 0) {
					if (drawPlayer == 1) {
						drawPlayer = 0;
					} else {
						drawPlayer = 1;
					}
				} else {
					drawPlayer = 1;
				}

				// Drawing the objects
				drawBasics(&player, &topBar, &middleBar, &bottomBar, &drawPlayer);
				updateSpiders(spiders, &playerHealth, &player, &playerInvincibility);
				playerMovement(&player, &upPressed, &downPressed, &topBar, &middleBar, &bottomBar);

				if (KEY_DOWN_NOW(BUTTON_SELECT)) {
					keepLooping = 0;
					state = 0;
				}

				// Cooldown to prevent walls of spiders from appearing
				if (cooldown != 0) {
					cooldown--;
				}

				// If player gets hit, they get invincibility frames that last 100 loops. This decreases the remaining frames.
				if (playerInvincibility != 0) {
					playerInvincibility--;
				}

				// Displays HP
				char hpDisplay[20];
				sprintf(hpDisplay, "Health: %d", playerHealth);
				drawString4(150, 3, hpDisplay, 250);
				drawRect4(151, 75, 6, playerHealth, 246);
				drawRect4(151, 75 + playerHealth, 6, 100 - playerHealth, 247);

				// MODE 4
				waitForVblank();
				FlipPage();

				counter++;
				if (counter > 1400 && counter < 1500) {
					generation = 0;
				} else if (counter > 1500) {
					keepLooping = 0;
					state = 24;
				}
			}
		} else if (state == 24) { // Story 12
			int keepLooping = 1;
			int counter = 0;
			int upPressed = 0;
			int downPressed = 0;
			int drawPlayer = 1;
			char text[50];
			char printing[50];
			char *textPointer = &text[0];
			int letters = 0;
			while (keepLooping) {
				CLEAR;

				// STORY TEXT HERE
				if (counter < 50) {
					memset(printing, 0, 50);
				}
				if (counter >= 50 && counter < 200) {
					sprintf(text, "Wasn't that fun?");
					printText(2);
					if (counter == 199) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 200 && counter < 350) {
					sprintf(text, "Let's keep going.");
					printText(2);
					if (counter == 349) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 350 && counter < 500) {
					sprintf(text, "This is just a little bit");
					printText(2);
					if (counter == 499) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 500 && counter < 650) {
					sprintf(text, "more fun than I had imagined.");
					printText(2);
					if (counter == 649) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				}else if (counter >= 650) {
					keepLooping = 0;
					state = 25;
				}

				drawString4(20, 10, printing, 244);
				drawBasics(&player, &topBar, &middleBar, &bottomBar, &drawPlayer);
				playerMovement(&player, &upPressed, &downPressed, &topBar, &middleBar, &bottomBar);

				if (KEY_DOWN_NOW(BUTTON_SELECT)) {
					keepLooping = 0;
					state = 0;
				}

				// Increments counter of the loop, this is basically the timer
				counter++;
				if (counter > 987654) {
					counter = 0;
				}

				// Displays HP
				char hpDisplay[20];
				sprintf(hpDisplay, "Health: %d", playerHealth);
				drawString4(150, 3, hpDisplay, 250);
				drawRect4(151, 75, 6, playerHealth, 246);
				drawRect4(151, 75 + playerHealth, 6, 100 - playerHealth, 247);

				// MODE 4
				waitForVblank();
				FlipPage();
			}
		} else if (state == 25) { // Stage 12
			int keepLooping = 1;
			int counter = 0;
			int upPressed = 0;
			int downPressed = 0;
			int playerInvincibility = 0;
			int cooldown = 0;
			int drawPlayer = 1;
			int generation = 1;
			int generate = 0;
			int generateGap = 45;
			int patternCount = 0;
			int pattern = 0;
			int randomness = 0;
			// Create SPIDER enemies for this stage
			SPIDER spiders[30];
			int nextEmptySpider = 0;
			for (int i = 0; i < 30; i++) { // Initialize them
				spiders[i].arrayPosition = 987654;
			}

			while (keepLooping) {
				CLEAR;
				if (counter == generate && generation == 1) {
					if (pattern == 0) {
						SPIDER temp;
						createSpider(&temp, 1, &nextEmptySpider, 0, 2, 0, 45, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 0, 2, 0, 75, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 1, 2, 230, 105, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
					} else if (pattern == 1) {
						SPIDER temp;
						createSpider(&temp, 1, &nextEmptySpider, 0, 2, 0, 45, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 0, 2, 0, 105, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 1, 2, 230, 75, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
					} else {
						SPIDER temp;
						createSpider(&temp, 1, &nextEmptySpider, 0, 2, 0, 75, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 0, 2, 0, 105, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 1, 2, 230, 45, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
					}
					generate = generate + generateGap;
					patternCount++;
					if (patternCount == 3 + randomness) {
						pattern = rand() % 3;
						randomness = rand() % 2;
						patternCount = 0;
					}

				}

				if (playerHealth <= 0) {
					keepLooping = 0;
					state = 999;
				}

				// Drawing

				// If player is invulnerable, flash the player to give player notice
				if (playerInvincibility > 0) {
					if (drawPlayer == 1) {
						drawPlayer = 0;
					} else {
						drawPlayer = 1;
					}
				} else {
					drawPlayer = 1;
				}

				// Drawing the objects
				drawBasics(&player, &topBar, &middleBar, &bottomBar, &drawPlayer);
				updateSpiders(spiders, &playerHealth, &player, &playerInvincibility);
				playerMovement(&player, &upPressed, &downPressed, &topBar, &middleBar, &bottomBar);

				if (KEY_DOWN_NOW(BUTTON_SELECT)) {
					keepLooping = 0;
					state = 0;
				}

				// Cooldown to prevent walls of spiders from appearing
				if (cooldown != 0) {
					cooldown--;
				}

				// If player gets hit, they get invincibility frames that last 100 loops. This decreases the remaining frames.
				if (playerInvincibility != 0) {
					playerInvincibility--;
				}

				// Displays HP
				char hpDisplay[20];
				sprintf(hpDisplay, "Health: %d", playerHealth);
				drawString4(150, 3, hpDisplay, 250);
				drawRect4(151, 75, 6, playerHealth, 246);
				drawRect4(151, 75 + playerHealth, 6, 100 - playerHealth, 247);

				// MODE 4
				waitForVblank();
				FlipPage();

				counter++;
				if (counter > 1400 && counter < 1500) {
					generation = 0;
				} else if (counter > 1500) {
					keepLooping = 0;
					state = 26;
				}
			}
		} else if (state == 26) { // Story 13
			int keepLooping = 1;
			int counter = 0;
			int upPressed = 0;
			int downPressed = 0;
			int drawPlayer = 1;
			char text[50];
			char printing[50];
			char *textPointer = &text[0];
			int letters = 0;
			while (keepLooping) {
				CLEAR;

				// STORY TEXT HERE
				if (counter < 50) {
					memset(printing, 0, 50);
				}
				if (counter >= 50 && counter < 200) {
					sprintf(text, "You sure are determined.");
					printText(2);
					if (counter == 199) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 200 && counter < 350) {
					sprintf(text, "Maybe spiders aren't enough.");
					printText(2);
					if (counter == 349) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 350 && counter < 500) {
					sprintf(text, "Try proving me wrong.");
					printText(2);
					if (counter == 499) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 500 && counter < 650) {
					sprintf(text, "Try surviving this.");
					printText(2);
					if (counter == 649) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 650) {
					keepLooping = 0;
					state = 27;
				}

				drawString4(20, 10, printing, 244);
				drawBasics(&player, &topBar, &middleBar, &bottomBar, &drawPlayer);
				playerMovement(&player, &upPressed, &downPressed, &topBar, &middleBar, &bottomBar);

				if (KEY_DOWN_NOW(BUTTON_SELECT)) {
					keepLooping = 0;
					state = 0;
				}

				// Increments counter of the loop, this is basically the timer
				counter++;
				if (counter > 987654) {
					counter = 0;
				}

				// Displays HP
				char hpDisplay[20];
				sprintf(hpDisplay, "Health: %d", playerHealth);
				drawString4(150, 3, hpDisplay, 250);
				drawRect4(151, 75, 6, playerHealth, 246);
				drawRect4(151, 75 + playerHealth, 6, 100 - playerHealth, 247);

				// MODE 4
				waitForVblank();
				FlipPage();
			}
		} else if (state == 27) { // Stage 13
			int keepLooping = 1;
			int counter = 0;
			int upPressed = 0;
			int downPressed = 0;
			int playerInvincibility = 0;
			int cooldown = 0;
			int drawPlayer = 1;
			int generation = 1;
			int generate = 0;
			int generateGap = 75;
			int patternCount = 0;
			int pattern = 0;
			int randomness = 0;
			// Create SPIDER enemies for this stage
			SPIDER spiders[30];
			int nextEmptySpider = 0;
			for (int i = 0; i < 30; i++) { // Initialize them
				spiders[i].arrayPosition = 987654;
			}

			while (keepLooping) {
				CLEAR;

				if (counter % (10 +  (rand() % 35)) == 0 && cooldown == 0 && generation == 1) {
					SPIDER temp;
					createSpider(&temp, 1, &nextEmptySpider, -1, 2, -1, -1, 0, 244);
					spiders[nextEmptySpider] = temp;
					nextEmptySpider++;
					if (nextEmptySpider >= 30) {
						nextEmptySpider = 0;
					}
					cooldown = 25;
				}

				if (counter == generate && generation == 1) {
					if (pattern == 0) {
						SPIDER temp;
						createSpider(&temp, 1, &nextEmptySpider, 0, 3, 0, 45, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 0, 3, 0, 75, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 1, 3, 230, 105, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
					} else if (pattern == 1) {
						SPIDER temp;
						createSpider(&temp, 1, &nextEmptySpider, 0, 3, 0, 45, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 0, 3, 0, 105, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 1, 3, 230, 75, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
					} else {
						SPIDER temp;
						createSpider(&temp, 1, &nextEmptySpider, 0, 3, 0, 75, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 0, 3, 0, 105, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 1, 3, 230, 45, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
					}
					generate = generate + generateGap;
					patternCount++;
					if (patternCount == 3 + randomness) {
						pattern = rand() % 3;
						randomness = rand() % 2;
						patternCount = 0;
					}

				}

				if (playerHealth <= 0) {
					keepLooping = 0;
					state = 999;
				}


				// Drawing

				// If player is invulnerable, flash the player to give player notice
				if (playerInvincibility > 0) {
					if (drawPlayer == 1) {
						drawPlayer = 0;
					} else {
						drawPlayer = 1;
					}
				} else {
					drawPlayer = 1;
				}

				// Drawing the objects
				drawBasics(&player, &topBar, &middleBar, &bottomBar, &drawPlayer);
				updateSpiders(spiders, &playerHealth, &player, &playerInvincibility);
				playerMovement(&player, &upPressed, &downPressed, &topBar, &middleBar, &bottomBar);

				if (KEY_DOWN_NOW(BUTTON_SELECT)) {
					keepLooping = 0;
					state = 0;
				}

				// Cooldown to prevent walls of spiders from appearing
				if (cooldown != 0) {
					cooldown--;
				}

				// If player gets hit, they get invincibility frames that last 100 loops. This decreases the remaining frames.
				if (playerInvincibility != 0) {
					playerInvincibility--;
				}

				// Displays HP
				char hpDisplay[20];
				sprintf(hpDisplay, "Health: %d", playerHealth);
				drawString4(150, 3, hpDisplay, 250);
				drawRect4(151, 75, 6, playerHealth, 246);
				drawRect4(151, 75 + playerHealth, 6, 100 - playerHealth, 247);

				// MODE 4
				waitForVblank();
				FlipPage();

				counter++;
				if (counter > 1200 && counter < 1300) {
					generation = 0;
				} else if (counter > 1300) {
					keepLooping = 0;
					state = 28;
				}
			}
		} else if (state == 28) { // Story 14
			int keepLooping = 1;
			int counter = 0;
			int upPressed = 0;
			int downPressed = 0;
			int drawPlayer = 1;
			char text[50];
			char printing[50];
			char *textPointer = &text[0];
			int letters = 0;
			while (keepLooping) {
				CLEAR;

				// STORY TEXT HERE
				if (counter < 50) {
					memset(printing, 0, 50);
				}
				if (counter >= 50 && counter < 200) {
					sprintf(text, "Well, time for more pain.");
					printText(2);
					if (counter == 199) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 200 && counter < 350) {
					sprintf(text, "I was hoping you'd have been dead.");
					printText(2);
					if (counter == 349) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 350 && counter < 500) {
					sprintf(text, "So you wouldn't have to...");
					printText(2);
					if (counter == 499) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 500 && counter < 650) {
					sprintf(text, "...see what's to come.");
					printText(2);
					if (counter == 649) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 650) {
					keepLooping = 0;
					state = 29;
				}

				if (counter >= 0 && counter < 50) {
					playerHealth++;
					if (playerHealth > 100) {
						playerHealth = 100;
					}
				}
				drawString4(20, 10, printing, 244);
				drawBasics(&player, &topBar, &middleBar, &bottomBar, &drawPlayer);
				playerMovement(&player, &upPressed, &downPressed, &topBar, &middleBar, &bottomBar);

				if (KEY_DOWN_NOW(BUTTON_SELECT)) {
					keepLooping = 0;
					state = 0;
				}

				// Increments counter of the loop, this is basically the timer
				counter++;
				if (counter > 987654) {
					counter = 0;
				}

				// Displays HP
				char hpDisplay[20];
				sprintf(hpDisplay, "Health: %d", playerHealth);
				drawString4(150, 3, hpDisplay, 250);
				drawRect4(151, 75, 6, playerHealth, 246);
				drawRect4(151, 75 + playerHealth, 6, 100 - playerHealth, 247);

				// MODE 4
				waitForVblank();
				FlipPage();
			}
		} else if (state == 29) { // Stage 14
			int keepLooping = 1;
			int counter = 0;
			int upPressed = 0;
			int downPressed = 0;
			int playerInvincibility = 0;
			int spiderCooldown = 0;
			int bulletCooldown = 0;
			int drawPlayer = 1;
			int generation = 1;
			// Create SPIDER enemies for this stage
			SPIDER spiders[30];
			int nextEmptySpider = 0;
			for (int i = 0; i < 30; i++) { // Initialize them
				spiders[i].arrayPosition = 987654;
			}
			// Create BULLET enemies for this stage
			BULLET bullets[30];
			int nextEmptyBullet = 0;
			for (int i = 0; i < 30; i++) {
				bullets[i].arrayPosition = 987654;
			}

			while (keepLooping) {
				CLEAR;

				if (counter % (10 +  (rand() % 35)) == 0 && spiderCooldown == 0 && generation == 1) {
					SPIDER temp;
					createSpider(&temp, 1, &nextEmptySpider, -1, 2, -1, -1, 0, 244);
					spiders[nextEmptySpider] = temp;
					nextEmptySpider++;
					if (nextEmptySpider >= 30) {
						nextEmptySpider = 0;
					}
					spiderCooldown = 40;
				}

				if (counter % (10 +  (rand() % 25)) == 0 && bulletCooldown == 0 && generation == 1) {
					BULLET temp;
					createBullet(&temp, 1, 0, 0, 0, 0, 0, 0, 0, &player);
					bullets[nextEmptyBullet] = temp;
					nextEmptyBullet++;
					if (nextEmptyBullet >= 30) {
						nextEmptyBullet = 0;
					}
					bulletCooldown = 20;
				}

				if (playerHealth <= 0) {
					keepLooping = 0;
					state = 999;
				}

				// Drawing

				// If player is invulnerable, flash the player to give player notice
				if (playerInvincibility > 0) {
					if (drawPlayer == 1) {
						drawPlayer = 0;
					} else {
						drawPlayer = 1;
					}
				} else {
					drawPlayer = 1;
				}

				// Drawing the objects
				drawBasics(&player, &topBar, &middleBar, &bottomBar, &drawPlayer);
				updateSpiders(spiders, &playerHealth, &player, &playerInvincibility);
				updateBullets(bullets, &playerHealth, &player, &playerInvincibility);
				playerMovement(&player, &upPressed, &downPressed, &topBar, &middleBar, &bottomBar);

				if (KEY_DOWN_NOW(BUTTON_SELECT)) {
					keepLooping = 0;
					state = 0;
				}

				// Cooldown to prevent walls of spiders from appearing
				if (spiderCooldown != 0) {
					spiderCooldown--;
				}

				if (bulletCooldown != 0) {
					bulletCooldown--;
				}

				// If player gets hit, they get invincibility frames that last 100 loops. This decreases the remaining frames.
				if (playerInvincibility != 0) {
					playerInvincibility--;
				}

				// Displays HP
				char hpDisplay[20];
				sprintf(hpDisplay, "Health: %d", playerHealth);
				drawString4(150, 3, hpDisplay, 250);
				drawRect4(151, 75, 6, playerHealth, 246);
				drawRect4(151, 75 + playerHealth, 6, 100 - playerHealth, 247);

				// MODE 4
				waitForVblank();
				FlipPage();

				counter++;
				if (counter > 1000 && counter < 1100) {
					generation = 0;
				} else if (counter > 1100) {
					keepLooping = 0;
					state = 30;
				}
			}
		} else if (state == 30) { // Story 15
			int keepLooping = 1;
			int counter = 0;
			int upPressed = 0;
			int downPressed = 0;
			int drawPlayer = 1;
			char text[50];
			char printing[50];
			char *textPointer = &text[0];
			int letters = 0;
			while (keepLooping) {
				CLEAR;

				// STORY TEXT HERE
				if (counter < 50) {
					memset(printing, 0, 50);
				}
				if (counter >= 50 && counter < 200) {
					sprintf(text, "So, how was that?");
					printText(2);
					if (counter == 199) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 200 && counter < 350) {
					sprintf(text, "Who said things have to stay");
					printText(2);
					if (counter == 349) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 350 && counter < 500) {
					sprintf(text, "on the line?");
					printText(2);
					if (counter == 499) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 500 && counter < 650) {
					sprintf(text, "I hope you weren't surprised.");
					printText(2);
					if (counter == 649) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 650 && counter < 800) {
					sprintf(text, "Since this is only the beginning.");
					printText(2);
					if (counter == 799) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 800) {
					keepLooping = 0;
					state = 31;
				}

				drawString4(20, 10, printing, 244);
				drawBasics(&player, &topBar, &middleBar, &bottomBar, &drawPlayer);
				playerMovement(&player, &upPressed, &downPressed, &topBar, &middleBar, &bottomBar);

				if (KEY_DOWN_NOW(BUTTON_SELECT)) {
					keepLooping = 0;
					state = 0;
				}

				// Increments counter of the loop, this is basically the timer
				counter++;
				if (counter > 987654) {
					counter = 0;
				}

				// Displays HP
				char hpDisplay[20];
				sprintf(hpDisplay, "Health: %d", playerHealth);
				drawString4(150, 3, hpDisplay, 250);
				drawRect4(151, 75, 6, playerHealth, 246);
				drawRect4(151, 75 + playerHealth, 6, 100 - playerHealth, 247);

				// MODE 4
				waitForVblank();
				FlipPage();
			}
		} else if (state == 31) { // Stage 15
			int keepLooping = 1;
			int counter = 0;
			int upPressed = 0;
			int downPressed = 0;
			int playerInvincibility = 0;
			int spiderCooldown = 0;
			int bulletCooldown = 0;
			int drawPlayer = 1;
			int generation = 1;
			// Create SPIDER enemies for this stage
			SPIDER spiders[30];
			int nextEmptySpider = 0;
			for (int i = 0; i < 30; i++) { // Initialize them
				spiders[i].arrayPosition = 987654;
			}
			// Create BULLET enemies for this stage
			BULLET bullets[30];
			int nextEmptyBullet = 0;
			for (int i = 0; i < 30; i++) {
				bullets[i].arrayPosition = 987654;
			}

			while (keepLooping) {
				CLEAR;

				if (counter % (5 +  (rand() % 20)) == 0 && spiderCooldown == 0 && generation == 1) {
					SPIDER temp;
					createSpider(&temp, 1, &nextEmptySpider, -1, 3, -1, -1, 0, 244);
					spiders[nextEmptySpider] = temp;
					nextEmptySpider++;
					if (nextEmptySpider >= 30) {
						nextEmptySpider = 0;
					}
					spiderCooldown = 20;
				}

				if (counter % (10 +  (rand() % 25)) == 0 && bulletCooldown == 0 && generation == 1) {
					BULLET temp;
					createBullet(&temp, 1, 0, 0, 0, 0, 0, 0, 0, &player);
					bullets[nextEmptyBullet] = temp;
					nextEmptyBullet++;
					if (nextEmptyBullet >= 30) {
						nextEmptyBullet = 0;
					}
					bulletCooldown = 10;
				}

				if (playerHealth <= 0) {
					keepLooping = 0;
					state = 999;
				}

				// Drawing

				// If player is invulnerable, flash the player to give player notice
				if (playerInvincibility > 0) {
					if (drawPlayer == 1) {
						drawPlayer = 0;
					} else {
						drawPlayer = 1;
					}
				} else {
					drawPlayer = 1;
				}

				// Drawing the objects
				drawBasics(&player, &topBar, &middleBar, &bottomBar, &drawPlayer);
				updateSpiders(spiders, &playerHealth, &player, &playerInvincibility);
				updateBullets(bullets, &playerHealth, &player, &playerInvincibility);
				playerMovement(&player, &upPressed, &downPressed, &topBar, &middleBar, &bottomBar);

				if (KEY_DOWN_NOW(BUTTON_SELECT)) {
					keepLooping = 0;
					state = 0;
				}

				// Cooldown to prevent walls of spiders from appearing
				if (spiderCooldown != 0) {
					spiderCooldown--;
				}

				if (bulletCooldown != 0) {
					bulletCooldown--;
				}

				// If player gets hit, they get invincibility frames that last 100 loops. This decreases the remaining frames.
				if (playerInvincibility != 0) {
					playerInvincibility--;
				}

				// Displays HP
				char hpDisplay[20];
				sprintf(hpDisplay, "Health: %d", playerHealth);
				drawString4(150, 3, hpDisplay, 250);
				drawRect4(151, 75, 6, playerHealth, 246);
				drawRect4(151, 75 + playerHealth, 6, 100 - playerHealth, 247);

				// MODE 4
				waitForVblank();
				FlipPage();

				counter++;
				if (counter > 1000 && counter < 1100) {
					generation = 0;
				} else if (counter > 1100) {
					keepLooping = 0;
					state = 32;
				}
			}
		} else if (state == 32) { // Story 16
			int keepLooping = 1;
			int counter = 0;
			int upPressed = 0;
			int downPressed = 0;
			int drawPlayer = 1;
			char text[50];
			char printing[50];
			char *textPointer = &text[0];
			int letters = 0;
			while (keepLooping) {
				CLEAR;

				// STORY TEXT HERE
				if (counter < 50) {
					memset(printing, 0, 50);
				}
				if (counter >= 50 && counter < 200) {
					sprintf(text, "Why don't you give up already?");
					printText(2);
					if (counter == 199) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 200 && counter < 350) {
					sprintf(text, "Isn't this getting boring?");
					printText(2);
					if (counter == 349) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 350 && counter < 500) {
					sprintf(text, "Why do you live?");
					printText(2);
					if (counter == 499) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 500 && counter < 650) {
					sprintf(text, "Why do you struggle?");
					printText(2);
					if (counter == 649) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 650 && counter < 800) {
					sprintf(text, "Why don't you just die?");
					printText(2);
					if (counter == 799) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 800) {
					keepLooping = 0;
					state = 33;
				}

				drawString4(20, 10, printing, 244);
				drawBasics(&player, &topBar, &middleBar, &bottomBar, &drawPlayer);
				playerMovement(&player, &upPressed, &downPressed, &topBar, &middleBar, &bottomBar);

				if (KEY_DOWN_NOW(BUTTON_SELECT)) {
					keepLooping = 0;
					state = 0;
				}

				// Increments counter of the loop, this is basically the timer
				counter++;
				if (counter > 987654) {
					counter = 0;
				}

				// Displays HP
				char hpDisplay[20];
				sprintf(hpDisplay, "Health: %d", playerHealth);
				drawString4(150, 3, hpDisplay, 250);
				drawRect4(151, 75, 6, playerHealth, 246);
				drawRect4(151, 75 + playerHealth, 6, 100 - playerHealth, 247);

				// MODE 4
				waitForVblank();
				FlipPage();
			}
		} else if (state == 33) { // Stage 16
			int keepLooping = 1;
			int counter = 0;
			int upPressed = 0;
			int downPressed = 0;
			int playerInvincibility = 0;
			int spiderCooldown = 0;
			int bulletCooldown = 0;
			int drawPlayer = 1;
			int generation = 1;
			// Create SPIDER enemies for this stage
			SPIDER spiders[30];
			int nextEmptySpider = 0;
			for (int i = 0; i < 30; i++) { // Initialize them
				spiders[i].arrayPosition = 987654;
			}
			// Create BULLET enemies for this stage
			BULLET bullets[30];
			int nextEmptyBullet = 0;
			for (int i = 0; i < 30; i++) {
				bullets[i].arrayPosition = 987654;
			}

			while (keepLooping) {
				CLEAR;

				if (counter % (5 +  (rand() % 20)) == 0 && spiderCooldown == 0 && generation == 1) {
					SPIDER temp;
					createSpider(&temp, 1, &nextEmptySpider, -1, 3, -1, -1, 0, 244);
					spiders[nextEmptySpider] = temp;
					nextEmptySpider++;
					if (nextEmptySpider >= 30) {
						nextEmptySpider = 0;
					}
					spiderCooldown = 20;
				}

				if (counter % (5 +  (rand() % 15)) == 0 && bulletCooldown == 0 && generation == 1) {
					BULLET temp;
					createBullet(&temp, 0, 0, 0, 0, 0, 0, 0, 0, &player);
					bullets[nextEmptyBullet] = temp;
					nextEmptyBullet++;
					if (nextEmptyBullet >= 30) {
						nextEmptyBullet = 0;
					}
					bulletCooldown = 5;
				}

				if (playerHealth <= 0) {
					keepLooping = 0;
					state = 999;
				}

				// Drawing

				// If player is invulnerable, flash the player to give player notice
				if (playerInvincibility > 0) {
					if (drawPlayer == 1) {
						drawPlayer = 0;
					} else {
						drawPlayer = 1;
					}
				} else {
					drawPlayer = 1;
				}

				// Drawing the objects
				drawBasics(&player, &topBar, &middleBar, &bottomBar, &drawPlayer);
				updateSpiders(spiders, &playerHealth, &player, &playerInvincibility);
				updateBullets(bullets, &playerHealth, &player, &playerInvincibility);
				playerMovement(&player, &upPressed, &downPressed, &topBar, &middleBar, &bottomBar);

				if (KEY_DOWN_NOW(BUTTON_SELECT)) {
					keepLooping = 0;
					state = 0;
				}

				// Cooldown to prevent walls of spiders from appearing
				if (spiderCooldown != 0) {
					spiderCooldown--;
				}

				if (bulletCooldown != 0) {
					bulletCooldown--;
				}

				// If player gets hit, they get invincibility frames that last 100 loops. This decreases the remaining frames.
				if (playerInvincibility != 0) {
					playerInvincibility--;
				}

				// Displays HP
				char hpDisplay[20];
				sprintf(hpDisplay, "Health: %d", playerHealth);
				drawString4(150, 3, hpDisplay, 250);
				drawRect4(151, 75, 6, playerHealth, 246);
				drawRect4(151, 75 + playerHealth, 6, 100 - playerHealth, 247);

				// MODE 4
				waitForVblank();
				FlipPage();

				counter++;
				if (counter > 1000 && counter < 1100) {
					generation = 0;
				} else if (counter > 1100) {
					keepLooping = 0;
					state = 34;
				}
			}
		} else if (state == 34) { // Story 17
			int keepLooping = 1;
			int counter = 0;
			int upPressed = 0;
			int downPressed = 0;
			int drawPlayer = 1;
			char text[50];
			char printing[50];
			char *textPointer = &text[0];
			int letters = 0;
			while (keepLooping) {
				CLEAR;

				// STORY TEXT HERE
				if (counter < 50) {
					memset(printing, 0, 50);
				}
				if (counter >= 50 && counter < 200) {
					sprintf(text, "At this point...");
					printText(2);
					if (counter == 199) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 200 && counter < 350) {
					sprintf(text, "You should really have realized...");
					printText(2);
					if (counter == 349) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 350 && counter < 500) {
					sprintf(text, "That I can do anything.");
					printText(2);
					if (counter == 499) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 500 && counter < 650) {
					sprintf(text, "Absolutely.");
					printText(2);
					if (counter == 649) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 650 && counter < 800) {
					sprintf(text, "Anything.");
					printText(2);
					if (counter == 799) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 800) {
					keepLooping = 0;
					state = 35;
				}

				if (counter >= 0 && counter < 50) {
					playerHealth++;
					if (playerHealth > 100) {
						playerHealth = 100;
					}
				}
				drawString4(20, 10, printing, 244);
				drawBasics(&player, &topBar, &middleBar, &bottomBar, &drawPlayer);
				playerMovement(&player, &upPressed, &downPressed, &topBar, &middleBar, &bottomBar);

				if (KEY_DOWN_NOW(BUTTON_SELECT)) {
					keepLooping = 0;
					state = 0;
				}

				// Increments counter of the loop, this is basically the timer
				counter++;
				if (counter > 987654) {
					counter = 0;
				}

				// Displays HP
				char hpDisplay[20];
				sprintf(hpDisplay, "Health: %d", playerHealth);
				drawString4(150, 3, hpDisplay, 250);
				drawRect4(151, 75, 6, playerHealth, 246);
				drawRect4(151, 75 + playerHealth, 6, 100 - playerHealth, 247);

				// MODE 4
				waitForVblank();
				FlipPage();
			}
		} else if (state == 35) { // Stage 17
			int keepLooping = 1;
			int counter = 0;
			int upPressed = 0;
			int downPressed = 0;
			int playerInvincibility = 0;
			int spiderCooldown = 0;
			int bulletCooldown = 0;
			int drawPlayer = 1;
			int generation = 1;
			int generate = 0;
			int generateGap = 60;
			int patternCount = 0;
			int pattern = 0;
			int randomness = 0;
			// Create SPIDER enemies for this stage
			SPIDER spiders[30];
			int nextEmptySpider = 0;
			for (int i = 0; i < 30; i++) { // Initialize them
				spiders[i].arrayPosition = 987654;
			}

			// Create BULLET enemies for this stage
			BULLET bullets[30];
			int nextEmptyBullet = 0;
			for (int i = 0; i < 30; i++) {
				bullets[i].arrayPosition = 987654;
			}

			while (keepLooping) {
				CLEAR;
				if (counter == generate && generation == 1) {
					if (pattern == 0) {
						SPIDER temp;
						createSpider(&temp, 1, &nextEmptySpider, 0, 2, 0, 45, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 0, 2, 0, 75, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 1, 2, 230, 105, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
					} else if (pattern == 1) {
						SPIDER temp;
						createSpider(&temp, 1, &nextEmptySpider, 0, 2, 0, 45, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 0, 2, 0, 105, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 1, 2, 230, 75, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
					} else {
						SPIDER temp;
						createSpider(&temp, 1, &nextEmptySpider, 0, 2, 0, 75, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 0, 2, 0, 105, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 1, 2, 230, 45, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
					}
					generate = generate + generateGap;
					patternCount++;
					if (patternCount == 3 + randomness) {
						pattern = rand() % 3;
						randomness = rand() % 2;
						patternCount = 0;
					}

				}

				if (counter % (10 +  (rand() % 15)) == 0 && bulletCooldown == 0 && generation == 1) {
					BULLET temp;
					createBullet(&temp, 0, 0, 0, 0, 0, 0, 0, 0, &player);
					bullets[nextEmptyBullet] = temp;
					nextEmptyBullet++;
					if (nextEmptyBullet >= 30) {
						nextEmptyBullet = 0;
					}
					bulletCooldown = 10;
				}

				if (playerHealth <= 0) {
					keepLooping = 0;
					state = 999;
				}

				// Drawing

				// If player is invulnerable, flash the player to give player notice
				if (playerInvincibility > 0) {
					if (drawPlayer == 1) {
						drawPlayer = 0;
					} else {
						drawPlayer = 1;
					}
				} else {
					drawPlayer = 1;
				}

				// Drawing the objects
				drawBasics(&player, &topBar, &middleBar, &bottomBar, &drawPlayer);
				updateSpiders(spiders, &playerHealth, &player, &playerInvincibility);
				updateBullets(bullets, &playerHealth, &player, &playerInvincibility);
				playerMovement(&player, &upPressed, &downPressed, &topBar, &middleBar, &bottomBar);

				if (KEY_DOWN_NOW(BUTTON_SELECT)) {
					keepLooping = 0;
					state = 0;
				}

				// Cooldown to prevent walls of spiders from appearing
				if (spiderCooldown != 0) {
					spiderCooldown--;
				}

				if (bulletCooldown != 0) {
					bulletCooldown--;
				}

				// If player gets hit, they get invincibility frames that last 100 loops. This decreases the remaining frames.
				if (playerInvincibility != 0) {
					playerInvincibility--;
				}

				// Displays HP
				char hpDisplay[20];
				sprintf(hpDisplay, "Health: %d", playerHealth);
				drawString4(150, 3, hpDisplay, 250);
				drawRect4(151, 75, 6, playerHealth, 246);
				drawRect4(151, 75 + playerHealth, 6, 100 - playerHealth, 247);

				// MODE 4
				waitForVblank();
				FlipPage();

				counter++;
				if (counter > 1400 && counter < 1550) {
					generation = 0;
				} else if (counter > 1550) {
					keepLooping = 0;
					state = 36;
				}
			}
		} else if (state == 36) { // Story 18
			int keepLooping = 1;
			int counter = 0;
			int upPressed = 0;
			int downPressed = 0;
			int drawPlayer = 1;
			char text[50];
			char printing[50];
			char *textPointer = &text[0];
			int letters = 0;
			while (keepLooping) {
				CLEAR;

				// STORY TEXT HERE
				if (counter < 50) {
					memset(printing, 0, 50);
				}
				if (counter >= 50 && counter < 200) {
					sprintf(text, "I could just kill you now.");
					printText(2);
					if (counter == 199) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 200) {
					keepLooping = 0;
					state = 37;
				}

				drawString4(20, 10, printing, 244);
				drawBasics(&player, &topBar, &middleBar, &bottomBar, &drawPlayer);
				playerMovement(&player, &upPressed, &downPressed, &topBar, &middleBar, &bottomBar);

				if (KEY_DOWN_NOW(BUTTON_SELECT)) {
					keepLooping = 0;
					state = 0;
				}

				// Increments counter of the loop, this is basically the timer
				counter++;
				if (counter > 987654) {
					counter = 0;
				}

				// Displays HP
				char hpDisplay[20];
				sprintf(hpDisplay, "Health: %d", playerHealth);
				drawString4(150, 3, hpDisplay, 250);
				drawRect4(151, 75, 6, playerHealth, 246);
				drawRect4(151, 75 + playerHealth, 6, 100 - playerHealth, 247);

				// MODE 4
				waitForVblank();
				FlipPage();
			}
		} else if (state == 37) { // Stage 18
			int keepLooping = 1;
			int counter = 0;
			int upPressed = 0;
			int downPressed = 0;
			int playerInvincibility = 0;
			int spiderCooldown = 0;
			int bulletCooldown = 0;
			int drawPlayer = 1;
			int generation = 1;
			int generate = 0;
			int generateGap = 40;
			int patternCount = 0;
			int pattern = 0;
			int randomness = 0;
			// Create SPIDER enemies for this stage
			SPIDER spiders[30];
			int nextEmptySpider = 0;
			for (int i = 0; i < 30; i++) { // Initialize them
				spiders[i].arrayPosition = 987654;
			}

			// Create BULLET enemies for this stage
			BULLET bullets[30];
			int nextEmptyBullet = 0;
			for (int i = 0; i < 30; i++) {
				bullets[i].arrayPosition = 987654;
			}

			while (keepLooping) {
				CLEAR;
				if (counter == generate && generation == 1) {
					if (pattern == 0) {
						SPIDER temp;
						createSpider(&temp, 1, &nextEmptySpider, 0, 2, 0, 45, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 0, 2, 0, 75, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 1, 2, 230, 105, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
					} else if (pattern == 1) {
						SPIDER temp;
						createSpider(&temp, 1, &nextEmptySpider, 0, 2, 0, 45, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 0, 2, 0, 105, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 1, 2, 230, 75, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
					} else if (pattern == 2) {
						SPIDER temp;
						createSpider(&temp, 1, &nextEmptySpider, 0, 2, 0, 75, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 0, 2, 0, 105, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 1, 2, 230, 45, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
					} else if (pattern == 3) {
						SPIDER temp;
						createSpider(&temp, 1, &nextEmptySpider, 1, 2, 230, 45, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 1, 2, 230, 75, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 0, 2, 0, 105, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
					} else if (pattern == 4) {
						SPIDER temp;
						createSpider(&temp, 1, &nextEmptySpider, 1, 2, 230, 45, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 1, 2, 230, 105, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 0, 2, 0, 75, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
					} else if (pattern == 5) {
						SPIDER temp;
						createSpider(&temp, 1, &nextEmptySpider, 1, 2, 230, 75, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 1, 2, 230, 105, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 0, 2, 0, 45, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
					}
					generate = generate + generateGap;
					patternCount++;
					if (patternCount == 3 + randomness) {
						pattern = rand() % 6;
						randomness = rand() % 2;
						patternCount = 0;
					}

				}

				if (counter % (999999) == 987654 && bulletCooldown == 0 && generation == 1) {
					BULLET temp;
					createBullet(&temp, 0, 0, 0, 0, 0, 0, 0, 0, &player);
					bullets[nextEmptyBullet] = temp;
					nextEmptyBullet++;
					if (nextEmptyBullet >= 30) {
						nextEmptyBullet = 0;
					}
					bulletCooldown = 50;
				}

				if (playerHealth <= 0) {
					keepLooping = 0;
					state = 999;
				}

				// Drawing

				// If player is invulnerable, flash the player to give player notice
				if (playerInvincibility > 0) {
					if (drawPlayer == 1) {
						drawPlayer = 0;
					} else {
						drawPlayer = 1;
					}
				} else {
					drawPlayer = 1;
				}

				// Drawing the objects
				drawBasics(&player, &topBar, &middleBar, &bottomBar, &drawPlayer);
				updateSpiders(spiders, &playerHealth, &player, &playerInvincibility);
				updateBullets(bullets, &playerHealth, &player, &playerInvincibility);
				playerMovement(&player, &upPressed, &downPressed, &topBar, &middleBar, &bottomBar);

				if (KEY_DOWN_NOW(BUTTON_SELECT)) {
					keepLooping = 0;
					state = 0;
				}

				// Cooldown to prevent walls of spiders from appearing
				if (spiderCooldown != 0) {
					spiderCooldown--;
				}

				if (bulletCooldown != 0) {
					bulletCooldown--;
				}

				// If player gets hit, they get invincibility frames that last 100 loops. This decreases the remaining frames.
				if (playerInvincibility != 0) {
					playerInvincibility--;
				}

				// Displays HP
				char hpDisplay[20];
				sprintf(hpDisplay, "Health: %d", playerHealth);
				drawString4(150, 3, hpDisplay, 250);
				drawRect4(151, 75, 6, playerHealth, 246);
				drawRect4(151, 75 + playerHealth, 6, 100 - playerHealth, 247);

				// MODE 4
				waitForVblank();
				FlipPage();

				counter++;
				if (counter > 1400 && counter < 1550) {
					generation = 0;
				} else if (counter > 1550) {
					keepLooping = 0;
					state = 38;
				}
			}
		} else if (state == 38) { // Story 19
			int keepLooping = 1;
			int counter = 0;
			int upPressed = 0;
			int downPressed = 0;
			int drawPlayer = 1;
			char text[50];
			char printing[50];
			char *textPointer = &text[0];
			int letters = 0;
			while (keepLooping) {
				CLEAR;

				// STORY TEXT HERE
				if (counter < 50) {
					memset(printing, 0, 50);
				}
				if (counter >= 50 && counter < 200) {
					sprintf(text, "But that wouldn't be fun.");
					printText(2);
					if (counter == 199) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 200) {
					keepLooping = 0;
					state = 39;
				}

				drawString4(20, 10, printing, 244);
				drawBasics(&player, &topBar, &middleBar, &bottomBar, &drawPlayer);
				playerMovement(&player, &upPressed, &downPressed, &topBar, &middleBar, &bottomBar);

				if (KEY_DOWN_NOW(BUTTON_SELECT)) {
					keepLooping = 0;
					state = 0;
				}

				// Increments counter of the loop, this is basically the timer
				counter++;
				if (counter > 987654) {
					counter = 0;
				}

				// Displays HP
				char hpDisplay[20];
				sprintf(hpDisplay, "Health: %d", playerHealth);
				drawString4(150, 3, hpDisplay, 250);
				drawRect4(151, 75, 6, playerHealth, 246);
				drawRect4(151, 75 + playerHealth, 6, 100 - playerHealth, 247);

				// MODE 4
				waitForVblank();
				FlipPage();
			}
		} else if (state == 39) { // Stage 19
			int keepLooping = 1;
			int counter = 0;
			int upPressed = 0;
			int downPressed = 0;
			int playerInvincibility = 0;
			int spiderCooldown = 0;
			int bulletCooldown = 0;
			int drawPlayer = 1;
			int generation = 1;
			int generate = 0;
			int generateGap = 40;
			int patternCount = 0;
			int pattern = 0;
			int randomness = 0;
			// Create SPIDER enemies for this stage
			SPIDER spiders[30];
			int nextEmptySpider = 0;
			for (int i = 0; i < 30; i++) { // Initialize them
				spiders[i].arrayPosition = 987654;
			}

			// Create BULLET enemies for this stage
			BULLET bullets[30];
			int nextEmptyBullet = 0;
			for (int i = 0; i < 30; i++) {
				bullets[i].arrayPosition = 987654;
			}

			while (keepLooping) {
				CLEAR;
				if (counter == generate && generation == 1) {
					if (pattern == 0) {
						SPIDER temp;
						createSpider(&temp, 1, &nextEmptySpider, 0, 3, 0, 45, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 0, 3, 0, 75, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 1, 3, 230, 105, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
					} else if (pattern == 1) {
						SPIDER temp;
						createSpider(&temp, 1, &nextEmptySpider, 0, 3, 0, 45, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 0, 3, 0, 105, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 1, 3, 230, 75, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
					} else if (pattern == 2) {
						SPIDER temp;
						createSpider(&temp, 1, &nextEmptySpider, 0, 3, 0, 75, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 0, 3, 0, 105, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 1, 3, 230, 45, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
					} else if (pattern == 3) {
						SPIDER temp;
						createSpider(&temp, 1, &nextEmptySpider, 1, 3, 230, 45, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 1, 3, 230, 75, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 0, 3, 0, 105, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
					} else if (pattern == 4) {
						SPIDER temp;
						createSpider(&temp, 1, &nextEmptySpider, 1, 3, 230, 45, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 1, 3, 230, 105, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 0, 3, 0, 75, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
					} else if (pattern == 5) {
						SPIDER temp;
						createSpider(&temp, 1, &nextEmptySpider, 1, 3, 230, 75, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 1, 3, 230, 105, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 0, 3, 0, 45, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
					}
					generate = generate + generateGap + (rand() % 15);
					patternCount++;
					if (patternCount == 3 + randomness) {
						pattern = rand() % 6;
						randomness = rand() % 2;
						patternCount = 0;
					}

				}

				if (counter % (10 + (rand() % 25)) == 0 && bulletCooldown == 0 && generation == 1) {
					BULLET temp;
					createBullet(&temp, 0, 0, 0, 0, 0, 0, 0, 0, &player);
					bullets[nextEmptyBullet] = temp;
					nextEmptyBullet++;
					if (nextEmptyBullet >= 30) {
						nextEmptyBullet = 0;
					}
					bulletCooldown = 20;
				}

				if (playerHealth <= 0) {
					keepLooping = 0;
					state = 999;
				}

				// Drawing

				// If player is invulnerable, flash the player to give player notice
				if (playerInvincibility > 0) {
					if (drawPlayer == 1) {
						drawPlayer = 0;
					} else {
						drawPlayer = 1;
					}
				} else {
					drawPlayer = 1;
				}

				// Drawing the objects
				drawBasics(&player, &topBar, &middleBar, &bottomBar, &drawPlayer);
				updateSpiders(spiders, &playerHealth, &player, &playerInvincibility);
				updateBullets(bullets, &playerHealth, &player, &playerInvincibility);
				playerMovement(&player, &upPressed, &downPressed, &topBar, &middleBar, &bottomBar);

				if (KEY_DOWN_NOW(BUTTON_SELECT)) {
					keepLooping = 0;
					state = 0;
				}

				// Cooldown to prevent walls of spiders from appearing
				if (spiderCooldown != 0) {
					spiderCooldown--;
				}

				if (bulletCooldown != 0) {
					bulletCooldown--;
				}

				// If player gets hit, they get invincibility frames that last 100 loops. This decreases the remaining frames.
				if (playerInvincibility != 0) {
					playerInvincibility--;
				}

				// Displays HP
				char hpDisplay[20];
				sprintf(hpDisplay, "Health: %d", playerHealth);
				drawString4(150, 3, hpDisplay, 250);
				drawRect4(151, 75, 6, playerHealth, 246);
				drawRect4(151, 75 + playerHealth, 6, 100 - playerHealth, 247);

				// MODE 4
				waitForVblank();
				FlipPage();

				counter++;
				if (counter > 1000 && counter < 1150) {
					generation = 0;
				} else if (counter > 1150) {
					keepLooping = 0;
					state = 40;
				}
			}
		} else if (state == 40) { // Story 20
			int keepLooping = 1;
			int counter = 0;
			int upPressed = 0;
			int downPressed = 0;
			int drawPlayer = 1;
			char text[50];
			char printing[50];
			char *textPointer = &text[0];
			int letters = 0;
			while (keepLooping) {
				CLEAR;

				// STORY TEXT HERE
				if (counter < 50) {
					memset(printing, 0, 50);
				}
				if (counter >= 50 && counter < 200) {
					sprintf(text, "Wow, you're still alive?");
					printText(2);
					if (counter == 199) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 200 && counter < 350) {
					sprintf(text, "I don't want to use my lasers yet.");
					printText(2);
					if (counter == 349) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 350 && counter < 500) {
					sprintf(text, "Oh wait, that's right.");
					printText(2);
					if (counter == 499) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 500) {
					keepLooping = 0;
					state = 41;
				}

				if (counter >= 0 && counter < 50) {
					playerHealth++;
					if (playerHealth > 100) {
						playerHealth = 100;
					}
				}
				drawString4(20, 10, printing, 244);
				drawBasics(&player, &topBar, &middleBar, &bottomBar, &drawPlayer);
				playerMovement(&player, &upPressed, &downPressed, &topBar, &middleBar, &bottomBar);

				if (KEY_DOWN_NOW(BUTTON_SELECT)) {
					keepLooping = 0;
					state = 0;
				}

				// Increments counter of the loop, this is basically the timer
				counter++;
				if (counter > 987654) {
					counter = 0;
				}

				// Displays HP
				char hpDisplay[20];
				sprintf(hpDisplay, "Health: %d", playerHealth);
				drawString4(150, 3, hpDisplay, 250);
				drawRect4(151, 75, 6, playerHealth, 246);
				drawRect4(151, 75 + playerHealth, 6, 100 - playerHealth, 247);

				// MODE 4
				waitForVblank();
				FlipPage();
			}
		} else if (state == 41) { // Stage 20
			int keepLooping = 1;
			int counter = 0;
			int upPressed = 0;
			int downPressed = 0;
			int playerInvincibility = 0;
			int spiderCooldown = 0;
			int bulletCooldown = 0;
			int drawPlayer = 1;
			int generation = 1;
			// Create SPIDER enemies for this stage
			SPIDER spiders[30];
			int nextEmptySpider = 0;
			for (int i = 0; i < 30; i++) { // Initialize them
				spiders[i].arrayPosition = 987654;
			}
			// Create BULLET enemies for this stage
			BULLET bullets[30];
			int nextEmptyBullet = 0;
			for (int i = 0; i < 30; i++) {
				bullets[i].arrayPosition = 987654;
			}

			while (keepLooping) {
				CLEAR;

				if (counter % (5 +  (rand() % 20)) == 0 && spiderCooldown == 0 && generation == 1) {
					SPIDER temp;
					createSpider(&temp, 1, &nextEmptySpider, -1, 3, -1, -1, 0, 244);
					spiders[nextEmptySpider] = temp;
					nextEmptySpider++;
					if (nextEmptySpider >= 30) {
						nextEmptySpider = 0;
					}
					spiderCooldown = 20;
				}

				if (counter % (5 +  (rand() % 15)) == 0 && bulletCooldown == 0 && generation == 1) {
					BULLET temp;
					if (rand() % 2) {
						createBullet(&temp, 0, 0, 0, 0, 0, 0, 0, 0, &player);
					} else {
						createBullet(&temp, 0, 0, 0, 0, 0, 0, 0, 1, &player);
					}
					bullets[nextEmptyBullet] = temp;
					nextEmptyBullet++;
					if (nextEmptyBullet >= 30) {
						nextEmptyBullet = 0;
					}
					bulletCooldown = 5;
				}

				if (playerHealth <= 0) {
					keepLooping = 0;
					state = 999;
				}

				// Drawing

				// If player is invulnerable, flash the player to give player notice
				if (playerInvincibility > 0) {
					if (drawPlayer == 1) {
						drawPlayer = 0;
					} else {
						drawPlayer = 1;
					}
				} else {
					drawPlayer = 1;
				}

				// Drawing the objects
				drawBasics(&player, &topBar, &middleBar, &bottomBar, &drawPlayer);
				updateSpiders(spiders, &playerHealth, &player, &playerInvincibility);
				updateBullets(bullets, &playerHealth, &player, &playerInvincibility);
				playerMovement(&player, &upPressed, &downPressed, &topBar, &middleBar, &bottomBar);

				if (KEY_DOWN_NOW(BUTTON_SELECT)) {
					keepLooping = 0;
					state = 0;
				}

				// Cooldown to prevent walls of spiders from appearing
				if (spiderCooldown != 0) {
					spiderCooldown--;
				}

				if (bulletCooldown != 0) {
					bulletCooldown--;
				}

				// If player gets hit, they get invincibility frames that last 100 loops. This decreases the remaining frames.
				if (playerInvincibility != 0) {
					playerInvincibility--;
				}

				// Displays HP
				char hpDisplay[20];
				sprintf(hpDisplay, "Health: %d", playerHealth);
				drawString4(150, 3, hpDisplay, 250);
				drawRect4(151, 75, 6, playerHealth, 246);
				drawRect4(151, 75 + playerHealth, 6, 100 - playerHealth, 247);

				// MODE 4
				waitForVblank();
				FlipPage();

				counter++;
				if (counter > 1000 && counter < 1150) {
					generation = 0;
				} else if (counter > 1150) {
					keepLooping = 0;
					state = 42;
				}
			}
		} else if (state == 42) { // Story 21
			int keepLooping = 1;
			int counter = 0;
			int upPressed = 0;
			int downPressed = 0;
			int drawPlayer = 1;
			char text[50];
			char printing[50];
			char *textPointer = &text[0];
			int letters = 0;
			while (keepLooping) {
				CLEAR;

				// STORY TEXT HERE
				if (counter < 50) {
					memset(printing, 0, 50);
				}
				if (counter >= 50 && counter < 200) {
					sprintf(text, "Like the purple bullets?");
					printText(2);
					if (counter == 199) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 200 && counter < 350) {
					sprintf(text, "They were made just for you.");
					printText(2);
					if (counter == 349) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 350) {
					keepLooping = 0;
					state = 43;
				}

				if (counter >= 0 && counter < 50) {
					playerHealth++;
					if (playerHealth > 100) {
						playerHealth = 100;
					}
				}

				drawString4(20, 10, printing, 244);
				drawBasics(&player, &topBar, &middleBar, &bottomBar, &drawPlayer);
				playerMovement(&player, &upPressed, &downPressed, &topBar, &middleBar, &bottomBar);

				if (KEY_DOWN_NOW(BUTTON_SELECT)) {
					keepLooping = 0;
					state = 0;
				}

				// Increments counter of the loop, this is basically the timer
				counter++;
				if (counter > 987654) {
					counter = 0;
				}

				// Displays HP
				char hpDisplay[20];
				sprintf(hpDisplay, "Health: %d", playerHealth);
				drawString4(150, 3, hpDisplay, 250);
				drawRect4(151, 75, 6, playerHealth, 246);
				drawRect4(151, 75 + playerHealth, 6, 100 - playerHealth, 247);

				// MODE 4
				waitForVblank();
				FlipPage();
			}
		} else if (state == 43) { // Stage 21
			int keepLooping = 1;
			int counter = 0;
			int upPressed = 0;
			int downPressed = 0;
			int playerInvincibility = 0;
			int spiderCooldown = 0;
			int bulletCooldown = 0;
			int drawPlayer = 1;
			int generation = 1;
			// Create SPIDER enemies for this stage
			SPIDER spiders[30];
			int nextEmptySpider = 0;
			for (int i = 0; i < 30; i++) { // Initialize them
				spiders[i].arrayPosition = 987654;
			}
			// Create BULLET enemies for this stage
			BULLET bullets[30];
			int nextEmptyBullet = 0;
			for (int i = 0; i < 30; i++) {
				bullets[i].arrayPosition = 987654;
			}

			while (keepLooping) {
				CLEAR;

				if (counter % (5 +  (rand() % 20)) == 0 && spiderCooldown == 0 && generation == 1) {
					SPIDER temp;
					createSpider(&temp, 1, &nextEmptySpider, -1, 3, -1, -1, 0, 244);
					spiders[nextEmptySpider] = temp;
					nextEmptySpider++;
					if (nextEmptySpider >= 30) {
						nextEmptySpider = 0;
					}
					spiderCooldown = 15;
				}

				if (counter % (5 +  (rand() % 15)) == 0 && bulletCooldown == 0 && generation == 1) {
					BULLET temp;
					createBullet(&temp, 0, 0, 0, 0, 0, 0, 0, 1, &player);
					bullets[nextEmptyBullet] = temp;
					nextEmptyBullet++;
					if (nextEmptyBullet >= 30) {
						nextEmptyBullet = 0;
					}
					bulletCooldown = 5;
				}

				if (playerHealth <= 0) {
					keepLooping = 0;
					state = 999;
				}

				// Drawing

				// If player is invulnerable, flash the player to give player notice
				if (playerInvincibility > 0) {
					if (drawPlayer == 1) {
						drawPlayer = 0;
					} else {
						drawPlayer = 1;
					}
				} else {
					drawPlayer = 1;
				}

				// Drawing the objects
				drawBasics(&player, &topBar, &middleBar, &bottomBar, &drawPlayer);
				updateSpiders(spiders, &playerHealth, &player, &playerInvincibility);
				updateBullets(bullets, &playerHealth, &player, &playerInvincibility);
				playerMovement(&player, &upPressed, &downPressed, &topBar, &middleBar, &bottomBar);

				if (KEY_DOWN_NOW(BUTTON_SELECT)) {
					keepLooping = 0;
					state = 0;
				}

				// Cooldown to prevent walls of spiders from appearing
				if (spiderCooldown != 0) {
					spiderCooldown--;
				}

				if (bulletCooldown != 0) {
					bulletCooldown--;
				}

				// If player gets hit, they get invincibility frames that last 100 loops. This decreases the remaining frames.
				if (playerInvincibility != 0) {
					playerInvincibility--;
				}

				// Displays HP
				char hpDisplay[20];
				sprintf(hpDisplay, "Health: %d", playerHealth);
				drawString4(150, 3, hpDisplay, 250);
				drawRect4(151, 75, 6, playerHealth, 246);
				drawRect4(151, 75 + playerHealth, 6, 100 - playerHealth, 247);

				// MODE 4
				waitForVblank();
				FlipPage();

				counter++;
				if (counter > 1000 && counter < 1150) {
					generation = 0;
				} else if (counter > 1150) {
					keepLooping = 0;
					state = 44;
				}
			}
		} else if (state == 44) { // Story 22
			spiderdance_stop();
			int keepLooping = 1;
			int counter = 0;
			int upPressed = 0;
			int downPressed = 0;
			int drawPlayer = 1;
			char text[50];
			char printing[50];
			char *textPointer = &text[0];
			int letters = 0;
			while (keepLooping) {
				CLEAR;

				// STORY TEXT HERE
				if (counter < 50) {
					memset(printing, 0, 50);
				}
				if (counter >= 50 && counter < 200) {
					sprintf(text, "Well, it's time.");
					printText(2);
					if (counter == 199) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 200) {
					keepLooping = 0;
					state = 45;
				}

				drawString4(20, 10, printing, 244);
				drawBasics(&player, &topBar, &middleBar, &bottomBar, &drawPlayer);
				playerMovement(&player, &upPressed, &downPressed, &topBar, &middleBar, &bottomBar);

				if (KEY_DOWN_NOW(BUTTON_SELECT)) {
					keepLooping = 0;
					state = 0;
				}

				// Increments counter of the loop, this is basically the timer
				counter++;
				if (counter > 987654) {
					counter = 0;
				}

				// Displays HP
				char hpDisplay[20];
				sprintf(hpDisplay, "Health: %d", playerHealth);
				drawString4(150, 3, hpDisplay, 250);
				drawRect4(151, 75, 6, playerHealth, 246);
				drawRect4(151, 75 + playerHealth, 6, 100 - playerHealth, 247);

				// MODE 4
				waitForVblank();
				FlipPage();
			}
		} else if (state == 45) { // Stage 22
			int keepLooping = 1;
			int counter = 0;
			int upPressed = 0;
			int downPressed = 0;
			int playerInvincibility = 0;
			int spiderCooldown = 0;
			int bulletCooldown = 0;
			int laserCooldown = 0;
			int drawPlayer = 1;
			int generation = 1;
			// // Create SPIDER enemies for this stage
			// SPIDER spiders[30];
			// int nextEmptySpider = 0;
			// for (int i = 0; i < 30; i++) { // Initialize them
			// 	spiders[i].arrayPosition = 987654;
			// }
			// // Create BULLET enemies for this stage
			// BULLET bullets[30];
			// int nextEmptyBullet = 0;
			// for (int i = 0; i < 30; i++) {
			// 	bullets[i].arrayPosition = 987654;
			// }
			// Create LASER enemies for this stage
			LASER lasers[5];
			int nextEmptyLaser = 0;
			for (int i = 0; i < 5; i++) {
				lasers[i].arrayPosition = 987654;
			}

			while (keepLooping) {
				CLEAR;

				// if (counter % (5 +  (rand() % 20)) == 0 && spiderCooldown == 0 && generation == 1) {
				// 	SPIDER temp;
				// 	createSpider(&temp, 1, &nextEmptySpider, -1, 3, -1, -1, 0, 244);
				// 	spiders[nextEmptySpider] = temp;
				// 	nextEmptySpider++;
				// 	if (nextEmptySpider >= 30) {
				// 		nextEmptySpider = 0;
				// 	}
				// 	spiderCooldown = 15;
				// }

				// if (counter % (5 +  (rand() % 15)) == 0 && bulletCooldown == 0 && generation == 1) {
				// 	BULLET temp;
				// 	createBullet(&temp, 0, 0, 0, 0, 0, 0, 0, 1, &player);
				// 	bullets[nextEmptyBullet] = temp;
				// 	nextEmptyBullet++;
				// 	if (nextEmptyBullet >= 30) {
				// 		nextEmptyBullet = 0;
				// 	}
				// 	bulletCooldown = 5;
				// }

				if (counter == 25) {
					LASER temp;
					createLaser(&temp, player.x + 3 - 15, 0, 30, 0, 30, 20, 0);
					lasers[nextEmptyLaser] = temp;
					nextEmptyLaser++;
					if (nextEmptyLaser >= 30) {
						nextEmptyLaser = 0;
					}
					laserCooldown = 50;
				}
				if (playerHealth <= 0) {
					keepLooping = 0;
					state = 999;
				}

				// Drawing

				// If player is invulnerable, flash the player to give player notice
				if (playerInvincibility > 0) {
					if (drawPlayer == 1) {
						drawPlayer = 0;
					} else {
						drawPlayer = 1;
					}
				} else {
					drawPlayer = 1;
				}

				// Drawing the objects
				drawBasics(&player, &topBar, &middleBar, &bottomBar, &drawPlayer);
				// updateSpiders(spiders, &playerHealth, &player, &playerInvincibility);
				// updateBullets(bullets, &playerHealth, &player, &playerInvincibility);
				updateLasers(lasers, &playerHealth, &player, &playerInvincibility);
				playerMovement(&player, &upPressed, &downPressed, &topBar, &middleBar, &bottomBar);

				if (KEY_DOWN_NOW(BUTTON_SELECT)) {
					keepLooping = 0;
					state = 0;
				}

				// Cooldown to prevent walls of spiders from appearing
				if (spiderCooldown != 0) {
					spiderCooldown--;
				}

				if (bulletCooldown != 0) {
					bulletCooldown--;
				}

				if (laserCooldown != 0) {
					laserCooldown--;
				}

				// If player gets hit, they get invincibility frames that last 100 loops. This decreases the remaining frames.
				if (playerInvincibility != 0) {
					playerInvincibility--;
				}

				// Displays HP
				char hpDisplay[20];
				sprintf(hpDisplay, "Health: %d", playerHealth);
				drawString4(150, 3, hpDisplay, 250);
				drawRect4(151, 75, 6, playerHealth, 246);
				drawRect4(151, 75 + playerHealth, 6, 100 - playerHealth, 247);

				// MODE 4
				waitForVblank();
				FlipPage();

				counter++;
				if (counter > 100 && counter < 110) {
					generation = 0;
				} else if (counter > 110) {
					keepLooping = 0;
					state = 46;
				}
			}
		} else if (state == 46) { // Story 23
			glamour_play();
			int keepLooping = 1;
			int counter = 0;
			int upPressed = 0;
			int downPressed = 0;
			int drawPlayer = 1;
			char text[50];
			char printing[50];
			char *textPointer = &text[0];
			int letters = 0;
			while (keepLooping) {
				CLEAR;

				// STORY TEXT HERE
				if (counter < 50) {
					memset(printing, 0, 50);
				}
				if (counter >= 50 && counter < 200) {
					sprintf(text, "Now, the party begins.");
					printText(2);
					if (counter == 199) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 200) {
					keepLooping = 0;
					state = 47;
				}

				if (counter >= 0 && counter < 50) {
					playerHealth++;
					if (playerHealth > 100) {
						playerHealth = 100;
					}
				}
				drawString4(20, 10, printing, 244);
				drawBasics(&player, &topBar, &middleBar, &bottomBar, &drawPlayer);
				playerMovement(&player, &upPressed, &downPressed, &topBar, &middleBar, &bottomBar);

				if (KEY_DOWN_NOW(BUTTON_SELECT)) {
					keepLooping = 0;
					state = 0;
				}

				// Increments counter of the loop, this is basically the timer
				counter++;
				if (counter > 987654) {
					counter = 0;
				}

				// Displays HP
				char hpDisplay[20];
				sprintf(hpDisplay, "Health: %d", playerHealth);
				drawString4(150, 3, hpDisplay, 250);
				drawRect4(151, 75, 6, playerHealth, 246);
				drawRect4(151, 75 + playerHealth, 6, 100 - playerHealth, 247);

				// MODE 4
				waitForVblank();
				FlipPage();
			}
		} else if (state == 47) { // Stage 23
			int keepLooping = 1;
			int counter = 0;
			int upPressed = 0;
			int downPressed = 0;
			int playerInvincibility = 0;
			int spiderCooldown = 0;
			int bulletCooldown = 0;
			int laserCooldown = 0;
			int drawPlayer = 1;
			int generation = 1;
			// // Create SPIDER enemies for this stage
			// SPIDER spiders[30];
			// int nextEmptySpider = 0;
			// for (int i = 0; i < 30; i++) { // Initialize them
			// 	spiders[i].arrayPosition = 987654;
			// }
			// // Create BULLET enemies for this stage
			// BULLET bullets[30];
			// int nextEmptyBullet = 0;
			// for (int i = 0; i < 30; i++) {
			// 	bullets[i].arrayPosition = 987654;
			// }
			// Create LASER enemies for this stage
			LASER lasers[5];
			int nextEmptyLaser = 0;
			for (int i = 0; i < 5; i++) {
				lasers[i].arrayPosition = 987654;
			}

			while (keepLooping) {
				CLEAR;

				// if (counter % (5 +  (rand() % 20)) == 0 && spiderCooldown == 0 && generation == 1) {
				// 	SPIDER temp;
				// 	createSpider(&temp, 1, &nextEmptySpider, -1, 3, -1, -1, 0, 244);
				// 	spiders[nextEmptySpider] = temp;
				// 	nextEmptySpider++;
				// 	if (nextEmptySpider >= 30) {
				// 		nextEmptySpider = 0;
				// 	}
				// 	spiderCooldown = 15;
				// }

				// if (counter % (5 +  (rand() % 15)) == 0 && bulletCooldown == 0 && generation == 1) {
				// 	BULLET temp;
				// 	createBullet(&temp, 0, 0, 0, 0, 0, 0, 0, 1, &player);
				// 	bullets[nextEmptyBullet] = temp;
				// 	nextEmptyBullet++;
				// 	if (nextEmptyBullet >= 30) {
				// 		nextEmptyBullet = 0;
				// 	}
				// 	bulletCooldown = 5;
				// }

				if (counter % 70 && laserCooldown == 0 && generation == 1) {
					LASER temp;
					createLaser(&temp, 15 + (rand() % 240 - 30), 0, 30, 0, 30, 20, 0);
					lasers[nextEmptyLaser] = temp;
					nextEmptyLaser++;
					if (nextEmptyLaser >= 5) {
						nextEmptyLaser = 0;
					}
					laserCooldown = 50;
				}
				if (playerHealth <= 0) {
					keepLooping = 0;
					state = 999;
				}
				// Drawing

				// If player is invulnerable, flash the player to give player notice
				if (playerInvincibility > 0) {
					if (drawPlayer == 1) {
						drawPlayer = 0;
					} else {
						drawPlayer = 1;
					}
				} else {
					drawPlayer = 1;
				}

				// Drawing the objects
				drawBasics(&player, &topBar, &middleBar, &bottomBar, &drawPlayer);
				// updateSpiders(spiders, &playerHealth, &player, &playerInvincibility);
				// updateBullets(bullets, &playerHealth, &player, &playerInvincibility);
				updateLasers(lasers, &playerHealth, &player, &playerInvincibility);
				playerMovement(&player, &upPressed, &downPressed, &topBar, &middleBar, &bottomBar);

				if (KEY_DOWN_NOW(BUTTON_SELECT)) {
					keepLooping = 0;
					state = 0;
				}

				// Cooldown to prevent walls of spiders from appearing
				if (spiderCooldown != 0) {
					spiderCooldown--;
				}

				if (bulletCooldown != 0) {
					bulletCooldown--;
				}

				if (laserCooldown != 0) {
					laserCooldown--;
				}

				// If player gets hit, they get invincibility frames that last 100 loops. This decreases the remaining frames.
				if (playerInvincibility != 0) {
					playerInvincibility--;
				}

				// Displays HP
				char hpDisplay[20];
				sprintf(hpDisplay, "Health: %d", playerHealth);
				drawString4(150, 3, hpDisplay, 250);
				drawRect4(151, 75, 6, playerHealth, 246);
				drawRect4(151, 75 + playerHealth, 6, 100 - playerHealth, 247);

				// MODE 4
				waitForVblank();
				FlipPage();

				counter++;
				if (counter > 800 && counter < 850) {
					generation = 0;
				} else if (counter > 850) {
					keepLooping = 0;
					state = 48;
				}
			}
		} else if (state == 48) { // Story 24
			int keepLooping = 1;
			int counter = 0;
			int upPressed = 0;
			int downPressed = 0;
			int drawPlayer = 1;
			char text[50];
			char printing[50];
			char *textPointer = &text[0];
			int letters = 0;
			while (keepLooping) {
				CLEAR;

				// STORY TEXT HERE
				if (counter < 50) {
					memset(printing, 0, 50);
				}
				if (counter >= 50 && counter < 200) {
					sprintf(text, "You're so persistent.");
					printText(2);
					if (counter == 199) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 200) {
					keepLooping = 0;
					state = 49;
				}

				drawString4(20, 10, printing, 244);
				drawBasics(&player, &topBar, &middleBar, &bottomBar, &drawPlayer);
				playerMovement(&player, &upPressed, &downPressed, &topBar, &middleBar, &bottomBar);

				if (KEY_DOWN_NOW(BUTTON_SELECT)) {
					keepLooping = 0;
					state = 0;
				}

				// Increments counter of the loop, this is basically the timer
				counter++;
				if (counter > 987654) {
					counter = 0;
				}

				// Displays HP
				char hpDisplay[20];
				sprintf(hpDisplay, "Health: %d", playerHealth);
				drawString4(150, 3, hpDisplay, 250);
				drawRect4(151, 75, 6, playerHealth, 246);
				drawRect4(151, 75 + playerHealth, 6, 100 - playerHealth, 247);

				// MODE 4
				waitForVblank();
				FlipPage();
			}
		} else if (state == 49) { // Stage 24
			int keepLooping = 1;
			int counter = 0;
			int upPressed = 0;
			int downPressed = 0;
			int playerInvincibility = 0;
			int spiderCooldown = 0;
			int bulletCooldown = 0;
			int laserCooldown = 0;
			int drawPlayer = 1;
			int generation = 1;
			// // Create SPIDER enemies for this stage
			// SPIDER spiders[30];
			// int nextEmptySpider = 0;
			// for (int i = 0; i < 30; i++) { // Initialize them
			// 	spiders[i].arrayPosition = 987654;
			// }
			// // Create BULLET enemies for this stage
			// BULLET bullets[30];
			// int nextEmptyBullet = 0;
			// for (int i = 0; i < 30; i++) {
			// 	bullets[i].arrayPosition = 987654;
			// }
			// Create LASER enemies for this stage
			LASER lasers[5];
			int nextEmptyLaser = 0;
			for (int i = 0; i < 5; i++) {
				lasers[i].arrayPosition = 987654;
			}

			while (keepLooping) {
				CLEAR;

				// if (counter % (5 +  (rand() % 20)) == 0 && spiderCooldown == 0 && generation == 1) {
				// 	SPIDER temp;
				// 	createSpider(&temp, 1, &nextEmptySpider, -1, 3, -1, -1, 0, 244);
				// 	spiders[nextEmptySpider] = temp;
				// 	nextEmptySpider++;
				// 	if (nextEmptySpider >= 30) {
				// 		nextEmptySpider = 0;
				// 	}
				// 	spiderCooldown = 15;
				// }

				// if (counter % (5 +  (rand() % 15)) == 0 && bulletCooldown == 0 && generation == 1) {
				// 	BULLET temp;
				// 	createBullet(&temp, 0, 0, 0, 0, 0, 0, 0, 1, &player);
				// 	bullets[nextEmptyBullet] = temp;
				// 	nextEmptyBullet++;
				// 	if (nextEmptyBullet >= 30) {
				// 		nextEmptyBullet = 0;
				// 	}
				// 	bulletCooldown = 5;
				// }

				if (counter % 70 && laserCooldown == 0 && generation == 1) {
					LASER temp;
					if (rand() % 3 == 2) {
						createLaser(&temp, player.x - 15 + 3, 0, 30, 0, 30, 20, 0);
					} else {
						createLaser(&temp, (rand() % 240 - 30), 0, 30, 0, 30, 20, 0);
					}
					lasers[nextEmptyLaser] = temp;
					nextEmptyLaser++;
					if (nextEmptyLaser >= 5) {
						nextEmptyLaser = 0;
					}
					laserCooldown = 50;
				}
				if (playerHealth <= 0) {
					keepLooping = 0;
					state = 999;
				}
				// Drawing

				// If player is invulnerable, flash the player to give player notice
				if (playerInvincibility > 0) {
					if (drawPlayer == 1) {
						drawPlayer = 0;
					} else {
						drawPlayer = 1;
					}
				} else {
					drawPlayer = 1;
				}

				// Drawing the objects
				drawBasics(&player, &topBar, &middleBar, &bottomBar, &drawPlayer);
				// updateSpiders(spiders, &playerHealth, &player, &playerInvincibility);
				// updateBullets(bullets, &playerHealth, &player, &playerInvincibility);
				updateLasers(lasers, &playerHealth, &player, &playerInvincibility);
				playerMovement(&player, &upPressed, &downPressed, &topBar, &middleBar, &bottomBar);

				if (KEY_DOWN_NOW(BUTTON_SELECT)) {
					keepLooping = 0;
					state = 0;
				}

				// Cooldown to prevent walls of spiders from appearing
				if (spiderCooldown != 0) {
					spiderCooldown--;
				}

				if (bulletCooldown != 0) {
					bulletCooldown--;
				}

				if (laserCooldown != 0) {
					laserCooldown--;
				}

				// If player gets hit, they get invincibility frames that last 100 loops. This decreases the remaining frames.
				if (playerInvincibility != 0) {
					playerInvincibility--;
				}

				// Displays HP
				char hpDisplay[20];
				sprintf(hpDisplay, "Health: %d", playerHealth);
				drawString4(150, 3, hpDisplay, 250);
				drawRect4(151, 75, 6, playerHealth, 246);
				drawRect4(151, 75 + playerHealth, 6, 100 - playerHealth, 247);

				// MODE 4
				waitForVblank();
				FlipPage();

				counter++;
				if (counter > 800 && counter < 850) {
					generation = 0;
				} else if (counter > 850) {
					keepLooping = 0;
					state = 50;
				}
			}
		} else if (state == 50) { // Story 25
			int keepLooping = 1;
			int counter = 0;
			int upPressed = 0;
			int downPressed = 0;
			int drawPlayer = 1;
			char text[50];
			char printing[50];
			char *textPointer = &text[0];
			int letters = 0;
			while (keepLooping) {
				CLEAR;

				// STORY TEXT HERE
				if (counter < 50) {
					memset(printing, 0, 50);
				}
				if (counter >= 50 && counter < 200) {
					sprintf(text, "You can't avoid these for long.");
					printText(2);
					if (counter == 199) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 200) {
					keepLooping = 0;
					state = 51;
				}

				drawString4(20, 10, printing, 244);
				drawBasics(&player, &topBar, &middleBar, &bottomBar, &drawPlayer);
				playerMovement(&player, &upPressed, &downPressed, &topBar, &middleBar, &bottomBar);

				if (KEY_DOWN_NOW(BUTTON_SELECT)) {
					keepLooping = 0;
					state = 0;
				}

				// Increments counter of the loop, this is basically the timer
				counter++;
				if (counter > 987654) {
					counter = 0;
				}

				// Displays HP
				char hpDisplay[20];
				sprintf(hpDisplay, "Health: %d", playerHealth);
				drawString4(150, 3, hpDisplay, 250);
				drawRect4(151, 75, 6, playerHealth, 246);
				drawRect4(151, 75 + playerHealth, 6, 100 - playerHealth, 247);

				// MODE 4
				waitForVblank();
				FlipPage();
			}
		} else if (state == 51) { // Stage 24
			int keepLooping = 1;
			int counter = 0;
			int upPressed = 0;
			int downPressed = 0;
			int playerInvincibility = 0;
			int spiderCooldown = 0;
			int bulletCooldown = 0;
			int laserCooldown = 0;
			int drawPlayer = 1;
			int generation = 1;
			// // Create SPIDER enemies for this stage
			// SPIDER spiders[30];
			// int nextEmptySpider = 0;
			// for (int i = 0; i < 30; i++) { // Initialize them
			// 	spiders[i].arrayPosition = 987654;
			// }
			// // Create BULLET enemies for this stage
			// BULLET bullets[30];
			// int nextEmptyBullet = 0;
			// for (int i = 0; i < 30; i++) {
			// 	bullets[i].arrayPosition = 987654;
			// }
			// Create LASER enemies for this stage
			LASER lasers[5];
			int nextEmptyLaser = 0;
			for (int i = 0; i < 5; i++) {
				lasers[i].arrayPosition = 987654;
			}

			while (keepLooping) {
				CLEAR;

				// if (counter % (5 +  (rand() % 20)) == 0 && spiderCooldown == 0 && generation == 1) {
				// 	SPIDER temp;
				// 	createSpider(&temp, 1, &nextEmptySpider, -1, 3, -1, -1, 0, 244);
				// 	spiders[nextEmptySpider] = temp;
				// 	nextEmptySpider++;
				// 	if (nextEmptySpider >= 30) {
				// 		nextEmptySpider = 0;
				// 	}
				// 	spiderCooldown = 15;
				// }

				// if (counter % (5 +  (rand() % 15)) == 0 && bulletCooldown == 0 && generation == 1) {
				// 	BULLET temp;
				// 	createBullet(&temp, 0, 0, 0, 0, 0, 0, 0, 1, &player);
				// 	bullets[nextEmptyBullet] = temp;
				// 	nextEmptyBullet++;
				// 	if (nextEmptyBullet >= 30) {
				// 		nextEmptyBullet = 0;
				// 	}
				// 	bulletCooldown = 5;
				// }

				if (counter % 70 && laserCooldown == 0 && generation == 1) {
					LASER temp;
					if (rand() % 3 == 2) {
						createLaser(&temp, player.x - 20 + 3, 0, 40, 0, 30, 20, 0);
					} else {
						createLaser(&temp, (rand() % 240 - 40), 0, 40, 0, 30, 20, 0);
					}
					lasers[nextEmptyLaser] = temp;
					nextEmptyLaser++;
					if (nextEmptyLaser >= 5) {
						nextEmptyLaser = 0;
					}
					laserCooldown = 50;
				}
				if (playerHealth <= 0) {
					keepLooping = 0;
					state = 999;
				}

				// Drawing

				// If player is invulnerable, flash the player to give player notice
				if (playerInvincibility > 0) {
					if (drawPlayer == 1) {
						drawPlayer = 0;
					} else {
						drawPlayer = 1;
					}
				} else {
					drawPlayer = 1;
				}

				// Drawing the objects
				drawBasics(&player, &topBar, &middleBar, &bottomBar, &drawPlayer);
				// updateSpiders(spiders, &playerHealth, &player, &playerInvincibility);
				// updateBullets(bullets, &playerHealth, &player, &playerInvincibility);
				updateLasers(lasers, &playerHealth, &player, &playerInvincibility);
				playerMovement(&player, &upPressed, &downPressed, &topBar, &middleBar, &bottomBar);

				if (KEY_DOWN_NOW(BUTTON_SELECT)) {
					keepLooping = 0;
					state = 0;
				}

				// Cooldown to prevent walls of spiders from appearing
				if (spiderCooldown != 0) {
					spiderCooldown--;
				}

				if (bulletCooldown != 0) {
					bulletCooldown--;
				}

				if (laserCooldown != 0) {
					laserCooldown--;
				}

				// If player gets hit, they get invincibility frames that last 100 loops. This decreases the remaining frames.
				if (playerInvincibility != 0) {
					playerInvincibility--;
				}

				// Displays HP
				char hpDisplay[20];
				sprintf(hpDisplay, "Health: %d", playerHealth);
				drawString4(150, 3, hpDisplay, 250);
				drawRect4(151, 75, 6, playerHealth, 246);
				drawRect4(151, 75 + playerHealth, 6, 100 - playerHealth, 247);

				// MODE 4
				waitForVblank();
				FlipPage();

				counter++;
				if (counter > 800 && counter < 850) {
					generation = 0;
				} else if (counter > 850) {
					keepLooping = 0;
					state = 52;
				}
			}
		} else if (state == 52) { // Story 26
			int keepLooping = 1;
			int counter = 0;
			int upPressed = 0;
			int downPressed = 0;
			int drawPlayer = 1;
			char text[50];
			char printing[50];
			char *textPointer = &text[0];
			int letters = 0;
			while (keepLooping) {
				CLEAR;

				// STORY TEXT HERE
				if (counter < 50) {
					memset(printing, 0, 50);
				}
				if (counter >= 50 && counter < 200) {
					sprintf(text, "There's more where that came from.");
					printText(2);
					if (counter == 199) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 200) {
					keepLooping = 0;
					state = 53;
				}

				drawString4(20, 10, printing, 244);
				drawBasics(&player, &topBar, &middleBar, &bottomBar, &drawPlayer);
				playerMovement(&player, &upPressed, &downPressed, &topBar, &middleBar, &bottomBar);

				if (KEY_DOWN_NOW(BUTTON_SELECT)) {
					keepLooping = 0;
					state = 0;
				}

				// Increments counter of the loop, this is basically the timer
				counter++;
				if (counter > 987654) {
					counter = 0;
				}

				// Displays HP
				char hpDisplay[20];
				sprintf(hpDisplay, "Health: %d", playerHealth);
				drawString4(150, 3, hpDisplay, 250);
				drawRect4(151, 75, 6, playerHealth, 246);
				drawRect4(151, 75 + playerHealth, 6, 100 - playerHealth, 247);

				// MODE 4
				waitForVblank();
				FlipPage();
			}
		} else if (state == 53) { // Stage 26
			int keepLooping = 1;
			int counter = 0;
			int upPressed = 0;
			int downPressed = 0;
			int playerInvincibility = 0;
			int spiderCooldown = 0;
			int bulletCooldown = 0;
			int laserCooldown = 0;
			int drawPlayer = 1;
			int generation = 1;
			// // Create SPIDER enemies for this stage
			// SPIDER spiders[30];
			// int nextEmptySpider = 0;
			// for (int i = 0; i < 30; i++) { // Initialize them
			// 	spiders[i].arrayPosition = 987654;
			// }
			// // Create BULLET enemies for this stage
			// BULLET bullets[30];
			// int nextEmptyBullet = 0;
			// for (int i = 0; i < 30; i++) {
			// 	bullets[i].arrayPosition = 987654;
			// }
			// Create LASER enemies for this stage
			LASER lasers[5];
			int nextEmptyLaser = 0;
			for (int i = 0; i < 5; i++) {
				lasers[i].arrayPosition = 987654;
			}

			while (keepLooping) {
				CLEAR;

				// if (counter % (5 +  (rand() % 20)) == 0 && spiderCooldown == 0 && generation == 1) {
				// 	SPIDER temp;
				// 	createSpider(&temp, 1, &nextEmptySpider, -1, 3, -1, -1, 0, 244);
				// 	spiders[nextEmptySpider] = temp;
				// 	nextEmptySpider++;
				// 	if (nextEmptySpider >= 30) {
				// 		nextEmptySpider = 0;
				// 	}
				// 	spiderCooldown = 15;
				// }

				// if (counter % (5 +  (rand() % 15)) == 0 && bulletCooldown == 0 && generation == 1) {
				// 	BULLET temp;
				// 	createBullet(&temp, 0, 0, 0, 0, 0, 0, 0, 1, &player);
				// 	bullets[nextEmptyBullet] = temp;
				// 	nextEmptyBullet++;
				// 	if (nextEmptyBullet >= 30) {
				// 		nextEmptyBullet = 0;
				// 	}
				// 	bulletCooldown = 5;
				// }

				if (counter % 70 && laserCooldown == 0 && generation == 1 && counter < 720) {
					LASER temp;
					if (rand() % 3 == 2) {
						createLaser(&temp, player.x - 25 + 3, 0, 50, 0, 30, 20, 0);
					} else {
						createLaser(&temp, 25 + (rand() % 240 - 50), 0, 50, 0, 30, 20, 0);
					}
					lasers[nextEmptyLaser] = temp;
					nextEmptyLaser++;
					if (nextEmptyLaser >= 5) {
						nextEmptyLaser = 0;
					}
					laserCooldown = 50;
				} else if (counter > 720 && laserCooldown == 0 && counter % 70 && generation == 1) {
					LASER temp;
					createLaser(&temp, 0, player.y - 25 + 3, 50, 0, 30, 20, 1);
					lasers[nextEmptyLaser] = temp;
					nextEmptyLaser++;
					if (nextEmptyLaser >= 5) {
						nextEmptyLaser = 0;
					}
					laserCooldown = 50;
				}
				if (playerHealth <= 0) {
					keepLooping = 0;
					state = 999;
				}

				// Drawing

				// If player is invulnerable, flash the player to give player notice
				if (playerInvincibility > 0) {
					if (drawPlayer == 1) {
						drawPlayer = 0;
					} else {
						drawPlayer = 1;
					}
				} else {
					drawPlayer = 1;
				}

				// Drawing the objects
				drawBasics(&player, &topBar, &middleBar, &bottomBar, &drawPlayer);
				// updateSpiders(spiders, &playerHealth, &player, &playerInvincibility);
				// updateBullets(bullets, &playerHealth, &player, &playerInvincibility);
				updateLasers(lasers, &playerHealth, &player, &playerInvincibility);
				playerMovement(&player, &upPressed, &downPressed, &topBar, &middleBar, &bottomBar);

				if (KEY_DOWN_NOW(BUTTON_SELECT)) {
					keepLooping = 0;
					state = 0;
				}

				// Cooldown to prevent walls of spiders from appearing
				if (spiderCooldown != 0) {
					spiderCooldown--;
				}

				if (bulletCooldown != 0) {
					bulletCooldown--;
				}

				if (laserCooldown != 0) {
					laserCooldown--;
				}

				// If player gets hit, they get invincibility frames that last 100 loops. This decreases the remaining frames.
				if (playerInvincibility != 0) {
					playerInvincibility--;
				}

				// Displays HP
				char hpDisplay[20];
				sprintf(hpDisplay, "Health: %d", playerHealth);
				drawString4(150, 3, hpDisplay, 250);
				drawRect4(151, 75, 6, playerHealth, 246);
				drawRect4(151, 75 + playerHealth, 6, 100 - playerHealth, 247);

				// MODE 4
				waitForVblank();
				FlipPage();

				counter++;
				if (counter > 800 && counter < 850) {
					generation = 0;
				} else if (counter > 850) {
					keepLooping = 0;
					state = 54;
				}
			}
		} else if (state == 54) { // Story 27
			int keepLooping = 1;
			int counter = 0;
			int upPressed = 0;
			int downPressed = 0;
			int drawPlayer = 1;
			char text[50];
			char printing[50];
			char *textPointer = &text[0];
			int letters = 0;
			while (keepLooping) {
				CLEAR;

				// STORY TEXT HERE
				if (counter < 50) {
					memset(printing, 0, 50);
				}
				if (counter >= 50 && counter < 200) {
					sprintf(text, "Like I said, ");
					printText(2);
					if (counter == 199) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 200 && counter < 350) {
					sprintf(text, "I can do anything.");
					printText(2);
					if (counter == 349) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 350) {
					keepLooping = 0;
					state = 55;
				}

				if (counter >= 0 && counter < 50) {
					playerHealth++;
					if (playerHealth > 100) {
						playerHealth = 100;
					}
				}
				drawString4(20, 10, printing, 244);
				drawBasics(&player, &topBar, &middleBar, &bottomBar, &drawPlayer);
				playerMovement(&player, &upPressed, &downPressed, &topBar, &middleBar, &bottomBar);

				if (KEY_DOWN_NOW(BUTTON_SELECT)) {
					keepLooping = 0;
					state = 0;
				}

				// Increments counter of the loop, this is basically the timer
				counter++;
				if (counter > 987654) {
					counter = 0;
				}

				// Displays HP
				char hpDisplay[20];
				sprintf(hpDisplay, "Health: %d", playerHealth);
				drawString4(150, 3, hpDisplay, 250);
				drawRect4(151, 75, 6, playerHealth, 246);
				drawRect4(151, 75 + playerHealth, 6, 100 - playerHealth, 247);

				// MODE 4
				waitForVblank();
				FlipPage();
			}
		} else if (state == 55) { // Stage 27
			int keepLooping = 1;
			int counter = 0;
			int upPressed = 0;
			int downPressed = 0;
			int playerInvincibility = 0;
			int spiderCooldown = 0;
			int bulletCooldown = 0;
			int laserCooldown = 0;
			int drawPlayer = 1;
			int generation = 1;
			// // Create SPIDER enemies for this stage
			// SPIDER spiders[30];
			// int nextEmptySpider = 0;
			// for (int i = 0; i < 30; i++) { // Initialize them
			// 	spiders[i].arrayPosition = 987654;
			// }
			// // Create BULLET enemies for this stage
			// BULLET bullets[30];
			// int nextEmptyBullet = 0;
			// for (int i = 0; i < 30; i++) {
			// 	bullets[i].arrayPosition = 987654;
			// }
			// Create LASER enemies for this stage
			LASER lasers[5];
			int nextEmptyLaser = 0;
			for (int i = 0; i < 5; i++) {
				lasers[i].arrayPosition = 987654;
			}

			while (keepLooping) {
				CLEAR;

				// if (counter % (5 +  (rand() % 20)) == 0 && spiderCooldown == 0 && generation == 1) {
				// 	SPIDER temp;
				// 	createSpider(&temp, 1, &nextEmptySpider, -1, 3, -1, -1, 0, 244);
				// 	spiders[nextEmptySpider] = temp;
				// 	nextEmptySpider++;
				// 	if (nextEmptySpider >= 30) {
				// 		nextEmptySpider = 0;
				// 	}
				// 	spiderCooldown = 15;
				// }

				// if (counter % (5 +  (rand() % 15)) == 0 && bulletCooldown == 0 && generation == 1) {
				// 	BULLET temp;
				// 	createBullet(&temp, 0, 0, 0, 0, 0, 0, 0, 1, &player);
				// 	bullets[nextEmptyBullet] = temp;
				// 	nextEmptyBullet++;
				// 	if (nextEmptyBullet >= 30) {
				// 		nextEmptyBullet = 0;
				// 	}
				// 	bulletCooldown = 5;
				// }

				if (counter % 70 && laserCooldown == 0 && generation == 1 && counter < 720) {
					LASER temp;
					int randomize = rand() % 8;
					if (randomize <= 2) {
						createLaser(&temp, player.x - 20 + 3, 0, 40, 0, 30, 20, 0);
					} else if (randomize == 3) {
						createLaser(&temp, 0 + (rand() % 240 - 40), 0, 40, 0, 30, 20, 0);
					} else if (randomize == 4) {
						createLaser(&temp, 0, player.y - 20 + 3, 40, 0, 30, 20, 1);
					} else if (randomize >= 5) {
						createLaser(&temp, 0, 20 + (rand() % 160 - 60), 40, 0, 30, 20, 1);
					}
					lasers[nextEmptyLaser] = temp;
					nextEmptyLaser++;
					if (nextEmptyLaser >= 5) {
						nextEmptyLaser = 0;
					}
					laserCooldown = 50;
				} 
				if (playerHealth <= 0) {
					keepLooping = 0;
					state = 999;
				}

				// Drawing

				// If player is invulnerable, flash the player to give player notice
				if (playerInvincibility > 0) {
					if (drawPlayer == 1) {
						drawPlayer = 0;
					} else {
						drawPlayer = 1;
					}
				} else {
					drawPlayer = 1;
				}

				// Drawing the objects
				drawBasics(&player, &topBar, &middleBar, &bottomBar, &drawPlayer);
				// updateSpiders(spiders, &playerHealth, &player, &playerInvincibility);
				// updateBullets(bullets, &playerHealth, &player, &playerInvincibility);
				updateLasers(lasers, &playerHealth, &player, &playerInvincibility);
				playerMovement(&player, &upPressed, &downPressed, &topBar, &middleBar, &bottomBar);

				if (KEY_DOWN_NOW(BUTTON_SELECT)) {
					keepLooping = 0;
					state = 0;
				}

				// Cooldown to prevent walls of spiders from appearing
				if (spiderCooldown != 0) {
					spiderCooldown--;
				}

				if (bulletCooldown != 0) {
					bulletCooldown--;
				}

				if (laserCooldown != 0) {
					laserCooldown--;
				}

				// If player gets hit, they get invincibility frames that last 100 loops. This decreases the remaining frames.
				if (playerInvincibility != 0) {
					playerInvincibility--;
				}

				// Displays HP
				char hpDisplay[20];
				sprintf(hpDisplay, "Health: %d", playerHealth);
				drawString4(150, 3, hpDisplay, 250);
				drawRect4(151, 75, 6, playerHealth, 246);
				drawRect4(151, 75 + playerHealth, 6, 100 - playerHealth, 247);

				// MODE 4
				waitForVblank();
				FlipPage();

				counter++;
				if (counter > 800 && counter < 850) {
					generation = 0;
				} else if (counter > 850) {
					keepLooping = 0;
					state = 56;
				}
			}
		} else if (state == 56) { // Story 28
			int keepLooping = 1;
			int counter = 0;
			int upPressed = 0;
			int downPressed = 0;
			int drawPlayer = 1;
			char text[50];
			char printing[50];
			char *textPointer = &text[0];
			int letters = 0;
			while (keepLooping) {
				CLEAR;

				// STORY TEXT HERE
				if (counter < 50) {
					memset(printing, 0, 50);
				}
				if (counter >= 50 && counter < 200) {
					sprintf(text, "Why don't you give up?");
					printText(2);
					if (counter == 199) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 200 && counter < 350) {
					sprintf(text, "There's nothing good out there.");
					printText(2);
					if (counter == 349) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 350 && counter < 500) {
					sprintf(text, "You're much better off dead.");
					printText(2);
					if (counter == 349) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 499) {
					keepLooping = 0;
					state = 57;
				}


				drawString4(20, 10, printing, 244);
				drawBasics(&player, &topBar, &middleBar, &bottomBar, &drawPlayer);
				playerMovement(&player, &upPressed, &downPressed, &topBar, &middleBar, &bottomBar);

				if (KEY_DOWN_NOW(BUTTON_SELECT)) {
					keepLooping = 0;
					state = 0;
				}

				// Increments counter of the loop, this is basically the timer
				counter++;
				if (counter > 987654) {
					counter = 0;
				}

				// Displays HP
				char hpDisplay[20];
				sprintf(hpDisplay, "Health: %d", playerHealth);
				drawString4(150, 3, hpDisplay, 250);
				drawRect4(151, 75, 6, playerHealth, 246);
				drawRect4(151, 75 + playerHealth, 6, 100 - playerHealth, 247);

				// MODE 4
				waitForVblank();
				FlipPage();
			}
		} else if (state == 57) { // Stage 28
			int keepLooping = 1;
			int counter = 0;
			int upPressed = 0;
			int downPressed = 0;
			int playerInvincibility = 0;
			int spiderCooldown = 0;
			int bulletCooldown = 0;
			int laserCooldown = 0;
			int drawPlayer = 1;
			int generation = 1;
			// // Create SPIDER enemies for this stage
			// SPIDER spiders[30];
			// int nextEmptySpider = 0;
			// for (int i = 0; i < 30; i++) { // Initialize them
			// 	spiders[i].arrayPosition = 987654;
			// }
			// // Create BULLET enemies for this stage
			// BULLET bullets[30];
			// int nextEmptyBullet = 0;
			// for (int i = 0; i < 30; i++) {
			// 	bullets[i].arrayPosition = 987654;
			// }
			// Create LASER enemies for this stage
			LASER lasers[5];
			int nextEmptyLaser = 0;
			for (int i = 0; i < 5; i++) {
				lasers[i].arrayPosition = 987654;
			}

			while (keepLooping) {
				CLEAR;

				// if (counter % (5 +  (rand() % 20)) == 0 && spiderCooldown == 0 && generation == 1) {
				// 	SPIDER temp;
				// 	createSpider(&temp, 1, &nextEmptySpider, -1, 3, -1, -1, 0, 244);
				// 	spiders[nextEmptySpider] = temp;
				// 	nextEmptySpider++;
				// 	if (nextEmptySpider >= 30) {
				// 		nextEmptySpider = 0;
				// 	}
				// 	spiderCooldown = 15;
				// }

				// if (counter % (5 +  (rand() % 15)) == 0 && bulletCooldown == 0 && generation == 1) {
				// 	BULLET temp;
				// 	createBullet(&temp, 0, 0, 0, 0, 0, 0, 0, 1, &player);
				// 	bullets[nextEmptyBullet] = temp;
				// 	nextEmptyBullet++;
				// 	if (nextEmptyBullet >= 30) {
				// 		nextEmptyBullet = 0;
				// 	}
				// 	bulletCooldown = 5;
				// }

				if (counter % 70 && laserCooldown == 0 && generation == 1 && counter < 720) {
					LASER temp;
					//int randomize = rand() % 8;
					int sign = rand() % 2;
					if (sign == 0) {
						sign = 2;
					} else {
						sign = -2;
					}
					createLaser(&temp, (rand() % 200), 0, 40, sign, 30, 20, 0); 
					lasers[nextEmptyLaser] = temp;
					nextEmptyLaser++;
					if (nextEmptyLaser >= 5) {
						nextEmptyLaser = 0;
					}
					laserCooldown = 50;
				} 
				if (playerHealth <= 0) {
					keepLooping = 0;
					state = 999;
				}

				// Drawing

				// If player is invulnerable, flash the player to give player notice
				if (playerInvincibility > 0) {
					if (drawPlayer == 1) {
						drawPlayer = 0;
					} else {
						drawPlayer = 1;
					}
				} else {
					drawPlayer = 1;
				}

				// Drawing the objects
				drawBasics(&player, &topBar, &middleBar, &bottomBar, &drawPlayer);
				// updateSpiders(spiders, &playerHealth, &player, &playerInvincibility);
				// updateBullets(bullets, &playerHealth, &player, &playerInvincibility);
				updateLasers(lasers, &playerHealth, &player, &playerInvincibility);
				playerMovement(&player, &upPressed, &downPressed, &topBar, &middleBar, &bottomBar);

				if (KEY_DOWN_NOW(BUTTON_SELECT)) {
					keepLooping = 0;
					state = 0;
				}

				// Cooldown to prevent walls of spiders from appearing
				if (spiderCooldown != 0) {
					spiderCooldown--;
				}

				if (bulletCooldown != 0) {
					bulletCooldown--;
				}

				if (laserCooldown != 0) {
					laserCooldown--;
				}

				// If player gets hit, they get invincibility frames that last 100 loops. This decreases the remaining frames.
				if (playerInvincibility != 0) {
					playerInvincibility--;
				}

				// Displays HP
				char hpDisplay[20];
				sprintf(hpDisplay, "Health: %d", playerHealth);
				drawString4(150, 3, hpDisplay, 250);
				drawRect4(151, 75, 6, playerHealth, 246);
				drawRect4(151, 75 + playerHealth, 6, 100 - playerHealth, 247);

				// MODE 4
				waitForVblank();
				FlipPage();

				counter++;
				if (counter > 800 && counter < 850) {
					generation = 0;
				} else if (counter > 850) {
					keepLooping = 0;
					state = 58;
				}
			}
		} else if (state == 58) { // Story 29
			int keepLooping = 1;
			int counter = 0;
			int upPressed = 0;
			int downPressed = 0;
			int drawPlayer = 1;
			char text[50];
			char printing[50];
			char *textPointer = &text[0];
			int letters = 0;
			while (keepLooping) {
				CLEAR;

				// STORY TEXT HERE
				if (counter < 50) {
					memset(printing, 0, 50);
				}
				if (counter >= 50 && counter < 200) {
					sprintf(text, "There would be a lot less suffering");
					printText(2);
					if (counter == 199) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 200 && counter < 350) {
					sprintf(text, "if you had already died.");
					printText(2);
					if (counter == 349) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 350 && counter < 500) {
					sprintf(text, "Why do you keep going?");
					printText(2);
					if (counter == 349) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 499) {
					keepLooping = 0;
					state = 59;
				}

				if (counter >= 0 && counter < 50) {
					playerHealth++;
					if (playerHealth > 100) {
						playerHealth = 100;
					}
				}

				drawString4(20, 10, printing, 244);
				drawBasics(&player, &topBar, &middleBar, &bottomBar, &drawPlayer);
				playerMovement(&player, &upPressed, &downPressed, &topBar, &middleBar, &bottomBar);

				if (KEY_DOWN_NOW(BUTTON_SELECT)) {
					keepLooping = 0;
					state = 0;
				}



				// Increments counter of the loop, this is basically the timer
				counter++;
				if (counter > 987654) {
					counter = 0;
				}

				// Displays HP
				char hpDisplay[20];
				sprintf(hpDisplay, "Health: %d", playerHealth);
				drawString4(150, 3, hpDisplay, 250);
				drawRect4(151, 75, 6, playerHealth, 246);
				drawRect4(151, 75 + playerHealth, 6, 100 - playerHealth, 247);

				// MODE 4
				waitForVblank();
				FlipPage();
			}
		} else if (state == 59) { // Stage 29
			int keepLooping = 1;
			int counter = 0;
			int upPressed = 0;
			int downPressed = 0;
			int playerInvincibility = 0;
			int spiderCooldown = 0;
			int bulletCooldown = 0;
			int laserCooldown = 0;
			int drawPlayer = 1;
			int generation = 1;
			// // Create SPIDER enemies for this stage
			// SPIDER spiders[30];
			// int nextEmptySpider = 0;
			// for (int i = 0; i < 30; i++) { // Initialize them
			// 	spiders[i].arrayPosition = 987654;
			// }
			// // Create BULLET enemies for this stage
			// BULLET bullets[30];
			// int nextEmptyBullet = 0;
			// for (int i = 0; i < 30; i++) {
			// 	bullets[i].arrayPosition = 987654;
			// }
			// Create LASER enemies for this stage
			LASER lasers[5];
			int nextEmptyLaser = 0;
			for (int i = 0; i < 5; i++) {
				lasers[i].arrayPosition = 987654;
			}

			while (keepLooping) {
				CLEAR;

				// if (counter % (5 +  (rand() % 20)) == 0 && spiderCooldown == 0 && generation == 1) {
				// 	SPIDER temp;
				// 	createSpider(&temp, 1, &nextEmptySpider, -1, 3, -1, -1, 0, 244);
				// 	spiders[nextEmptySpider] = temp;
				// 	nextEmptySpider++;
				// 	if (nextEmptySpider >= 30) {
				// 		nextEmptySpider = 0;
				// 	}
				// 	spiderCooldown = 15;
				// }

				// if (counter % (5 +  (rand() % 15)) == 0 && bulletCooldown == 0 && generation == 1) {
				// 	BULLET temp;
				// 	createBullet(&temp, 0, 0, 0, 0, 0, 0, 0, 1, &player);
				// 	bullets[nextEmptyBullet] = temp;
				// 	nextEmptyBullet++;
				// 	if (nextEmptyBullet >= 30) {
				// 		nextEmptyBullet = 0;
				// 	}
				// 	bulletCooldown = 5;
				// }

				if (counter % 70 && laserCooldown == 0 && generation == 1 && counter < 720) {
					LASER temp;
					int randomize = rand() % 8;
					int sign = rand() % 2;
					if (sign == 0) {
						sign = 1;
					} else {
						sign = -1;
					}

					if (randomize >= 3) {
						createLaser(&temp, (rand() % 200), 0, 40, sign * 2, 30, 20, 0); 
					} else {
						createLaser(&temp, 0, (rand() % 120), 40, sign, 30, 10, 1);
					}
					lasers[nextEmptyLaser] = temp;
					nextEmptyLaser++;
					if (nextEmptyLaser >= 5) {
						nextEmptyLaser = 0;
					}
					laserCooldown = 50;
				} 
				if (playerHealth <= 0) {
					keepLooping = 0;
					state = 999;
				}

				// Drawing

				// If player is invulnerable, flash the player to give player notice
				if (playerInvincibility > 0) {
					if (drawPlayer == 1) {
						drawPlayer = 0;
					} else {
						drawPlayer = 1;
					}
				} else {
					drawPlayer = 1;
				}

				// Drawing the objects
				drawBasics(&player, &topBar, &middleBar, &bottomBar, &drawPlayer);
				// updateSpiders(spiders, &playerHealth, &player, &playerInvincibility);
				// updateBullets(bullets, &playerHealth, &player, &playerInvincibility);
				updateLasers(lasers, &playerHealth, &player, &playerInvincibility);
				playerMovement(&player, &upPressed, &downPressed, &topBar, &middleBar, &bottomBar);

				if (KEY_DOWN_NOW(BUTTON_SELECT)) {
					keepLooping = 0;
					state = 0;
				}

				// Cooldown to prevent walls of spiders from appearing
				if (spiderCooldown != 0) {
					spiderCooldown--;
				}

				if (bulletCooldown != 0) {
					bulletCooldown--;
				}

				if (laserCooldown != 0) {
					laserCooldown--;
				}

				// If player gets hit, they get invincibility frames that last 100 loops. This decreases the remaining frames.
				if (playerInvincibility != 0) {
					playerInvincibility--;
				}

				// Displays HP
				char hpDisplay[20];
				sprintf(hpDisplay, "Health: %d", playerHealth);
				drawString4(150, 3, hpDisplay, 250);
				drawRect4(151, 75, 6, playerHealth, 246);
				drawRect4(151, 75 + playerHealth, 6, 100 - playerHealth, 247);

				// MODE 4
				waitForVblank();
				FlipPage();

				counter++;
				if (counter > 800 && counter < 850) {
					generation = 0;
				} else if (counter > 850) {
					keepLooping = 0;
					state = 60;
				}
			}
		} else if (state == 60) { // Story 30
			int keepLooping = 1;
			int counter = 0;
			int upPressed = 0;
			int downPressed = 0;
			int drawPlayer = 1;
			char text[50];
			char printing[50];
			char *textPointer = &text[0];
			int letters = 0;
			while (keepLooping) {
				CLEAR;

				// STORY TEXT HERE
				if (counter < 50) {
					memset(printing, 0, 50);
				}
				if (counter >= 50 && counter < 200) {
					sprintf(text, "...");
					printText(2);
					if (counter == 199) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 200 && counter < 350) {
					sprintf(text, "Surprised?");
					printText(2);
					if (counter == 349) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 350 && counter < 500) {
					sprintf(text, "...I thought as much.");
					printText(2);
					if (counter == 349) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 499) {
					keepLooping = 0;
					state = 61;
				}

				if (counter >= 0 && counter < 80) {
					playerHealth++;
					if (playerHealth >= 100) {
						playerHealth = 100;
					}
				}
				drawString4(20, 10, printing, 244);
				drawBasics(&player, &topBar, &middleBar, &bottomBar, &drawPlayer);
				playerMovement(&player, &upPressed, &downPressed, &topBar, &middleBar, &bottomBar);

				if (KEY_DOWN_NOW(BUTTON_SELECT)) {
					keepLooping = 0;
					state = 0;
				}

				// Increments counter of the loop, this is basically the timer
				counter++;
				if (counter > 987654) {
					counter = 0;
				}

				// Displays HP
				char hpDisplay[20];
				sprintf(hpDisplay, "Health: %d", playerHealth);
				drawString4(150, 3, hpDisplay, 250);
				drawRect4(151, 75, 6, playerHealth, 246);
				drawRect4(151, 75 + playerHealth, 6, 100 - playerHealth, 247);

				// MODE 4
				waitForVblank();
				FlipPage();
			}
		} else if (state == 61) { // Stage 30
			int keepLooping = 1;
			int counter = 0;
			int upPressed = 0;
			int downPressed = 0;
			int playerInvincibility = 0;
			int spiderCooldown = 0;
			int bulletCooldown = 0;
			int laserCooldown = 0;
			int drawPlayer = 1;
			int generation = 1;
			// // Create SPIDER enemies for this stage
			// SPIDER spiders[30];
			// int nextEmptySpider = 0;
			// for (int i = 0; i < 30; i++) { // Initialize them
			// 	spiders[i].arrayPosition = 987654;
			// }
			// // Create BULLET enemies for this stage
			// BULLET bullets[30];
			// int nextEmptyBullet = 0;
			// for (int i = 0; i < 30; i++) {
			// 	bullets[i].arrayPosition = 987654;
			// }
			// Create LASER enemies for this stage
			LASER lasers[5];
			int nextEmptyLaser = 0;
			for (int i = 0; i < 5; i++) {
				lasers[i].arrayPosition = 987654;
			}

			while (keepLooping) {
				CLEAR;

				// if (counter % (5 +  (rand() % 20)) == 0 && spiderCooldown == 0 && generation == 1) {
				// 	SPIDER temp;
				// 	createSpider(&temp, 1, &nextEmptySpider, -1, 3, -1, -1, 0, 244);
				// 	spiders[nextEmptySpider] = temp;
				// 	nextEmptySpider++;
				// 	if (nextEmptySpider >= 30) {
				// 		nextEmptySpider = 0;
				// 	}
				// 	spiderCooldown = 15;
				// }

				// if (counter % (5 +  (rand() % 15)) == 0 && bulletCooldown == 0 && generation == 1) {
				// 	BULLET temp;
				// 	createBullet(&temp, 0, 0, 0, 0, 0, 0, 0, 1, &player);
				// 	bullets[nextEmptyBullet] = temp;
				// 	nextEmptyBullet++;
				// 	if (nextEmptyBullet >= 30) {
				// 		nextEmptyBullet = 0;
				// 	}
				// 	bulletCooldown = 5;
				// }

				if (counter % 70 && laserCooldown == 0 && generation == 1 && counter < 720) {
					LASER temp;
					int randomize = rand() % 8;

					if (randomize >= 3) {
						createLaser(&temp, (rand() % 200), 0, 40, 0, 40, 20, 0); 
					} else {
						createLaser(&temp, (rand() % 200), 0, 40, 0, 40, 20, 0); 
					}
					lasers[nextEmptyLaser] = temp;
					nextEmptyLaser++;
					if (nextEmptyLaser >= 5) {
						nextEmptyLaser = 0;
					}
				} 

				if (counter % 70 && laserCooldown == 0 && generation == 1 && counter < 720) {
					LASER temp;
					int randomize = rand() % 8;

					if (randomize >= 3) {
						createLaser(&temp, (rand() % 200), 0, 40, 0, 40, 20, 0); 
					} else {
						createLaser(&temp, (rand() % 200), 0, 40, 0, 40, 20, 0); 
					}
					lasers[nextEmptyLaser] = temp;
					nextEmptyLaser++;
					if (nextEmptyLaser >= 5) {
						nextEmptyLaser = 0;
					}
					laserCooldown = 60;
				} 

				if (playerHealth <= 0) {
					keepLooping = 0;
					state = 999;
				}

				// Drawing

				// If player is invulnerable, flash the player to give player notice
				if (playerInvincibility > 0) {
					if (drawPlayer == 1) {
						drawPlayer = 0;
					} else {
						drawPlayer = 1;
					}
				} else {
					drawPlayer = 1;
				}

				// Drawing the objects
				drawBasics(&player, &topBar, &middleBar, &bottomBar, &drawPlayer);
				// updateSpiders(spiders, &playerHealth, &player, &playerInvincibility);
				// updateBullets(bullets, &playerHealth, &player, &playerInvincibility);
				updateLasers(lasers, &playerHealth, &player, &playerInvincibility);
				playerMovement(&player, &upPressed, &downPressed, &topBar, &middleBar, &bottomBar);

				if (KEY_DOWN_NOW(BUTTON_SELECT)) {
					keepLooping = 0;
					state = 0;
				}

				// Cooldown to prevent walls of spiders from appearing
				if (spiderCooldown != 0) {
					spiderCooldown--;
				}

				if (bulletCooldown != 0) {
					bulletCooldown--;
				}

				if (laserCooldown != 0) {
					laserCooldown--;
				}

				// If player gets hit, they get invincibility frames that last 100 loops. This decreases the remaining frames.
				if (playerInvincibility != 0) {
					playerInvincibility--;
				}

				// Displays HP
				char hpDisplay[20];
				sprintf(hpDisplay, "Health: %d", playerHealth);
				drawString4(150, 3, hpDisplay, 250);
				drawRect4(151, 75, 6, playerHealth, 246);
				drawRect4(151, 75 + playerHealth, 6, 100 - playerHealth, 247);

				// MODE 4
				waitForVblank();
				FlipPage();

				counter++;
				if (counter > 800 && counter < 850) {
					generation = 0;
				} else if (counter > 850) {
					keepLooping = 0;
					state = 62;
				}
			}
		} else if (state == 62) { // Story 31
			int keepLooping = 1;
			int counter = 0;
			int upPressed = 0;
			int downPressed = 0;
			int drawPlayer = 1;
			char text[50];
			char printing[50];
			char *textPointer = &text[0];
			int letters = 0;
			while (keepLooping) {
				CLEAR;

				// STORY TEXT HERE
				if (counter < 50) {
					memset(printing, 0, 50);
				}
				if (counter >= 50 && counter < 200) {
					sprintf(text, "There is no end.");
					printText(2);
					if (counter == 199) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 200 && counter < 350) {
					sprintf(text, "You cannot pass.");
					printText(2);
					if (counter == 349) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 350 && counter < 500) {
					sprintf(text, "I can't allow that.");
					printText(2);
					if (counter == 349) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 499) {
					keepLooping = 0;
					state = 63;
				}

				if (counter >= 0 && counter < 50) {
					playerHealth++;
					if (playerHealth > 100) {
						playerHealth = 100;
					}
				}

				drawString4(20, 10, printing, 244);
				drawBasics(&player, &topBar, &middleBar, &bottomBar, &drawPlayer);
				playerMovement(&player, &upPressed, &downPressed, &topBar, &middleBar, &bottomBar);

				if (KEY_DOWN_NOW(BUTTON_SELECT)) {
					keepLooping = 0;
					state = 0;
				}

				// Increments counter of the loop, this is basically the timer
				counter++;
				if (counter > 987654) {
					counter = 0;
				}

				// Displays HP
				char hpDisplay[20];
				sprintf(hpDisplay, "Health: %d", playerHealth);
				drawString4(150, 3, hpDisplay, 250);
				drawRect4(151, 75, 6, playerHealth, 246);
				drawRect4(151, 75 + playerHealth, 6, 100 - playerHealth, 247);

				// MODE 4
				waitForVblank();
				FlipPage();
			}
		} else if (state == 63) { // Stage 31
			int keepLooping = 1;
			int counter = 0;
			int upPressed = 0;
			int downPressed = 0;
			int playerInvincibility = 0;
			int spiderCooldown = 0;
			int bulletCooldown = 0;
			int laserCooldown = 0;
			int drawPlayer = 1;
			int generation = 1;
			// // Create SPIDER enemies for this stage
			// SPIDER spiders[30];
			// int nextEmptySpider = 0;
			// for (int i = 0; i < 30; i++) { // Initialize them
			// 	spiders[i].arrayPosition = 987654;
			// }
			// // Create BULLET enemies for this stage
			// BULLET bullets[30];
			// int nextEmptyBullet = 0;
			// for (int i = 0; i < 30; i++) {
			// 	bullets[i].arrayPosition = 987654;
			// }
			// Create LASER enemies for this stage
			LASER lasers[5];
			int nextEmptyLaser = 0;
			for (int i = 0; i < 5; i++) {
				lasers[i].arrayPosition = 987654;
			}

			while (keepLooping) {
				CLEAR;

				// if (counter % (5 +  (rand() % 20)) == 0 && spiderCooldown == 0 && generation == 1) {
				// 	SPIDER temp;
				// 	createSpider(&temp, 1, &nextEmptySpider, -1, 3, -1, -1, 0, 244);
				// 	spiders[nextEmptySpider] = temp;
				// 	nextEmptySpider++;
				// 	if (nextEmptySpider >= 30) {
				// 		nextEmptySpider = 0;
				// 	}
				// 	spiderCooldown = 15;
				// }

				// if (counter % (5 +  (rand() % 15)) == 0 && bulletCooldown == 0 && generation == 1) {
				// 	BULLET temp;
				// 	createBullet(&temp, 0, 0, 0, 0, 0, 0, 0, 1, &player);
				// 	bullets[nextEmptyBullet] = temp;
				// 	nextEmptyBullet++;
				// 	if (nextEmptyBullet >= 30) {
				// 		nextEmptyBullet = 0;
				// 	}
				// 	bulletCooldown = 5;
				// }

				if (counter % 70 && laserCooldown == 0 && generation == 1 && counter < 750) {
					LASER temp;
					int randomize = rand() % 8;

					if (randomize >= 3) {
						createLaser(&temp, (rand() % 190), 0, 50, 0, 40, 20, 0); 
					} else {
						createLaser(&temp, (rand() % 190), 0, 50, 0, 40, 20, 0); 
					}
					lasers[nextEmptyLaser] = temp;
					nextEmptyLaser++;
					if (nextEmptyLaser >= 5) {
						nextEmptyLaser = 0;
					}
				} 

				if (counter % 70 && laserCooldown == 0 && generation == 1 && counter < 750) {
					LASER temp;
					int randomize = rand() % 8;

					if (randomize >= 3) {
						createLaser(&temp, (rand() % 190), 0, 50, 0, 40, 20, 0); 
					} else {
						createLaser(&temp, (rand() % 190), 0, 50, 0, 40, 20, 0); 
					}
					lasers[nextEmptyLaser] = temp;
					nextEmptyLaser++;
					if (nextEmptyLaser >= 5) {
						nextEmptyLaser = 0;
					}
					laserCooldown = 60;
				} 

				if (playerHealth <= 0) {
					keepLooping = 0;
					state = 999;
				}

				// Drawing

				// If player is invulnerable, flash the player to give player notice
				if (playerInvincibility > 0) {
					if (drawPlayer == 1) {
						drawPlayer = 0;
					} else {
						drawPlayer = 1;
					}
				} else {
					drawPlayer = 1;
				}

				// Drawing the objects
				drawBasics(&player, &topBar, &middleBar, &bottomBar, &drawPlayer);
				// updateSpiders(spiders, &playerHealth, &player, &playerInvincibility);
				// updateBullets(bullets, &playerHealth, &player, &playerInvincibility);
				updateLasers(lasers, &playerHealth, &player, &playerInvincibility);
				playerMovement(&player, &upPressed, &downPressed, &topBar, &middleBar, &bottomBar);

				if (KEY_DOWN_NOW(BUTTON_SELECT)) {
					keepLooping = 0;
					state = 0;
				}

				// Cooldown to prevent walls of spiders from appearing
				if (spiderCooldown != 0) {
					spiderCooldown--;
				}

				if (bulletCooldown != 0) {
					bulletCooldown--;
				}

				if (laserCooldown != 0) {
					laserCooldown--;
				}

				// If player gets hit, they get invincibility frames that last 100 loops. This decreases the remaining frames.
				if (playerInvincibility != 0) {
					playerInvincibility--;
				}

				// Displays HP
				char hpDisplay[20];
				sprintf(hpDisplay, "Health: %d", playerHealth);
				drawString4(150, 3, hpDisplay, 250);
				drawRect4(151, 75, 6, playerHealth, 246);
				drawRect4(151, 75 + playerHealth, 6, 100 - playerHealth, 247);

				// MODE 4
				waitForVblank();
				FlipPage();

				counter++;
				if (counter > 800 && counter < 850) {
					generation = 0;
				} else if (counter > 850) {
					keepLooping = 0;
					state = 64;
				}
			}
		} else if (state == 64) { // Story 32
			int keepLooping = 1;
			int counter = 0;
			int upPressed = 0;
			int downPressed = 0;
			int drawPlayer = 1;
			char text[50];
			char printing[50];
			char *textPointer = &text[0];
			int letters = 0;
			while (keepLooping) {
				CLEAR;

				// STORY TEXT HERE
				if (counter < 50) {
					memset(printing, 0, 50);
				}
				if (counter >= 50 && counter < 200) {
					sprintf(text, "I cannot...");
					printText(2);
					if (counter == 199) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 200 && counter < 350) {
					sprintf(text, "The world is far too cruel.");
					printText(2);
					if (counter == 349) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 350 && counter < 500) {
					sprintf(text, "It's in your own good to die.");
					printText(2);
					if (counter == 349) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 499) {
					keepLooping = 0;
					state = 65;
				}

				drawString4(20, 10, printing, 244);
				drawBasics(&player, &topBar, &middleBar, &bottomBar, &drawPlayer);
				playerMovement(&player, &upPressed, &downPressed, &topBar, &middleBar, &bottomBar);

				if (KEY_DOWN_NOW(BUTTON_SELECT)) {
					keepLooping = 0;
					state = 0;
				}

				// Increments counter of the loop, this is basically the timer
				counter++;
				if (counter > 987654) {
					counter = 0;
				}

				// Displays HP
				char hpDisplay[20];
				sprintf(hpDisplay, "Health: %d", playerHealth);
				drawString4(150, 3, hpDisplay, 250);
				drawRect4(151, 75, 6, playerHealth, 246);
				drawRect4(151, 75 + playerHealth, 6, 100 - playerHealth, 247);

				// MODE 4
				waitForVblank();
				FlipPage();
			}
		} else if (state == 65) { // Stage 32
			int keepLooping = 1;
			int counter = 0;
			int upPressed = 0;
			int downPressed = 0;
			int playerInvincibility = 0;
			int spiderCooldown = 0;
			int bulletCooldown = 0;
			int laserCooldown = 0;
			int drawPlayer = 1;
			int generation = 1;
			// // Create SPIDER enemies for this stage
			// SPIDER spiders[30];
			// int nextEmptySpider = 0;
			// for (int i = 0; i < 30; i++) { // Initialize them
			// 	spiders[i].arrayPosition = 987654;
			// }
			// // Create BULLET enemies for this stage
			// BULLET bullets[30];
			// int nextEmptyBullet = 0;
			// for (int i = 0; i < 30; i++) {
			// 	bullets[i].arrayPosition = 987654;
			// }
			// Create LASER enemies for this stage
			LASER lasers[5];
			int nextEmptyLaser = 0;
			for (int i = 0; i < 5; i++) {
				lasers[i].arrayPosition = 987654;
			}

			while (keepLooping) {
				CLEAR;

				// if (counter % (5 +  (rand() % 20)) == 0 && spiderCooldown == 0 && generation == 1) {
				// 	SPIDER temp;
				// 	createSpider(&temp, 1, &nextEmptySpider, -1, 3, -1, -1, 0, 244);
				// 	spiders[nextEmptySpider] = temp;
				// 	nextEmptySpider++;
				// 	if (nextEmptySpider >= 30) {
				// 		nextEmptySpider = 0;
				// 	}
				// 	spiderCooldown = 15;
				// }

				// if (counter % (5 +  (rand() % 15)) == 0 && bulletCooldown == 0 && generation == 1) {
				// 	BULLET temp;
				// 	createBullet(&temp, 0, 0, 0, 0, 0, 0, 0, 1, &player);
				// 	bullets[nextEmptyBullet] = temp;
				// 	nextEmptyBullet++;
				// 	if (nextEmptyBullet >= 30) {
				// 		nextEmptyBullet = 0;
				// 	}
				// 	bulletCooldown = 5;
				// }

				if (counter % 70 && laserCooldown == 0 && generation == 1 && counter < 780) {
					LASER temp;
					int randomize = rand() % 8;

					if (randomize >= 3) {
						createLaser(&temp, (rand() % 190), 0, 50, 0, 50, 20, 0); 
					} else {
						createLaser(&temp, (rand() % 190), 0, 50, 0, 50, 20, 0); 
					}
					lasers[nextEmptyLaser] = temp;
					nextEmptyLaser++;
					if (nextEmptyLaser >= 5) {
						nextEmptyLaser = 0;
					}
				} 

				if (counter % 70 && laserCooldown == 0 && generation == 1 && counter < 780) {
					LASER temp;
					int randomize = rand() % 8;

					if (randomize >= 3) {
						createLaser(&temp, 0, (rand() % 110), 50, 0, 50, 20, 1); 
					} else {
						createLaser(&temp, 0, (rand() % 110), 50, 0, 50, 20, 1); 
					}
					lasers[nextEmptyLaser] = temp;
					nextEmptyLaser++;
					if (nextEmptyLaser >= 5) {
						nextEmptyLaser = 0;
					}
					laserCooldown = 70;
				} 

				if (playerHealth <= 0) {
					keepLooping = 0;
					state = 999;
				}

				// Drawing

				// If player is invulnerable, flash the player to give player notice
				if (playerInvincibility > 0) {
					if (drawPlayer == 1) {
						drawPlayer = 0;
					} else {
						drawPlayer = 1;
					}
				} else {
					drawPlayer = 1;
				}

				// Drawing the objects
				drawBasics(&player, &topBar, &middleBar, &bottomBar, &drawPlayer);
				// updateSpiders(spiders, &playerHealth, &player, &playerInvincibility);
				// updateBullets(bullets, &playerHealth, &player, &playerInvincibility);
				updateLasers(lasers, &playerHealth, &player, &playerInvincibility);
				playerMovement(&player, &upPressed, &downPressed, &topBar, &middleBar, &bottomBar);

				if (KEY_DOWN_NOW(BUTTON_SELECT)) {
					keepLooping = 0;
					state = 0;
				}

				// Cooldown to prevent walls of spiders from appearing
				if (spiderCooldown != 0) {
					spiderCooldown--;
				}

				if (bulletCooldown != 0) {
					bulletCooldown--;
				}

				if (laserCooldown != 0) {
					laserCooldown--;
				}

				// If player gets hit, they get invincibility frames that last 100 loops. This decreases the remaining frames.
				if (playerInvincibility != 0) {
					playerInvincibility--;
				}

				// Displays HP
				char hpDisplay[20];
				sprintf(hpDisplay, "Health: %d", playerHealth);
				drawString4(150, 3, hpDisplay, 250);
				drawRect4(151, 75, 6, playerHealth, 246);
				drawRect4(151, 75 + playerHealth, 6, 100 - playerHealth, 247);

				// MODE 4
				waitForVblank();
				FlipPage();

				counter++;
				if (counter > 800 && counter < 850) {
					generation = 0;
				} else if (counter > 850) {
					keepLooping = 0;
					state = 66;
				}
			}
		} else if (state == 66) { // Story 33
			int keepLooping = 1;
			int counter = 0;
			int upPressed = 0;
			int downPressed = 0;
			int drawPlayer = 1;
			char text[50];
			char printing[50];
			char *textPointer = &text[0];
			int letters = 0;
			while (keepLooping) {
				CLEAR;

				// STORY TEXT HERE
				if (counter < 50) {
					memset(printing, 0, 50);
				}
				if (counter >= 50 && counter < 200) {
					sprintf(text, "Don't you believe me?");
					printText(2);
					if (counter == 199) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 200 && counter < 350) {
					sprintf(text, "What's so bad about dying?");
					printText(2);
					if (counter == 349) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 350 && counter < 500) {
					sprintf(text, "It'll end the suffering.");
					printText(2);
					if (counter == 349) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 499) {
					keepLooping = 0;
					state = 67;
				}

				if (counter >= 0 && counter < 50) {
					playerHealth++;
					if (playerHealth > 100) {
						playerHealth = 100;
					}
				}

				drawString4(20, 10, printing, 244);
				drawBasics(&player, &topBar, &middleBar, &bottomBar, &drawPlayer);
				playerMovement(&player, &upPressed, &downPressed, &topBar, &middleBar, &bottomBar);

				if (KEY_DOWN_NOW(BUTTON_SELECT)) {
					keepLooping = 0;
					state = 0;
				}

				// Increments counter of the loop, this is basically the timer
				counter++;
				if (counter > 987654) {
					counter = 0;
				}

				// Displays HP
				char hpDisplay[20];
				sprintf(hpDisplay, "Health: %d", playerHealth);
				drawString4(150, 3, hpDisplay, 250);
				drawRect4(151, 75, 6, playerHealth, 246);
				drawRect4(151, 75 + playerHealth, 6, 100 - playerHealth, 247);

				// MODE 4
				waitForVblank();
				FlipPage();
			}
		} else if (state == 67) { // Stage 33
			int keepLooping = 1;
			int counter = 0;
			int upPressed = 0;
			int downPressed = 0;
			int playerInvincibility = 0;
			int spiderCooldown = 0;
			int bulletCooldown = 0;
			int laserCooldown = 0;
			int drawPlayer = 1;
			int generation = 1;
			// // Create SPIDER enemies for this stage
			// SPIDER spiders[30];
			// int nextEmptySpider = 0;
			// for (int i = 0; i < 30; i++) { // Initialize them
			// 	spiders[i].arrayPosition = 987654;
			// }
			// // Create BULLET enemies for this stage
			// BULLET bullets[30];
			// int nextEmptyBullet = 0;
			// for (int i = 0; i < 30; i++) {
			// 	bullets[i].arrayPosition = 987654;
			// }
			// Create LASER enemies for this stage
			LASER lasers[5];
			int nextEmptyLaser = 0;
			for (int i = 0; i < 5; i++) {
				lasers[i].arrayPosition = 987654;
			}

			while (keepLooping) {
				CLEAR;

				// if (counter % (5 +  (rand() % 20)) == 0 && spiderCooldown == 0 && generation == 1) {
				// 	SPIDER temp;
				// 	createSpider(&temp, 1, &nextEmptySpider, -1, 3, -1, -1, 0, 244);
				// 	spiders[nextEmptySpider] = temp;
				// 	nextEmptySpider++;
				// 	if (nextEmptySpider >= 30) {
				// 		nextEmptySpider = 0;
				// 	}
				// 	spiderCooldown = 15;
				// }

				// if (counter % (5 +  (rand() % 15)) == 0 && bulletCooldown == 0 && generation == 1) {
				// 	BULLET temp;
				// 	createBullet(&temp, 0, 0, 0, 0, 0, 0, 0, 1, &player);
				// 	bullets[nextEmptyBullet] = temp;
				// 	nextEmptyBullet++;
				// 	if (nextEmptyBullet >= 30) {
				// 		nextEmptyBullet = 0;
				// 	}
				// 	bulletCooldown = 5;
				// }

				if (counter % 70 && laserCooldown == 0 && generation == 1) {
					LASER temp;
					int randomize = rand() % 8;

					if (randomize >= 3) {
						createLaser(&temp, (rand() % 190), 0, 50, 0, 40, 30, 0); 
					} else {
						createLaser(&temp, (rand() % 190), 0, 50, 0, 40, 30, 0); 
					}
					lasers[nextEmptyLaser] = temp;
					nextEmptyLaser++;
					if (nextEmptyLaser >= 5) {
						nextEmptyLaser = 0;
					}
				} 

				if (counter % 70 && laserCooldown == 0 && generation == 1) {
					LASER temp;
					int randomize = rand() % 8;

					if (randomize >= 3) {
						createLaser(&temp, 0, (rand() % 110), 50, 0, 40, 30, 1); 
					} else {
						createLaser(&temp, 0, (rand() % 110), 50, 0, 40, 30, 1); 
					}
					lasers[nextEmptyLaser] = temp;
					nextEmptyLaser++;
					if (nextEmptyLaser >= 5) {
						nextEmptyLaser = 0;
					}
					laserCooldown = 70;
				} 

				if (playerHealth <= 0) {
					keepLooping = 0;
					state = 999;
				}

				// Drawing

				// If player is invulnerable, flash the player to give player notice
				if (playerInvincibility > 0) {
					if (drawPlayer == 1) {
						drawPlayer = 0;
					} else {
						drawPlayer = 1;
					}
				} else {
					drawPlayer = 1;
				}

				// Drawing the objects
				drawBasics(&player, &topBar, &middleBar, &bottomBar, &drawPlayer);
				// updateSpiders(spiders, &playerHealth, &player, &playerInvincibility);
				// updateBullets(bullets, &playerHealth, &player, &playerInvincibility);
				updateLasers(lasers, &playerHealth, &player, &playerInvincibility);
				playerMovement(&player, &upPressed, &downPressed, &topBar, &middleBar, &bottomBar);

				if (KEY_DOWN_NOW(BUTTON_SELECT)) {
					keepLooping = 0;
					state = 0;
				}

				// Cooldown to prevent walls of spiders from appearing
				if (spiderCooldown != 0) {
					spiderCooldown--;
				}

				if (bulletCooldown != 0) {
					bulletCooldown--;
				}

				if (laserCooldown != 0) {
					laserCooldown--;
				}

				// If player gets hit, they get invincibility frames that last 100 loops. This decreases the remaining frames.
				if (playerInvincibility != 0) {
					playerInvincibility--;
				}

				// Displays HP
				char hpDisplay[20];
				sprintf(hpDisplay, "Health: %d", playerHealth);
				drawString4(150, 3, hpDisplay, 250);
				drawRect4(151, 75, 6, playerHealth, 246);
				drawRect4(151, 75 + playerHealth, 6, 100 - playerHealth, 247);

				// MODE 4
				waitForVblank();
				FlipPage();

				counter++;
				if (counter > 1200 && counter < 1300) {
					generation = 0;
				} else if (counter > 1300) {
					keepLooping = 0;
					state = 68;
				}
			}
		} else if (state == 68) { // Story 34
			int keepLooping = 1;
			int counter = 0;
			int upPressed = 0;
			int downPressed = 0;
			int drawPlayer = 1;
			char text[50];
			char printing[50];
			char *textPointer = &text[0];
			int letters = 0;
			while (keepLooping) {
				CLEAR;

				// STORY TEXT HERE
				if (counter < 50) {
					memset(printing, 0, 50);
				}
				if (counter >= 50 && counter < 200) {
					sprintf(text, "Stop trying.");
					printText(2);
					if (counter == 199) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 200 && counter < 350) {
					sprintf(text, "Why are you wasting your energy?");
					printText(2);
					if (counter == 349) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 350 && counter < 500) {
					sprintf(text, "This is all for NOTHING.");
					printText(2);
					if (counter == 349) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 499) {
					keepLooping = 0;
					state = 69;
				}

				if (counter >= 0 && counter < 100) {
					playerHealth++;
					if (playerHealth > 100) {
						playerHealth = 100;
					}
				}

				drawString4(20, 10, printing, 244);
				drawBasics(&player, &topBar, &middleBar, &bottomBar, &drawPlayer);
				playerMovement(&player, &upPressed, &downPressed, &topBar, &middleBar, &bottomBar);

				if (KEY_DOWN_NOW(BUTTON_SELECT)) {
					keepLooping = 0;
					state = 0;
				}

				// Increments counter of the loop, this is basically the timer
				counter++;
				if (counter > 987654) {
					counter = 0;
				}

				// Displays HP
				char hpDisplay[20];
				sprintf(hpDisplay, "Health: %d", playerHealth);
				drawString4(150, 3, hpDisplay, 250);
				drawRect4(151, 75, 6, playerHealth, 246);
				drawRect4(151, 75 + playerHealth, 6, 100 - playerHealth, 247);

				// MODE 4
				waitForVblank();
				FlipPage();
			}
		} else if (state == 69) { // Stage 34
			int keepLooping = 1;
			int counter = 0;
			int upPressed = 0;
			int downPressed = 0;
			int playerInvincibility = 0;
			int spiderCooldown = 0;
			int bulletCooldown = 0;
			int drawPlayer = 1;
			int generation = 1;
			int generate = 0;
			int generateGap = 75;
			int patternCount = 0;
			int pattern = 0;
			int randomness = 0;
			int laserCooldown = 0;
			// Create SPIDER enemies for this stage
			SPIDER spiders[30];
			int nextEmptySpider = 0;
			for (int i = 0; i < 30; i++) { // Initialize them
				spiders[i].arrayPosition = 987654;
			}

			// Create BULLET enemies for this stage
			// BULLET bullets[30];
			// int nextEmptyBullet = 0;
			// for (int i = 0; i < 30; i++) {
			// 	bullets[i].arrayPosition = 987654;
			// }

			// Create LASER enemies for this stage
			LASER lasers[5];
			int nextEmptyLaser = 0;
			for (int i = 0; i < 5; i++) {
				lasers[i].arrayPosition = 987654;
			}

			while (keepLooping) {
				CLEAR;

				if (laserCooldown == 0 && generation == 1) {
					LASER temp;
					int randomize = rand() % 8;

					if (randomize >= 3) {
						createLaser(&temp, (rand() % 190), 0, 50, 0, 50, 20, 0); 
					} else {
						createLaser(&temp, (rand() % 190), 0, 50, 0, 50, 20, 0); 
					}
					lasers[nextEmptyLaser] = temp;
					nextEmptyLaser++;
					if (nextEmptyLaser >= 5) {
						nextEmptyLaser = 0;
					}
					laserCooldown = 100;
				}

				if (counter == generate && generation == 1) {
					if (pattern == 0) {
						SPIDER temp;
						createSpider(&temp, 1, &nextEmptySpider, 0, 3, 0, 45, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 0, 3, 0, 75, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 1, 3, 230, 105, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
					} else if (pattern == 1) {
						SPIDER temp;
						createSpider(&temp, 1, &nextEmptySpider, 0, 3, 0, 45, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 0, 3, 0, 105, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 1, 3, 230, 75, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
					} else if (pattern == 2) {
						SPIDER temp;
						createSpider(&temp, 1, &nextEmptySpider, 0, 3, 0, 75, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 0, 3, 0, 105, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 1, 3, 230, 45, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
					} else if (pattern == 3) {
						SPIDER temp;
						createSpider(&temp, 1, &nextEmptySpider, 1, 3, 230, 45, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 1, 3, 230, 75, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 0, 3, 0, 105, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
					} else if (pattern == 4) {
						SPIDER temp;
						createSpider(&temp, 1, &nextEmptySpider, 1, 3, 230, 45, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 1, 3, 230, 105, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 0, 3, 0, 75, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
					} else if (pattern == 5) {
						SPIDER temp;
						createSpider(&temp, 1, &nextEmptySpider, 1, 3, 230, 75, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 1, 3, 230, 105, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 0, 3, 0, 45, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
					}
					generate = generate + generateGap + (rand() % 15);
					patternCount++;
					if (patternCount == 3 + randomness) {
						pattern = rand() % 6;
						randomness = rand() % 2;
						patternCount = 0;
					}

				}

				// if (counter % (10 + (rand() % 25)) == 0 && bulletCooldown == 0 && generation == 1) {
				// 	BULLET temp;
				// 	createBullet(&temp, 0, 0, 0, 0, 0, 0, 0, 0, &player);
				// 	bullets[nextEmptyBullet] = temp;
				// 	nextEmptyBullet++;
				// 	if (nextEmptyBullet >= 30) {
				// 		nextEmptyBullet = 0;
				// 	}
				// 	bulletCooldown = 20;
				// }

				if (playerHealth <= 0) {
					keepLooping = 0;
					state = 999;
				}

				// Drawing

				// If player is invulnerable, flash the player to give player notice
				if (playerInvincibility > 0) {
					if (drawPlayer == 1) {
						drawPlayer = 0;
					} else {
						drawPlayer = 1;
					}
				} else {
					drawPlayer = 1;
				}

				// Drawing the objects
				drawBasics(&player, &topBar, &middleBar, &bottomBar, &drawPlayer);
				updateSpiders(spiders, &playerHealth, &player, &playerInvincibility);
				// updateBullets(bullets, &playerHealth, &player, &playerInvincibility);
				updateLasers(lasers, &playerHealth, &player, &playerInvincibility);
				playerMovement(&player, &upPressed, &downPressed, &topBar, &middleBar, &bottomBar);

				if (KEY_DOWN_NOW(BUTTON_SELECT)) {
					keepLooping = 0;
					state = 0;
				}

				// Cooldown to prevent walls of spiders from appearing
				if (spiderCooldown != 0) {
					spiderCooldown--;
				}

				if (bulletCooldown != 0) {
					bulletCooldown--;
				}

				if (laserCooldown != 0) {
					laserCooldown--;
				}

				// If player gets hit, they get invincibility frames that last 100 loops. This decreases the remaining frames.
				if (playerInvincibility != 0) {
					playerInvincibility--;
				}

				// Displays HP
				char hpDisplay[20];
				sprintf(hpDisplay, "Health: %d", playerHealth);
				drawString4(150, 3, hpDisplay, 250);
				drawRect4(151, 75, 6, playerHealth, 246);
				drawRect4(151, 75 + playerHealth, 6, 100 - playerHealth, 247);

				// MODE 4
				waitForVblank();
				FlipPage();

				counter++;
				if (counter > 1000 && counter < 1150) {
					generation = 0;
				} else if (counter > 1150) {
					keepLooping = 0;
					state = 70;
				}
			}
		} else if (state == 70) { // Story 35
			int keepLooping = 1;
			int counter = 0;
			int upPressed = 0;
			int downPressed = 0;
			int drawPlayer = 1;
			char text[50];
			char printing[50];
			char *textPointer = &text[0];
			int letters = 0;
			while (keepLooping) {
				CLEAR;

				// STORY TEXT HERE
				if (counter < 50) {
					memset(printing, 0, 50);
				}
				if (counter >= 50 && counter < 200) {
					sprintf(text, "I...");
					printText(2);
					if (counter == 199) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 200 && counter < 350) {
					sprintf(text, "I don't understand.");
					printText(2);
					if (counter == 349) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 350 && counter < 500) {
					sprintf(text, "I DON'T UNDERSTAND.");
					printText(2);
					if (counter == 349) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 499) {
					keepLooping = 0;
					state = 71;
				}

				if (counter >= 0 && counter < 50) {
					playerHealth++;
					if (playerHealth > 100) {
						playerHealth = 100;
					}
				}

				drawString4(20, 10, printing, 244);
				drawBasics(&player, &topBar, &middleBar, &bottomBar, &drawPlayer);
				playerMovement(&player, &upPressed, &downPressed, &topBar, &middleBar, &bottomBar);

				if (KEY_DOWN_NOW(BUTTON_SELECT)) {
					keepLooping = 0;
					state = 0;
				}

				// Increments counter of the loop, this is basically the timer
				counter++;
				if (counter > 987654) {
					counter = 0;
				}

				// Displays HP
				char hpDisplay[20];
				sprintf(hpDisplay, "Health: %d", playerHealth);
				drawString4(150, 3, hpDisplay, 250);
				drawRect4(151, 75, 6, playerHealth, 246);
				drawRect4(151, 75 + playerHealth, 6, 100 - playerHealth, 247);

				// MODE 4
				waitForVblank();
				FlipPage();
			}
		} else if (state == 71) { // Stage 35
			int keepLooping = 1;
			int counter = 0;
			int upPressed = 0;
			int downPressed = 0;
			int playerInvincibility = 0;
			int spiderCooldown = 0;
			int bulletCooldown = 0;
			int drawPlayer = 1;
			int generation = 1;
			int generate = 0;
			int generateGap = 75;
			int patternCount = 0;
			int pattern = 0;
			int randomness = 0;
			int laserCooldown = 0;
			// Create SPIDER enemies for this stage
			SPIDER spiders[30];
			int nextEmptySpider = 0;
			for (int i = 0; i < 30; i++) { // Initialize them
				spiders[i].arrayPosition = 987654;
			}

			// Create BULLET enemies for this stage
			BULLET bullets[30];
			int nextEmptyBullet = 0;
			for (int i = 0; i < 30; i++) {
				bullets[i].arrayPosition = 987654;
			}

			// Create LASER enemies for this stage
			LASER lasers[5];
			int nextEmptyLaser = 0;
			for (int i = 0; i < 5; i++) {
				lasers[i].arrayPosition = 987654;
			}

			while (keepLooping) {
				CLEAR;

				if (laserCooldown == 0 && generation == 1) {
					LASER temp;
					int randomize = rand() % 8;

					if (randomize >= 3) {
						createLaser(&temp, (rand() % 190), 0, 50, 0, 50, 20, 0); 
					} else {
						createLaser(&temp, (rand() % 190), 0, 50, 0, 50, 20, 0); 
					}
					lasers[nextEmptyLaser] = temp;
					nextEmptyLaser++;
					if (nextEmptyLaser >= 5) {
						nextEmptyLaser = 0;
					}
					laserCooldown = 100;
				}

				if (counter == generate && generation == 1) {
					if (pattern == 0) {
						SPIDER temp;
						createSpider(&temp, 1, &nextEmptySpider, 0, 3, 0, 45, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 0, 3, 0, 75, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 1, 3, 230, 105, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
					} else if (pattern == 1) {
						SPIDER temp;
						createSpider(&temp, 1, &nextEmptySpider, 0, 3, 0, 45, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 0, 3, 0, 105, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 1, 3, 230, 75, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
					} else if (pattern == 2) {
						SPIDER temp;
						createSpider(&temp, 1, &nextEmptySpider, 0, 3, 0, 75, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 0, 3, 0, 105, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 1, 3, 230, 45, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
					} else if (pattern == 3) {
						SPIDER temp;
						createSpider(&temp, 1, &nextEmptySpider, 1, 3, 230, 45, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 1, 3, 230, 75, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 0, 3, 0, 105, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
					} else if (pattern == 4) {
						SPIDER temp;
						createSpider(&temp, 1, &nextEmptySpider, 1, 3, 230, 45, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 1, 3, 230, 105, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 0, 3, 0, 75, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
					} else if (pattern == 5) {
						SPIDER temp;
						createSpider(&temp, 1, &nextEmptySpider, 1, 3, 230, 75, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 1, 3, 230, 105, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 0, 3, 0, 45, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
					}
					generate = generate + generateGap + (rand() % 15);
					patternCount++;
					if (patternCount == 3 + randomness) {
						pattern = rand() % 6;
						randomness = rand() % 2;
						patternCount = 0;
					}

				}

				if (counter % (10 + (rand() % 25)) == 0 && bulletCooldown == 0 && generation == 1) {
					BULLET temp;
					createBullet(&temp, 0, 0, 0, 0, 0, 0, 0, 0, &player);
					bullets[nextEmptyBullet] = temp;
					nextEmptyBullet++;
					if (nextEmptyBullet >= 30) {
						nextEmptyBullet = 0;
					}
					bulletCooldown = 30;
				}

				if (playerHealth <= 0) {
					keepLooping = 0;
					state = 999;
				}

				// Drawing

				// If player is invulnerable, flash the player to give player notice
				if (playerInvincibility > 0) {
					if (drawPlayer == 1) {
						drawPlayer = 0;
					} else {
						drawPlayer = 1;
					}
				} else {
					drawPlayer = 1;
				}

				// Drawing the objects
				drawBasics(&player, &topBar, &middleBar, &bottomBar, &drawPlayer);
				updateSpiders(spiders, &playerHealth, &player, &playerInvincibility);
				updateBullets(bullets, &playerHealth, &player, &playerInvincibility);
				updateLasers(lasers, &playerHealth, &player, &playerInvincibility);
				playerMovement(&player, &upPressed, &downPressed, &topBar, &middleBar, &bottomBar);

				if (KEY_DOWN_NOW(BUTTON_SELECT)) {
					keepLooping = 0;
					state = 0;
				}

				// Cooldown to prevent walls of spiders from appearing
				if (spiderCooldown != 0) {
					spiderCooldown--;
				}

				if (bulletCooldown != 0) {
					bulletCooldown--;
				}

				if (laserCooldown != 0) {
					laserCooldown--;
				}

				// If player gets hit, they get invincibility frames that last 100 loops. This decreases the remaining frames.
				if (playerInvincibility != 0) {
					playerInvincibility--;
				}

				// Displays HP
				char hpDisplay[20];
				sprintf(hpDisplay, "Health: %d", playerHealth);
				drawString4(150, 3, hpDisplay, 250);
				drawRect4(151, 75, 6, playerHealth, 246);
				drawRect4(151, 75 + playerHealth, 6, 100 - playerHealth, 247);

				// MODE 4
				waitForVblank();
				FlipPage();

				counter++;
				if (counter > 1000 && counter < 1150) {
					generation = 0;
				} else if (counter > 1150) {
					keepLooping = 0;
					state = 72;
				}
			}
		} else if (state == 72) { // Story 36
			int keepLooping = 1;
			int counter = 0;
			int upPressed = 0;
			int downPressed = 0;
			int drawPlayer = 1;
			char text[50];
			char printing[50];
			char *textPointer = &text[0];
			int letters = 0;
			while (keepLooping) {
				CLEAR;

				// STORY TEXT HERE
				if (counter < 50) {
					memset(printing, 0, 50);
				}
				if (counter >= 50 && counter < 200) {
					sprintf(text, "STOP TRYING.");
					printText(2);
					if (counter == 199) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 200 && counter < 350) {
					sprintf(text, "I COULD KILL YOU.");
					printText(2);
					if (counter == 349) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 350 && counter < 500) {
					sprintf(text, "RIGHT. NOW.");
					printText(2);
					if (counter == 349) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 499) {
					keepLooping = 0;
					state = 73;
				}

				if (counter >= 0 && counter < 50) {
					playerHealth++;
					if (playerHealth > 100) {
						playerHealth = 100;
					}
				}

				drawString4(20, 10, printing, 244);
				drawBasics(&player, &topBar, &middleBar, &bottomBar, &drawPlayer);
				playerMovement(&player, &upPressed, &downPressed, &topBar, &middleBar, &bottomBar);

				if (KEY_DOWN_NOW(BUTTON_SELECT)) {
					keepLooping = 0;
					state = 0;
				}

				// Increments counter of the loop, this is basically the timer
				counter++;
				if (counter > 987654) {
					counter = 0;
				}

				// Displays HP
				char hpDisplay[20];
				sprintf(hpDisplay, "Health: %d", playerHealth);
				drawString4(150, 3, hpDisplay, 250);
				drawRect4(151, 75, 6, playerHealth, 246);
				drawRect4(151, 75 + playerHealth, 6, 100 - playerHealth, 247);

				// MODE 4
				waitForVblank();
				FlipPage();
			}
		} else if (state == 73) { // Stage 35
			int keepLooping = 1;
			int counter = 0;
			int upPressed = 0;
			int downPressed = 0;
			int playerInvincibility = 0;
			int spiderCooldown = 0;
			int bulletCooldown = 0;
			int drawPlayer = 1;
			int generation = 1;
			int generate = 0;
			int generateGap = 75;
			int patternCount = 0;
			int pattern = 0;
			int randomness = 0;
			int laserCooldown = 0;
			// Create SPIDER enemies for this stage
			SPIDER spiders[30];
			int nextEmptySpider = 0;
			for (int i = 0; i < 30; i++) { // Initialize them
				spiders[i].arrayPosition = 987654;
			}

			// Create BULLET enemies for this stage
			BULLET bullets[30];
			int nextEmptyBullet = 0;
			for (int i = 0; i < 30; i++) {
				bullets[i].arrayPosition = 987654;
			}

			// Create LASER enemies for this stage
			LASER lasers[5];
			int nextEmptyLaser = 0;
			for (int i = 0; i < 5; i++) {
				lasers[i].arrayPosition = 987654;
			}

			while (keepLooping) {
				CLEAR;

				if (laserCooldown == 0 && generation == 1) {
					LASER temp;
					int randomize = rand() % 8;

					if (randomize >= 3) {
						createLaser(&temp, (rand() % 190), 0, 50, 0, 50, 20, 0); 
					} else {
						createLaser(&temp, (rand() % 190), 0, 50, 0, 50, 20, 0); 
					}
					lasers[nextEmptyLaser] = temp;
					nextEmptyLaser++;
					if (nextEmptyLaser >= 5) {
						nextEmptyLaser = 0;
					}
					laserCooldown = 100;
				}

				if (counter % (10 +  (rand() % 35)) == 0 && spiderCooldown == 0 && generation == 1) {
					SPIDER temp;
					createSpider(&temp, 1, &nextEmptySpider, -1, 2, -1, -1, 0, 244);
					spiders[nextEmptySpider] = temp;
					nextEmptySpider++;
					if (nextEmptySpider >= 30) {
						nextEmptySpider = 0;
					}
					spiderCooldown = 40;
				}

				if (counter == generate && generation == 1) {
					if (pattern == 0) {
						SPIDER temp;
						createSpider(&temp, 1, &nextEmptySpider, 0, 3, 0, 45, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 0, 3, 0, 75, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 1, 3, 230, 105, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
					} else if (pattern == 1) {
						SPIDER temp;
						createSpider(&temp, 1, &nextEmptySpider, 0, 3, 0, 45, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 0, 3, 0, 105, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 1, 3, 230, 75, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
					} else if (pattern == 2) {
						SPIDER temp;
						createSpider(&temp, 1, &nextEmptySpider, 0, 3, 0, 75, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 0, 3, 0, 105, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 1, 3, 230, 45, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
					} else if (pattern == 3) {
						SPIDER temp;
						createSpider(&temp, 1, &nextEmptySpider, 1, 3, 230, 45, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 1, 3, 230, 75, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 0, 3, 0, 105, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
					} else if (pattern == 4) {
						SPIDER temp;
						createSpider(&temp, 1, &nextEmptySpider, 1, 3, 230, 45, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 1, 3, 230, 105, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 0, 3, 0, 75, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
					} else if (pattern == 5) {
						SPIDER temp;
						createSpider(&temp, 1, &nextEmptySpider, 1, 3, 230, 75, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 1, 3, 230, 105, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 0, 3, 0, 45, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
					}
					generate = generate + generateGap + (rand() % 15);
					patternCount++;
					if (patternCount == 3 + randomness) {
						pattern = rand() % 6;
						randomness = rand() % 2;
						patternCount = 0;
					}

				}

				if (counter % (10 + (rand() % 25)) == 0 && bulletCooldown == 0 && generation == 1) {
					BULLET temp;
					createBullet(&temp, 0, 0, 0, 0, 0, 0, 0, 0, &player);
					bullets[nextEmptyBullet] = temp;
					nextEmptyBullet++;
					if (nextEmptyBullet >= 30) {
						nextEmptyBullet = 0;
					}
					bulletCooldown = 30;
				}

				if (playerHealth <= 0) {
					keepLooping = 0;
					state = 999;
				}

				// Drawing

				// If player is invulnerable, flash the player to give player notice
				if (playerInvincibility > 0) {
					if (drawPlayer == 1) {
						drawPlayer = 0;
					} else {
						drawPlayer = 1;
					}
				} else {
					drawPlayer = 1;
				}

				// Drawing the objects
				drawBasics(&player, &topBar, &middleBar, &bottomBar, &drawPlayer);
				updateSpiders(spiders, &playerHealth, &player, &playerInvincibility);
				updateBullets(bullets, &playerHealth, &player, &playerInvincibility);
				updateLasers(lasers, &playerHealth, &player, &playerInvincibility);
				playerMovement(&player, &upPressed, &downPressed, &topBar, &middleBar, &bottomBar);

				if (KEY_DOWN_NOW(BUTTON_SELECT)) {
					keepLooping = 0;
					state = 0;
				}

				// Cooldown to prevent walls of spiders from appearing
				if (spiderCooldown != 0) {
					spiderCooldown--;
				}

				if (bulletCooldown != 0) {
					bulletCooldown--;
				}

				if (laserCooldown != 0) {
					laserCooldown--;
				}

				// If player gets hit, they get invincibility frames that last 100 loops. This decreases the remaining frames.
				if (playerInvincibility != 0) {
					playerInvincibility--;
				}

				// Displays HP
				char hpDisplay[20];
				sprintf(hpDisplay, "Health: %d", playerHealth);
				drawString4(150, 3, hpDisplay, 250);
				drawRect4(151, 75, 6, playerHealth, 246);
				drawRect4(151, 75 + playerHealth, 6, 100 - playerHealth, 247);

				// MODE 4
				waitForVblank();
				FlipPage();

				counter++;
				if (counter > 1000 && counter < 1150) {
					generation = 0;
				} else if (counter > 1150) {
					keepLooping = 0;
					state = 74;
				}
			}
		} else if (state == 74) { // Story 37
			int keepLooping = 1;
			int counter = 0;
			int upPressed = 0;
			int downPressed = 0;
			int drawPlayer = 1;
			char text[50];
			char printing[50];
			char *textPointer = &text[0];
			int letters = 0;
			while (keepLooping) {
				CLEAR;

				// STORY TEXT HERE
				if (counter < 50) {
					memset(printing, 0, 50);
				}
				if (counter >= 50 && counter < 200) {
					sprintf(text, "Why are you so determined?");
					printText(2);
					if (counter == 199) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 200 && counter < 350) {
					sprintf(text, "Why are you so hopeful?");
					printText(2);
					if (counter == 349) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 350 && counter < 500) {
					sprintf(text, "Isn't this painful?");
					printText(2);
					if (counter == 349) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 499) {
					keepLooping = 0;
					state = 75;
				}

				if (counter >= 0 && counter < 60) {
					playerHealth++;
					if (playerHealth > 100) {
						playerHealth = 100;
					}
				}

				drawString4(20, 10, printing, 244);
				drawBasics(&player, &topBar, &middleBar, &bottomBar, &drawPlayer);
				playerMovement(&player, &upPressed, &downPressed, &topBar, &middleBar, &bottomBar);

				if (KEY_DOWN_NOW(BUTTON_SELECT)) {
					keepLooping = 0;
					state = 0;
				}

				// Increments counter of the loop, this is basically the timer
				counter++;
				if (counter > 987654) {
					counter = 0;
				}

				// Displays HP
				char hpDisplay[20];
				sprintf(hpDisplay, "Health: %d", playerHealth);
				drawString4(150, 3, hpDisplay, 250);
				drawRect4(151, 75, 6, playerHealth, 246);
				drawRect4(151, 75 + playerHealth, 6, 100 - playerHealth, 247);

				// MODE 4
				waitForVblank();
				FlipPage();
			}
		} else if (state == 75) { // Stage 37
			int keepLooping = 1;
			int counter = 0;
			int upPressed = 0;
			int downPressed = 0;
			int playerInvincibility = 0;
			int spiderCooldown = 0;
			int bulletCooldown = 0;
			int drawPlayer = 1;
			int generation = 1;
			int generate = 0;
			int generateGap = 75;
			int patternCount = 0;
			int pattern = 0;
			int randomness = 0;
			int laserCooldown = 0;
			// Create SPIDER enemies for this stage
			SPIDER spiders[30];
			int nextEmptySpider = 0;
			for (int i = 0; i < 30; i++) { // Initialize them
				spiders[i].arrayPosition = 987654;
			}

			// Create BULLET enemies for this stage
			BULLET bullets[30];
			int nextEmptyBullet = 0;
			for (int i = 0; i < 30; i++) {
				bullets[i].arrayPosition = 987654;
			}

			// Create LASER enemies for this stage
			LASER lasers[5];
			int nextEmptyLaser = 0;
			for (int i = 0; i < 5; i++) {
				lasers[i].arrayPosition = 987654;
			}

			while (keepLooping) {
				CLEAR;

				if (laserCooldown == 0 && generation == 1) {
					LASER temp;
					int randomize = rand() % 8;

					if (randomize >= 5) {
						createLaser(&temp, (rand() % 190), 0, 50, 0, 50, 20, 0); 
					} else {
						createLaser(&temp, player.x - 25 + 3, 0, 50, 0, 50, 20, 0); 
					}
					lasers[nextEmptyLaser] = temp;
					nextEmptyLaser++;
					if (nextEmptyLaser >= 5) {
						nextEmptyLaser = 0;
					}
					laserCooldown = 100;
				}

				if (counter % (10 +  (rand() % 35)) == 0 && spiderCooldown == 0 && generation == 1) {
					SPIDER temp;
					createSpider(&temp, 1, &nextEmptySpider, -1, 2, -1, -1, 0, 244);
					spiders[nextEmptySpider] = temp;
					nextEmptySpider++;
					if (nextEmptySpider >= 30) {
						nextEmptySpider = 0;
					}
					spiderCooldown = 40;
				}

				if (counter == generate && generation == 1) {
					if (pattern == 0) {
						SPIDER temp;
						createSpider(&temp, 1, &nextEmptySpider, 0, 3, 0, 45, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 0, 3, 0, 75, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 1, 3, 230, 105, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
					} else if (pattern == 1) {
						SPIDER temp;
						createSpider(&temp, 1, &nextEmptySpider, 0, 3, 0, 45, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 0, 3, 0, 105, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 1, 3, 230, 75, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
					} else if (pattern == 2) {
						SPIDER temp;
						createSpider(&temp, 1, &nextEmptySpider, 0, 3, 0, 75, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 0, 3, 0, 105, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 1, 3, 230, 45, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
					} else if (pattern == 3) {
						SPIDER temp;
						createSpider(&temp, 1, &nextEmptySpider, 1, 3, 230, 45, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 1, 3, 230, 75, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 0, 3, 0, 105, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
					} else if (pattern == 4) {
						SPIDER temp;
						createSpider(&temp, 1, &nextEmptySpider, 1, 3, 230, 45, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 1, 3, 230, 105, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 0, 3, 0, 75, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
					} else if (pattern == 5) {
						SPIDER temp;
						createSpider(&temp, 1, &nextEmptySpider, 1, 3, 230, 75, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 1, 3, 230, 105, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 0, 3, 0, 45, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
					}
					generate = generate + generateGap + (rand() % 15);
					patternCount++;
					if (patternCount == 3 + randomness) {
						pattern = rand() % 6;
						randomness = rand() % 2;
						patternCount = 0;
					}

				}

				if (counter % (10 + (rand() % 25)) == 0 && bulletCooldown == 0 && generation == 1) {
					BULLET temp;
					createBullet(&temp, 0, 0, 0, 0, 0, 0, 0, 0, &player);
					bullets[nextEmptyBullet] = temp;
					nextEmptyBullet++;
					if (nextEmptyBullet >= 30) {
						nextEmptyBullet = 0;
					}
					bulletCooldown = 20;
				}

				if (playerHealth <= 0) {
					keepLooping = 0;
					state = 999;
				}

				// Drawing

				// If player is invulnerable, flash the player to give player notice
				if (playerInvincibility > 0) {
					if (drawPlayer == 1) {
						drawPlayer = 0;
					} else {
						drawPlayer = 1;
					}
				} else {
					drawPlayer = 1;
				}

				// Drawing the objects
				drawBasics(&player, &topBar, &middleBar, &bottomBar, &drawPlayer);
				updateSpiders(spiders, &playerHealth, &player, &playerInvincibility);
				updateBullets(bullets, &playerHealth, &player, &playerInvincibility);
				updateLasers(lasers, &playerHealth, &player, &playerInvincibility);
				playerMovement(&player, &upPressed, &downPressed, &topBar, &middleBar, &bottomBar);

				if (KEY_DOWN_NOW(BUTTON_SELECT)) {
					keepLooping = 0;
					state = 0;
				}

				// Cooldown to prevent walls of spiders from appearing
				if (spiderCooldown != 0) {
					spiderCooldown--;
				}

				if (bulletCooldown != 0) {
					bulletCooldown--;
				}

				if (laserCooldown != 0) {
					laserCooldown--;
				}

				// If player gets hit, they get invincibility frames that last 100 loops. This decreases the remaining frames.
				if (playerInvincibility != 0) {
					playerInvincibility--;
				}

				// Displays HP
				char hpDisplay[20];
				sprintf(hpDisplay, "Health: %d", playerHealth);
				drawString4(150, 3, hpDisplay, 250);
				drawRect4(151, 75, 6, playerHealth, 246);
				drawRect4(151, 75 + playerHealth, 6, 100 - playerHealth, 247);

				// MODE 4
				waitForVblank();
				FlipPage();

				counter++;
				if (counter > 1000 && counter < 1150) {
					generation = 0;
				} else if (counter > 1150) {
					keepLooping = 0;
					state = 76;
				}
			}
		} else if (state == 76) { // Story 38
			int keepLooping = 1;
			int counter = 0;
			int upPressed = 0;
			int downPressed = 0;
			int drawPlayer = 1;
			char text[50];
			char printing[50];
			char *textPointer = &text[0];
			int letters = 0;
			while (keepLooping) {
				CLEAR;

				// STORY TEXT HERE
				if (counter < 50) {
					memset(printing, 0, 50);
				}
				if (counter >= 50 && counter < 200) {
					sprintf(text, "Maybe...");
					printText(2);
					if (counter == 199) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 200 && counter < 350) {
					sprintf(text, "That's the difference...");
					printText(2);
					if (counter == 349) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 350 && counter < 500) {
					sprintf(text, "Between you and me...");
					printText(2);
					if (counter == 349) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 499) {
					keepLooping = 0;
					state = 77;
				}

				if (counter >= 0 && counter < 70) {
					playerHealth++;
					if (playerHealth > 100) {
						playerHealth = 100;
					}
				}

				drawString4(20, 10, printing, 244);
				drawBasics(&player, &topBar, &middleBar, &bottomBar, &drawPlayer);
				playerMovement(&player, &upPressed, &downPressed, &topBar, &middleBar, &bottomBar);

				if (KEY_DOWN_NOW(BUTTON_SELECT)) {
					keepLooping = 0;
					state = 0;
				}

				// Increments counter of the loop, this is basically the timer
				counter++;
				if (counter > 987654) {
					counter = 0;
				}

				// Displays HP
				char hpDisplay[20];
				sprintf(hpDisplay, "Health: %d", playerHealth);
				drawString4(150, 3, hpDisplay, 250);
				drawRect4(151, 75, 6, playerHealth, 246);
				drawRect4(151, 75 + playerHealth, 6, 100 - playerHealth, 247);

				// MODE 4
				waitForVblank();
				FlipPage();
			}
		} else if (state == 77) { // Stage 38
			int keepLooping = 1;
			int counter = 0;
			int upPressed = 0;
			int downPressed = 0;
			int playerInvincibility = 0;
			int spiderCooldown = 0;
			int bulletCooldown = 0;
			int drawPlayer = 1;
			int generation = 1;
			int generate = 0;
			int generateGap = 75;
			int patternCount = 0;
			int pattern = 0;
			int randomness = 0;
			int laserCooldown = 0;
			// Create SPIDER enemies for this stage
			SPIDER spiders[30];
			int nextEmptySpider = 0;
			for (int i = 0; i < 30; i++) { // Initialize them
				spiders[i].arrayPosition = 987654;
			}

			// Create BULLET enemies for this stage
			BULLET bullets[30];
			int nextEmptyBullet = 0;
			for (int i = 0; i < 30; i++) {
				bullets[i].arrayPosition = 987654;
			}

			// Create LASER enemies for this stage
			LASER lasers[5];
			int nextEmptyLaser = 0;
			for (int i = 0; i < 5; i++) {
				lasers[i].arrayPosition = 987654;
			}

			while (keepLooping) {
				CLEAR;

				if (laserCooldown == 0 && generation == 1) {
					LASER temp;
					int randomize = rand() % 8;

					if (randomize >= 5) {
						createLaser(&temp, (rand() % 190), 0, 50, 0, 40, 20, 0); 
					} else {
						createLaser(&temp, player.x - 25 + 3, 0, 50, 0, 40, 20, 0); 
					}
					lasers[nextEmptyLaser] = temp;
					nextEmptyLaser++;
					if (nextEmptyLaser >= 5) {
						nextEmptyLaser = 0;
					}
					laserCooldown = 90;
				}

				if (counter % (10 +  (rand() % 35)) == 0 && spiderCooldown == 0 && generation == 1) {
					SPIDER temp;
					createSpider(&temp, 1, &nextEmptySpider, -1, 2, -1, -1, 0, 244);
					spiders[nextEmptySpider] = temp;
					nextEmptySpider++;
					if (nextEmptySpider >= 30) {
						nextEmptySpider = 0;
					}
					spiderCooldown = 40;
				}

				if (counter == generate && generation == 1) {
					if (pattern == 0) {
						SPIDER temp;
						createSpider(&temp, 1, &nextEmptySpider, 0, 3, 0, 45, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 0, 3, 0, 75, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 1, 3, 230, 105, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
					} else if (pattern == 1) {
						SPIDER temp;
						createSpider(&temp, 1, &nextEmptySpider, 0, 3, 0, 45, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 0, 3, 0, 105, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 1, 3, 230, 75, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
					} else if (pattern == 2) {
						SPIDER temp;
						createSpider(&temp, 1, &nextEmptySpider, 0, 3, 0, 75, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 0, 3, 0, 105, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 1, 3, 230, 45, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
					} else if (pattern == 3) {
						SPIDER temp;
						createSpider(&temp, 1, &nextEmptySpider, 1, 3, 230, 45, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 1, 3, 230, 75, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 0, 3, 0, 105, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
					} else if (pattern == 4) {
						SPIDER temp;
						createSpider(&temp, 1, &nextEmptySpider, 1, 3, 230, 45, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 1, 3, 230, 105, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 0, 3, 0, 75, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
					} else if (pattern == 5) {
						SPIDER temp;
						createSpider(&temp, 1, &nextEmptySpider, 1, 3, 230, 75, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 1, 3, 230, 105, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 0, 3, 0, 45, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
					}
					generate = generate + generateGap + (rand() % 15);
					patternCount++;
					if (patternCount == 3 + randomness) {
						pattern = rand() % 6;
						randomness = rand() % 2;
						patternCount = 0;
					}

				}

				if (counter % (5 + (rand() % 20)) == 0 && bulletCooldown == 0 && generation == 1) {
					BULLET temp;
					int rng = rand() % 2;
					if (rng == 0) {
						createBullet(&temp, 0, 0, 0, 0, 0, 0, 0, 0, &player);
					} else {
						createBullet(&temp, 0, 0, 0, 0, 0, 0, 0, 1, &player);
					}
					bullets[nextEmptyBullet] = temp;
					nextEmptyBullet++;
					if (nextEmptyBullet >= 30) {
						nextEmptyBullet = 0;
					}
					bulletCooldown = 15;
				}

				if (playerHealth <= 0) {
					keepLooping = 0;
					state = 999;
				}

				// Drawing

				// If player is invulnerable, flash the player to give player notice
				if (playerInvincibility > 0) {
					if (drawPlayer == 1) {
						drawPlayer = 0;
					} else {
						drawPlayer = 1;
					}
				} else {
					drawPlayer = 1;
				}

				// Drawing the objects
				drawBasics(&player, &topBar, &middleBar, &bottomBar, &drawPlayer);
				updateSpiders(spiders, &playerHealth, &player, &playerInvincibility);
				updateBullets(bullets, &playerHealth, &player, &playerInvincibility);
				updateLasers(lasers, &playerHealth, &player, &playerInvincibility);
				playerMovement(&player, &upPressed, &downPressed, &topBar, &middleBar, &bottomBar);

				if (KEY_DOWN_NOW(BUTTON_SELECT)) {
					keepLooping = 0;
					state = 0;
				}

				// Cooldown to prevent walls of spiders from appearing
				if (spiderCooldown != 0) {
					spiderCooldown--;
				}

				if (bulletCooldown != 0) {
					bulletCooldown--;
				}

				if (laserCooldown != 0) {
					laserCooldown--;
				}

				// If player gets hit, they get invincibility frames that last 100 loops. This decreases the remaining frames.
				if (playerInvincibility != 0) {
					playerInvincibility--;
				}

				// Displays HP
				char hpDisplay[20];
				sprintf(hpDisplay, "Health: %d", playerHealth);
				drawString4(150, 3, hpDisplay, 250);
				drawRect4(151, 75, 6, playerHealth, 246);
				drawRect4(151, 75 + playerHealth, 6, 100 - playerHealth, 247);

				// MODE 4
				waitForVblank();
				FlipPage();

				counter++;
				if (counter > 1000 && counter < 1150) {
					generation = 0;
				} else if (counter > 1150) {
					keepLooping = 0;
					state = 78;
				}
			}
		} else if (state == 78) { // Story 38
			int keepLooping = 1;
			int counter = 0;
			int upPressed = 0;
			int downPressed = 0;
			int drawPlayer = 1;
			char text[50];
			char printing[50];
			char *textPointer = &text[0];
			int letters = 0;
			while (keepLooping) {
				CLEAR;

				// STORY TEXT HERE
				if (counter < 50) {
					memset(printing, 0, 50);
				}
				if (counter >= 50 && counter < 200) {
					sprintf(text, "Maybe...");
					printText(2);
					if (counter == 199) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 200 && counter < 350) {
					sprintf(text, "You'd actually survive out there.");
					printText(2);
					if (counter == 349) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 350 && counter < 500) {
					sprintf(text, "Unlike me...");
					printText(2);
					if (counter == 349) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 499) {
					keepLooping = 0;
					state = 79;
				}

				if (counter >= 0 && counter < 80) {
					playerHealth++;
					if (playerHealth > 100) {
						playerHealth = 100;
					}
				}

				drawString4(20, 10, printing, 244);
				drawBasics(&player, &topBar, &middleBar, &bottomBar, &drawPlayer);
				playerMovement(&player, &upPressed, &downPressed, &topBar, &middleBar, &bottomBar);

				if (KEY_DOWN_NOW(BUTTON_SELECT)) {
					keepLooping = 0;
					state = 0;
				}

				// Increments counter of the loop, this is basically the timer
				counter++;
				if (counter > 987654) {
					counter = 0;
				}

				// Displays HP
				char hpDisplay[20];
				sprintf(hpDisplay, "Health: %d", playerHealth);
				drawString4(150, 3, hpDisplay, 250);
				drawRect4(151, 75, 6, playerHealth, 246);
				drawRect4(151, 75 + playerHealth, 6, 100 - playerHealth, 247);

				// MODE 4
				waitForVblank();
				FlipPage();
			}
		} else if (state == 79) { // Stage 38
			int keepLooping = 1;
			int counter = 0;
			int upPressed = 0;
			int downPressed = 0;
			int playerInvincibility = 0;
			int spiderCooldown = 0;
			int bulletCooldown = 0;
			int drawPlayer = 1;
			int generation = 1;
			int generate = 0;
			int generateGap = 75;
			int patternCount = 0;
			int pattern = 0;
			int randomness = 0;
			int laserCooldown = 0;
			// Create SPIDER enemies for this stage
			SPIDER spiders[30];
			int nextEmptySpider = 0;
			for (int i = 0; i < 30; i++) { // Initialize them
				spiders[i].arrayPosition = 987654;
			}

			// Create BULLET enemies for this stage
			BULLET bullets[30];
			int nextEmptyBullet = 0;
			for (int i = 0; i < 30; i++) {
				bullets[i].arrayPosition = 987654;
			}

			// Create LASER enemies for this stage
			LASER lasers[5];
			int nextEmptyLaser = 0;
			for (int i = 0; i < 5; i++) {
				lasers[i].arrayPosition = 987654;
			}

			while (keepLooping) {
				CLEAR;

				if (laserCooldown == 0 && generation == 1) {
					LASER temp;
					int randomize = rand() % 8;

					if (randomize >= 5) {
						createLaser(&temp, (rand() % 210), 0, 30, 0, 50, 10, 0); 
					} else {
						createLaser(&temp, (rand() % 210), 0, 30, 0, 50, 10, 0); 
					}
					lasers[nextEmptyLaser] = temp;
					nextEmptyLaser++;
					if (nextEmptyLaser >= 5) {
						nextEmptyLaser = 0;
					}
				}

				if (laserCooldown == 0 && generation == 1) {
					LASER temp;
					int randomize = rand() % 8;

					if (randomize >= 5) {
						createLaser(&temp, 0, (rand() % 130), 30, 0, 50, 10, 1); 
					} else {
						createLaser(&temp, 0, (rand() % 130), 30, 0, 50, 10, 1); 
					}
					lasers[nextEmptyLaser] = temp;
					nextEmptyLaser++;
					if (nextEmptyLaser >= 5) {
						nextEmptyLaser = 0;
					}
					laserCooldown = 100;
				}

				if (counter % (10 +  (rand() % 35)) == 0 && spiderCooldown == 0 && generation == 1) {
					SPIDER temp;
					createSpider(&temp, 1, &nextEmptySpider, -1, 2, -1, -1, 0, 244);
					spiders[nextEmptySpider] = temp;
					nextEmptySpider++;
					if (nextEmptySpider >= 30) {
						nextEmptySpider = 0;
					}
					spiderCooldown = 40;
				}

				if (counter == generate && generation == 1) {
					if (pattern == 0) {
						SPIDER temp;
						createSpider(&temp, 1, &nextEmptySpider, 0, 3, 0, 45, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 0, 3, 0, 75, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 1, 3, 230, 105, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
					} else if (pattern == 1) {
						SPIDER temp;
						createSpider(&temp, 1, &nextEmptySpider, 0, 3, 0, 45, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 0, 3, 0, 105, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 1, 3, 230, 75, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
					} else if (pattern == 2) {
						SPIDER temp;
						createSpider(&temp, 1, &nextEmptySpider, 0, 3, 0, 75, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 0, 3, 0, 105, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 1, 3, 230, 45, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
					} else if (pattern == 3) {
						SPIDER temp;
						createSpider(&temp, 1, &nextEmptySpider, 1, 3, 230, 45, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 1, 3, 230, 75, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 0, 3, 0, 105, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
					} else if (pattern == 4) {
						SPIDER temp;
						createSpider(&temp, 1, &nextEmptySpider, 1, 3, 230, 45, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 1, 3, 230, 105, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 0, 3, 0, 75, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
					} else if (pattern == 5) {
						SPIDER temp;
						createSpider(&temp, 1, &nextEmptySpider, 1, 3, 230, 75, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 1, 3, 230, 105, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 0, 3, 0, 45, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
					}
					generate = generate + generateGap + (rand() % 15);
					patternCount++;
					if (patternCount == 3 + randomness) {
						pattern = rand() % 6;
						randomness = rand() % 2;
						patternCount = 0;
					}

				}

				if (counter % (5 + (rand() % 20)) == 0 && bulletCooldown == 0 && generation == 1) {
					BULLET temp;
					int rng = rand() % 2;
					if (rng == 0) {
						createBullet(&temp, 0, 0, 0, 0, 0, 0, 0, 0, &player);
					} else {
						createBullet(&temp, 0, 0, 0, 0, 0, 0, 0, 1, &player);
					}
					bullets[nextEmptyBullet] = temp;
					nextEmptyBullet++;
					if (nextEmptyBullet >= 30) {
						nextEmptyBullet = 0;
					}
					bulletCooldown = 15;
				}

				if (playerHealth <= 0) {
					keepLooping = 0;
					state = 999;
				}

				// Drawing

				// If player is invulnerable, flash the player to give player notice
				if (playerInvincibility > 0) {
					if (drawPlayer == 1) {
						drawPlayer = 0;
					} else {
						drawPlayer = 1;
					}
				} else {
					drawPlayer = 1;
				}

				// Drawing the objects
				drawBasics(&player, &topBar, &middleBar, &bottomBar, &drawPlayer);
				updateSpiders(spiders, &playerHealth, &player, &playerInvincibility);
				updateBullets(bullets, &playerHealth, &player, &playerInvincibility);
				updateLasers(lasers, &playerHealth, &player, &playerInvincibility);
				playerMovement(&player, &upPressed, &downPressed, &topBar, &middleBar, &bottomBar);

				if (KEY_DOWN_NOW(BUTTON_SELECT)) {
					keepLooping = 0;
					state = 0;
				}

				// Cooldown to prevent walls of spiders from appearing
				if (spiderCooldown != 0) {
					spiderCooldown--;
				}

				if (bulletCooldown != 0) {
					bulletCooldown--;
				}

				if (laserCooldown != 0) {
					laserCooldown--;
				}

				// If player gets hit, they get invincibility frames that last 100 loops. This decreases the remaining frames.
				if (playerInvincibility != 0) {
					playerInvincibility--;
				}

				// Displays HP
				char hpDisplay[20];
				sprintf(hpDisplay, "Health: %d", playerHealth);
				drawString4(150, 3, hpDisplay, 250);
				drawRect4(151, 75, 6, playerHealth, 246);
				drawRect4(151, 75 + playerHealth, 6, 100 - playerHealth, 247);

				// MODE 4
				waitForVblank();
				FlipPage();

				counter++;
				if (counter > 1000 && counter < 1150) {
					generation = 0;
				} else if (counter > 1150) {
					keepLooping = 0;
					state = 80;
				}
			}
		} else if (state == 80) { // Story 40
			int keepLooping = 1;
			int counter = 0;
			int upPressed = 0;
			int downPressed = 0;
			int drawPlayer = 1;
			char text[50];
			char printing[50];
			char *textPointer = &text[0];
			int letters = 0;
			while (keepLooping) {
				CLEAR;

				// STORY TEXT HERE
				if (counter < 50) {
					memset(printing, 0, 50);
				}
				if (counter >= 50 && counter < 200) {
					sprintf(text, "You know what?");
					printText(2);
					if (counter == 199) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 200 && counter < 350) {
					sprintf(text, "Prove to me...");
					printText(2);
					if (counter == 349) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 350 && counter < 500) {
					sprintf(text, "...that you're strong enough.");
					printText(2);
					if (counter == 349) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 499) {
					keepLooping = 0;
					state = 81;
				}

				if (counter >= 0 && counter < 80) {
					playerHealth++;
					if (playerHealth > 100) {
						playerHealth = 100;
					}
				}

				drawString4(20, 10, printing, 244);
				drawBasics(&player, &topBar, &middleBar, &bottomBar, &drawPlayer);
				playerMovement(&player, &upPressed, &downPressed, &topBar, &middleBar, &bottomBar);

				if (KEY_DOWN_NOW(BUTTON_SELECT)) {
					keepLooping = 0;
					state = 0;
				}

				// Increments counter of the loop, this is basically the timer
				counter++;
				if (counter > 987654) {
					counter = 0;
				}

				// Displays HP
				char hpDisplay[20];
				sprintf(hpDisplay, "Health: %d", playerHealth);
				drawString4(150, 3, hpDisplay, 250);
				drawRect4(151, 75, 6, playerHealth, 246);
				drawRect4(151, 75 + playerHealth, 6, 100 - playerHealth, 247);

				// MODE 4
				waitForVblank();
				FlipPage();
			}
		} else if (state == 81) { // Stage 40
			int keepLooping = 1;
			int counter = 0;
			int upPressed = 0;
			int downPressed = 0;
			int playerInvincibility = 0;
			int spiderCooldown = 0;
			int bulletCooldown = 0;
			int drawPlayer = 1;
			int generation = 1;
			int generate = 0;
			int generateGap = 75;
			int patternCount = 0;
			int pattern = 0;
			int randomness = 0;
			int laserCooldown = 0;
			int spid = 50;
			int bull = 50;
			int lase = 100;
			int deton = 50;
			// Create SPIDER enemies for this stage
			SPIDER spiders[30];
			int nextEmptySpider = 0;
			for (int i = 0; i < 30; i++) { // Initialize them
				spiders[i].arrayPosition = 987654;
			}

			// Create BULLET enemies for this stage
			BULLET bullets[30];
			int nextEmptyBullet = 0;
			for (int i = 0; i < 30; i++) {
				bullets[i].arrayPosition = 987654;
			}

			// Create LASER enemies for this stage
			LASER lasers[5];
			int nextEmptyLaser = 0;
			for (int i = 0; i < 5; i++) {
				lasers[i].arrayPosition = 987654;
			}

			while (keepLooping) {
				CLEAR;

				if (laserCooldown == 0 && generation == 1) {
					LASER temp;
					int randomize = rand() % 8;

					if (randomize >= 5) {
						createLaser(&temp, (rand() % 210), 0, 30, 0, deton, 10, 0); 
					} else {
						createLaser(&temp, (rand() % 210), 0, 30, 0, deton, 10, 0); 
					}
					lasers[nextEmptyLaser] = temp;
					nextEmptyLaser++;
					if (nextEmptyLaser >= 5) {
						nextEmptyLaser = 0;
					}
				}

				if (laserCooldown == 0 && generation == 1) {
					LASER temp;
					int randomize = rand() % 8;

					if (randomize >= 5) {
						createLaser(&temp, 0, (rand() % 130), 30, 0, deton, 10, 1); 
					} else {
						createLaser(&temp, 0, (rand() % 130), 30, 0, deton, 10, 1); 
					}
					lasers[nextEmptyLaser] = temp;
					nextEmptyLaser++;
					if (nextEmptyLaser >= 5) {
						nextEmptyLaser = 0;
					}
					laserCooldown = lase;
				}

				if (counter % (5 +  (rand() % 25)) == 0 && spiderCooldown == 0 && generation == 1) {
					SPIDER temp;
					createSpider(&temp, 1, &nextEmptySpider, -1, 2, -1, -1, 0, 244);
					spiders[nextEmptySpider] = temp;
					nextEmptySpider++;
					if (nextEmptySpider >= 30) {
						nextEmptySpider = 0;
					}
					spiderCooldown = spid;
				}

				if (counter == generate && generation == 1) {
					if (pattern == 0) {
						SPIDER temp;
						createSpider(&temp, 1, &nextEmptySpider, 0, 3, 0, 45, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 0, 3, 0, 75, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 1, 3, 230, 105, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
					} else if (pattern == 1) {
						SPIDER temp;
						createSpider(&temp, 1, &nextEmptySpider, 0, 3, 0, 45, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 0, 3, 0, 105, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 1, 3, 230, 75, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
					} else if (pattern == 2) {
						SPIDER temp;
						createSpider(&temp, 1, &nextEmptySpider, 0, 3, 0, 75, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 0, 3, 0, 105, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 1, 3, 230, 45, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
					} else if (pattern == 3) {
						SPIDER temp;
						createSpider(&temp, 1, &nextEmptySpider, 1, 3, 230, 45, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 1, 3, 230, 75, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 0, 3, 0, 105, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
					} else if (pattern == 4) {
						SPIDER temp;
						createSpider(&temp, 1, &nextEmptySpider, 1, 3, 230, 45, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 1, 3, 230, 105, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 0, 3, 0, 75, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
					} else if (pattern == 5) {
						SPIDER temp;
						createSpider(&temp, 1, &nextEmptySpider, 1, 3, 230, 75, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 1, 3, 230, 105, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
						createSpider(&temp, 1, &nextEmptySpider, 0, 3, 0, 45, 0, 250);
						spiders[nextEmptySpider] = temp;
						nextEmptySpider++;
						if (nextEmptySpider >= 30) {
							nextEmptySpider = 0;
						}
					}
					generate = generate + generateGap + (rand() % 15);
					patternCount++;
					if (patternCount == 3 + randomness) {
						pattern = rand() % 6;
						randomness = rand() % 2;
						patternCount = 0;
					}

				}

				if (counter % (5 + (rand() % 15)) == 0 && bulletCooldown == 0 && generation == 1) {
					BULLET temp;
					int rng = rand() % 2;
					if (rng == 0) {
						createBullet(&temp, 0, 0, 0, 0, 0, 0, 0, 0, &player);
					} else {
						createBullet(&temp, 0, 0, 0, 0, 0, 0, 0, 1, &player);
					}
					bullets[nextEmptyBullet] = temp;
					nextEmptyBullet++;
					if (nextEmptyBullet >= 30) {
						nextEmptyBullet = 0;
					}
					bulletCooldown = bull;
				}

				if (playerHealth <= 0) {
					keepLooping = 0;
					state = 999;
				}

				if (counter == 1000) {
					spid = 50;
					bull = 50;
					deton = 50;
					lase = 100;
					generateGap = 95;
				} else if (counter == 2000) {
					spid = 40;
					bull = 40;
					deton = 40;
					lase = 80;
					generateGap = 75;
				} else if (counter == 3000) {
					spid = 30;
					bull = 30;
					deton = 30;
					lase = 60;
					generateGap = 55;
				} else if (counter == 4000) {
					spid = 20;
					bull = 10;
					deton = 20;
					lase = 50;
					generateGap = 40;
				}

				// Drawing

				// If player is invulnerable, flash the player to give player notice
				if (playerInvincibility > 0) {
					if (drawPlayer == 1) {
						drawPlayer = 0;
					} else {
						drawPlayer = 1;
					}
				} else {
					drawPlayer = 1;
				}
				int oldHealth = playerHealth;
				// Drawing the objects
				drawBasics(&player, &topBar, &middleBar, &bottomBar, &drawPlayer);
				updateSpiders(spiders, &playerHealth, &player, &playerInvincibility);
				updateBullets(bullets, &playerHealth, &player, &playerInvincibility);
				updateLasers(lasers, &playerHealth, &player, &playerInvincibility);
				playerMovement(&player, &upPressed, &downPressed, &topBar, &middleBar, &bottomBar);

				playerDamage = playerDamage + (oldHealth - playerHealth);

				if (KEY_DOWN_NOW(BUTTON_SELECT)) {
					keepLooping = 0;
					state = 0;
				}

				// Cooldown to prevent walls of spiders from appearing
				if (spiderCooldown != 0) {
					spiderCooldown--;
				}

				if (bulletCooldown != 0) {
					bulletCooldown--;
				}

				if (laserCooldown != 0) {
					laserCooldown--;
				}

				// If player gets hit, they get invincibility frames that last 100 loops. This decreases the remaining frames.
				if (playerInvincibility != 0) {
					playerInvincibility--;
				}

				if (counter % 30 == 0 && counter < 500) {
					playerHealth++;
					if (playerHealth > 100) {
						playerHealth = 100;
					}
				} else if (counter % 25 == 0 && counter >= 500 && counter < 1000) {
					playerHealth++;
					if (playerHealth > 100) {
						playerHealth = 100;
					}
				} else if (counter % 20 == 0 && counter >= 1000 && counter < 2000) {
					playerHealth++;
					if (playerHealth > 100) {
						playerHealth = 100;
					}
				} else if (counter % 15 == 0 && counter >= 2000 && counter < 3000) {
					playerHealth++;
					if (playerHealth > 100) {
						playerHealth = 100;
					}
				} else if (counter % 10 == 0 && counter >= 3000 && counter < 4000) {
					playerHealth++;
					if (playerHealth > 100) {
						playerHealth = 100;
					}
				} else if (counter % 5 == 0 && counter >= 4000 && counter < 5150) {
					playerHealth++;
					if (playerHealth > 100) {
						playerHealth = 100;
					}
				}

				// Displays HP
				char hpDisplay[20];
				sprintf(hpDisplay, "Health: %d", playerHealth);
				drawString4(150, 3, hpDisplay, 250);
				drawRect4(151, 75, 6, playerHealth, 246);
				drawRect4(151, 75 + playerHealth, 6, 100 - playerHealth, 247);

				// MODE 4
				waitForVblank();
				FlipPage();

				counter++;
				if (counter > 5000 && counter < 5150) {
					generation = 0;
				} else if (counter > 5150) {
					keepLooping = 0;
					state = 82;
				}
			}
		} else if (state == 82) { // Story END
			int keepLooping = 1;
			int counter = 0;
			int upPressed = 0;
			int downPressed = 0;
			int drawPlayer = 1;
			char text[50];
			char printing[50];
			char *textPointer = &text[0];
			int letters = 0;
			while (keepLooping) {
				CLEAR;

				// STORY TEXT HERE
				if (counter < 50) {
					memset(printing, 0, 50);
				}
				if (counter >= 50 && counter < 200) {
					sprintf(text, "Well...");
					printText(2);
					if (counter == 199) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 200 && counter < 350) {
					sprintf(text, "You took %d damage.", playerDamage);
					printText(2);
					if (counter == 349) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 350 && counter < 500) {
					sprintf(text, "What?");
					printText(2);
					if (counter == 499) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 500 && counter < 650) {
					sprintf(text, "You didn't think I was counting?");
					printText(2);
					if (counter == 649) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 650 && counter < 800) {
					sprintf(text, "At the very least...");
					printText(2);
					if (counter == 799) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 800 && counter < 950) {
					sprintf(text, "You persevered through.");
					printText(2);
					if (counter == 949) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 950 && counter < 1100) {
					sprintf(text, "You really are different.");
					printText(2);
					if (counter == 1099) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 1100 && counter < 1250) {
					sprintf(text, "You might be able to do");
					printText(2);
					if (counter == 1249) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 1250 && counter < 1400) {
					sprintf(text, "what I wasn't able to.");
					printText(2);
					if (counter == 1399) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 1400 && counter < 1550) {
					sprintf(text, "It's over. Go on ahead.");
					printText(2);
					if (counter == 1549) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 1550 && counter < 1700) {
					sprintf(text, "Greater evils await you.");
					printText(2);
					if (counter == 1699) {
						letters = 0;
						textPointer = &text[0];
						memset(printing, 0, 50);
					}
				} else if (counter >= 1700) {
					keepLooping = 0;
					state = 100;
				}

				if (counter >= 0 && counter < 80) {
					playerHealth++;
					if (playerHealth > 100) {
						playerHealth = 100;
					}
				}

				drawString4(20, 10, printing, 244);
				drawBasics(&player, &topBar, &middleBar, &bottomBar, &drawPlayer);
				playerMovement(&player, &upPressed, &downPressed, &topBar, &middleBar, &bottomBar);

				if (KEY_DOWN_NOW(BUTTON_SELECT)) {
					keepLooping = 0;
					state = 0;
				}

				// Increments counter of the loop, this is basically the timer
				counter++;
				if (counter > 987654) {
					counter = 0;
				}

				// Displays HP
				char hpDisplay[20];
				sprintf(hpDisplay, "Health: %d", playerHealth);
				drawString4(150, 3, hpDisplay, 250);
				drawRect4(151, 75, 6, playerHealth, 246);
				drawRect4(151, 75 + playerHealth, 6, 100 - playerHealth, 247);

				// MODE 4
				waitForVblank();
				FlipPage();
			}
		}
	}
}
