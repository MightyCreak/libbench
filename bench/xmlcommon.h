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

#ifndef BENCH_XMLCOMMON_H
#define BENCH_XMLCOMMON_H

namespace bench
{
    // Tags.
    static Glib::ustring const kXmlTagBenchmark("benchmark");
    static Glib::ustring const kXmlTagThread("thread");
    static Glib::ustring const kXmlTagBench("bench");
    static Glib::ustring const kXmlTagCores("cores");
    static Glib::ustring const kXmlTagCore("core");

    // Attributes.
    static Glib::ustring const kXmlAttrVersion("version");
    static Glib::ustring const kXmlAttrName("name");
    static Glib::ustring const kXmlAttrId("id");
    static Glib::ustring const kXmlAttrCore("core");
    static Glib::ustring const kXmlAttrStart("start");
    static Glib::ustring const kXmlAttrStop("stop");
    static Glib::ustring const kXmlAttrDuration("duration");

    // Data.
    static Glib::ustring const kXmlDataVersion("0.1");
    static Glib::ustring const kXmlDataSecond("s");
    static Glib::ustring const kXmlDataMiliSecond("ms");
    static Glib::ustring const kXmlDataNanoSecond("µs");
}

#endif // BENCH_XMLCOMMON_H
