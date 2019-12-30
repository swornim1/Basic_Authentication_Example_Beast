/*******************************************************************************
 * Author: Swornim Baral
 * 
 * Description: Header file for base64 encoding
 * 
 * References
 * 1) modoran (2011) base64.h [Source code], http://www.cplusplus.com/forum/beginner/51572/
 *  **********************************************************************************/

#ifndef _BASE64_H_
#define _BASE64_H_
#include <string>
std::string base64_encode(unsigned char const* , unsigned int len);
#endif 