#pragma once

extern void generate_random_input();

#include <ostream>

#include "Johnson.hpp"

extern void display_output(std::ostream& file, const ans_matrix& d, const ans_matrix& pi);

extern void run_experiment();
