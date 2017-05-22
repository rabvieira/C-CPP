/* Developed by Renan Vieira /*

/*igraph library */
#include <igraph.h>
#include <math.h>

/* constants */
#define vertex_n 1000 //number of vertex
#define edge_m 3000   //number of edges
#define DFS_order_max 20 //size of the depth-first search

igraph_vector_t depth;  //stores the depths from 0 to DFS_order_max
igraph_matrix_t desvio;

/*
//Callback type for the DFS function. If TRUE, stop the DFS
igraph_bool_t dfs_callback(const igraph_t *graph, //input graph
			   igraph_integer_t vid,   //id of the vertex just found
			   igraph_integer_t dist,  //distance (number of hops) of the current vertex from the root
			   void *extra)            //extra: argument to passed
{
  //printf(" %li", (long int) dist);
  return 0;
}
*/

void print_matrix(igraph_matrix_t *m)
{
    long int i, j;
    for (i=0 ; i<igraph_matrix_nrow(m) ; i++)
    {
        for (j=0 ; j<igraph_matrix_ncol(m) ; j++)
        {
            printf("%li ", (long int)MATRIX(*m, i, j));
        }
        printf("\n");
    }
}

//Calcula o desvio padrao do atributo referente a coluna col
float desvio_padrao(igraph_matrix_t *m, float media, int line){
    int i;
    float res=0;
    for(i=0 ; i<igraph_matrix_ncol(m) ; i++)
        res += pow((MATRIX(*m, line, i) - media),2);
    res = res/(igraph_matrix_ncol(m)-1);
    return (sqrt(res));
}

void print_results(igraph_vector_t *depth, igraph_matrix_t *desvio, FILE *f)
{
    igraph_matrix_t copy;
    long int i,j;
    float desvio_p=0;
    float media=0;
    igraph_matrix_copy(&copy, desvio);
    for(i=0 ; i<igraph_matrix_nrow(desvio) ; i++)
    {
        MATRIX(*desvio, i, 0) = MATRIX(*desvio, i, 0) -1;
    }
    for(i=0 ; i<igraph_matrix_nrow(desvio) ; i++)
    {
        for (j=1 ; j<igraph_matrix_ncol(desvio) ; j++)
        {
            MATRIX(*desvio, i, j) = (MATRIX(copy, i, j) - MATRIX(copy, i, j-1)) -1;
        }
    }
    fprintf(f, "V=%d \t E=%d \n", vertex_n, edge_m);
    fprintf(f, "depth\tmedia\tdesvio\t\tperc.media\n");
    printf("depth\tmedia\tdesvio\t\tperc.media\n");
    for (i=0 ; i<igraph_vector_size(depth) ; i++)
    {
        media = ((float)VECTOR(*depth)[i]-vertex_n) / vertex_n;
        desvio_p = desvio_padrao(desvio, media, i);
        fprintf(f, "%li\t%.3f\t%.3f\t\t%.3f\n", i, media, desvio_p, ((float)VECTOR(*depth)[i] - vertex_n)  / (vertex_n*vertex_n));
        printf("%li\t%.3f\t%.3f\t\t%.3f\n", i, media, desvio_p, ((float)VECTOR(*depth)[i] - vertex_n)  / (vertex_n*vertex_n));
    }
    igraph_matrix_destroy(&copy);
}

void save_depth(igraph_vector_t *dist, int current_DFS)
{
    int i, sum=0;
    //igraph_vector_print(dist);
    for(i=0 ; i<igraph_vector_size(dist) ; i++)
    {
        if(VECTOR(*dist)[i] <= current_DFS)
        {
            sum += 1;
        }
    }
    VECTOR(depth)[current_DFS] += sum;
}
//420
void print_graph(const igraph_t *graph)
{
    FILE *plot;
    plot = fopen("plot.dot","w");

    //if (igraph_is_directed(graph))
    //    fprintf(plot, "directed\n");
    //else
    //    fprintf(plot, "undirected\n");

  //igraph_write_graph_edgelist(graph, plot);
  //fprintf(plot, "-----------------\n");
  igraph_write_graph_dot(graph, plot);

}

