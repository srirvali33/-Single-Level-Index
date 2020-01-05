/******************************************************************************
* Single level Index creation Program.
* This program is divided into 2 parts index creation and Listing the records in alphabetical order
* Create_index creates a index into a idx file with key and position.
* List_index the index keys and corresponding values are displayed in sorted manner on console.
* The user gives two commands from console -c and -l with input text file name and 
* output idx file name  along with key length he wants.
* Given -c create index method is executed and key and it's position are created in sorted manner 
* in idx file.
* Given -l the list of records from index file and corresponding values from input file are displayed.
*You must have a input testdata file called CS6360Asg5TestDataA.TXT or any input file in the same directory as the INDEX.c file
* The output index file DATA.idx will also be created in the same directory as above 2 files.
* program
* Written by Sriravali Baru
******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
/* For linux system make Linux parameter to 1 */
#define Linux 1

// defining 2 differnt values for record structure
struct record
{
	char key[25];
	long pos;
};

/* Function declaration before main for 2 functionalities */
void create_index(FILE*, FILE*,int);
void list_index(FILE*,FILE*,int);

int main(int argc, char *argv[])
{
  int key_len;
  // assign two file pointers in_fp and out_fp for traversal
  FILE *in_fp, *out_fp;
	char in_file_name;
	// making the output file to readonly mode using below commands
	#if Linux==1
	char write_cmd[] = "chmod 666 ";
	char read_cmd[] = "chmod 444 ";
	#else 
	char write_cmd[] = "attrib -R ";
	char read_cmd[] = "attrib +R ";
	#endif
	/*command line arguments reading 
	 *program_name cmd_type in_txt_file out_file key_length
   *cmd_type --- c -> create index, l -> list the file using index */
   
   /* checks for the total no of input parameters and runs if input parameters is 5 only */
	if(argc != 5)
	{	
		printf("Wrong Arguments..\nexiting program\n");
		// end the program
		exit(1);
	}
	// setting input pointer in read mode
	in_fp = fopen(argv[2],"r");
	if(in_fp == NULL)
	{
		printf("unable to open input text file\n");
		// end the program
		exit(2);
	}
	// converting ascii to integer for key value
	key_len = atoi(argv[4]);
	// checking for key length only between 1 and 24
	if((key_len > 0) && (key_len < 25))
	{
		// if -c is given the below code is executed to create index
		if(strcmp(argv[1],"-c")==0)
		{
			// setting ouput pointer in read mode and checking read condition
			out_fp = fopen(argv[3],"r");
			
			//checking if there is some value in output pointer
			if(out_fp != NULL)
			{
				//combining the commands and setting the outputfile to readonly format to make it non editable
				strcat(write_cmd,argv[3]);
				system(write_cmd);
			}
			// else display this below message
			else
			printf("unable to open output file in read mode\n");
			
			// setting ouput pointer in write mode and checking the write condition to write the DATA.idx file
			out_fp = fopen(argv[3],"w");
			if(out_fp == NULL)
			{
				printf("unable to open output file in write mode\n");
				exit(3);
			}
			printf("creating index for file\n");
			// create index file by using below method
			create_index(in_fp,out_fp,key_len);
			// making the file to remove read only format which we assigned before to write key and position in it
			strcat(read_cmd,argv[3]);
			system(read_cmd);
		}
		
		// action for performing the list operation if -l is given as input
		else if(strcmp(argv[1],"-l")==0)
		{
			printf("listing file using index\n");
			// opening the output file and storing in output file pointer
			out_fp = fopen(argv[3],"r");
			
			// if there is no data program is exited
			if(out_fp == NULL)
			{
				printf("unable to open output file in read mode\n");
				exit(4);
			}
			// if output file pointer is assigned some value then it is further moved to list index method
			list_index(in_fp,out_fp,key_len);
		}
		
		// apart from '-c' or '-l' are given then error message below is displayed on console
		else
		{
			printf("Invalid command\n");
			exit(5);
		}
	}
	
	// if more than 24 char is given then below error message is displayed
	else
		printf("Invalid key length\n");
	return 0;
} 

