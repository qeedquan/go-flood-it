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
#ifndef BOARD_H
#define BOARD_H

#include <SDL/SDL.h>
#include "Globals.h"

class Board
{
	private:
		SDL_Surface *board_surface;
		int board[BOARD_SIZE][BOARD_SIZE];
		int oldcolor;
		int step_count;
		bool win; //set if win
	
		void Generate();
		void DrawBlock(SDL_Rect *rect, Uint8 r, Uint8 g, Uint8 b);
		void DrawBoard();
		bool CheckForWin();
	
	public:
		Board();
		~Board();
	
		int GetNumberColor(int i, int y){return board[i][y];};
		
		bool SetColorByCoordinate(int x, int y);
		
		bool SetColorByNumber(int color);
		
		void ChangeColor(int x, int y, int color);
		
		void OnRender(SDL_Surface *surf, int x, int y);
		
		void Reset();
		
		void IncCount(){step_count++;};
		int GetCount(){return step_count;};
		
		bool IsLose();
		bool IsWin();
};
#endif
