/****************************************************************************
** Copyright (C) 2012 Boris Timofeev <mashin87@gmail.com> www.emunix.org
**
**
** This file may be used under the terms of the GNU General Public
** License version 3.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of
** this file.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
**  You should have received a copy of the GNU General Public License
**  along with this program.  If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/
#ifndef GLOBALS_H
#define GLOBALS_H

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

//Global constants
//The width, height and depth of the screen.
const int SCREEN_WIDTH=320;
const int SCREEN_HEIGHT=240;
const int SCREEN_BPP=16;

const int BOARD_SIZE=14;
const int BLOCK_SIZE=16;

//the offset of the board relative to the screen
const int OFFSET_BOARD_X=8;
const int OFFSET_BOARD_Y=8;

extern SDL_Surface *screen;	//Screen surface
extern TTF_Font *font;

enum Colors {
	COLOR_BLUE = 0,			// #0000FF
	COLOR_GREEN = 1,		// #008000
	COLOR_ORANGE = 2,		// #FFA500
	COLOR_RED = 3,			// #FF0000
	COLOR_VIOLET = 4,		// #EE82EE
	COLOR_YELLOW = 5		// #FFFF00
};
	
#ifdef CAANOO
extern SDL_Joystick *joystick;
	
enum CaanooButtons {
	CAANOO_BUTTON_A = 0,
	CAANOO_BUTTON_X = 1,
	CAANOO_BUTTON_B = 2,
	CAANOO_BUTTON_Y = 3,
	CAANOO_BUTTON_L = 4,
	CAANOO_BUTTON_R = 5,
	CAANOO_BUTTON_HOME = 6,
	CAANOO_BUTTON_1 = 8,
	CAANOO_BUTTON_2 = 9,
	CAANOO_BUTTON_JOY = 10
};
#endif
#endif
