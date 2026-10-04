CXX = g++
CPPFLAGS = -std=c++17 -Wall -I../toml
CXX_SRCS := main.cpp content.cpp filler.cpp html_filler.cpp gopher_filler.cpp gemini_filler.cpp
OBJECTS = $(CXX_SRCS:.cpp=.o)
EXECUTABLE = template_filler

.PHONY: all clean
.SILENT: all clean

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(CPPFLAGS) -o $@ $^

%.o: %.cpp content.h filler.h html_filler.h gopher_filler.h gemini_filler.h
	$(CXX) $(CPPFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
