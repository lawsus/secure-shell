/**
 * This function gets called whenever
 * a server fails to fork.
 */
void print_fork_failed();

/**
 * This function prints the received
 * long long value.
 */
void print_received(char *name, long long val);

/**
 * This function prints the sent
 * long long value.
 */
void print_sent(char *name, long long val);

/**
 * This function prints the shared secret.
 */
void print_shared_secret(long long val);

/**
 * This function prints when the componenent
 * disconnects or an error occurs.
 */
void print_disconnected(char *component);