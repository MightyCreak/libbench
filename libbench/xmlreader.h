/*
 * xmlreader.h
 *
 *  Created on: 2013-10-12
 *      Author: creak
 */

#ifndef XMLREADER_H_
#define XMLREADER_H_

namespace Glib
{
	class ustring;
}

namespace xmlpp
{
	class TextReader;
}

namespace libbench
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

#endif /* XMLREADER_H_ */
