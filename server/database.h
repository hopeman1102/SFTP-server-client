#ifndef DATABASE_H
#define DATABASE_H

/*
 * @param userID - User ID string
 * @param userPass - User Password string
 * @param db - pointer to db
 * @returns 0 if specified password is correct, otherwise returns 1
 */
int checkPassword(char[], char[], sqlite3**);

#endif