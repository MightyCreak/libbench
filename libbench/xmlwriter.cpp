/*
 * serializer.cpp
 *
 *  Created on: 2013-09-26
 *      Author: creak
 */

#include <stdio.h>
#include <assert.h>
#include <libxml++/document.h>
#include "xmlwriter.h"
#include "xmlcommon.h"
#include "benchmark.h"
#include "thread.h"

namespace libbench
{
	XmlWriter::XmlWriter(std::ostream& stream)
		: m_stream(stream)
		, m_starttime(0)
	{
	}

	void XmlWriter::Write(BenchMark const& benchmark)
	{
		std::setlocale(LC_ALL, "C");

		xmlpp::Document doc;
		xmlpp::Element* root = doc.create_root_node(kXmlTagBenchmark);
		root->set_attribute(kXmlAttrVersion, kXmlDataVersion);

		// Dump each core.
    	WriteCores(root, benchmark);

		// Find start time.
		ThreadVector const& threads = benchmark.GetThreadVector();
		bool first = true;
		for (ThreadVector::const_iterator it = threads.begin(); it != threads.end(); ++it)
		{
			Thread const* thread = *it;
			Bench const& bench = thread->m_benchList.m_benches.front();
			if(first)
			{
				m_starttime = bench.m_start.tv_sec;
				first = false;
			}
			else
			{
				if(bench.m_start.tv_sec < m_starttime)
				{
					m_starttime = bench.m_start.tv_sec;
				}
			}
		}

		// Dump each thread.
		for (ThreadVector::const_iterator it = threads.begin(); it != threads.end(); ++it)
		{
			Thread const* thread = *it;
			xmlpp::Element* threadElt = root->add_child(kXmlTagThread);
			WriteThread(threadElt, *thread);
		}

		doc.write_to_stream_formatted(m_stream);
	}
	
	void XmlWriter::WriteCores(xmlpp::Element* element, BenchMark const& benchmark)
	{
		xmlpp::Element* coreListElt = element->add_child(kXmlTagCores);
		for(unsigned int u = 0; u < benchmark.GetNumberOfCores(); ++u)
		{
		    xmlpp::Element* coreElt = coreListElt->add_child(kXmlTagCore);
	        char coreIdStr[20];
	        sprintf(coreIdStr, "%d", u);
    		coreElt->set_attribute(kXmlAttrId, coreIdStr);
    		coreElt->set_attribute(kXmlAttrName, benchmark.GetCoreName(u));
		}
	}

	void XmlWriter::WriteThread(xmlpp::Element* element, Thread const& thread)
	{
		element->set_attribute(kXmlAttrName, thread.m_name);
        char coreIdStr[20];
        sprintf(coreIdStr, "%d", thread.m_coreId);
		element->set_attribute(kXmlAttrCore, coreIdStr);
		WriteBenchList(element, thread.m_benchList);
	}

	void XmlWriter::WriteBenchList(xmlpp::Element* element, BenchList const& benchList)
	{
		// Dump bench list.
		for(BenchVector::const_iterator it = benchList.m_benches.begin(); it != benchList.m_benches.end(); ++it)
		{
			Bench const& bench = *it;
			if(bench.m_parent == BenchList::kNoParent)
				WriteBenchRec(element, benchList, bench);
		}
	}

	void XmlWriter::WriteBenchRec(xmlpp::Element* element, BenchList const& benchList, Bench const& bench)
	{
		// Get name.
		StringMap::const_iterator nameIt = benchList.m_benchNames.find(bench.m_nameId);
		assert(nameIt != benchList.m_benchNames.end());
		char const* name = nameIt->second;

		// Get times.
		char startTimeStr[20];
		sprintf(startTimeStr, "%ld.%ld", bench.m_start.tv_sec - m_starttime, bench.m_start.tv_nsec);
		char stopTimeStr[20];
		sprintf(stopTimeStr, "%ld.%ld", bench.m_stop.tv_sec - m_starttime, bench.m_stop.tv_nsec);

		// Get duration.
		__time_t sec = bench.m_stop.tv_sec - bench.m_start.tv_sec;
		__syscall_slong_t nsec = bench.m_stop.tv_nsec - bench.m_start.tv_nsec;
		double factor = 1e-9;
		Glib::ustring const* unit = &kXmlDataSecond;
		if(sec == 0)
		{
			if(nsec > 1e6)
			{
				factor = 1e-6;
				unit = &kXmlDataMiliSecond;
			}
			else
			{
				factor = 1e-3;
				unit = &kXmlDataNanoSecond;
			}
			// No need to go lower, the bench under a nanosecond aren't kept.
		}
		double duration = sec + nsec * factor;
		char durationStr[20];
		sprintf(durationStr, "%.4f %s", duration, unit->c_str());

		// Add node.
		xmlpp::Element* benchElt = element->add_child(kXmlTagBench);
		benchElt->set_attribute(kXmlAttrName, name);
		benchElt->set_attribute(kXmlAttrStart, startTimeStr);
		benchElt->set_attribute(kXmlAttrStop, stopTimeStr);
		benchElt->set_attribute(kXmlAttrDuration, durationStr);

		int benchIdx = &bench - &benchList.m_benches[0];
		BenchVector::const_iterator it = benchList.m_benches.begin() + benchIdx + 1;
		for(; it != benchList.m_benches.end(); ++it)
		{
			Bench const& other = *it;
			if(other.m_parent == benchIdx)
				WriteBenchRec(benchElt, benchList, other);
		}
	}
}
