#ifndef __LISTA__
#define __LISTA__


typedef struct info_node * info_deikti;
typedef struct typos_komvou * typos_deikti;


info_deikti LIST_dimiourgia(info_deikti*);
void eisagogi(info_deikti* linfo,int, double);
void lista_diagrafi(info_deikti* linfo);
int size_of_list(info_deikti* linfo);
void take_product(info_deikti* linfo, int komvos,int *,double *);


#endif
