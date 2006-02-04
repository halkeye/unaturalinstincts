#include "config.h"

/* Perror */
#include <stdio.h>

/* read() */
#include <unistd.h>

#include "Application.hpp"
#include "Room.hpp"
#include "Player.hpp"

#include "AnsiColors.hpp"

Player::Player(int nfd) 
{
   _fd = nfd;
   _state = CONN_CONNECTING;

   _room = room1;
   if (_room) 
      _room->addPlayer(this);
}

void Player::onRead() {
   char buf[1000];
   ssize_t ret = read(_fd, &buf, 1000);
   if (ret == 0) { 
      /* Connection Dropped */
      close();
      return;
   }
   else if (ret == -1) {
      perror("read");
      fflush(stderr);
      exit(1);
      return;
   }

   _inbuf += buf;

   std::string::size_type i = _inbuf.find ('\n');
   if (i != std::string::npos)
   {
      std::string sLine = _inbuf.substr (0, i-1);  /* extract first line */
      _inbuf.clear();

      // process sLine
      DO_FUN * fptr = getApplication()->getCommand((char *)sLine.c_str());
      if (fptr != cmdNotFound) {
         fptr(this, std::string(""));
      }
      else {
         std::string output("no such command found\n\r");
         this->send(output);
      }
   }
}

void Player::send(std::string &str)
{
   ssize_t ret = write(fd(), (char *) str.c_str(), str.length());
   /* Fixme ALOT */
   if (ret == 0) { 
      /* Connection Dropped */
      close();
      return;
   }
   else if (ret == -1) {
      perror("read");
   }
}

void Player::onConnect() {
   /* Do the stuff that is needed to be done when a player connects */

   /* Setup connection status */
   _room->showToPlayer(this);
}

/**
 * @brief Closes a player connection
 **/
void Player::close() {
   /* Remove me from Appliaction->fds */
   getApplication()->queueRemove(this);
}

/**
 * @brief room
 *
 * Gets / Sets the room for a player
 *
 * @param r (optional) New room for the player
 *
 * @return room the player is currently in
 **/
      
const Room * Player::room(Room * r) {
   if (r != NULL) {
      if (_room != NULL) {
         _room->removePlayer(this);
      }
      _room = r;
      _room->addPlayer(this);
   }
   return _room; 
} 

