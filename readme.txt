Boost is required during compilation.

Download boost from http://sourceforge.net/projects/boost/files/boost/1.51.0/boost_1_51_0.7z/download and 
extract it to simple/external_libs/boost folder.

if you are having problems right click project, go to properties-->c/c++-->general .
Change additional include directories to 
external_libs\boost;%(AdditionalIncludeDirectories)

Update 2/4
Implemented some stuff that requires new boost functions.
http://sourceforge.net/projects/boost/files/boost/1.53.0/boost_1_53_0.7z/download

Put the extracted files under C:\boost\boost_1_53_0 so that the folder contains bin.v2 boost doc etc.
Use cmd to cd to that folder and type:
.\bootstrap.bat
.\b2 toolset=msvc-10.0 runtime-link=static

To enable threading, right click the SPA project and go to properties. Look for C/C++ preprocessor and insert ENABLE_THREADING in preprocessor definition.