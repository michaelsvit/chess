CC = gcc
EXEC = chessprog
SOURCES = $(wildcard *.c)
OBJS = $(SOURCES:.c=.o)
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
	$(CC) $(COMP_FLAG) -c $<

# Source files
game_engine.o: game_engine.c game_engine.h array_list.h
	$(CC) $(COMP_FLAG) -c $<
print_utils.o: print_utils.c print_utils.h game_engine.h
	$(CC) $(COMP_FLAG) -c $<
main_aux.o: main_aux.c main_aux.h command_parser.h game_engine.h print_utils.h
	$(CC) $(COMP_FLAG) -c $<
main.o: main.c main_aux.h
	$(CC) $(COMP_FLAG) -c $<

# General rule to create object files
%.o: %.c %.h
	$(CC) $(COMP_FLAG) -c $<
# array_list.o: array_list.c array_list.h
	# $(CC) $(COMP_FLAG) -c $*.c

clean:
	rm -f $(OBJS) $(EXEC) $(UNIT_TESTS)
