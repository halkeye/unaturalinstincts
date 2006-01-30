#include <string>

#include "Room.hpp"

const char * sExitTypes[EXIT_NONE] = {
   "northwest", "north", "northeast", "east",
   "southeast", "south", "southwet",  "west",
   "other"
};

Room::Room(std::string name) 
{
   _roomName = name;
   Room::Room();
   for (int i = 0; i < EXIT_NONE; i++) {
      _exits[i] = NULL;
   }
}

Room::Room() 
{
   if (_roomName.empty()) {
      _roomName = "Unamed Room";
   }
   
   if (_roomDescription.empty()) {
      _roomDescription = "Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Sed aliquet. Curabitur nonummy, velit id dignissim posuere, ligula arcu ultrices erat, et interdum mi sem interdum lectus. Duis sodales, tortor congue tincidunt vehicula, ante orci varius pede, at fermentum nulla nulla pellentesque velit. Aliquam mi. Duis eros tortor, mollis vel, eleifend et, rhoncus ac, leo. Aenean vestibulum orci ac odio. Suspendisse a massa.";
   }
}
      
void Room::addExit(eExitTypes spot, Exit * e) {
   if (_exits[spot] != NULL) {
      delete _exits[spot];
      _exits[spot] = NULL;
   }
   _exits[spot] = e;
}

void Room::addExit(eExitTypes spot, Room * r, bool biDirectional) {
   Exit * e = new Exit();
   e->room(r);
   addExit(spot, e);

   if (biDirectional) {
      Exit * e2 = new Exit();
      e2->room(this);
      r->addExit(Exit::getOpposite(spot), e2);
   }
}

      
Exit * Room::getExit(eExitTypes pos) {
   return _exits[pos];
}
   
eExitTypes Exit::getOpposite(eExitTypes pos) {
   switch (pos) {
      case EXIT_NORTHWEST:
         return EXIT_SOUTHEAST;
      case EXIT_NORTH:
         return EXIT_SOUTH;
      case EXIT_NORTHEAST:
         return EXIT_SOUTHWEST;
      case EXIT_EAST:
         return EXIT_WEST;

      case EXIT_SOUTHEAST:
         return EXIT_NORTHWEST;
      case EXIT_SOUTH:
         return EXIT_NORTH;
      case EXIT_SOUTHWEST:
         return EXIT_NORTHEAST;
      case EXIT_WEST:
         return EXIT_EAST;
      case EXIT_OTHER:
         return EXIT_OTHER;
      default:
         throw("Unknown exit type");
         return EXIT_NONE;
   }
}
