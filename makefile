CC = gcc
EXEC = chessprog
SOURCES = $(wildcard src/*.c)
OBJS = $(SOURCES:src/%.c=obj/%.o)
ARRAY_LIST_TEST_OBJS = obj/array_list_unit_test.o obj/array_list.o
# TODO: Remove debug flag
COMP_FLAG = -g -std=c99 -Wall -Wextra -Werror -pedantic-errors
UNIT_TESTS = array_list_unit_test

.PHONY: all
all: $(EXEC)
$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $@

# Tests
array_list_unit_test: $(ARRAY_LIST_TEST_OBJS)
	$(CC) $(ARRAY_LIST_TEST_OBJS) -o $@
obj/array_list_unit_test.o: tests/array_list_unit_test.c src/array_list.h
	$(CC) -Isrc $(COMP_FLAG) -c $< -o $@

# Source files
obj/game_engine.o: src/game_engine.c src/game_engine.h src/array_list.h
	$(CC) $(COMP_FLAG) -c $< -o $@
obj/print_utils.o: src/print_utils.c src/print_utils.h src/game_engine.h
	$(CC) $(COMP_FLAG) -c $< -o $@
obj/main_aux.o: src/main_aux.c src/main_aux.h src/command_parser.h src/game_engine.h src/print_utils.h
	$(CC) $(COMP_FLAG) -c $< -o $@
obj/minimax.o: src/minimax.c src/minimax.h src/game_engine.h
	$(CC) $(COMP_FLAG) -c $< -o $@
obj/main.o: src/main.c src/main_aux.h
	$(CC) $(COMP_FLAG) -c $< -o $@

# Generic rule to create object files
obj/%.o: src/%.c src/%.h
	$(CC) $(COMP_FLAG) -c $< -o $@

clean:
	rm -f $(OBJS) $(EXEC) $(UNIT_TESTS)