int main(void)
{
    igraph_t graph;             //input graph
    igraph_vector_t order;      //vertex id as they were discovered
    igraph_vector_t order_out;  //vertex id as they were completed of their subtree
    igraph_vector_t father, dist;
    int i, root;                //root id
    int mode=0;
    FILE *outstream;
    printf("escolha:\n[0] DFS\t  ou\t[1] BFS: ");
    scanf("%d", &mode);
    if(mode>1 || mode<0)
        mode = 0;
/*
    igraph_erdos_renyi_game(&graph,                 //uninitialized graph object
                            IGRAPH_ERDOS_RENYI_GNM, //G(n,m): m edges are selected uniformly randomly in a graph with n vertices
                            vertex_n,               //the number of vertices
                            edge_m,                 //the number of edges
                            IGRAPH_UNDIRECTED,      //undirected graph
                            IGRAPH_NO_LOOPS);       //no loop (self) edges allowed
*/
    igraph_barabasi_game(&graph, vertex_n, /*power=*/ 1, edge_m / vertex_n, 0, 0, /*A=*/ 1, IGRAPH_UNDIRECTED,
                        IGRAPH_BARABASI_BAG, /*start_from=*/ 0);
    //igraph_full(&graph, vertex_n, IGRAPH_UNDIRECTED, IGRAPH_NO_LOOPS);
    //igraph_tree(&graph, vertex_n, 2, IGRAPH_TREE_UNDIRECTED);
    //igraph_ring(&graph, vertex_n, 0, 0, 1);
    //igraph_full(&graph, vertex_n, IGRAPH_UNDIRECTED, IGRAPH_NO_LOOPS);
    //igraph_grg_game(&graph, vertex_n, sqrt(2.0)/2, 1, 0, 0); //full
    //igraph_barabasi_game(&graph, vertex_n, /*power=*/ 1, edge_m / vertex_n, 0, 0, /*A=*/ 1, IGRAPH_UNDIRECTED,
    //                    IGRAPH_BARABASI_BAG, /*start_from=*/ 0);
    //igraph_k_regular_game(&graph, vertex_n, edge_m / vertex_n, 0, 0);
/*
    igraph_erdos_renyi_game(&graph,                 //uninitialized graph object
                            IGRAPH_ERDOS_RENYI_GNP, //G(n,p): every possible edge is included in the graph with probability p
                            vertex_n,               //the number of vertices
                            0.3,                    //probability p
                            IGRAPH_UNDIRECTED,      //undirected graph
                            IGRAPH_NO_LOOPS);       //no loop (self) edges allowed
*/


    igraph_vector_init(&order, 0);
    igraph_vector_init(&order_out, 0);
    igraph_vector_init(&father, 0);
    igraph_vector_init(&dist, 0);
    igraph_vector_init(&depth, DFS_order_max+1);
    igraph_matrix_init(&desvio, DFS_order_max+1, vertex_n);

    if(!mode)
    {
        //DFS
        for(i=0 ; i<=DFS_order_max ; i++)
        {
            for(root=0 ; root<vertex_n ; root++)
            {
                //printf("callback:\n");
                igraph_dfs(&graph,          //input graph
                               root,        //id of the root vertex
                               IGRAPH_ALL,  //mode: defines which edges to follow
                               0,           //unreachable: additional searches are performed
                               &order,      //vertex ids in the same order as they were discovered
                               &order_out,  //vertex ids in the order of the completion of their subtree
                               &father,     //father: id of the father of each vertex
                               &dist,       //dist: distance from the root of the current search tree
                               0,           //in_callback: pointer to a function which is called whenever a new vertex is discovered (&dfs_callback)
                               0,           //out_callback: pointer to a function which is called whenever the subtree of a vertex is completed
                               0);          //extra: argument to pass to the callback function(s)

                save_depth(&dist,i);
                //igraph_vector_print(&dist);
                //VECTOR(desvio)[root] = VECTOR(depth)[i];
                MATRIX(desvio, i, root) = VECTOR(depth)[i];
            }
            //MATRIX(desvio, i, root) = VECTOR(depth)[i];
        }
    }

    else
    {
        igraph_vector_t rank;
        igraph_vector_init(&rank, 0);
        //BFS
        for(i=0 ; i<=DFS_order_max ; i++)
        {
            for(root=0 ; root<vertex_n ; root++)
            {
                //printf("callback:\n");
                igraph_bfs(&graph,          //input graph
                               root,        //id of the root vertex
                               0,           //roots: ordered vector
                               IGRAPH_ALL,  //mode: defines which edges to follow
                               0,           //unreachable: additional searches are performed
                               0,           //restricted vertex
                               &order,      //vertex ids in the same order as they were discovered
                               &rank,       //vertex ids in the order of the completion of their subtree
                               &father,     //father: id of the father of each vertex
                               0,           //pred: id before the current
                               0,           //succ: id after the current
                               &dist,       //dist: distance from the root of the current search tree
                               0,           //in_callback: pointer to a function which is called whenever a new vertex is discovered (&dfs_callback)
                               0);          //extra: argument to pass to the callback function(s)

                save_depth(&dist,i);
                //igraph_vector_print(&dist);
                //VECTOR(desvio)[root] = VECTOR(depth)[i];
                MATRIX(desvio, i, root) = VECTOR(depth)[i];
            }
            //MATRIX(desvio, i, root) = VECTOR(depth)[i];
        }
        igraph_vector_destroy(&rank);
    }
//---------------------------------------------------------------------

    //printf("\n\norder:\n");
    //igraph_vector_print(&order);
    //printf("order_out:\n");
    //igraph_vector_print(&order_out);

    //printf("depth:\n");
    //igraph_vector_print(&depth);
    //printf("desvio:\n");
    //print_matrix(&desvio);
    outstream = fopen("barabasi.txt","w");
    //outstream = fopen("erdos_renyi.txt","w");
    print_results(&depth, &desvio, outstream);

    print_graph(&graph);
    //printf("desvio:\n");
    //print_matrix(&desvio);

    igraph_vector_destroy(&order);
    igraph_vector_destroy(&order_out);
    igraph_vector_destroy(&father);
    igraph_vector_destroy(&dist);
    igraph_vector_destroy(&depth);
    igraph_matrix_destroy(&desvio);
    igraph_destroy(&graph);
    return 0;
}


