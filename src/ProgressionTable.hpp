/*
 * ProgressionTable.hpp
 *
 *  Created on: May 15, 2011
 *      Author: derrickstolee
 */

#ifndef PROGRESSIONTABLE_HPP_
#define PROGRESSIONTABLE_HPP_

#include <stack>


typedef enum progression_enum
{
	QUASI_PROGRESSION, PSEUDO_PROGRESSION
} progression_mode;

class ProgressionTable
{
protected:
	progression_mode mode;

	int k;
	int d;
	int minN;
	int minSolN;
	int maxN;
	int curN;

	bool use_backward_propagation;
	bool use_forward_propagation;

	int maxL;

	bool has_progression;

	char* colors;

	char* implied_colors;

	char** quasi_table;

	char* forward_table;
	char* backward_table;


	/**
	 * given the set of parameters (in Pseudo mode), return the size of the forward_table and backward_table arrays
	 */
	int getPseudoTableSize(int nColors);

	/**
	 * given the set of inputs (in Pseudo mode), return the index for the forward_table and backward_table arrays.
	 */
	int getPseudoTableIndex(int color, int pos, int l, int dp);


	/**
	 * given the set of parameters (in Quasi mode), return the size of the forward_table and backward_table arrays
	 */
	int getQuasiTableSize(int nColors);

	/**
	 * given the set of inputs (in Quasi mode), return the index for the forward_table and backward_table arrays.
	 */
	int getQuasiTableIndex(int color, int pos, int l);

	/**
	 * setBackwardTable
	 *
	 * Set a position in the backward table, while updating the snapshot sizes. (Pseudo Mode)
	 */
	bool setBackwardTable(int color, int pos, int l, int dp, char value);

	/**
	 * setBackwardTable
	 *
	 * Set a position in the backward table, while updating the snapshot sizes. (Quasi Mode)
	 */
	bool setBackwardTable(int color, int pos, int l, char value);

	/**
	 * getBackwardTable
	 *
	 * Returns the value of the backward table. (Pseudo Mode)
	 */
	char getBackwardTable(int color, int pos, int l, int dp);

	/**
	 * getBackwardTable
	 *
	 * Returns the value of the backward table. (Quasi Mode)
	 */
	char getBackwardTable(int color, int pos, int l);

	/**
	 * setForwardTable
	 *
	 * Set a position in the forward table, while updating the snapshot sizes. (Pseudo Mode)
	 */
	bool setForwardTable(int color, int pos, int l, int dp, char value);

	/*
	 * setForwardTable
	 *
	 * Set a position in the forward table, while updating the snapshot sizes. (Quasi Mode)
	 */
	bool setForwardTable(int color, int pos, int l, char value);

	/**
	 * getForwardTable
	 *
	 * Returns the value of the forward table. (Pseudo Mode)
	 */
	char getForwardTable(int color, int pos, int l, int dp);

	/**
	 * getForwardTable
	 *
	 * Returns the value of the forward table. (Quasi Mode)
	 */
	char getForwardTable(int color, int pos, int l);

	/**
	 * snapshot_sizes stores the size of color_augmentations when each snapshot()
	 * 	method is called.
	 */
	int num_snapshots;
	int max_snapshots;
	int* snapshot_sizes;

	/**
	 * backward_sizes stores the size of backward_augmentations when each snapshot() method is called.
	 */
	int* backward_sizes;
	int* forward_sizes;

	/**
	 * backward_augmentations stores pairs (pushed as pos, prev_value) for
	 * 	modifying the backward_table
	 */

	void pushBackwardAugmentation(int pos, int val);
	void popBackwardAugmantation();

	int num_backward_augmentations;
	int max_backward_augmentations;
	int* backward_augmentations;

	void pushForwardAugmentation(int pos, int val);
	void popForwardAugmentation();

	int num_forward_augmentations;
	int max_forward_augmentations;
	int* forward_augmentations;

	/**
	 * color_augmentations stores the list of elements that receive their color
	 * 	in the order that they get their color.
	 */
	void pushColorAugmentation(int pos);
	void popColorAugmentation();

	int num_color_augmentations;
	int max_color_augmentations;
	int* color_augmentations;


public:
	/**
	 * ProgressionTable constructor
	 */
	ProgressionTable(progression_mode mode, int k, int d, int minN, int maxN);

	/**
	 * Destructor
	 */
	virtual ~ProgressionTable();

	/**
	 * getN -- get the current level of colors.
	 */
	int getN();

	/**
	 * getColor
	 */
	char getColor(int j);

	/**
	 * setColor
	 *
	 * @return false if the color is incorrect or if the propagation fails.
	 */
	bool setColor(int j, char c, bool top_level);

	/**
	 * hasProgression
	 */
	bool hasProgression();


	/**
	 * snapshot
	 */
	void snapshot();


	/**
	 * rollback
	 */
	void rollback();


	/**
	 * setBackwardPropagation()
	 */
	 void setBackwardPropagation(bool val);

	/**
	 * setForwardPropagation()
	 */
	void setForwardPropagation(bool val);



	/**
	 * print
	 */
	void print();


	void reset();
};

#endif /* PROGRESSIONTABLE_HPP_ */
