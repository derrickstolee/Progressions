/*
 * ProgressionTable.cpp
 *
 *  Created on: May 15, 2011
 *      Author: derrickstolee
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ProgressionTable.hpp"

/**
 * given the set of parameters (in Pseudo mode), return the size of the forward_table and backward_table arrays
 */
int ProgressionTable::getPseudoTableSize(int nColors)
{
	int size = nColors * this->maxN * this->maxL * (this->d + 1);

	return size;
}

/**
 * given the set of inputs (in Pseudo mode), return the index for the forward_table and backward_table arrays.
 */
int ProgressionTable::getPseudoTableIndex(int color, int pos, int l, int dp)
{
	int colorMult = this->maxN * this->maxL * (this->d + 1);
	int posMult = this->maxL * (this->d + 1);
	int lMult = (this->d + 1);

	int index = color * colorMult;
	index += pos * posMult;
	index += l * lMult;
	index += dp;

	return index;
}

/**
 * given the set of parameters (in Quasi mode), return the size of the forward_table and backward_table arrays
 */
int ProgressionTable::getQuasiTableSize(int nColors)
{
	int size = nColors * this->maxN * this->maxL;

	return size;
}

/**
 * given the set of inputs (in Quasi mode), return the index for the forward_table and backward_table arrays.
 */
int ProgressionTable::getQuasiTableIndex(int color, int pos, int l)
{
	int colorMult = this->maxN * this->maxL;
	int posMult = this->maxL;

	int index = color * colorMult;
	index += pos * posMult;
	index += l;

	return index;
}

/**
 * setBackwardTable
 *
 * Set a position in the backward table, while updating the snapshot sizes. (Pseudo Mode)
 */
bool ProgressionTable::setBackwardTable(int color, int pos, int l, int dp, char value)
{
	int index = this->getPseudoTableIndex(color, pos, l, dp);

	int prev_val = this->backward_table[index];

	if ( prev_val != value )
	{
		this->pushBackwardAugmentation(index, prev_val);

		this->backward_table[index] = value;
	}

	if ( pos < this->minSolN && this->colors[pos] == color && value >= this->k - 1 )
	{
		this->has_progression = true;
		return false;
	}

	if ( this->use_backward_propagation && value >= this->k - 1 )
	{
		bool result = this->setColor(pos, 1 - color, false);

		if ( pos < this->minSolN && !result )
		{
			/* we have a contradiction! */
			this->has_progression = true;
			return false;
		}
	}

	/* IF USING FORWARD PROPAGATION, THEN DO SO! */
	if ( this->use_forward_propagation && pos < this->minSolN && value > 0 )
	{
		int f_val = this->getForwardTable(color, pos, l, this->d - dp);

		if ( value + f_val >= this->k - 1 )
		{
			/* This position is in the middle of an otherwise monochromatic
			 * (k,d)-PAP with low-difference l.
			 */
			bool result = this->setColor(pos, 1 - color, false);

			if ( !result )
			{
				this->has_progression = true;
				return false;
			}
		}
	}
	/* update all later values, if the color matches */
	if ( color == this->colors[pos] )
	{
		for ( int D = 0; D <= this->d - dp; D++ )
		{
			int next_pos = pos + (l + 1) + D;

			if ( next_pos < this->maxN )
			{
				int next_val = this->getBackwardTable(color, next_pos, l, dp + D);

				if ( next_val < value + 1 )
				{
					bool result = this->setBackwardTable(color, next_pos, l, dp + D, value + 1);

					if ( next_pos < this->minSolN && !result )
					{
						this->has_progression = true;
						return false;
					}
				}
			}
		}
	}

	return true;
}

/**
 * setBackwardTable
 *
 * Set a position in the backward table, while updating the snapshot sizes. (Quasi Mode)
 */
