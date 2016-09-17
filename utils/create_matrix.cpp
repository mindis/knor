/*
 * Copyright 2016 neurodata (http://neurodata.io/)
 * Written by Disa Mhembere (disa@jhu.edu)
 *
 * This file is part of k-par-means
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY CURRENT_KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// Create a test matrix to check the conversion to other formats

#include <math.h>

#include <fstream>
#include <boost/log/trivial.hpp>

#include "types.hpp"

namespace kpmutil = kpmeans::utils;

int main (int argc, char* argv[]) {
	if (argc < 4) {
		fprintf(stderr, "usage: ./create_matrix nrow ncol [row/col/rrow/rcol]\n");
		exit(911);
	}

	const size_t nrow = atol(argv[1]);
	const size_t ncol = atol(argv[2]);
	std::string outfn = "matrix_r"+ std::to_string(nrow)+"_c"+std::to_string(ncol);
	std::string argv1 = std::string(argv[3]);

	const kpmutil::conv_layout lay = argv1 == "rrow" ? kpmutil::RAWROW
		: kpmutil::RAWCOL;

	if (lay == kpmutil::RAWROW || lay == kpmutil::RAWCOL) {
		int min = 1; int max = 5;

		double* dmat = new double [nrow*ncol];
		for (size_t i = 0; i < nrow*ncol; i++) {
			dmat[i] = min + ((double)random() / (double)RAND_MAX * (max - min));
		}

		BOOST_LOG_TRIVIAL(info) << "Writing the matrix";
		std::ofstream outfile;
		if (lay == kpmutil::RAWCOL)
			outfile.open(outfn+"_rcw.bin", std::ios::binary |
                    std::ios::trunc | std::ios::out);
		else
			outfile.open(outfn+"_rrw.bin", std::ios::binary |
                    std::ios::trunc | std::ios::out);

		outfile.write((char*)&dmat[0], (sizeof(double)*nrow*ncol));
		outfile.close();
		delete [] dmat;

#if 0
		BOOST_LOG_TRIVIAL(info) << "Test read of matrix";
		double* read_mat;
		if (lay == kpmutil::RAWCOL)
			read_mat = read_fg(outfn+"_cw.bin", kpmutil::RAWCOL, nrow, ncol);
		else
			read_mat = read_fg(outfn+"_rrw.bin", kpmutil::RAWROW, nrow, ncol);

		std::cout << "Read matrix: \n";
		print_mat(read_mat, nrow, ncol, lay);
		delete [] read_mat;
#endif
	} else {
		fprintf(stderr, "Unknown matrix type '%s'", argv[3]);
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}