#pragma once
#include <array>
#include "data_types.h"

namespace realty
{
	class sample_data
	{
	public:
		explicit sample_data(const std::string &str)
		{
			std::string::size_type start = 0;

			for (int i = 0; i < SAMPLE_SIZE; ++i)
			{
				auto stop = str.find_first_of(';', start);
				data[i] = stop == start
					? 0
					: stod(str.substr(start, stop - start));
				start = stop + 1;
			}
			if (start && start != str.size())
			{
				data[6] = 1 == data[6] || stod(str.substr(start)) == data[6]
							? 0
							: 1;
			}			
		}

		bool is_area_defined() const { return 0 != data[4]; }

		double sq_distance(const sample_data& s) const
		{
			return	(data[0] - s.data[0]) * (data[0] - s.data[0]) + 
					(data[1] - s.data[1]) * (data[1] - s.data[1]);
		}

		std::string to_string() const
		{
			char buffer[100];
			sprintf(buffer, "%2.6lf;%2.6lf;%1.0lf;%7.2lf;%2.2lf;%2.2lf;%1.0lf",
				data[0], data[1], data[2], data[3], data[4], data[5], data[6]);

			return buffer;
		}

		template<size_t SZ>
		calculated_t<SZ> get_calculated() const
		{
			calculated_t<SZ> n;
			if (4 == SZ) //modified clusterization
			{
				for (int i = 0; i < 3; ++i)
				{
					n(i) = data[i];
				}
				n(3) = data[3] / data[4];
			}
			else
			{
				for (int i = 0; i < SZ; ++i)
				{
					n(i) = data[i];
				}
			}
			return n;
		}
	private:
		static const size_t SAMPLE_SIZE = 7;
		std::array<double, SAMPLE_SIZE> data;
	};

	inline std::vector<sample_data> create_sample_data(std::istream& stream, bool isMod)
	{
		std::vector<sample_data> samples;

		for (std::string line; std::getline(stream, line);)
		{
			auto s = sample_data(line);

			if (isMod && !s.is_area_defined())
				continue;

			samples.push_back(s);
		}
		return samples;
	}
}
