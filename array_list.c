#include <stdlib.h>
#include <string.h>

#include "array_list.h"

/**
 *  Creates an empty array list with the specified maximum capacity.
 *  @param maxSize - the maximum capacity of the target array list.
 *  @return
 *  NULL, if an allocation error occurred or maxSize  <= 0 or elemSize <= 0.
 *  An instant of an array list otherwise.
 */
SPArrayList* spArrayListCreate(int elemSize, int maxSize)
{
    SPArrayList* arr = NULL;
    if (maxSize <= 0 || elemSize <= 0) {
        return NULL;
    }
    arr = malloc(sizeof(SPArrayList));
    if (arr == NULL) {
        return NULL;
    }
    arr->elements = malloc(maxSize*elemSize);
    if (arr->elements == NULL) {
        free(arr);
        return NULL;
    }
    arr->actualSize = 0;
    arr->maxSize = maxSize;
    return arr;
}

/**
 *	Creates an exact copy of the src array list. Elements in the new copy will
 *	be in the same order as they appeared in the source list.
 *	@param src - the source array list.
 *	@return
 *	NULL if either an allocation error occurs or src == NULL.
 *	A new copy of the source array list, otherwise.
 */
SPArrayList* spArrayListCopy(SPArrayList* src)
{
    if(src == NULL) {
        return NULL;
    }
    SPArrayList* arr = spArrayListCreate(src->elemSize, src->maxSize);
    arr->actualSize = src->actualSize;
    memcpy(arr->elements, src->elements, src->actualSize*src->elemSize);
    return arr;
}

/**
 * Frees all memory resources associated with the source array list. If the
 * source array is NULL, then the function does nothing.
 * @param src - the source array list
 */
void spArrayListDestroy(SPArrayList* src) {
    if (src != NULL) {
        free(src->elements);
        free(src);
    }
}

/**
 * Clears all elements from the source array list. After invoking this function,
 * the size of the source list will be reduced to zero and maximum capacity is
 * not affected.
 * @param src - the source array list
 * @return
 * SP_ARRAY_LIST_INVALID_ARGUMENT if src == NULL
 * SP_ARRAY_LIST_SUCCESS otherwise
 */
SP_ARRAY_LIST_MESSAGE spArrayListClear(SPArrayList* src) {
    if (src == NULL) {
        return SP_ARRAY_LIST_INVALID_ARGUMENT;
    }
    src->actualSize = 0;
    return SP_ARRAY_LIST_SUCCESS;
}

/**
 * Inserts element at a specified index. The elements residing at and after the
 * specified index will be shifted to make place for the new element. If the
 * array list reached its maximum capacity and error message is returned and
 * the source list is not affected
 * @param src   - the source array list
 * @param elem  - the new element to be inserted
 * @param index - the index where the new element will be placed. The index is
 *                0-based.
 * @return
 * SP_ARRAY_LIST_INVALID_ARGUMENT - if src == NULL or the index is out of bound
 * SP_ARRAY_LIST_FULL - if the source array list reached its maximum capacity
 * SP_ARRAY_LIST_SUCCESS - otherwise
 */
SP_ARRAY_LIST_MESSAGE spArrayListAddAt(SPArrayList* src, void *elem, int index) {
    if (src == NULL || index < 0 || index > src->actualSize) {
        return SP_ARRAY_LIST_INVALID_ARGUMENT;
    }
    if (src->actualSize == src->maxSize) {
        return SP_ARRAY_LIST_FULL;
    }
	char *address = (char *)src->elements + index*src->elemSize;
    if (index != src->actualSize) {
        memmove(address+1, address, (src->actualSize-index)*src->elemSize);
    }
	memcpy(address, elem, src->elemSize);
    src->actualSize++;
    return SP_ARRAY_LIST_SUCCESS;
}

/**
 * Inserts element at a the beginning of the source element. The elements
 * will be shifted to make place for the new element. If the
 * array list reached its maximum capacity and error message is returned and
 * the source list is not affected
 * @param src   - the source array list
 * @param elem  - the new element to be inserted
 * @return
 * SP_ARRAY_LIST_INVALID_ARGUMENT - if src == NULL or the index is out of bound
 * SP_ARRAY_LIST_FULL - if the source array list reached its maximum capacity
 * SP_ARRAY_LIST_SUCCESS - otherwise
 */
SP_ARRAY_LIST_MESSAGE spArrayListAddFirst(SPArrayList* src, void *elem){
    return spArrayListAddAt(src, elem, 0);
}

/**
 * Inserts element at a the end of the source element. If the array list
 * reached its maximum capacity and error message is returned and the source
 * list is not affected.
 * @param src   - the source array list
 * @param elem  - the new element to be inserted
 * @return
 * SP_ARRAY_LIST_INVALID_ARGUMENT - if src == NULL or the index is out of bound
 * SP_ARRAY_LIST_FULL - if the source array list reached its maximum capacity
 * SP_ARRAY_LIST_SUCCESS - otherwise
 */
SP_ARRAY_LIST_MESSAGE spArrayListAddLast(SPArrayList* src, void *elem){
    if (src == NULL) {
        return SP_ARRAY_LIST_INVALID_ARGUMENT;
    }
    return spArrayListAddAt(src, elem, src->actualSize);
}

/**
 * Removes an element from a specified index. The elements residing after the
 * specified index will be shifted to make to keep the list continuous. If the
 * array list is empty then an error message is returned and the source list
 * is not affected
 * @param src   - The source array list
 * @param index - The index from where the element will be removed.
 *                The index is 0-based.
 * @return
 * SP_ARRAY_LIST_INVALID_ARGUMENT - if src == NULL or the index is out of bound
 * SP_ARRAY_LIST_EMPTY - if the source array list is empty
 * SP_ARRAY_LIST_SUCCESS - otherwise
 */
