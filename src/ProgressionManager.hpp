/*
 * ProgressionManager.hpp
 *
 *  Created on: May 15, 2011
 *      Author: derrickstolee
 */

#ifndef PROGRESSIONMANAGER_HPP_
#define PROGRESSIONMANAGER_HPP_

#include "ProgressionTable.hpp"
#include "SearchManager.hpp"


/**
 * ProgressionManager is the SearchManager for the Progressions project,
 * searching for quasiprogressions and pseudoprogressions.
 */
class ProgressionManager: public SearchManager
{
protected:
	/**
	 * mode This is the mode we are considering.
	 */
	progression_mode mode;

	/**
	 * d This is the diameter of the progression.
	 */
	int d;

	/**
	 * k is the length of the progressions we are looking for.
	 */
	int k;

	/**
	 * minN is the minimum number of colored points we need
	 * 	before outputting the solution.
	 */
	int minN;

	/**
	 * maxN is the maximum number of points we need to consider for
	 * 	propagating the colors. This should be a known upper bound
	 * 	on the progression number.
	 */
	int maxN;

	int maxSolSize;

	bool last_push;

	/* a progression table stores */
	ProgressionTable* table;

public:
	/**
	 * ProgressionManager constructor
	 */
	ProgressionManager(progression_mode mode, int k, int d, int minN, int maxN);

	/**
	 * Destructor
	 */
	virtual ~ProgressionManager();

	/**
	 * pushNext -- deepen the search to the next child
	 * 	of the current node.
	 *
	 * @return the label for the new node. -1 if none.
	 */
	virtual LONG_T pushNext();

	/**
	 * pushTo -- deepen the search to the specified child
	 * 	of the current node.
	 *
	 * @param child the specified label for the new node
	 * @return the label for the new node. -1 if none, or failed.
	 */
	virtual LONG_T pushTo(LONG_T child);

	/**
	 * pop -- remove the current node and move up the tree.
	 *
	 * @return the label of the node after the pop.
	 * 		This return value is used for validation purposes
	 * 		to check proper implementation of push*() and pop().
	 */
	virtual LONG_T pop();

	/**
	 * prune -- Perform a check to see if this node should be pruned.
	 *
	 * @return 0 if no prune should occur, 1 if prune should occur.
	 */
	virtual int prune();

	/**
	 * isSolution -- Perform a check to see if a solution exists
	 * 		at this point.
	 *
	 * @return 0 if no solution is found, 1 if a solution is found.
	 */
	virtual int isSolution();

	/**
	 * writeSolution -- create a buffer that contains a
	 * 	description of the solution.
	 *
	 * Solution strings start with S.
	 *
	 * @return a string of data allocated with malloc().
	 * 	It will be deleted using free().
	 */
	virtual char* writeSolution();

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
	virtual char* writeStatistics();

	void reset();


	/**
	 * setBackwardPropagation()
	 */
	 void setBackwardPropagation(bool val);

	/**
	 * setForwardPropagation()
	 */
	void setForwardPropagation(bool val);



};

#endif /* PROGRESSIONMANAGER_HPP_ */
