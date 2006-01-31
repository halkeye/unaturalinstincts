bin_PROGRAMS = unaturalinstincts

#AM_CXXFLAGS = -gdwarf-23 -W -Wall -pedantic -fno-inline
AM_CXXFLAGS = -g3 -W -Wall -pedantic -fno-inline

noinst_HEADERS = Room.hpp Player.hpp Application.hpp AnsiColors.hpp
unaturalinstincts_SOURCES = Application.cpp Room.cpp Player.cpp \
			    Commands.cpp

#unaturalinstincts_LDADD   = -ldl
#unaturalinstincts_LDFLAGS = -export-dynamic
unaturalinstincts_LDFLAGS = -export-all
