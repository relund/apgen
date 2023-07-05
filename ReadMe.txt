Read me file for the APGen distribution
======================================

    Version 2.0 - December 2005

    Copyright (C) 2005 Lars Relund Nielsen and Christian Roed Pedersen

    This program is free software; you can redistribute it and/or modify it
    under the terms of the GNU General Public License version 2 as published
    by the Free Software Foundation.

    This program is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
    or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License along
    with this program (file gpl.txt); if not, see http://www.gnu.org.

    You can contact us at: lars@relund.dk

    Updated information about this software will be available at the following
    www address: http://www.research.relund.dk.

===============================================================================

    Contents of this file:

     1. Introduction
     2. List of files
     3. How to compile APGen
     4. How to run APGen


1. Introduction
===============================================================================

  This distribution contains an implementation of the APGen generator for
  generating generate problem instances for the bicriterion assignment
  problem. Also instances for the bicriterion multi modal assignment problem
  can be generated. For more details see the documentation in the doc folder.


2. List of files
===============================================================================

  You should find in this distribution the following files and folders:

     gpl.txt              - The GNU General Public License.
     ReadMe.txt           - This file.
     binaries folder      - The compiled binaries for Linux and Windows.
     doc folder           - Documentation.
     source folder        - The source code.

3. How to run APGen
===============================================================================

  Binaries for APGen are given in the 'binaries' folder for both Linux and
  Windows. To run the program you must have the linked libraries of libxml2
  (v2.6.19), iconv (v1.9.1) and zlib (v1.2.2) used by the program. Higher
  versions should properly also work but I have not tested it. These libraries
  (dll's) for Windows are in this distribution and for Linux properly already
  installed on your system (just check that the version is okay). Otherwise
  check ftp://xmlsoft.org/libxml2/.

  For details on how to run APGen see the documentation (doc folder). To test
  the generator you may run the example given in the 'test_example' folder.
  Simply run 'run_win.bat' or 'run_linux' from the command line and two files
  should be produced 'run.log' and 'test_stdn.xml'. A perl script
  (gen_problems.pl) for generating a set of test instances is also provided.


4. How to compile APGen
===============================================================================

  If for some reason you want to make your own binaries you should compile
  them yourself using the 'source' folder. First, you should check if you have
  installed libxml2, iconv and zlib on your system. I have used libxml2
  (v2.6.19), iconv (v1.9.1) and zlib (v1.2.2). Higher versions should properly
  also work but I have not tested it. Binaries/source for both Linux and
  Windows can be downloaded from ftp://xmlsoft.org/libxml2/. For more details
  see http://xmlsoft.org/. Under Windows it may be a good idea to extract the
  binaries/source in the 'code' folder.

  To compile on Linux:

  From the command prompt goto folder 'linux' and run the makefile by writing
  >make. The program APGen should now have been compiled. If you face problems
  it may be because you have not installed the correct versions of libxml2 and
  iconv or that the path is not correct in the makefile.

  To compile on Windows:

  The program can be compiled using MS VC++ (v6.0). Goto the 'win' folder and
  open the workspace file 'apgen.dsw'. The files will then be loaded in VC++.
  Use the compile bottom to compile the program. The program should now be
  located in the 'Debug' folder. You may have to change the path to libxml2
  and iconv in Project->Settings under the C/C++ tab in the 'Additional
  include directories' field. Moreover, the path to libxml2.lib included in
  the FileView of the workspace should be correct.
