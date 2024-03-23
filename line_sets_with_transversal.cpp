/*
 * line_sets_with_transversal.cpp
 *
 *  Created on: Mar 5, 2024
 *      Author: betten
 */

#include "orbiter.h"
#include <vector>

using namespace std;
using namespace orbiter;

#include "joel.h"

line_sets_with_transversal::line_sets_with_transversal()
{
	Classify_line_sets_with_transversal = NULL;
	A = NULL;
	A_on_lines = NULL;
	size = 0;
	f_has_seed = false;
	Seed = NULL;
	Previous_one = NULL;
	Horizontal_slice = NULL;
	nb_orbits_in = 0;
	nb_flag_orbits = 0;
	pt_representation_sz = 0;
	dataset = NULL;
	dataset2 = NULL;
	dataset3 = NULL;
	Flag_orbits = NULL;
	Po = NULL;
	Trees = NULL;
	Next_one = NULL;
}

line_sets_with_transversal::~line_sets_with_transversal()
{

}

void line_sets_with_transversal::init(
		actions::action *A,
		actions::action *A_on_lines,
		int size,
		classify_line_sets_with_transversal *Classify_line_sets_with_transversal,
		int verbose_level)
{
	int f_v = (verbose_level >= 1);


	if (f_v) {
		cout << "line_sets_with_transversal::init" << endl;
	}

	line_sets_with_transversal::Classify_line_sets_with_transversal = Classify_line_sets_with_transversal;
	line_sets_with_transversal::A = A;
	line_sets_with_transversal::A_on_lines = A_on_lines;
	line_sets_with_transversal::size = size;
	Flag_orbits = NULL;
	Next_one = NULL;

	pt_representation_sz = size + 1;

	dataset = NEW_lint(pt_representation_sz);
	dataset2 = NEW_lint(pt_representation_sz);
	dataset3 = NEW_lint(pt_representation_sz);

	if (f_v) {
		cout << "line_sets_with_transversal::init done" << endl;
	}
}

void line_sets_with_transversal::init_seed(
		line_sets_with_transversal_seed *Seed,
		int verbose_level)
{
	int f_v = (verbose_level >= 1);


	if (f_v) {
		cout << "line_sets_with_transversal::init_seed" << endl;
	}
	f_has_seed = true;
	line_sets_with_transversal::Seed = Seed;
	if (f_v) {
		cout << "line_sets_with_transversal::init_seed done" << endl;
	}
}

void line_sets_with_transversal::init_previous_one(
		line_sets_with_transversal *Previous_one,
		int verbose_level)
{
	int f_v = (verbose_level >= 1);


	if (f_v) {
		cout << "line_sets_with_transversal::init_previous_one" << endl;
	}
	f_has_seed = false;
	line_sets_with_transversal::Previous_one = Previous_one;
	if (f_v) {
		cout << "line_sets_with_transversal::init_previous_one done" << endl;
	}
}

int line_sets_with_transversal::nb_input_orbits()
{
	int nb = 0;

	if (f_has_seed) {
		nb = Seed->nb_orbits();
	}
	else {
		nb = Previous_one->Next_one->nb_orbits;
	}
	return nb;
}

int line_sets_with_transversal::nb_output_orbits()
{
	int nb = 0;

	nb = Next_one->nb_orbits;
	return nb;
}

void line_sets_with_transversal::get_admissible_set(
		int orbit_idx,
		long int *&admissible_set, int &admissible_set_size,
		int verbose_level)
{
	int f_v = (verbose_level >= 1);


	if (f_v) {
		cout << "line_sets_with_transversal::get_admissible_set "
				"orbit_idx = " << orbit_idx << endl;
	}

	if (f_has_seed) {
		Seed->get_admissible_set(
				orbit_idx,
				admissible_set, admissible_set_size,
				verbose_level);
	}
	else {

		line_sets_with_transversal *Previous_level;

		Previous_level = &Classify_line_sets_with_transversal->Lifting[size - 1];

		int f, po;

		if (!Previous_level->Next_one) {
			cout << "line_sets_with_transversal::get_admissible_set "
					"!Previous_level->Next_one" << endl;
			exit(1);
		}

		if (orbit_idx >= Previous_level->Next_one->nb_orbits) {
			cout << "line_sets_with_transversal::get_admissible_set "
					"orbit_idx >= Previous_level->Next_one->nb_orbits" << endl;
			exit(1);
		}

		f = Previous_level->Next_one->Orbit[orbit_idx].flag_orbit_idx;

		if (f_v) {
			cout << "line_sets_with_transversal::get_admissible_set f = " << f << endl;
		}

		po = Previous_level->Po[f];

		if (f_v) {
			cout << "line_sets_with_transversal::get_admissible_set po = " << po << endl;
		}


		admissible_set_size = Previous_level->Horizontal_slice[po].admissible_set_size;
		admissible_set = NEW_lint(admissible_set_size);
		Lint_vec_copy(Previous_level->Horizontal_slice[po].admissible_set,
				admissible_set, admissible_set_size);

		if (f_v) {
			cout << "line_sets_with_transversal::get_admissible_set "
					"admissible_set_size = " << admissible_set_size << endl;
		}

	}

	if (f_v) {
		cout << "line_sets_with_transversal::get_admissible_set" << endl;
	}


}

