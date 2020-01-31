/*  Andy Velazquz  
 *  Caleb Rabbon
 *  Assignment 2
 *  lpw library
 */

#include "lwp.h"
#include <stdlib.h>
#include <stdio.h>

static unsigned long counter = 1;
static thread lib_head_ptr = NULL;
static thread sched_head_ptr = NULL;
static thread sched_tail_ptr = NULL;
static thread current_process = NULL;
static rfile main_rfile;
static struct scheduler rr_publish = {NULL, NULL, rr_admit, rr_remove, rr_next};
scheduler RoundRobin = &rr_publish;

/*printing function*/
void print_thread(thread t)
{
    if(t == NULL)
    {
        printf("warning thread is null\n");
    }
    printf("tid: %lu stacksize: %d base of stack: %lu\n",
            t->tid, t->stacksize, t->stack);
    printf("rdi: %lu, rbp: %lu, rsp: %lu\n", t->state.rdi,
            t->state.rbp, t->state.rsp);
    
}
void print_scheduler(thread c)
{
    if(c == NULL)
    {
        printf("head_ptr is null\n\n\n");
        return;
    }
    thread current = c;
    while(current)
    {
        print_thread(current);
        current = current->sched_one;
    }
    printf("after while in print scheduler\n");
    if(current_process)
    {
        printf("current thread: %lu\n\n", current_process->tid);
    }
    printf("\n\n"); 
}
void print_list()
{
    if(lib_head_ptr == NULL)
    {
        printf("head_ptr is null\n\n\n");
        return;
    }
    thread current = lib_head_ptr;
    while(current)
    {
        print_thread(current);
        current = current->lib_one;
    }
    if(current_process)
    {
        printf("current thread: %lu\n\n", current_process->tid);
    }
    printf("\n\n"); 
}
/* scheduler function */
void rr_init(void)
{

}

void rr_shutdown(void)
{

}

void rr_admit(thread new)
{
    if(new == NULL)
    {
        return ;
    }
    if(sched_head_ptr == NULL)
    {
        /*scheduler list is empty */
        sched_head_ptr = new;
        sched_tail_ptr = new;
        new->sched_one = NULL;
        new->sched_two = NULL;
        return;
    }
    
    /*scheduler list is not empty, add to end of list*/
    new->sched_one = NULL; 
    sched_tail_ptr->sched_one = new;
    sched_tail_ptr = new; 
}

void rr_remove(thread victim)
{
    thread current = sched_head_ptr;
    thread previous = NULL;
    while(current->sched_one !=NULL)
    {
        if(current->tid == victim->tid)
        {
           /*i have found my thread that i want to remove*/
            /* if prevous == NULL removing head */
            if(previous == NULL)
            {
                sched_head_ptr = current->sched_one;
                current->sched_one = NULL;
                return; 
            }
            else
            {
                previous->sched_one = current->sched_one;
                current->sched_one = NULL;
                return;
            }
        }
        previous = current;
        current = current->sched_one;
    }
    /*two cases: current = last node or 
 *    current = first node and only one node in list*/
    if(current->tid == victim->tid)
    {
        if(previous == NULL)
        { 
            /*first node*/
            sched_head_ptr = NULL;
            sched_tail_ptr = NULL;
            current->sched_one = NULL;
            current->sched_two = NULL;
            //victim->sched_one = NULL;
            //victim->sched_two = NULL;
        } 
        else
        {
            /*last node*/
            previous->sched_one = NULL;
            sched_tail_ptr = previous;
            
        }
        
    }
}

thread rr_next(void)
{
    thread current = sched_head_ptr;
    thread last = sched_tail_ptr;
    thread next = NULL;

    if(current && current->sched_one)
    {
        next = current->sched_one;
        sched_head_ptr = next;
        last->sched_one = current;
        current->sched_one = NULL;
        sched_tail_ptr = current;
        if(current_process == NULL)
        {
            return current;
        }
        return current;
    }
    if(current != NULL)
    {
        
        /*there is only one thread in the scheduler */
        current->sched_two = NULL;
        current->sched_one = NULL;
        sched_head_ptr = current;
        sched_tail_ptr = current;
        return current;
    }
    return NULL;

}

/*end of scheduler functions */



