ipv6stable
==========

ipv6stable is a simpler wrapper for any command that injects an LD_PRELOAD library
that sets IPV6_PREFER_SRC_PUBLIC socket option on any created socket.

This cause the kernel to prefer non-temporary aka stable ipv6 addresses. This is useful
for preventing an expired ipv6 privacy address from destroying a long-running TCP connection
(ssh client, irc client, etc).

Building
========
```bash
$ ./build.sh
```