void line_sets_with_transversal::get_primary_orbit_rep(
		int orbit_idx, int *primary_orbit_rep, int verbose_level)
{
	int f_v = (verbose_level >= 1);


	if (f_v) {
		cout << "line_sets_with_transversal::get_primary_orbit_rep "
				"size = " << size << " orbit_idx=" << orbit_idx << endl;
	}


	if (size == 0) {
		primary_orbit_rep[0] = 0;
	}
	else {

		line_sets_with_transversal *Previous_level;

		Previous_level = &Classify_line_sets_with_transversal->Lifting[size - 1];


		Lint_vec_copy_to_int(
				Previous_level->Next_one->Rep_ith(orbit_idx),
				primary_orbit_rep, Previous_level->Next_one->representation_sz);
		if (f_v) {
			cout << "line_sets_with_transversal::get_primary_orbit_rep "
					"size = " << size << " orbit_idx=" << orbit_idx << " : ";
			Int_vec_print(cout, primary_orbit_rep,
					Previous_level->Next_one->representation_sz);
			cout << endl;
		}

	}

	if (f_v) {
		cout << "line_sets_with_transversal::get_primary_orbit_rep done" << endl;
	}

}


data_structures_groups::set_and_stabilizer *line_sets_with_transversal::get_previous_set_and_stabilizer(
		int orbit_idx, int verbose_level)
{
	int f_v = (verbose_level >= 1);


	if (f_v) {
		cout << "line_sets_with_transversal::get_previous_set_and_stabilizer" << endl;
	}

	data_structures_groups::set_and_stabilizer *SaS;



	if (size == 0) {
		SaS = NEW_OBJECT(data_structures_groups::set_and_stabilizer);

		SaS->init(A, A_on_lines, 0 /*verbose_level */);

		SaS->allocate_data(
				size, 0 /* verbose_level */);

		SaS = Seed->get_set_and_stabilizer(orbit_idx, verbose_level);
		SaS->sz = 0;
	}
	else {

		line_sets_with_transversal *Previous_level;

		Previous_level = &Classify_line_sets_with_transversal->Lifting[size - 1];


		SaS = Previous_level->get_output_set_and_stabilizer(
				orbit_idx,
				verbose_level);

		if (f_v) {
			cout << "line_sets_with_transversal::get_previous_set_and_stabilizer "
					"size = " << size << " orbit_idx=" << orbit_idx << " : ";
			Lint_vec_print(cout, SaS->data, SaS->sz);
			cout << endl;
		}

		if (f_v) {
			cout << "line_sets_with_transversal::get_previous_set_and_stabilizer "
					"The stabilizer of the orbit representative is:" << endl;
			SaS->Strong_gens->print_generators_tex(cout);
		}
	}

	if (f_v) {
		cout << "line_sets_with_transversal::get_previous_set_and_stabilizer done" << endl;
	}

	return SaS;
}


data_structures_groups::set_and_stabilizer *line_sets_with_transversal::get_output_set_and_stabilizer(
		int orbit_idx, int verbose_level)
{
	int f_v = (verbose_level >= 1);


	if (f_v) {
		cout << "line_sets_with_transversal::get_output_set_and_stabilizer" << endl;
	}

	data_structures_groups::set_and_stabilizer *SaS;

	if (f_v) {
		cout << "line_sets_with_transversal::get_output_set_and_stabilizer "
				"before Next_one->get_set_and_stabilizer" << endl;
	}
	SaS = Next_one->get_set_and_stabilizer(
			orbit_idx, verbose_level);
	if (f_v) {
		cout << "line_sets_with_transversal::get_output_set_and_stabilizer "
				"after Next_one->get_set_and_stabilizer" << endl;
	}


	if (f_v) {
		cout << "line_sets_with_transversal::get_output_set_and_stabilizer done" << endl;
	}

	return SaS;

}

