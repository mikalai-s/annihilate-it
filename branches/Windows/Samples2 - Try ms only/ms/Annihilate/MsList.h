/*
	MsList
*/

#ifndef _MsList_H
#define _MsList_H


#include "MsCommon.h"


typedef struct MsList
{
	struct MsListItem *first;
	int count;
} MsList;



typedef struct MsListItem
{
	generic value;
	struct MsListItem *next;
} MsListItem;



MsList*			ms_list_create_empty ();

MsList*			ms_list_create (generic *values, int count);

// it doesn't free MsListItem's values
void			ms_list_free (MsList **list);

// returns 1 if list contains specific value
int				ms_list_contains (MsList *list, generic value);

MsListItem*		ms_list_add_item (MsList *list, generic value);

void			ms_list_remove_item (MsList *list, MsListItem *item);

MsListItem*		ms_list_find_item (MsList *list, generic value);

void			ms_list_unit_test ();



#endif