
/**
 *   \file     cstack.h
 *   \version  0.04
 *   \data     2023.12.10
 */

#ifndef  CSTACK_H
#define  CSTACK_H

#include  <stdint.h>


typedef  int  hstack_t;


typedef  struct  stack_t_element
{
	uint32_t  size_element;
    void  * data;
	struct  stack_t_element  * prev;
}  stack_t_element;


/* Узел */
typedef  struct  node_t
{
    hstack_t  hstack;
	stack_t_element  * stack;
	int32_t  stack_size;
	struct  node_t  * next;  
	struct  node_t  * prev;
}  node_t;


/* Список */
typedef  struct
{
    node_t  * head;
	node_t  * tail;
}  list_t;


hstack_t  stack_new( void );
void  stack_free( const  hstack_t  stack );
int  stack_valid_handler( const  hstack_t  stack );
unsigned  int  stack_size( const  hstack_t  stack );
void  stack_push( const  hstack_t  stack, const  void  * data_in, const  unsigned  int  size );
unsigned  int  stack_pop( const  hstack_t  stack, void  * data_out, const  unsigned  int  size );

#endif /* CSTACK_H */

