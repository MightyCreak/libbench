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
    class DomParser;
    class Node;
}

namespace bench
{
    class Document;
    class DocumentCore;
    class DocumentThread;
    class DocumentBench;

    class XmlReader
    {
    public:
        void Read(Glib::ustring const& filename, Document& doc);

    private:
        void ReadCore(xmlpp::Node* coreNode, DocumentCore& docCore);
        void ReadThread(xmlpp::Node* threadNode, DocumentThread& docThread);
        void ReadBench(xmlpp::Node* benchNode, DocumentBench& docBench);
    };
}

#endif // BENCH_XMLREADER_H
