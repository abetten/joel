/*
 * line_sets_with_transversal_horizontal_slice.cpp
 *
 *  Created on: Mar 5, 2024
 *      Author: betten
 */


#include "orbiter.h"
#include <vector>

using namespace std;
using namespace orbiter;

#include "joel.h"


line_sets_with_transversal_horizontal_slice::line_sets_with_transversal_horizontal_slice()
{
	Classify_line_sets_with_transversal = NULL;
	Lifting = NULL;
	po = 0;
	R = NULL;
	//ring_theory::longinteger_object go;

	primary_orbit_rep = NULL;

	old_admissible_set = NULL;
	old_admissible_set_size = 0;

	admissible_set = NULL;
	admissible_set_size = 0;

	A_on_admissible_set = NULL;

	Orbits = NULL;

	first_flag_orbit_idx = -1;
	nb_flag_orbits_in_horizontal_slice = -1;


}

line_sets_with_transversal_horizontal_slice::~line_sets_with_transversal_horizontal_slice()
{

}

void line_sets_with_transversal_horizontal_slice::init(
		classify_line_sets_with_transversal *Classify_line_sets_with_transversal,
		line_sets_with_transversal *Lifting,
		int po,
		int verbose_level)
{
	int f_v = (verbose_level >= 1);


	if (f_v) {
		cout << "line_sets_with_transversal_horizontal_slice::init" << endl;
	}
	line_sets_with_transversal_horizontal_slice::Classify_line_sets_with_transversal = Classify_line_sets_with_transversal;
	line_sets_with_transversal_horizontal_slice::Lifting = Lifting;
	line_sets_with_transversal_horizontal_slice::po = po;


	primary_orbit_rep = NEW_int(Lifting->size);

	if (f_v) {
		cout << "line_sets_with_transversal_horizontal_slice::init "
				"before Lifting->get_primary_orbit_rep" << endl;
	}
	Lifting->get_primary_orbit_rep(
			po /* orbit_idx */,
			primary_orbit_rep,
			verbose_level - 2);
	if (f_v) {
		cout << "line_sets_with_transversal_horizontal_slice::init "
				"after Lifting->get_primary_orbit_rep" << endl;
	}

	if (f_v) {
		cout << "line_sets_with_transversal_horizontal_slice::init "
				"primary_orbit_rep = ";
		Int_vec_print(cout, primary_orbit_rep, Lifting->size);
		cout << endl;
	}

	if (f_v) {
		cout << "line_sets_with_transversal_horizontal_slice::init "
				"before Lifting->get_admissible_set" << endl;
	}
	Lifting->get_admissible_set(
			po /* orbit_idx */,
			old_admissible_set, old_admissible_set_size,
			verbose_level);
	if (f_v) {
		cout << "line_sets_with_transversal_horizontal_slice::init "
				"after Lifting->get_admissible_set" << endl;
	}

	if (f_v) {
		cout << "line_sets_with_transversal_horizontal_slice::init "
				"size of old admissible set = " << old_admissible_set_size << endl;
		cout << "line_sets_with_transversal_horizontal_slice::init "
				"old admissible set = " << endl;
		Lint_vec_print(cout, old_admissible_set, old_admissible_set_size);
		cout << endl;
	}

	//data_structures_groups::set_and_stabilizer *R;
	//ring_theory::longinteger_object ol;
	//ring_theory::longinteger_object go;
	//long int dataset[23];

	if (f_v) {
		cout << "line_sets_with_transversal_horizontal_slice::init "
				"before Lifting->get_previous_set_and_stabilizer" << endl;
	}
	R = Lifting->get_previous_set_and_stabilizer(
			po /* orbit_at_level */,
			verbose_level);
	if (f_v) {
		cout << "line_sets_with_transversal_horizontal_slice::init "
				"after Lifting->get_previous_set_and_stabilizer" << endl;
	}

	R->Strong_gens->group_order(go);
	if (f_v) {
		cout << "line_sets_with_transversal_horizontal_slice::init "
				"go=" << go << endl;
	}

	//data_structures_groups::set_and_stabilizer *line_sets_with_transversal::get_set_and_stabilizer(
	//		int orbit_idx, int verbose_level);


	if (f_v) {
		cout << "line_sets_with_transversal_horizontal_slice::init "
				"Lifting->size=" << Lifting->size << endl;
	}
	if (f_v) {
		cout << "line_sets_with_transversal_horizontal_slice::init "
				"before testing lines" << endl;
	}


	if (f_v) {
		cout << "line_sets_with_transversal_horizontal_slice::init "
				"before test_lines" << endl;
	}
	test_lines(0 /*verbose_level - 2*/);
	if (f_v) {
		cout << "line_sets_with_transversal_horizontal_slice::init "
				"after test_lines" << endl;
	}
	if (f_v) {
		cout << "line_sets_with_transversal_horizontal_slice::init "
				"size of new admissible set = " << admissible_set_size << endl;
	}


	string label_of_set;
	string label_of_set_tex;

	label_of_set = "layer" + std::to_string(Lifting->size) + "_po" + std::to_string(po);
	label_of_set_tex = "layer" + std::to_string(Lifting->size) + "_po" + std::to_string(po);

	if (f_v) {
		cout << "line_sets_with_transversal_horizontal_slice::init "
				"before creating restricted action" << endl;
	}
	A_on_admissible_set = Classify_line_sets_with_transversal->A_on_lines->Induced_action->restricted_action(
			admissible_set, admissible_set_size,
			label_of_set,
			label_of_set_tex,
			verbose_level - 2);
	if (f_v) {
		cout << "line_sets_with_transversal_horizontal_slice::init "
				"after creating restricted action" << endl;
	}


	layer5_applications::orbits::orbits_global Orbits_global;
	//groups::orbits_on_something *Orbits;

	int f_load_save = false;
	string prefix0;

	prefix0 = label_of_set;

	if (f_v) {
		cout << "line_sets_with_transversal_horizontal_slice::init "
				"before Orbits_global.orbits_on_points" << endl;
	}
	Orbits_global.orbits_on_points(
			A_on_admissible_set,
			R->Strong_gens,
			f_load_save,
			prefix0,
			Orbits,
			verbose_level - 2);
	if (f_v) {
		cout << "line_sets_with_transversal_horizontal_slice::init "
				"after Orbits_global.orbits_on_points" << endl;
	}

	if (f_v) {
		cout << "line_sets_with_transversal_horizontal_slice::init "
				"after Orbits_global.orbits_on_points" << endl;
	}

	nb_flag_orbits_in_horizontal_slice = Orbits->Sch->nb_orbits;

	if (f_v) {
		cout << "line_sets_with_transversal_horizontal_slice::init "
				"nb_flag_orbits_in_horizontal_slice=" << nb_flag_orbits_in_horizontal_slice << endl;
	}
	if (f_v) {
		cout << "line_sets_with_transversal_horizontal_slice::init "
				"Orbits->report_orbit_lengths:" << endl;
		Orbits->report_orbit_lengths(cout);
	}



#if 0

	if (f_v) {
		cout << "line_sets_with_transversal_horizontal_slice::init "
				"before Orbit_of_transversal_line->stabilizer_of" << endl;
	}
	Orbits->stabilizer_of(
			0 /* orbits_idx */, Stab_transversal_line, verbose_level);
	if (f_v) {
		cout << "line_sets_with_transversal_horizontal_slice::init "
				"after Orbit_of_transversal_line->stabilizer_of" << endl;
	}

	transversal_line_rk = 0;
	Stab_transversal_line->group_order(go0);

	cout << "line_sets_with_transversal_horizontal_slice::init "
			"The stabilizer of " << prefix0 << " has order " << go0 << endl;

#endif

	if (f_v) {
		cout << "line_sets_with_transversal_horizontal_slice::init done" << endl;
	}
}

