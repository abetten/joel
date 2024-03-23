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



int main()
{
	orbiter::layer5_applications::user_interface::orbiter_top_level_session Orbiter;
	int q = 7;
	int verbose_level = 2;
	int f_v = (verbose_level >= 1);

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



