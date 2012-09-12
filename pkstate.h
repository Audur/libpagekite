/******************************************************************************
pkstate.h - Global program state

This file is Copyright 2011, 2012, The Beanstalks Project ehf.

This program is free software: you can redistribute it and/or modify it under
the terms of the  GNU  Affero General Public License as published by the Free
Software Foundation, either version 3 of the License, or (at your option) any
later version.

This program is distributed in the hope that it will be useful,  but  WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more
details.

You should have received a copy of the GNU Affero General Public License
along with this program.  If not, see: <http://www.gnu.org/licenses/>

Note: For alternate license terms, see the file COPYING.md.

******************************************************************************/

#define PKS_LOG_DATA_MAX     64*1024

struct pk_global_state {
  /* Synchronization */
  pthread_mutex_t lock;
  pthread_cond_t  cond;

  /* Global logging state */
  FILE*           log_file;
  unsigned int    log_mask;
  char            log_ring_buffer[PKS_LOG_DATA_MAX+1];
  char*           log_ring_start;
  char*           log_ring_end;

  /* Global program state */
  unsigned int    live_streams;
  unsigned int    live_frontends;
  int             have_ssl:1;
};

extern struct pk_global_state pk_state;

#define PKS_STATE(change) { pthread_mutex_lock(&(pk_state.lock)); \
                            change; \
                            pthread_cond_broadcast(&(pk_state.cond)); \
                            pthread_mutex_unlock(&(pk_state.lock)); } 

void pks_global_init(unsigned int log_level);
int pks_logcopy(const char*, size_t len);
void pks_copylog(char*);
void pks_printlog(FILE *dest);