tid_t lwp_create(lwpfun function, void * arguments, size_t size)
{
    unsigned long *base = (unsigned long *)malloc(sizeof(unsigned long)*size);
    thread new = (thread)malloc(sizeof(context));
    /*setting up thread and stack */
    new->state.fxsave=FPU_INIT;
    new->tid = counter++; 
    new->stacksize = size;
    new->stack = (unsigned long *)base;
    new->stack[size -1] = (unsigned long )lwp_exit;
    new->stack[size -2] =  (unsigned long )function;
    new->state.rbp = (unsigned long)&(new->stack[size - 3]);
    new->state.rsp = (unsigned long)&(new->stack[size - 3]);
    new->state.rdi = (unsigned long) arguments;
    new->lib_one = NULL;
    new->lib_two = NULL;
    
    /*setting up list for library */
    if(lib_head_ptr == NULL)
    {
        /*emptry library list */
        lib_head_ptr = new;
        new->lib_one = NULL;
        new->lib_two = NULL;
        //current_process=new;
    }
    else
    {
       /*library list is not empty */
        thread current = lib_head_ptr;
        while(current->lib_one != NULL)
        {
            current = current->lib_one;
        } 
        /*current is now the last thread in the list */
        current->lib_one = new;
        new->lib_one = NULL;
    }
    /*admit created thread to scheduler */
    RoundRobin->admit(new);      
    return new->tid;
}

tid_t lwp_gettid(void)
{
    return current_process->tid;
}

void lwp_yield(void)
{ 
    thread next; 
    thread current = current_process;
    if(NULL == (next = RoundRobin->next()))
    {
        current_process = NULL;
        swap_rfiles(NULL,&(main_rfile));
        
        
    }
    
    current_process = next;
    swap_rfiles(&(current->state), &(next->state));
}

void really_exit(void )
{
    thread current = current_process;
    thread dummy = current->sched_one;
    
    thread next;
    thread libprev = NULL;
    if(NULL == (next = RoundRobin->next()))
    {
        swap_rfiles(NULL, &main_rfile);
    }
    if(next->tid == current->tid)
    {
        next = dummy;
    }
    /*lets find current in lib list */
    thread libcurrent = lib_head_ptr;
    while(libcurrent->lib_one != NULL)
    {
        if(libcurrent->tid == current->tid)
        {
            if(libprev == NULL)
            {
                /*current lwp is head of liblist */
                lib_head_ptr = libcurrent->lib_one;
                current_process = next;
                RoundRobin->remove(libcurrent);
                free(libcurrent->stack);
                free(libcurrent);
                swap_rfiles(NULL, &(next->state));
            }
            else
            {
                /*current lwp is not head of list */
                /*nor can it be the tail */
                libprev->lib_one = libcurrent->lib_one;
                current_process = next;
                RoundRobin->remove(libcurrent);
                free(libcurrent->stack);
                free(libcurrent);
                swap_rfiles(NULL, &(next->state));
            }
            
        }
        libprev = libcurrent;
        libcurrent = libcurrent->lib_one;
        
    }
    /*lib current is the last thread in lib list */
    if(libprev == NULL)
    {
        /*list only has one thread in it */
        /*terminate its context carefully */
        current_process = NULL;
        lib_head_ptr = NULL;
        RoundRobin->remove(libcurrent);
        free(libcurrent->stack);
        free(libcurrent);
        swap_rfiles(NULL, &(main_rfile));
        
    }
    else
    {
        /*list have more than one thread in it */
        /*this means that im removing my tail */
        libprev->lib_one = NULL;
        current_process = next;
        RoundRobin->remove(libcurrent);
        free(libcurrent->stack);
        free(libcurrent);
        swap_rfiles(NULL, &(next->state));
    }
    
}

void lwp_exit(void)
{
    SetSP(main_rfile.rsp);
    really_exit();
}

void lwp_start(void)
{
    thread next;
    if(NULL == (next = RoundRobin->next()))
    {
        current_process = NULL;
        return;
    } 
    current_process = next;
    swap_rfiles(&(main_rfile), &(next->state));

}

void lwp_stop(void)
{
    thread dummy = current_process;
    current_process = NULL;
    
    swap_rfiles(&(dummy->state), &(main_rfile));
}

void lwp_set_scheduler(scheduler fun)
{
    tid_t seen;
    thread next;
    if(fun == NULL)
    {
        return;
    }
    scheduler current = RoundRobin;
    if(fun->init)
    {
        fun->init();
    }
    while(NULL != (next = current->next()))
    {
        
        current->remove(next);
        fun->admit(next);
    } 
    if(RoundRobin->shutdown)
    {
        RoundRobin->shutdown();
    }
    RoundRobin = fun;
    return;
}

scheduler lwp_get_scheduler(void)
{
    return RoundRobin;
}

thread tid2thread(tid_t tid)
{
    /*rememver tid_t is unsgined long */
    thread current = lib_head_ptr;
    if(current == NULL)
    {
        return NULL;
    }
    if(tid < 1 || tid > counter)
    {   
        return NULL;
    }
    while(current->lib_one != NULL)
    {
        if(current->tid == tid)
        {
            /* I have found my thread id */
            return current;
        } 
        current = current->lib_one;
    }
    if(current->tid == tid)
    {
        return current;
    }
    return NULL;
}

