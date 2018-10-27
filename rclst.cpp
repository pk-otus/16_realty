#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <fstream>

#include "data_types.h"
#include "normalizer.h"
#include "sample_data.h"

namespace realty
{
	template<size_t SZ>
	class pre_calc
	{
	public:
		pre_calc(size_t n) :
			num_clusters(n),
			samples		(create_sample_data(std::cin, 4 == SZ)),
			calculated	(create_calculated_data()),
			norm		(normalizer<SZ>(calculated))
		{
			norm.normalize(calculated);
		}

		void save_all(const std::string& fname) const
		{
			auto labels = create_labels();

			save_clusters(fname, labels);
			norm.save_data(fname + ".nrm");
			save_classifier(fname + ".df", labels);
		}
	private:
		std::vector<calculated_t<SZ>> create_calculated_data()
		{
			std::vector<calculated_t<SZ>> result;
			result.reserve(samples.size());
			for (const auto& s : samples)
			{
				result.emplace_back(s.get_calculated<SZ>());
			}		

			return result;
		}

		std::vector<double> create_labels() const
		{
			dlib::kcentroid<rbf_kernel<SZ>>		kc(rbf_kernel<SZ>(0.1), 0.01, 8);
			dlib::kkmeans<rbf_kernel<SZ>>		test(kc);
			std::vector<calculated_t<SZ>>		initial_centers;

			test.set_number_of_centers(num_clusters);
			pick_initial_centers(num_clusters, initial_centers, calculated, test.get_kernel());

			test.train(calculated, initial_centers);

			auto labels = std::vector<double>();
			for (int i = 0; i < calculated.size(); ++i)
			{
				labels.push_back(test(calculated[i]));
			}

			return labels;
		}

		void save_clusters(const std::string& fname, const std::vector<double>& labels) const
		{
			std::vector<std::ofstream> cluster_files;
			for (int i = 0; i < num_clusters; ++i)
			{
				cluster_files.emplace_back(fname + '.' + std::to_string(i));
			}

			for (int i = 0; i < samples.size(); ++i)
			{
				cluster_files[labels[i]] << samples[i].to_string() << '\n';
			}
		}

		void save_classifier(const std::string& fname, const std::vector<double>& labels) const
		{
			ovo_trainer<SZ> trainer;

			dlib::krr_trainer<rbf_kernel<SZ>> rbf_trainer;
			dlib::svm_nu_trainer<poly_kernel<SZ>> poly_trainer;

			poly_trainer.set_kernel(poly_kernel<SZ>(0.1, 1, 2));
			rbf_trainer.set_kernel(rbf_kernel<SZ>(0.1));

			trainer.set_trainer(rbf_trainer);
			trainer.set_trainer(poly_trainer, 1, 2);

			dlib::one_vs_one_decision_function<ovo_trainer<SZ>> df = trainer.train(calculated, labels);

			saved_classifier_function_t<SZ> df2 = df;

			dlib::serialize(fname) << df2;
		}

		const size_t					num_clusters;
		std::vector<sample_data>		samples;
		std::vector<calculated_t<SZ>>	calculated;
		normalizer<SZ>					norm;
	};
}

int main(int argc, char const *argv[])
{
	if (argc < 3)
	{
		std::cout << "Usage: rclst <clusters quantity> <cluster file name> [mod]\n"
			"'mod' parameter uses modified clusterization with 4 tokens:\n"
			"latitude, longitude, rooms number and price per sqare meter\n";
		return 1;
	}

	int num_clusters = std::stoi(argv[1]);
	bool isModificationUsed = argc >= 4 && !strcmp("mod", argv[3]);

	if (isModificationUsed)
		realty::pre_calc<4>(num_clusters).save_all(argv[2]);
	else
		realty::pre_calc<7>(num_clusters).save_all(argv[2]);

	return 0;
}
