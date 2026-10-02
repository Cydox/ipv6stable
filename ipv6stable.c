#define _GNU_SOURCE

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

extern const unsigned char _binary_libipv6stable_so_start[];
extern const unsigned char _binary_libipv6stable_so_end[];

int main(int argc, char **argv)
{
	if (argc < 2) {
		fprintf(stderr, "usage: %s COMMAND [ARG...]\n", argv[0]);
		return EXIT_FAILURE;
	}

	size_t size =
		(size_t)(_binary_libipv6stable_so_end -
			_binary_libipv6stable_so_start);

	int fd = memfd_create("ipv6stable", 0);
	assert(fd >= 0);

	const unsigned char *p = _binary_libipv6stable_so_start;

	while (size) {
		ssize_t n = write(fd, p, size);
		assert(n > 0);

		p += n;
		size -= (size_t)n;
	}

	#define PROC_SELF_FD "/proc/self/fd/"
	static char combined[16 * 4096];

	size_t prefix = sizeof(PROC_SELF_FD) - 1;
	memcpy(combined, PROC_SELF_FD, prefix);

	size_t available = sizeof(combined) - prefix;
	const char *old = getenv("LD_PRELOAD");

	int n;

	if (old && old[0])
		n = snprintf(combined + prefix, available,
			"%d:%s", fd, old);
	else
		n = snprintf(combined + prefix, available,
			"%d", fd);

	assert(n >= 0 && (size_t)n < available);

	int result = setenv("LD_PRELOAD", combined, 1);
	assert(result == 0);

	execvp(argv[1], &argv[1]);
	assert(!"execvp failed");
}

