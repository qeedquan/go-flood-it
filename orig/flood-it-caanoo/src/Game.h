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
#ifndef GAME_H
#define GAME_H

#include <SDL/SDL.h>
#include "SDL/SDL_ttf.h"
#include "Globals.h"
#include "Board.h"

class Game
{
	private:
		bool running;
		Board board;
		SDL_Surface *message;
		SDL_Color textColor;
	
		void DrawText(char *text, int x, int y);
		void ClearScreen();
		void DrawRect(int x, int y, int w, int h, Uint8 r, Uint8 g, Uint8 b);
		void DrawButton(char *text, int x, int y, int w, int h, int xt, int yt);
		void Reset();
	
	public:
		Game();
	
		bool OnInit();
		
		int OnExecute();
 
		void OnEvent(SDL_Event* Event);
 
		void OnExit();
 
		void OnLoop();
 
		void OnRender();
 
		void OnCleanup();
};

#endif
