
#ifndef PARSE_UTILS_H
#define PARSE_UTILS_H

/**
 * Reads line from inout stream
 *
 * @param is	reference to input file stream
 * @param s		reference to string where output is saved
 */
bool readLine (std::ifstream& is, std::string& s);

/**
 * Parses integer value from p string
 *
 * @param	p 	reference to pointer to string
 * @return		returns integer value
 */
int parseInt (char *& p);

/**
 * Parses float value from p string
 *
 * @param	p 	reference to pointer to string
 * @return		returns float value
 */
float parseFloat (char *& p);

/**
 * Parses integer value param from one line in the file
 *
 * @param	file	reference to input file stream
 */
int parseLineParam (std::ifstream& file);

#endif
