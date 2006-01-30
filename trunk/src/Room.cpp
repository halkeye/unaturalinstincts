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
      _roomDescription = "Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Sed aliquet. Curabitur nonummy, velit id dignissim posuere, ligula arcu ultrices erat, et interdum mi sem interdum lectus. Duis sodales, tortor congue tincidunt vehicula, ante orci varius pede, at fermentum nulla nulla pellentesque velit. Aliquam mi. Duis eros tortor, mollis vel, eleifend et, rhoncus ac, leo. Aenean vestibulum orci ac odio. Suspendisse a massa. Suspendisse potenti. Aenean eleifend, risus eget aliquet elementum, arcu quam lacinia nisi, at auctor eros lectus eu purus. Donec ultricies, orci vel laoreet fringilla, purus tortor interdum neque, sed tempor massa enim nec nibh. Duis gravida augue sit amet libero. Praesent id est id metus rutrum sagittis. Quisque mauris orci, tempus ut, porta non, tempus vitae, lectus. Vivamus non est. Cras in nisi. Donec vulputate neque vitae orci. Ut blandit, felis non congue lobortis, turpis ligula ullamcorper odio, auctor lobortis dui lectus vitae urna. Mauris faucibus urna a est.";
   }
}
      
void Room::addExit(eExitTypes spot, Exit * e) {
   if (_exits[spot] != NULL) {
      delete _exits[spot];
      _exits[spot] = NULL;
   }
   _exits[spot] = e;
}

      
Exit * Room::getExit(eExitTypes pos) {
   return _exits[pos];
}
