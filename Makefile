.PHONY: build

build:
	gcc *.c -std=c17 -no-pie -Wall -Werror=return-type -Werror=address -Werror=sequence-point  -Werror=format-security -Wextra -pedantic \
		-Wimplicit-fallthrough -Wsequence-point -Wswitch-unreachable -Wswitch-enum -Wstringop-truncation -Wbool-compare -Wtautological-compare \
		-Wfloat-equal -Wshadow=global -Wpointer-arith -Wpointer-compare -Wcast-align -Wcast-qual -Wwrite-strings -Wdangling-else -Wlogical-op \
		-Wconversion -O3 -lwiringPi -o ./build/pi-fan

clean:
	rm -rf build/*