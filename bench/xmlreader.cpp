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

#include <libxml++/parsers/domparser.h>
#include <iostream>
#include "xmlreader.h"
#include "xmlcommon.h"
#include "thread.h"

namespace bench
{
    void XmlReader::Read(Glib::ustring const& filename, Document& doc)
    {
        std::setlocale(LC_ALL, "C");

        xmlpp::DomParser domParser(filename);
        if(domParser)
        {
            xmlpp::Document* domDoc = domParser.get_document();
            xmlpp::Element* root = domDoc->get_root_node();
            if(root->get_name() == kXmlTagBenchmark)
            {
                xmlpp::Node::NodeList coreNodeList = root->get_children(kXmlTagCore);
                if(coreNodeList.size() > 0)
                {
                    doc.m_cores.reserve(coreNodeList.size());
                    for(xmlpp::Node* coreNode : coreNodeList)
                    {
                        doc.m_cores.push_back(DocumentCore());
                        DocumentCore& docCore = doc.m_cores.back();
                        ReadCore(coreNode, docCore);
                    }
                }
            }
        }
    }

    void XmlReader::ReadCore(xmlpp::Node* coreNode, DocumentCore& docCore)
    {
        xmlpp::Element* coreElt = dynamic_cast<xmlpp::Element*>(coreNode);
        docCore.m_name = coreElt->get_attribute_value(kXmlAttrName);

        xmlpp::Node::NodeList threadNodeList = coreNode->get_children(kXmlTagThread);
        if(threadNodeList.size() > 0)
        {
            docCore.m_threads.reserve(threadNodeList.size());
            for(xmlpp::Node* threadNode : threadNodeList)
            {
                docCore.m_threads.push_back(DocumentThread());
                DocumentThread& docThread = docCore.m_threads.back();
                ReadThread(threadNode, docThread);
            }
        }
    }

    void XmlReader::ReadThread(xmlpp::Node* threadNode, DocumentThread& docThread)
    {
        xmlpp::Element* threadElt = dynamic_cast<xmlpp::Element*>(threadNode);
        docThread.m_name = threadElt->get_attribute_value(kXmlAttrName);

        xmlpp::Node::NodeList benchNodeList = threadNode->get_children(kXmlTagBench);
        if(benchNodeList.size() > 0)
        {
            docThread.m_benches.reserve(benchNodeList.size());
            for(xmlpp::Node* benchNode : benchNodeList)
            {
                docThread.m_benches.push_back(DocumentBench());
                DocumentBench& docBench = docThread.m_benches.back();
                ReadBench(benchNode, docBench);
            }
        }
    }

    void XmlReader::ReadBench(xmlpp::Node* benchNode, DocumentBench& docBench)
    {
        xmlpp::Element* benchElt = dynamic_cast<xmlpp::Element*>(benchNode);
        xmlpp::Attribute* nameAttr = benchElt->get_attribute(kXmlAttrName);
        xmlpp::Attribute* startAttr = benchElt->get_attribute(kXmlAttrStart);
        xmlpp::Attribute* stopAttr = benchElt->get_attribute(kXmlAttrStop);
        if(nameAttr && startAttr && stopAttr)
        {
            docBench.m_name = nameAttr->get_value();

            Glib::ustring benchStartTime = startAttr->get_value();
            Glib::ustring benchStopTime = stopAttr->get_value();
            sscanf(benchStartTime.c_str(), "%lf", &docBench.m_start);
            sscanf(benchStopTime.c_str(), "%lf", &docBench.m_stop);
        }

        xmlpp::Node::NodeList benchNodeList = benchNode->get_children(kXmlTagBench);
        if(benchNodeList.size() > 0)
        {
            docBench.m_benches.reserve(benchNodeList.size());
            for(xmlpp::Node* benchNode : benchNodeList)
            {
                docBench.m_benches.push_back(DocumentBench());
                DocumentBench& docChildBench = docBench.m_benches.back();
                ReadBench(benchNode, docChildBench);
            }
        }
    }
}
