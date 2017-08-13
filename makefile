CC = gcc
OBJS = array_list.o
EXEC = chessprog
ARRAY_LIST_TEST_OBJS = array_list_unit_test.o array_list.o
# TODO: Remove debug flag
COMP_FLAG = -g -std=c99 -Wall -Wextra -Werror -pedantic-errors
UNIT_TESTS = array_list_unit_test

$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $@

# Tests
array_list_unit_test: $(ARRAY_LIST_TEST_OBJS)
	$(CC) $(ARRAY_LIST_TEST_OBJS) -o $@
array_list_unit_test.o: array_list_unit_test.c array_list.h
	$(CC) $(COMP_FLAG) -c $*.c

# Source files
array_list.o: array_list.c array_list.h
	$(CC) $(COMP_FLAG) -c $*.c

clean:
	rm -f *.o $(EXEC) $(UNIT_TESTS)
