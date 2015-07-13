#ifndef _AAP_FRAME_H
#define _AAP_FRAME_H

#include <stdint.h>
#include <unistd.h>

typedef struct aap_frame_s aap_frame_t;

typedef ssize_t (*aap_frame_read_fn)(int fd, void *buf, size_t count);

typedef ssize_t (*aap_frame_write_fn)(int fd, const void *buf, size_t count);

struct aap_frame_s {
  unsigned char data[256];
  uint8_t len;
};

uint8_t aap_frame_parity(const void *buf, uint8_t len);

int aap_frame_read(int fd, aap_frame_read_fn fn, aap_frame_t *f);

int aap_frame_write(int fd, aap_frame_write_fn fn, aap_frame_t *f);

#endif
