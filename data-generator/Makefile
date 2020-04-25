CXX = g++
CFLAGS = -std=c++14 -g -Wall -Wextra -isystem./deps $(patsubst -I%,-isystem%,$(shell pkg-config --cflags libbitcoin))
LDFLAGS = $(shell pkg-config --libs libbitcoin)

GENERATOR_EXE = build/bcgen

ODIR = build
DEPS = deps/CLI11.hpp

HEADERS := $(wildcard src/*.h)
SRC := $(wildcard src/*.cc)
OBJ := $(patsubst src/%.cc,build/%.o,$(SRC))

all: deps $(GENERATOR_EXE)

deps: $(DEPS)

deps/CLI11.hpp:
	wget https://github.com/CLIUtils/CLI11/releases/download/v1.9.0/CLI11.hpp -O $@

$(ODIR)/%.o: src/%.cc
	$(CXX) $(CFLAGS) -c $< -o $@

$(GENERATOR_EXE): $(OBJ)
	$(CXX) -o $@ $^ $(CFLAGS) $(LDFLAGS)

-include $(OBJ:.o=.d)

clean:
	rm -f $(ODIR)/*.o $(DEPS) $(GENERATOR_EXE)
