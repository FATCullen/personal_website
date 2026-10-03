SHELL := /bin/bash

BUILD_DIR := build
TEMPLATE_FILLER := ./template_filler/template_filler
MARKDOWN_PARSER := ./markdown_parser/markdown_parser

.PHONY: build clean
.SILENT: build clean

build:
	mkdir -p $(BUILD_DIR)
	rm -rf $(BUILD_DIR)/*
	cp -r animations $(BUILD_DIR)/animations
	cp -r images $(BUILD_DIR)/images
	cp -r favicon $(BUILD_DIR)/favicon
	cp -r styling/ $(BUILD_DIR)/
	$(MAKE) -C template_filler
	while IFS= read -r -d '' file; do \
		[ -f "$$file" ] && $(TEMPLATE_FILLER) "$$file" "$(BUILD_DIR)$${file#templates}"; \
	done < <(find templates -type f -print0)
	$(MAKE) -C markdown_parser
	for file in blog/*.md; do \
		[ -f "$$file" ] && $(MARKDOWN_PARSER) "$$file"; \
		mv "$${file%.*}.html" "$(BUILD_DIR)/$${file%.*}.html"; \
		mv "$${file%.*}.gmi" "$(BUILD_DIR)/$${file%.*}.gmi"; \
		mv "$${file%.*}.gophermap" "$(BUILD_DIR)/$${file%.*}.gophermap"; \
	done

clean:
	rm -rf $(BUILD_DIR)/
