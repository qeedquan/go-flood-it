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
#include "Globals.h"
#include <SDL/SDL.h>

SDL_Surface *screen = NULL;
TTF_Font *font = NULL;

#ifdef CAANOO
SDL_Joystick* joystick = NULL;
#endif
