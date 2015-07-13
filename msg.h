#ifndef _AAP_MSG_H
#define _AAP_MSG_H

#include <stdint.h>
#include <stdio.h>

#include "frame.h"

typedef struct aap_msg_s aap_msg_t;

typedef void (*msg_print_fn)(FILE *, aap_msg_t *);

struct aap_msg_s {
  struct aap_frame_s frame;

  msg_print_fn print;

  uint8_t mode;
  uint16_t command;

  union {
    struct {
      uint8_t result;
      uint16_t command;
    } m4_c0x01;

    struct {
      unsigned char *str;
    } m4_c0x15;

    struct {
      uint8_t type;
      uint32_t number;
    } m4_c0x17;

    struct {
      uint8_t type;
    } m4_c0x18;

    struct {
      uint32_t count;
    } m4_c0x19;

    struct {
      uint8_t type;
      uint32_t offset;
      uint32_t count;
    } m4_c0x1a;

    struct {
      uint32_t index;
      unsigned char *str;
    } m4_c0x1b;

    struct {
      uint32_t length;
      uint32_t elapsed;
      uint8_t status;
    } m4_c0x1d;

    struct {
      uint32_t i;
    } m4_c0x1f;

    struct {
      uint32_t i;
    } m4_c0x20;

    struct {
      unsigned char *str;
    } m4_c0x21;

    struct {
      uint32_t i;
    } m4_c0x22;

    struct {
      unsigned char *str;
    } m4_c0x23;

    struct {
      uint32_t i;
    } m4_c0x24;

    struct {
      unsigned char *str;
    } m4_c0x25;

    struct {
      uint8_t cmd;
    } m4_c0x26;

    struct {
      uint8_t eof;
      uint32_t elapsed;
    } m4_c0x27;

    struct {
      uint32_t i;
    } m4_c0x28;

    struct {
      uint8_t cmd;
    } m4_c0x29;

    struct {
      uint8_t mode;
    } m4_c0x2d;

    struct {
      uint8_t mode;
    } m4_c0x2e;

    struct {
      uint8_t mode;
    } m4_c0x30;

    struct {
      uint8_t mode;
    } m4_c0x31;

    struct {
      uint32_t count;
    } m4_c0x36;
  };
};

int aap_msg_read(aap_frame_t *f, aap_msg_t *m);

int aap_msg_write(aap_frame_t *f, aap_msg_t *m);

#endif
