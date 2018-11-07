cmake_minimum_required (VERSION 2.8.11)
project (LLV)
add_library(LLV STATIC src/collections/DLL.c src/collections/LL.c src/helper.c src/llv.c src/collection_helper.c src/types/shared_types.c)
