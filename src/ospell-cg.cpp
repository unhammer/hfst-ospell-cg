/*
 * Copyright (C) 2016, Kevin Brubeck Unhammer <unhammer@fsfe.org>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "ospell-cg.hpp"

namespace gtd {

const float WEIGHT_FACTOR = 1000.0;

const void hacky_cg_anaformat(const std::string& ana, std::ostream& os) {
	const auto lemma_end = ana.find("+");
	if(lemma_end > 0) {
		os << "\"" << ana.substr(0, lemma_end) << "\"";
		auto from = lemma_end + 1;
		ssize_t to = ana.find('+', from);
		while(to > -1) {
			os << " " << ana.substr(from, to-from);
			from = to + 1;
			to = ana.find('+', from);
		}
		os << " " << ana.substr(from);
	}
}

void run(std::istream& is, std::ostream& os, hfst_ol::ZHfstOspeller& s, int limit, int max_weight, int max_analysis_weight)
{
	std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> utf16conv;
	for (std::string line; std::getline(is, line);) {
		os << "\"<" << line << ">\"" << std::endl;
		auto correct = s.spell(line);
		if(correct) {
			auto acq = s.analyse(line);
			while(!acq.empty()) {
				const auto elt = acq.top();
				acq.pop();
				const auto a = elt.first;
				const auto w = (int)(elt.second * WEIGHT_FACTOR);
				// No max_weight for regular words
				os << "\t";
				hacky_cg_anaformat(a, os);
				os << " <W:" << w << ">" << std::endl;
			}
		}
		else {
			auto cq = s.suggest(line);
			while(!cq.empty() && (limit--) > 0) {
				const auto& elt = cq.top();
				const auto& f = elt.first;
				const auto& w = (int)(elt.second * WEIGHT_FACTOR);
				if(w > max_weight) {
					break;
				}
				auto aq = s.analyse(f, true);
				while(!aq.empty()) {
					const auto& elt = aq.top();
					const auto& a = elt.first;
					const auto& w_a = (int)(elt.second);
					if(w > max_analysis_weight) {
						break;
					}
					os << "\t";
					hacky_cg_anaformat(a, os);
					os << " <W:" << w << "> <WA:" << w_a << "> \"<" << f << ">\"" << std::endl;
					aq.pop();
				}
				cq.pop();
			}
		}

	}
}

}
