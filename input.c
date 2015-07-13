#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>
#include <fcntl.h>
#include <string.h>

#include "frame.h"
#include "msg.h"
#include "io.h"

int split(char *buf, int *argc, char ***argv) {
  int len;
  int i;

  *argc = 0;
  *argv = NULL;

  len = strlen(buf);
  for (i = 0; i < len; i++) {
    if (!isspace(buf[i])) {
      // Start new argument
      *argc = *argc + 1;
      *argv = realloc(*argv, sizeof(*argv) * *argc);
      (*argv)[*argc-1] = &buf[i];

      // Skip over all non-spaces
      while (buf[++i] != '\0') {
        if (!isspace(buf[i])) {
          continue;
        }

        // Space marks end of argument
        buf[i] = 0;
        break;
      }
    }
  }

  return 0;
}

int main(int argc, char **argv) {
  int infd = 0;
  int outfd = 1;

  for (;;) {
    char buf[512];
    int len = 0;
    int i;

    aap_frame_t out;
    memset(&out, 0, sizeof(out));

    // Read line
    for (;;) {
      int rv;

      rv = read(infd, buf+len, 1);
      if (rv < 0) {
        return -1;
      }
      if (rv == 0) {
        return 0;
      }

      if (buf[len] == '\n') {
        buf[len] = 0;
        break;
      }

      len++;
    }

    int bargc;
    char **bargv;
    split(buf, &bargc, &bargv);
    for (i = 0; i < bargc; i++) {
      unsigned char byte;
      sscanf(bargv[i], "0x%hhx", &byte);
      out.data[i] = byte;
      out.len++;
    }

    // Add parity byte
    out.data[bargc] = aap_frame_parity(out.data, bargc);
    out.len++;

    // Write message to output
    aap_frame_write(outfd, aap_full_write, &out);
  }
}
