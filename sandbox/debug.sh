emcc factory.c -o factory.wasm --no-entry -O0 -g -s STANDALONE_WASM=1 -s MALLOC='emmalloc' -s FILESYSTEM=0 -s ERROR_ON_UNDEFINED_SYMBOLS=0 -s ALLOW_MEMORY_GROWTH=1 -s USE_SDL=0 -s EVAL_CTORS=0