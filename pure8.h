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

/* To get the input file size */
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>


/* The pure8 virtual computer system is : 
 * - 64Kb of RAM, 
 * - 64Kb of byte-aligned stack,
 * - 16bits program-counter (PC)
 */
typedef struct {
	unsigned char* ram;
	unsigned char* stack;
	unsigned short pc;
	unsigned char run;
} pure8_t;


/* Store the instructions into an Enum to ease the code reading */
enum intruction_list {
	/* HALT : halt the cpu until reset */
	HALT = 0xFF,
};


void read_file(const char* file, pure8_t* p)
{
	struct stat file_stat;

	if (stat(file, &file_stat) < 0)
	{
		fprintf(stderr, "Can't get the size of %s !!!\n", file);
		exit(-1);
	}

	#ifdef DEBUG
		fprintf(stderr, "The size of %s is : %ld byte(s).\n", file, file_stat.st_size);
	#endif

	FILE* f = fopen(file, "rb");
	if (!f)
	{
		fprintf(stderr, "Can't open %s !!!\n", file);
		exit(-1);
	}

	unsigned int size = fread(p->ram, (size_t)1, (size_t)file_stat.st_size, f);

	#ifdef DEBUG
		fprintf(stderr, "Size of data read by fread() : %d byte(s).\n", size);
	#endif

	fclose(f);
}


pure8_t* pure8_init(void)
{
	pure8_t* s = calloc(1, sizeof(pure8_t));

	if (!s) { goto alloc_failed; }

	s->ram = calloc(0x10000, sizeof(unsigned char));
	if (!s->ram) { free(s); goto alloc_failed; }

	s->stack = calloc(0x10000, sizeof(unsigned char));
	if (!s->stack) { free(s->ram); free(s); goto alloc_failed; }

	s->pc  = 0;
	s->run = 1;

	return s;
	
	alloc_failed:
		fprintf(stderr, "Can't allocate memory for the pure8 instance !!!\n");
		exit(-1);
}


void pure8_free(pure8_t* s)
{
	free(s->stack);
	free(s->ram);
	free(s);
}


void pure8_core(pure8_t* vm)
{
	unsigned char inst   = HALT;
	unsigned char h_inst = 0;

	while (vm->run)
	{
		inst   = vm->ram[vm->pc];
		h_inst = inst >> 4;

		switch(inst)
		{
			case HALT:
				/* Stop the CPU */

				#ifdef DEBUG
					fprintf(stderr, "HALT at 0x%X\n", vm->pc);
				#endif
				vm->run = 0;
				break;

			default:
				fprintf(stderr, "At 0x%X : Unknown instruction : 0x%X !!!\n", vm->pc, inst);
				vm->run = 0;
				break;
		}
	}
}
