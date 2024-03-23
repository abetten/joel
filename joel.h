/*
 * joel.h
 *
 *  Created on: Mar 5, 2024
 *      Author: betten
 */

#ifndef SRC_JOEL_JOEL_H_
#define SRC_JOEL_JOEL_H_


// forward declarations of classes:
class line_sets_with_transversal_seed;
class line_sets_with_transversal_horizontal_slice;
class line_sets_with_transversal;
class classify_line_sets_with_transversal;

class line_sets_with_transversal_seed {
public:

	classify_line_sets_with_transversal *Classify_line_sets_with_transversal;

	actions::action *A;

	actions::action *A_on_lines;

	int size; // = 1
	int target_size;

	groups::orbits_on_something *Orbit_of_transversal_line;
		// the orbit of transversal_line_rk

	line_sets_with_transversal_seed();
	~line_sets_with_transversal_seed();
	void init(
			classify_line_sets_with_transversal *Classify_line_sets_with_transversal,
			actions::action *A,
			actions::action *A2,
			groups::orbits_on_something *Orbit_of_transversal_line,
			int target_size,
			int verbose_level);
	int nb_orbits();
	void get_admissible_set(
			int orbit_idx,
			long int *&admissible_set, int &admissible_set_size,
			int verbose_level);
	int get_orbit_rep(
			int orbit_idx, int verbose_level);
	data_structures_groups::set_and_stabilizer *get_set_and_stabilizer(
			int orbit_idx, int verbose_level);

};

class line_sets_with_transversal_horizontal_slice {

public:
	classify_line_sets_with_transversal *Classify_line_sets_with_transversal;
	line_sets_with_transversal *Lifting;
	int po;

	data_structures_groups::set_and_stabilizer *R;
	ring_theory::longinteger_object go;

	int *primary_orbit_rep; // [Lifting->size]

	long int *old_admissible_set;
	int old_admissible_set_size;

	long int *admissible_set;
	int admissible_set_size;

	actions::action *A_on_admissible_set;

	groups::orbits_on_something *Orbits;

	int first_flag_orbit_idx;
	int nb_flag_orbits_in_horizontal_slice;

	line_sets_with_transversal_horizontal_slice();
	~line_sets_with_transversal_horizontal_slice();
	void init(
			classify_line_sets_with_transversal *Classify_line_sets_with_transversal,
			line_sets_with_transversal *Lifting,
			int po,
			int verbose_level);
	void test_lines(
			int verbose_level);
	long int original_point(
			long int pt);
	long int restricted_point(
			long int pt);
	void get_orbit_rep(
			int orbit_idx, int &orbit_rep, int verbose_level);
	void get_orbit_rep_unpacked(
			int orbit_idx, int &orbit_rep, int verbose_level);
	void transporter_from_orbit_rep_to_point(
			int pt,
		int &orbit_idx, int *Elt, int verbose_level);
	void transporter_from_point_to_orbit_rep(
			int pt,
		int &orbit_idx, int *Elt, int verbose_level);


};



class line_sets_with_transversal {
public:

	classify_line_sets_with_transversal *Classify_line_sets_with_transversal;

	actions::action *A;

	actions::action *A_on_lines;

	int size;

	int f_has_seed;
	line_sets_with_transversal_seed *Seed;

	line_sets_with_transversal *Previous_one;

	// one for each input orbit:
	line_sets_with_transversal_horizontal_slice *Horizontal_slice; // [nb_orbits_in]

	int nb_orbits_in;
	int nb_flag_orbits;

	int pt_representation_sz; // size + 1
	long int *dataset; // [pt_representation_sz]
	long int *dataset2; // [pt_representation_sz]
	long int *dataset3; // [pt_representation_sz]

	invariant_relations::flag_orbits *Flag_orbits;

	int *Po; // [nb_flag_orbits]
	graph_theory::layered_graph **Trees; // [nb_flag_orbits]

	// output orbits:
	invariant_relations::classification_step *Next_one;


	line_sets_with_transversal();
	~line_sets_with_transversal();
	void init(
			actions::action *A,
			actions::action *A_on_lines,
			int size,
			classify_line_sets_with_transversal *Classify_line_sets_with_transversal,
			int verbose_level);
	void init_seed(
			line_sets_with_transversal_seed *Seed,
			int verbose_level);
	void init_previous_one(
			line_sets_with_transversal *Previous_one,
			int verbose_level);
	int nb_input_orbits();
	int nb_output_orbits();
	void get_admissible_set(
			int orbit_idx,
			long int *&admissible_set, int &admissible_set_size,
			int verbose_level);
	void get_primary_orbit_rep(
			int orbit_idx, int *primary_orbit_rep, int verbose_level);
	data_structures_groups::set_and_stabilizer *get_previous_set_and_stabilizer(
			int orbit_idx, int verbose_level);
	data_structures_groups::set_and_stabilizer *get_output_set_and_stabilizer(
			int orbit_idx, int verbose_level);
	void classify(
			int verbose_level);
	void downstep(
			int verbose_level);
	void upstep(
			int verbose_level);
	void print_output_orbits();
	void make_trees(
			int verbose_level);
	void pick_flag_in_down_set(
			int coset_index, int verbose_level);
	int trace_flag(
			long int *set, int size,
			int level,
			long int *canonical_set, int *Elt_transporter,
			int &po, int &so,
			int verbose_level);
	int trace_flag_recursion(
		int cur_level, int cur_node,
		int size, int level,
		long int *canonical_set,
		int *Elt_transporter, int *tmp_Elt1, int *tmp_Elt2,
		int &po, int &so,
		int verbose_level);
	void apply(
			long int *canonical_set, int size,
			int *Elt_transporter, int *Elt_to_apply, int *tmp_Elt,
			int verbose_level);

};


class classify_line_sets_with_transversal {

public:

	orbiter::layer1_foundations::geometry::projective_space *PG;

	int target_size;
	int nb_lines;

	long int transversal_line_rk;

	actions::action *A;
	actions::action *A_on_lines;

	groups::strong_generators *Stab_transversal_line;
	ring_theory::longinteger_object go0;

	vector<long int> Linear_complex;
	long int *Linear_complex_Pts; // [Linear_complex_nb_pts]
	int Linear_complex_nb_pts; // = Linear_complex.size()

	// create the restricted action on rows:
	actions::action *A_on_linear_complex;

	std::string label_of_set;
	std::string label_of_set_tex;

	line_sets_with_transversal_seed *Seed;
	line_sets_with_transversal *Lifting; // [target_size + 1]

	int *Nb_orbits_in; // [target_size]
	int *Nb_orbits_out; // [target_size]
	int *Nb_orbits_flags; // [target_size]


	classify_line_sets_with_transversal();
	~classify_line_sets_with_transversal();
	void init(
			orbiter::layer1_foundations::geometry::projective_space *PG,
			int target_size,
			int verbose_level);
	void compute_linear_complex(
			int verbose_level);

};



#endif /* SRC_JOEL_JOEL_H_ */
