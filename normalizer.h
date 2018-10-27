#pragma once
#include "sample_data.h"
#include <cassert>
#include <fstream>

namespace realty
{
	template<size_t SZ> class normalizer
	{
	public:		
		explicit normalizer(const std::vector<calculated_t<SZ>>& calculated)
		{
			for (int i = 0; i < SZ; ++i)
			{
				min_values[i] = std::numeric_limits<double>::infinity();
				max_values[i] = -std::numeric_limits<double>::infinity();
			}

			for (auto& n : calculated)
			{
				for (int i = 0; i < SZ; ++i)
				{
					if (n(i) < min_values[i]) min_values[i] = n(i);
					if (n(i) > max_values[i]) max_values[i] = n(i);
				}
			}

			init_norm_factors();			
		}

		explicit normalizer(const std::string& fname)
		{
			std::ifstream fin(fname);

			std::string line;
			std::getline(fin, line);

			assert(SZ == stoi(line));

			for (int i = 0; i < SZ; ++i)
			{
				std::getline(fin, line);
				auto delimiter = line.find_first_of(';', 0);
				min_values[i] = stod(line.substr(0, delimiter));
				max_values[i] = stod(line.substr(delimiter + 1));
			}

			init_norm_factors();
		}

		void normalize_single(calculated_t<SZ> &c) const
		{
			for (int i = 0; i < SZ; ++i)
			{
				c(i) = (c(i) - min_values[i]) * norm_factors[i];
			}
		}

		void normalize(std::vector<calculated_t<SZ>>& calculated) const
		{
			for (auto& n : calculated)
			{
				normalize_single(n);					
			}
		}

		void save_data(const std::string& file_name) const
		{
			std::ofstream fout(file_name);
			fout << SZ << '\n';
			for (int i = 0; i < SZ; ++i)
			{
				fout << min_values[i] << ';' <<	max_values[i] << '\n';
			}

		}

	private:
		void init_norm_factors()
		{
			for (int i = 0; i < SZ; ++i)
			{
				norm_factors[i] = 1 / (max_values[i] - min_values[i]);
			}
		}

		double min_values[SZ];
		double max_values[SZ];
		double norm_factors[SZ];
	};

	inline size_t get_normalizer_size(const std::string& fname)
	{
		std::ifstream fin(fname);

		std::string line;
		std::getline(fin, line);

		return stoi(line);
	}
}
