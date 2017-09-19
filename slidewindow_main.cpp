#define HAVE_OPENCV_FLANN
#define HAVE_OPENCV_IMGPROC
#define HAVE_OPENCV_VIDEO
#define HAVE_OPENCV_OBJDETECT
#define HAVE_OPENCV_CALIB3D
#define HAVE_OPENCV_ML
#define HAVE_OPENCV_HIGHGUI
#define HAVE_OPENCV_CONTRIB


#include<opencv2/opencv.hpp>

#include<iostream>

using namespace std;
using namespace cv;



class Line
{
public:

	Line(double k_, double b_){ k=k_;b=b_;}

	double k, b;  // lean rate (slope), cut distance(intercept).

	

	void drawLine(Mat& img)
	{
					
	}	


	void calStartEnd()
	{
		
	}


	Point2d startp;
	Point2d endp;
	int img_rows;
	int img_cols;
	
};

typedef struct Segment_
{
public:
	int startpos;
	int length;
	//int wb; // white 1; black 0;
}Segment;



int ratio=4;

typedef struct Pair_
{
	int left;
	int right;
}Pair;



int minimumMeanlen=0;


bool checkValidLine(vector<Segment>& vS, Pair& ret)
{
	for (int i=0;i<vS.size();i++)
		for(int j=i+1;j<vS.size();j++)
	{

		int deltaLength=abs ( vS.at(j).length - vS.at(i).length );
		int meanLength=( vS.at(j).length + vS.at(i).length)/2;
		int deltaStart= vS.at(j).startpos - vS.at(i).startpos;

		int lengthEqualLimit= meanLength/5; //unit:pixels
		int ratioCheckLimit= meanLength/5;  //unit:pixels


		if(   ( deltaLength < lengthEqualLimit)  &&  (  abs(meanLength - deltaStart/(ratio +1)) < ratioCheckLimit )  &&  ( meanLength> minimumMeanlen)   )  /// line check
	
			{
				cout<<"one valid doubleline pair detected!"<<endl;
			
			 	ret.left=i;
				ret.right=j;
				cout<<"num. i=" <<i << "  and num. j=" <<j<<" are a good pair"<<endl; 
				cout<<"meanLength="<< meanLength<<endl;

				cout<<"ret.left="<<ret.left<<"   "<<"ret.right="<<ret.right<<endl;
				return 1;

			}
			
			
			
			
	}

	return 0;
}



int main()
{

											Mat raw=imread("1_4_1_wrapdirty.jpg",0);

											imshow("raw",raw);

											Mat bina(raw.rows, raw.cols, CV_8UC1, Scalar(0));



cout<<"----------------------------------binarized--------------------------------"<<endl;
										
											threshold( raw, bina, 200, 255,CV_THRESH_BINARY);
	
											
											imshow("binarized",bina);

											imwrite("binarized.jpg",bina);

cout<<"----------------------------------binarized end--------------------------------"<<endl;



cout<<"--------------------------------- Add line---------------------------------"<<endl;

											Mat binaline=bina.clone();

											Point start(0,0), end(400,100);
											int thickness=1;
											int linetype=8;
											line(binaline,start, end, Scalar(0,0,0),thickness,linetype);

											imshow("imgline",binaline);


cout<<"------------------------------------------------------------------"<<endl;
					
						

cout<<"-------------------------------Longitude Scan-----------------------------------"<<endl;

											int cur=0;;

											int startposition=0;

											int flag_first=1;

											Segment temp;

											vector<Segment>vSeg;
											vector<Point2d>vMid;
										
cout<<raw.rows<<endl;
cout<<raw.cols<<endl;

											int successnum=0;

							for (int i=0;i<bina.rows;i+=5)
							{
												
												int length=0;	
												
												int last=255;

												vSeg.clear(); // clear up all 

												cout<<endl<<endl;
												cout<<"============================================================================"<<endl;
												cout<<"longitude scan in row="<<i<<endl;
			
												for(int j=0;j<bina.cols;j++)
													{
														if(bina.at<uchar>(i,j)==0)
															
														{
																if(last==255)// ==1 true first comes here; ==0 not the first time

																{
																	startposition=j;

																	cout<<"j="<<j<<endl;
								
																}	
																length++;
																last=0;

														}

														else

														{
																if(last==0)
																{
																	temp.startpos=startposition;
																	temp.length=length;

																	length=0;

																	vSeg.push_back(temp);
																}
																
																last=255;


														}	

														

													  }	

	
										if(!vSeg.empty())
										{
												Pair success={0,0};
												bool gotit=checkValidLine(vSeg, success);

												if(gotit)
												{
													Point2d middle(( vSeg.at(success.right).startpos + vSeg.at(success.left).startpos + vSeg.at(success.left).length)/2 , i ) ;
													vMid.push_back(middle);

													cout<<"Middle="<< middle<<endl;
													successnum++;
												
												}

										}




							}

															
cout<<"success num="<<successnum<<endl;
cout<<"all middle are---------"<<endl;		

for (int iter=0;iter<vMid.size();iter++)
	cout<<vMid.at(iter)<<endl;

cout<<endl;
																
line(bina,vMid.at(0), vMid.at(7), Scalar(0,0,0),thickness,linetype);		

imshow("Middle line", bina);									

											//}											



											vector<Segment>vLineSeg;

											
		
											waitKey(0);

											return 1;


}














































