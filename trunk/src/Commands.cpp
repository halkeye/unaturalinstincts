#include <string>
#include "Application.hpp"
#include "Player.hpp"

DECLARE_FUNC(cmdNotFound)
{
   std::string output ("Huh?\n\r");
   p->send(output);
   return;
}

DECLARE_FUNC(do_north)
{
   std::string output ("North\n\r");
   p->send(output);
   return;
}
