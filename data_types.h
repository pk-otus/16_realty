#pragma once
#include <dlib/clustering.h>
#include <dlib/svm_threaded.h>

namespace realty
{
	template<size_t SZ>
	using calculated_t = dlib::matrix<double, SZ, 1>;
	template<size_t SZ>
	using ovo_trainer = dlib::one_vs_one_trainer<dlib::any_trainer<calculated_t<SZ>> >;
	template<size_t SZ>
	using poly_kernel = dlib::polynomial_kernel<calculated_t<SZ>>;
	template<size_t SZ>
	using rbf_kernel = dlib::radial_basis_kernel<calculated_t<SZ>>;

	template<size_t SZ>
	using saved_classifier_function_t =
		dlib::one_vs_one_decision_function<ovo_trainer<SZ>,
		dlib::decision_function<poly_kernel<SZ>>,
		dlib::decision_function<rbf_kernel<SZ>>>;
}