#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int main(int argc, char *argv[]) {
    if (argc == 1) {
        printf("[*] You must provide an action.");
        return 1;
    }

    // Get the command for the program
    char* cmd = argv[1];

    // Match the command
    // Adding an item to the list
    if (strcmp(cmd, "add") == 0) {
        // Check to make sure there are enough args
        if (argc <= 2) {
            printf("[*] You can not add nothing.");
            return 1;
        }

        // Init the contents buf and add the check box at the start
        char contents[1024] = "";
        strncat(contents, "[ ] ", sizeof(contents) - strlen(contents) - 1);

        for (int i = 2; i < argc; i++) {
            // Apenned the contents safely
            strncat(contents, argv[i], sizeof(contents) - strlen(contents) - 1);
        
            if (i < argc - 1) {
                strncat(contents, " ", sizeof(contents) - strlen(contents) - 1);
            }
        }
        
        // Open the file in append mode
        FILE *file = fopen("list.txt", "a");

        // See if file opened successfully
        if (file == NULL) {
            printf("[*] Error opening the todo file\n"); 
            return 1;
        }

        // Add newline
        strncat(contents, "\n", sizeof(contents) - strlen(contents) - 1);

        // Write the new todo item
        fprintf(file, "%s", contents);

        // Close the file
        fclose(file);
    
    // Print the entire list
    } else if (strcmp(cmd, "list") == 0) {
        // Open the file in read mode
        FILE *file = fopen("list.txt", "r");

        // See if file opened successfully
        if (file == NULL) {
            printf("[*] Error opening the todo file\n");
            return 1;
        }

        // Print the file
        int ch;
        while ((ch = fgetc(file)) != EOF) {
            putchar(ch);
        }

        // Close the file
        fclose(file);
    
    // Mark an item as done
    } else if (strcmp(cmd, "done") == 0) {
        // Check arg numbers
        if (argc <= 2) {
            printf("[*] You must specify an item to mark as done.");
            return 1;
        }

        int targetlineNum = atoi(argv[2]);
        bool compareNum = targetlineNum > 0;
        int current_line = 1;
        char buffer[1024]; // Max line length

        // Open the src file in read mode
        FILE *src = fopen("list.txt", "r");

        // See if file opened successfully
        if (src == NULL) {
            printf("[*] Error opening the todo file\n");
            return 1;
        }

        // Open the temp file in write mode
        FILE *temp = fopen("temp.txt", "w");

        // See if file opened successfully
        if (temp == NULL) {
            printf("[*] Error opening the todo file\n");
            return 1;
        }

        // Read file line by line
        while (fgets(buffer, sizeof(buffer), src)  != NULL) {
            // Check for crossing of by task string or by line num
            if (compareNum) {
                // Check if correct line and cross it off
                if (current_line == targetlineNum) {
                    if (strlen(buffer) >= 2 && buffer[1] != '\n') {
                        buffer[1] ='X';
                    }
                }
            } else {
                if (strstr(buffer, argv[2]) != NULL) {
                    if (strlen(buffer) >= 2 && buffer[1] != '\n') {
                        buffer[1] ='X';
                    }
                }
            }

            // Write line to temp file
            fputs(buffer, temp);
            current_line++;
        }

        // Close the files
        fclose(src);
        fclose(temp);

        // Removes old list file
        if (remove("list.txt") != 0) {
            printf("[*] Error deleting old list file\n");
            return 1;
        }

        // Renames old temp file to new list file
        if (rename("temp.txt", "list.txt") != 0) {
            printf("[*] Error renaming temp file\n");
            return 1;
        }
    
    // Clears the list
    } else if (strcmp(cmd, "clear") == 0) {
        // Open the temp file in append mode
        FILE *file = fopen("list.txt", "w");

        // See if file opened successfully
        if (file == NULL) {
            printf("[*] Error opening the todo file\n");
            return 1;
        }

        // Close the file to save as empty
        fclose(file);

    // Delete a specific item
    } else if (strcmp(cmd, "delete") == 0) {
        // Check arg numbers
        if (argc <= 2) {
            printf("[*] You must specify an item to remove.");
            return 1;
        }

        int targetlineNum = atoi(argv[2]); 
        bool compareNum = targetlineNum > 0;
        int current_line = 1;
        char buffer[1024]; // Max line length

        // Open the src file in read mode
        FILE *src = fopen("list.txt", "r");

        // See if file opened successfully
        if (src == NULL) {
            printf("[*] Error opening the todo file\n");
            return 1;
        }

        // Open the temp file in write mode
        FILE *temp = fopen("temp.txt", "w");

        // See if file opened successfully
        if (temp == NULL) {
            printf("[*] Error opening the todo file\n");
            return 1;
        }

        // Read file line by line
        while (fgets(buffer, sizeof(buffer), src)  != NULL) {
            if (compareNum) {
                if (current_line != targetlineNum) {
                    fputs(buffer, temp);
                }    
            } else {
                if (strstr(buffer, argv[2]) == NULL) {
                    fputs(buffer, temp);
                }
            }
            current_line++;
        }

        // Close the files
        fclose(src);
        fclose(temp);

        // Removes old list file
        if (remove("list.txt") != 0) {
            printf("[*] Error deleting old list file\n");
            return 1;
        }

        // Renames old temp file to new list file
        if (rename("temp.txt", "list.txt") != 0) {
            printf("[*] Error renaming temp file\n");
            return 1;
        }
    
    // Export the list
    } else if (strcmp(cmd, "export") == 0) {
        if (argc <= 2) {
            printf("[*] You must specify a name for the file.");
            return 1;
        }
        
        if (rename("list.txt", argv[2]) != 0) {
            printf("[*] Error renaming file\n");
            return 1;
        }
    
    // Import a list
    } else if (strcmp(cmd, "import") == 0) {
        if (argc <= 2) {
            printf("[*] You must specify the of the file.");
            return 1;
        }
        
        if (rename(argv[2], "list.txt") != 0) {
            printf("[*] Error renaming file\n");
            return 1;
        }
    
    // Print total tasks
    } else if (strcmp(cmd, "total") == 0) {
        FILE *file = fopen("list.txt", "r");
        
        if (file == NULL) {
            printf("[*] Error opening the todo file\n");
            return 1;
        }

        int ch;
        int lines = 0;

        // Read charecters till end of the line
        while ((ch = fgetc(file)) != EOF) {
            if (ch == '\n') {
                lines++;
            }
        }

        fclose(file);

        // Print the total lines
        printf("Total tasks: %d\n", lines);
        
    // Prints the version
    } else if (strcmp(cmd, "version") == 0) {
        printf("Todo CLI v0.0.1\n");
        printf("Written in C\n");

    // Prints available commands
    } else if (strcmp(cmd, "help") == 0) {
        printf("Available commands:\n");
        printf("    todo add <task to add> (Adds the task to the todo list)\n");
        printf("    todo list (Lists all the files in the task)\n");
        printf("    todo done <line number of item or item name in quotes> (Marks an item as done)\n");
        printf("    todo clear (Clears the entire list)\n");
        printf("    todo delete <line number of item or item name in quotes> (Deletes the task based on it's #)\n");
        printf("    todo export <filename> (Exports the list)\n");
        printf("    todo import <filename> (Imports a file)\n");
        printf("    todo total (Prints the total amount of tasks)\n");
        printf("    todo help (Prints all the commands)\n");
        printf("    todo version (Prints the version)");
    
    } else {
        printf("Unknown command");
    }
}
