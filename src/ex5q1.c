#include <stdio.h>
#include <stdlib.h>

// TODO: Implement these functions
int nodeExists(int id);
int edgeExists(int source_id, int dest_id);
int insertNode(int id);
int insertEdge(int source_id, int dest_id, int weight);
int removeEdge(int source_id, int dest_id);
int removeNode(int id);
int path(int source_id);
void start();
void end();

struct Edge {
    int dest_id;
    int weight;
    struct Edge *next;
};

struct Node {
    int id; /* data stored in the node */
    struct Edge *edges; /* Pointer to the head of the list of edges */
    struct Node *next; /* pointer to the next node */
};

struct Node *first = NULL;

// Checks if a node with the given ID exists
int nodeExists(int id) {
    struct Node *current = first;
    while (current) {
        if (current->id == id) {
            return 1; // Node found
        }
        current = current->next;
    }
    return 0; // Node not found
}

// This function helps us check if an edge exists between source and destination nodes
int edgeExists(int source_id, int dest_id) {
    if (!nodeExists(source_id) || !nodeExists(dest_id)) {
        return 0; // This returns 0 (edge does not exist) as either source node or destination node dont exist
    }

    struct Node *source = first; 
    while (source && source -> id != source_id) {
        source = source->next;
    }
    if (!source) {
        return 0; // Source node does not exist
    }
    struct Edge *edge = source -> edges;
    while (edge) {
        if (edge -> dest_id == dest_id) {
            return 1; // Edge exists
        }
        edge = edge->next;
    }
    return 0; // Edge does not exist
}

// Inserts a new node with the given ID
int insertNode(int id) {
    if (nodeExists(id)) {
        return 0; // Node already exists
    }

    struct Node *newNode = malloc(sizeof(struct Node));
    if (!newNode) {
        return 0; // Memory allocation failed
    }

    newNode->id = id;
    newNode->edges = NULL;
    newNode->next = first;
    first = newNode;

    return 1; // Node successfully added
}

// Inserts or updates an edge between source and destination nodes
int insertEdge(int source_id, int dest_id, int weight) {
    if (source_id == dest_id) {
        return 0; // Cannot connect a node to itself
    }
    if (!nodeExists(source_id) || !nodeExists(dest_id)) {
        return 0; // One of the nodes does not exist
    }

    struct Node *source = first;
    while (source && source->id != source_id) {
        source = source->next;
    }

    // Check if the edge already exists
    struct Edge *edge = source->edges;
    while (edge) {
        if (edge->dest_id == dest_id) {
            edge->weight = weight; // Update the weight
            return 1; // Edge updated successfully
        }
        edge = edge->next;
    }

    // Edge does not exist; create a new one
    struct Edge *newEdge = malloc(sizeof(struct Edge));
    if (!newEdge) {
        return 0; // Memory allocation failed
    }

    newEdge->dest_id = dest_id;
    newEdge->weight = weight;
    newEdge->next = source->edges;
    source->edges = newEdge;

    return 1; // Edge successfully added
}

// Removes a node with the given ID and all associated edges
int removeNode(int id) {
    struct Node *current = first;
    struct Node *prev = NULL;

    // Find the node to be removed
    while (current && current->id != id) {
        prev = current;
        current = current->next;
    }

    if (!current) {
        return 0; // Node does not exist
    }

    // Remove all edges originating from the node
    struct Edge *edge = current->edges;
    while (edge) {
        struct Edge *temp = edge;
        edge = edge->next;
        free(temp);
    }
    current->edges = NULL;

    // Remove all edges pointing to this node
    struct Node *node_iter = first;
    while (node_iter) {
        if (node_iter->id != id) {
            removeEdge(node_iter->id, id);
        }
        node_iter = node_iter->next;
    }

    // Remove the node from the linked list
    if (prev) {
        prev->next = current->next;
    } else {
        first = current->next; // Node to be deleted is the first node
    }

    free(current); // Free the node

    return 1; // Node successfully removed
}

