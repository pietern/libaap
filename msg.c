#include <string.h>

#include "msg.h"

static uint32_t __ntohl(unsigned char *buf) {
  uint32_t x = 0;
  x |= (buf[0] << 24) & 0xff000000;
  x |= (buf[1] << 16) & 0x00ff0000;
  x |= (buf[2] <<  8) & 0x0000ff00;
  x |= (buf[3] <<  0) & 0x000000ff;
  return x;
}

static uint16_t __ntohs(unsigned char *buf) {
  uint16_t x = 0;
  x |= (buf[0] <<  8) & 0xff00;
  x |= (buf[1] <<  0) & 0x00ff;
  return x;
}

static void p4_c0x01(FILE *out, aap_msg_t *m) {
  char *status;

  status = "(unknown)";
  switch (m->m4_c0x01.result) {
  case 0x00:
    status = "success";
    break;
  case 0x02:
    status = "failure";
    break;
  case 0x04:
    status = "exceeded limit";
    break;
  case 0x05:
    status = "is a response not a command";
    break;
  }

  fprintf(out, "Result (for %04x): %s", m->m4_c0x01.command, status);
}

static void p4_c0x14(FILE *out, aap_msg_t *m) {
  fprintf(out, "Get iPod name");
}

static void p4_c0x15(FILE *out, aap_msg_t *m) {
  fprintf(out, "iPod name: %s", m->m4_c0x15.str);
}

static void p4_c0x16(FILE *out, aap_msg_t *m) {
  fprintf(out, "Switch to main library playlist");
}

static char *types[] = {
  "(unknown)",
  "playlist",
  "artist",
  "album",
  "genre",
  "song",
  "composer"
};

static void p4_c0x17(FILE *out, aap_msg_t *m) {
  fprintf(out, "Switch to %s (#%u)", types[m->m4_c0x17.type], m->m4_c0x17.number);
}

static void p4_c0x18(FILE *out, aap_msg_t *m) {
  fprintf(out, "Get count for %ss", types[m->m4_c0x18.type]);
}

static void p4_c0x19(FILE *out, aap_msg_t *m) {
  fprintf(out, "Count: %u", m->m4_c0x19.count);
}

static void p4_c0x1a(FILE *out, aap_msg_t *m) {
  fprintf(out, "Get names of %s (offset: %d, count: %d)", types[m->m4_c0x1a.type], m->m4_c0x1a.offset, m->m4_c0x1a.count);
}

static void p4_c0x1b(FILE *out, aap_msg_t *m) {
  fprintf(out, "Name #%u: %s", m->m4_c0x1b.index, m->m4_c0x1b.str);
}

static void p4_c0x1c(FILE *out, aap_msg_t *m) {
  fprintf(out, "Get time and status info");
}

static char *p4_c0x1d_status[] = {
  "stop",
  "playing",
  "paused",
};

static void p4_c0x1d(FILE *out, aap_msg_t *m) {
  fprintf(out, "Length %ums, elapsed: %ums, status: %s", m->m4_c0x1d.length, m->m4_c0x1d.elapsed, p4_c0x1d_status[m->m4_c0x1d.status]);
}

static void p4_c0x1e(FILE *out, aap_msg_t *m) {
  fprintf(out, "Get current position in playlist");
}

static void p4_c0x1f(FILE *out, aap_msg_t *m) {
  fprintf(out, "Position in playlist: #%u", m->m4_c0x1f.i);
}

static void p4_c0x20(FILE *out, aap_msg_t *m) {
  fprintf(out, "Get title of song number #%u", m->m4_c0x20.i);
}

static void p4_c0x21(FILE *out, aap_msg_t *m) {
  fprintf(out, "Title: %s", m->m4_c0x21.str);
}

static void p4_c0x22(FILE *out, aap_msg_t *m) {
  fprintf(out, "Get artist of song number #%u", m->m4_c0x22.i);
}

static void p4_c0x23(FILE *out, aap_msg_t *m) {
  fprintf(out, "Artist: %s", m->m4_c0x23.str);
}

static void p4_c0x24(FILE *out, aap_msg_t *m) {
  fprintf(out, "Get album of song number #%u", m->m4_c0x24.i);
}

static void p4_c0x25(FILE *out, aap_msg_t *m) {
  fprintf(out, "Album: %s", m->m4_c0x25.str);
}

static void p4_c0x26(FILE *out, aap_msg_t *m) {
  char *what = "Stop";
  if (m->m4_c0x26.cmd == 0x1) {
    what = "Start";
  }

  fprintf(out, "%s streaming mode", what);
}

static void p4_c0x27(FILE *out, aap_msg_t *m) {
  if (m->m4_c0x27.eof) {
    fprintf(out, "Elapsed: EOF");
  } else {
    fprintf(out, "Elapsed: %ums", m->m4_c0x27.elapsed);
  }
}

static void p4_c0x28(FILE *out, aap_msg_t *m) {
  fprintf(out, "Execute playlist and to song number #%u", m->m4_c0x28.i);
}

