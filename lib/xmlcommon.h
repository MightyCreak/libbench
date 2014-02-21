/*
 * xmlcommon.h
 *
 *  Created on: 2013-10-12
 *      Author: creak
 */

#ifndef XMLCOMMON_H_
#define XMLCOMMON_H_

namespace libbench
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

#endif /* XMLCOMMON_H_ */