bool ProgressionTable::setBackwardTable(int color, int pos, int l, char value)
{
	int index = this->getQuasiTableIndex(color, pos, l);

	int prev_val = this->backward_table[index];

	if ( prev_val != value )
	{
		this->pushBackwardAugmentation(index, prev_val);

		this->backward_table[index] = value;
	}

	if ( pos < this->minSolN && this->colors[pos] == color && value >= this->k - 1 )
	{
		this->has_progression = true;
		return false;
	}

	if ( this->use_backward_propagation && value >= this->k - 1 )
	{
		bool result = this->setColor(pos, 1 - color, false);

		if ( pos < this->minSolN && !result )
		{
			/* we have a contradiction! */
			this->has_progression = true;
			return false;
		}
	}

	/* IF USING FORWARD PROPAGATION, THEN DO SO! */
	if ( pos < this->minSolN && value > 0 && this->use_forward_propagation )
	{
		int f_val = this->getForwardTable(color, pos, l);

		if ( value + f_val >= this->k - 1 )
		{
			/* This position is in the middle of an otherwise monochromatic
			 * (k,d)-QAP with low-difference l.
			 */
			bool result = this->setColor(pos, 1 - color, false);

			if ( !result )
			{
				this->has_progression = true;
				return false;
			}
		}
	}

	/* update all later values, if the color matches */
	if ( color == this->colors[pos] )
	{
		for ( int D = 0; D <= this->d; D++ )
		{
			int next_pos = pos + (l + 1) + D;

			if ( next_pos < this->maxN )
			{
				int next_val = this->getBackwardTable(color, next_pos, l);

				if ( next_val < value + 1 )
				{
					bool result = this->setBackwardTable(color, next_pos, l, value + 1);

					if ( next_pos < this->minSolN && !result )
					{
						this->has_progression = true;
						return false;
					}
				}
			}
		}
	}

	return true;
}

/**
 * getBackwardTable
 *
 * Returns the value of the backward table.
 */
char ProgressionTable::getBackwardTable(int color, int pos, int l, int dp)
{
	int index = this->getPseudoTableIndex(color, pos, l, dp);

	return this->backward_table[index];
}
/**
 * getBackwardTable
 *
 * Returns the value of the backward table.
 */
char ProgressionTable::getBackwardTable(int color, int pos, int l)
{
	int index = this->getQuasiTableIndex(color, pos, l);

	return this->backward_table[index];
}

/**
 * setForwardTable
 *
 * Set a position in the forward table, while updating the snapshot sizes. (Pseudo Mode)
 */
bool ProgressionTable::setForwardTable(int color, int pos, int l, int dp, char value)
{
	if ( pos < this->curN )
	{
		return true;
	}

	int index = this->getPseudoTableIndex(color, pos, l, dp);

	int prev_val = this->forward_table[index];

	if ( prev_val != value )
	{
		this->pushForwardAugmentation(index, prev_val);

		this->forward_table[index] = value;
	}

	if ( pos < this->minSolN && this->use_forward_propagation && value >= this->k - 1 )
	{
		/* This color cannot be color, or else we have a (k,d)-PAP with low-diff l */
		bool result = this->setColor(pos, 1 - color, false);

		if ( !result )
		{
			this->has_progression = true;
			return false;
		}
	}

	if ( pos < this->minSolN && value > 0 && this->use_backward_propagation )
	{
		int b_val = this->getBackwardTable(color, pos, l, this->d - dp);

		if ( value + b_val >= this->k - 1 )
		{
			/* This position is in the middle of an otherwise monochromatic
			 * (k,d)-PAP with low-difference l.
			 */
			bool result = this->setColor(pos, 1 - color, false);

			if ( !result )
			{
				this->has_progression = true;
				return false;
			}
		}
	}

	if ( pos < this->minSolN && this->colors[pos] == color )
	{
		/* advance the forward value (for any given diameter) */
		for ( int D = 0; D <= this->d - dp; D++ )
		{
			int next_pos = pos - ((l + 1) + D);

			if ( next_pos >= 0 )
			{
				int next_val = this->getForwardTable(color, next_pos, l, dp + D);

				if ( next_val < value + 1 )
				{
					bool result = this->setForwardTable(color, next_pos, l, dp + D, value + 1);

					if ( result == false )
					{
						this->has_progression = true;
						return false;
					}
				}
			}
		}
	}

	return true;
}

/**
 * setForwardTable
 *
 * Set a position in the forward table, while updating the snapshot sizes. (Quasi Mode)
 */
