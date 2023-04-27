#ifndef __NW_GRAPH_
#define __NW_GRAPH_

#include <assert.h>
#include "gluethread/glthread.h"


#define ROUTER_NAME_SIZE        16
#define SLOT_NAME_SIZE          16
#define MAX_SLOTS_PER_ROUTER    10
#define GRAPH_NAME_SIZE      32

/*Forward Declarations*/
typedef struct router_ router_t;
typedef struct link_ link_t;

typedef struct slot_ {
    char name[SLOT_NAME_SIZE];
    router_t *owning_router;
    link_t *link;
} slot_t;

struct link_ {
    slot_t from_slot;
    slot_t to_slot;
    unsigned int cost;
}; 

struct router_ {
    char name[ROUTER_NAME_SIZE];
    slot_t *slots[MAX_SLOTS_PER_ROUTER];
    glthread_t graph_glue;
};

GLTHREAD_TO_STRUCT(graph_glue_to_router, router_t, graph_glue);

typedef struct graph_ {
    char name[GRAPH_NAME_SIZE];
    glthread_t router_list; 
} graph_t;

router_t *
create_graph_router(graph_t *graph, char *router_name);

graph_t *
create_new_graph(char *name);

void
insert_link_between_two_routers(
    router_t *router_1, 
    router_t *router_2,
    char *from_slot_name, 
    char *to_slot_name, 
    unsigned int cost);

/*Helper functions*/
static router_t *
get_nbr_router(slot_t *slot);

static int
get_router_available_slot(router_t *router);

/*Display Routines*/
void print_graph(graph_t *graph);
void print_router(router_t *router);
void print_slot(slot_t *slot);


#endif /* __NW_GRAPH_ */

