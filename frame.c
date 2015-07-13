#include "frame.h"

uint8_t aap_frame_parity(const void *buf, uint8_t len) {
  uint8_t parity;
  uint8_t byte;

  parity = len;
  for (byte = 0; byte < len; byte++) {
    parity += ((const unsigned char *) buf)[byte];
  }

  return ((0x100 - parity) & 0xff);
}

int aap_frame_read(int fd, aap_frame_read_fn fn, aap_frame_t *f) {
  uint8_t byte;
  uint8_t len;
  int rv;

  for (;;) {
    // Read header byte
    rv = fn(fd, &byte, 1);
    if (rv < 0) {
      return rv;
    }

    // Expect 0x55 after 0xff
    if (byte == 0xff) {
      rv = fn(fd, &byte, 1);
      if (rv < 0) {
        return rv;
      }

      if (byte != 0x55) {
        continue;
      }
    } else {
      // Expect 0x55 as first byte
      if (byte != 0x55) {
        continue;
      }
    }

    // Read length of (mode + command + parameter)
    rv = fn(fd, &len, 1);
    if (rv < 0) {
      return rv;
    }

    if (len == 0) {
      continue;
    }

    // Read (mode + command + parameter + parity)
    rv = fn(fd, f->data, len+1);
    if (rv < 0) {
      return rv;
    }

    // Check parity byte
    if (f->data[len] != aap_frame_parity(f->data, len)) {
      return -2;
    }

    f->len = len+1;
    break;
  }

  return 0;
}

int aap_frame_write(int fd, aap_frame_write_fn fn, aap_frame_t *f) {
  uint8_t byte;
  int rv;

  // Write 0xff header byte
  byte = 0xff;
  rv = fn(fd, &byte, 1);
  if (rv < 0) {
    return rv;
  }

  // Write 0x55 header byte
  byte = 0x55;
  rv = fn(fd, &byte, 1);
  if (rv < 0) {
    return rv;
  }

  // Write length without parity
  byte = f->len-1;
  rv = fn(fd, &byte, 1);
  if (rv < 0) {
    return rv;
  }

  // Write payload
  rv = fn(fd, f->data, f->len);
  if (rv < 0) {
    return rv;
  }

  return 0;
}
