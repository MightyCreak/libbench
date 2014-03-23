libbench
========

The in-code profiler library.

This library is intended to provide profiling thanks to markers set directly
in the code sources of the application to which it is linked.

It produces an XML file that can easily be read by the tool distributed with
this project.

The official source code is accessible under LGPLv3 here:  
https://github.com/MightyCreak/libbench

Directory structure
-------------------

* **bench/**  
  The core library.

* **test/**  
  A simple multi-threaded test that generates a profiling file.

* **tool/**  
  The tools that display the content created by the library.
  At the moment it includes a GTK viewer.

License
-------

libbench is licensed under the GNU LGPLv3.

> This program is free software: you can redistribute it and/or modify it
> under the terms of the GNU Lesser General Public License as published by
> the Free Software Foundation, either version 3 of the License, or
> (at your option) any later version.

> This program is distributed in the hope that it will be useful,
> but WITHOUT ANY WARRANTY; without even the implied warranty of
> MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
> GNU Lesser General Public License for more details.

> You should have received a copy of the GNU Lesser General Public License
> along with this program. If not, see <http://www.gnu.org/licenses/>.

How to use it?
--------------

I wanted the library to be easy to use, so you'll need to *link* your
application with the bench library, *initialize* it, *add markers* you want to
profile your code, *save* the profiling into a file, and then *shutdown* the
library. That's all you'll need to do!

### Initialize the library

To initialize the library, simply call `bench::Initialize()`.

### Place the markers

The markers are presented in the form of macros:
* `BENCH_START_PROFILE`  
  Start a profile. Must be ended by a `BENCH_STOP_PROFILE`.
* `BENCH_SCOPED_PROFILE`  
  Start a scoped profile. It will be automatically ended at the end of the
  scope.

### Save to a file

Once your profiling is over, you can dump it into a file thanks to the
function: `bench::WriteToXml(std::ostream& stream)`.

### Shutdown the library

As for intialization, to shutdown the library, simply call
`bench::Shutdown()`.

### Switch the profiler

Profiling takes time and once your application is optimized, you don't want to
waste time in the overhead induced by the markers.

To do so, simply switch the activation with the define: `USE_LIBBENCH`.

### Example

You'll find a multi-threaded example in the **test** directory.

Authors
-------

Romain "Creak" Failliot

