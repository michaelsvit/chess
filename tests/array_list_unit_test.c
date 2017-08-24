#include "unit_test_util.h"
#include "array_list.h"
#include <stdbool.h>
#include <stdlib.h>

#define SIZE 10

static void freeResources(SPArrayList *al){
	for(int i = 0; i < spArrayListSize(al); i++){
		free((int *)spArrayListGetAt(al, i));
	}
}

static bool spArrayListCreateTest(){
	SPArrayList *al = spArrayListCreate(sizeof(int *), 0);
	ASSERT_FALSE(al);
	al = spArrayListCreate(sizeof(int *), -5);
	ASSERT_FALSE(al);
	al = spArrayListCreate(sizeof(int *), SIZE);
	ASSERT_TRUE(al);
	ASSERT_TRUE(spArrayListSize(al) == 0);
	ASSERT_TRUE(spArrayListMaxCapacity(al) == SIZE);
	spArrayListDestroy(al);
	return true;
}

static bool spArrayListCopyTest(){
	ASSERT_FALSE(spArrayListCopy(NULL));

	SPArrayList *al = spArrayListCreate(sizeof(int *), SIZE);
	SPArrayList *alc = spArrayListCopy(al);
	ASSERT_TRUE(alc && spArrayListSize(alc) == 0 && spArrayListMaxCapacity(alc) == SIZE);
	spArrayListDestroy(alc);

	int *item1 = malloc(sizeof(int));
	*item1 = 7;
	int *item2 = malloc(sizeof(int));
	*item2 = 5;
	spArrayListAddFirst(al, item1);
	spArrayListAddLast(al, item2);
	ASSERT_TRUE(alc = spArrayListCopy(al));
	ASSERT_TRUE(spArrayListSize(alc) == 2);
	ASSERT_TRUE(spArrayListMaxCapacity(alc) == SIZE);
	ASSERT_TRUE(*((int *)spArrayListGetFirst(alc)) == 7);
	ASSERT_TRUE(*((int *)spArrayListGetLast(alc)) == 5);

	spArrayListDestroy(al);
	spArrayListDestroy(alc);
	return true;
}

static bool spArrayListClearTest(){
	ASSERT_TRUE(spArrayListClear(NULL) == SP_ARRAY_LIST_INVALID_ARGUMENT);

	SPArrayList *al = spArrayListCreate(sizeof(int *), SIZE);
	int *item1 = malloc(sizeof(int));
	*item1 = 1;
	spArrayListAddFirst(al, item1);
	free(item1);
	ASSERT_TRUE(spArrayListClear(al) == SP_ARRAY_LIST_SUCCESS);
	ASSERT_TRUE(spArrayListSize(al) == 0);
	ASSERT_TRUE(spArrayListMaxCapacity(al) == SIZE);

	for(int i = 0; i < SIZE; i++){
		int *item = malloc(sizeof(int));
		*item = i;
		spArrayListAddFirst(al, item);
	}
	freeResources(al);
	ASSERT_TRUE(spArrayListClear(al) == SP_ARRAY_LIST_SUCCESS);
	ASSERT_TRUE(spArrayListSize(al) == 0);
	ASSERT_TRUE(spArrayListMaxCapacity(al) == SIZE);

	spArrayListDestroy(al);
	return true;
}

static bool spArrayListAddAtTest(){
	ASSERT_TRUE(spArrayListAddAt(NULL, 0, 0) == SP_ARRAY_LIST_INVALID_ARGUMENT);

	SPArrayList *al = spArrayListCreate(sizeof(int *), SIZE);
	int *item1 = malloc(sizeof(int));
	*item1 = 1;
	ASSERT_TRUE(spArrayListAddAt(al, item1, -1) == SP_ARRAY_LIST_INVALID_ARGUMENT);
	ASSERT_TRUE(spArrayListAddAt(al, item1, 1) == SP_ARRAY_LIST_INVALID_ARGUMENT);
	ASSERT_TRUE(spArrayListAddAt(al, item1, SIZE+1) == SP_ARRAY_LIST_INVALID_ARGUMENT);

	int *item2 = malloc(sizeof(int));
	*item2 = 2;
	int *item3 = malloc(sizeof(int));
	*item3 = 3;
	ASSERT_TRUE(spArrayListAddAt(al, item2, 0) == SP_ARRAY_LIST_SUCCESS);
	ASSERT_TRUE(spArrayListAddAt(al, item3, 1) == SP_ARRAY_LIST_SUCCESS);
	ASSERT_TRUE(spArrayListAddAt(al, item1, 0) == SP_ARRAY_LIST_SUCCESS);
	ASSERT_TRUE(spArrayListSize(al) == 3);
	for(int i = 0; i < 3; i++){
		ASSERT_TRUE(*((int *)spArrayListGetAt(al, i)) == i+1);
	}
	for(int i = 0; i < SIZE-3; i++){
		int *item = malloc(sizeof(int));
		*item = 42;
		spArrayListAddLast(al, item);
	}
	ASSERT_TRUE(spArrayListAddAt(al, item1, 5) == SP_ARRAY_LIST_FULL);

	spArrayListDestroy(al);
	return true;
}

