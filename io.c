#include <stdio.h>

#include "io.h"

ssize_t aap_full_read(int fd, void *buf, size_t count) {
  unsigned char *cbuf = buf;
  ssize_t n = 0;
  ssize_t rv;

  while (n < count) {
    rv = read(fd, cbuf+n, count-n);
    if (rv < 0) {
      return rv;
    }
    if (rv == 0) {
      return -1;
    }

#ifdef DEBUG
    fprintf(stderr, "Read %ld bytes:", rv);
    while (rv--) {
      fprintf(stderr, " 0x%02x", cbuf[n]);
      n++;
    }
    fprintf(stderr, "\n");
#else
    n += rv;
#endif
  }

  return n;
}

ssize_t aap_full_write(int fd, const void *buf, size_t count) {
#ifdef DEBUG
  {
    const unsigned char *cbuf = buf;
    int i;

    fprintf(stderr, "Wrote %ld bytes:", count);
    for (i = 0; i < count; i++) {
      fprintf(stderr, " 0x%02x", cbuf[i]);
    }
    fprintf(stderr, "\n");
  }
#endif

  return write(fd, buf, count);
}
