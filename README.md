Ultimate Webshots Converter 2 (Qt5.5)
=====================================
This is a fork of the original at https://github.com/setaou/uwc2/.
This version is now compiling with Qt5.5 and using QtCreator (mingw C++11).

Not all original (1.6) features are implemented yet. Compared to the old Qt4 beta, I've added the about dialog
and an export webshots archive feature. This exports an orginal "Webshots Data" directory to an output directory which will contain a directory tree of collection/album subdirectories, and jpg files with filenames ph%ID%-%Title%.jpg

Old info:

Ultimate Webshots Converter (also known under the short name "UWC"), is a Webshots pictures converter (see www.webshots.com).
It is able to show and save into JPEG files the pictures contained in the following files : 

 * .WBC and .WBP - Webshots Collections
 * .WBZ - Downloaded Webshots pictures
 * .WB1 and .WBD - Webshots pictures

This is the second version, a complete rewrite in C++/Qt, but it has never been finished.

It uses the cWebshots classes to handle the file formats.
