/*
 * classify_line_sets_with_transversal.cpp
 *
 *  Created on: Mar 5, 2024
 *      Author: betten
 */



#include "orbiter.h"
#include <vector>

using namespace std;
using namespace orbiter;

#include "joel.h"

classify_line_sets_with_transversal::classify_line_sets_with_transversal()
{
	PG = NULL;

	target_size = 0;
	nb_lines = 0;
	transversal_line_rk = 0;

	A = NULL;
	A_on_lines = NULL;

	Stab_transversal_line = NULL;
	//ring_theory::longinteger_object go0;

	//vector<long int> Linear_complex;
	Linear_complex_Pts = NULL;
	Linear_complex_nb_pts = 0;

	A_on_linear_complex = NULL;

	//std::string label_of_set;
	//std::string label_of_set_tex;

	Seed = NULL;
	Lifting = NULL;

	Nb_orbits_in = NULL;
	Nb_orbits_out = NULL;
	Nb_orbits_flags = NULL;

}

classify_line_sets_with_transversal::~classify_line_sets_with_transversal()
{

}


void classify_line_sets_with_transversal::init(
		orbiter::layer1_foundations::geometry::projective_space *PG,
		int target_size,
		int verbose_level)
{
	int f_v = (verbose_level >= 1);


	if (f_v) {
		cout << "classify_line_sets_with_transversal::init verbose_level = " << verbose_level << endl;
	}
	classify_line_sets_with_transversal::PG = PG;
	classify_line_sets_with_transversal::target_size = target_size;

	vector<vector<long int>> columns;


	nb_lines = PG->Subspaces->N_lines;


	int d = PG->Subspaces->n + 1;

	// from projective_space_with_action.cpp:


	int f_semilinear = true;

	number_theory::number_theory_domain NT;

	if (f_v) {
		cout << "q=" << PG->Subspaces->F->q << endl;
	}

	if (NT.is_prime(PG->Subspaces->F->q)) {
		f_semilinear = false;
	}
	else {
		f_semilinear = true;
	}


	A = NEW_OBJECT(actions::action);
	data_structures_groups::vector_ge *nice_gens;

	if (f_v) {
		cout << "classify_line_sets_with_transversal::init "
				"before A->Known_groups->init_linear_group" << endl;
	}
	A->Known_groups->init_linear_group(
			PG->Subspaces->F, d,
		true /*f_projective*/,
		false /* f_general*/,
		false /* f_affine */,
		f_semilinear,
		false /* f_special */,
		nice_gens,
		verbose_level - 2);
	if (f_v) {
		cout << "classify_line_sets_with_transversal::init "
				"after A->Known_groups->init_linear_group" << endl;
	}
	FREE_OBJECT(nice_gens);
	ring_theory::longinteger_object go;

	A->group_order(go);
	cout << "classify_line_sets_with_transversal::init "
			"The group order is " << go << endl;


	if (f_v) {
		cout << "classify_line_sets_with_transversal::init "
				"creating action on lines" << endl;
	}
	A_on_lines = A->Induced_action->induced_action_on_grassmannian(
			2, verbose_level - 2);
	if (f_v) {
		cout << "classify_line_sets_with_transversal::init "
				"creating action on lines done" << endl;
	}

	layer5_applications::orbits::orbits_global Orbits;
	groups::orbits_on_something *Orbit_of_transversal_line;


	int f_load_save = false;
	string prefix0;

	prefix0 = "transversal_line";

	if (f_v) {
		cout << "classify_line_sets_with_transversal::init "
				"before Orbits.orbits_on_points" << endl;
	}
	Orbits.orbits_on_points(
			A_on_lines,
			A->Strong_gens,
			f_load_save,
			prefix0,
			Orbit_of_transversal_line,
			verbose_level - 2);
	if (f_v) {
		cout << "classify_line_sets_with_transversal::init "
				"after Orbits.orbits_on_points" << endl;
	}

	if (f_v) {
		cout << "classify_line_sets_with_transversal::init "
				"orbit lengths Orbit_of_transversal_line:" << endl;
	}
	Orbit_of_transversal_line->report_orbit_lengths(
				cout);


	if (f_v) {
		cout << "classify_line_sets_with_transversal::init "
				"before Orbit_of_transversal_line->stabilizer_of" << endl;
	}
	Orbit_of_transversal_line->stabilizer_of(
			0 /* orbits_idx */, Stab_transversal_line, verbose_level - 2);
	if (f_v) {
		cout << "classify_line_sets_with_transversal::init "
				"after Orbit_of_transversal_line->stabilizer_of" << endl;
	}

	transversal_line_rk = 0;
	Stab_transversal_line->group_order(go0);

	if (f_v) {
		cout << "classify_line_sets_with_transversal::init "
			"The stabilizer of " << prefix0 << " has order " << go0 << endl;
	}



	Seed = NEW_OBJECT(line_sets_with_transversal_seed);
	Seed->init(
			this,
			A,
			A_on_lines,
			Orbit_of_transversal_line,
			target_size,
			verbose_level - 2);


	int i;

	Lifting = NEW_OBJECTS(line_sets_with_transversal, target_size + 1);

	if (f_v) {
		cout << "classify_line_sets_with_transversal::init "
				"before setting up Lifting[]" << endl;
	}
	for (i = 0; i <= target_size; i++) {
		Lifting[i].init(
				A,
				A_on_lines,
				i,
				this,
				verbose_level - 2);

	}

	Lifting[0].init_seed(Seed, verbose_level);
	for (i = 1; i <= target_size; i++) {
		Lifting[i].init_previous_one(&Lifting[i - 1], verbose_level - 2);
	}
	if (f_v) {
		cout << "classify_line_sets_with_transversal::init "
				"after setting up Lifting[]" << endl;
	}

	if (f_v) {
		cout << "classify_line_sets_with_transversal::init "
				"before compute_linear_complex" << endl;
	}
	compute_linear_complex(verbose_level - 2);
	if (f_v) {
		cout << "classify_line_sets_with_transversal::init "
				"after compute_linear_complex" << endl;
	}

	if (f_v) {
		cout << "classify_line_sets_with_transversal::init "
				"target_size = " << target_size << endl;
		cout << "classify_line_sets_with_transversal::init "
				"before doing the lifting" << endl;
	}


	for (i = 0; i < target_size; i++) {

		if (f_v) {
			cout << "classify_line_sets_with_transversal::init "
					"before Lifting[" << i << "].classify" << endl;
		}
		Lifting[i].classify(verbose_level - 2);
		if (f_v) {
			cout << "classify_line_sets_with_transversal::init "
					"after Lifting[" << i << "].classify" << endl;
		}


	}

	if (f_v) {
		cout << "classify_line_sets_with_transversal::init "
				"after doing the lifting" << endl;
	}


	Nb_orbits_in = NEW_int(target_size);
	Nb_orbits_out = NEW_int(target_size);
	Nb_orbits_flags = NEW_int(target_size);


	Nb_orbits_in[0] = 1;

	for (i = 0; i < target_size; i++) {
		if (i) {
			Nb_orbits_in[i] = Nb_orbits_out[i - 1];
		}
		Nb_orbits_out[i] = Lifting[i].Next_one->nb_orbits;
		Nb_orbits_flags[i] = Lifting[i].nb_flag_orbits;
	}

	if (f_v) {
		cout << "level : nb_orb_in : nb_flag_orbits : nb_orbits_out" << endl;
		for (i = 0; i < target_size; i++) {
			cout << i;
			cout << " : " << Nb_orbits_in[i];
			cout << " : " << Nb_orbits_flags[i];
			cout << " : " << Nb_orbits_out[i];
			cout << endl;
		}
	}


	if (f_v) {
		cout << "classify_line_sets_with_transversal::init done" << endl;
	}
}


