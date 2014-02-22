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

namespace xmlpp
{
    class Element;
}

namespace bench
{
    class Document;
    class DocumentCore;
    class DocumentThread;
    class DocumentBench;

    class XmlWriter
    {
    public:
        XmlWriter(std::ostream& stream);

        void Write(Document const& doc);

    private:
        void WriteCore(xmlpp::Element* element, DocumentCore const& docCore);
        void WriteThread(xmlpp::Element* element, DocumentThread const& docThread);
        void WriteBench(xmlpp::Element* element, DocumentBench const& docBench);

    private:
        std::ostream& m_stream;
        double m_starttime;
    };
}

#endif // BENCH_XMLWRITER_H
