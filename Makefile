CC=g++

SRCDIR=.
ODIR=./.obj

CFLAGS=-I$(SRCDIR)

_DEPS = server_client_common.h
DEPS = $(patsubst %,$(SRCDIR)/%,$(_DEPS))

_CLIENT_OBJ = server_client_common.o client.o
CLIENT_OBJ = $(patsubst %,$(ODIR)/%,$(_CLIENT_OBJ))

_SERVER_OBJ = server_client_common.o server.o
SERVER_OBJ = $(patsubst %,$(ODIR)/%,$(_SERVER_OBJ))

.phony: test
test:
	@echo "CFLAGS: "${CFLAGS}
	@echo "DEPS: "${DEPS}
	@echo "OBJ: "${OBJ}

$(ODIR)/%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

server: $(SERVER_OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

client: $(CLIENT_OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.phony: clean
clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~
	rm -f client
	rm -f server

.phony: all
all: server client