bool ProgressionTable::setForwardTable(int color, int pos, int l, char value)
{
	if ( pos < this->curN )
	{
		return true;
	}

	int index = this->getQuasiTableIndex(color, pos, l);

	int prev_val = this->forward_table[index];

	if ( prev_val != value )
	{
		this->pushForwardAugmentation(index, prev_val);

		this->forward_table[index] = value;
	}

	if ( pos < this->minSolN && this->use_forward_propagation && value >= this->k - 1 )
	{
		/* This color cannot be color, or else we have a (k,d)-QAP with low-diff l */
		bool result = this->setColor(pos, 1 - color, false);

		if ( !result )
		{
			this->has_progression = true;
			return false;
		}
	}

	if ( pos < this->minSolN && value > 0 && this->use_backward_propagation )
	{
		int b_val = this->getBackwardTable(color, pos, l);

		if ( value + b_val >= this->k - 1 )
		{
			/* This position is in the middle of an otherwise monochromatic
			 * (k,d)-QAP with low-difference l.
			 */
			bool result = this->setColor(pos, 1 - color, false);

			if ( !result )
			{
				this->has_progression = true;
				return false;
			}
		}
	}

	if ( pos < this->minSolN && this->colors[pos] == color )
	{
		/* advance the forward value (for any given diameter) */
		for ( int D = 0; D <= this->d; D++ )
		{
			int next_pos = pos - ((l + 1) + D);

			if ( next_pos >= 0 )
			{
				int next_val = this->getForwardTable(color, next_pos, l);

				if ( next_val < value + 1 )
				{
					bool result = this->setForwardTable(color, next_pos, l, value + 1);

					if ( result == false )
					{
						this->has_progression = true;
						return false;
					}
				}
			}
		}
	}

	return true;
}

/**
 * getForwardTable
 *
 * Returns the value of the forward table. (Pseudo Mode)
 */
char ProgressionTable::getForwardTable(int color, int pos, int l, int dp)
{
	int index = this->getPseudoTableIndex(color, pos, l, dp);

	return this->forward_table[index];
}

/**
 * getForwardTable
 *
 * Returns the value of the forward table. (Quasi Mode)
 */
char ProgressionTable::getForwardTable(int color, int pos, int l)
{
	int index = this->getQuasiTableIndex(color, pos, l);

	return this->forward_table[index];
}

/**
 * ProgressionTable constructor
 */
ProgressionTable::ProgressionTable(progression_mode mode, int k, int d, int minN, int maxN)
{
	this->mode = mode;
	this->k = k;
	this->d = d;
	this->minN = minN;
	this->minSolN = minN;
	this->maxN = maxN;

	this->use_backward_propagation = false;
	this->use_forward_propagation = false;

	this->maxL = maxN / (k - 1);

	this->has_progression = false;

	this->num_snapshots = 0;
	this->max_snapshots = maxN;
	this->backward_sizes = (int*) malloc(this->max_snapshots * sizeof(int));
	this->forward_sizes = (int*) malloc(this->max_snapshots * sizeof(int));
	this->snapshot_sizes = (int*) malloc(this->max_snapshots * sizeof(int));

	bzero(this->backward_sizes, this->max_snapshots * sizeof(int));
	bzero(this->forward_sizes, this->max_snapshots * sizeof(int));
	bzero(this->snapshot_sizes, this->max_snapshots * sizeof(int));

	this->num_backward_augmentations = 2;
	this->max_backward_augmentations = maxN * this->k;

	this->num_forward_augmentations = 2;
	this->max_forward_augmentations = maxN * this->k;

	this->backward_augmentations = (int*) malloc(this->max_backward_augmentations * sizeof(int));
	this->forward_augmentations = (int*) malloc(this->max_forward_augmentations * sizeof(int));

	bzero(this->backward_augmentations, this->max_backward_augmentations * sizeof(int));
	bzero(this->forward_augmentations, this->max_forward_augmentations * sizeof(int));

	this->num_color_augmentations = 2;
	this->max_color_augmentations = maxN;
	this->color_augmentations = (int*) malloc(this->max_color_augmentations * sizeof(int));
	bzero(this->color_augmentations, this->max_color_augmentations * sizeof(int));

	this->snapshot();
	this->snapshot();

	this->curN = 0;
	this->colors = (char*) malloc(maxN);

	this->implied_colors = 0;

	for ( int i = 0; i < maxN; i++ )
	{
		this->colors[i] = 2;
	}

	this->quasi_table = 0;

	if ( this->mode == QUASI_PROGRESSION )
	{
		int table_size = this->getQuasiTableSize(2);
		this->forward_table = (char*) malloc(table_size);
		this->backward_table = (char*) malloc(table_size);
		bzero(this->forward_table, table_size);
		bzero(this->backward_table, table_size);
	}
	else if ( this->mode == PSEUDO_PROGRESSION )
	{
		int table_size = this->getPseudoTableSize(2);
		this->forward_table = (char*) malloc(table_size);
		this->backward_table = (char*) malloc(table_size);
		bzero(this->forward_table, table_size);
		bzero(this->backward_table, table_size);
	}

	this->setColor(0, 0, true);
}

