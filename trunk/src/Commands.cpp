#include <string>
#include "Application.hpp"
#include "Room.hpp"
#include "Player.hpp"

DECLARE_FUNC(cmdNotFound)
{
   std::string output ("Huh?\n\r");
   p->send(output);
   return;
}

DECLARE_FUNC(north)
{
   std::string output;

   Room * r = p->room();
   Exit * e = r->getExit(EXIT_NORTH);

   if (!e) {
      output = "Can't go that way\n\r";
      p->send(output);
      return;
   }

   p->room(e->room());
   p->printRoom();
   return;
}

DECLARE_FUNC(south)
{
   std::string output;

   Room * r = p->room();
   Exit * e = r->getExit(EXIT_SOUTH);

   if (!e) {
      output = "Can't go that way\n\r";
      p->send(output);
      return;
   }

   p->room(e->room());
   p->printRoom();
   return;
}
