#include <bits/stdc++.h>

using namespace cv;
using namespace std;

/*
Description - This function is used to write the camera matrix and distortion coefficient matrix into JSON file
Parameters - Camera matrix and Distortion coefficient matrix of type cv::Mat
Return - None
*/
void storeJSON(Mat &cammat, Mat &distcoef)
{
	FileStorage fs("Camera Configuration.json", FileStorage::WRITE);
    fs << "Camera Matrix" << cammat;
    fs << "Distortion Coefficients" << distcoef;
    fs.release();
}

/*
Description - This function is used to read the camera matrix and distortion coefficient matrix from JSON file
Parameters - Camera matrix and Distortion coefficient matrix of type cv::Mat
Return - None
*/
void readJSON(Mat &cammat, Mat &distcoef)
{
    FileStorage fs("Camera Configuration.json", FileStorage::READ);
    fs ["Camera Matrix"] >> cammat;
    fs ["Distortion Coefficients"] >> distcoef;
    fs.release();
}

/*
Description - This function is used to find all vertices and face vertices in an .obj file
Parameters - file name of string type, vertices of vector array of cv::Point3f type and face vertices of vector of vector of type int
Return - None
*/
void OBJextractor(string file, vector<Point3f> &vertices, vector<vector<int>> &face)
{
	string line;
    ifstream inFile(file);
    if (!inFile.is_open())
    {
        cout << "Fail to open: " << file << endl;
        return;
    }
	vector<string> tokens, indices;
	
    while (getline(inFile, line))
    {
    	stringstream ss(line);
    	string s;
    	vector<string> tokens, indices;
    	while (getline(ss, s, ' ')) 
    	{
		    tokens.push_back(s);
		}
		if (tokens.size() > 0)
        {
            vector<int> faceindex;
			if (tokens[0].compare("v") == 0)
		    {
		    	vertices.push_back(Point3f(stof(tokens[1]),stof(tokens[3]),(-1 * stof(tokens[2]))));
		    }
		    else if (tokens[0].compare("f") == 0)
		    {
		    	for(int i = 1 ; i<tokens.size() ; i++)
		    	{
		    		faceindex.push_back(stoi(tokens[i]));
		    	}
		    	face.push_back(faceindex);
		    	faceindex.clear();
		    }
		}
    }
}