void line_sets_with_transversal::classify(
		int verbose_level)
{
	int f_v = (verbose_level >= 1);


	if (f_v) {
		cout << "line_sets_with_transversal::classify" << endl;
		cout << "line_sets_with_transversal::classify size=" << size << endl;
	}

	if (f_v) {
		cout << "line_sets_with_transversal::classify "
				"before downstep" << endl;
	}
	downstep(verbose_level);
	if (f_v) {
		cout << "line_sets_with_transversal::classify "
				"after downstep" << endl;

			cout << "we found " << Flag_orbits->nb_flag_orbits
					<< " flag orbits out of "
					<< nb_input_orbits()
					<< " orbits" << endl;
	}

	if (f_v) {
		cout << "line_sets_with_transversal::classify "
				"before upstep" << endl;
	}
	upstep(verbose_level);
	if (f_v) {
		cout << "line_sets_with_transversal::classify "
				"after upstep" << endl;
		cout << "we found " << Next_one->nb_orbits
				<< " orbits by considering "
				<< Flag_orbits->nb_flag_orbits
				<< " flag orbits" << endl;
	}

	if (f_v) {
		cout << "line_sets_with_transversal::classify done" << endl;
	}
}


void line_sets_with_transversal::downstep(
		int verbose_level)
{
	int f_v = (verbose_level >= 1);

	if (f_v) {
		cout << "line_sets_with_transversal::downstep" << endl;
		cout << "verbose_level = " << verbose_level << endl;
	}


	nb_orbits_in = nb_input_orbits();

	if (f_v) {
		cout << "line_sets_with_transversal::downstep" << endl;
		cout << "nb_orbits_in = " << nb_orbits_in << endl;
	}

	Horizontal_slice = NEW_OBJECTS(line_sets_with_transversal_horizontal_slice, nb_orbits_in);

	int po;

	nb_flag_orbits = 0;
	for (po = 0; po < nb_orbits_in; po++) {


		if (f_v) {
			cout << "line_sets_with_transversal::downstep "
					"po = " << po << endl;
			cout << "line_sets_with_transversal::downstep "
					"before H[" << po << "].init" << endl;
		}
		Horizontal_slice[po].init(
				Classify_line_sets_with_transversal,
				this,
				po,
				verbose_level);
		if (f_v) {
			cout << "line_sets_with_transversal::downstep "
					"po = " << po << endl;
			cout << "line_sets_with_transversal::downstep "
					"number of flag orbits in this case = "
					<< Horizontal_slice[po].Orbits->Sch->nb_orbits << endl;
			cout << "line_sets_with_transversal::downstep "
					"after Horizontal_slice["<< po << "].init" << endl;
		}

		nb_flag_orbits += Horizontal_slice[po].nb_flag_orbits_in_horizontal_slice;

	}

	if (f_v) {
		cout << "line_sets_with_transversal::downstep "
				"nb_flag_orbits = " << nb_flag_orbits << endl;
	}

	Po = NEW_int(nb_flag_orbits);

	int nb_f, so;

	nb_f = 0;
	for (po = 0; po < nb_orbits_in; po++) {

		Horizontal_slice[po].first_flag_orbit_idx = nb_f;
		for (so = 0; so < Horizontal_slice[po].nb_flag_orbits_in_horizontal_slice; so++) {
			Po[nb_f + so] = po;
		}
		nb_f += Horizontal_slice[po].nb_flag_orbits_in_horizontal_slice;
	}
	if (nb_f != nb_flag_orbits) {
		cout << "nb_f != nb_flag_orbits" << endl;
		exit(1);
	}


	Flag_orbits = NEW_OBJECT(invariant_relations::flag_orbits);
	Flag_orbits->init(
			A,
			A_on_lines,
			nb_orbits_in, // nb_primary_orbits_lower,
		pt_representation_sz,
		nb_flag_orbits, // nb_flag_orbits
		pt_representation_sz, // upper_bound_for_number_of_traces
		NULL, // void (*func_to_free_received_trace)(void *trace_result, void *data, int verbose_level)
		NULL, // void (*func_latex_report_trace)(std::ostream &ost, void *trace_result, void *data, int verbose_level)
		NULL, // void *free_received_trace_data
		verbose_level);

	if (f_v) {
		cout << "line_sets_with_transversal::downstep "
				"initializing flag orbits" << endl;
	}


	nb_f = 0;
	for (po = 0; po < nb_orbits_in; po++) {

		for (so = 0; so < Horizontal_slice[po].Orbits->Sch->nb_orbits; so++) {

			groups::strong_generators *Stab_gens;
			int orbit_rep;

			Horizontal_slice[po].Orbits->stabilizer_of_orbit_representative(
						so /* orbit_idx */,
						Stab_gens,
						orbit_rep,
						0 /*verbose_level*/);


			Int_vec_copy_to_lint(Horizontal_slice[po].primary_orbit_rep, dataset, size);


			dataset[size] = Horizontal_slice[po].original_point(orbit_rep);



			Flag_orbits->Flag_orbit_node[nb_f + so].init(
					Flag_orbits,
					nb_f + so, po, so,
					Horizontal_slice[po].Orbits->Sch->orbit_len[so],
					false /*f_long_orbit*/,
					dataset,
					Stab_gens,
					verbose_level);
			Po[nb_f + so] = po;
		}

		nb_f += Horizontal_slice[po].Orbits->Sch->nb_orbits;

	}


	if (f_v) {
		cout << "line_sets_with_transversal::downstep we found "
			<< nb_flag_orbits << " flag orbits out of "
			<< nb_orbits_in << " input orbits" << endl;
	}

	if (f_v) {
		cout << "line_sets_with_transversal::downstep "
				"flag orbits: " << endl;
		Flag_orbits->print();
	}

	if (f_v) {
		cout << "line_sets_with_transversal::downstep "
				"all A_on_admissible_set:" << endl;
	}
	for (po = 0; po < nb_orbits_in; po++) {

		Horizontal_slice[po].A_on_admissible_set->print_info();

	}

	if (f_v) {
		cout << "line_sets_with_transversal::downstep "
				"initializing flag orbits done" << endl;
	}
}


