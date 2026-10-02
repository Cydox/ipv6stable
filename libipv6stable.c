// SPDX-License-Identifier: 0BSD
#include <dlfcn.h>
#include <stddef.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <linux/ipv6.h>
#include <assert.h>



int socket(int domain, int type, int protocol)
{
    static int (*real_socket)(int, int, int) = NULL;
    if (real_socket == NULL)
        real_socket = dlsym(RTLD_NEXT, "socket");

    assert(real_socket > 0);

    int fd = real_socket(domain, type, protocol);

    if (fd < 0)
        return fd;

    if (domain == AF_INET6) {
        int preference = IPV6_PREFER_SRC_PUBLIC;

        (void)setsockopt(fd,
                         IPPROTO_IPV6,
                         IPV6_ADDR_PREFERENCES,
                         &preference,
                         sizeof(preference));
    }

    return fd;
}
