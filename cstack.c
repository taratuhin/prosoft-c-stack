
/**
 *   \file     cstack.c
 *   \version  0.05
 *   \data     2023.12.10
 */

#include  "cstack.h"
#include  <stddef.h>
#include  <stdlib.h>
#include  <string.h>


static  list_t  * list_stacks;


/**
 *   \brief   Создание стека
 *   \param   Нет
 *   \return  Дескриптор стека
 */
hstack_t  stack_new( void )
{
    hstack_t  hstack = -1;
    node_t  * node = NULL;


    if ( list_stacks )
    {
        if ( ( list_stacks->head ) && ( list_stacks->tail ) )
        {
            node = ( node_t  * ) malloc( sizeof ( node_t ) );

            if ( node )
            {
                node->hstack = list_stacks->head->hstack + 1;
                node->stack = NULL;
                node->stack_size = 0;
                node->prev = list_stacks->head;
                node->next = NULL;
                list_stacks->head->next = node;
                list_stacks->head = node;
                hstack = node->hstack;
            }
            else
            {
                hstack = -1;
            }
        }
        else
        {
            node = ( node_t  * ) malloc( sizeof ( node_t ) );

            if ( node )
            {
                node->hstack = 0;
                node->stack = NULL;
                node->stack_size = 0;
                node->next = NULL;
                node->prev = NULL;

                if ( ( list_stacks->tail == NULL ) && ( list_stacks->head == NULL ) )
                {
                    list_stacks->head = node;
                    list_stacks->tail = node;
                }
                else
                {
                    free( node );
                }

                hstack = 0;
            }
            else
            {
                hstack = -1;
            }
        }
    }
    else
    {
        list_stacks = ( list_t  * ) malloc( sizeof ( list_t ) );

        list_stacks->head = NULL;
        list_stacks->tail = NULL;

        node = ( node_t  * ) malloc( sizeof ( node_t ) );

        if ( node )
        {
            node->hstack = 0;
            node->stack = NULL;
            node->stack_size = 0;
            node->next = NULL;
            node->prev = NULL;

            list_stacks->head = node;
            list_stacks->tail = node;

            hstack = 0;
        }
        else
        {
            hstack = -1;
        }
    }

    return  hstack;
}


/**
 *   \brief   Удалить стек
 *   \param   hstack - хэндлер
 *   \return  Нет
 */
void  stack_free( const  hstack_t  hstack )
{
    node_t  * node = NULL;
    stack_t_element  * prev = NULL;


    if ( ( list_stacks ) && ( hstack >= 0 ) )
    {
        node = list_stacks->head;
    
        while ( node )
        {
            if ( node->hstack == hstack )
            {
                if ( node->stack )
                {
                    while ( node->stack->prev )
                    {
                        prev = node->stack->prev;
                        free( node->stack->data );
                        free( node->stack );
                        node->stack = prev;
                    }

                    if ( node->stack )
                    {
                        free( node->stack->data );
                        free( node->stack );
                        node->stack = NULL;
                    }
                }

                if ( node->prev )
                {
                    node->prev->next = node->next;
                }
                else
                {
                    list_stacks->head = node->next;
                }

                if ( node->next )
                {
                    node->next->prev = node->prev;
                }
                else
                {
                    list_stacks->tail = node->prev;
                }

                free( node );

                if ( list_stacks->head == list_stacks->tail )
                {
                    free( list_stacks );
                    list_stacks = NULL;
                }

                break;
            }

            node = node->prev;
        }
    }
}


/**
 *   \brief   Проверка хэндлера
 *   \param   hstack - хэндлер
 *   \return  0 - соответствующий хэндлеру стек существует, 1 - нет
 */
int  stack_valid_handler( const  hstack_t  hstack )
{
    int32_t  valid_handler = 1;
    node_t  * node = NULL;


    if ( hstack >= 0 )
    {
        if ( list_stacks )
        {
            node = list_stacks->head;

            while ( node )
            {
                if ( node->hstack == hstack )
                {
                    valid_handler = 0;

                    break;
                }

                node = node->prev;
            }
        }
    }

    return  valid_handler;
}


/**
 *   \brief   Получить количество элементов в стеке
 *   \param   hstack - хэндлер
 *   \return  Количество элементов в стеке
 */
unsigned  int  stack_size( const  hstack_t  hstack )
{
    uint32_t  size = 0;
    node_t  * node = NULL;


    if ( hstack >= 0 )
    {
        node = list_stacks->head;

        while ( node )
        {
            if ( node->hstack == hstack )
            {
                size = node->stack_size;

                break;
            }

            node = node->next;
        }
    }

    return  size;
}


/**
 *   \brief   Добавление данных в стек
 *   \param   hstack - хэндлер стека
 *   \param  *data_in - дынне
 *   \param   size - размер данных
 *   \return  Нет
 */
void  stack_push( const  hstack_t  hstack, const  void  * data_in, const  unsigned  int  size )
{
    node_t  * node = NULL;
    stack_t_element  * element = NULL;


    if ( ( list_stacks ) && ( hstack >= 0 ) && ( data_in ) && ( size ) )
    {
        node = list_stacks->tail;

        while ( node )
        {
            if ( node->hstack == hstack )
            {
                element = ( stack_t_element  * ) malloc( sizeof ( stack_t_element ) );

                if ( element )
                {
                    node->stack_size++;

                    if ( node->stack )
                    {
                        element->prev = node->stack;
                    }
                    else
                    {
                        element->prev = NULL;
                    }

                    element->size_element = size;
                    element->data = malloc( size );

                    if ( element->data )
                    {
                        memcpy( element->data, data_in, size );
                    }

                    node->stack = element;
                }

                break;
            }

            node = node->next;
        }
    }
}


/**
 *   \brief   Извлечение данных из стека
 *   \param   hstack - хэндлер
 *   \param  *data_out - данные
 *   \param   size - размер данных
 *   \return  Размер извлеченных данных из стека, 0 если указанного стека не существует
 */
unsigned  int  stack_pop( const  hstack_t  hstack, void  * data_out, const  unsigned  int  size )
{
    uint32_t  size_read_data = 0U;
    node_t  * node = NULL;
    stack_t_element  * element = NULL;


    if ( ( list_stacks ) && ( hstack >= 0 ) && ( data_out ) && ( size ) )
    {
        node = list_stacks->head;

        while ( node )
        {
            if ( node->hstack == hstack )
            {
                if ( node->stack_size )
                {
                    if ( node->stack->size_element == size )
                    {
                        node->stack_size--;

                        memcpy( data_out, node->stack->data, size );
                        element = node->stack->prev;
                        free( node->stack->data );
                        free( node->stack );
                        node->stack = element;
                        size_read_data = size;
                    }
                }

                break;
            }

            node = node->next;
        }
    }

    return  size_read_data;
}

