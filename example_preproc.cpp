/*
*
* Creado: 02/20/24
* Objetivo: Crear macros
*
*/

#include "orbiter.h"
#include <vector>

using namespace std;
using namespace orbiter;

#include "joel.h"



int main(int argc, const char **argv)
{

	orbiter::layer5_applications::user_interface::orbiter_top_level_session Orbiter;

	int verbose_level = 0;
	int f_v = (verbose_level >= 1);

	std::string *Argv;
	data_structures::string_tools ST;
	int i;
	int f_q = false;
	int q = 0;

	//cout << "before ST.convert_arguments, argc=" << argc << endl;

	ST.convert_arguments(argc, argv, Argv);
		// argc has changed!

	for (i = 1; i < argc; i++) {
		if (ST.stringcmp(Argv[i], "-v") == 0) {
			verbose_level = ST.strtoi(Argv[++i]);
			f_v = (verbose_level >= 1);
			if (f_v) {
				cout << "-v " << verbose_level << endl;
			}
		}
		else if (ST.stringcmp(Argv[i], "-q") == 0) {
			f_q = true;
			q = ST.strtoi(Argv[++i]);
			if (f_v) {
				cout << "-q " << q << endl;
			}
		}
		else {
			cout << "unknown command line option " << Argv[i] << endl;
			exit(1);
		}
	}

	if (!f_q) {
		cout << "please use option -q to specify the field size" << endl;
		exit(1);
	}
	//int verbose_level = 2;


	if (f_v) {
		cout << "field size q = " << q << endl;
	}

	field_theory::finite_field *F_q;


	F_q = NEW_OBJECT(field_theory::finite_field);


	orbiter::layer1_foundations::geometry::projective_space *PG;

	F_q->finite_field_init_small_order(q,false,true,0);

	PG = NEW_OBJECT(orbiter::layer1_foundations::geometry::projective_space);


	int n = 3;

	PG->projective_space_init(n,F_q,0,0);

	classify_line_sets_with_transversal C;
	int target_size = 5;

	if (f_v) {
		cout << "main before classify_line_sets_with_transversal::init" << endl;
	}
	C.init(PG, target_size, verbose_level);
	if (f_v) {
		cout << "main after classify_line_sets_with_transversal::init" << endl;
	}


	FREE_OBJECT(PG);
	FREE_OBJECT(F_q);

	return 0;
}