void classify_line_sets_with_transversal::compute_linear_complex(
		int verbose_level)
// compute all lines which intersect transversal_line
// but not equal to transversal_line
// Also, compute the restricted action of A_on_lines on this set.
// This action can only be used for the stabilizer of transversal_line,
// which is Stab_transversal_line.
{
	int f_v = (verbose_level >= 1);


	if (f_v) {
		cout << "classify_line_sets_with_transversal::compute_linear_complex" << endl;
	}

	long int line1_rk;
	int f_skew;

	for (line1_rk = 0; line1_rk < nb_lines; line1_rk++){

		if (line1_rk == transversal_line_rk) {
			continue;
		}

		f_skew = PG->Solid->test_if_lines_are_skew(
				transversal_line_rk, line1_rk,0);

		if (!f_skew) {
			Linear_complex.push_back(line1_rk);
		}
	}


	label_of_set = "linear_complex";
	label_of_set_tex = "linear\\_complex";

	int i;

	Linear_complex_nb_pts = Linear_complex.size();
	Linear_complex_Pts = NEW_lint(Linear_complex_nb_pts);
	for (i = 0; i < Linear_complex_nb_pts; i++) {
		Linear_complex_Pts[i] = Linear_complex[i];
	}
	A_on_linear_complex = A_on_lines->Induced_action->restricted_action(
			Linear_complex_Pts, Linear_complex_nb_pts,
			label_of_set,
			label_of_set_tex,
			verbose_level);

	if (f_v) {
		cout << "classify_line_sets_with_transversal::compute_linear_complex done" << endl;
	}

}


