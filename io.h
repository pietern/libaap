#ifndef _AAP_IO_H
#define _AAP_IO_H

#include <unistd.h>

ssize_t aap_full_read(int fd, void *buf, size_t count);

ssize_t aap_full_write(int fd, const void *buf, size_t count);

#endif
