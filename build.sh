#!/bin/bash
rm -rf build/*

cp -r animations build/animations
cp -r images build/images
cp -r styling/ build

make -C template_filler

while IFS= read -r -d '' file; do
    [ -f "$file" ] && ./template_filler/template_filler "$file" build"${file#templates}"
done < <(find "templates" -type f -print0)

make -C markdown_parser

for file in blog/*.md; do
    [ -f "$file" ] && ./markdown_parser/markdown_parser "$file"
    mv "${file%.*}.html" build/"${file%.*}.html"
    mv "${file%.*}.gmi" build/"${file%.*}.gmi"
    mv "${file%.*}.gophermap" build/"${file%.*}.gophermap"
done