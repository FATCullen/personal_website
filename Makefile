SHELL := /bin/bash

BUILD_DIR := build
BUILD_BLOG_DIR := build/blog

BLOG_DIR := blog
TEMPLATE_DIR := templates
CONTENT_DIR := content

TEMPLATE_FILLER_DIR := build_programs/template_filler
TEMPLATE_FILLER := $(TEMPLATE_FILLER_DIR)/template_filler
MARKDOWN_PARSER_DIR := build_programs/markdown_parser
MARKDOWN_PARSER := $(MARKDOWN_PARSER_DIR)/markdown_parser

.PHONY: build clean
.SILENT: build clean

build:
	mkdir -p $(BUILD_DIR)
	rm -rf $(BUILD_DIR)/*
	cp -r animations $(BUILD_DIR)/animations
	cp -r images $(BUILD_DIR)/images
	cp -r favicon $(BUILD_DIR)/favicon
	cp -r styling/ $(BUILD_DIR)/
	$(MAKE) -C $(TEMPLATE_FILLER_DIR)
	$(TEMPLATE_FILLER) "$(TEMPLATE_DIR)" "$(BUILD_DIR)" "$(CONTENT_DIR)" "$(BLOG_DIR)"
	$(MAKE) -C $(MARKDOWN_PARSER_DIR)
	$(MARKDOWN_PARSER) "$(BLOG_DIR)" "$(BUILD_BLOG_DIR)"

clean:
	rm -rf $(BUILD_DIR)
	$(MAKE) -C $(TEMPLATE_FILLER_DIR) clean
	$(MAKE) -C $(MARKDOWN_PARSER_DIR) clean
