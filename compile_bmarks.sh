#!/usr/bin/env sh

for i in benchmarks/event-driven/**/*.c benchmarks/event-driven/*.c; do
    out=${i%.c}.ll
    echo $i
    /usr/lib/llvm14/bin/clang-14 $i -emit-llvm -S -o $out
done