/* Function for creating index for file */
void create_index(FILE *in_fp, FILE *out_fp,int len)
{
	// declaring temporary variables to store data
	char key[25];
	long pos;
	int i,j,rec_num,lines; 
	char c;
	struct record r;
	// record pointer used for searching various key and record values
	struct record *ptr;
	lines = 0;
	
	// counting the no of lines charecter by charecter to write the same no of keys in idx file
	while((c=fgetc(in_fp))!=EOF)
	{
		if(c=='\n')
			lines++;
	}
	// adding 1 extra as the lines are counted when '\n' is encountered which happens after 1st line
	lines = lines +1;
	//setting ptr how much memory to be allocated for one record and total lines counted
	ptr = (struct record*)malloc(lines*sizeof(struct record));
	// checking the ptr could be allocted required memory
	if(ptr == NULL)
	{
		printf("memeory requested in bytes is %d\n",lines*sizeof(struct record));
		printf("unable to allocate memory for storing records\n");
		exit(0);
	}
	
	// setting file pointer to beginning position
	fseek(in_fp,0,0);
	// get current position of file in pos
	pos = ftell(in_fp); 
	i = 0;
	rec_num = 0;
	// searching key and storing it 
	while(!feof(in_fp))
	{
		c = fgetc(in_fp);
		// check the length of key specified by user and EoF condition and read charecter by cahraecter and store in memory location
		if((i < len) && (c!=EOF))
		{	
			// store as array of char one by one
			key[i]=c;
			i=i+1;
			// if i becomes equal to keylength specified by user assign \0 to end of key array and copy key and position to ptr address
			if(i == len)
			{
				key[i]='\0';
				printf("%s[%ld]\n",key,pos);
				// give values to key and position for each record by record number one by one
				strcpy((ptr+rec_num)->key,key);
				(ptr+rec_num)->pos = pos;
				// move to next record after assigning key and position
				rec_num = rec_num +1; 
				
			}
		}
		// executes only when \n encounters in textfile and sets i pointer to zero and takes the position of input file pointer to perform same
		// operations again
		if(c == '\n')
		{
			i = 0;
			pos = ftell(in_fp);
		}
	}

    // sorting the values stored in memory by bubblesort	
	// comparing each value till the length is total record number-1 for loop 1
	for(j=0;j<rec_num-1;j++)
	{
		// using other for loop to move forward one by one and compare keyvalues
		for(i=j+1;i<rec_num;i++)
		{
			// using strcmp and checking if 1st key value is greater than 2nd value then below code is executed even for duplicates
			if(strcmp((ptr+j)->key,(ptr+i)->key)>0)
			{
				//taking temporaray variables r.key and r.pos to swap values of i and j and sort
				strcpy(r.key,(ptr+j)->key);
				r.pos = (ptr+j)->pos;
				strcpy((ptr+j)->key,(ptr+i)->key);
				(ptr+j)->pos = (ptr+i)->pos;
				strcpy((ptr+i)->key,r.key);
				(ptr+i)->pos = r.pos;
			}
		}
	}
	//for loop to write index file for the rec_num(which is total records counted) one by one
	for(i=0;i<rec_num;i++)
	{
		pos = (ptr+i)->pos;
		// writing key value with its specific length in idx file
		fwrite((ptr+i),len,1,out_fp);
		// adding pointer or offset after key in idx file of 8 bytes long
		fwrite(&pos,8,1,out_fp);
	}
	// closing input and output file pointers
	fclose(in_fp);
	fclose(out_fp);
	// freeing memory space for ptr pointer
	free(ptr);
}

void list_index(FILE *in_fp, FILE *out_fp,int len)
{
	char key[25];
	long pos;
	int i;
	char c;
	char temp[1000];
	//reading keys from index file
	while(fread(key,len,1,out_fp))
	{
		// reading position from index file
		fread(&pos,8,1,out_fp);
		// placing inputtext file pointer to the position given in index file
		if(!fseek(in_fp,pos,0))
		{
			//reading 1000 charecters at a time and displaying charecter by charecter
			fread(temp,1,1000,in_fp);
			// checking for key matching between index and input text file
			for(i=0;i<len;i++)
			{
				// if key in index file doesn't match with the key in input text file error message below is displayed
				if(key[i]!=temp[i])
				{
					printf("%c",temp[i]);
					printf("invalid index file\n");
					exit(0);
				}
			}
			// displaying key and value from input file on console
			for(i=0;i<1000;i++)
			{
				printf("%c",temp[i]);
				if(temp[i]=='\n')
					break;
			}
		}
	}
	// close both input and output pointers
	fclose(in_fp);
	fclose(out_fp);
}
