#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>

#include "sample_data.h"
#include "normalizer.h"

namespace realty
{
	template<size_t SZ>
	void find_realty(const std::string& fname, const normalizer<SZ>& norm)
	{
		saved_classifier_function_t<SZ> test;
		dlib::deserialize(fname + ".df") >> test;

		for (std::string line; std::getline(std::cin, line);)
		{
			auto s = sample_data(line);
			auto c = s.get_calculated<SZ>();
			norm.normalize_single(c);
			const std::string cluster_file = fname + '.' + std::to_string((int)test(c));
			std::cout << "\nCluster file: " << cluster_file << '\n';

			std::ifstream fin(cluster_file);
			auto samples = create_sample_data(fin, 4 == SZ);

			auto compare_by_distance = [&s](const sample_data& l, const sample_data& r)
			{
				return l.sq_distance(s) < r.sq_distance(s);
			};

			std::sort(samples.begin(), samples.end(), compare_by_distance);
			for (const auto& ss : samples)
			{
				std::cout << ss.to_string() << '\n';
			}

		}
	}
}
int main(int argc, char const *argv[])
{
	if (argc < 2)
	{
		std::cout << "Usage: rclss <cluster file name>\n";
		return 1;
	}
	std::string fname = argv[1];
	const std::string normalizer_file = fname + ".nrm";

	switch (realty::get_normalizer_size(normalizer_file))
	{
	case 4:
		find_realty(fname, realty::normalizer<4>(normalizer_file));
		break;
	case 7:
		find_realty(fname, realty::normalizer<7>(normalizer_file));
		break;
	}

	return 0;
}