void line_sets_with_transversal::upstep(
		int verbose_level)
{
	int f_v = (verbose_level >= 1);

	if (f_v) {
		cout << "line_sets_with_transversal::upstep" << endl;
		cout << "verbose_level = " << verbose_level << endl;
	}

	int f_vv = false; //(verbose_level >= 2);
	//int i;
	int f, po, so;
	int *f_processed;
	int nb_processed;
	data_structures::sorting Sorting;

	f_processed = NEW_int(Flag_orbits->nb_flag_orbits);
	Int_vec_zero(f_processed, Flag_orbits->nb_flag_orbits);
	nb_processed = 0;

	Next_one = NEW_OBJECT(invariant_relations::classification_step);

	ring_theory::longinteger_object go;
	A->group_order(go);

	Next_one->init(
			A,
			A_on_lines,
			Flag_orbits->nb_flag_orbits, /* max_orbits */
			pt_representation_sz,  /* representation_sz */
			go,
			verbose_level - 2);


	if (f_v) {
		cout << "flag orbit : downstep_primary_orbit" << endl;
		cout << "f : po" << endl;
		for (f = 0; f < Flag_orbits->nb_flag_orbits; f++) {
			po = Flag_orbits->Flag_orbit_node[f].downstep_primary_orbit;
			cout << f << " : " << po << endl;
		}
	}
	for (f = 0; f < Flag_orbits->nb_flag_orbits; f++) {

		double progress;
	//	long int dataset[23];

		if (f_processed[f]) {
			continue;
		}

		progress = ((double)nb_processed * 100. ) /
				(double) Flag_orbits->nb_flag_orbits;

		if (f_v) {
			cout << "line_sets_with_transversal::upstep "
				"Defining new orbit "
				<< Flag_orbits->nb_primary_orbits_upper
				<< " from flag orbit " << f << " / "
				<< Flag_orbits->nb_flag_orbits
				<< " progress=" << progress << "%" << endl;
		}
		Flag_orbits->Flag_orbit_node[f].upstep_primary_orbit
			= Flag_orbits->nb_primary_orbits_upper;

		po = Flag_orbits->Flag_orbit_node[f].downstep_primary_orbit;
		so = Flag_orbits->Flag_orbit_node[f].downstep_secondary_orbit;
		if (f_v) {
			cout << "line_sets_with_transversal::upstep "
					"f=" << f << " po=" << po << " so=" << so << endl;
		}
		Lint_vec_copy(Flag_orbits->Pt + f * pt_representation_sz,
				dataset, pt_representation_sz);



		int *Elt_transporter;

		Elt_transporter = NEW_int(A->elt_size_in_int);

		data_structures_groups::vector_ge *coset_reps;
		int nb_coset_reps;

		coset_reps = NEW_OBJECT(data_structures_groups::vector_ge);
		coset_reps->init(A, verbose_level - 2);
		coset_reps->allocate(pt_representation_sz, verbose_level - 2);


		groups::strong_generators *S;
		ring_theory::longinteger_object go;


		S = Flag_orbits->Flag_orbit_node[f].gens->create_copy(verbose_level - 2);
		S->group_order(go);
		if (f_v) {
			cout << "line_sets_with_transversal::upstep "
					"f=" << f << " po=" << po << " so=" << so << " go=" << go << endl;
		}

		int coset_index;

		nb_coset_reps = 0;
		for (coset_index = 0; coset_index < pt_representation_sz; coset_index++) {

			if (f_v) {
				cout << "line_sets_with_transversal::upstep "
						"coset_index=" << coset_index << endl;
			}


			pick_flag_in_down_set(coset_index, verbose_level - 2);


			if (f_v) {
				cout << "line_sets_with_transversal::upstep "
						"f=" << f << " po=" << po << " so=" << so
						<< " coset_index=" << coset_index << endl;
			}
			if (f_v) {
				cout << "line_sets_with_transversal::upstep "
						"dataset=";
				Lint_vec_print(cout, dataset, pt_representation_sz);
				cout << endl;
				cout << "line_sets_with_transversal::upstep dataset2=";
				Lint_vec_print(cout, dataset2, pt_representation_sz);
				cout << endl;
			}

			line_sets_with_transversal *First_level;

			First_level = &Classify_line_sets_with_transversal->Lifting[0];
			if (f_v) {
				cout << "line_sets_with_transversal::upstep "
						"First_level->size=" << First_level->size << endl;
			}

			int flag_orbit_idx;
			int po2, so2;

			if (f_v) {
				cout << "line_sets_with_transversal::upstep "
						"before First_level->trace_flag" << endl;
			}
			flag_orbit_idx = First_level->trace_flag(
					dataset2, size + 1,
					size,
					dataset3 /* canonical_set */, Elt_transporter,
					po2, so2,
					verbose_level);
			if (f_v) {
				cout << "line_sets_with_transversal::upstep "
						"after First_level->trace_flag" << endl;
			}

			if (f_v) {
				cout << "line_sets_with_transversal::upstep "
						"f=" << f << " po=" << po << " so=" << so
						<< " coset_index=" << coset_index << endl;
				cout << "line_sets_with_transversal::upstep "
						"f2=" << flag_orbit_idx << " po2=" << po2 << " so2=" << so2
						<< " coset_index=" << coset_index << endl;
			}

			if (flag_orbit_idx == f) {
				if (f_v) {
					cout << "line_sets_with_transversal::upstep "
							"automorphism event" << endl;
				}
				A->Group_element->element_move(
						Elt_transporter, coset_reps->ith(nb_coset_reps), 0);
				nb_coset_reps++;

			}
			else {
				if (f_v) {
					cout << "line_sets_with_transversal::upstep "
							"isomorphism event" << endl;
				}
				if (f_v) {
					cout << "line_sets_with_transversal::upstep "
							"We are identifying flag orbit "
							<< flag_orbit_idx << " with flag orbit " << f << endl;
				}
				if (!f_processed[flag_orbit_idx]) {
					Flag_orbits->Flag_orbit_node[flag_orbit_idx].upstep_primary_orbit
						= Flag_orbits->nb_primary_orbits_upper;
					Flag_orbits->Flag_orbit_node[flag_orbit_idx].f_fusion_node
						= true;
					Flag_orbits->Flag_orbit_node[flag_orbit_idx].fusion_with
						= f;
					Flag_orbits->Flag_orbit_node[flag_orbit_idx].fusion_elt
						= NEW_int(A->elt_size_in_int);
					A->Group_element->element_invert(
							Elt_transporter,
							Flag_orbits->Flag_orbit_node[flag_orbit_idx].fusion_elt,
							0);
					f_processed[flag_orbit_idx] = true;
					nb_processed++;
				}
				else {
					cout << "line_sets_with_transversal::upstep "
							"Flag orbit " << flag_orbit_idx << " has already been "
							"identified with flag orbit " << f << endl;
					if (Flag_orbits->Flag_orbit_node[flag_orbit_idx].fusion_with != f) {
						cout << "line_sets_with_transversal::upstep "
								"Flag_orbits->Flag_orbit_node[f2]."
								"fusion_with != f" << endl;
						exit(1);
					}
				}

			} // else



		} // next i



		coset_reps->reallocate(nb_coset_reps, verbose_level - 2);

		groups::strong_generators *Aut_gens;

		{
			ring_theory::longinteger_object ago;

			if (f_v) {
				cout << "line_sets_with_transversal::upstep "
						"Extending the group by a factor of "
						<< nb_coset_reps << endl;
			}
			Aut_gens = NEW_OBJECT(groups::strong_generators);
			Aut_gens->init_group_extension(S,
					coset_reps, nb_coset_reps,
					verbose_level - 4);
			if (f_v) {
				cout << "line_sets_with_transversal::upstep "
						"Aut_gens tl = ";
				Int_vec_print(cout,
						Aut_gens->tl, Aut_gens->A->base_len());
				cout << endl;
			}

			Aut_gens->group_order(ago);


			if (f_v) {
				cout << "line_sets_with_transversal "
						"the new orbit representative has a stabilizer of order "
						<< ago << endl;
			}
			if (f_vv) {
				cout << "line_sets_with_transversal "
						"The automorphism group of the new orbit representative is:" << endl;
				Aut_gens->print_generators_tex(cout);
			}
		}


		if (f_v) {
			cout << "line_sets_with_transversal::upstep new orbit "
					<< Flag_orbits->nb_primary_orbits_upper
					<< " will be created" << endl;
		}

		Next_one->Orbit[Flag_orbits->nb_primary_orbits_upper].init(
				Next_one,
			Flag_orbits->nb_primary_orbits_upper,
			f,
			Aut_gens,
			dataset,
			NULL /* extra_data */,
			verbose_level - 2);

		if (f_v) {
			cout << "line_sets_with_transversal::upstep new orbit "
					<< Flag_orbits->nb_primary_orbits_upper
					<< " has been created" << endl;
		}

		FREE_OBJECT(coset_reps);
		FREE_OBJECT(S);

		f_processed[f] = true;
		nb_processed++;
		Flag_orbits->nb_primary_orbits_upper++;
	} // next f


	if (nb_processed != Flag_orbits->nb_flag_orbits) {
		cout << "line_sets_with_transversal "
				"nb_processed != Flag_orbits->nb_flag_orbits" << endl;
		cout << "nb_processed = " << nb_processed << endl;
		cout << "Flag_orbits->nb_flag_orbits = "
				<< Flag_orbits->nb_flag_orbits << endl;
		exit(1);
	}

	Next_one->nb_orbits = Flag_orbits->nb_primary_orbits_upper;

	if (f_v) {
		cout << "line_sets_with_transversal::upstep "
				"We found " << Flag_orbits->nb_primary_orbits_upper
				<< " orbits of size " << size + 1 << endl;

		print_output_orbits();
	}


	if (f_v) {
		cout << "line_sets_with_transversal::upstep before make_trees" << endl;
	}
	make_trees(verbose_level);
	if (f_v) {
		cout << "line_sets_with_transversal::upstep after make_trees" << endl;
	}

	FREE_int(f_processed);

	if (f_v) {
		cout << "line_sets_with_transversal::upstep done" << endl;
	}
}

