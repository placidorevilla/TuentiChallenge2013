/* Some syscalls */
#define __NR_exit 1
#define __NR_ptrace 26

#define PTRACE_TRACEME 0

/* A congruential generator */
static unsigned int rng_r(unsigned int r)
{
	return (16807 * r) % 0x7FFFFFF;
}

#ifndef GENERATOR
#ifndef PASSWORD
// char p[] = "ElvisHasLeftTheBuilding!";
#define PASSWORD 0x2d, 0x17, 0x0a, 0x25, 0xb9, 0x0f, 0x8b, 0xd7, 0x9d, 0x96, 0x98, 0x64, 0x17, 0x2b, 0x90, 0x6c, 0x21, 0x60, 0x67, 0x37, 0x67, 0xb2, 0x04, 0x06
#endif
static const unsigned char password[] __attribute__((section(".text#"))) = { PASSWORD };

/* This will return -1 if the program is run under a debugger */
static int check_ptrace()
{
	int ret;
	asm("int $0x80"
		: "=a" (ret)
		: "a" (__NR_ptrace), "b" (PTRACE_TRACEME), "c" (0), "d" (0), "S" (0)
	   );
	return ret;
}

static int main(int argc, char** argv)
{
	int result = 0;

	if (check_ptrace())
		return -1;
	if (argc != 2)
		return -1;

	char *p = argv[1];
	unsigned int r = 1337;
	while (*p) {
		r = rng_r(r);
		if ((unsigned)(p - argv[1]) >= sizeof(password) || (*p ^ r & 0xFF) != password[p - argv[1]])
			result = -1;
		p++;
	};

	if ((unsigned)(p - argv[1]) < sizeof(password))
		result = -1;

	return result;
}

/* The function calls the exit syscall so, it doesn't return */
void _start() __attribute__((noreturn));
/* Minimum bootstrap code I can write :-P */
void _start() {
	int argc;
	char **argv;
	asm("movl 4(%%ebp), %0;"
		"leal 8(%%ebp), %1;"
		: "=r" (argc), "=r" (argv)
	   );
	asm("int  $0x80"
		:: "a" (__NR_exit), "b" (main(argc, argv))
	   );
	while (1);
}
#else
#include <stdio.h>

int main(int argc, char **argv)
{
	if (argc < 2)
		return 0;

	char *p = argv[1];
	unsigned int r = 1337;
	while (*p) {
		r = rng_r(r);
		printf("0x%02x", *p ^ r & 0xFF);
		p++;
		if (*p)
			printf(", ");
	}
	printf ("\n");

	return 0;
}
#endif
