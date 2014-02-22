/*
 * Copyright (C) 2014 Romain "Creak" Failliot.
 *
 * This file is part of libbench.
 *
 * libbench is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * libbench is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with libbench. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef BENCH_XMLWRITER_H
#define BENCH_XMLWRITER_H

#include <ostream>
#include <time.h>

namespace xmlpp
{
	class Element;
}

namespace bench
{
	class BenchMark;
	class Thread;
	class BenchList;
	struct Bench;

	class XmlWriter
	{
	public:
		XmlWriter(std::ostream& stream);

		void Write(BenchMark const& value);

	private:
    	void WriteCores(xmlpp::Element* element, BenchMark const& benchmark);
		void WriteThread(xmlpp::Element* element, Thread const& thread);
		void WriteBenchList(xmlpp::Element* element, BenchList const& benchList);
		void WriteBenchRec(xmlpp::Element* element, BenchList const& benchList, Bench const& bench);

	private:
		std::ostream& m_stream;
		__time_t m_starttime;
	};
}

#endif // BENCH_XMLWRITER_H
