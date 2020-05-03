#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX_LINE_LENGTH 500
#define MAX_LABEL_LENGTH 50
#define MAX_NUM_OF_LINES 65536
#define MAX_LEN_OF_ADDRESS_OF_LABEL_IN_HEX 6 //65536=10000(HEX)
char labels[MAX_NUM_OF_LINES][MAX_LINE_LENGTH]; //array that stores all the labels and their addresses 
char lines[MAX_NUM_OF_LINES][MAX_LINE_LENGTH]; //array that stores the contents of the main memory



void convert_opcode_to_hexa(char* ins, char* res)
{
	/*
	converts the opcode to hexadecimal number
	*/

	if (strcmp(ins, "add") == 0)
		strcat(res, "0");
	if (strcmp(ins, "sub") == 0)
		strcat(res, "1");
	if (strcmp(ins, "and") == 0)
		strcat(res, "2");
	if (strcmp(ins, "or") == 0)
		strcat(res, "3");
	if (strcmp(ins, "sll") == 0)
		strcat(res, "4");
	if (strcmp(ins, "sra") == 0)
		strcat(res, "5");
	if (strcmp(ins, "reserved") == 0)
		strcat(res, "6");
	if (strcmp(ins, "beq") == 0)
		strcat(res, "7");
	if (strcmp(ins, "bgt") == 0)
		strcat(res, "8");
	if (strcmp(ins, "ble") == 0)
		strcat(res, "9");
	if (strcmp(ins, "bne") == 0)
		strcat(res, "A");
	if (strcmp(ins, "jal") == 0)
		strcat(res, "B");
	if (strcmp(ins, "lw") == 0)
		strcat(res, "C");
	if (strcmp(ins, "sw") == 0)
		strcat(res, "D");
	if (strcmp(ins, "lhi") == 0)
		strcat(res, "E");
	if (strcmp(ins, "halt") == 0)
		strcat(res, "F");
}




char convert_register_to_hexa(char* reg, char* res)
{
	/*
	converts the register to hexadecimal number
	*/


	if (strcmp(reg, "$zero") == 0)
		strcat(res, "0");
	if (strcmp(reg, "$imm") == 0)
		strcat(res, "1");
	if (strcmp(reg, "$v0") == 0)
		strcat(res, "2");
	if (strcmp(reg, "$a0") == 0)
		strcat(res, "3");
	if (strcmp(reg, "$a1") == 0)
		strcat(res, "4");
	if (strcmp(reg, "$t0") == 0)
		strcat(res, "5");
	if (strcmp(reg, "$t1") == 0)
		strcat(res, "6");
	if (strcmp(reg, "$t2") == 0)
		strcat(res, "7");
	if (strcmp(reg, "$t3") == 0)
		strcat(res, "8");
	if (strcmp(reg, "$s0") == 0)
		strcat(res, "9");
	if (strcmp(reg, "$s1") == 0)
		strcat(res, "A");
	if (strcmp(reg, "$s2") == 0)
		strcat(res, "B");
	if (strcmp(reg, "$gp") == 0)
		strcat(res, "C");
	if (strcmp(reg, "$sp") == 0)
		strcat(res, "D");
	if (strcmp(reg, "$fp") == 0)
		strcat(res, "E");
	if (strcmp(reg, "$ra") == 0)
		strcat(res, "F");
}


char* dec_to_hex_converter(int num, char* x)
{
	//converts a decimal number to 4 hexa digits


	int i = 1;
	int size = 0;

	sprintf(x, "%04X\n", num);
	size = strlen(x);
	if (size > 4)
	{
		while (size > 4)
		{
			x++;
			size--;
		}
		x--;
	}
	*(x + 4) = '\0';
	return x;
}



char* search_for_label(int num_of_labels, char label[])
{
	//searches the label in the labels's array

	int i = 0, len_of_label = 0, a = 0;
	char *helper_ptr = NULL;

	len_of_label = strlen(label);
	for (i = 0; i < num_of_labels; i++)
	{
		if (strncmp(labels[i], label, len_of_label) == 0)
		{
			helper_ptr = strstr(labels[i], ":");
			return helper_ptr + 1;
		}
	}

}


int has_immediate(char line[])
{
	
	//returns 1 if a line contains immediate
	

	int i = 0, counter = 0;
	while (line[i] != '\0')
	{
		if (line[i] == ',')
			counter++;
		i++;
	}
	if (counter == 3)
		return 1;
	return 0;
}

int is_label(char line[])
{
	
	//returns 1 if a line is a label
	

	int i = 0, counter = 0;
	while (line[i] != '\0')
	{
		if (line[i] == ':')
			return 1;
		i++;
	}
	return 0;
}