static bool spArrayListAddFirstTest(){
	ASSERT_TRUE(spArrayListAddFirst(NULL, 0) == SP_ARRAY_LIST_INVALID_ARGUMENT);

	SPArrayList *al = spArrayListCreate(sizeof(int *), SIZE);
	for(int i = 0; i < SIZE; i++){
		int *item = malloc(sizeof(int));
		*item = i;
		ASSERT_TRUE(spArrayListAddFirst(al, item) == SP_ARRAY_LIST_SUCCESS);
	}
	ASSERT_TRUE(spArrayListAddFirst(al, NULL) == SP_ARRAY_LIST_FULL);
	for(int i = 0; i < SIZE; i++){
		ASSERT_TRUE(*((int *)spArrayListGetAt(al, SIZE-i-1)) == i);
	}

	spArrayListDestroy(al);
	return true;
}

static bool spArrayListAddLastTest(){
	ASSERT_TRUE(spArrayListAddLast(NULL, 0) == SP_ARRAY_LIST_INVALID_ARGUMENT);

	SPArrayList *al = spArrayListCreate(sizeof(int *), SIZE);
	for(int i = 0; i < SIZE; i++){
		int *item = malloc(sizeof(int));
		*item = i;
		ASSERT_TRUE(spArrayListAddLast(al, item) == SP_ARRAY_LIST_SUCCESS);
	}
	ASSERT_TRUE(spArrayListAddLast(al, NULL) == SP_ARRAY_LIST_FULL);
	for(int i = 0; i < SIZE; i++){
		ASSERT_TRUE(*((int *)spArrayListGetAt(al, i)) == i);
	}

	spArrayListDestroy(al);
	return true;
}

static bool spArrayListRemoveAtTest(){
	ASSERT_TRUE(spArrayListRemoveAt(NULL, 0) == SP_ARRAY_LIST_INVALID_ARGUMENT);

	SPArrayList *al = spArrayListCreate(sizeof(int *), SIZE);
	ASSERT_TRUE(spArrayListRemoveAt(al, -1) == SP_ARRAY_LIST_INVALID_ARGUMENT);
	ASSERT_TRUE(spArrayListRemoveAt(al, 1) == SP_ARRAY_LIST_INVALID_ARGUMENT);
	ASSERT_TRUE(spArrayListRemoveAt(al, SIZE+1) == SP_ARRAY_LIST_INVALID_ARGUMENT);

	int *item1 = malloc(sizeof(int));
	*item1 = 5;
	spArrayListAddFirst(al, item1);
	free(item1);
	ASSERT_TRUE(spArrayListRemoveAt(al, 0) == SP_ARRAY_LIST_SUCCESS);
	ASSERT_TRUE(spArrayListSize(al) == 0 && spArrayListMaxCapacity(al) == SIZE);

	for(int i = 0; i < SIZE; i++){
		int *item = malloc(sizeof(int));
		*item = 4;
		spArrayListAddFirst(al, item);
	}
	freeResources(al);
	for(int i = 0; i < SIZE; i++){
		ASSERT_TRUE(spArrayListRemoveAt(al, spArrayListSize(al)-1) == SP_ARRAY_LIST_SUCCESS);
	}
	ASSERT_TRUE(spArrayListSize(al) == 0);
	for(int i = 0; i < SIZE; i++){
		int *item = malloc(sizeof(int));
		*item = i;
		spArrayListAddLast(al, item);
	}
	for(int i = 0; i < 5; i++){
		free((int *)spArrayListGetAt(al, 0));
		ASSERT_TRUE(spArrayListRemoveAt(al, 0) == SP_ARRAY_LIST_SUCCESS);
	}
	ASSERT_TRUE(spArrayListSize(al) == SIZE-5);
	for(int i = 5; i < SIZE; i++){
		ASSERT_TRUE(*((int *)spArrayListGetAt(al, i-5)) == i);
	}

	spArrayListDestroy(al);
	return true;
}

