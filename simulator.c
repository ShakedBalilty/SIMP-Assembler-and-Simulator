#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define NUMBER_OF_LINES_MEMO 65536
#define NUMBER_OF_REGISTERS 16
#define CHARS_IN_LINE 6  
#define _CRT_SECURE_NO_WARNINGS
int meminLines[NUMBER_OF_LINES_MEMO]; // the memory with all the operations and words							  
signed int registers[NUMBER_OF_REGISTERS]; // registers array
										 
void createFromFile(FILE *fPtrReadMemin) { 
	//create a new array of int numbers named meminLines, with the lines of memin text file
	//gets pointers for the file memin and for the array meminLines
	int i = 0;
	char temp[CHARS_IN_LINE]; 
	
	while (fgets(temp, sizeof temp, fPtrReadMemin) != NULL)
	{
		meminLines[i] = (int)strtol(temp, NULL, 16);
		i++;
	}
}

void defineWords(int memcurrline, int* pbopcode, int* pbrd, int* pbrs, int* pbrt)
{
	//copy opcode, rd, rs, rt
	int mask = 0xf000;
	*pbopcode = (int)(memcurrline & mask);
	*pbopcode >>=12;
	mask = 0x0f00;
	*pbrd = (int)(memcurrline & mask);
	*pbrd >>= 8;
	mask = 0x00f0;
	*pbrs = (int)(memcurrline & mask);
	*pbrs >>= 4;
	mask = 0x000f;
	*pbrt = (int)(memcurrline & mask);
}

void writeToFiles(FILE* fPtrWriteMemout, FILE* fPtrWriteRegout, FILE* fPtrWriteCount,int opSum)
{
	int i;
	//write to the Memout
	for (i=0; i<NUMBER_OF_LINES_MEMO;i++)
		fprintf(fPtrWriteMemout, "%04X\n", meminLines[i]);
	//write to Memout Regout 
	for (i = 2; i<NUMBER_OF_REGISTERS; i++)
		fprintf(fPtrWriteRegout, "%08X\n", registers[i]);
	//write to Count
	fprintf(fPtrWriteCount, "%d" ,opSum) ;
}

void writeToTrace(FILE* fPtrWriteTrace, int pc,int memcurrline, int isImm, signed int immediate)
{
	//write to the file Trace
	int maskImm = 0x0000ffff;//mask for the immediate that comes with 8 hexa digits
	if(isImm==0)
	{ 
	fprintf(fPtrWriteTrace, "%08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X\n",
		pc, memcurrline, registers[0], registers[1], registers[2], registers[3], registers[4], registers[5], registers[6], registers[7], registers[8],
		registers[9], registers[10], registers[11], registers[12], registers[13], registers[14], registers[15]);
	}
	else
	{
		immediate = maskImm & immediate;
		fprintf(fPtrWriteTrace, "%08X %04X%04X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X\n",
			pc, immediate, memcurrline, registers[0], registers[1], registers[2], registers[3], registers[4], registers[5], registers[6], registers[7], registers[8],
			registers[9], registers[10], registers[11], registers[12], registers[13], registers[14], registers[15]);
	}
}

void opAdd(int brd, int brs, int brt)
{
	//opperation add, number 0
	registers[brd] = registers[brs] + registers[brt];
}

void opSub(int brd, int brs, int brt)
{
	//opperation subtract, number 1
	registers[brd] = registers[brs] - registers[brt];
}

void opAnd(int brd, int brs, int brt)
{
	//opperation AND, number 2
	registers[brd] = registers[brs]&registers[brt];
}

void opOr(int brd, int brs, int brt)
{
	//opperation OR, number 3
	registers[brd] = registers[brs] | registers[brt];
}

void opSll(int brd, int brs, int brt)
{
	//opperation shift left logical, number 4
	registers[brd] = registers[brs] << registers[brt];
}

void opSra(int brd, int brs, int brt)
{
	//opperation shift right logical, number 5
	registers[brd] = registers[brs] >> registers[brt];
}

void opReserved(int brd, int brs, int brt)
{
	//opperation reserved, number 6
}

