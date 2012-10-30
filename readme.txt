Boost is required during compilation.

Download boost from http://sourceforge.net/projects/boost/files/boost/1.51.0/boost_1_51_0.7z/download and 
extract it to simple/external_libs/boost folder.

if you are having problems right click project, go to properties-->c/c++-->general .
Change additional include directories to 
external_libs\boost;%(AdditionalIncludeDirectories)
