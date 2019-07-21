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
#include "pure8.h"


int main(int argc, char* argv[])
{
	/* GNU/GPL License output. DO NOT REMOVE !!! */
	char* GNU_GPL = "\nPure8 Virtual Computer  Copyright (C) 2019 SemperFis\n"
			"This program comes with ABSOLUTELY NO WARRANTY;\n"
			"This is free software, and you are welcome to redistribute it\n"
			"under certain conditions;\n";
	puts(GNU_GPL);
	
	
	/* Create an instance of the pure8 vm & initialize it */
	pure8_t* pure8 = pure8_init();
	
	/* Test the number of command-line arguments */
	if (argc != 2) 
	{
		printf("Please use the following syntax : pure8 [binary file]\n");
		pure8_free(pure8);
		return -1;
	}
	else
	{
		read_file((const char*)argv[1], pure8);
	}
	
	
	#ifdef DEBUG
		fprintf(stderr, "Start executing instruction...\n");
	#endif
	pure8_core(pure8);


	#ifdef DEBUG
		fprintf(stderr, "Free the pure8 instance & exit the program...\n");
	#endif

	/* Free the pure8 & quit */
	pure8_free(pure8);
	return 0;
}