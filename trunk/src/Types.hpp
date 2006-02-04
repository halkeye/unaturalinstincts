#ifndef __UNRATURALINSTINCTS_TYPES_HPP_
#define __UNRATURALINSTINCTS_TYPES_HPP_

/*#include "../config.h"*/

#if !defined(HAVE_STL)
#   error "Can't find stl headers !"
#endif

#include <list>

class Player;

typedef std::list<Player *> PLAYERLIST;



#endif
