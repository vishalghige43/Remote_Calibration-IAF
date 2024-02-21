#include <opencv2/opencv.hpp>
#include<iostream>
#include<vector>
#include<cmath>

using namespace std;
using namespace cv;

#define PI 3.1415926535897932384626433832795

class CalibratedValue{
    int minVal,maxVal,minAng,maxAng,X,Y,R;
    string units;
public:
    CalibratedValue(const int & X,const int &Y,const int &R,const int &minAng,const int &maxAng,const int &minVal,const int &maxVal,const string &units){
        this->minVal=minVal;
        this->maxVal=maxVal;
        this->minAng=minAng;
        this->maxAng=maxAng;
        this->units=units;
        this->X=X;
        this->Y=Y;
        this->R=R;
    }
    friend ostream& operator<<(ostream& os,const CalibratedValue &obj){
        os<<"Min Angle(lowest possible angle of dial): "<<obj.minAng<<"\n";
        os<<"Max Angle(highest possible angle of dial): "<<obj.maxAng<<"\n";
        os<<"Min Value: "<<obj.minVal<<"\n";
        os<<"Max Value: "<<obj.maxVal<<"\n";
        os<<"Enter Units: "<<obj.units<<"\n";
        os<<"X of Center of Circle: "<<obj.X<<"\n";
        os<<"Y of Center of Circle: "<<obj.Y<<"\n";
        os<<"Raduis of Circle: "<<obj.R<<"\n";
        return os;
    }
};

void biggestCircle(const vector<Vec3f> &circles,int &X,int &Y,int &R){
    float area=INT_MIN;
    for(int i=0;i<circles.size();i++){
        float tempArea=(float)(PI*(circles[i][2])*(circles[i][2]));
        if(tempArea<(2*area))continue;
        X=circles[i][0];
        Y=circles[i][1];
        R=circles[i][2];
        area=tempArea;
    }
}

void avgCircle(const vector<Vec3f> &circles,int &avgX,int &avgY,int &avgR){
    float X=0,Y=0,R=0,n=(float)circles.size();
    for(int i=0;i<circles.size();i++){
        X+=circles[i][0];
        Y+=circles[i][1];
        R+=circles[i][2];
    }
    avgX=(int)(X/n);
    avgY=(int)(Y/n);
    avgR=(int)(R/n);
}

