#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <sys/time.h>

#include "frame.h"
#include "msg.h"
#include "io.h"

int main(int argc, char **argv) {
  int infd = 0;
  int outfd = 1;
  int rv;

  for (;;) {
    aap_frame_t frame;
    aap_msg_t msg;

    rv = aap_frame_read(infd, aap_full_read, &frame);
    if (rv == -1) {
      return -1;
    }

    rv = aap_msg_read(&frame, &msg);
    if (rv == -1) {
      return -1;
    }

    struct timeval tv;
    rv = gettimeofday(&tv, NULL);
    if (rv < 0) {
      perror("gettimeofday");
      exit(1);
    }

    struct tm *tmp;
    tmp = localtime(&tv.tv_sec);

    char timebuf[128];
    int timelen;
    timelen = strftime(timebuf, sizeof(timebuf), "%T", tmp);
    timelen += snprintf(timebuf+timelen, sizeof(timebuf)-timelen, ".%06lu", tv.tv_usec);
    fprintf(stderr, "%s ", timebuf);
    fprintf(stderr, "mode %d message (%04x)", msg.mode, msg.command);

    if (msg.print != NULL) {
      fprintf(stderr, ": ");
      msg.print(stderr, &msg);
    } else {
      int i;
      for (i = 3; i < frame.len-1;i++) {
        char c = '.';
        if (isalnum(frame.data[i])) {
          c = frame.data[i];
        }

        if (i == 3) {
          fprintf(stderr, ": ");
        }

        fprintf(stderr, "%02x (%c) ", frame.data[i], c);
      }
    }

    fprintf(stderr, "\n");

    // Duplicate message to output
    aap_frame_write(outfd, aap_full_write, &frame);
  }
}