void line_sets_with_transversal::print_output_orbits()
{
	int orbit_idx;
	int verbose_level = 0;

	cout << "# of orbits at level " << size + 1 << " is " << Next_one->nb_orbits << endl;

	for (orbit_idx = 0; orbit_idx < Next_one->nb_orbits; orbit_idx++) {
		data_structures_groups::set_and_stabilizer *SaS;

		SaS = Next_one->get_set_and_stabilizer(
			orbit_idx, verbose_level);

		cout << "orbit " << orbit_idx << " / " << Next_one->nb_orbits << " is : ";
		SaS->print_set_tex(cout);
		cout << endl;

		FREE_OBJECT(SaS);
	}

}

void line_sets_with_transversal::make_trees(
		int verbose_level)
{
	int f_v = (verbose_level >= 1);


	if (f_v) {
		cout << "line_sets_with_transversal::make_trees" << endl;
	}

	Trees = (graph_theory::layered_graph **) NEW_pvoid(nb_flag_orbits);

	int f;
	int po;
	int so;

	for (f = 0; f < nb_flag_orbits; f++) {

		po = Po[f];
		so = f - Horizontal_slice[po].first_flag_orbit_idx;

		if (f_v) {
			cout << "orbit " << so << " / " <<  Horizontal_slice[po].Orbits->Sch->nb_orbits
					<< " before Sch->export_tree_as_layered_graph" << endl;
		}

		Horizontal_slice[po].Orbits->Sch->export_tree_as_layered_graph(
				so,
				Trees[f],
				verbose_level - 1);
		if (f_v) {
			cout << "orbit " << so << " / " <<  Horizontal_slice[po].Orbits->Sch->nb_orbits
					<< " after Sch->export_tree_as_layered_graph" << endl;
		}
	}

}


