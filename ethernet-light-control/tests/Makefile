#CFLAGS += -Wall -Werror -Wextra
CFLAGS += -Wall -Wextra
CFLAGS += -Wmissing-prototypes
CFLAGS += -Wstrict-prototypes
CFLAGS += -Wmissing-declarations

# Uncomment to demo c99 parametric testing.
CFLAGS += -std=c99

CFLAGS += -Iinclude

all: example

example: example.c greatest.h can2udp.o avr_mock.o network_mock.o can_mock.o
	${CC} -o $@ example.c can2udp.o avr_mock.o network_mock.o can_mock.o ${CFLAGS} ${LDFLAGS}

can2udp.o: can2udp.c

clean:
	rm -f example *.o *.core

