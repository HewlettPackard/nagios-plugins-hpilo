SRCS=nagios_hpilo_engine.c nagios_hpilo_snmp.c

CC = gcc

LDFLAGS=-lsnmp -Wl,--no-as-needed -L/usr/lib/x86_64-linux-gnu -lnetsnmp

IFLAGS=-I./include -I../

TARGETBIN = $(DESTDIR)$(PREFIX)/libexec
TARGET=nagios_hpilo_engine

.SUFFIXS: .c

all: $(TARGET)

%.o: %.c
	$(CC) -c $< $(CFLAGS) $(IFLAGS) 

$(TARGET): nagios_hpilo_engine.o nagios_hpilo_snmp.o
	$(CC) $(LDFLAGS) -o $@ $^

clean:
	rm -f *.o
	rm -f $(TARGET)

install: $(TARGET)
	install -m 755 $^ $(TARGETBIN)

