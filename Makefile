CC ?= cc
CFLAGS ?= -std=c11 -Wall -Wextra -Werror -pedantic -O2
CPPFLAGS += -Ifirmware/core
LDLIBS += -lm

.PHONY: all test demo clean
all: test demo

build:
	mkdir -p build

build/test_control: firmware/core/fan_control.c tests/test_control.c | build
	$(CC) $(CFLAGS) $(CPPFLAGS) $^ $(LDLIBS) -o $@

build/fan_demo: firmware/core/fan_control.c firmware/app/fan_demo.c | build
	$(CC) $(CFLAGS) $(CPPFLAGS) $^ $(LDLIBS) -o $@

test: build/test_control
	./build/test_control

demo: build/fan_demo
	./build/fan_demo

clean:
	rm -rf build