SP_ARRAY_LIST_MESSAGE spArrayListRemoveAt(SPArrayList* src, int index){
    if (src == NULL || index < 0 || index >= src->actualSize) {
        return SP_ARRAY_LIST_INVALID_ARGUMENT;
    }
    if (src->actualSize == 0) {
        return SP_ARRAY_LIST_EMPTY;
    }
    if (index != src->actualSize-1) {
		char *address = (char *)src->elements + index*src->elemSize;
        memmove(address, address+1, (src->actualSize-index-1)*src->elemSize);
    }
    src->actualSize--;
    return SP_ARRAY_LIST_SUCCESS;
}

/**
 * Removes an element from a the beginning of the list.
 * The elements will be shifted to make to keep the list continuous. If the
 * array list is empty then an error message is returned and the source list
 * is not affected
 * @param src   - The source array list
 * @param elem  - The new element to be inserted
 * @return
 * SP_ARRAY_LIST_INVALID_ARGUMENT - if src == NULL
 * SP_ARRAY_LIST_EMPTY - if the source array list is empty
 * SP_ARRAY_LIST_SUCCESS - otherwise
 */
SP_ARRAY_LIST_MESSAGE spArrayListRemoveFirst(SPArrayList* src) {
    if (src == NULL) {
        return SP_ARRAY_LIST_INVALID_ARGUMENT;
    } else if(src->actualSize == 0) {
        return SP_ARRAY_LIST_EMPTY;
    }
    return spArrayListRemoveAt(src, 0);
}

/**
 * Removes an element from a the end of the list.
 * The elements will be shifted to make to keep the list continuous. If the
 * array list is empty then an error message is returned and the source list
 * is not affected
 * @param src   - The source array list
 * @param elem  - The new element to be inserted
 * @return
 * SP_ARRAY_LIST_INVALID_ARGUMENT - if src == NULL
 * SP_ARRAY_LIST_EMPTY - if the source array list is empty
 * SP_ARRAY_LIST_SUCCESS - otherwise.
 */
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
	int arrayIndex = ((char *)item - (char *)src->elements)/src->elemSize;
	return spArrayListRemoveAt(src, arrayIndex);
} 

/**
 * Returns the element at the specified index. The function is called
 * with the assertion that all arguments are valid. If any of the arguments is
 * invalid then an undefined value is returned.
 * @param src - the source array list
 * @param index - the specified index, the index is 0-based.
 * @return
 * Undefined value if either src == NULL or index out of bound.
 * Otherwise, the element at the specified index is returned.
 */
void *spArrayListGetAt(SPArrayList* src, int index) {
    if (src == NULL || index >= src->actualSize || index < 0) {
        return NULL;
    }
	char *address = (char *)src->elements + index*src->elemSize;
    return address;
}

/**
 * Returns the element at the beginning of the list. The function is called
 * with the assertion that all arguments are valid. If any of the arguments is
 * invalid then an undefined value is returned.
 * @param src - the source array list
 * @return
 * Undefined value if either src == NULL or the list is empty
 * Otherwise, the element at the beginning of the list is returned.
 */
void *spArrayListGetFirst(SPArrayList* src) {
    return spArrayListGetAt(src, 0);
}

/**
 * Returns the element at the end of the list. The function is called
 * with the assertion that all arguments are valid. If any of the arguments is
 * invalid then an undefined value is returned.
 * @param src - the source array list
 * @return
 * Undefined value if either src == NULL or the list is empty
 * Otherwise, the element at the end of the list is returned.
 */
void *spArrayListGetLast(SPArrayList* src) {
    if (src == NULL) {
        return NULL;
    }
    return spArrayListGetAt(src, src->actualSize-1);
}

/**
 * Returns the maximum capacity of the list. The function is called
 * with the assertion that all arguments are valid. If any of the arguments is
 * invalid then an undefined value is returned.
 * @param src - the source array list
 * @return
 * Undefined value if either src == NULL
 * Otherwise, the maximum capacity of the list is returned.
 */
int spArrayListMaxCapacity(SPArrayList* src) {
    if (src == NULL) {
        return -1;
    }
    return src->maxSize;
}

/**
 * Returns the number of elements in the list. The function is called
 * with the assertion that all arguments are valid. If any of the arguments is
 * invalid then an undefined value is returned.
 * @param src - the source array list
 * @return
 * Undefined value if either src == NULL
 * Otherwise, the number of the elements in the list is returned.
 */
int spArrayListSize(SPArrayList* src) {
    if (src == NULL) {
        return -1;
    }
    return src->actualSize;
}

/**
 * Returns true if the list is full, that is the number of elements in the list
 * equals its maximum capacity.
 * @param src - the source array list
 * @return
 * false if either src == NULL or the number of elements in the list is less
 * than its maximum capacity.
 * Otherwise, true is returned.
 */
bool spArrayListIsFull(SPArrayList* src) {
    if (src == NULL || src->actualSize < src->maxSize) {
        return false;
    }
    return true;
}

/**
 * Returns true if the list is empty, that is the number of elements in the list
 * equals to zero.
 * @param src - the source array list
 * @return
 * false if either src == NULL or the number of elements in the list is not zero.
 * Otherwise, true is returned.
 */
bool spArrayListIsEmpty(SPArrayList* src) {
    if (src == NULL || src->actualSize > 0) {
        return false;
    }
    return true;    
}
