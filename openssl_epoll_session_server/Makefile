CC      := gcc
CFLAGS  := -Wall -Wextra -O2 -g -Iinclude
LDFLAGS := -lssl -lcrypto
TARGET  := server
SRCS    := src/main.c src/http.c src/session.c src/user.c src/util.c
OBJS    := $(SRCS:.c=.o)

.PHONY: all clean cert run

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

cert:
	mkdir -p cert
	openssl req -x509 -newkey rsa:2048 -nodes -days 365 \
		-subj "/CN=localhost" \
		-keyout cert/server.key \
		-out cert/server.crt

run: all cert
	./$(TARGET) 8443

clean:
	rm -f $(TARGET) $(OBJS)
