#!/bin/bash
# Generate assembly output for all C examples
# Usage: ./scripts/generate_asm.sh [chapter_number]

if [ -n "$1" ]; then
    DIRS="chapters/$(printf '%02d' $1)-*"
else
    DIRS="chapters/*"
fi

for dir in $DIRS; do
    if [ -d "$dir/examples" ]; then
        echo "=== $dir ==="
        for f in "$dir/examples"/*.c; do
            base=$(basename "$f" .c)
            echo "  $base.c → $base.s"
            gcc -S -O0 -fno-asynchronous-unwind-tables -masm=att "$f" -o "$dir/examples/$base.s" 2>&1
        done
    fi
done
