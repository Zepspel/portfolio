#ifndef _DIALOG_H_
#define _DIALOG_H_


ErrorCode getInt(short *intp, short low, short high);
ErrorCode getIntRight(short *m, char *c, short low, short high);
ErrorCode getWord(char *choice);
ErrorCode getWordPattern(char *choice, const char * const *option, int amount_option);
ErrorCode getWordRight(char *word);

void caseHelp();
void caseCleanGraph(Graph *graph);
ErrorCode caseSearchNode(Graph *graph);
ErrorCode caseBFS(Graph *graph);
ErrorCode caseShortestPass(Graph *graph);

ErrorCode caseInsertNode(Graph **graph);
ErrorCode caseInsertEdge(Graph *graph);
ErrorCode caseDeleteNode(Graph *graph);
ErrorCode caseDeleteEdge(Graph *graph);
ErrorCode caseChange(Graph **graph);

void casePrintGraph(Graph *graph);
void caseExportGraph(Graph *graph);

ErrorCode dialog(Graph *graph);




#endif