void line_sets_with_transversal::pick_flag_in_down_set(
		int coset_index, int verbose_level)
{
	int f_v = (verbose_level >= 1);


	if (f_v) {
		cout << "line_sets_with_transversal::pick_flag_in_down_set" << endl;
		cout << "line_sets_with_transversal::pick_flag_in_down_set "
				"coset_index " << coset_index << endl;
	}

	Lint_vec_copy(dataset,
			dataset2, pt_representation_sz - 1 - coset_index);
	Lint_vec_copy(dataset + pt_representation_sz - coset_index,
			dataset2 + pt_representation_sz - 1 - coset_index, coset_index);
	dataset2[pt_representation_sz - 1] =
			dataset[pt_representation_sz - 1 - coset_index];

	if (f_v) {
		cout << "line_sets_with_transversal::pick_flag_in_down_set" << endl;
	}
}

int line_sets_with_transversal::trace_flag(
		long int *set, int size,
		int level,
		long int *canonical_set, int *Elt_transporter,
		int &po, int &so,
		int verbose_level)
{
	int f_v = (verbose_level >= 1);


	if (f_v) {
		cout << "line_sets_with_transversal::trace_flag" << endl;
		cout << "line_sets_with_transversal::trace_flag "
				"size=" << size << " level=" << level << endl;
	}

	int *tmp_Elt1;
	int *tmp_Elt2;

	tmp_Elt1 = NEW_int(Classify_line_sets_with_transversal->A->elt_size_in_int);
	tmp_Elt2 = NEW_int(Classify_line_sets_with_transversal->A->elt_size_in_int);

	if (f_v) {
		cout << "line_sets_with_transversal::trace_flag" << endl;
		cout << "tracing set ";
		Lint_vec_print(cout, set, size);
		cout << endl;
		cout << "verbose_level=" << verbose_level << endl;
		cout << "level=" << level << endl;
	}

	Lint_vec_copy(set, canonical_set, size);

	Classify_line_sets_with_transversal->A->Group_element->element_one(Elt_transporter, 0);


	int flag_orbit_index;

	if (f_v) {
		cout << "line_sets_with_transversal::trace_flag "
				"before line_sets_with_transversal::trace_flag" << endl;
	}
	flag_orbit_index = trace_flag_recursion(
		0 /* cur_level */, 0 /* cur_po */,
		size, level,
		canonical_set,
		Elt_transporter, tmp_Elt1, tmp_Elt2,
		po, so,
		verbose_level - 1);
	if (f_v) {
		cout << "line_sets_with_transversal::trace_flag "
				"after line_sets_with_transversal::trace_flag" << endl;
		cout << "flag_orbit_index = " << flag_orbit_index << endl;
	}

	FREE_int(tmp_Elt1);
	FREE_int(tmp_Elt2);

	if (f_v) {
		cout << "line_sets_with_transversal::trace_flag done" << endl;
	}
	return flag_orbit_index;

}