void opBeq(int brd, int brs, int brt, int* pointPc, int isImm)
{
	//opperation branch if the two are equal, number 7
	int mask = 0x0000ffff;
	if (registers[brs] == registers[brt])
	{
		*pointPc = registers[brd] & mask;//bitwise for 16 lsb of brd
	}
	else {
		if (isImm = 1)
			*pointPc = *pointPc + 2;
		else
			*pointPc = *pointPc + 1;
	}
}

void opBgt(int brd, int brs, int brt, int* pointPc, int isImm)
{
	//opperation branch if rs>rt, number 8
	int mask = 0x0000ffff;
	if (registers[brs] > registers[brt])
		*pointPc = registers[brd] & mask;//bitwise for 16 lsb of brd
	else {
		if (isImm = 1)
			*pointPc = *pointPc + 2;
		else
			*pointPc = *pointPc + 1;
	}

}

void opBle(int brd, int brs, int brt, int* pointPc, int isImm)
{
	//opperation branch if rs<=rt, number 9
	int mask = 0x0000ffff;
	if (registers[brs] <= registers[brt])
		*pointPc = registers[brd] & mask;//bitwise for 16 lsb of brd
	else {
		if (isImm = 1)
			*pointPc = *pointPc + 2;
		else
			*pointPc = *pointPc + 1;
	}
}

void opBne(int brd, int brs, int brt, int* pointPc, int isImm)
{
	//opperation branch if rs!=rt, number 10
	int mask = 0x0000ffff;
	if (registers[brs] != registers[brt])
		*pointPc = registers[brd] & mask;//bitwise for 16 lsb of brd
	else {
		if (isImm = 1)
			*pointPc = *pointPc + 2;
		else
			*pointPc = *pointPc + 1;
	}
}

void opJal(int brd, int brs, int brt, int* pointPc)
{
	//opperation jump to rd, number 11

	int mask = 0x0000ffff;
	if(brd==1||brs==1||brt==1)//if the operation called includes immediate
		registers[15] = *pointPc + 2;
	else
		registers[15] = *pointPc + 1; // when jumping to label and not to immediate
	*pointPc = registers[brd] & mask;//bitwise for 16 lsb of brd
}

void opLw(int brd, int brs, int brt, int* pointPc)
{
	//opperation load word from memory into register, number 12
	signed int inside = registers[brs] + registers[brt];
	int word = meminLines[inside];
	int mask = 0xf000;
	int msb = (int)(word & mask);
	msb >>= 12;
	if (msb >= 8) {//the number is negative
		registers[brd] = 0xffff0000 + word;
	}
	else {//the number is positive
		registers[brd] = 0x00000000 + word;
	}
}

void opSw(int brd, int brs, int brt, int* pointPc)
{
	//opperation store word to memory, number 13
	int mask = 0x0000ffff;
	int word = registers[brd] & mask;//bitwise for 16 lsb
	meminLines[registers[brs] + registers[brt]]=word;
}

void opLhi(int brd, int brs, int brt, int* pointPc)
{
	//opperation lhi, number 14
	signed int maskless = 0x0000ffff;
	signed int temp2 = registers[brs] & maskless;
	temp2=temp2 << 16;
	signed int temp1 = registers[brd] & maskless;
	registers[brd] = temp1 + temp2;
}

void opHalt() {

}

void simulateProg(int bopcode, int brd, int brs, int brt, int* pointPc, int isImm)
{
	//simulate by choosing the operation and if needed use immediate
	int temp_pc;
	if (isImm == 1)//check how to update pc
		temp_pc = *pointPc + 2;
	 if(isImm == 0){
		temp_pc = *pointPc+1;
	}
	if (bopcode == 0){
		opAdd(brd, brs, brt);
		*pointPc = temp_pc;
	}
	if (bopcode == 1){
		opSub(brd, brs, brt);
		*pointPc = temp_pc;
	}
	if (bopcode == 2){
		opAnd(brd, brs, brt);
		*pointPc = temp_pc;
	}
	if (bopcode == 3){
		opOr(brd, brs, brt);
		*pointPc = temp_pc;
	}
	if (bopcode == 4){
		opSll(brd, brs, brt);
		*pointPc = temp_pc;
	}
	if (bopcode == 5){
		opSra(brd, brs, brt);
		*pointPc = temp_pc;
	}
	if (bopcode == 6){
		opReserved(brd, brs, brt);
		*pointPc = temp_pc;
	}
	if (bopcode == 7)
		opBeq(brd, brs, brt, pointPc, isImm);
	if (bopcode == 8)
		opBgt(brd, brs, brt, pointPc, isImm);
	if (bopcode == 9)
		opBle(brd, brs, brt, pointPc, isImm);
	if (bopcode == 10)
		opBne(brd, brs, brt, pointPc, isImm);
	if (bopcode == 11)
		opJal(brd, brs, brt, pointPc);
	if (bopcode == 12){
		opLw(brd, brs, brt, pointPc);
		*pointPc = temp_pc;
	}
	if (bopcode == 13){
		opSw(brd, brs, brt, pointPc);
		*pointPc = temp_pc;
	}
	if (bopcode == 14){
		opLhi(brd, brs, brt, pointPc);
		*pointPc = temp_pc;
	}
	if (bopcode == 15){
		opHalt();
		*pointPc = temp_pc;
	}
}

