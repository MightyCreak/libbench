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

#include <libxml++/parsers/textreader.h>
#include <iostream>
#include "xmlreader.h"
#include "xmlcommon.h"
#include "benchmark.h"
#include "thread.h"

//namespace
//{
//	bool IsEmptySpace(Glib::ustring const& string)
//	{
//		Glib::ustring::const_iterator it = string.begin();
//		while(it != string.end() && isspace(*it))
//			++it;
//		return it == string.end();
//	}
//}

namespace libbench
{
//	struct indent
//	{
//		indent(int depth) : m_depth(depth) { };
//
//		int m_depth;
//	};
//
//	std::ostream & operator << (std::ostream& o, indent const& in)
//	{
//		for(int i = 0; i != in.m_depth; ++i)
//		{
//			o << "  ";
//		}
//		return o;
//	}

	void XmlReader::Read(Glib::ustring const& filename, BenchMark& benchmark)
	{
		std::setlocale(LC_ALL, "C");

		try
		{
			xmlpp::TextReader reader(filename);
			while(reader.read())
			{
//				std::cout << "--- node (name: " << reader.get_name()
//						  << ", type: " << reader.get_node_type() << ") ---"
//						  << std::endl;

				Glib::ustring tagName = reader.get_name();
				if(tagName == kXmlTagCores)
				{
				    ReadCores(reader, benchmark);
				}
				else if(tagName == kXmlTagThread)
				{
					ReadThread(reader, benchmark);
				}
			}
		}
		catch(std::exception const& e)
		{
			std::cerr << "Exception caught: " << e.what() << std::endl;
		}

//		std::locale::global(std::locale(""));
//
//		try
//		{
//			xmlpp::TextReader reader(filename);
//			while(reader.read())
//			{
//				Glib::ustring value;
//				if(reader.has_value())
//				{
//					value = reader.get_value();
//					if(IsEmptySpace(value))
//						continue;
//				}
//
//				int depth = reader.get_depth();
//				std::cout << indent(depth) << "--- node ---" << std::endl;
//				std::cout << indent(depth) << "name: " << reader.get_name() << std::endl;
//				std::cout << indent(depth) << "depth: " << depth << std::endl;
//
//				if(reader.has_attributes())
//				{
//					std::cout << indent(depth) << "attributes: " << std::endl;
//					reader.move_to_first_attribute();
//					do
//					{
//						std::cout << indent(depth) << "  " << reader.get_name() << ": " << reader.get_value() << std::endl;
//					} while(reader.move_to_next_attribute());
//					reader.move_to_element();
//				}
//				else
//				{
//					std::cout << indent(depth) << "no attributes" << std::endl;
//				}
//
//				if(reader.has_value())
//					std::cout << indent(depth) << "value: '" << value << "'" << std::endl;
//				else
//					std::cout << indent(depth) << "novalue" << std::endl;
//			}
//		}
//		catch(std::exception const& e)
//		{
//			std::cerr << "Exception caught: " << e.what() << std::endl;
//		}
	}

	void XmlReader::ReadCores(xmlpp::TextReader& reader, BenchMark& benchmark)
	{
	    std::vector<unsigned int> coreIds;
	    std::vector<Glib::ustring> coreNames;
		int coresDepth = reader.get_depth();
		while(reader.read())
		{
			Glib::ustring tagName = reader.get_name();
			if(tagName == kXmlTagCores && reader.get_depth() == coresDepth)
			    break;
			if(tagName == kXmlTagCore && reader.get_depth() == coresDepth + 1)
			{
			    int res = 0;
			    unsigned int coreId;
			    Glib::ustring coreName;
			    
			    reader.move_to_first_attribute();
			    do
			    {
        			Glib::ustring attrName = reader.get_name();
				    if(reader.get_name() == kXmlAttrId)
				    {
				        ++res;
            			sscanf(reader.get_value().c_str(), "%d", &coreId);
				    }
				    else if(reader.get_name() == kXmlAttrName)
				    {
				        ++res;
				        coreName = reader.get_value();
				    }
			    } while(reader.move_to_next_attribute());
			    reader.move_to_element();
			    
			    if(res == 2)
			    {
			        benchmark.SetCoreName(coreId, coreName.c_str());
			    }
            }
		}
	}
	
	void XmlReader::ReadThread(xmlpp::TextReader& reader, BenchMark& benchmark)
	{
	    int res = 0;
	    int coreId = 0;
	    Glib::ustring threadName;
		if(reader.has_attributes())
		{
			reader.move_to_first_attribute();
			do
			{
    			Glib::ustring attrName = reader.get_name();
				if(attrName == kXmlAttrName)
				{
				    ++res;
        			threadName = reader.get_value();
				}
				else if(attrName == kXmlAttrCore)
				{
        			Glib::ustring const& attrValue = reader.get_value();
					sscanf(attrValue.c_str(), "%d", &coreId);
				}
			} while(reader.move_to_next_attribute());
			reader.move_to_element();
		}

        if(res == 0)
            return;

	    ThreadId threadId = benchmark.AddThread(threadName.c_str(), coreId);
		if(threadId == kInvalidThread)
			return;

		Thread* thread = benchmark.GetThread(threadId);
		ReadBenchList(reader, thread->m_benchList);
	}

	void XmlReader::ReadBenchList(xmlpp::TextReader& reader, BenchList& benchList)
	{
		int threadDepth = reader.get_depth();
		int lastDepth = threadDepth + 1;
		while(reader.read())
		{
			Glib::ustring tagName = reader.get_name();
			if(tagName == kXmlTagThread && reader.get_depth() == threadDepth)
				break;

			if(tagName == kXmlTagBench)
			{
//				if(reader.get_node_type() == xmlpp::TextReader::EndElement)
//				{
//					benchList.m_curNode = benchList.m_benches.back().m_parent;
//				}
				if(reader.get_depth() < lastDepth)
				{
					benchList.m_curNode = benchList.m_benches[benchList.m_curNode].m_parent;
				}
				else
				{
					if(reader.get_depth() > lastDepth)
						benchList.m_curNode = benchList.m_benches.size() - 1;
					ReadBench(reader, benchList);
				}

				lastDepth = reader.get_depth();
			}
		}
	}

	void XmlReader::ReadBench(xmlpp::TextReader& reader, BenchList& benchList)
	{
		if(reader.has_attributes())
		{
			Glib::ustring benchName;
			Glib::ustring benchStartTime;
			Glib::ustring benchStopTime;

			reader.move_to_first_attribute();
			do
			{
				if(reader.get_name() == kXmlAttrName)
					benchName = reader.get_value();
				else if(reader.get_name() == kXmlAttrStart)
					benchStartTime = reader.get_value();
				else if(reader.get_name() == kXmlAttrStop)
					benchStopTime = reader.get_value();
			} while(reader.move_to_next_attribute());
			reader.move_to_element();

			Bench& bench = benchList.AddBench(benchName.c_str());
			sscanf(benchStartTime.c_str(), "%ld.%ld", &bench.m_start.tv_sec, &bench.m_start.tv_nsec);
			sscanf(benchStopTime.c_str(), "%ld.%ld", &bench.m_stop.tv_sec, &bench.m_stop.tv_nsec);
		}
	}
}
