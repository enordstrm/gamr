#include <stdlib.h>
#include <stdio.h>
typedef struct {
	int max_size;
	int size;
	void **items;
} vector_t;

void
vector_initialize(vector_t *vec)
{
	vec->max_size = 20;
	vec->size = 0;
	void **items = malloc(sizeof(void *) * vec->max_size);
	vec->items = items;
}

vector_t*
vector_resize(vector_t *vec, float size)
{
	void **items = realloc(vec->items, sizeof(void *) * (int)size);
	vec->items = items;
	return vec;
};

void
vector_push(vector_t *vec, void *item)
{
	printf("omgealul?");
	if (vec)
	{
		printf("omgealul?");
		if (vec->size == vec->max_size)
		{
			vector_resize(vec, vec->max_size * 1.6f);
			vec->items[vec->size++] = item;
			printf("resizing and pushing things to the vec\n");
		}
		else {
			printf("pushing things to the vec\n");
			vec->items[vec->size++] = item;
		}
	}
}

void*
vector_get(vector_t *vec, int index)
{
	void *read_data = NULL;
	if(vec)
	{
		if ((index >= 0) && (index < vec->size))
		{
			read_data = vec->items[index];
		}
	}

	return read_data;
}


int main()
{

	vector_t * vec;
	int n = 20;
	vector_initialize(vec);
	vector_push(vec, &n);


	//printf("%d\n", *(int*)vector_get(vec, 0));
	return 0;
}
