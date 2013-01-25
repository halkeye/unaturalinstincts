#include "config.h"

/* Perror */
#include <stdio.h>

/* read() */
#include <unistd.h>

#include "Application.hpp"
#include "Room.hpp"
#include "Player.hpp"

#include "AnsiColors.hpp"
#include "Commands.hpp"

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
   buf[ret] = '\0';

   _inbuf += buf;

   std::string::size_type char_newline = _inbuf.find ('\n');
   std::string::size_type char_return = _inbuf.find ('\n');

   std::string::size_type i = MIN_RANGE(char_newline, char_return);
   if (i != std::string::npos)
   {
      std::string::size_type space = _inbuf.rfind (' ',i);

      std::string cmd;
      std::string arguments;
      if (space != std::string::npos) {
         cmd = _inbuf.substr (0, space);  /* extract first line */
         /* Need a reusable function to translate spaces -> array of args */
         arguments = _inbuf.substr(space+1,i-space-2);
      }
      else {
         cmd = _inbuf.substr (0, i-1);  /* extract first line */
         arguments = "";
      }

      _inbuf = "";

      // process sLine
      Command * command = Command::getCommand(cmd);
      if (command) {
         debug("Command found");
         DO_FUN * fptr = command->doCode;
         if (fptr != cmdNotFound) {
            fptr(this, arguments);
            return;
         }
      }
      std::string output("no such command found\n\r");
      this->send(output);
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

