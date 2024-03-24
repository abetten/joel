/*
 * line_sets_with_transversal_seed.cpp
 *
 *  Created on: Mar 5, 2024
 *      Author: betten
 */


#include "orbiter.h"
#include <vector>

using namespace std;
using namespace orbiter;

#include "joel.h"

line_sets_with_transversal_seed::line_sets_with_transversal_seed()
{
	Classify_line_sets_with_transversal = NULL;
	A = NULL;
	A_on_lines = NULL;
	size = 1;
	//target_size = 0;
	Orbit_of_transversal_line = NULL;
}

line_sets_with_transversal_seed::~line_sets_with_transversal_seed()
{

}


void line_sets_with_transversal_seed::init(
		classify_line_sets_with_transversal *Classify_line_sets_with_transversal,
		actions::action *A,
		actions::action *A_on_lines,
		groups::orbits_on_something *Orbit_of_transversal_line,
		//int target_size,
		int verbose_level)
{
	int f_v = (verbose_level >= 1);


	if (f_v) {
		cout << "line_sets_with_transversal_seed::init" << endl;
	}
	line_sets_with_transversal_seed::Classify_line_sets_with_transversal = Classify_line_sets_with_transversal;
	line_sets_with_transversal_seed::A = A;
	line_sets_with_transversal_seed::A_on_lines = A_on_lines;
	line_sets_with_transversal_seed::Orbit_of_transversal_line = Orbit_of_transversal_line;
	line_sets_with_transversal_seed::size = 1;
	//line_sets_with_transversal_seed::target_size = target_size;
	if (f_v) {
		cout << "line_sets_with_transversal_seed::init done" << endl;
	}
}

int line_sets_with_transversal_seed::nb_orbits()
{
	int nb;

	nb = Orbit_of_transversal_line->Sch->nb_orbits;
	return nb;
}

void line_sets_with_transversal_seed::get_admissible_set(
		int orbit_idx,
		long int *&admissible_set, int &admissible_set_size,
		int verbose_level)
// the admissible set after the orbit representative of Orb0
// (which is transversal line) has been chosen.
{
	int f_v = (verbose_level >= 1);


	if (f_v) {
		cout << "line_sets_with_transversal_seed::get_admissible_set" << endl;
	}

	admissible_set_size = Classify_line_sets_with_transversal->Linear_complex_nb_pts;
	admissible_set = Classify_line_sets_with_transversal->Linear_complex_Pts;

	if (f_v) {
		cout << "line_sets_with_transversal_seed::get_admissible_set" << endl;
	}


}

int line_sets_with_transversal_seed::get_orbit_rep(
		int orbit_idx, int verbose_level)
{
	int rep;

	rep = Orbit_of_transversal_line->get_orbit_rep(
		orbit_idx, verbose_level);

	return rep;
}

data_structures_groups::set_and_stabilizer *line_sets_with_transversal_seed::get_set_and_stabilizer(
		int orbit_idx, int verbose_level)
{
	int f_v = (verbose_level >= 1);


	if (f_v) {
		cout << "line_sets_with_transversal_seed::get_set_and_stabilizer" << endl;
	}

	data_structures_groups::set_and_stabilizer *SaS;
	ring_theory::longinteger_object ol;
	ring_theory::longinteger_object go;


	SaS = NEW_OBJECT(data_structures_groups::set_and_stabilizer);

	SaS->init(A, A_on_lines, 0 /*verbose_level */);

	SaS->allocate_data(size, 0 /* verbose_level */);

	groups::strong_generators *Stab0;
	int orbit_rep;

	if (f_v) {
		cout << "line_sets_with_transversal_seed::get_set_and_stabilizer "
				"before Orbit_of_transversal_line->stabilizer_of_orbit_representative" << endl;
	}
	Orbit_of_transversal_line->stabilizer_of_orbit_representative(
			orbit_idx, Stab0, orbit_rep, verbose_level);
	if (f_v) {
		cout << "line_sets_with_transversal_seed::get_set_and_stabilizer "
				"after Orbit_of_transversal_line->stabilizer_of_orbit_representative" << endl;
	}
	SaS->data[0] = orbit_rep;
	SaS->Strong_gens = Stab0;
	Stab0->group_order(SaS->target_go);

#if 0
	ring_theory::longinteger_object go0;
	get_set_by_level(size, orbit_at_level, SaS->data);

	get_stabilizer_generators(SaS->Strong_gens,
		level, orbit_at_level, 0 /* verbose_level */);
	SaS->Strong_gens->group_order(SaS->target_go);

#endif


	SaS->Stab = SaS->Strong_gens->create_sims(0 /*verbose_level*/);
	if (f_v) {
		cout << "poset_classification::get_set_and_stabilizer done" << endl;
	}

	if (f_v) {
		cout << "line_sets_with_transversal_seed::get_set_and_stabilizer done" << endl;
	}

	return SaS;

}


