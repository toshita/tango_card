TARGET := tango_card
CC := gcc
CFLAGS := -g -O0 -Wall

$(TARGET): tc_main.o tango_card.o
	$(CC) $(CFLAGS) tc_main.o tango_card.o -o tango_card

tc_main.o: 
	$(CC) $(CFLAGS) -c tc_main.c

tango_card.o:
	$(CC) $(CFLAGS) -c tango_card.c

clean:
	$(RM) tc_main.o tango_card.o tango_card

all:$(TARGET)
	.PHONY: all clean
