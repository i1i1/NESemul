CC = gcc
CFLAGS = -lSDL2 -Wextra -Wall -std=c99 -Iinclude
OUT = emul
ROM = Metal_Gear.nes
#ROM = castleva.nes

src = $(wildcard src/*.c)
hdr = $(wildcard include/*.h)

obj = $(subst src, build, $(src:.c=.o))
dep = $(subst src, build, $(src:.c=.d))

all: $(OUT)

debug: CFLAGS += -DDEBUG
debug: clean make $(OUT) test

test: $(OUT)
	./$(OUT) $(ROM)

$(OUT): $(obj)
	$(CC) $(CFLAGS) -o $@ $^

build/%.o: src/%.c build/%.d
	$(CC) $(CFLAGS) -c -o $@ $<

make: $(dep)

build/%.d: src/%.c
#	./makedeps.awk -v inc=include -v build=build $< > $@
	$(CC) $(CFLAGS) -MM $< | sed -e "s/^[^ \t]\+\.o:/build\/&/" > $@

clean:
	rm -rf $(OUT)
	rm -rf $(obj)

fullclean: clean
	rm -rf $(dep)

.PHONY: fullclean clean debug test make
.SECONDARY: $(dep)

ifneq "$(MAKECMDGOALS)" "clean"
-include $(dep)
endif

