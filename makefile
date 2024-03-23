SRC = ../../orbiter/src
ORBITER = $(SRC)/apps/orbiter
DISCRETA_CPP = g++
CPPFLAGS = -Wall -I$(SRC)/lib -std=c++14
LIB = $(SRC)/lib/liborbiter.a -lpthread
LFLAGS = -lm -Wl,-rpath -Wl,/usr/local/gcc-8.2.0/lib64

example_preproc.out:  classify_line_sets_with_transversal.o example_preproc.o line_sets_with_transversal.o line_sets_with_transversal_horizontal_slice.o line_sets_with_transversal_seed.o ;
	$(DISCRETA_CPP) \
		classify_line_sets_with_transversal.o \
		example_preproc.o \
		line_sets_with_transversal.o \
		line_sets_with_transversal_horizontal_slice.o \
		line_sets_with_transversal_seed.o \
		$(CPPFLAGS) $(LIB) $(LFLAGS) -o $@

grassmann_codes.out:  grassmann_codes.o ;
	$(DISCRETA_CPP) grassmann_codes.o $(CPPFLAGS) $(LIB) $(LFLAGS) -o $@

line_configurations.out: line_configurations.o ;
	$(DISCRETA_CPP) line_configurations.o $(CPPFLAGS) $(LIB) $(LFLAGS) -o $@

PG_3_2:
	$(ORBITER)/orbiter.out -v 1 \
		-define F -finite_field -q 2 -end \
		-define P -projective_space -n 3 -field F -v 0 -end \
		-with P -do -projective_space_activity \
			-cheat_sheet \
		-end

surface_classify_q3:
	$(ORBITER)/orbiter.out -v 4 \
		-define F -finite_field -q 3 -end \
		-define P -projective_space -n 3 -field F -v 0 -end \
		-define Control -poset_classification_control -W \
			-problem_label "surf_q3" \
		-end \
		-with P -do \
		-projective_space_activity \
			-classify_surfaces_with_double_sixes Surf27 Control \
		-end \
		-with Surf27 -do \
		-classification_of_cubic_surfaces_with_double_sixes_activity \
			-report -end \
		-end \
		-print_symbols

surface_classify_q4:
	$(ORBITER)/orbiter.out -v 4 \
		-define F -finite_field -q 4 -end \
		-define P -projective_space -n 3 -field F -v 0 -end \
		-define Control -poset_classification_control -W \
			-problem_label "surf_q4" \
		-end \
		-with P -do \
		-projective_space_activity \
			-classify_surfaces_with_double_sixes Surf27 Control \
		-end \
		-with Surf27 -do \
		-classification_of_cubic_surfaces_with_double_sixes_activity \
			-report -end \
		-end \
		-with Surf27 -do \
		-classification_of_cubic_surfaces_with_double_sixes_activity \
			-create_source_code \
		-end \
		-print_symbols

surface_classify_q7:
	$(ORBITER)/orbiter.out -v 50 \
		-define F -finite_field -q 7 -end \
		-define P -projective_space -n 3 -field F -v 0 -end \
		-define Control -poset_classification_control -gv 10 -W -end \
		-with P -do \
		-projective_space_activity \
			-classify_surfaces_with_double_sixes Surf27 Control \
		-end \
		-with Surf27 -do \
		-classification_of_cubic_surfaces_with_double_sixes_activity \
			-report -end \
		-end \
		-print_symbols

clean:
	-rm *.o *.a *.out *~
