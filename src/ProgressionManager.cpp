/*
 * ProgressionManager.cpp
 *
 *  Created on: May 15, 2011
 *      Author: derrickstolee
 */

#include <stdlib.h>
#include <string.h>

#include "SearchManager.hpp"

#include "ProgressionTable.hpp"
#include "ProgressionManager.hpp"

/**
 * ProgressionManager constructor
 */
ProgressionManager::ProgressionManager(progression_mode mode, int k, int d, int minN, int maxN)
{
	this->maxSolSize = 0;
	this->haltAtSolutions = false;
	this->mode = mode;
	this->k = k;
	this->d = d;
	this->last_push = true;
	this->minN = minN;
	this->maxN = maxN;

	this->table = new ProgressionTable(mode, k, d, minN, maxN);
}

/**
 * Destructor
 */
ProgressionManager::~ProgressionManager()
{
	delete this->table;
}

/**
 * pushNext -- deepen the search to the next child
 * 	of the current node.
 *
 * @return the label for the new node. -1 if none.
 */
LONG_T ProgressionManager::pushNext()
{
	/* we need to push to a certain point */
	LONG_T child = 2;
	SearchNode* node = 0;

	if ( this->stack.size() == 0 )
	{
		node = this->root;
	}
	else
	{
		node = this->stack.back();
	}

	if ( this->table->getN() >= this->maxN )
	{
		return -1;
	}

	child = node->curChild;
	child++;
	node->curChild = child;

	if ( child >= 2 )
	{
		return -1;
	}

	return this->pushTo(child);
}

/**
 * pushTo -- deepen the search to the specified child
 * 	of the current node.
 *
 * @param child the specified label for the new node
 * @return the label for the new node. -1 if none, or failed.
 */
LONG_T ProgressionManager::pushTo(LONG_T child)
{
	//	printf("--pushTo: %llX\n", child);

	if ( child < 0 || child >= 2 )
	{
		return -1;
	}

	if ( this->table->getN() >= this->maxN )
	{
		this->last_push = false;
		this->stack.push_back(new SearchNode(child));
		return child;
	}

	this->table->snapshot();

	SearchNode* node = 0;

	if ( this->stack.size() == 0 )
	{
		node = this->root;
	}
	else
	{
		node = this->stack.back();
	}

	node->curChild = child;

	int j = this->stack.size(); // this->table->getN();

	last_push = this->table->setColor(j, child, true);

	this->stack.push_back(new SearchNode(child));

	return child;
}

/**
 * pop -- remove the current node and move up the tree.
 *
 * @return the label of the node after the pop.
 * 		This return value is used for validation purposes
 * 		to check proper implementation of push*() and pop().
 */
LONG_T ProgressionManager::pop()
{
	if ( this->stack.size() > 0 )
	{
		SearchNode* node = this->stack.back();
		this->stack.pop_back();
		this->table->rollback();

		LONG_T child = node->label;

		delete node;
		node = 0;

		return child;
	}

	return 0;
}

/**
 * prune -- Perform a check to see if this node should be pruned.
 *
 * @return 0 if no prune should occur, 1 if prune should occur.
 */
int ProgressionManager::prune()
{
	//	this->table->print();

	if ( this->last_push == false )
	{
		return 1;
	}
	else if ( this->table->hasProgression() )
	{
		return 1;
	}

	return 0;
}

/**
 * isSolution -- Perform a check to see if a solution exists
 * 		at this point.
 *
 * @return 0 if no solution is found, 1 if a solution is found.
 */
int ProgressionManager::isSolution()
{
	if ( this->table->getN() >= this->minN && this->table->getN() >= this->maxSolSize && this->table->hasProgression()
			== false )
	{
		this->maxSolSize = this->table->getN();
		return 1;
	}

	return 0;
}

/**
 * writeSolution -- create a buffer that contains a
 * 	description of the solution.
 *
 * Solution strings start with S.
 *
 * @return a string of data allocated with malloc().
 * 	It will be deleted using free().
 */
char* ProgressionManager::writeSolution()
{
	/* write the solution */
	printf("\nn = %d\n", this->table->getN());

	for ( int i = 0; i < this->table->getN(); i++ )
	{
		int c = this->table->getColor(i);
		int num_block = 1;

		while ( i + 1 < this->table->getN() && this->table->getColor(i + 1) == c )
		{
			i++;
			num_block++;
		}

		printf(" %d^{%d} ", c, num_block);
	}
	printf("\n\n");

	//	this->table->print();

	return 0;
}

/**
 * writeStatistics -- create a buffer that contains a
 * 	description of the solution.
 *
 * Statistics take the following format in each line:
 *
 * T [TYPE] [ID] [VALUE]
 *
 * @return a string of data allocated with malloc().
 * 	It will be deleted using free().
 */
char* ProgressionManager::writeStatistics()
{
	char* buffer = this->writeBaseStats();

	char* buffer2 = (char*) malloc(strlen(buffer) + 200);

	sprintf(buffer2, "%s\nT MAX BIGGEST_N %d\nT SUM NUM_SOLUTIONS %d\n", buffer, this->maxSolSize, this->foundSolutions);

	free(buffer);


	return buffer2;
}

void ProgressionManager::reset()
{
	this->maxSolSize = 0;
	this->foundSolutions = 0;
	this->table->reset();
}

/**
 * setBackwardPropagation()
 */
void ProgressionManager::setBackwardPropagation(bool val)
{
	this->table->setBackwardPropagation(val);
}

/**
 * setForwardPropagation()
 */
void ProgressionManager::setForwardPropagation(bool val)
{
	this->table->setForwardPropagation(val);
}
