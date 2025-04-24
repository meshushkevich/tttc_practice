# Implicit Conversion Counter Plugin for Clang

## Prerequirements

### Check variables

```sh
export LLVM_DIR <PATH_TO_LLVM_DIR>
export LLVM_INCLUDE_DIRS <PATH_TO_LLVM_HEADERS>

export Clang_DIR <PATH_TO_CLANG_DIR>
export CLANG_INCLUDE_DIRS <PATH_TO_CLANG_HEADERS>
```

## Compiling

```sh
cmake -S ICC_Plugin -B build -G Ninja
ninja -C build
```

## Using example

```sh
clang -fplugin=build/ImplicitConversionsCounter.so -c ./tests/test1.cpp
```
