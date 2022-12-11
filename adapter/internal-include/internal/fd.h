#ifndef _FD_H_
#define _FD_H_

enum coojaa_fd {
  COOJAA_FD_FLOOR = 100,
  RADIO_FD,
  COOJAA_FD_CEIL
};

#define COOJAA_FD_NUM (COOJAA_FD_CEIL - COOJAA_FD_FLOOR - 1)

static inline int valid_coojaa_fd(int fd)
{
  return (fd > COOJAA_FD_FLOOR) && (fd < COOJAA_FD_CEIL); 
}

#endif /* _FD_H_*/