/**
 * Destructor
 */
ProgressionTable::~ProgressionTable()
{
	if ( this->colors != 0 )
	{
		free(this->colors);
		this->colors = 0;
	}

	if ( this->implied_colors != 0 )
	{
		free(this->implied_colors);
		this->implied_colors = 0;
	}

	if ( this->quasi_table != 0 )
	{
		for ( int i = 0; i < this->maxN; i++ )
		{
			free(this->quasi_table[i]);
			this->quasi_table[i] = 0;
		}

		free(this->quasi_table);
		this->quasi_table = 0;
	}

	if ( this->forward_table != 0 )
	{
		free(this->forward_table);
		this->forward_table = 0;
	}

	if ( this->backward_table != 0 )
	{
		free(this->backward_table);
		this->backward_table = 0;
	}

	if ( this->forward_augmentations != 0 )
	{
		free(this->forward_augmentations);
		this->forward_augmentations = 0;
	}
	this->num_forward_augmentations = 0;
	this->max_forward_augmentations = 0;

	if ( this->backward_augmentations != 0 )
	{
		free(this->backward_augmentations);
		this->backward_augmentations = 0;
	}
	this->num_backward_augmentations = 0;
	this->max_backward_augmentations = 0;

	if ( this->color_augmentations != 0 )
	{
		free(this->color_augmentations);
		this->color_augmentations = 0;
	}
	this->num_color_augmentations = 0;
	this->max_color_augmentations = 0;

	if ( this->snapshot_sizes != 0 )
	{
		free(this->snapshot_sizes);
		this->snapshot_sizes = 0;
	}
	if ( this->forward_sizes != 0 )
	{
		free(this->forward_sizes);
		this->forward_sizes = 0;
	}
	if ( this->backward_sizes != 0 )
	{
		free(this->backward_sizes);
		this->backward_sizes = 0;
	}
	this->num_snapshots = 0;
	this->max_snapshots = 0;
}

/**
 * getN -- get the current level of colors.
 */
int ProgressionTable::getN()
{
	return this->curN;
}

/**
 * getColor
 */
char ProgressionTable::getColor(int j)
{
	if ( j < 0 || j >= this->maxN )
	{
		return 5;
	}

	return this->colors[j];
}

/**
 * setColor
 *
 * @return false if the color is incorrect or if the propagation fails.
 */
bool ProgressionTable::setColor(int j, char c, bool top_level)
{
	if ( j < 0 || j >= this->maxN )
	{
		return false;
	}

	/* check color! */
	if ( this->colors[j] == 2 )
	{
		/* set color! */
		this->colors[j] = c;
		this->pushColorAugmentation(j);

		if ( top_level )
		{
			this->curN = j + 1;
		}
	}
	else if ( this->colors[j] != c )
	{
		return false;
	}
	else
	{
		if ( top_level )
		{
			this->curN = j + 1;
		}
		/* color already set! */
		return true;
	}

	/* update dynamically */
	if ( mode == PSEUDO_PROGRESSION )
	{
		/*  backward propagation */
		/* update the backward tables for this color by using the CURRENT position
		 * and taking ONE step farther.
		 */

		for ( int l = 0; l < this->maxL; l++ )
		{
			for ( int D = 0; D <= this->d; D++ )
			{
				int value = this->getBackwardTable(c, j, l, D);

				if ( j < this->minSolN && value >= this->k - 1 )
				{
					this->has_progression = true;
					return false;
				}

				bool result = this->setBackwardTable(c, j, l, D, value);

				if ( !result )
				{
					this->has_progression = true;
					return false;
				}

				if ( this->use_forward_propagation && j < this->minSolN )
				{
					result = this->setForwardTable(c, j, l, D, this->getForwardTable(c, j, l, D));
					if ( !result )
					{
						this->has_progression = true;
						return false;
					}
				}
			}
		}
	}
	else if ( mode == QUASI_PROGRESSION )
	{
		/*  backward propagation */
		/* update the backward tables for this color by using the CURRENT position
		 * and taking ONE step farther.
		 */

		for ( int l = 0; l < this->maxL; l++ )
		{
			bool result = this->setBackwardTable(c, j, l, this->getBackwardTable(c, j, l));

			if ( !result )
			{
				this->has_progression = true;
				return false;
			}

			if ( this->use_forward_propagation && j < this->minSolN )
			{
				result = this->setForwardTable(c, j, l, this->getForwardTable(c, j, l));

				if ( !result )
				{
					this->has_progression = true;
					return false;
				}
			}
		}
	}

	return true;
}