static bool spArrayListRemoveFirstTest(){
	ASSERT_TRUE(spArrayListRemoveFirst(NULL) == SP_ARRAY_LIST_INVALID_ARGUMENT);

	SPArrayList *al = spArrayListCreate(sizeof(int *), SIZE);
	ASSERT_TRUE(spArrayListRemoveFirst(al) == SP_ARRAY_LIST_EMPTY);

	for(int i = 0; i < SIZE; i++){
		int *item = malloc(sizeof(int));
		*item = i;
		spArrayListAddLast(al, item);
	}
	for(int i = 0; i < 5; i++){
		free((int *)spArrayListGetFirst(al));
		ASSERT_TRUE(spArrayListRemoveFirst(al) == SP_ARRAY_LIST_SUCCESS);
	}
	ASSERT_TRUE(spArrayListSize(al) == SIZE-5);
	for(int i = 5; i < SIZE; i++){
		ASSERT_TRUE(*((int *)spArrayListGetAt(al, i-5)) == i);
	}

	spArrayListDestroy(al);
	return true;
}

static bool spArrayListRemoveLastTest(){
	ASSERT_TRUE(spArrayListRemoveLast(NULL) == SP_ARRAY_LIST_INVALID_ARGUMENT);

	SPArrayList *al = spArrayListCreate(sizeof(int *), SIZE);
	ASSERT_TRUE(spArrayListRemoveLast(al) == SP_ARRAY_LIST_EMPTY);

	for(int i = 0; i < SIZE; i++){
		int *item = malloc(sizeof(int));
		*item = i;
		spArrayListAddLast(al, item);
	}
	for(int i = 0; i < 5; i++){
		free((int *)spArrayListGetLast(al));
		ASSERT_TRUE(spArrayListRemoveLast(al) == SP_ARRAY_LIST_SUCCESS);
	}
	ASSERT_TRUE(spArrayListSize(al) == SIZE-5);
	for(int i = 0; i < SIZE-5; i++){
		ASSERT_TRUE(*((int *)spArrayListGetAt(al, i)) == i);
	}

	spArrayListDestroy(al);
	return true;
}

static bool spArrayListRemoveItemTest(){
	ASSERT_TRUE(spArrayListRemoveItem(NULL, NULL) == SP_ARRAY_LIST_INVALID_ARGUMENT);

	SPArrayList *al = spArrayListCreate(sizeof(int *), SIZE);

	int item1 = 5;
	spArrayListAddFirst(al, &item1);
	int *item2 = NULL;
	for(int i = 0; i < SIZE-1; i++){
		int *item = malloc(sizeof(int));
		*item = i;
		if(i == 4) item2 = item;
		spArrayListAddFirst(al, item);
	}
	ASSERT_TRUE(spArrayListRemoveItem(al, &item1) == SP_ARRAY_LIST_SUCCESS);
	ASSERT_TRUE(spArrayListRemoveItem(al, item2) == SP_ARRAY_LIST_SUCCESS);

	free(item2);
	spArrayListDestroy(al);
	return true;
}

static bool spArrayListGetAtTest(){
	SPArrayList *al = spArrayListCreate(sizeof(int *), SIZE);
	int *item1 = malloc(sizeof(int));
	*item1 = 0;
	spArrayListAddFirst(al, item1);
	ASSERT_TRUE(*((int *)spArrayListGetAt(al, 0)) == 0);

	for(int i = 1; i < SIZE; i++){
		int *item = malloc(sizeof(int));
		*item = i;
		spArrayListAddLast(al, item);
	}
	for(int i = 0; i < SIZE; i++){
		ASSERT_TRUE(*((int *)spArrayListGetAt(al, i)) == i);
	}

	spArrayListDestroy(al);
	return true;
}

