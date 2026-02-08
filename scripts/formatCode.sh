#!/bin/bash
for file in $(find {apps,include,src}/. -iname '*.hpp' -o -iname '*.cpp');
do
    if [[ $(echo "$file" | grep -o ".cmake") == ".cmake" ]]; then
        continue
    fi

    clang-format -i "$file"
done