#include "config.h"

#include <string.h>
#include <string>
#include "Types.hpp"

#include "Application.hpp"
#include "Room.hpp"
#include "Player.hpp"
#include "Commands.hpp"


COMMANDHASH cmds;
void Command::initCommands() {
   Command * cmd;
   {
      debug("Adding look command");
      cmd = new Command;
      cmd->code("look");
      cmd->addAlias("l");
      cmds.insert(std::make_pair("look", cmd));
   }

   {
      cmd = new Command;
      cmd->code("north");
      cmd->addAlias("n");
      cmds.insert(std::make_pair("north", cmd));
   }
   
   {
      cmd = new Command;
      cmd->code("south");
      cmd->addAlias("s");
      cmds.insert(std::make_pair("south", cmd));
   }
   {
      cmd = new Command;
      cmd->code("say");
      cmd->addAlias(";");
      cmds.insert(std::make_pair("say", cmd));
   }


}
     
Command * Command::getCommand(std::string cmd)
{
   const char * tmp = cmd.c_str();
//   const char * tmp = "look";
   debug("Trying to find %s", tmp);
   COMMANDHASH::const_iterator command_iter = cmds.find(tmp);
   if (command_iter != cmds.end()) {
      return command_iter->second;
   }
   return NULL;
}

void Command::addAlias(const char *cmd) {
   this->aliases.push_back(cmd);
   cmds.insert(std::make_pair(cmd, this));
}

void Command::code(const char * cmdname) {
   DO_FUN * fptr = getApplication()->getCommand(cmdname);
   if (fptr == cmdNotFound) {
      debug("no such command(%s) found", cmdname);
      return;
   }
   this->doCode = fptr;
   this->name = cmdname;
   this->codeName = cmdname;
}

CMDF cmdNotFound(Player * p, std::string arguments)
{
   arguments = "";
   std::string output ("Huh?\n\r");
   p->send(output);
   return;
}

CMDF north(Player * p, std::string arguments)
{
   std::string output;
   arguments = "";

   const Room * r = p->room();
   Exit * e = r->getExit(EXIT_NORTH);

   if (!e) {
      output = "Can't go that way\n\r";
      p->send(output);
      return;
   }

   p->room(e->room());
   p->room()->showToPlayer(p);
   return;
}

CMDF south(Player * p, std::string arguments)
{
   std::string output;
   arguments = "";

   const Room * r = p->room();
   Exit * e = r->getExit(EXIT_SOUTH);

   if (!e) {
      output = "Can't go that way\n\r";
      p->send(output);
      return;
   }

   p->room(e->room());
   p->room()->showToPlayer(p);
   return;
}

CMDF quit(Player * p, std::string arguments)
{
   arguments = "";
   std::string output;
   output = "Quitting\n";
   p->send(output);
   p->close();
}

CMDF look(Player * p, std::string arguments)
{
   arguments = "";
   p->room()->showToPlayer(p);
}

CMDF say(Player * p, std::string arguments)
{
   std::string output;
   output = p->name() + "says '" + arguments + "'\n";
   p->room()->echo(output, p);

   output = "You say '" + arguments + ".'\n";
   p->send(output);
}
