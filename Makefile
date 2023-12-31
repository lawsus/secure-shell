CC = gcc

CFLAGS = -Wall -Wextra

OPENSSL_INCLUDE = /opt/homebrew/opt/openssl@3/include
OPENSSL_LIB = /opt/homebrew/opt/openssl@3/lib

CLIENT_SRCS = client.c diffie_hellman.c crypto_utils.c format.c bignum.c
SERVER_SRCS = server.c format.c diffie_hellman.c crypto_utils.c bignum.c

BIGNUM_TEST_SRCS = bignum.c bignum_test.c crypto_utils.c format.c

CLIENT_EXEC = client
SERVER_EXEC = server

BIGNUM_TEST_EXEC = bignum_test

all: ${CLIENT_EXEC} ${SERVER_EXEC}

# ${CLIENT_EXEC}: ${CLIENT_SRCS}
# 	${CC} ${CFLAGS} -o $@ $^

# ${SERVER_EXEC}: ${SERVER_SRCS}
# 	${CC} ${CFLAGS} -o $@ $^
${CLIENT_EXEC}: ${CLIENT_SRCS}
	${CC} ${CFLAGS} -I$(OPENSSL_INCLUDE) -L$(OPENSSL_LIB) -o $@ $^ -lssl -lcrypto

${SERVER_EXEC}: ${SERVER_SRCS}
	${CC} ${CFLAGS} -I$(OPENSSL_INCLUDE) -L$(OPENSSL_LIB) -o $@ $^ -lssl -lcrypto

test: ${BIGNUM_TEST_EXEC}

${BIGNUM_TEST_EXEC}: ${BIGNUM_TEST_SRCS}
	${CC} ${CFLAGS} -o $@ $^

clean:
	rm -f ${CLIENT_EXEC} ${SERVER_EXEC}