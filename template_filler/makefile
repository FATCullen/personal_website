CXX = g++
CXXFLAGS = -std=c++17 -Wall
CXX_SRCS := main.cpp contents.cpp filler.cpp html_filler.cpp gopher_filler.cpp gemini_filler.cpp
OBJECTS = $(CXX_SRCS:.cpp=.o)
EXECUTABLE = template_filler

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp contents.h filler.h html_filler.h gopher_filler.h gemini_filler.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
