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
   _room = NULL;
   _room = new Room();
   Room * r = new Room();
   _room->addExit(EXIT_NORTH,r,true);
}

void Player::onRead() {
   char buf[1000];
   ssize_t ret = read(_fd, &buf, 1000);
   if (ret == 0) { 
      /* Connection Dropped */
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

void Player::printRoom() {
   std::string output = AnsiColors::RESET;
   if (!_room) {
      output += "No room thingie\n\r";
      this->send(output);
      return;
   }

   output += AnsiColors::RED + _room->name() + AnsiColors::RESET + "\n\r";
   output += AnsiColors::BLUE;
   for (unsigned int i = 0; i < _room->name().length(); i++) {
      output += "=";
   }
   output += AnsiColors::RESET;

   output += "\n\r";
   output += _room->description();
   output += "\n\r\n\r";
   output += "Exits\n\r";
   output += AnsiColors::BLUE;
   output += "=====";
   output += AnsiColors::RESET;
   output += "\n\r";

   for (int i = EXIT_NORTHWEST; i < EXIT_NONE; i++) {
      Exit * e = _room->getExit((eExitTypes)i);
      if (!e) {
         continue;
      }
      output += "" + std::string(sExitTypes[i]) + " --- " + e->roomName() + "\n\r";
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

