#include <iostream>
#include <string>
#include <vector>

using namespace std;

#include <opencv2/opencv.hpp>

using namespace cv;

	
struct CAMERA_INTRINSIC_PARAMS{
	double cx, cy, fx, fy, scale;};

int main(int argc, char const *argv[])
{
	

	CAMERA_INTRINSIC_PARAMS camera;

	//Camera intrinsic para
	camera.scale = 1000;
	camera.cx = 325.5;
	camera.cy = 253.5;
	camera.fx = 518.0;
	camera.fy = 519.0;


	Mat rgb, depth;

	rgb = imread("../data/rgb.png");
	depth = imread("../data/depth.png",-1);

	Mat synthesis(rgb.rows, rgb.cols, CV_8UC3);


	for (int m = 0; m < depth.rows; m++){
		for (int n = 0; n < depth.cols; n++)
		{

			ushort d = depth.ptr<ushort>(m)[n];
			if (d == 0)
				continue;

		

			Point3f p; //point 3d in camera frame

			p.z = double(d) / camera.scale;
			p.x = (n - camera.cx) * p.z / camera.fx;
			p.y = (m - camera.cy) * p.z / camera.fy;

			Mat p1(3,1, CV_64F);
			p1.at<double>(0,0) = p.x;
    		p1.at<double>(1,0) = p.y;
    		p1.at<double>(2,0) = p.z;


			Vec3b color = rgb.at<Vec3b>(Point(n,m));

			// rotation & translation

			Mat R(3,3, CV_64F);
			Mat invR(3,3, CV_64F);

			float localroll = 10;
			float localpitch = 0;
			float localyaw = 0;

			double c1 = cos(localyaw*M_PI/180);
    		double s1 = sin(localyaw*M_PI/180);
    		double c2 = cos(localpitch*M_PI/180);
    		double s2 = sin(localpitch*M_PI/180);
    		double c3 = cos(localroll*M_PI/180);
    		double s3 = sin(localroll*M_PI/180);

    		double m1[3][3] = {{c1,s1,0},{-s1,c1,0},{0,0,1}}; //heading
    		double m2[3][3] = {{c2,0,-s2},{0,1,0},{s2,0,c2}};  //pitch
    		double m3[3][3] = {{1,0,0},{0,c3,s3},{0,-s3,c3}};  //pitch

    
   			Mat R1(3,3,CV_64F,m1);//D
    		Mat R2(3,3,CV_64F,m2);//C
    		Mat R3(3,3,CV_64F,m3);//B
 
 			//Rotation matrix R = BCD
    		R = R2*R3*R1; 
    		//invR = R1.t() *R3.t() *R2.t();

    		Mat tvec(3,1,CV_64F);
    		tvec.at<double>(0,0) = 0;
    		tvec.at<double>(1,0) = 0;
    		tvec.at<double>(2,0) = 1;
 
    		// new 3d points
    		Mat p2(3,1, CV_64F);
    		p2 = R*p1+ tvec;


    		//cout  << "p2 position =" << p2.at<double>(0,0) << " "<< p2.at<double>(1,0)  << " "<< p2.at<double>(2,0)<< endl;

    		Point3f imagep;

    		imagep.z = p2.at<double>(2,0)*camera.scale;
    		imagep.x = (camera.fx * p2.at<double>(0,0)) / p2.at<double>(2,0) + camera.cx;
    		imagep.y = (camera.fy * p2.at<double>(1,0)) / p2.at<double>(2,0) + camera.cy;

    		//cout  << "imagep position =" << imagep.z << " "<< imagep.x  << " "<< imagep.y<< endl;

    		if (isnan(imagep.x)||isnan(imagep.y) || imagep.x < 0 || imagep.y < 0 ) //|| imagep.x > rgb.cols || imagep.y > rgb.cols 
    		{
    			continue;
    		}else{
    			int cInt = int (imagep.x);
    			int rInt = int (imagep.y);
    			synthesis.at<Vec3b>(Point(cInt,rInt)) = color;
    		}
    	}
    }

    char *savepath = new char[100];
    sprintf(savepath, "%s", "../data/rgbsyn.png");
    imwrite(savepath, synthesis);
    delete [] savepath;

	return 0;
}


