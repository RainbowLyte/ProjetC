#include <list.h>
#include <assert.h>

struct list {
	void* data;
	int x, y;
	s_type type;
	struct list* next;
};

struct list* list_new()
{
	return NULL;
}

int list_get_x(struct list* l) {
	assert(l);
	return(l->x);
}

int list_get_y(struct list* l) {
	assert(l);
	return(l->y);
}

void list_inc_x(struct list* l) {
	assert(l);
	l->x++;
}

void list_dec_x(struct list* l) {
	assert(l);
	l->x--;
}

void list_inc_y(struct list* l) {
	assert(l);
	l->y++;
}

void list_dec_y(struct list* l) {
	assert(l);
	l->y--;
}

struct list* list_get_next(struct list* l) {
	assert(l);
	return l->next;
}

void* list_get_data(struct list* l) {
	assert(l);
	return l->data;
}

struct list* list_get_tail(struct list* l)
{
	if(l != NULL)
		while(l->next != NULL)
			l = l->next;
	return l;
}

struct list* list_insert_back(struct list* l, int x, int y, s_type type, void* data)
{
	struct list* l_or = l;
	struct list* l_new = malloc( sizeof(*l_new) );
		l_new->data = data;
		l_new->x = x;
		l_new->y = y;
		l_new->type = type;
		l_new->next = NULL;

	if( l != NULL ){
		l = list_get_tail(l);
		l->next = l_new;
	} else {
		l_or = l_new;
	}

	return l_or;
}


struct list* list_cut_head(struct list* l)
{
	assert(l);

	struct list* l_h = l;

	if(l != NULL && l->next != NULL)
		l = l->next;
	else
		l = NULL;
	
	free(l_h);

	return l;
}

struct list* list_find(struct list* l, int x, int y)
{
	while(l != NULL)
	{
		if(l->x == x && l->y == y)
			break;
		else
			l = l->next;
	}
	return l;
}

struct list* list_remove(struct list* l, int x, int y)
{
	struct list* l_or = l;
	struct list* l_new = list_new();

	while(l != NULL)
	{
		if(l->x != x || l->y != y) {
			l_new = list_insert_back(l_new, l->x, l->y, l->type, l->data);
		}
		l = l->next;
	}

	l_or = list_delete(l_or);

	return l_new;
}

struct list* list_remove2(struct list* l_head, struct list* l) {
	assert(l_head);
	struct list* l_return = NULL;
	struct list* lb = l_head;

	if(l->data != NULL)
		free(l->data);

	if(l == l_head) {
		l_return = l->next;
		free(l);
	}
	else {
		l_return = l_head;
		while(lb->next != l) {
			lb = lb->next;
		}
		lb->next = l->next;
		free(l);
	}
	return(l_return);
}

struct list* list_delete(struct list* l)
{
	while(l != NULL)
		l = list_cut_head(l);
	return l;
}
