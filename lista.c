//File: lista.c
#include <stdio.h>
#include <stdlib.h>
#include "lista.h"
#include "ask1.h"
#include <stdint.h>


 typedef struct info_node{
 	int size;
	typos_deikti arxi;
	typos_deikti telos;
 }info_node;


 typedef struct typos_komvou{
 	mystruct proion;
	/***************/
	typos_deikti epomenos;
	typos_deikti proigoumenos;
 }typos_komvou;





 info_deikti LIST_dimiourgia(info_deikti* lista){

 	info_deikti linfo;
 	linfo = malloc(sizeof(info_node));
 	linfo->size = 0;
 	linfo->arxi = NULL;
	linfo->telos = NULL;

 	return linfo;
 }



 void eisagogi(info_deikti* linfo,int a,double b){
 	typos_deikti prosorinos;
 	prosorinos = malloc(sizeof(typos_komvou));
 	prosorinos->proion.part_number = a;
	prosorinos->proion.time = b;


 	if((*linfo)->size == 0){
 		(*linfo)->arxi = prosorinos;
		(*linfo)->telos = prosorinos;
		prosorinos->epomenos = NULL;
		prosorinos->proigoumenos = NULL;

 	}
 	if((*linfo)->size > 0){
		prosorinos->proigoumenos = (*linfo)->telos;
		prosorinos->epomenos = NULL;
 		(*linfo)->telos->epomenos = prosorinos;
		(*linfo)->telos = prosorinos;
 	}
 	(*linfo)->size++;

	return;
 }


 void lista_diagrafi(info_deikti* linfo){
 	typos_deikti mapas = (*linfo)->arxi;
	typos_deikti mapas2;


 	while(mapas != NULL){
		mapas2 = mapas;
		mapas = mapas->epomenos;
		free(mapas2);
	}
	(*linfo)->arxi = NULL;
	(*linfo)->telos = NULL;
	(*linfo)->size = 0;
	free(*linfo);
	(*linfo) = NULL;

	return;
 }


 int size_of_list(info_deikti* linfo){
 	return (*linfo)->size;
 }



 void take_product(info_deikti* linfo, int komvos,int *a,double *b){

 	typos_deikti mapas = (*linfo)->arxi;
        typos_deikti mapas2;
        while(komvos-- > 0){
        	mapas2 = mapas;
                mapas = mapas->epomenos;
        }

	*a = mapas2->proion.part_number;
	*b = mapas2->proion.time;

 	return;
 }
