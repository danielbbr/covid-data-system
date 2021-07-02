#ifndef CLEAR_UNIX.H
#define CLEAR_UNIX.H

#include <unistd.h>
#include <term.h>

void ClearScreen() {
  if (!cur_term) {
    int result;
    setupterm( NULL, STDOUT_FILENO, &result );
    if (result <= 0) return;
    }

  putp( tigetstr( "clear" ) );
  }

#endif