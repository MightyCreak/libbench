libbench
========

The in-code profiler library.

This library is intended to provide profiling thanks to markers set directly
in the code sources of the application to which it is linked.

It produces an XML file that can be easily read by the viewer distributed
within this project.

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

Authors
-------

Romain "Creak" Failliot
