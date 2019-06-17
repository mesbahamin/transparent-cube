mkdir -p ./out/wasm
clang -cc1 -Ofast -emit-llvm-bc -triple=wasm32-unknown-unknown-unknown-wasm -std=c11 \
    -DGAME_WEBGL \
    src/game.c
llvm-link -o wasm.bc src/*.bc
opt -O3 -disable-simplify-libcalls wasm.bc -o wasm.bc
llc -O3 -disable-simplify-libcalls -filetype=obj wasm.bc -o wasm.o
wasm-ld --no-entry wasm.o -o binary.wasm --strip-all -allow-undefined-file wasm_js_implemented_symbols.txt --import-memory

#clang \
#  --target=wasm32 \
#  -emit-llvm \
#  -fno-builtin \
#  -c \
#  -S \
#  -DGAME_WEBGL \
#  src/game.c
