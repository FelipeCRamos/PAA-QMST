# Makefile for the PAA NP-Hard project

# Authors:
# Felipe Ramos
# Paulo Augusto
#
# Makefile conventions
SHELL = /bin/sh

# Directories
INCDIR = ./include
SRCDIR = ./src
OBJDIR = ./obj
BINDIR = ./bin
DATADIR = ./data
# DOCDIR = ./Documentation

# Macros
CC = g++
CFLAGS = -Wall -w -g -ggdb -std=c++11 -I. -I$(INCDIR)
RM = -rm
PROJ_NAME = agmq
# DOC_NAME = index.html

HEADERS := $(wildcard $(INCDIR)/*)
SOURCES := $(wildcard $(SRCDIR)/*.cpp)
OBJECTS := $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)

all: project

project: $(OBJECTS) $(HEADERS) | $(BINDIR)
	$(CC) $(OBJECTS) $(CFLAGS) -o $(BINDIR)/$(PROJ_NAME)
	@ln -sfv $(BINDIR)/$(PROJ_NAME) $(PROJ_NAME)

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.cpp $(HEADERS) | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR):
	@mkdir -p $(OBJDIR)

$(BINDIR):
	@mkdir -p $(BINDIR)

# Clean PHONY targets
.PRONY: clean clean_proj

clean: clean_proj

clean_proj:
	@echo "Removing OBJDIR..."
	@$(RM) -r $(OBJDIR)
	@echo "Removing BINDIR..."
	@$(RM) -r $(BINDIR)
	@echo "Removing symlink..."
	@$(RM) -f $(PROJ_NAME)
	@echo "Clean-up completed!"
