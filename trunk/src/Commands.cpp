#include <string>
#include "Application.hpp"
#include "Room.hpp"
#include "Player.hpp"

CMDF cmdNotFound(Player * p, std::string arguments)
{
   std::string output ("Huh?\n\r");
   p->send(output);
   return;
}

CMDF north(Player * p, std::string arguments)
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

CMDF south(Player * p, std::string arguments)
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
