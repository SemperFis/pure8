/*   DO NOT REMOVE !!!
 *
 *   My pure8 virtual computer
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
#include <stdbool.h>

#define REGS_NUM 9


/* The pure8 virtual computer system is : 
 * - 64Kb of RAM, 
 * - 8 8bits general-purposes registers (R0 to R7), 
 * - 1 16bits memory-purpose register (M0),
 * - 1 16bits program-counter (PC)
 */
typedef struct {
	unsigned char  mem[0x1000];
	unsigned char  regs[REGS_NUM];
	unsigned short pc;
} _pure8;


/* Store the instructions into an Enum to ease the code reading */
enum intruction_list {
	/* HALT : stop the cpu until reset */
	HALT = 0xFF,
	
	/* MOV : move an immediate value, a register or a memory address to a register or a memory address */
	MOV = 0x01
};


/* The pure8 main function that contain all the code, there isn't external function to ease the code reading and support */
int main(int argc, char* argv[])
{
	/* The GNU/GPL license screen, DO NOT REMOVE */
	printf("\nPure8 Virtual Computer  Copyright (C) 2019 Evan Dondon\nThis program comes with ABSOLUTELY NO WARRANTY;\nThis is free software, and you are welcome to redistribute it\nunder certain conditions;\n\n");
	
	#ifdef DEBUG
		/* Create the debug file for dev purpose only */
		FILE* debug_file = fopen("debug.txt", "w+");
	#endif
	
	
	/* Create an instance of the pure8 vm & initialize it */
	_pure8 pure8 = { .mem = { 0 }, .regs = { 0 }, .pc =  0 };
	
	
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
	unsigned char inst = 0xFF;
	unsigned char conf = 0;
	unsigned char dest = 0;
	unsigned char src  = 0;
	unsigned char tmp  = 0;
	int run = 1;
	
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
				
				run = false;
				break;
				
			case MOV:
				/* MOV [DEST] [SRC] | Example : LOAD M0, #0xFF, LOAD R5, M0 */
				/* ENCODING : 0x01[CONF][DEST][SRC] */
				
				conf = pure8.mem[++pure8.pc];
				dest = pure8.mem[++pure8.pc];
				src  = pure8.mem[++pure8.pc];
				
				#ifdef DEBUG
					fprintf(debug_file, "\n>>> MOV at RAM address %X : CONF : %X; DEST : %X; SRC : %X;\n", (int)pure8.pc, (int)conf, (int)dest, (int)src);
				#endif
				
				/* CONF(iguration) BYTE : 
				 * 0x10 -> DEST = REG, SRC = IMM,
				 * 0x11 -> DEST = REG, SRC = REG,
				 * 0x12 -> DEST = REG, SRC = MEM,
				 * 0x21 -> DEST = MEM, SRC = REG,
				 * 0x20 -> DEST = MEM, SRC = IMM
				 */
				
				switch (conf)
				{
					case 0x10:
						/* DEST = REG, SRC = IMM */
						
						if (dest >= 0x0 && dest < REGS_NUM)
							pure8.regs[dest] = (unsigned char)src;
						else 
						{
							printf("Error at %X : no valid register for the destination field of MOV !\n", pure8.pc-2);
						}
						break;
						
					case 0x11:
						/* DEST = REG, SRC = REG */
						
						if (dest >= 0x0 && dest < REGS_NUM && src >= 0x0 && src < REGS_NUM)
								pure8.regs[dest] = pure8.regs[src];
						else 
						{
							printf("Error at address %X : no valid register for the destination or source field of MOV !\n", pure8.pc-2);
						}
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
				fprintf(debug_file, "REGISTER %d : 0x%X\n", i, (int)pure8.regs[i]);
		#endif
		
		/* Increment the pure8 PC */
		pure8.pc++;
	}
	
	/* End of the pure8 virtual computer */
	return 0;
}