int  findDistance(int x1,int y1,int x2,int y2){
    return sqrt(((x2-x1)*(x2-x1))+((y2-y1)*(y2-y1)));
}
CalibratedValue calibrateGauge(Mat img){
    
    int height=img.rows;//get height 
    int width=img.cols; //get width 
    printf("Size of the Image : [%d x %d]\n",width,height);

    //convert it into GrayScale image.
    Mat imgGray;    
    cvtColor(img,imgGray,COLOR_BGR2GRAY);   

    //get all circles in the Image using HoughCircles().
    vector<Vec3f> circles;
    HoughCircles(imgGray,circles,HOUGH_GRADIENT,1,2);
    printf("Number of Circle Detected : %zu\n",circles.size());

    if(circles.size()==0){
        putText(img,"Can't see the Gauge !!!",Point(20,20),FONT_HERSHEY_PLAIN,2,Scalar(0,0,255),1);
        cout<<"returing...\n";
    }

    //get Coordinates of center and radius of the biggest Circle and avg Circle
    int X,Y,R,avgX,avgY,avgR;
    biggestCircle(circles,X,Y,R);
    avgCircle(circles,avgX,avgY,avgR);//optional_1

    //draw the biggest circle on the image.
    circle(img,Point(X,Y),R,Scalar(0,255,0),3,LINE_AA);
    circle(img,Point(X,Y),2,Scalar(0,255,0),3,LINE_AA);
    circle(img,Point(avgX,avgY),avgR,Scalar(0,0,255),3,LINE_AA);//optional_1
    circle(img,Point(avgX,avgY),2,Scalar(0,0,255),3,LINE_AA);//optional_1

    //init for the vAle
    int separation=10;
    int interval=360/separation;
    vector<vector<double>>p1(interval,vector<double>(2,0));
    vector<vector<double>>p2(interval,vector<double>(2,0));
    vector<vector<double>>pText(interval,vector<double>(2,0));

    //record the points of interval for line and Text placement
    for(int i=0;i<p1.size();i++){
        for(int j=0;j<2;j++){
            if(j%2==0)p1[i][j]=X+(0.9*R*cos(separation*i* PI/180));
            else p1[i][j]=Y+(0.9*R*sin(separation*i* PI/180));
        }
    }
    int textOffsetX = 30,textOffsetY = 5;
    for(int i=0;i<p2.size();i++){
        for(int j=0;j<2;j++){
            if(j%2==0){
                p2[i][j]=X+(R*cos(separation*i* PI/180));
                pText[i][j]=X-textOffsetX+(1.2*R*cos(separation*(i+9)* PI/180));            
            }
            else{
                p2[i][j]=Y+(R*sin(separation*i* PI/180));
                pText[i][j]=Y+textOffsetY+(1.2*R*sin(separation*(i+9)* PI/180));
            }
        }
    }

    //draw line and text of respective angle
    for(int i=0;i<interval;i++){
        line(img,Point((int)p1[i][0],(int)p1[i][1]),Point((int)p2[i][0],(int)p2[i][1]),Scalar(0,255,0),3,LINE_AA);
        putText(img,to_string(i*separation),Point(pText[i][0],pText[i][1]),FONT_HERSHEY_PLAIN,2,Scalar(255,0,255),2,LINE_AA);
    }
    // //show updated Image.
    imshow("circle",img);

    /*int minVal,maxVal,minAng,maxAng;
    string units;
    cout<<"\nLowest possible angle of dial: ";
    cin>>minAng;
    cout<<"Highest possible angle of dial: ";
    cin>>maxAng;
    cout<<"Min Value: ";
    cin>>minVal;
    cout<<"Max Value: ";
    cin>>maxVal;
    cout<<"Enter Units: ";
    cin>>units;

    CalibratedValue val(X,Y,R,minAng,maxAng,minVal,maxVal,units);*/
    CalibratedValue val(X,Y,R,1,1,1,1,"psi");
    return val;
}

void getCurrentValue(Mat img,CalibratedValue){
    Mat imgGray,imgThr;
    cvtColor(img,imgGray,COLOR_BGR2GRAY);

    double thr=140,maxValue=255;
    double dst=threshold(imgGray,imgThr,thr,maxValue,THRESH_BINARY_INV);

    double minLineLength=10,maxLineGap=0;
    vector<Vec4i> lines;
    HoughLinesP(imgGray,lines,3.0,(PI/180),100,minLineLength,maxLineGap);
    printf("Number of Line detected : %zu",lines.size());
    // draw lines
    Mat resultImage = img.clone();
    for (int i = 0; i < lines.size(); ++i) {
        Vec4i l = lines[i];
        line(resultImage, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, 255), 1, LINE_AA);
    }
    imshow("line",resultImage);

    // int diff1LowerBound = 0.15; 
    // int diff1UpperBound = 0.25;
    // int diff2LowerBound = 0.5;
    // int diff2UpperBound = 1.0;
    


    imshow("thr",imgThr);
}  

int main(){
    //Read Img and resize accordingly
    string path="/Users/vishalghige/Desktop/#C0DE/Comp Vision/Remote Calibrate/assets/gauge2.jpg";
    Mat img=imread(path);
    resize(img,img,Size(0,0),0.5,0.5);

    //get Calibrarted Value
    CalibratedValue c=calibrateGauge(img);
    getCurrentValue(img,c);
    //show
    imshow("img",img);
    waitKey();
    return 0;
}


// int main(){
//     VideoCapture cap(0);
//     Mat img;
//     while(1){
//         cap.read(img);
//         resize(img,img,Size(0,0),0.5,0.5);
//         flip(img,img,1);
//         // imshow("IMG",img);
//         calibrateGauge(img);
//        if(waitKey(1)==27)break;
//     }
//     return 0;
// }