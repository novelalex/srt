default:
    build

run: build
    ./bin/srt.exe

build: ensure-bin
    clang ./src/main.c -o bin/srt.exe

ensure-bin:
    mkdir -p ./bin

clean:
    rm -r ./bin