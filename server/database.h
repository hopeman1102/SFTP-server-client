#ifndef DATABASE_H
#define DATABASE_H

/*
 * Finds specified user's password from the database and sets userPass to it
 * @param userID - User ID string
 * @param userPass - Pointer to user password string
 * @returns 0 if found the specified user, otherwise 1
 */
int getPassword(char[], char*);

/*
 * @param userID - User ID string
 * @param userPass - User Password string
 * @returns 0 if specified password is correct, otherwise returns 1
 */
int checkPassword(char[], char[]);

#endif