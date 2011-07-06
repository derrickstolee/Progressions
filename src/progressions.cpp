/*
 * progressions.cpp
 *
 *  Created on: May 15, 2011
 *      Author: derrickstolee
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "ProgressionManager.hpp"

int main(int argc, char** argv)
{
	ProgressionManager* manager = 0;

	bool FORWARD_MODE = false;
	bool BACKWARD_MODE = false;

	progression_mode mode = QUASI_PROGRESSION;
	int k = 0, d = 0, minN = 0, maxN = 0;

	for ( int i = 1; i < argc; i++ )
	{
		if ( argv[i][0] == '-' && i < argc - 1 )
		{
			switch ( argv[i][1] )
			{
			case 'K':
				k = atoi(argv[i + 1]);
				break;

			case 'D':
				d = atoi(argv[i + 1]);
				break;

			case 'I':
				d = k - atoi(argv[i + 1]);
				break;

			case 'n':
				minN = atoi(argv[i + 1]);
				break;

			case 'N':
				maxN = atoi(argv[i + 1]);
				break;

			case '-':
				if ( strcmp(argv[i], "--mode") == 0 )
				{
					if ( strcmp(argv[i + 1], "quasi") == 0 )
					{
						mode = QUASI_PROGRESSION;
					}
					else if ( strcmp(argv[i + 1], "pseudo") == 0 )
					{
						mode = PSEUDO_PROGRESSION;
					}
				}
				else if ( argv[i][1] == '-' )
				{
					if ( strcmp(argv[i],"--forward") == 0 )
					{
						if ( strcmp(argv[i+1],"on") == 0 )
						{
							FORWARD_MODE = true;
						}
					}
					else if ( strcmp(argv[i],"--backward") == 0 )
					{
						if ( strcmp(argv[i+1],"on") == 0 )
						{
							BACKWARD_MODE = true;
						}

					}
				}
			}
		}
	}

	manager = new ProgressionManager(mode, k, d, minN, maxN);

	manager->importArguments(argc, argv);

	manager->setBackwardPropagation(BACKWARD_MODE);
	manager->setForwardPropagation(FORWARD_MODE);

	while ( manager->readJob(stdin) >= 0 )
	{
		manager->doSearch();
		manager->reset();
	}

	delete manager;

	return 0;
}
