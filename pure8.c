/*   DO NOT REMOVE !!!
 *
 *   pure8 virtual computer SemperFis
 *   Copyright (C) 2019 
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *   Also add information on how to contact you by electronic and paper mail.
 */

#include <stdlib.h>
#include <stdio.h>

#define REGS_NUM 0x8


/* The pure8 virtual computer system is : 
 * - 64Kb of RAM, 
 * - 8 16bits general-purposes registers (R0 to R7), 
 * - 1 16bits program-counter (PC)
 */
typedef struct {
	unsigned char  mem[0x1000];
	unsigned short regs[REGS_NUM];
	unsigned short pc;
} pure8_t;


/* Store the instructions into an Enum to ease the code reading */
enum intruction_list {
	/* HALT : halt the cpu until reset */
	HALT = 0xFF,
	
	/* LOAD : load an immediate value into a register or copy the content of a register into another register */
	LOAD = 0x01,
};


/* The pure8 main function that contain all the code, there isn't external function to ease the code reading and support */
int main(int argc, char* argv[])
{
	/* GNU/GPL License output. DO NOT REMOVE !!! */
	char* GNU_GPL = 	"\nPure8 Virtual Computer  Copyright (C) 2019 SemperFis\n"
				"This program comes with ABSOLUTELY NO WARRANTY;\n"
				"This is free software, and you are welcome to redistribute it\n"
				"under certain conditions;\n";
	puts(GNU_GPL);
	
	#ifdef DEBUG
		/* Create the debug file for dev purpose only */
		FILE* debug_file = fopen("debug.txt", "w+");
	#endif
	
	
	/* Create an instance of the pure8 vm & initialize it */
	pure8_t pure8 = { .mem = { 0 }, .regs = { 0 }, .pc =  0 };
	
	
	/* Test the number of command-line arguments & open the rom file */
	if (argc != 2)
		printf("Please use the following syntax : pure8 [binary file]\n");
	else
	{
		/* Open the rom file as a binary */
		FILE* rom_file = NULL;
		rom_file = fopen(argv[1], "rb");
		
		/* Test if the rom file isn't correctly open */
		if (rom_file == NULL)
		{
			printf("Unable to open %s !\n", argv[1]);
			exit(-1);
		}
		
		/* Finally, copy the rom content into the pure8 RAM */
		int i = -1;
		while ((int)pure8.mem[i] != 0xFF)
		{
			i++;
			fread(&pure8.mem[i], (size_t)1, (size_t)1, rom_file);
		}
		
		/* And close the file */
		fclose(rom_file);
	}
	
	/* -------------------------------------------------------------------- */
	/* Now, execute the rom content load into the pure8 RAM */
	
	/* Declare variable to handle the different instruction field */
	unsigned char inst, conf, dest_low, dest_high, src_low, src_high =  0;
	unsigned short tmp = 0, run = 1;
	
	#ifdef DEBUG
		fprintf(debug_file, "\nEXECUTE INSTRUCTION IN THE RAM\n", pure8.pc);
	#endif
	
	while (run)
	{
		/* Load the current instruction from the memory location pointed by the pure8 PC */
		inst = pure8.mem[pure8.pc];		

		switch (inst)
		{
			case HALT:
				/* HALT */
				/* ENCODING : 0xFF */

				#ifdef DEBUG
					fprintf(debug_file, "\n>>> HALT at RAM address %X\n", pure8.pc);
				#endif
				
				run = 0;
				break;
				
			case LOAD:
				/* LOAD [DEST] [SRC] | Example : LOAD M0, #0xFF, LOAD R5, M0 */
				/* ENCODING : 0x01[CONF][DEST][SRC] */
				
				conf = pure8.mem[++pure8.pc];
				
				#ifdef DEBUG
					fprintf(debug_file, "\n>>> LOAD at RAM address %X\n", (int)pure8.pc);
				#endif
				
				/* CONF(iguration) BYTE : 
				 * 0x10 -> DEST = REG, SRC = IMM;
				 * 0x11 -> DEST = REG, SRC = REG;
				 */
				
				switch (conf)
				{
					case 0x10:
						/* DEST = REG, SRC = IMM */
						
						dest_high = pure8.mem[++pure8.pc];
						src_high  = pure8.mem[++pure8.pc];
						src_low   = pure8.mem[++pure8.pc];
						
						/* Concatenate the two char src_low & src_high to make a short */
						tmp = src_high << 8;
						tmp = tmp | (unsigned short)src_low;

						pure8.regs[dest_high] = tmp;
						break;
						
					case 0x11:
						/* DEST = REG, SRC = REG */

						dest_high = pure8.mem[++pure8.pc];
						src_high  = pure8.mem[++pure8.pc];

						pure8.regs[dest_high] = pure8.regs[src_high];
						break;
				}
				
				break;
				
			default:
				printf("Unknown instruction code : 0x%X at ram 0x%X\n", (int)inst, (int)pure8.pc);
				break;
		}
		
		/* Dump all the registers into the debug file */
		#ifdef DEBUG
			fprintf(debug_file, "\nREGISTERS DUMP\n\n");
			for (register int i = 0; i < REGS_NUM; i++)
				fprintf(debug_file, "REGISTER R%d : 0x%X\n", i, (int)pure8.regs[i]);
		#endif
		
		/* Increment the pure8 PC */
		pure8.pc++;
	}
	
	/* End of the pure8 virtual computer */
	return 0;
}
