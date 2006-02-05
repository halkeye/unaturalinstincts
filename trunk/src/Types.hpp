#ifndef __UNRATURALINSTINCTS_TYPES_HPP_
#define __UNRATURALINSTINCTS_TYPES_HPP_

/*#include "../config.h"*/

#if !defined(HAVE_STL)
#   error "Can't find stl headers !"
#endif

#include <list>
#include <map>
#include <string>

class Player;
struct Command;

struct ltstr
{
   bool operator()(const char* s1, const char* s2) const
   {
      return strcmp(s1, s2) < 0;
   }
};

typedef std::list<Player *> PLAYERLIST;
typedef std::list<const char *> CHARLIST;
typedef std::list<const char *> STRINGLIST;
typedef std::map<const char *, Command *, ltstr> COMMANDHASH;

#endif
