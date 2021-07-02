/**
* Bryan Solis
* Prof. Lawrence Sebald
* CMSC 421
* HW1
*/

#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

//calling functions
int cdCommand(char **data);
int exitCommand();
int exeProcess(char** ptChar);
int emptyStart(char **cdInfo);

//strings for cd, exit
char *insideString[] = { "cd","exit", "	"};
int(*insideFunc[]) (char **) = {
	&cdCommand,
	&exitCommand,
	&emptyStart
};


//checking the number of commands
int numCommands(){
	return (sizeof(insideString)/ sizeof(char *));
}


//CD command
int cdCommand(char **cdInfo){

	//checks if second cdInfo[1] exits, if not error
	if(cdInfo[1] == NULL || cdInfo[1] != 0 ){
		fprintf(stderr, "Error: Command \"cd\" is not supported \n");
		return 0;
	}

	return 0;
}

//error if space is read
int emptyStart(char **cdInfo){

	//checks if second cdInfo[1] exits, if not error
	if(cdInfo[1] == NULL || cdInfo[1] != 0 ){
		fprintf(stderr, "Error: Command \" \" is not supported \n");
		return 0;
	}

	return 0;
}



//exit command if appears
int exitCommand(){
	//printf("exit was here");
	return 0;
}

//executing command and checking exit or cd
int exeCommands(char **dataIn){

	int pointer;
	//int status;

	//if empty command was read
	if(dataIn[0] == NULL){
		return 1;
	}

	for(pointer = 0 ; pointer < numCommands(); pointer++ ){

		if(strcmp(dataIn[0],insideString[pointer]) == 0){

			return (*insideFunc[pointer])(dataIn);

		}

	}

//for future checks
	/*status =
	printf("this is stats %d\n", status);

	if(status == 0){

			return 0;
	}*/

	return exeProcess(dataIn);

}



//staring the precesses
int exeProcess(char** ptChar){

		//creating processes ID
		pid_t processID;
		pid_t waitID;
		int stats;

		//fork system call
		processID = fork();

		//fork returns 0 to the child process
		if(processID == 0){
			//the child process
			if(execvp(ptChar[0], ptChar) == -1){
				perror("Warning: Command Error");
			}
			//return 0;
			exit(EXIT_FAILURE);

		}
		//checking if fork had an error, if it does print error and keep going
		else if (processID < 0){
			//forking
			perror("Warning: Forking");
			return 0;

		}
		//fork executed with no problems
		else{

			//loop for the parent process and wait to keep tabs on its children
			do{

				//wait till till the processes are exited, WUNTRACED
				waitID = waitpid(processID, &stats, 0);

				//printf("status is: %d \n", (stats) );
				//printf("waitID is: %d \n", waitID);

				//if 256 stats means it was a problem wth command exiting
				if(stats ==256){

					//printf("Warning: Command Error\n" );
					return 0;
				}

			}while(!WIFEXITED(stats) && !WIFSIGNALED(stats));

		}


	return 1;

}


#define MAXSIZESPLIT 64
#define DELIMETERHELPER " \t\r\n\a"
char **splitLine(char *charLines){

	int posChar = 0;
	int charSize = MAXSIZESPLIT;
	char **splitChars = malloc(charSize * sizeof(char*));
	char *singleSplit;
	void **tmp;

	//error if no allocation 
        if (!splitChars) {
		printf("Allocation Failed \n");
		exit(EXIT_FAILURE);
	}

	//breaking line command into smaller sections using the delimiter
	singleSplit = strtok(charLines,DELIMETERHELPER);

	//looping till we have a null
	while(singleSplit != NULL){

		splitChars[posChar] = singleSplit;
		posChar++;

		//if char pointer is full reallocate
		if(posChar >= charSize){

			charSize += MAXSIZESPLIT;
			//splitChars = realloc(splitChars, charSize * sizeof(char*));
			tmp = realloc(splitChars, charSize * sizeof(char*));

			//Chech if allocation failed
			if (!splitChars) {
		        printf("Reallocation Failed \n");
						free(splitChars);
		        exit(EXIT_FAILURE);
		  }
			splitChars = (char**)tmp;


		}

		//dealing with the NULL
		singleSplit = strtok(NULL, DELIMETERHELPER);
	}

	splitChars[posChar] = NULL;
	return splitChars;

}

#define MAXSIZE 1024
char *readLine(FILE *fp, char x){

	//variables for reading the commands
	int posChar = 0;
	int charSize = MAXSIZE;
	char *charLine = malloc(sizeof(char) * charSize);
	char c = x;
	void *tmpC;

	//Error if no allocation
	if (!charLine) {
				printf("Allocation error\n");
				exit(EXIT_FAILURE);
	}


	//looop for the file and commands
	while( (c) != EOF){
    	//c = fgetc(fp);

		if(c == '\n'){
			//printf("\n im here how many times");
			charLine[posChar] = '\0';
			return charLine;

		}
		else{
			charLine[posChar] = c;
		}

		//increasing pointer position on the array
		posChar++;

		//getting the next character
		c = fgetc(fp);


		//if char array is full, increase the size by double
		if(posChar >= charSize){
			charSize += MAXSIZE;
			//charLine = realloc(charLine, charSize);

			tmpC = realloc(charLine, charSize);
			//check if allocation failed
			if (!charLine) {
						printf("Reallocation error\n");
						free(charLine);
						exit(EXIT_FAILURE);
			}
			charLine =(char*)tmpC;
		}

	}

	return charLine;

}



/**
main for the program
*/

int main(int argc, char *argv[]){


//checking if file to read is beeing passed
	if(argv[1] == NULL){
		printf("ERROR: Not File Selected\n");
		return 0;

	}

//checking if the user wants to add more than 1 file
	if(argc > 2 ){
		printf("Error: Cant add more than 1 File\n");
		return 0;
	}



	//opening file for reading
	FILE *fp = fopen(argv[1], "r");

	//variables to read line exeCommands
	//variables for split exeCommands and status of their execution
	char *charLines;
	char c;
	char ** ptChar;
	int curr_stats;

		//if the file is NULL close
   if (fp == NULL)
      return 0;

    //loop to read the commands from the FILE
		//checking that fgetc is not EOF if its close and exit
    while( (c = fgetc(fp)) != EOF ){

			//reading lines of codes
      charLines = readLine(fp, c);
      printf("> %s \n", charLines);

			//splitting the lines into individual pieces
      ptChar = splitLine(charLines);

			//executing the commands return 0 if problem, 1 if its correct
      curr_stats = exeCommands(ptChar);

      //if curr_stats is 0 meaning an exit, close, if 1 then keep going
      if(curr_stats == 0){

				free(charLines);
	      free(ptChar);
      	return 0;
      }

			//freeing pointers
      free(charLines);
      free(ptChar);


   }
	 //closing FILE
   fclose(fp);


	return 0;
}
