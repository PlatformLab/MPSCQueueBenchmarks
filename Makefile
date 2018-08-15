CC = g++
CXX ?= g++
CXXFLAGS=-std=c++11 -O3 -Wall -Werror -Wformat=2 -Wextra -Wwrite-strings -Wno-unused-parameter -Wmissing-format-attribute -Wno-non-template-friend -Woverloaded-virtual -Wcast-qual -Wcast-align -Wconversion -fomit-frame-pointer $(EXTRA_CXXFLAGS)

# Input and output directory
SRC_DIR = src
BIN_DIR = bin

# Dependencies
PERFUTILS=../PerfUtils
UTHREAD_MPSC=../MPSCQ/src

CPPFLAGS=-I$(PERFUTILS)/include -I$(UTHREAD_MPSC)
LDLIBS=$(PERFUTILS)/lib/libPerfUtils.a -lpcrecpp -pthread

BINS = EnqueueToDequeLatencyDist
FULL_BINS = $(patsubst %,$(BIN_DIR)/%,$(BINS))

all: $(FULL_BINS)

$(BIN_DIR)/EnqueueToDequeLatencyDist : $(BIN_DIR)/EnqueueToDequeLatencyDist.o

$(BIN_DIR)/%.o : $(SRC_DIR)/%.cc | $(BIN_DIR)
	$(CXX) -c $(CXXFLAGS) $(CPPFLAGS) -o $@ $<

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

clean:
	rm -rf $(BIN_DIR)
