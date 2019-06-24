#!/usr/bin/env bash

set -e                     # fail if any command has a non-zero exit status
set -u                     # fail if any undefined variable is referenced
set -o pipefail            # propagate failure exit status through a pipeline
shopt -s globstar nullglob # enable recursive and null globbing

out_dir="./out"
wasm_dir="${out_dir}/wasm"

mkdir -p $wasm_dir
cp src/platform_wasm_loader.js $wasm_dir/script.js
cp src/platform_wasm_index.html $wasm_dir/index.html
cp -r shader/ $wasm_dir

clang \
    -cc1 \
    -Ofast \
    -emit-llvm-bc \
    -triple=wasm32-unknown-unknown-unknown-wasm \
    -ffreestanding \
    -fno-builtin \
    -std=c11 \
    -DGAME_WEBGL \
    src/platform_wasm.c

llvm-link -o $wasm_dir/wasm.bc src/*.bc
opt -O3 -disable-simplify-libcalls $wasm_dir/wasm.bc -o $wasm_dir/wasm.bc
llc -O3 -disable-simplify-libcalls -filetype=obj $wasm_dir/wasm.bc -o $wasm_dir/wasm.o

wasm-ld \
    --no-entry $wasm_dir/wasm.o \
    -o $wasm_dir/binary.wasm \
    -allow-undefined-file src/platform_wasm_js_symbols.txt \
    --export-all \
    --import-memory

rm $wasm_dir/*.o
rm $wasm_dir/*.bc
rm src/*.bc
