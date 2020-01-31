#include "lwp.h"
#include <stdlib.h>
#include <stdio.h>
#include "rr.h"

static thread qhead=NULL;
static int advance=FALSE;
#define tnext sched_one
#define tprev sched_two


static void rr3_admit(thread new) {
  printf("Admitting new thread to Second scheduler\n");
  /* add to queue */
  if ( qhead ) {
    new->tnext = qhead;
    new->tprev = qhead->tprev;
    new->tprev->tnext = new;
    qhead->tprev = new;
  } else {
    advance = FALSE;
    qhead = new;
    qhead->tnext = new;
    qhead->tprev = new;
  }

}

static void rr3_remove(thread victim) {
  printf("Removing thread from Second scheduler\n");
  /* cut out of queue */
  victim->tprev->tnext = victim->tnext;
  victim->tnext->tprev = victim->tprev;

  /* what if it were qhead? */
  if ( victim == qhead ) {
    if ( victim->tnext != victim)
      qhead = victim->tprev;  /* preserve who would've been next */
    else
      qhead = NULL;
  }
}

static thread rr3_next() {
  printf("(second->next())\n");

  if ( qhead ) {
    if ( advance  )
      qhead = qhead->tnext;
    else
      advance = TRUE;
  }
    printf("in second, returning tid: %lu\n", qhead->tid);

  return qhead;
}

void init() {
  printf("%s: called on second scheduler\n",__FUNCTION__);
}

void shutdown () {
  printf("%s: called on second scheduler\n",__FUNCTION__);
}

static struct scheduler rr3_publish = 
    {init, shutdown, rr3_admit,rr3_remove,rr3_next};
scheduler AltAltRoundRobin = &rr3_publish;