char* analyzeImm(char* immediate, int num_of_labels, char *helper_ptr, char* x)
{
	//checks the kind of the immidate and converts it to 4 hexadecimal digits depending on it's type

	int num = 0, size = 0;
	char buffer[MAX_LINE_LENGTH];

	if (isalpha(*immediate) != 0) // the immediate is a label
	{

		helper_ptr = search_for_label(num_of_labels, immediate);
		return helper_ptr;
	}

	if (*immediate == '0' && (*(immediate + 1) == 'x' || *(immediate + 1) == 'X')) //the immediate is hexadecimal number
	{
		immediate = immediate + 2;
		size = strlen(immediate);
		if (size >= 4)
		{
			return(immediate + size - 4);
		}

		if (size < 4)
		{
			memset(x, '0', 4 - size);
			*(x + 4 - size) = '\0';
			strcat(x, immediate);
		}
		return x;
	}

	dec_to_hex_converter(atoi(immediate), x); //the immediate is decimal number

}

int is_space(char line[])
{
	/*
	returns 1 if a line is empty
	*/

	int i = 0, counter = 0;
	char* pointer = NULL;

	pointer = strstr(line, "$");
	if (pointer == NULL & (is_label(line) == 0))
		return 1;
	return 0;
}


int first_search(FILE *fPtrRead)
{

	//search all the labels and put them in an array {label1:address in hex, label2:address in hex,...}


	int num_of_labels = 0, ind = 0, address_of_label_int = 0;
	char line[MAX_LINE_LENGTH];
	char address_of_label[] = "";
	char* helper_ptr = NULL;
	char* helper_ptr1 = NULL;
	char* helper_ptr2 = NULL;
	char buffer[MAX_LINE_LENGTH];
	char x[6];

	while (!feof(fPtrRead))
	{
		fgets(line, MAX_LINE_LENGTH, fPtrRead);
		helper_ptr1 = line;
		helper_ptr2 = strstr(helper_ptr1, "#"); //removes the comment, if one exist
		if (helper_ptr2 != NULL)
			*helper_ptr2 = '\0';
		if (is_empty(line))
			continue;
		if (has_immediate(line))
			address_of_label_int = address_of_label_int + 2;
		else if (is_label(line) == 0)
			address_of_label_int = address_of_label_int + 1;
		if (is_label(helper_ptr1))
		{
			while (*helper_ptr1 == 9 || *helper_ptr1 == ' ') //if at the begining of the line we have got a tab or a space- ignore it
			{
				helper_ptr1++;
			}
			helper_ptr = strstr(helper_ptr1, ":");
			helper_ptr = helper_ptr + 1;
			*helper_ptr = '\0';
			strcpy(buffer, helper_ptr1);
			dec_to_hex_converter(address_of_label_int, x);
			strcat(buffer, x);
			strcpy(labels[ind], buffer);
			ind++;
			num_of_labels++;

		}
	}
	return num_of_labels;
}


int is_empty(char* string)
{
//This function will return 0 if any character is not whitespace or 1 otherwise.

char* copy_string;

copy_string = string;
while (*string != '\0')
{
if (!isspace((unsigned char)*string))
return 0;
string++;
}
return 1;
}




int word(char* line)
{
	//checks if a line contains the unique instruction .word- If so put the data in the required address of the memory

	char x[10];
	char* data;
	char* copy_line;
	char* helper_ptr1 = NULL;
	int int_address = 0, size = 0, num = 0, word_flag = 0;

	if (is_empty(line))
		return 0;
	while (*line == 9 || *line == ' ') //if at the begining of the line we have got a tab or a space
	{
		line++;
	}
	if (strncmp(line, ".word", 5) == 0)
	{
		word_flag = 1;
		copy_line = line + 5;
		while (memcmp(copy_line, " ", 1) == 0 || memcmp(copy_line, "	", 1) == 0) //ignore the empty spaces
		{
			copy_line++;
		}

		helper_ptr1 = strstr(copy_line, " ");
		if (helper_ptr1 != NULL)
		{
			data = helper_ptr1 + 1;
			*helper_ptr1 = '\0';
		}
		else
		{
			helper_ptr1 = strstr(copy_line, "	");
			data = helper_ptr1 + 1;
			*helper_ptr1 = '\0';
		}
		if (*copy_line == '0' && (*(copy_line + 1) == 'x' || *(copy_line + 1) == 'X')) //the address is a hexadecimal number
		{
			copy_line = copy_line + 2;
			uppercase(copy_line);
			int_address = hexadecimalToDecimal(copy_line);
		}
		else
			int_address = atoi(copy_line); // the address is a decimal number
		while (memcmp(data, " ", 1) == 0 || memcmp(data, "	", 1) == 0) //ignore the empty spaces
		{
			data++;
		}
		helper_ptr1 = data;
		while (*(helper_ptr1) != ' ' || *(helper_ptr1) != 9 || *(helper_ptr1) != '\n')
		{
			helper_ptr1++;
			if (*(helper_ptr1) == ' ' || *(helper_ptr1) == 9 || *(helper_ptr1) == '\n') //ignore empty spaces
			{
				*helper_ptr1 = '\0';
				break;
			}
		}
		if (*data == '0' && (*(data + 1) == 'x' || *(data + 1) == 'X')) //the data is a hexadecimal number
		{
			data = data + 2;
			size = strlen(data);
			if (size >= 4)
			{
				data = data + size - 4;
				uppercase(data);
			}
			else
			{

				memset(x, '0', 4 - size);
				*(x + 4 - size) = '\0';

				strcat(x, data);
				strcpy(data, x);
				uppercase(data);

			}
		}

		else //the data is a decimal number
		{
			num = atoi(data);
			data = dec_to_hex_converter(num, x);
			uppercase(data);
		}
		strcpy(lines[int_address], data);
	}
	return word_flag;
}


