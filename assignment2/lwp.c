/*  Andy Velazquz  
 *  avelaz02
 *  Assignment 2
 *  lpw library
 */

#include "lwp.h"
#include <stdlib.h>

static unsigned long counter = 1;
static thread lib_head_ptr = NULL;
static thread sched_head_ptr = NULL;
static thread sched_tail_ptr = NULL;

/* scheduler function */

void rr_init(void)
{

}

void rr_shutdown(void)
{

}

void rr_admit(thread new)
{
    if(sched_head_ptr == NULL)
    {
        /*scheduler list is emptry */
        sched_head_ptr = new;
        sched_tail_ptr = new;
        new->sched_one = NULL;
        new->sched_two = NULL;
    }
    
    /*scheduler list is not empty, add to end of list*/
    new->sched_two = sched_tail_ptr;
    new->sched_one = NULL;
    sched_tail_ptr->sched_one = new;
    sched_tail_ptr = new; 
}

void rr_remove(thread victim)
{
    thread current = sched_head_ptr;
    while(current->sched_one !=NULL)
    {
        if(current->sched_one->tid == victim->tid)
        {
           /*i have found my thread that i want to remove*/
            /*do i want to call free on the thread?*/
            current->sched_two->sched_one = current->sched_one;
            current->sched_one->sched_two = current->sched_two;
            return;
        }
        current = current->sched_one;
    }
    /*two cases: current = last node or 
 *    current = first node and only one node in list*/
    if(current->tid == victim->tid)
    {
        if(current->sched_two == NULL)
        {  
            /*first node*/
            sched_head_ptr = NULL;
            sched_tail_ptr = NULL;
            /*might have to free current come back to this */
        } 
        else
        {
            /*last node*/
            current->sched_two->sched_one = NULL;
            sched_tail_ptr = current->sched_two;
            current->sched_two = NULL;
            /*might have to free current come back to this */
        }
        
    }
 
}

thread rr_next(void)
{
    thread current = sched_head_ptr;
    thread last = sched_tail_ptr;
    /*if next is called, how do we know which thread we are?
 * do we assume we are the head? how do i keep track of
 * current position? if i am the head and i call next
 * what do i do with the current thread? do i put it the 
 * end of the list(scheduler)?  */

    if(current && current->sched_one)
    {
        sched_head_ptr = current->sched_one;
        current->sched_one->sched_two = NULL;
        last->sched_one = current;
        current->sched_two = last;
        current->sched_one = NULL;
        sched_tail_ptr = current;
        return sched_head_ptr;
    }
    return NULL;

}

/*end of scheduler functions */

static struct scheduler rr_publish = {NULL, NULL, rr_admit, rr_remove, rr_next};
scheduler RoundRobin = &rr_publish;


tid_t lwp_create(lwpfun function, void * arguments, size_t size)
{
    void *base = malloc(size);
    thread new = (thread)malloc(sizeof(context));
    unsigned long *TOS = (unsigned long *)((uintptr_t)base + size);
    new->tid = counter++;
    new->stacksize = size;
    new->stack = (unsigned long *)base;
    TOS--;
    *TOS = lwp_exit;
    if(lib_head_ptr == NULL)
    {
        lib_head_ptr = new;
    }
}

tid_t lwp_gettid(void)
{


}
void lwp_yield(void)
{

}
void lwp_exit(void )
{


}

void lwp_start(void)
{

}

void lwp_stop(void)
{
}

void lwp_set_scheduler(scheduler fun)
{
}

scheduler lwp_get_scheduler(void)
{
}

thread tid2thread(tid_t tid)
{
}




