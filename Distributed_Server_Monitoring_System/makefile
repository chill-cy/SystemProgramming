CC := gcc
SHDIR := ../common.h
CFLAGS := -Wall -g -Os -lpthread -I $(SHDIR)
OBJS = master.o $(SHDIR)/common.o
# test
all: pihealthd
	@echo "This is OK"

pihealthd: $(OBJS)
	@$(CC) -o $@ $(CFLAGS) $(OBJS)

$(OBJS) : %.o: %.c
    @$(CC) -c $(CFLAGS) $< -o $@

#.PHONY: clean
clean:
	@rm -f *.o pihealthd
	@rm -f ../common/*.o
	@echo Done cleaning

install: pihealthd install.sh pihealthd.conf.sample
	@test -f /etc/pihealthd.conf || cp pihealthd.conf.sample /etc/pihealthd.conf
	@bash install.sh
	@cp -f pihealthd /user/bin
	@echo "pihealthd isntalled"
