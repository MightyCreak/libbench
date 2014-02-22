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

#ifndef BENCH_XMLREADER_H
#define BENCH_XMLREADER_H

namespace Glib
{
	class ustring;
}

namespace xmlpp
{
	class TextReader;
}

namespace bench
{
	class BenchMark;
	class Thread;
	class BenchList;

	class XmlReader
	{
	public:
		void Read(Glib::ustring const& filename, BenchMark& benchmark);

	private:
		void ReadCores(xmlpp::TextReader& reader, BenchMark& benchmark);
		void ReadThread(xmlpp::TextReader& reader, BenchMark& benchmark);
		void ReadBenchList(xmlpp::TextReader& reader, BenchList& benchList);
		void ReadBench(xmlpp::TextReader& reader, BenchList& benchList);
	};
}

#endif // BENCH_XMLREADER_H
