#ifndef _GRAPH_H_
#define _GRAPH_H_

typedef enum {
	OK = 3,          	  
   	OOM = -1,    		  
	FAIL = -2,		  
	DUBBED = -3
} ErrorCode;

typedef struct Graph Graph;
typedef struct KeySpace KeySpace;
typedef struct List List;

Graph *graphMake();
ErrorCode graphErase(Graph *graph);
ErrorCode graphClean(Graph *graph);

ErrorCode graphInsertNode(Graph **graph, const char *source);
ErrorCode graphDeleteNode(Graph *graph, const char *node);
ErrorCode graphInsertEdge(Graph *graph, const char *source, const char *drain, short weight);
ErrorCode graphDeleteEdge(Graph *graph, const char *source, const char *drain);
ErrorCode graphChangeNode(Graph **graph, const char *old, const char *);
ErrorCode graphChangeEdge(Graph *graph, const char *source, const char *drain, short weight);

ErrorCode graphPrint(Graph *graph);
ErrorCode graphDot(Graph *graph, char *file_name);

ErrorCode BFS(Graph *graph, const char *node, short *visited);
ErrorCode graphBFS(Graph *graph, const char *node, short hands);
//ErrorCode graphDFS(Graph *graph, const char *node, short hand);
ErrorCode graphShortestPass(Graph *graph, short *weight, const char *node, const char *drain, List **list);
ErrorCode graphGroup(Graph *graph);


List *listMake();
ErrorCode listErase(List *list);
ErrorCode listPrint(List *list);

#endif
