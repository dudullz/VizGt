#ifndef	VIZ_GT_HEADER_H
#define	VIZ_GT_HEADER_H

#include <cv.h>
#include <cxcore.h>
#include <highgui.h>

#include "libxml/xmlreader.h"
#include "libxml/parser.h"
#include "libxml/tree.h"
#include "libxml/encoding.h"
#include "libxml/xmlwriter.h"

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
using namespace std;

#define BASH_ESC_WHITE "\033[00m"
#define BASH_ESC_RED   "\033[1;31m"
#define BASH_ESC_GREEN  "\033[1;32m"
#define BASH_ESC_YELLOW "\033[1;33m"
#define BASH_ESC_BLUE  "\033[1;34m"
#define BASH_ESC_PURPLE  "\033[1;35m"
#define BASH_ESC_CYAN  "\033[1;36m"

#define GT_COLOUR	CV_RGB(0, 255, 0)	//green
#define AR_COLOUR	CV_RGB(0, 0, 255)	//blue
#define BAG_COLOUR	CV_RGB(255, 0, 0)	//red

#define MY_ENCODING "iso-8859-1"

#endif