int line_sets_with_transversal::trace_flag_recursion(
	int cur_level, int cur_po,
	int size, int level,
	long int *canonical_set,
	int *Elt_transporter, int *tmp_Elt1, int *tmp_Elt2,
	int &po, int &so,
	int verbose_level)
{
	int f_v = (verbose_level >= 1);
	//int f_vv = (verbose_level >= 2);

	if (f_v) {
		cout << "line_sets_with_transversal::trace_flag_recursion" << endl;
	}
	if (f_v) {
		cout << "line_sets_with_transversal::trace_flag_recursion "
				"cur_level = " << cur_level
				<< " cur_po = " << cur_po << " : ";
		Lint_vec_print(cout, canonical_set, size);
		cout << endl;
	}

	long int pt, pt2;
	int orbit_idx, orbit_rep;

	pt = canonical_set[cur_level];

	if (f_v) {
		cout << "line_sets_with_transversal::trace_flag_recursion "
				"pt=" << pt << endl;
	}
	pt2 = Horizontal_slice[cur_po].restricted_point(pt);
	if (f_v) {
		cout << "line_sets_with_transversal::trace_flag_recursion "
				"restricted pt=" << pt2 << endl;
	}

	if (f_v) {
		cout << "line_sets_with_transversal::trace_flag_recursion "
				"before transporter_from_point_to_orbit_rep" << endl;
	}
	Horizontal_slice[cur_po].transporter_from_point_to_orbit_rep(
			pt2,
		orbit_idx, tmp_Elt1, verbose_level);
	if (f_v) {
		cout << "line_sets_with_transversal::trace_flag_recursion "
				"after transporter_from_point_to_orbit_rep" << endl;
	}

	if (f_v) {
		cout << "line_sets_with_transversal::trace_flag_recursion "
				"before get_orbit_rep_unpacked" << endl;
	}
	Horizontal_slice[cur_po].get_orbit_rep_unpacked(
			orbit_idx, orbit_rep, verbose_level);
	if (f_v) {
		cout << "line_sets_with_transversal::trace_flag_recursion "
				"after get_orbit_rep_unpacked" << endl;
	}

	if (f_v) {
		cout << "line_sets_with_transversal::trace_flag_recursion "
				"tracing point " << pt << " in cur_po=" << cur_po
				<< " to orbit " << orbit_idx << " with rep " << orbit_rep << endl;
		cout << "line_sets_with_transversal::trace_flag_recursion beta = " << endl;
		A->Group_element->element_print_quick(tmp_Elt1, cout);
	}


	if (f_v) {
		cout << "line_sets_with_transversal::trace_flag_recursion "
				"before apply" << endl;
	}
	apply(
			canonical_set, size,
			Elt_transporter, tmp_Elt1 /* Elt_to_apply */, tmp_Elt2,
			verbose_level);
	if (f_v) {
		cout << "line_sets_with_transversal::trace_flag_recursion "
				"after apply" << endl;
	}

	int flag_orbit_idx;

	flag_orbit_idx = Horizontal_slice[cur_po].first_flag_orbit_idx + orbit_idx;
	if (f_v) {
		cout << "line_sets_with_transversal::trace_flag_recursion "
				"cur_level=" << cur_level << endl;
		cout << "line_sets_with_transversal::trace_flag_recursion "
				"cur_po=" << cur_po << " orbit_idx=" << orbit_idx
				<< " flag_orbit_idx=" << flag_orbit_idx << endl;
	}



	if (cur_level + 1 == size) {
		po = cur_po;
		so = orbit_rep;
		return flag_orbit_idx;
	}
	else {
		if (f_v) {
			cout << "line_sets_with_transversal::trace_flag_recursion "
					"flag_orbit_idx=" << flag_orbit_idx << endl;
		}


		invariant_relations::flag_orbit_node *Flag_orbit_node;

		Flag_orbit_node = &Flag_orbits->Flag_orbit_node[flag_orbit_idx];


		if (Flag_orbit_node->flag_orbit_index != flag_orbit_idx) {
			cout << "line_sets_with_transversal::trace_flag_recursion "
					"Flag_orbit_node->flag_orbit_index != flag_orbit_idx" << endl;
			exit(1);
		}

		int up_po;

		up_po = Flag_orbit_node->upstep_primary_orbit;

		if (Flag_orbit_node->f_fusion_node) {

			if (f_v) {
				cout << "line_sets_with_transversal::trace_flag_recursion "
						"The flag orbit is a fusion node" << endl;
			}

			int fusion_with;
			int *fusion_elt;

			fusion_with = Flag_orbit_node->fusion_with;
			fusion_elt = Flag_orbit_node->fusion_elt;

			if (f_v) {
				cout << "line_sets_with_transversal::trace_flag_recursion "
						"before applying fusion element" << endl;
			}
			apply(
					canonical_set, size,
					Elt_transporter, fusion_elt /* Elt_to_apply */, tmp_Elt2,
					verbose_level);
			if (f_v) {
				cout << "line_sets_with_transversal::trace_flag_recursion "
						"after applying fusion element" << endl;
			}

			up_po = fusion_with;

			line_sets_with_transversal *Next_level;

			Next_level = &Classify_line_sets_with_transversal->Lifting[cur_level + 1];

			if (f_v) {
				cout << "line_sets_with_transversal::trace_flag_recursion "
						"before Next_level->trace_flag_recursion" << endl;
			}

			flag_orbit_idx = Next_level->trace_flag_recursion(
					cur_level + 1, up_po,
					size, level,
					canonical_set,
					Elt_transporter, tmp_Elt1, tmp_Elt2,
					po, so,
					verbose_level);

			if (f_v) {
				cout << "line_sets_with_transversal::trace_flag_recursion "
						"after Next_level->trace_flag_recursion" << endl;
				cout << "line_sets_with_transversal::trace_flag_recursion "
						"flag_orbit_idx = " << flag_orbit_idx << endl;
			}


		}
		else {

			if (f_v) {
				cout << "line_sets_with_transversal::trace_flag_recursion "
						"The flag orbit is a definition node" << endl;
			}

			line_sets_with_transversal *Next_level;

			Next_level = &Classify_line_sets_with_transversal->Lifting[cur_level + 1];

			if (f_v) {
				cout << "line_sets_with_transversal::trace_flag_recursion "
						"before Next_level->trace_flag_recursion" << endl;
			}


			flag_orbit_idx = Next_level->trace_flag_recursion(
					cur_level + 1, up_po,
					size, level,
					canonical_set,
					Elt_transporter, tmp_Elt1, tmp_Elt2,
					po, so,
					verbose_level);

			if (f_v) {
				cout << "line_sets_with_transversal::trace_flag_recursion "
						"after Next_level->trace_flag_recursion" << endl;
				cout << "line_sets_with_transversal::trace_flag_recursion "
						"flag_orbit_idx = " << flag_orbit_idx << endl;
			}

		}
	}

	if (f_v) {
		cout << "line_sets_with_transversal::trace_flag_recursion done" << endl;
	}

	return flag_orbit_idx;
}

