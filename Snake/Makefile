# Makefile du projet Serpent
#
#   make              compile le jeu et les tests
#   make verifier     compile puis lance les tests
#   make clean        efface les exécutables
#
# Sous Windows (MinGW) la commande s'appelle souvent mingw32-make.
# Sans make : lancer compiler.bat (Windows).

CC     = gcc
CFLAGS = -std=c11 -Wall -Wextra -g
RAYLIB ?= raylib

MODULES = src/a_faire.c src/alea.c src/types.c src/carte.c src/serpent.c \
          src/partie.c src/scores.c
ENTETES = $(wildcard src/*.h)

ifeq ($(OS),Windows_NT)
    EXE       = .exe
    LANCER    =
    RL_CFLAGS = -I$(RAYLIB)/include
    RL_LIBS   = -L$(RAYLIB)/lib -lraylib -lopengl32 -lgdi32 -lwinmm
    EFFACER   = cmd /C del /Q /F
else
    # Linux / macOS : raylib installée par le gestionnaire de paquets
    # (apt install libraylib-dev, brew install raylib...).
    EXE       =
    LANCER    = ./
    RL_CFLAGS = $(shell pkg-config --cflags raylib)
    RL_LIBS   = $(shell pkg-config --libs raylib) -lm
    EFFACER   = rm -f
endif

JEU   = serpent$(EXE)
TESTS = tests$(EXE)

all: $(TESTS) $(JEU)

$(TESTS): src/tests.c $(MODULES) $(ENTETES)
	$(CC) $(CFLAGS) -o $@ src/tests.c $(MODULES)

$(JEU): src/interface.c $(MODULES) $(ENTETES)
	$(CC) $(CFLAGS) $(RL_CFLAGS) -o $@ src/interface.c $(MODULES) $(RL_LIBS)

verifier: $(TESTS)
	$(LANCER)$(TESTS)

clean:
	$(EFFACER) $(JEU) $(TESTS)

.PHONY: all verifier clean
