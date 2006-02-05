#ifndef __UNRATURALINSTINCTS_COMMANDS_HPP_
#define __UNRATURALINSTINCTS_COMMANDS_HPP_

#include "Types.hpp"

extern COMMANDHASH cmds;
struct Command {
      static void initCommands();

      /* Actual command name.. for pretty pretting */
      const char * name;
      /* Accepted aliases... will still be in the hash, but pointing to this struct */
      CHARLIST aliases;
      /* Code Name */
      const char * codeName;
      /* Pointer so we don't have to look up constantly */
      DO_FUN * doCode;

      void addAlias(const char * cmd);
      void code(const char * cmdname);
      static Command * getCommand(std::string);
};

#endif