/**
 * hasProgression
 */
bool ProgressionTable::hasProgression()
{
	if ( this->has_progression )
	{
		return true;
	}

	if ( this->minSolN < this->curN && this->mode == QUASI_PROGRESSION )
	{
		/* only check the positions which previously didn't send a kill! */
		for ( int c = 0; c < 2; c++ )
		{
			for ( int i = this->minSolN; i < this->curN; i++ )
			{
				for ( int l = 0; l < this->maxL; l++ )
				{
					int val = this->getBackwardTable(c, i, l);
					if ( this->colors[i] == c && val >= k - 1 )
					{
						return true;
					}

					if ( val >= k )
					{
						return true;
					}

				}
			}
		}

		/* update! this is a good solution! */
		this->minSolN = this->curN;
	}
	else if ( this->minSolN < this->curN && this->mode == PSEUDO_PROGRESSION )
	{
		/* only check the positions which previously didn't send a kill! */
		for ( int c = 0; c < 2; c++ )
		{
			for ( int i = this->minSolN; i < this->curN; i++ )
			{
				for ( int l = 0; l < this->maxL; l++ )
				{
					for ( int D = 0; D <= this->d; D++ )
					{
						int val = this->getBackwardTable(c, i, l, D);
						if ( val >= k - 1 && this->colors[i] == c )
						{
							return true;
						}

						if ( val >= k )
						{
							return true;
						}
					}
				}
			}
		}

		/* update! this is a good solution! */
		this->minSolN = this->curN;
	}

	return false;
}

/**
 * snapshot
 */
/**
 * rollback
 */
void ProgressionTable::snapshot()
{
	if ( this->num_snapshots >= this->max_snapshots )
	{
		this->max_snapshots += 100;

		this->snapshot_sizes = (int*) realloc(this->snapshot_sizes, this->max_snapshots * sizeof(int));
		this->backward_sizes = (int*) realloc(this->backward_sizes, this->max_snapshots * sizeof(int));
		this->forward_sizes = (int*) realloc(this->forward_sizes, this->max_snapshots * sizeof(int));
	}

	this->snapshot_sizes[this->num_snapshots] = this->num_color_augmentations;
	this->backward_sizes[this->num_snapshots] = this->num_backward_augmentations;
	this->forward_sizes[this->num_snapshots] = this->num_forward_augmentations;

	this->num_snapshots = this->num_snapshots + 1;
}

void ProgressionTable::rollback()
{
	this->has_progression = false;

	if ( this->num_snapshots > 0 )
	{
		this->num_snapshots = this->num_snapshots - 1;

		int csize = this->snapshot_sizes[this->num_snapshots];
		int bsize = this->backward_sizes[this->num_snapshots];
		int fsize = this->forward_sizes[this->num_snapshots];

		if ( csize > 0 )
		{
			while ( this->num_color_augmentations > csize )
			{
				this->popColorAugmentation();
			}
		}

		if ( bsize > 0 )
		{
			while ( this->num_backward_augmentations > bsize )
			{
				this->popBackwardAugmantation();
			}
		}

		if ( fsize > 0 )
		{
			while ( this->num_forward_augmentations > fsize )
			{
				this->popForwardAugmentation();
			}
		}
	}
}

/**
 * setBackwardPropagation()
 */
void ProgressionTable::setBackwardPropagation(bool val)
{
	this->use_backward_propagation = val;
}

/**
 * setForwardPropagation()
 */
void ProgressionTable::setForwardPropagation(bool val)
{
	this->use_forward_propagation = val;
}

