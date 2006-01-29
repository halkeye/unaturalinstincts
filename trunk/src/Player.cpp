/* Perror */
#include <stdio.h>

/* read() */
#include <unistd.h>

#include <string>

#include "Room.hpp"
#include "Player.hpp"

Player::Player(int nfd) 
{
   _fd = nfd;
   _room = new Room();
}

void Player::onRead() {
   char buf[150];
   ssize_t ret = read(_fd, &buf, 151);
   if (ret == 0) { 
      /* Connection Dropped */
   }
   else if (ret == -1) {
      perror("read");
   }
}

void Player::printRoom() {
}
