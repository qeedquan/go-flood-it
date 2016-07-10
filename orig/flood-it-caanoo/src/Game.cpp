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
#include "Game.h"

Game::Game() 
{ 
	running = true;
	Board board;
	message = NULL;
	
	//Set white text color
	textColor.r = 255;
	textColor.g = 255;
	textColor.b = 255;
}
 
int Game::OnExecute() 
{
	if(OnInit() == false) {
		return 1;
	}
 
	SDL_Event Event;
 
	while(running) {
		while(SDL_PollEvent(&Event)) {
			OnEvent(&Event);
		}
 
		OnLoop();
		OnRender();
		SDL_Delay(100);
	}
 
	OnCleanup();
 
	return 0;
}

void Game::OnCleanup()
{
	SDL_FreeSurface(message);
	TTF_CloseFont(font);
	SDL_FreeSurface(screen);
	
#ifdef CAANOO
	SDL_JoystickClose(joystick);
#endif
	
	TTF_Quit();
	SDL_Quit();
}

bool Game::OnInit() //Initialization SDL
{
	if(SDL_Init(SDL_INIT_VIDEO) != 0) {
		fprintf(stderr,"FATAL ERROR: SDL_Init failed\n");
		return false;
	}
	
	if (TTF_Init() == -1) {
		fprintf(stderr,"FATAL ERROR: TTF_Init failed\n");
		return false;
	}
	
	font = TTF_OpenFont("Vera.ttf", 11);
	if (font == NULL){
		fprintf(stderr,"FATAL ERROR: Unable to load font: %s \n", TTF_GetError());
		return false;
	}
#ifdef CAANOO
	SDL_ShowCursor(SDL_DISABLE);			//Hide cursor
	
	SDL_InitSubSystem(SDL_INIT_JOYSTICK);	// Initialize caanoo joystick
	if (SDL_NumJoysticks() > 0) {
		joystick = SDL_JoystickOpen(0);
		if(!joystick){
			fprintf (stderr,"FATAL ERROR: Couldn't open joystick.\n");
			return false;
		}
	}
#endif

	SDL_WM_SetCaption("flood-it!", "flood-it!");
	
	if((screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE | SDL_DOUBLEBUF)) == NULL) {
		fprintf(stderr,"FATAL ERROR: Set video mode failed\n");
		return false;
	}
	
	return true;
}

void Game::OnRender()
{	
	ClearScreen();
	
	//draw board
	board.OnRender(screen, OFFSET_BOARD_X, OFFSET_BOARD_Y);
	
	//draw text
	char text[50];
	sprintf(text, "Step: %d%s", board.GetCount(), "/25");
	DrawText(text, 244, 8);
	
	if (board.IsLose()){
		sprintf(text, "You lose");
		DrawText(text, 250, 30);
	}
	
	if (board.IsWin()){
		sprintf(text, "You win");
		DrawText(text, 253, 30);
	}
		
	DrawButton("New game", 240, 180, 72, 20, 246, 183);
	DrawButton("Exit", 240, 212, 72, 20, 267, 215);
	
	DrawRect(240, 56, 32, 32, 0, 0, 0xff);			//blue button
	DrawRect(280, 56, 32, 32, 0, 0x80, 0);			//green button
	DrawRect(240, 96, 32, 32, 0xff, 0xa5, 0);		//orange button
	DrawRect(280, 96, 32, 32, 0xff, 0, 0);			//red button
	DrawRect(240, 136, 32, 32, 0xee, 0x82, 0xee);	//violet button
	DrawRect(280, 136, 32, 32, 0xff, 0xff, 0);		//yellow button
	
    SDL_Flip(screen);
}

void Game::DrawText(char *text, int x, int y)
{
	message = TTF_RenderText_Solid( font, text, textColor );
	SDL_Rect dest;
    dest.x = x;
    dest.y = y;
	SDL_BlitSurface(message, NULL, screen, &dest);
}

void Game::DrawRect(int x, int y, int w, int h, Uint8 r, Uint8 g, Uint8 b)
{
	SDL_Rect rect;
	rect.x=x;
	rect.y=y;
	rect.w=w;
	rect.h=h;
	SDL_FillRect(screen, &rect, SDL_MapRGB(screen->format, r, g, b));
}

void Game::DrawButton(char *text, int x, int y, int w, int h, int xt, int yt)
{
	DrawRect(x, y, w, h, 255, 255, 255);
	DrawRect(x+1, y+1, w-2, h-2, 0, 0, 0);
	DrawText(text, xt, yt);
}

void Game::ClearScreen()
{
	SDL_Rect dest;
	dest.x = 0;
	dest.y = 0;
	dest.w = SCREEN_WIDTH;
    dest.h = SCREEN_HEIGHT;
	SDL_FillRect(screen, &dest, 0);
}

void Game::OnEvent(SDL_Event* Event)
{
	switch(Event->type) {
		case SDL_QUIT:
			OnExit();
			break;
		case SDL_MOUSEBUTTONDOWN:
			if (Event->button.button = SDL_BUTTON_LEFT){
				int x = Event->button.x, y = Event->button.y;
	
				// click on board
				if ((x < BLOCK_SIZE * BOARD_SIZE +OFFSET_BOARD_X && y < BLOCK_SIZE * BOARD_SIZE +OFFSET_BOARD_X) && (x > OFFSET_BOARD_X && y > OFFSET_BOARD_Y))
					board.SetColorByCoordinate((x-OFFSET_BOARD_X)/BLOCK_SIZE, (y-OFFSET_BOARD_Y)/BLOCK_SIZE);

				//click on new game button
				if ((x < 240+72 && y < 180+20)&&(x > 240 && y > 180))
					Reset();
					
				//click on exit button
				if ((x < 240+72 && y < 212+20)&&(x > 240 && y > 212)) 
					OnExit();
					
				//click on blue button
				if ((x < 240+32 && y < 56+32)&&(x > 240 && y > 56))
					board.SetColorByNumber(0);
					
				//click on green button
				if ((x < 280+32 && y < 56+32)&&(x > 280 && y > 56))
					board.SetColorByNumber(1);
					
				//click on orange button
				if ((x < 240+32 && y < 96+32)&&(x > 240 && y > 96))
					board.SetColorByNumber(2);
					
				//click on red button
				if ((x < 280+32 && y < 96+32)&&(x > 280 && y > 96))
					board.SetColorByNumber(3);
					
				//click on violet button
				if ((x < 240+32 && y < 136+32)&&(x > 240 && y > 136))
					board.SetColorByNumber(4);
					
				//click on yellow button
				if ((x < 280+32 && y < 136+32)&&(x > 280 && y > 136))
					board.SetColorByNumber(5);
			}
			break;
#ifdef CAANOO
		case SDL_JOYBUTTONDOWN:
			switch(Event->jbutton.button){
				case CAANOO_BUTTON_HOME:
					OnExit();
					break;
			}
#endif
	}
}

void Game::OnExit()
{
	running = false;
}

void Game::OnLoop()
{

}

void Game::Reset()
{
	board.Reset();
}