// Removes an edge from source to destination node
int removeEdge(int source_id, int dest_id) {
    struct Node *current = first;

    // Find the source node
    while (current && current->id != source_id) {
        current = current->next;
    }

    if (!current) {
        return 0; // Source node does not exist
    }

    // Find the edge to remove
    struct Edge *edge = current->edges;
    struct Edge *prev_edge = NULL;

    while (edge && edge->dest_id != dest_id) {
        prev_edge = edge;
        edge = edge->next;
    }

    if (!edge) {
        return 0; // Edge does not exist
    }

    // Remove the edge
    if (prev_edge) {
        prev_edge->next = edge->next;
    } else {
        current->edges = edge->next; // Edge is the first in the list
    }

    free(edge); // Free the edge

    return 1; // Edge successfully removed
}

// Traverses a path and sums the weights of the edges
int path(int source_id) {
    int capacity = 10; // Initial capacity for dynamic array
    int size = 1;      // Start with the source_id
    int *id_list = malloc(capacity * sizeof(int));
    if (!id_list) {
        return -1; // Memory allocation failed
    }
    id_list[0] = source_id;

    char line[1024];
    // Read the rest of the line
    if (!fgets(line, sizeof(line), stdin)) {
        free(id_list);
        return -1;
    }

    // Parse integers from the line
    char *ptr = line;
    int id;
    int res;
    while (sscanf(ptr, "%d%n", &id, &res) == 1) {
        if (size == capacity) {
            capacity *= 2;
            int *new_list = realloc(id_list, capacity * sizeof(int));
            if (!new_list) {
                free(id_list);
                return -1; // Memory allocation failed
            }
            id_list = new_list;
        }
        id_list[size++] = id;
        ptr += res;
    }

    // Validate that the path has at least two nodes
    if (size < 2) {
        free(id_list);
        return -1;
    }

    int total_weight = 0;
    // Traverse the path and sum the weights
    for (int i = 0; i < size - 1; i++) {
        int source_id = id_list[i];
        int dest_id = id_list[i + 1];

        // Find the source node
        struct Node *source_node = first;
        while (source_node && source_node->id != source_id) {
            source_node = source_node->next;
        }
        if (!source_node) {
            free(id_list);
            return -1; // Source node does not exist
        }

        // Find the edge from source to destination
        struct Edge *edge = source_node->edges;
        while (edge && edge->dest_id != dest_id) {
            edge = edge->next;
        }
        if (!edge) {
            free(id_list);
            return -1; // Edge does not exist
        }

        total_weight += edge->weight;
    }

    free(id_list);
    return total_weight;
}

// Initializes the graph
void start() {
    first = NULL;
}

// Frees all allocated memory
void end() {
    struct Node *current_node = first;
    while (current_node) {
        // Free all edges of the current node
        struct Edge *current_edge = current_node->edges;
        while (current_edge) {
            struct Edge *temp_edge = current_edge;
            current_edge = current_edge->next;
            free(temp_edge);
        }
        // Move to the next node and free the current node
        struct Node *temp_node = current_node;
        current_node = current_node->next;
        free(temp_node);
    }
    first = NULL;
}




// ! DO NOT EDIT THE MAIN FUNCTION
int main(int argc, char **argv)
{
    start();
    
    // Input loop
    int result;
    while (1)
    {
        char command;
        scanf("%c", &command);

        if (command == 't')
        {
            break;
        }
        else if (command == 'n')
        {
            int id;
            scanf("%d", &id);
            result = nodeExists(id);
            printf("%d\n", result);
        }
        else if (command == 'e')
        {
            int source_id, dest_id;
            scanf("%d %d", &source_id, &dest_id);
            result = edgeExists(source_id, dest_id);
            printf("%d\n", result);
        }
        else if (command == 'i')
        {
            int id;
            scanf("%d", &id);
            result = insertNode(id);
            printf("%d\n", result);
        }
        else if (command == 'l')
        {
            int source_id, dest_id, weight;
            scanf("%d %d %d", &source_id, &dest_id, &weight);
            result = insertEdge(source_id, dest_id, weight);
            printf("%d\n", result);
        }
        else if (command == 'd')
        {
            int id;
            scanf("%d", &id);
            result = removeNode(id);
            printf("%d\n", result);
        }
        else if (command == 'r')
        {
            int source_id, dest_id;
            scanf("%d %d", &source_id, &dest_id);
            result = removeEdge(source_id, dest_id);
            printf("%d\n", result);
        }
        else if (command == 'p')
        {
            int source_id;
            scanf("%d", &source_id);
            result = path(source_id);
            printf("%d\n", result);
        }
    }

    end();
    return 0;
}