int second_search(FILE *fPtrRead, int num_of_labels, char *helper_ptr, char* x)
{

	//converts every line to hexadecimal sequence of digits

	int ind = 0, word_flag = 0;
	char line[MAX_LINE_LENGTH];
	char* helper_ptr0 = NULL;
	char* helper_ptr1 = NULL;
	char* helper_ptr2 = NULL;
	char* helper_ptr3 = NULL;
	char digits[5] = "";
	int has_immediate_in_line = 0;


	while (!feof(fPtrRead))
	{
		strcpy(line, "");
		fgets(line, MAX_LINE_LENGTH, fPtrRead);
		strcpy(digits, "");
		helper_ptr1 = NULL;
		helper_ptr2 = NULL;
		helper_ptr3 = NULL;
		helper_ptr0 = line;
		if (strcmp(line, "\n") == 0)    //if the line is empty
			continue;
		if (is_empty(line))
			continue;
		while (*helper_ptr0 == 9 || *helper_ptr0 == ' ') //if at the begining of the line we have got a tab
		{
			helper_ptr0++;
		}
		helper_ptr3 = strstr(helper_ptr0, "#"); //removes the comment, if there is
		if (helper_ptr3 != NULL)
			*helper_ptr3 = '\0';
		has_immediate_in_line = has_immediate(helper_ptr0);
		word_flag = word(helper_ptr0);
		if (word_flag == 1)
			continue;
		if (is_label(helper_ptr0) != 1 & is_space(helper_ptr0) != 1) //if the line is not a label or an empty space 
		{
			helper_ptr1 = strstr(helper_ptr0, "$");
			helper_ptr2 = helper_ptr1;
			helper_ptr1--;
			if ((*helper_ptr1 == ' ' || *helper_ptr1 == 9))
			{
				while (*helper_ptr1 == ' ' || *helper_ptr1 == 9) //ignores empty spaces or tab
				{
					helper_ptr1--;
				}
				helper_ptr1++;

			}
			*helper_ptr1 = '\0';
			convert_opcode_to_hexa(helper_ptr0, digits); //first digit
			helper_ptr1 = strstr(helper_ptr2, ",");
			helper_ptr0 = helper_ptr1 + 1;
		
		
			if (*(helper_ptr1 - 1) == ' ' || *(helper_ptr1 - 1) == 9)
			{
				helper_ptr1--;
				while (*(helper_ptr1) == ' ' || *(helper_ptr1) == 9) //ignores empty spaces
				{
					helper_ptr1--;
				}
				helper_ptr1++;
			}
			*helper_ptr1 = '\0';
			convert_register_to_hexa(helper_ptr2, digits); //second digit
			helper_ptr1 = strstr(helper_ptr0, "$");
			helper_ptr2 = strstr(helper_ptr1, ",");
			helper_ptr3 = helper_ptr2 + 1;
			if (*(helper_ptr2 - 1) == ' ' || *(helper_ptr2 - 1) == 9)
			{
				helper_ptr2--;
				while (*(helper_ptr2) == ' ' || *(helper_ptr2) == 9) //ignores empty spaces
				{
					helper_ptr2--;
				}
				helper_ptr2++;
			}
			*helper_ptr2 = '\0';
			convert_register_to_hexa(helper_ptr1, digits); //third digit
			while (memcmp(helper_ptr3, " ", 1) == 0 || *helper_ptr3 == 9)
			{
				helper_ptr3++;
			}
			if (has_immediate_in_line == 0) //the line does not include immediate
			{
				helper_ptr1 = helper_ptr3;
				while (*(helper_ptr1) != ' ' || *(helper_ptr1) != 9 || *(helper_ptr1) != '\n')
				{
					helper_ptr1++;
					if (*(helper_ptr1) == ' ' || *(helper_ptr1) == 9 || *(helper_ptr1) == '\n') //ignore empty spaces
					{
						*helper_ptr1 = '\0';
						break;
					}
				}

				convert_register_to_hexa(helper_ptr3, digits); //fourth digit
				strcpy(lines[ind], digits);
				ind++;
			}
			else                                    //the line includes immidiate
			{
				helper_ptr1 = strstr(helper_ptr3, ",");
				helper_ptr2 = helper_ptr1 + 1;
				if (*(helper_ptr1 - 1) == ' ' || *(helper_ptr1 - 1) == 9)
				{
					helper_ptr1--;
					while (*(helper_ptr1) == ' ' || *(helper_ptr1) == 9) //ignores empty spaces
					{
						helper_ptr1--;
					}
					helper_ptr1++;
				}
				*helper_ptr1 = '\0';
				convert_register_to_hexa(helper_ptr3, digits); //fourth digit
				strcpy(lines[ind], digits);
				ind++;
				while (*(helper_ptr2) == ' ' || *(helper_ptr2) == 9)
				{
					helper_ptr2++;
				}
				helper_ptr0 = helper_ptr2;
				while (*helper_ptr0 != ' ' && *helper_ptr0 != 9 && *helper_ptr0 != '\n')
				{
					helper_ptr0++;
				}
				if (*helper_ptr0 == ' ' || *helper_ptr0 == 9 || *helper_ptr0 == '\n')
					*(helper_ptr0) = '\0'; 
				strcpy(lines[ind], analyzeImm(helper_ptr2, labels, num_of_labels, x));
				ind++;
			}

		}
	}
	return ind;
}





