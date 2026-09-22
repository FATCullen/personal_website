CXX = g++
CXXFLAGS = -std=c++17 -Wall
ALIBS = libcmark.a
CXX_SRCS := main.cpp gopher_renderer.cpp text_tools.cpp gemini_renderer.cpp html_renderer.cpp
OBJECTS = $(CXX_SRCS:.cpp=.o)
EXECUTABLE = markdown_parser

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(ALIBS) $(CXXFLAGS) -o $@ $^

%.o: %.cpp cmark.h cmark_version.h cmark_export.h gopher_renderer.h text_tools.h gemini_renderer.h html_renderer.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