void line_sets_with_transversal_horizontal_slice::test_lines(
		int verbose_level)
{
	int f_v = (verbose_level >= 1);


	if (f_v) {
		cout << "line_sets_with_transversal_horizontal_slice::test_lines" << endl;
	}

	int i, f_skew;
	vector<long int> survivors;

	for (i = 0; i < old_admissible_set_size; i++) {


		if (Lifting->size == 0) {

			if (f_v) {
				cout << "i=" << i << " : ";
				Int_vec_print(cout, primary_orbit_rep, Lifting->size);
				cout << " : " << old_admissible_set[i] << " : accept" << endl;
			}

			survivors.push_back(old_admissible_set[i]);
		}
		else {
			f_skew = Classify_line_sets_with_transversal->PG->Solid->test_if_lines_are_skew(
					primary_orbit_rep[Lifting->size - 1], old_admissible_set[i],0);

			if (f_skew) {
				if (f_v) {
					cout << "is skew" << endl;
				}
				if (f_v) {
					cout << "i=" << i << " : ";
					Int_vec_print(cout, primary_orbit_rep, Lifting->size);
					cout << " : " << old_admissible_set[i] << " : accept" << endl;
				}

				survivors.push_back(old_admissible_set[i]);
			}
			else {
				if (f_v) {
					cout << "i=" << i << " : ";
					Int_vec_print(cout, primary_orbit_rep, Lifting->size);
					cout << " : " << old_admissible_set[i] << " : reject" << endl;
				}


			}
		}
	}
	if (f_v) {
		cout << "line_sets_with_transversal_horizontal_slice::test_lines "
				"after testing lines" << endl;
	}
	admissible_set_size = survivors.size();
	admissible_set = NEW_lint(admissible_set_size);
	for (i = 0; i < admissible_set_size; i++) {
		admissible_set[i] = survivors[i];
	}
	if (f_v) {
		cout << "line_sets_with_transversal_horizontal_slice::test_lines "
				"size of new admissible set = " << admissible_set_size << endl;
	}
	if (f_v) {
		cout << "line_sets_with_transversal_horizontal_slice::test_lines "
				"reduced admissible set:" << endl;
		Lint_vec_print(cout, admissible_set, admissible_set_size);
		cout << endl;
	}

	if (f_v) {
		cout << "line_sets_with_transversal_horizontal_slice::test_lines done" << endl;
	}
}

