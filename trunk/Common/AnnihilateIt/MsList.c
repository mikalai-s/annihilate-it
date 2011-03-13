#include "MsList.h"
#include "stdlib.h"
#include "stdio.h"

MsListItem* ms_listitem_create(generic value)
{
	MsListItem *item;
	
	item = (MsListItem*)malloc(sizeof(MsListItem));
	item->value = value;
	item->next = 0;
	return item;
}

MsList* ms_list_create(generic *values, int count)
{
	int i;
	MsListItem *prev;
	MsList *list = ms_list_create_empty();

	prev = 0;
	for(i = 0; i < count; i ++)
	{
		MsListItem *item = ms_listitem_create(values[i]);
		if(prev == 0)
			list->first = item;			
		else
			prev->next = item;

		prev = item;
	}

	list->count = count;

	return list;
}

MsList* ms_list_create_empty()
{
	MsList *list = (MsList*)malloc(sizeof(MsList));
	list->count = 0;
	list->first = 0;
	return list;
}

void ms_listitem_free(MsListItem **item)
{
	MsListItem *i = *item;
	free(i);
	*item = 0;
}

void ms_list_free(MsList **list)
{
	MsList *l = *list;
	
	while(l->count > 0)
		ms_list_remove_item(l, l->first);
	free(l);
}

int ms_list_contains(MsList *list, generic value)
{
	MsListItem *i = list->first;
	while(i != 0)
	{
		if(i->value.i == value.i)
			return 1;
		i = i->next;
	}
	return 0;
}

MsListItem* ms_list_add_item(MsList *list, generic value)
{
	MsListItem *i;
	MsListItem *item = ms_listitem_create(value);
	if(list->first == 0)
	{
		list->first = item;
	}
	else
	{
		i = list->first;
		while(i->next != 0)
			i = i->next;
		i->next = item;
	}
	list->count ++;
	return item;
}

void ms_list_remove_item(MsList *list, MsListItem *item)
{
	MsListItem *prev = 0;
	MsListItem *i = list->first;

	// if item is the first element:
	if(list->first == item)
	{
		list->first = item->next;
	}
	else
	{
		// itereate through items to find previous element
		while(i != item)
		{
			prev = i;
			i = i->next;
		}

		prev->next = item->next;
	}	

	ms_listitem_free(&item);
	list->count --;
}

MsListItem* ms_list_find_item(MsList *list, generic value)
{
	MsListItem *i = list->first;	
	while(i != 0)
	{
		if(i->value.i == value.i)
			return i;
		i = i->next;
	}
	return 0;
}

void _display(MsList *list)
{
	MsListItem *i = list->first;
	while(i != 0)
	{
		printf("%d ", i->value.i);
		i = i->next;
	}
	printf("\r\n");
}

void ms_list_unit_test()
{
	MsList *list;
	MsListItem *item;
	generic ints[1];
	ints[0] = _itog(0);	

	list = ms_list_create(ints, 1);
	_display(list);

	ms_list_add_item(list, _itog(10));
	_display(list);

	item = ms_list_find_item(list, _itog(0));
	if(item != 0)
		ms_list_remove_item(list, item);
	_display(list);

	ms_list_free(&list);
}