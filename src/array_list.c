#include <stdlib.h>
#include <string.h>

#include "array_list.h"

SPArrayList* spArrayListCreate(int elemSize, int maxSize)
{
	SPArrayList* arr = NULL;
	if (maxSize <= 0 || elemSize <= 0) {
		return NULL;
	}
	arr = calloc(1, sizeof(SPArrayList));
	if (arr == NULL) {
		return NULL;
	}
	arr->elements = calloc(maxSize, sizeof(void *));
	if (arr->elements == NULL) {
		free(arr);
		return NULL;
	}
	arr->actualSize = 0;
	arr->elemSize = elemSize;
	arr->maxSize = maxSize;
	return arr;
}

SPArrayList* spArrayListCopy(SPArrayList* src)
{
	if(src == NULL) {
		return NULL;
	}
	SPArrayList* arr = spArrayListCreate(src->elemSize, src->maxSize);
	if(!arr) return NULL;
	arr->actualSize = src->actualSize;
	for(int i = 0; i < src->actualSize; i++){
		if(!src->elements[i]){
			arr->elements[i] = NULL;
			continue;
		}
		void *item = malloc(src->elemSize);
		memcpy(item, src->elements[i], src->elemSize);
		arr->elements[i] = item;
	}
	return arr;
}

void spArrayListDestroy(SPArrayList* src) {
	if (src != NULL) {
		for(int i = 0; i < spArrayListSize(src); i++){
			free(src->elements[i]);
		}
		free(src->elements);
		free(src);
	}
}

SP_ARRAY_LIST_MESSAGE spArrayListClear(SPArrayList* src) {
	if (src == NULL) {
		return SP_ARRAY_LIST_INVALID_ARGUMENT;
	}
	for(int i = 0; i < spArrayListSize(src); i++){
		free(src->elements[i]);
	}
	src->actualSize = 0;
	return SP_ARRAY_LIST_SUCCESS;
}

SP_ARRAY_LIST_MESSAGE spArrayListAddAt(SPArrayList* src, void *elem, int index) {
	if (src == NULL || index < 0 || index > src->actualSize) {
		return SP_ARRAY_LIST_INVALID_ARGUMENT;
	}
	if (src->actualSize == src->maxSize) {
		return SP_ARRAY_LIST_FULL;
	}
	if (index != src->actualSize) {
		memmove(src->elements+index+1, src->elements+index, (src->actualSize-index)*sizeof(void *));
	}
	src->elements[index] = elem;
	src->actualSize++;
	return SP_ARRAY_LIST_SUCCESS;
}

SP_ARRAY_LIST_MESSAGE spArrayListAddFirst(SPArrayList* src, void *elem){
	return spArrayListAddAt(src, elem, 0);
}

SP_ARRAY_LIST_MESSAGE spArrayListAddLast(SPArrayList* src, void *elem){
	if (src == NULL) {
		return SP_ARRAY_LIST_INVALID_ARGUMENT;
	}
	return spArrayListAddAt(src, elem, src->actualSize);
}

SP_ARRAY_LIST_MESSAGE spArrayListRemoveAt(SPArrayList* src, int index){
	if (src == NULL || index < 0 || index >= src->actualSize) {
		return SP_ARRAY_LIST_INVALID_ARGUMENT;
	}
	if (src->actualSize == 0) {
		return SP_ARRAY_LIST_EMPTY;
	}
	if (index != src->actualSize-1) {
		memmove(src->elements+index, src->elements+index+1, (src->actualSize-index-1)*sizeof(void *));
	}
	src->actualSize--;
	return SP_ARRAY_LIST_SUCCESS;
}

SP_ARRAY_LIST_MESSAGE spArrayListRemoveFirst(SPArrayList* src) {
	if (src == NULL) {
		return SP_ARRAY_LIST_INVALID_ARGUMENT;
	} else if(src->actualSize == 0) {
		return SP_ARRAY_LIST_EMPTY;
	}
	return spArrayListRemoveAt(src, 0);
}

SP_ARRAY_LIST_MESSAGE spArrayListRemoveLast(SPArrayList* src) {
	if (src == NULL) {
		return SP_ARRAY_LIST_INVALID_ARGUMENT;
	} else if(src->actualSize == 0) {
		return SP_ARRAY_LIST_EMPTY;
	}
	return spArrayListRemoveAt(src, src->actualSize - 1);
}

SP_ARRAY_LIST_MESSAGE spArrayListRemoveItem(SPArrayList *src, void *item){
	if(!src || !item) return SP_ARRAY_LIST_INVALID_ARGUMENT;
	int arrayIndex = 0;
	for(int i = 0; i < src->actualSize; i++){
		if(item == src->elements[i]) arrayIndex = i;
	}
	return spArrayListRemoveAt(src, arrayIndex);
}

void *spArrayListGetAt(SPArrayList* src, int index) {
	if (src == NULL || index >= src->actualSize || index < 0) {
		return NULL;
	}
	return src->elements[index];
}

void *spArrayListGetFirst(SPArrayList* src) {
	return spArrayListGetAt(src, 0);
}

void *spArrayListGetLast(SPArrayList* src) {
	if (src == NULL) {
		return NULL;
	}
	return spArrayListGetAt(src, src->actualSize-1);
}

int spArrayListMaxCapacity(SPArrayList* src) {
	if (src == NULL) {
		return -1;
	}
	return src->maxSize;
}

int spArrayListSize(SPArrayList* src) {
	if (src == NULL) {
		return -1;
	}
	return src->actualSize;
}

bool spArrayListIsFull(SPArrayList* src) {
	if (src == NULL || src->actualSize < src->maxSize) {
		return false;
	}
	return true;
}

bool spArrayListIsEmpty(SPArrayList* src) {
	if (src == NULL || src->actualSize > 0) {
		return false;
	}
	return true;
}