/**
 * print
 */
void ProgressionTable::print()
{
	printf("colors: ");
	for ( int i = 0; i < this->curN; i++ )
	{
		printf("%d ", this->colors[i]);
	}
	printf("\n");

	if ( mode == QUASI_PROGRESSION )
	{
		printf("quasi_table:\n");

		for ( int l = 0; l < this->maxL; l++ )
		{
			printf("l=%d:\t", l + 1);

			for ( int i = 0; i < this->curN; i++ )
			{
				printf("%d ", this->quasi_table[i][l]);
			}

			printf("\n");
		}

		printf("\n");
	}
	else if ( mode == PSEUDO_PROGRESSION )
	{
		printf("pseudo_table:\n");

		for ( int c = 0; c < 2; c++ )
		{

			printf("color %d : \n", c);

			printf("\t        \t");
			for ( int i = 0; i < this->maxN; i++ )
			{
				if ( i == this->minSolN )
				{
					printf("| ");
				}
				printf("%2d ", this->colors[i]);
			}
			printf("\n");
			for ( int l = 0; l < this->maxL; l++ )
			{
				for ( int D = 0; D <= this->d; D++ )
				{
					printf("\tl=%d, d=%d\t", l + 1, D);

					for ( int i = 0; i < this->maxN; i++ )
					{
						if ( i == this->minSolN )
						{
							printf("| ");
						}
						printf("%2d ", this->backward_table[this->getPseudoTableIndex(c, i, l, D)]);
					}

					printf("\n");
				}
			}
			printf("\n\n");
		}

		printf("\n");
	}
}

void ProgressionTable::reset()
{
	if ( this->colors != 0 )
	{
		free(this->colors);
		this->colors = 0;
	}

	if ( this->implied_colors != 0 )
	{
		free(this->implied_colors);
		this->implied_colors = 0;
	}

	if ( this->quasi_table != 0 )
	{
		for ( int i = 0; i < this->maxN; i++ )
		{
			free(this->quasi_table[i]);
			this->quasi_table[i] = 0;
		}

		free(this->quasi_table);
		this->quasi_table = 0;
	}

	if ( this->forward_table != 0 )
	{
		free(this->forward_table);
		this->forward_table = 0;
	}

	if ( this->backward_table != 0 )
	{
		free(this->backward_table);
		this->backward_table = 0;
	}

	if ( this->forward_augmentations != 0 )
	{
		free(this->forward_augmentations);
		this->forward_augmentations = 0;
	}
	this->num_forward_augmentations = 0;
	this->max_forward_augmentations = 0;

	if ( this->backward_augmentations != 0 )
	{
		free(this->backward_augmentations);
		this->backward_augmentations = 0;
	}
	this->num_backward_augmentations = 0;
	this->max_backward_augmentations = 0;

	if ( this->color_augmentations != 0 )
	{
		free(this->color_augmentations);
		this->color_augmentations = 0;
	}
	this->num_color_augmentations = 0;
	this->max_color_augmentations = 0;

	if ( this->snapshot_sizes != 0 )
	{
		free(this->snapshot_sizes);
		this->snapshot_sizes = 0;
	}
	if ( this->forward_sizes != 0 )
	{
		free(this->forward_sizes);
		this->forward_sizes = 0;
	}
	if ( this->backward_sizes != 0 )
	{
		free(this->backward_sizes);
		this->backward_sizes = 0;
	}
	this->num_snapshots = 0;
	this->max_snapshots = 0;

	this->has_progression = false;

	this->num_snapshots = 0;
	this->max_snapshots = maxN;
	this->backward_sizes = (int*) malloc(this->max_snapshots * sizeof(int));
	this->forward_sizes = (int*) malloc(this->max_snapshots * sizeof(int));
	this->snapshot_sizes = (int*) malloc(this->max_snapshots * sizeof(int));

	bzero(this->backward_sizes, this->max_snapshots * sizeof(int));
	bzero(this->forward_sizes, this->max_snapshots * sizeof(int));
	bzero(this->snapshot_sizes, this->max_snapshots * sizeof(int));

	this->num_backward_augmentations = 2;
	this->max_backward_augmentations = maxN * this->k;

	this->num_forward_augmentations = 2;
	this->max_forward_augmentations = maxN * this->k;

	this->backward_augmentations = (int*) malloc(this->max_backward_augmentations * sizeof(int));
	this->forward_augmentations = (int*) malloc(this->max_forward_augmentations * sizeof(int));

	bzero(this->backward_augmentations, this->max_backward_augmentations * sizeof(int));
	bzero(this->forward_augmentations, this->max_forward_augmentations * sizeof(int));

	this->num_color_augmentations = 2;
	this->max_color_augmentations = maxN;
	this->color_augmentations = (int*) malloc(this->max_color_augmentations * sizeof(int));
	bzero(this->color_augmentations, this->max_color_augmentations * sizeof(int));

	this->snapshot();
	this->snapshot();

	this->curN = 0;

	this->colors = (char*) malloc(this->maxN);
	this->implied_colors = (char*) malloc(this->maxN);
	for ( int i = 0; i < this->maxN; i++ )
	{
		this->colors[i] = 2;
		this->implied_colors[i] = 2;
	}

	if ( this->mode == QUASI_PROGRESSION )
	{
		int table_size = this->getQuasiTableSize(2);
		this->forward_table = (char*) malloc(table_size);
		this->backward_table = (char*) malloc(table_size);
		bzero(this->forward_table, table_size);
		bzero(this->backward_table, table_size);
	}
	else if ( this->mode == PSEUDO_PROGRESSION )
	{
		int table_size = this->getPseudoTableSize(2);
		this->forward_table = (char*) malloc(table_size);
		this->backward_table = (char*) malloc(table_size);
		bzero(this->forward_table, table_size);
		bzero(this->backward_table, table_size);
	}

	this->setColor(0, 0, true);
}