void line_sets_with_transversal::apply(
		long int *canonical_set, int size,
		int *Elt_transporter, int *Elt_to_apply, int *tmp_Elt,
		int verbose_level)
{
	int f_v = (verbose_level >= 1);

	if (f_v) {
		cout << "line_sets_with_transversal::apply" << endl;
	}
	if (f_v) {
		cout << "line_sets_with_transversal::applying group element:" << endl;
		A->Group_element->element_print_quick(Elt_to_apply, cout);
		cout << "line_sets_with_transversal::apply in action " << A_on_lines->label << endl;
	}
	if (f_v) {
		cout << "line_sets_with_transversal::apply before: ";
		Lint_vec_print(cout, canonical_set, size);
		cout << endl;
	}
	A->Group_element->element_mult(
			Elt_transporter, Elt_to_apply, tmp_Elt,
			0 /* verbose_level */);

	A->Group_element->element_move(
			tmp_Elt, Elt_transporter,
			0 /* verbose_level */);

	int i;

	for (i = 0; i < size; i++) {
		canonical_set[i] = A_on_lines->Group_element->element_image_of(
				canonical_set[i], Elt_to_apply,
				0 /* verbose_level */);
	}
	if (f_v) {
		cout << "line_sets_with_transversal::apply after: ";
		Lint_vec_print(cout, canonical_set, size);
		cout << endl;
	}

	if (f_v) {
		cout << "line_sets_with_transversal::apply done" << endl;
	}
}




