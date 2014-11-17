all: game server

DOXYGENCONF=doxygen.conf

OBJS_server := server.o

server: $(OBJS_server)

game: $(OBJS) $(COBJS)
	gcc -o $@ -lm -L/usr/lib -lncurses-lprotobuf -lz $(OBJS) $(COBJS) 

-include *.d

%.o: %.c
	gcc -c -g3 -o $@ $<
	gcc -MM $< > $*.d

clean:
	rm -rf $(OBJS) game menu
	rm -rf $(COBJS) game menu

test: all

doc:
	doxygen $(DOXYGENCONF)
