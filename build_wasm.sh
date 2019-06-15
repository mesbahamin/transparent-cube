mkdir -p ./out/wasm
clang -cc1 -Ofast -emit-llvm-bc -triple=wasm32-unknown-unknown-unknown-wasm -std=c11 \
    -DGAME_WEBGL \
    src/game.c
llvm-link -o wasm.bc src/*.bc
llc -O3 -filetype=obj wasm.bc -o wasm.o
wasm-ld --no-entry wasm.o -o binary.wasm --strip-all -allow-undefined-file wasm_js_implemented_symbols.txt --import-memory
