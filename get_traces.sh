#!/usr/bin/env sh
d=benchmarks/event-driven
ds=$d/*.c
mkdir -p traces
mkdir -p outputs
mkdir -p bytecode
for i in ${d}/*.c ${d}/**/*.c; do
    echo $i

    out=bytecode/${i%.c}.ll
    mkdir -p $(dirname $out)
    mkdir -p $(dirname traces/${i%.c})
    mkdir -p $(dirname outputs/$i)
    echo "Compiling..."
    /usr/lib/llvm14/bin/clang-14 $i -emit-llvm -S -o $out 1>/dev/null 2>&1
    echo "Running Nidhugg..."
    ./src/nidhugg -event -sc --debug-print-on-reset $out 2> outputs/${i%.c}.log 1>/dev/null

    echo "Generating traces..."
    python3 /home/grahnen/build/EDchecker/converters/nidhugg.py outputs/${i%.c}.log traces 1>/dev/null 2>&1
done