/***************************************************************************************************************end of functions*/ 

int main(int argc, char* argv[]) /////////////////////real main
{
	FILE*fPtrReadMemin, *fPtrWriteMemout, *fPtrWriteRegout, *fPtrWriteTrace, *fPtrWriteCount;
	fPtrReadMemin = fopen(argv[1], "r"); //open Memin to read
	fPtrWriteMemout = fopen(argv[2], "w"); //open Memout to write
	fPtrWriteRegout = fopen(argv[3], "w"); //open Regout to write
	fPtrWriteTrace = fopen(argv[4], "w"); //open Trace to write
    fPtrWriteCount = fopen(argv[5], "w"); //open count to count
	int pc = 0;
	int deb = 0;
	int* pointPc=&pc;
	int i; int mask; int msb; int memcurrline; int bopcode, brd, brs, brt;

	int *pbopcode = &bopcode;
	int *pbrd = &brd;
	int *pbrs = &brs;
	int *pbrt = &brt;
	signed int immediate; //immediate value
	int isImm = 0; //0 for non immediate operation, 1 for immediate operation
	int opSum = 0; //num of operations executed
	
	for (i = 0; i < NUMBER_OF_REGISTERS; i++)// initialize array of registers
		registers[i]=0x0;
	if (fPtrReadMemin == NULL || fPtrWriteMemout == NULL || fPtrWriteRegout == NULL || fPtrWriteTrace == NULL || fPtrWriteCount == NULL) { //check if the files were open correctly
		printf("There was error using files\n");
		fclose(fPtrReadMemin);//close all files
		fclose(fPtrWriteMemout);
		fclose(fPtrWriteRegout);
		fclose(fPtrWriteTrace);
		fclose(fPtrWriteCount);
		return 1;
	}
	
	createFromFile(fPtrReadMemin); //create the memory
	while (pc < NUMBER_OF_LINES_MEMO && bopcode != 0xf)
	{
		isImm = 0;
		memcurrline = meminLines[pc];// copy line of opertaion to int memcurrline
		defineWords(memcurrline, pbopcode, pbrd, pbrs, pbrt);
		if (brd == 1 || brs == 1 || brt == 1 &&pc<NUMBER_OF_LINES_MEMO-1) {//check if this is an immediate operation and if so make sign extention
			isImm = 1; 
			immediate= meminLines[pc+1];
			mask = 0xf000;
			msb = (int)(immediate & mask);
			msb >>= 12;
			if (msb >= 8) {//the immediate number is negative
				immediate = 0xffff0000 + immediate;
			}
			else{
				immediate = 0x00000000 + immediate;
			}
		}

		if(isImm==1)
			registers[1] = immediate;
		else if (isImm == 0)
			registers[1] = 0x00000000;

		writeToTrace(fPtrWriteTrace, pc, memcurrline, isImm, immediate);// write to trace before simulation
		simulateProg(bopcode, brd, brs, brt, pointPc, isImm); //run the operation of this line
		opSum++;
	}
		
	writeToFiles(fPtrWriteMemout, fPtrWriteRegout, fPtrWriteCount, opSum); //write to memout, regout, count
	fclose(fPtrReadMemin);//close all files
	fclose(fPtrWriteMemout);
	fclose(fPtrWriteRegout);
	fclose(fPtrWriteTrace);
	fclose(fPtrWriteCount);
	return 0;
}
