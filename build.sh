rm -rf build/*

cp -r animations build/animations
cp -r images build/images

for file in template/*; do
    cp "$file" build/"${file#template/}"
done

make -C markdown_parser

for file in blog/*; do
    [ -f "$file" ] && ./markdown_parser/markdown_parser "$file"
    mv "${file%.*}.html" build/"${file%.*}.html"
    mv "${file%.*}.gmi" build/"${file%.*}.gmi"
    mv "${file%.*}.gophermap" build/"${file%.*}.gophermap"
done