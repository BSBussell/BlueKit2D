#Executable Name
BUILDNAME = main

#Directories
OBJDIR = obj

#CC specifies which compiler we're using
CC = g++

# Grabs all the files in src
src = $(wildcard src/*.cpp)
src := $(wildcard src/*/*.cpp)

obj := $(notdir $(src))
obj := $(addprefix $(OBJDIR)/, $(obj))
obj := $(obj:.cpp=.o)
		 
#CFLAGS specifies the additional compilation options we're using
CFLAGS := -Wall -Wextra -std=c++17 -O3

#IFLAGS specifies which directory to check for include
IFLAGS := -Iinc

#LFLAGS specify the libraries we're linking against
LFLAGS  := -lSDL2 -lSDL2_image -lSDL2_mixer -lBML


#Give us some bold text plz
bold := $(shell tput bold)
sgr0 := $(shell tput sgr0)

#Get OS name
OS := $(shell uname)

# Pick Library Path Based on it, generally not need but incase lol
ifeq ($(OS), Darwin)
	LPATH = /usr/local/lib
else
	LPATH = /usr/lib
endif	

#all rule for just compiling everything
.PHONY: all
all: OBJ build
	

# What we do when building obj's
.PHONY: OBJ
OBJ: $(obj)


# Building the Actual Executable
.PHONY: build
build: OBJ
	@printf "\n$(bold)----------COMPILING EXECUTABLE: $@----------$(sgr0)\n"
	$(CC) $(CFLAGS) $(IFLAGS) $(LFLAGS)  -o bin/$(BUILDNAME) $(obj)


# Rules for obj files
$(OBJDIR)/%.o: src/%.cpp
	@printf "\n$(bold)----------COMPILING OBJ FILE: $(notdir $@)----------$(sgr0)\n"
	$(CC) $^ $(CFLAGS) $(IFLAGS) -c -o $@ 

.PHONY: clean
clean: 
	@printf "\n$(bold)----------REMOVING PREVIOUS BUILDS----------$(sgr0)\n"
	rm -f $(obj) 
	rm -f bin/*
	rm -f a.out