static bool spArrayListGetFirstTest(){
	SPArrayList *al = spArrayListCreate(sizeof(int *), SIZE);
	spArrayListAddFirst(al, NULL);
	ASSERT_TRUE(spArrayListGetFirst(al) == NULL);
	spArrayListClear(al);

	for(int i = 1; i < SIZE; i++){
		int *item = malloc(sizeof(int));
		*item = i;
		spArrayListAddLast(al, item);
	}
	ASSERT_TRUE(*((int *)spArrayListGetFirst(al)) == 1);

	spArrayListDestroy(al);
	return true;
}

static bool spArrayListGetLastTest(){
	SPArrayList *al = spArrayListCreate(sizeof(int *), SIZE);
	spArrayListAddLast(al, NULL);
	ASSERT_TRUE(spArrayListGetLast(al) == NULL);

	for(int i = 1; i < SIZE; i++){
		int *item = malloc(sizeof(int));
		*item = i;
		spArrayListAddLast(al, item);
	}
	ASSERT_TRUE(*((int *)spArrayListGetLast(al)) == SIZE-1);

	spArrayListDestroy(al);
	return true;
}

static bool spArrayListMaxCapacityTest(){
	SPArrayList *al = spArrayListCreate(sizeof(int *), SIZE);
	ASSERT_TRUE(spArrayListMaxCapacity(al) == SIZE);
	spArrayListDestroy(al);
	return true;
}

static bool spArrayListSizeTest(){
	SPArrayList *al = spArrayListCreate(sizeof(int *), SIZE);
	ASSERT_TRUE(spArrayListSize(al) == 0);
	spArrayListAddFirst(al, NULL);
	ASSERT_TRUE(spArrayListSize(al) == 1);
	for(int i = 0; i < SIZE-1; i++){
		int *item = malloc(sizeof(int));
		*item = i;
		spArrayListAddLast(al, item);
	}
	ASSERT_TRUE(spArrayListSize(al) == SIZE);
	spArrayListDestroy(al);
	return true;
}

static bool spArrayListIsFullTest(){
	SPArrayList *al = spArrayListCreate(sizeof(int *), SIZE);
	ASSERT_FALSE(spArrayListIsFull(al));
	spArrayListAddFirst(al, 0);
	ASSERT_FALSE(spArrayListIsFull(al));
	for(int i = 0; i < SIZE-1; i++){
		int *item = malloc(sizeof(int));
		*item = i;
		spArrayListAddLast(al, item);
	}
	ASSERT_TRUE(spArrayListIsFull(al));
	spArrayListDestroy(al);
	return true;
}

static bool spArrayListIsEmptyTest(){
	SPArrayList *al = spArrayListCreate(sizeof(int *), SIZE);
	ASSERT_TRUE(spArrayListIsEmpty(al));
	spArrayListAddFirst(al, 0);
	ASSERT_FALSE(spArrayListIsEmpty(al));
	for(int i = 0; i < SIZE-1; i++){
		int *item = malloc(sizeof(int));
		*item = i;
		spArrayListAddLast(al, item);
	}
	ASSERT_FALSE(spArrayListIsEmpty(al));
	spArrayListDestroy(al);
	return true;
}

int main(){
	RUN_TEST(spArrayListCreateTest);
	RUN_TEST(spArrayListCopyTest);
	RUN_TEST(spArrayListClearTest);
	RUN_TEST(spArrayListAddAtTest);
	RUN_TEST(spArrayListAddFirstTest);
	RUN_TEST(spArrayListAddLastTest);
	RUN_TEST(spArrayListRemoveAtTest);
	RUN_TEST(spArrayListRemoveFirstTest);
	RUN_TEST(spArrayListRemoveLastTest);
	RUN_TEST(spArrayListRemoveItemTest);
	RUN_TEST(spArrayListGetAtTest);
	RUN_TEST(spArrayListGetFirstTest);
	RUN_TEST(spArrayListGetLastTest);
	RUN_TEST(spArrayListMaxCapacityTest);
	RUN_TEST(spArrayListSizeTest);
	RUN_TEST(spArrayListIsFullTest);
	RUN_TEST(spArrayListIsEmptyTest);
	return 0;
}