static char *p4_c0x29_cmds[] = {
  "(unknown)",
  "play/pause",
  "stop",
  "skip++",
  "skip--",
  "ffwd",
  "frwd",
  "stop ffwd/frwd",
};

static void p4_c0x29(FILE *out, aap_msg_t *m) {
  fprintf(out, "Playback control: %s", p4_c0x29_cmds[m->m4_c0x29.cmd]);
}

static char *p4_shuffle_modes[] = {
  "off",
  "songs",
  "albums"
};

static void p4_c0x2c(FILE *out, aap_msg_t *m) {
  fprintf(out, "Get shuffle mode");
}

static void p4_c0x2d(FILE *out, aap_msg_t *m) {
  fprintf(out, "Current shuffle mode: %s", p4_shuffle_modes[m->m4_c0x2d.mode]);
}

static void p4_c0x2e(FILE *out, aap_msg_t *m) {
  fprintf(out, "Set shuffle mode: %s", p4_shuffle_modes[m->m4_c0x2e.mode]);
}

static char *p4_repeat_modes[] = {
  "off",
  "songs",
  "albums"
};

static void p4_c0x2f(FILE *out, aap_msg_t *m) {
  fprintf(out, "Get repeat mode");
}

static void p4_c0x30(FILE *out, aap_msg_t *m) {
  fprintf(out, "Current repeat mode: %s", p4_repeat_modes[m->m4_c0x30.mode]);
}

static void p4_c0x31(FILE *out, aap_msg_t *m) {
  fprintf(out, "Set repeat mode: %s", p4_repeat_modes[m->m4_c0x31.mode]);
}

static void p4_c0x35(FILE *out, aap_msg_t *m) {
  fprintf(out, "Get number of songs in playlist");
}

static void p4_c0x36(FILE *out, aap_msg_t *m) {
  fprintf(out, "Number of songs in playlist: %u", m->m4_c0x36.count);
}

