/*
 * serializer.h
 *
 *  Created on: 2013-09-26
 *      Author: creak
 */

#ifndef SERIALIZER_H_
#define SERIALIZER_H_

#include <ostream>
#include <time.h>

namespace xmlpp
{
	class Element;
}

namespace libbench
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

#endif /* SERIALIZER_H_ */
