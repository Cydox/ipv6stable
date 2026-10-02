#!/usr/bin/env sh
# SPDX-License-Identifier: 0BSD
set -x
gcc -Os -Wall -fPIC -shared -o libipv6stable.so libipv6stable.c -ldl
objcopy --input-target=binary --output-target=elf64-x86-64 --binary-architecture=i386:x86-64 libipv6stable.so libipv6stable.o
gcc -Os -Wall -Wextra -o ipv6stable ipv6stable.c libipv6stable.o
