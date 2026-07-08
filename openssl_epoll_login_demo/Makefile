CC ?= gcc
CFLAGS ?= -Wall -Wextra -O2 -g
PKG_CONFIG ?= pkg-config
OPENSSL_CFLAGS := $(shell $(PKG_CONFIG) --cflags openssl 2>/dev/null)
OPENSSL_LIBS := $(shell $(PKG_CONFIG) --libs openssl 2>/dev/null)
ifeq ($(OPENSSL_LIBS),)
OPENSSL_LIBS := -lssl -lcrypto
endif

TARGET := server
OBJS := main.o server.o tls.o http.o auth.o session.o util.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(OPENSSL_LIBS)

%.o: %.c
	$(CC) $(CFLAGS) $(OPENSSL_CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS) server.crt server.key cookies.txt

cert:
	openssl req -x509 -newkey rsa:2048 -nodes \
		-keyout server.key -out server.crt -days 365 \
		-subj "/CN=localhost"

.PHONY: all clean cert
