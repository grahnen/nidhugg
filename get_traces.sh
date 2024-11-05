#!/usr/bin/env sh
for i in benchmarks/event-driven/**/*.c benchmarks/event-driven/*.c; do
    echo $i
    out=${i%.c}.ll
    echo "Compiling..."
    /usr/lib/llvm14/bin/clang-14 $i -emit-llvm -S -o $out 1>/dev/null 2>&1
    echo "Running Nidhugg..."
    nidhugg -event -sc --debug-print-on-reset $out 2> ${i%.c}.log 1>/dev/null

    echo "Generating traces..."
    python3 /home/grahnen/build/EDchecker/converters/nidhugg.py ${i%.c}.log 1>/dev/null 2>&1
done
