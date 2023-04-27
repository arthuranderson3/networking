#include "graph.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

router_t *
get_nbr_router(slot_t *slot){

    assert(slot->owning_router);
    assert(slot->link);
    
    link_t *link = slot->link;
    if(&link->from_slot == slot)
        return link->to_slot.owning_router;
    else
        return link->from_slot.owning_router;
}

int
get_router_available_slot(router_t *router){

    int i ;
    for( i = 0 ; i < MAX_SLOTS_PER_ROUTER; i++){
        if(router->slots[i])
            continue;
        return i;
    }
    return -1;
}

void
insert_link_between_two_routers(router_t *router_1,
        router_t *router_2,
        char *from_slot_name,
        char *to_slot_name,
        unsigned int cost){

    link_t *link = calloc(1, sizeof(link_t));

    /*Set interface properties*/
    strncpy(link->from_slot.name, from_slot_name, SLOT_NAME_SIZE);
    link->from_slot.name[SLOT_NAME_SIZE - 1] = '\0';
    strncpy(link->to_slot.name, to_slot_name, SLOT_NAME_SIZE);
    link->to_slot.name[SLOT_NAME_SIZE - 1] = '\0';
    
    link->from_slot.link = link; /*set back pointer to link*/
    link->to_slot.link = link; /*set back pointer to link*/

    link->from_slot.owning_router = router_1;
    link->to_slot.owning_router = router_2;
    link->cost = cost;

    int empty_router_slot;

    /*Plugin Slots ends into Router*/
    empty_router_slot = get_router_available_slot(router_1);
    router_1->slots[empty_router_slot] = &link->from_slot;

    empty_router_slot = get_router_available_slot(router_2);
    router_2->slots[empty_router_slot] = &link->to_slot;
}

graph_t *
create_new_graph(char *name){

    graph_t *graph = calloc(1, sizeof(graph_t));
    strncpy(graph->name, name, GRAPH_NAME_SIZE);
    graph->name[GRAPH_NAME_SIZE - 1] = '\0';

    init_glthread(&graph->router_list);
    return graph;
}

router_t *
create_graph_router(graph_t *graph, char *name){

    router_t *router = calloc(1, sizeof(router_t));
    strncpy(router->name, name, ROUTER_NAME_SIZE);
    router->name[ROUTER_NAME_SIZE - 1] = '\0';

    init_glthread(&router->graph_glue);
    glthread_add_next(&graph->router_list, &router->graph_glue);
    return router;
}

void 
print_graph(graph_t *graph){

    glthread_t *curr;
    router_t *router;
    
    printf("Graph name = %s\n", graph->name);

    ITERATE_GLTHREAD_BEGIN(&graph->router_list, curr){

        router = graph_glue_to_router(curr);
        print_router(router);    
    } ITERATE_GLTHREAD_END(&graph->router_list, curr);
}

void 
print_router(router_t *router){

    unsigned int i = 0;
    slot_t *slot;

    printf("Router %s : \n", router->name);
    for( ; i < MAX_SLOTS_PER_ROUTER; i++){
        
        slot = router->slots[i];
        if(!slot) break;
        print_slot(slot);
    }
}

void 
print_slot(slot_t *slot){

   link_t *link = slot->link;
   router_t *nbr_router = get_nbr_router(slot);

   printf(" Local : %s, Slot %s, Nbr : %s, cost : %u\n", 
            slot->owning_router->name, 
            slot->name, nbr_router->name, link->cost); 
}

