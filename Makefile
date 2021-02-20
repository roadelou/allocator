################################### METADATA ###################################

# Contributors: roadelou
# Contacts: 
# Creation Date: 2021-02-19
# Language: Makefile

################################### ALIASES ####################################

CC = clang

src = src/*.c

head = include/*.h

exec = build/allocator.elf

################################### SPECIAL ####################################

.PHONY: clean

#################################### RULES #####################################

$(exec): $(src) $(head)
	mkdir -p build
	$(CC) -Wall -pedantic $(src) -I./include/ -o $(exec)

clean:
	rm -f $(exec)

##################################### EOF ######################################
