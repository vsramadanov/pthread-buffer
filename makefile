CFLAGS=-Wall -Wextra -Wpedantic -MD -fsanitize=address -ggdb -O0
LDLIBS=-lpthread

DIR=bin
INCLUDE=-I.
SRC=$(shell find * -name '*.c')
OBJ=$(addprefix $(DIR)/, $(SRC:.c=.o))
VPATH=../

.PHONY: clean all

all: $(DIR)/runme

$(DIR)/runme: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(INCLUDE) -o $(DIR)/runme $(LDLIBS)

$(DIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@
    
test:
	@echo 'SRC:' $(SRC)
	@echo 'OBJ:' $(OBJ)

clean:
	rm -rf $(DIR)

include $(shell find * -name '*.d')

