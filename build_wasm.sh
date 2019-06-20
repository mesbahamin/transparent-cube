
mkdir -p ./out/wasm
cat js/globals.js js/utils.js js/imports.js js/loader.js > ./out/wasm/script.js
cp index.html ./out/wasm/index.html
cp -r shader/ ./out/wasm/

clang -cc1 -Ofast -emit-llvm-bc -triple=wasm32-unknown-unknown-unknown-wasm -std=c11 \
    -ffreestanding \
    -fno-builtin \
    -DGAME_WEBGL \
    src/platform_wasm.c
llvm-link -o wasm.bc src/*.bc
opt -O3 -disable-simplify-libcalls wasm.bc -o wasm.bc
llc -O3 -disable-simplify-libcalls -filetype=obj wasm.bc -o wasm.o

wasm-ld --no-entry wasm.o \
    -o ./out/wasm/binary.wasm \
    -allow-undefined-file wasm_js_implemented_symbols.txt \
    --export-all \
    --import-memory

#clang \
#  --target=wasm32 \
#  -emit-llvm \
#  -fno-builtin \
#  -c \
#  -S \
#  -DGAME_WEBGL \
#  src/game.c
