#ifndef SFTP_H
#define SFTP_H

/*
 * @param client_fd - client file discripter
 * @param message - string that stores the message to send
 * @param buffer - string that stores the message recieved
 * @param is_closed - boolean used to check if the server was closed
 */
void done(int, char*, char*, bool*);

void user(int, sqlite3*, sqlite3_stmt*, char*, char*);

void u_acct(int, sqlite3*, sqlite3_stmt*, char*, char*);

void pass(int, sqlite3*, sqlite3_stmt*, char*, char*);

/*
 * returns 1 in case of an error, returns 0 for success
 */
int type(int, char*, char*);

void stor(int, char*, char*);

void size(int, char*, char*);

void kill(int, char*, char*);

void list(int, char*, char*);

void name(int, char*, char*);

void tobe(int, char*, char*);

void retr(int, char*, char*);

void send_retr(int, char*, char*);

void stop_retr(int, char*, char*);

void reset_state();

#endif