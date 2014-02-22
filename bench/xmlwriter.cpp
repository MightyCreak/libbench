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

#include <stdio.h>
#include <assert.h>
#include <libxml++/document.h>
#include "xmlwriter.h"
#include "xmlcommon.h"
#include "manager.h"
#include "core.h"
#include "thread.h"

namespace bench
{
    XmlWriter::XmlWriter(std::ostream& stream)
        : m_stream(stream)
        , m_starttime(0)
    {
    }

    void XmlWriter::Write(Document const& doc)
    {
        std::setlocale(LC_ALL, "C");
        xmlpp::Document xmlDoc;
        xmlpp::Element* root = xmlDoc.create_root_node(kXmlTagBenchmark);
        root->set_attribute(kXmlAttrVersion, kXmlDataVersion);

        // Dump each core.
        for(DocumentCore const& docCore : doc.m_cores)
        {
            WriteCore(root, docCore);
        }

        xmlDoc.write_to_stream_formatted(m_stream);
    }

    void XmlWriter::WriteCore(xmlpp::Element* element, DocumentCore const& docCore)
    {
        xmlpp::Element* coreElt = element->add_child(kXmlTagCore);
        coreElt->set_attribute(kXmlAttrName, docCore.m_name);

        // Dump each thread.
        for(DocumentThread const& docThread : docCore.m_threads)
        {
            WriteThread(coreElt, docThread);
        }
    }

    void XmlWriter::WriteThread(xmlpp::Element* element, DocumentThread const& docThread)
    {
        xmlpp::Element* threadElt = element->add_child(kXmlTagThread);
        threadElt->set_attribute(kXmlAttrName, docThread.m_name);

        // Dump each bench.
        for(DocumentBench const& docBench : docThread.m_benches)
        {
            WriteBench(threadElt, docBench);
        }
    }

    void XmlWriter::WriteBench(xmlpp::Element* element, DocumentBench const& docBench)
    {
        xmlpp::Element* benchElt = element->add_child(kXmlTagBench);

        // Set name.
        benchElt->set_attribute(kXmlAttrName, docBench.m_name);

        // Set times.
        char startTimeStr[20];
        sprintf(startTimeStr, "%lf", docBench.m_start - m_starttime);
        char stopTimeStr[20];
        sprintf(stopTimeStr, "%lf", docBench.m_stop - m_starttime);
        benchElt->set_attribute(kXmlAttrStart, startTimeStr);
        benchElt->set_attribute(kXmlAttrStop, stopTimeStr);

        // Set duration.
        double duration = docBench.m_stop - docBench.m_start;
        Glib::ustring const* unit = &kXmlDataSecond;
        if(duration < 1e-3)
        {
            duration *= 1e3;
            unit = &kXmlDataMiliSecond;
        }
        else if(duration < 1e-6)
        {
            duration *= 1e6;
            unit = &kXmlDataMicroSecond;
        }
        char durationStr[20];
        sprintf(durationStr, "%.4lf %s", duration, unit->c_str());
        benchElt->set_attribute(kXmlAttrDuration, durationStr);

        // Dump each bench.
        for(DocumentBench const& docChildBench : docBench.m_benches)
        {
            WriteBench(benchElt, docChildBench);
        }
    }
}