void ProgressionTable::pushBackwardAugmentation(int pos, int val)
{
	if ( this->num_backward_augmentations >= this->max_backward_augmentations - 1 )
	{
		this->max_backward_augmentations += this->maxN * 2;
		this->backward_augmentations = (int*) realloc(this->backward_augmentations, this->max_backward_augmentations
				* sizeof(int));
	}

	this->backward_augmentations[this->num_backward_augmentations] = pos;
	this->backward_augmentations[this->num_backward_augmentations + 1] = val;

	this->num_backward_augmentations = this->num_backward_augmentations + 2;
}

void ProgressionTable::popBackwardAugmantation()
{
	if ( this->num_backward_augmentations > 1 )
	{
		this->num_backward_augmentations = this->num_backward_augmentations - 2;
		int pos = this->backward_augmentations[this->num_backward_augmentations];
		int old_val = this->backward_augmentations[this->num_backward_augmentations + 1];

		this->backward_table[pos] = old_val;
	}
}

void ProgressionTable::pushForwardAugmentation(int pos, int val)
{
	if ( this->num_forward_augmentations >= this->max_forward_augmentations - 1 )
	{
		this->max_forward_augmentations += this->maxN * 2;
		this->forward_augmentations = (int*) realloc(this->forward_augmentations, this->max_forward_augmentations
				* sizeof(int));
	}

	this->forward_augmentations[this->num_forward_augmentations] = pos;
	this->forward_augmentations[this->num_forward_augmentations + 1] = val;

	this->num_forward_augmentations = this->num_forward_augmentations + 2;
}

void ProgressionTable::popForwardAugmentation()
{
	if ( this->num_forward_augmentations > 1 )
	{
		this->num_forward_augmentations = this->num_forward_augmentations - 2;
		int pos = this->forward_augmentations[this->num_forward_augmentations];
		int old_val = this->forward_augmentations[this->num_forward_augmentations + 1];

		this->forward_table[pos] = old_val;
	}
}

void ProgressionTable::pushColorAugmentation(int pos)
{
	if ( this->num_color_augmentations >= this->max_color_augmentations )
	{
		this->max_color_augmentations += 100;
		this->color_augmentations = (int*) realloc(this->color_augmentations, this->max_color_augmentations
				* sizeof(int));
	}

	this->color_augmentations[this->num_color_augmentations] = pos;

	this->num_color_augmentations = this->num_color_augmentations + 1;
}

void ProgressionTable::popColorAugmentation()
{
	if ( this->num_color_augmentations > 0 )
	{
		this->num_color_augmentations = this->num_color_augmentations - 1;

		int pos = this->color_augmentations[this->num_color_augmentations];

		this->colors[pos] = 2;
	}
}
