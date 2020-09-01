// THIS CODE IS MY OWN WORK. IT WAS WRITTEN WITHOUT CONSULTING A TUTOR OR CODE WRITTEN BY OTHER STUDENTS
// - David Lu
//  main.c
//  Project3
//
//  Created by David Lu on 11/21/18.
//  Copyright © 2018 David Lu. All rights reserved.
//

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int main(int argc, const char * argv[]) {
    int pfd1[2], pfd2[2];
    FILE *file = NULL;
    //create the pipes
    pipe(pfd1);
    pipe(pfd2);
    char c;
    int count = 0;
    char buff[32];
    switch (fork()) { //create the first child-parent proccesses
        case 0: //child: gets out the sorted list and prints with count of multiplicity
            //close the unnecessary fds and redirects them
            close(pfd1[1]);
            close(pfd1[0]);
            close(pfd2[1]);
            dup2(pfd2[0], 0);
            close(pfd2[0]);
            char tempword[32];
            char nextword[32];
            int cnt = 1;
            fgets(tempword, 32, stdin); //read by line
            while(fgets(nextword, 32, stdin) != NULL){ //if next line exists
                if (strcmp(tempword, nextword) == 0 ){ //compares current word to next word; if same,
                    cnt++;
                    continue;
                }
                printf("%5d %s", cnt, tempword);
                cnt = 1;
                strcpy(tempword, nextword);
                memset(nextword, '\0', 32);
            }
            printf("%5d %s", cnt, tempword);
            break;
        default: //parent: sorting and prasing
            switch (fork()) { //new fork for other child-parent proccess
                case 0: //child: sorting process
                    //close unecessary fds and redirects them
                    close(pfd1[1]);
                    dup2(pfd1[0], 0);
                    close(pfd1[0]);
                    close(pfd2[0]);
                    dup2(pfd2[1], 1);
                    close(pfd2[1]);
                    execl("/usr/bin/sort", "sort", (char *) NULL); //exec sort
                    break;
                default: //parent: parsing
                    //close unnecessary fds
                    close(pfd1[0]);
                    file = fdopen(pfd1[1], "w"); //open write end of pipe for writing
                    while ((c = fgetc(stdin))!= EOF) {
                        if (isalpha(c)) {
                            if (count < 30){ //checks if less than 30 characters: truncates
                                buff[count++] = tolower(c);
                            }
                        }
                        else { //not alphanumeric, checking entire word length
                            if (count>3){ //discards words shorter than 4 characters
                                //add newline and null terminator
                                buff[count++] = '\n';
                                buff[count++] = '\0';
                                fputs(buff,file); //puts word into stream
                            }
                            memset(&buff, '\0', 32);
                            count = 0;
                        }
                    }
                    break;
            }
            break;
    }
 
    fclose(file);
    close(pfd1[1]);
    close(pfd2[0]);
    close(pfd2[1]);
    return 0;
}
