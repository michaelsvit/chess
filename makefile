$(shell mkdir -p 'obj/graphics')
CC = gcc
EXEC = chessprog
SOURCES = $(wildcard src/*.c) $(wildcard src/graphics/*.c)
OBJS = $(SOURCES:src/%.c=obj/%.o)
# TODO: Remove debug flag
COMP_FLAG = -g -std=c99 -Wall -Wextra -Werror -pedantic-errors `sdl2-config --cflags`

.PHONY: all
all: $(EXEC)
$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $@ `sdl2-config --libs`

# Source files
obj/game_engine.o: src/game_engine.c src/game_engine.h src/array_list.h src/xml_serializer.h src/xml_parser.h
	$(CC) $(COMP_FLAG) -c $< -o $@
obj/print_utils.o: src/print_utils.c src/print_utils.h src/game_engine.h
	$(CC) $(COMP_FLAG) -c $< -o $@
obj/main_aux.o: src/main_aux.c src/main_aux.h src/command_parser.h src/game_engine.h src/minimax.h src/print_utils.h src/xml_serializer.h
	$(CC) $(COMP_FLAG) -c $< -o $@
obj/xml_serializer.o: src/xml_serializer.c src/xml_serializer.h src/game_engine.h src/print_utils.h
	$(CC) $(COMP_FLAG) -c $< -o $@
obj/xml_parser.o: src/xml_parser.c src/xml_parser.h src/game_engine.h
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
