/* Perror */
#include <stdio.h>

/* read() */
#include <unistd.h>

#include "Room.hpp"
#include "Player.hpp"

Player::Player(int nfd) 
{
   _fd = nfd;
   _room = new Room();
   _room->addExit(EXIT_NORTH, new Exit());
}

void Player::onRead() {
   char buf[1000];
   ssize_t ret = read(_fd, &buf, 1000);
   if (ret == 0) { 
      /* Connection Dropped */
   }
   else if (ret == -1) {
      perror("read");
      exit(1);
      return;
   }

   _inbuf += buf;

   std::string::size_type i = _inbuf.find ('\n');
   if (i != std::string::npos)
   {
      std::string sLine = _inbuf.substr (0, i);  /* extract first line */
      _inbuf.clear();
      // process sLine
   }
}

void Player::printRoom() {
   std::string output = "";

   output += _room->name() + "\n\r";
   for (unsigned int i = 0; i < _room->name().length(); i++) {
      output += "=";
   }

   output += "\n\r";
   output += _room->description();
   output += "\n\r\n\r";
   output += "Exits\n\r";
   output += "=====\n\r";

   for (int i = EXIT_NORTHWEST; i < EXIT_NONE; i++) {
      Exit * e = _room->getExit((eExitTypes)i);
      if (!e) {
         continue;
      }
      output += "" + std::string(sExitTypes[i]) + " --- " + e->name() + "\n\r";
   }

   this->send(output);
}

void Player::send(std::string &str)
{
   ssize_t ret = write(fd(), (char *) str.c_str(), str.length());
   /* Fixme ALOT */
   if (ret == 0) { 
      /* Connection Dropped */
   }
   else if (ret == -1) {
      perror("read");
   }
}

void Player::onConnect() {
   /* Do the stuff that is needed to be done when a player connects */

   /* Setup connection status */
   printRoom();
}

void Player::close() {
   /* Remove me from Appliaction->fds */
}

