############################################################################
### Project config
# Robot-only sources
ROBOT_SOURCES = $(addprefix src/robot/, \
	main.c )
# Simulation-only sources
SIM_SOURCES = $(addprefix src/simulation/, \
	RobolabSimClient.c )
# Shared sources (i.e. robot AND simulation)
SHARED_SOURCES = $(addprefix src/, \
	Algo.c )
# User include
USER_INC_PATH = ./h/


############################################################################
### DO NOT MODIFY THE FILE BELOW THIS LINE #################################
############################################################################

# OS name
UNAME := $(shell uname)
############################################################################
### Robo build config

# Sources for robot build
TARGET_SOURCES = $(ROBOT_SOURCES) $(SHARED_SOURCES)
TOPPERS_OSEK_OIL_SOURCE = ./main.oil


# Target specific macros
TARGET = main
O_PATH ?= build

ifeq ($(UNAME), Darwin)
	INC=/Applications/robolab/nxtOSEK/toppers_osek/kernel
	include /Applications/robolab/nxtOSEK/ecrobot/ecrobot.mak
else
ifeq ($(UNAME), Linux)
	# TODO: add Linux includes here!
else
	INC=/cygdrive/C/cygwin/nxtOSEK/toppers_osek/kernel
	include /cygdrive/C/cygwin/nxtOSEK/ecrobot/ecrobot.mak
endif
endif


############################################################################
### Simulation build config
# Simulation client include
SIMCLIENT_INCLUDE = simclient/h/
# Simulation client sources
SIMCLIENT_SOURCES = $(addprefix simclient/src/, \
	Communication.c RobotProxy.c Urlcode.c )

SIM_CC = gcc
SIM_RM = rm -f
SIM_CFLAGS = -v -Wall -g -I$(SIMCLIENT_INCLUDE)
SIM_LFLAGS = 
ifeq ($(UNAME), Darwin) # MAC
	SIM_CFLAGS_ = -ansi -I/usr/include/python2.7
	SIM_LFLAGS_ = -lpython2.7
	SIM_TARGET = RoboSimClient
else
ifeq ($(UNAME), Linux) # Linux
	SIM_CFLAGS_ = -I/usr/include/python2.7
	SIM_LFLAGS_ = -lpython2.7
	SIM_TARGET = RoboSimClient
else # windows
	SIM_CFLAGS_ = -IC:/Python27/include
	SIM_LFLAGS_ = -LC:/Python27/libs -lpython27
	SIM_TARGET = RoboSimClient.exe
endif
endif

simulation:
	$(SIM_CC)														\
	$(SIM_CFLAGS) $(SIM_CFLAGS_) -I$(USER_INC_PATH)					\
	-o$(SIM_TARGET)													\
	$(SIM_SOURCES) $(SIMCLIENT_SOURCES) $(SHARED_SOURCES)			\
	$(SIM_LFLAGS) $(SIM_LFLAGS_)
simulation-clean:
	$(SIM_RM) $(SIM_TARGET)

