CXX = g++
CXXFLAGS = -std=c++17 -Wall
ALIBS = libcmark.a
CXX_SRCS := main.cpp gopher_renderer.cpp text_tools.cpp
OBJECTS = $(CXX_SRCS:.cpp=.o)
EXECUTABLE = test

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(ALIBS) $(CXXFLAGS) -o $@ $^ $(LFLAGS)

%.o: %.cpp cmark.h cmark_version.h cmark_export.h gopher_renderer.h text_tools.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
