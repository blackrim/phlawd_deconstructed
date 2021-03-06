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
 *  utils.cpp
 */

#include <sstream>
#include <iostream>
#include <algorithm>
#include <vector>
#include <dirent.h>
#include <math.h>
#include <fstream>
#include "libsqlitewrapped.h"

using namespace std;


template <class T>
inline std::string to_string (const T& t){
    std::stringstream ss;
    ss << t;
    return ss.str();
}


void Tokenize(const string& str, vector<string>& tokens,
                      const string& delimiters = " "){
    // Skip delimiters at beginning.
    string::size_type lastPos = str.find_first_not_of(delimiters, 0);
    // Find first "non-delimiter".
    string::size_type pos     = str.find_first_of(delimiters, lastPos);

    while (string::npos != pos || string::npos != lastPos)
    {
        // Found a token, add it to the vector.
        tokens.push_back(str.substr(lastPos, pos - lastPos));
        // Skip delimiters.  Note the "not_of"
        lastPos = str.find_first_not_of(delimiters, pos);
        // Find next "non-delimiter"
        pos = str.find_first_of(delimiters, lastPos);
    }
}

void TrimSpaces( string& str)  {
    // Trim Both leading and trailing spaces
    size_t startpos = str.find_first_not_of(" \t\r\n"); // Find the first character position after excluding leading blank spaces
    size_t endpos = str.find_last_not_of(" \t\r\n"); // Find the first character position from reverse af

    // if all spaces or empty return an empty string
    if(( string::npos == startpos ) || ( string::npos == endpos))
    {
	str = "";
    }
    else
	str = str.substr( startpos, endpos-startpos+1 );
    /*
    // Code for  Trim Leading Spaces only
    size_t startpos = str.find_first_not_of(� \t�); // Find the first character position after excluding leading blank spaces
    if( string::npos != startpos )
    str = str.substr( startpos );
    */
    /*
    // Code for Trim trailing Spaces only
    size_t endpos = str.find_last_not_of(� \t�); // Find the first character position from reverse af
    if( string::npos != endpos )
    str = str.substr( 0, endpos+1 );
    */
}

double median(vector<double> x){
    sort(x.begin(),x.end());
    double n = x.size();
    return ( x [ n / 2.0 ] + x [ n / 2.0 - 1.0] ) / 2.0F;
}

double mean(vector<double> & x){
    double size = x.size();
    double ret = 0;
    for(int i=0;i<x.size();i++){
	ret += x[i]/size;
    }
    return ret;
}

double stdev(vector<double> & x){
    double mn = mean(x);
    vector <double> devs(x.size());
    for(int i=0;i<x.size();i++){
	devs[i] = x[i] - mn;
	devs[i] = (devs[i]*devs[i]);
    }
    double sum = 0;
    for(int i=0;i<x.size();i++){
	sum += devs[i];
    }
    sum = (sum/(x.size()-1));
    return sqrt(sum);
}

int getdir (string dir, vector<string> &files){
    DIR *dp;
    struct dirent *dirp;
    if((dp  = opendir(dir.c_str())) == NULL) {
        cout << "Error(" << errno << ") opening " << dir << endl;
        return errno;
    }

    while ((dirp = readdir(dp)) != NULL) {
    	/*
    	 * no dot files
    	 */
    	bool test = false;
    	//if(*(string(dirp->d_name).begin())=='\.')
        if(*(string(dirp->d_name).begin())=='.')
	    test = true;
    	if(test == false){
	    files.push_back(string(dirp->d_name));
    	}
    }
    closedir(dp);
    return 0;
}

void splitstring(string str, string seperater, string &first, string &second){
    int i = (int)str.find(seperater); //find seperator
    if(i != -1){
	int y = 0;
	if(!str.empty()){
	    while(y != i){
		first += str[y++]; //creating first string
	    }
	    y = y+(int)seperater.length(); //jumping forward seperater length
	    while(y != str.length()){
		second += str[y++]; //creating second string
	    }

	}
    }
    else{
	first = str;
	second = "NULL"; //if seperator is not there then second string == null
    }
}

void fix_bad_chars(string & tfilen){
    size_t found;
    found = tfilen.find(" ");
    while(found!=string::npos){
	tfilen.replace(found,1,"\\ ");
	found = tfilen.find(" ",found+2);
    }
    //(take out the parenthetical stuff too)
    found = tfilen.find("(");
    while(found!=string::npos){
	tfilen.replace(found,1,"\\(");
	found = tfilen.find("(",found+2);
    }
    found = tfilen.find(")");
    while(found!=string::npos){
	tfilen.replace(found,1,"\\)");
	found = tfilen.find(")",found+2);
    }
}

void fix_bad_chars_for_seq_names(string & tfilen){
    size_t found;
    found = tfilen.find(" ");
    while(found!=string::npos){
	tfilen.replace(found,1,"_");
	found = tfilen.find(" ",found+2);
    }
    //(take out the parenthetical stuff too)
    found = tfilen.find("(");
    while(found!=string::npos){
	tfilen.replace(found,1,"_");
	found = tfilen.find("(",found+2);
    }
    found = tfilen.find(")");
    while(found!=string::npos){
	tfilen.replace(found,1,"_");
	found = tfilen.find(")",found+2);
    }
    found = tfilen.find(".");
    while(found!=string::npos){
	tfilen.replace(found,1,"_");
	found = tfilen.find(".",found+2);
    }
    found = tfilen.find("&");
    while(found!=string::npos){
	tfilen.replace(found,1,"_");
	found = tfilen.find("&",found+2);
    }
}

string int_to_string(int n){
   stringstream ss;
   ss << n;
   return ss.str();
}



/*
 * This will query the mask given the mask url provided in the config file.
 * It relies on curl to be installed and in the path. Though this is a 
 * relatively low bar considering it is installed generally by default in 
 * both MAC and Linux (basically all UNIX) machines.
 *
 * Would rather use libcurl but might be more difficult for source
 * installers.
 *
 * url = the input url
 * output = vector of the gi strings
 */
vector<string> query_mask(string url){
    string systemcall = "curl "+url;
    systemcall += " > gbmask.downloaded";
    cout << systemcall << endl;
    //need a check here to make sure that it is valid
    system(systemcall.c_str());
    vector<string> returngis;

    return returngis;
}

// division codes: https://www.ncbi.nlm.nih.gov/genbank/htgs/divisions/
// purpose here it to abort early if the provided division code is invalid
// downstream code expects lowercase div code, so do this prior to checking
// comments if uppercase
// exits if invalid
void check_valid_division_code(string & inDiv) {
    // currently available codes
    vector<string> divs = {"BCT", "PRI", "ROD", "MAM", "VRT", "INV", "PLN",
                           "VRL", "PHG", "RNA", "SYN", "UNA"};
    string test = inDiv;
    std::transform(test.begin(),test.end(),test.begin(),::toupper);
    
    if(std::find(divs.begin(),divs.end(),test)!=divs.end()){
        if(inDiv==test){
            // provided div code was uppercase. change to lowercase
            std::transform(inDiv.begin(),inDiv.end(),inDiv.begin(),::tolower);
        }
    } else {
        cout<<"invalid ncbi division code provided. see https://www.ncbi.nlm.nih.gov/genbank/htgs/divisions"<<endl;
        exit(0);
    }
}
