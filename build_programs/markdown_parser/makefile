CPP = g++
CC = gcc
CPPFLAGS = -std=c++17 -Wall -I../cmark/build -I../cmark
CFLAGS = -std=c11 -Wall -DCMARK_STATIC_DEFINE -I../cmark/build -I../cmark
CMARK_DIR := ../cmark
CMARK_BUILD_DIR := $(CMARK_DIR)/build
CMARK_SRCS := $(filter-out $(CMARK_DIR)/main.c,$(wildcard $(CMARK_DIR)/*.c))
CMARK_OBJS := $(patsubst $(CMARK_DIR)/%.c,$(CMARK_BUILD_DIR)/%.o,$(CMARK_SRCS))

CPP_SRCS := main.cpp gopher_renderer.cpp text_tools.cpp gemini_renderer.cpp html_renderer.cpp
OBJECTS = $(CPP_SRCS:.cpp=.o)
EXECUTABLE = markdown_parser

.PHONY: all clean
.SILENT: all clean

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) $(CMARK_OBJS)
	$(CPP) $(CPPFLAGS) -o $@ $(OBJECTS) $(CMARK_OBJS)

$(CMARK_BUILD_DIR):
	mkdir -p $(CMARK_BUILD_DIR)

$(CMARK_BUILD_DIR)/cmark_version.h: $(CMARK_DIR)/cmark_version.h.in | $(CMARK_BUILD_DIR)
	printf '%s\n' '#ifndef CMARK_VERSION_H' '#define CMARK_VERSION_H' '' '#define CMARK_VERSION ((0 << 16) | (0 << 8) | 0)' '#define CMARK_VERSION_STRING "0.0.0"' '' '#endif' > $@

$(CMARK_BUILD_DIR)/cmark_export.h: | $(CMARK_BUILD_DIR)
	printf '%s\n' '#ifndef CMARK_EXPORT_H' '#define CMARK_EXPORT_H' '' '#if defined(_WIN32) && defined(CMARK_STATIC_DEFINE)' '#define CMARK_EXPORT' '#elif defined(_WIN32)' '#define CMARK_EXPORT __declspec(dllexport)' '#else' '#define CMARK_EXPORT' '#endif' '' '#endif' > $@

$(CMARK_BUILD_DIR)/%.o: $(CMARK_DIR)/%.c $(CMARK_BUILD_DIR)/cmark_export.h $(CMARK_BUILD_DIR)/cmark_version.h | $(CMARK_BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.cpp gopher_renderer.h text_tools.h gemini_renderer.h html_renderer.h $(CMARK_BUILD_DIR)/cmark_export.h $(CMARK_BUILD_DIR)/cmark_version.h
	$(CPP) $(CPPFLAGS) -c -o $@ $<

clean:
	rm -rf $(CMARK_BUILD_DIR) $(OBJECTS) $(EXECUTABLE)
