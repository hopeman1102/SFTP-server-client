#include <stdio.h>

int getPassword(char userID[], char* userPass) {
    FILE *fp = fopen("data.csv", "r");
    if (!fp) {
        perror("Can't open file");
        exit(1);
    } else {
        char line[1024];
        int row = 0;
        int col = 0;

        while (fgets(line, 1024, fp)) {
            char* val = strtok(line, ", ");
            if (strcmp(userID, val) == 0) {
                val = strtok(NULL, ", ");
                val[strcspn(val, "\n")] = 0; // remove "\n" if exists
                strcpy(userPass, val);
                fclose(fp);
                return 0;
            }
        }
        fclose(fp);
        return 1;
    }
}

int checkPassword(char userID[], char userPass[]) {
    return 1;
}