#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

char *isValidCommand(char *command);
int isFileExsist(char *filePath);

int main(int argc, char **argv) {
    
    int pid ,stat,i = 0;
    char cmd[100]; 
    char *command; 
    char tempCmd[100]; 
    char *filePath; 
    char *arrArguments[50]; 
    
    while (1) {
        
        printf("shell> ");
        fflush(stdout); 
        // Read input from the user
        fgets(cmd, sizeof(cmd), stdin);        
        // Remove trailing newline character
        cmd[strcspn(cmd, "\n")] = '\0';
        // Check if the command is "leave"
        if (strcmp(cmd,"leave") == 0) { 
            printf("exited shell...\n");
            exit(1);
        }
        // Tokenize the command and its arguments manually
        strcpy(tempCmd,cmd);
        arrArguments[i++] = strtok(tempCmd, " \t");// arrArguments[0] is the command 
        
        if (arrArguments[0]== NULL) {
            continue;  // If no command was entered, go back to the prompt
        }
        // Get the arguments
        while ((arrArguments[i++] = strtok(NULL, " \t")) != NULL && i < 50);
	

        if (strcmp(arrArguments[0], "cd") == 0) {
      		// Check if no directory is provided after "cd"
        	if (arrArguments[1] == NULL) {
        	    // If no directory provided, print an error message
        	    printf("cd - MISSING ARGUMENT\n");
                }   
                else {
                  // Change directory
                    if (chdir(arrArguments[1]) == -1) {
                     // If chdir fails, print an error message
                     perror("cd");
                     }
                 }
                continue;
          }  

         if (pid=fork()==0) {
             		
             		filePath=isValidCommand(arrArguments[0]);//arrArguments[0]the command

             		
             		if(filePath==NULL) {
             			printf("%s - INVALID COMMAND\n",arrArguments[0]);
             			exit(1);  // Exit child process if command is invalid
             		}
             		else {
             		      
 			                
             			execv(filePath,arrArguments);
             		
             		}
            
        }
        else{
              wait(&stat);

        }
           
       
    }
   
    return 0;
   
} 


char *isValidCommand (char*command)
{
   char *pathVar = getenv("PATH");
   char *tempDir = strtok(pathVar, ":");
   char filePath[100]="";
   char fileExist=0; //yet file not found 
   char *returnPathCmd;
  
   while(tempDir!=NULL && fileExist==0) {
  	
  	if(fileExist ==0) {
           strcpy(filePath,tempDir); 
  	   strcat(filePath, "/");
  	   strcat(filePath,command);
  	   
  	 	if((isFileExsist(filePath)) == 1) {
  	 	   fileExist=1;
  	 	   returnPathCmd=calloc(strlen(filePath)+1,sizeof(char));
  	 	   strcpy(returnPathCmd,filePath);
  	 	   return returnPathCmd;
  	 	}
  	
  	 }
  	 
  	 tempDir = strtok(NULL, ":");
  	 
   }
   return NULL;
  
}


int isFileExsist(char *filePath) {
    // Use access to check if the file exists and is executable
    if (access(filePath, X_OK) == 0) {
        return 1; // The file exists and is executable
    } else {
        return 0; // The file does not exist or is not executable
    }
}



   