int hexadecimalToDecimal(char hexVal[])
{
	int len = strlen(hexVal);

	// Initializing base value to 1, i.e 16^0
	int base = 1;

	int dec_val = 0;

	// Extracting characters as digits from last character
	for (int i = len - 1; i >= 0; i--)
	{
		// if character lies in '0'-'9', converting 
		// it to integral 0-9 by subtracting 48 from
		// ASCII value.
		if (hexVal[i] >= '0' && hexVal[i] <= '9')
		{
			dec_val += (hexVal[i] - 48)*base;

			// incrementing base by power
			base = base * 16;
		}

		// if character lies in 'A'-'F' , converting 
		// it to integral 10 - 15 by subtracting 55 
		// from ASCII value
		else if (hexVal[i] >= 'A' && hexVal[i] <= 'F')
		{
			dec_val += (hexVal[i] - 55)*base;

			// incrementing base by power
			base = base * 16;
		}
	}

	return dec_val;
}


int uppercase(char *sPtr)
{
	//converts each lower char in to string to upper

	while (*sPtr != '\0')
	{
		*sPtr = toupper((unsigned char)*sPtr);
		++sPtr;
	}
}



void WriteToFile(FILE* fPtrWrite, int num_of_elements)
{
	//writes the memory in the mem file 

	int ind = 0;

	for (ind = 0; ind < num_of_elements; ind++)
	{
		fprintf(fPtrWrite, "%s\n", lines[ind]);
	}

}



int main(int argc, char* argv[])
{
	int num_of_labels = 0, ind = 0, address_of_label_int = 0, num_of_elements = 0;
	char *helper_ptr = NULL;
	char x[10];

	FILE *fPtrRead;
	FILE *fPtrWrite;

	fPtrRead = fopen(argv[1], "r");
	fPtrWrite = fopen(argv[2], "w");
	if (fPtrRead == NULL || fPtrWrite == NULL) //if there is a oroblem with opening the files
	{
		printf("There was error using files\n");
		exit(1);
	}
	for (ind = 0; ind < MAX_NUM_OF_LINES; ind++)
	{
		strcpy(lines[ind], "0000"); //initialize the array
	}

	num_of_labels = first_search(fPtrRead);

	rewind(fPtrRead);
	num_of_elements = second_search(fPtrRead, num_of_labels, helper_ptr, x);
	WriteToFile(fPtrWrite, MAX_NUM_OF_LINES);
	fclose(fPtrRead);
	fclose(fPtrWrite);
	return 0;

}
