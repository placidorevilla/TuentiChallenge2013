.PHONY: all
all:
	./generate.sh

password: password.c
	gcc -ggdb3 -march=i386 -Os -m32 -fno-asynchronous-unwind-tables -W -nostdlib -nostartfiles -o password $(CFLAGS_PASSWORD) password.c;
	strip -R .comment -R .note.gnu.build-id password;

generator: password.c
	gcc -DGENERATOR -W -O2 -o generator password.c

.PHONY: clean
clean:
	rm -f password check_* generator list.txt list.sql
