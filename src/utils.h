/*
 * PHLAWD: Phylogeny assembly with databases
 * Copyright (C) 2010  Stephen A. Smith
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 */

/*
 *  utils.h
 */

#ifndef _UTILS_H_
#define _UTILS_H_

#include <string>
#include <vector>

using namespace std;

#include "libsqlitewrapped.h"

template <class T>
inline std::string to_string (const T& t);
void Tokenize(const string& str, vector<string>& tokens,const string& delimiters = " ");
void TrimSpaces( string& str);
double median(vector<double> arr);
double mean(vector<double> & x);
double stdev(vector<double> & x);
int getdir (string dir, vector<string> &files);
void splitstring(string str, string seperater, string &first, string &second);
void fix_bad_chars(string & tfilen);
void fix_bad_chars_for_seq_names(string & tfilen);
string int_to_string(int n);
typedef int8_t * SBMatrix;
//took out the const

vector<string> query_mask(string url);
void convert_to_phylip(string,string);
vector<int> get_left_right_exclude(vector<int> * lefts, vector<int> * rights, vector<int> * exlefts, vector<int> * exrights);
int get_distance_from_child_to_parent(string ncbidb, string child, string parent);

void check_valid_division_code(string & div);
#endif