int aap_msg_parse_mode4(aap_msg_t *m, unsigned char *buf, int len) {
  switch (m->command) {
  case 0x01:
    // Command result
    m->print = p4_c0x01;
    if (len < 3) {
      return -1;
    }

    m->m4_c0x01.result = buf[0];
    m->m4_c0x01.command = __ntohs(&buf[1]);
    break;

  case 0x14:
    // Get iPod name
    m->print = p4_c0x14;
    break;

  case 0x15:
    // Return iPod name
    m->print = p4_c0x15;
    if (buf[len-1] != 0) {
      return -1;
    }

    m->m4_c0x15.str = buf;
    break;

  case 0x16:
    // Switch to main library playlist
    m->print = p4_c0x16;
    break;

  case 0x17:
    // Switch to item identified by number and type given.
    m->print = p4_c0x17;
    if (len < 5) {
      return -1;
    }
    if (buf[0] < 1 || buf[0] > 6) {
      return -1;
    }

    m->m4_c0x17.type = buf[0];
    m->m4_c0x17.number = __ntohl(&buf[1]);
    break;

  case 0x18:
    // Get count for the given type (e.g. number of tracks)
    m->print = p4_c0x18;
    if (len < 1) {
      return -1;
    }
    if (buf[0] < 1 || buf[0] > 6) {
      return -1;
    }

    m->m4_c0x18.type = buf[0];
    break;

  case 0x19:
    // Returns count for the given type (see above)
    m->print = p4_c0x19;
    if (len < 4) {
      return -1;
    }

    m->m4_c0x19.count = __ntohl(&buf[0]);
    break;

  case 0x1a:
    m->print = p4_c0x1a;
    if (len < 9) {
      return -1;
    }
    if (buf[0] < 1 || buf[0] > 6) {
      return -1;
    }

    m->m4_c0x1a.type = buf[0];
    m->m4_c0x1a.offset = __ntohl(&buf[1]);
    m->m4_c0x1a.count = __ntohl(&buf[5]);
    break;

  case 0x1b:
    m->print = p4_c0x1b;
    if (len < 5) {
      return -1;
    }
    if (buf[len-1] != 0) {
      return -1;
    }

    m->m4_c0x1b.index = __ntohl(&buf[0]);
    m->m4_c0x1b.str = &buf[4];
    break;

  case 0x1c:
    // Get time and status info
    m->print = p4_c0x1c;
    break;

  case 0x1d:
    // Time and status info
    m->print = p4_c0x1d;
    if (len < 9) {
      return -1;
    }
    if (buf[8] > 2) {
      return -1;
    }

    m->m4_c0x1d.elapsed = __ntohl(&buf[0]);
    m->m4_c0x1d.length = __ntohl(&buf[4]);
    m->m4_c0x1d.status = buf[8];
    break;

  case 0x1e:
    // Get current position in playlist
    m->print = p4_c0x1e;
    break;

  case 0x1f:
    // Current position in playlist
    m->print = p4_c0x1f;
    if (len < 4) {
      return -1;
    }

    m->m4_c0x1f.i = __ntohl(&buf[0]);
    break;

  case 0x20:
    // Get title of a specific song
    m->print = p4_c0x20;
    if (len < 4) {
      return -1;
    }

    m->m4_c0x20.i = __ntohl(&buf[0]);
    break;

  case 0x21:
    // Title
    m->print = p4_c0x21;
    if (buf[len-1] != 0) {
      return -1;
    }

    m->m4_c0x21.str = buf;
    break;

  case 0x22:
    // Get artist of a specific song
    m->print = p4_c0x22;
    if (len < 4) {
      return -1;
    }

    m->m4_c0x22.i = __ntohl(&buf[0]);
    break;

  case 0x23:
    // Artist
    m->print = p4_c0x23;
    if (buf[len-1] != 0) {
      return -1;
    }

    m->m4_c0x23.str = buf;
    break;

  case 0x24:
    // Get album of a specific song
    m->print = p4_c0x24;
    if (len < 4) {
      return -1;
    }

    m->m4_c0x24.i = __ntohl(&buf[0]);
    break;

  case 0x25:
    // Album
    m->print = p4_c0x25;
    if (buf[len-1] != 0) {
      return -1;
    }

    m->m4_c0x25.str = buf;
    break;

  case 0x26:
    // Start/stop streaming mode
    m->print = p4_c0x26;
    if (len < 1) {
      return -1;
    }

    m->m4_c0x26.cmd = buf[0] & 0x1;
    break;

  case 0x27:
    // Time elapsed on current song
    m->print = p4_c0x27;
    if (len == 0) {
      return -1;
    }

    if (len == 1) {
      // Can only be EOF
      if (buf[0] != 0) {
        return -1;
      }

      m->m4_c0x27.eof = 1;
      break;
    }

    if (len == 5) {
      m->m4_c0x27.elapsed = __ntohl(&buf[1]);
      break;
    }

    return -1;

  case 0x28:
    // Execute playlist and jump to specific song
    m->print = p4_c0x28;
    if (len < 4) {
      return -1;
    }

    m->m4_c0x28.i = __ntohl(&buf[0]);
    break;

  case 0x29:
    // Playback control
    m->print = p4_c0x29;
    if (len < 1) {
      return -1;
    }
    if (buf[0] < 1 || buf[0] > 7) {
      return -1;
    }

    m->m4_c0x29.cmd = buf[0];
    break;

  case 0x2c:
    // Get shuffle mode
    m->print = p4_c0x2c;
    break;

  case 0x2d:
    // Current shuffle mode
    m->print = p4_c0x2d;
    if (len < 1) {
      return -1;
    }
    if (buf[0] > 2) {
      return -1;
    }

    m->m4_c0x2d.mode = buf[0];
    break;

  case 0x2e:
    // Set shuffle mode
    m->print = p4_c0x2e;
    if (len < 1) {
      return -1;
    }
    if (buf[0] > 2) {
      return -1;
    }

    m->m4_c0x2e.mode = buf[0];
    break;

  case 0x2f:
    // Get repeat mode
    m->print = p4_c0x2f;
    break;

  case 0x30:
    // Current repeat mode
    m->print = p4_c0x30;
    if (len < 1) {
      return -1;
    }
    if (buf[0] > 2) {
      return -1;
    }

    m->m4_c0x30.mode = buf[0];
    break;

  case 0x31:
    // Set repeat mode
    m->print = p4_c0x31;
    if (len < 1) {
      return -1;
    }
    if (buf[0] > 2) {
      return -1;
    }

    m->m4_c0x31.mode = buf[0];
    break;

  case 0x35:
    // Get number of songs in playlist
    m->print = p4_c0x35;
    break;

  case 0x36:
    // Number of songs in playlist
    m->print = p4_c0x36;
    if (len < 4) {
      return -1;
    }

    m->m4_c0x36.count = __ntohl(&buf[0]);
    break;

  }

  return 0;
}

int aap_msg_read(aap_frame_t *f, aap_msg_t *m) {
  uint8_t len;
  uint8_t pos;

  // Ignore parity byte
  len = f->len-1;
  if (len < 2) {
    return -1;
  }

  memset(m, 0, sizeof(*m));
  m->mode = f->data[0];
  m->command = f->data[1] << 8;
  m->command |= f->data[2];

  // Skip over mode + command
  pos = 3;
  len -= 3;

  // Parse mode specific commands
  switch (m->mode) {
  case 4:
    aap_msg_parse_mode4(m, &f->data[pos], len);
    break;
  }

  return 0;
}

int aap_msg_write(aap_frame_t *f, aap_msg_t *m) {
  uint8_t pos;
  uint8_t len;

  f->data[0] = m->mode;
  f->data[1] = (m->command >> 8) & 0xff;
  f->data[2] = (m->command >> 0) & 0xff;
  pos = 3;
  len = 3;

  // Add parity byte
  f->data[pos] = aap_frame_parity(f->data, len);
  pos++;
  len++;

  f->len = len;
  return 0;
}
