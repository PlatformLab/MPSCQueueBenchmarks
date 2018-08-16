CC = g++
CXX ?= g++
CXXFLAGS=-std=c++11 -O3 -Wall -Werror -Wformat=2 -Wextra -Wwrite-strings -Wno-unused-parameter -Wmissing-format-attribute -Wno-non-template-friend -Woverloaded-virtual -Wcast-qual -Wcast-align -Wconversion -fomit-frame-pointer $(EXTRA_CXXFLAGS)

# Input and output directory
SRC_DIR = src
BIN_DIR = bin
OBJECT_DIR = obj

# Dependencies
PERFUTILS=../PerfUtils
UTHREAD_MPSC=../MPSCQ/src
WAIT_FREE_QUEUE=../waitfree-mpsc-queue

CPPFLAGS=-I$(PERFUTILS)/include -I$(UTHREAD_MPSC) -I$(WAIT_FREE_QUEUE) -I$(SRC_DIR)
LDLIBS=$(PERFUTILS)/lib/libPerfUtils.a $(WAIT_FREE_QUEUE)/libmpscq.a -lpcrecpp -pthread

BINS = EnqueueToDequeLatencyDist
FULL_BINS = $(patsubst %,$(BIN_DIR)/%,$(BINS))

# Objects used in the binaries
OBJECT_NAMES=EnqueueToDequeLatencyDist.o
OBJECTS = $(patsubst %,$(OBJECT_DIR)/%,$(OBJECT_NAMES))
HEADERS= $(shell find $(SRC_DIR) $(WRAPPER_DIR) -name '*.h')

DEP=$(OBJECTS:.o=.d)

all: $(FULL_BINS)

$(BIN_DIR)/EnqueueToDequeLatencyDist : $(OBJECT_DIR)/EnqueueToDequeLatencyDist.o | $(BIN_DIR)
	$(CXX) -o $@ $< $(LDLIBS)

$(OBJECT_DIR)/%.o: $(SRC_DIR)/%.cc $(HEADERS) | $(OBJECT_DIR)
	$(CXX) -c $(CXXFLAGS) $(CPPFLAGS) -o $@ $<

-include $(DEP)

$(OBJECT_DIR)/%.d: $(SRC_DIR)/%.cc | $(OBJECT_DIR)
	$(CXX)  $(CXXFLAGS) $(CPPFLAGS) $< -MM -MT $(@:.d=.o) > $@

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(OBJECT_DIR):
	mkdir -p $(OBJECT_DIR)

clean:
	rm -rf $(BIN_DIR) $(OBJECT_DIR)

# The following target is useful for debugging Makefiles; it
# prints the value of a make variable.
print-%:
	@echo $* = $($*)
