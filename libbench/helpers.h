/*
 * helpers.h
 *
 *  Created on: 2013-10-12
 *      Author: creak
 */

#ifndef HELPERS_H_
#define HELPERS_H_

#ifdef USE_LIBBENCH

#	define LIBBENCH_START_PROFILE(name)		libbench::Manager::GetInstance()->StartBench(#name)
#	define LIBBENCH_STOP_PROFILE()			libbench::Manager::GetInstance()->StopBench()
#	define LIBBENCH_SCOPED_PROFILE(name)	libbench::ScopedBench __libbench ## name(#name)

#else

#	define LIBBENCH_START_PROFILE(name)
#	define LIBBENCH_STOP_PROFILE()
#	define LIBBENCH_SCOPED_PROFILE(name)

#endif

#endif /* HELPERS_H_ */
