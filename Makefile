CC = gcc

CFLAGS = -Wall -Wextra

CLIENT_SRCS = client.c diffie_hellman.c crypto_utils.c format.c
SERVER_SRCS = server.c format.c diffie_hellman.c crypto_utils.c

CLIENT_EXEC = client
SERVER_EXEC = server

all: ${CLIENT_EXEC} ${SERVER_EXEC}

${CLIENT_EXEC}: ${CLIENT_SRCS}
	${CC} ${CFLAGS} -o $@ $^

${SERVER_EXEC}: ${SERVER_SRCS}
	${CC} ${CFLAGS} -o $@ $^

clean:
	rm -f ${CLIENT_EXEC} ${SERVER_EXEC}