long int line_sets_with_transversal_horizontal_slice::original_point(
		long int pt)
{
	long int pt2;

	if (!A_on_admissible_set) {
		cout << "line_sets_with_transversal_horizontal_slice::original_point !A_on_admissible_set" << endl;
		cout << "size = " << Lifting->size << endl;
		exit(1);
	}
	pt2 = A_on_admissible_set->G.ABR->original_point(pt);
	return pt2;
}

long int line_sets_with_transversal_horizontal_slice::restricted_point(
		long int pt)
{
	long int pt2;

	if (!A_on_admissible_set) {
		cout << "line_sets_with_transversal_horizontal_slice::restricted_point !A_on_admissible_set" << endl;
		cout << "size = " << Lifting->size << endl;
		exit(1);
	}
	pt2 = A_on_admissible_set->G.ABR->restricted_point_idx(pt);
	return pt2;
}




void line_sets_with_transversal_horizontal_slice::get_orbit_rep(
		int orbit_idx, int &orbit_rep, int verbose_level)
{
	int f_v = (verbose_level >= 1);


	if (f_v) {
		cout << "line_sets_with_transversal_horizontal_slice::get_orbit_rep" << endl;
	}

	orbit_rep = Orbits->get_orbit_rep(orbit_idx, verbose_level);

	if (f_v) {
		cout << "line_sets_with_transversal_horizontal_slice::get_orbit_rep done" << endl;
	}
}

void line_sets_with_transversal_horizontal_slice::get_orbit_rep_unpacked(
		int orbit_idx, int &orbit_rep, int verbose_level)
{
	int f_v = (verbose_level >= 1);


	if (f_v) {
		cout << "line_sets_with_transversal_horizontal_slice::get_orbit_rep_unpacked" << endl;
	}

	orbit_rep = Orbits->get_orbit_rep_unpacked(orbit_idx, verbose_level);

	if (f_v) {
		cout << "line_sets_with_transversal_horizontal_slice::get_orbit_rep_unpacked done" << endl;
	}
}

void line_sets_with_transversal_horizontal_slice::transporter_from_orbit_rep_to_point(
		int pt,
	int &orbit_idx, int *Elt, int verbose_level)
{
	int f_v = (verbose_level >= 1);


	if (f_v) {
		cout << "line_sets_with_transversal_horizontal_slice::transporter_from_orbit_rep_to_point" << endl;
	}

	Orbits->transporter_from_orbit_rep_to_point(
			pt,
		orbit_idx, Elt, verbose_level - 1);

	if (f_v) {
		cout << "line_sets_with_transversal_horizontal_slice::transporter_from_orbit_rep_to_point done" << endl;
	}

}


void line_sets_with_transversal_horizontal_slice::transporter_from_point_to_orbit_rep(
		int pt,
	int &orbit_idx, int *Elt, int verbose_level)
{
	int f_v = (verbose_level >= 1);


	if (f_v) {
		cout << "line_sets_with_transversal_horizontal_slice::transporter_from_point_to_orbit_rep" << endl;
	}

	Orbits->transporter_from_point_to_orbit_rep(
			pt,
		orbit_idx, Elt, verbose_level - 1);

	if (f_v) {
		cout << "line_sets_with_transversal_horizontal_slice::transporter_from_point_to_orbit_rep done" << endl;
	}

}


