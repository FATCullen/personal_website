CPP = g++
CC = gcc
CPPFLAGS = -std=c++17 -Wall -DCMARK_GFM_STATIC_DEFINE -I../cmark-gfm/build -I../cmark-gfm
CFLAGS = -std=c11 -Wall -DCMARK_GFM_STATIC_DEFINE -I../cmark-gfm/build -I../cmark-gfm
CMARK_DIR := ../cmark-gfm
CMARK_BUILD_DIR := $(CMARK_DIR)/build
CMARK_VERSION_MAJOR := 0
CMARK_VERSION_MINOR := 29
CMARK_VERSION_PATCH := 0
CMARK_VERSION_GFM := 13
CMARK_GENERATED_HEADERS := $(CMARK_BUILD_DIR)/cmark-gfm_export.h $(CMARK_BUILD_DIR)/cmark-gfm_version.h $(CMARK_BUILD_DIR)/config.h
CMARK_SRCS := $(filter-out $(CMARK_DIR)/main.c,$(wildcard $(CMARK_DIR)/*.c))
CMARK_OBJS := $(patsubst $(CMARK_DIR)/%.c,$(CMARK_BUILD_DIR)/%.o,$(CMARK_SRCS))

CPP_SRCS := main.cpp gopher_renderer.cpp text_tools.cpp gemini_renderer.cpp html_renderer.cpp node_processing.cpp table_renderer.cpp
OBJECTS = $(CPP_SRCS:.cpp=.o)
EXECUTABLE = markdown_parser

.PHONY: all clean
.SILENT: all clean

all: $(EXECUTABLE)

table_renderer.o: ../tabulate/tabulate.hpp

$(EXECUTABLE): $(OBJECTS) $(CMARK_OBJS)
	$(CPP) $(CPPFLAGS) -o $@ $(OBJECTS) $(CMARK_OBJS)

$(CMARK_BUILD_DIR):
	mkdir -p $(CMARK_BUILD_DIR)

$(CMARK_BUILD_DIR)/cmark-gfm_version.h: $(CMARK_DIR)/cmark-gfm_version.h.in makefile | $(CMARK_BUILD_DIR)
	sed -e 's/@PROJECT_VERSION_MAJOR@/$(CMARK_VERSION_MAJOR)/g' \
	    -e 's/@PROJECT_VERSION_MINOR@/$(CMARK_VERSION_MINOR)/g' \
	    -e 's/@PROJECT_VERSION_PATCH@/$(CMARK_VERSION_PATCH)/g' \
	    -e 's/@PROJECT_VERSION_GFM@/$(CMARK_VERSION_GFM)/g' $< > $@

$(CMARK_BUILD_DIR)/cmark-gfm_export.h: makefile | $(CMARK_BUILD_DIR)
	printf '%s\n' '#ifndef CMARK_GFM_EXPORT_H' '#define CMARK_GFM_EXPORT_H' '' '#define CMARK_GFM_EXPORT' '' '#endif' > $@

$(CMARK_BUILD_DIR)/config.h: $(CMARK_DIR)/config.h.in makefile | $(CMARK_BUILD_DIR)
	sed -e 's/^#cmakedefine HAVE_STDBOOL_H$$/#define HAVE_STDBOOL_H/' \
	    -e 's/^#cmakedefine \(.*\)$$/\/\* #undef \1 *\//' $< > $@

$(CMARK_BUILD_DIR)/%.o: $(CMARK_DIR)/%.c $(CMARK_GENERATED_HEADERS) | $(CMARK_BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.cpp gopher_renderer.h text_tools.h gemini_renderer.h html_renderer.h node_processing.h table_renderer.h $(CMARK_GENERATED_HEADERS)
	$(CPP) $(CPPFLAGS) -c -o $@ $<

clean:
	rm -rf $(CMARK_BUILD_DIR) $(OBJECTS) $(EXECUTABLE)
