/*
 * Exported with nin10kit v1.1
 * Invocation command was nin10kit -mode=4 -start=223 -palette=17 story story.jpg 
 * Time-stamp: Monday 11/09/2015, 14:40:42
 * 
 * Image Information
 * -----------------
 * story.jpg 40@15
 * 
 * Quote/Fortune of the Day!
 * -------------------------
 * 
 * All bug reports / feature requests are to be sent to Brandon (bwhitehead0308@gmail.com)
 */

#ifndef STORY_H
#define STORY_H

#define STORY_PALETTE_OFFSET  223

extern const unsigned short story_palette[17];
#define STORY_PALETTE_SIZE 17

extern const unsigned short story[300];
#define STORY_SIZE 300
#define STORY_WIDTH 40
#define STORY_HEIGHT 15

#endif

