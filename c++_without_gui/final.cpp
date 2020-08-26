#include <iostream>
#include <string.h>
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/core/core.hpp"
#include <bits/stdc++.h>
#include <math.h>

using namespace std;
using namespace cv; 

// Sudoku grid
int grid[9][9];

// For a single datapoint of KNN algorithm dataset
struct datapoint 
{
    int val;               // Group of datapoint
    Mat digit;             // Feature values
    double distance;       // 'Distance' from test point
};

// To enable sorting to find the k nearest neighbour
bool comparison ( datapoint a , datapoint b )
{
    return (a.distance < b.distance);
}

// KNN algorithm
class KNearestNeighbors
{
    private:
        int k;
        vector <datapoint> imgs;        

    public:
        
        // Constructor
        KNearestNeighbors ( int n_neighbors = 5 ) { k = n_neighbors; }

        // To find the 'distance' between two datapoints ; Note: cosine similarity can also be used
        double dist ( Mat a , Mat b );

        // To load an image from the dataset
        void load(string s,int group);

        // To load the dataset
        void fit_transform();

        // To predict a number between 1-9 given its image
        int predict ( Mat img );

        // Destructor
        ~KNearestNeighbors(){}
        
};

// Sudoku solver 
class sudoku
{
    private:
        int arr[9][9];

    public:

        // Constructor
        sudoku(){}
        
        // To start the solving of sudoku
        void initiate ();
        
        // To display the array x of dimension mxn
        void show ( int x[][9] , int m , int n );
        
        // To the find the center of small 3x3 squares in 9x9
        void findc ( int &c );
        
        // To check whether num is valid at index (x,y)
        bool isValid ( int arr[][9] , int x , int y , int num );
        
        // To solve and dislay final array
        void solve ( int arr[][9] );
        
        // Displays the array once solved
        void whenDone ();

        // Destructor
        ~sudoku(){}        
};

// For image preprocessing
class scanner
{
    private:
        Mat img;
        KNearestNeighbors k;

    public:

        // Constructor
        scanner ( string s , KNearestNeighbors temp );

        // Returns a number using the KNN algorithm
        int getNum ( Mat img );

        // Finds the Euclidean distance between two points
        float distance ( Point p , int i , int j );

        // To find corresponding points for homography transformation
        int findQuad ( Point p , Mat img );

        // Returns a 900x900 resized version of a binary image of the sudoku grid
        Mat preprocessing ( Mat img );

        // To scan each square of the grid and return the digit it contains
        void getDigits ();

        // Destructor
        ~scanner(){}        
};

double KNearestNeighbors :: dist ( Mat a , Mat b )
{
    double val = 0;

    for ( int i = 0 ; i < 20 ; i++ )
    {
        for ( int j = 0 ; j < 20 ; j++ )
        {
            val = val + (a.at<uchar>(i,j)-b.at<uchar>(i,j))*(a.at<uchar>(i,j)-b.at<uchar>(i,j));
        }
    }

    return sqrt(val);
}

void KNearestNeighbors :: load(string s,int group)
{
    vector<cv::String> fn;
    glob(s, fn, false);

    vector<Mat> images;
    size_t count = fn.size(); //number of png files in images folder
            
    for (size_t i=0; i<count; i++)
    {
        Mat temp = imread(fn[i],0);
        datapoint t;
        t.val = group;
        t.digit = temp;
        imgs.push_back(t);
    }
}

void KNearestNeighbors :: fit_transform()
{
    string direct = "Data/";
    string temp = "/*.png";

    for ( int i = 0 ; i <= 9 ; i++ )
    {
        string new_direct = direct + to_string(i) + temp;
        load(new_direct,i);
    }
}

int KNearestNeighbors :: predict ( Mat img )
{
    for ( int i = 0 ; i < imgs.size() ; i++ )
    {
        imgs[i].distance = dist ( imgs[i].digit , img );
    }

    sort(imgs.begin(),imgs.end(),comparison);

    int freq[10] = {0};

    for ( int i = 0 ; i < k ; i++ )
    {
        for ( int j = 0 ; j < 10 ; j++ )
        {
            if ( imgs[i].val == j )
            {
                freq[j]++;
            }
        }
    }

    return max_element(freq, freq + sizeof(freq)/sizeof(int)) - freq ;
}

void sudoku :: show ( int x[][9] , int m , int n )
{
    for ( int i = 0 ; i < 9 ; i++ )
    {
        for ( int j = 0 ; j < 9 ; j++ )
        {
            cout<<x[i][j]<<" ";
        }
        cout<<endl;
    }
    cout<<endl;
}

void sudoku :: findc ( int &c )
{
    switch(c)
    {
        case 0:
        case 1:
        case 2: c = 1;
                return;

        case 3:
        case 4:
        case 5: c = 4;
                return;

        case 6:
        case 7:
        case 8: c = 7;
                return;
    }
}

bool sudoku :: isValid ( int arr[9][9] , int x , int y , int num )
{
    bool check = true;
    int i,j;

    for ( i = 0 ; i < 9 ; i++ )
    {
        if ( i == x )
            continue;

        if ( arr[i][y] == num && check == true )
        {
            check = false;
            return check;
        }
    }

    for ( j = 0 ; j < 9 ; j++ )
    {
        if ( j == y )
            continue; 

        if ( arr[x][j] == num && check == true )
        {
            check = false;
            return check;
        }
    }

    i = x;
    j = y;
    findc(i);
    findc(j);

    for ( int k = i-1 ; k < i+2 ; k++ )
    {
        for ( int l = j-1 ; l < j+2 ; l++ )
        {
            if ( k == x && l == y )
                continue;

            if ( arr[k][l] == num && check == true )
            {
                check = false;
                return check;
            }
        }
    }

    return check;
}

void sudoku :: solve ( int arr[9][9] )
{
    for ( int i = 0 ; i < 9 ; i++ )
    {
        for ( int j = 0 ; j < 9 ; j++ )
        {
            if ( arr[i][j] == 0 )
            {
                for ( int k = 1 ; k <= 9 ; k++ )
                {
                    if ( isValid (arr,i,j,k) == true )
                    {
                        arr[i][j] = k;
                        whenDone();
                        solve(arr);
                        arr[i][j] = 0;
                    }

                    if ( k == 9 )
                    {
                        arr[i][j] = 0;
                        return;
                    }
                }

                if ( !( isValid(arr,i,j,1) || isValid(arr,i,j,2) || isValid(arr,i,j,3) || isValid(arr,i,j,4) || isValid(arr,i,j,5) || isValid(arr,i,j,6) || isValid(arr,i,j,7) || isValid(arr,i,j,8) || isValid(arr,i,j,9) ) )
                {
                    arr[i][j] = 0;
                    return;
                }
            }
        }
    }
    show(arr,9,9);
    exit(0);
}

void sudoku :: initiate ()
{
    for ( int i = 0 ; i < 9 ; i++ )
    {
        for ( int j = 0 ; j < 9 ; j++ )
        {
            arr[i][j] = grid[i][j];
        }
    }

    solve(arr);
}

void sudoku :: whenDone()
{
    for ( int i = 0 ; i < 9 ; i++ )
    {
        int sum = 0;

        for ( int j = 0 ; j < 9 ; j++ )
        {
            sum = sum + arr[i][j];
        }

        if ( sum != 55 )
        {
            return;
        }
    }

    show(arr,9,9);
}

scanner :: scanner ( string s , KNearestNeighbors temp )
{
    img = imread(s,0);
    k = temp;
    k.fit_transform();
}

int scanner :: getNum ( Mat img )
{
    return k.predict(img);
}

float scanner :: distance ( Point p , int i , int j )
{
    return (i-p.x)*(i-p.x) + (j-p.y)*(j-p.y);
}

int scanner :: findQuad ( Point p , Mat img )
{
    vector<Point> s;
    Point p1(0,0) , p2(img.cols,0) , p3(img.cols,img.rows) , p4(0,img.rows);
    s.push_back(p1);
    s.push_back(p2);
    s.push_back(p3);
    s.push_back(p4);

    double d = 0;
    int min = 0;

    for ( int i = 0 ; i < 4 ; i++ )
    {
        if ( distance(p,s[i].x,s[i].y) > d )
        {
            d = distance(p,s[i].x,s[i].y);
            min = i;
        }
    }

    return min;
}

Mat scanner :: preprocessing ( Mat img )
{
    Mat img_blur , canny_output , warp_output , binary_output , square ;

    GaussianBlur( img , img_blur , Size(3,3) , 0 );                     //blurring to remove noise ; Note: we can use an edge preserving filter

    double otsu_thresh_val = threshold(img_blur, img_blur , 0, 255, THRESH_BINARY | THRESH_OTSU);           // Using Otsu thresholding to find the thresholds for Canny Edge detection
    double high_thresh_val  = otsu_thresh_val, lower_thresh_val = otsu_thresh_val * 0.5;

    Canny ( img_blur , canny_output , lower_thresh_val , high_thresh_val );                   //Canny edge detection                  

    vector<vector<Point>> contours;                                     //contour detection ; Note: needs improvement as in some cases it detects some random contour and not the grid
    findContours( canny_output , contours , RETR_EXTERNAL , CHAIN_APPROX_SIMPLE );       
    double max_area = 0;
    int temp = 0;

    for ( int  i = 0 ; i < contours.size() ; i++ )
    {
        if ( contourArea(contours[i]) >= max_area )
        {
            max_area = contourArea(contours[i]);                       //finding contour of maximum area ; Note: can also use sort function and use contour at index 0
            temp = i;
        }
    }

    double peri = arcLength ( contours[temp] , true );                 //perimeter of outer rectangle
    vector<Point> rect;
    approxPolyDP ( contours[temp] , rect , 0.02*peri , true );         //approxiating the contour to a rectangle

    Point2f inputQuad[4];                                              //Input Quadilateral or Image plane coordinates
    Point2f outputQuad[4];                                             //Output Quadilateral or World plane coordinates
    Mat perspectiveMatrix( 2, 4, CV_32FC1 );                           //perspectiveMatrix

    perspectiveMatrix = Mat::zeros( img.rows, img.cols, img.type() );  //setting it as same type as input image

    for ( int i = 0 ; i < 4 ; i++ )
    {
        inputQuad[findQuad(rect[i],img)] = rect[i];
    }

    outputQuad[0] = Point2f( img.cols , img.rows );
    outputQuad[1] = Point2f( 0 , img.rows );
    outputQuad[2] = Point2f( 0 , 0 );
    outputQuad[3] = Point2f( img.cols , 0 );

    perspectiveMatrix = getPerspectiveTransform( inputQuad, outputQuad );     //Get the Perspective Transform Matrix i.e. perspectiveMatrix
    warpPerspective(img,warp_output,perspectiveMatrix,warp_output.size() );   //Apply the Perspective Transform to the input image

    int size = warp_output.rows*warp_output.cols/2188;                        //Get the kernel size for adaptive threshold
    if ( size%2 == 0 )  size++;                                               //Making it odd

    adaptiveThreshold( warp_output , binary_output , 255 , ADAPTIVE_THRESH_MEAN_C , THRESH_BINARY , size , 0 );       //Using adaptive thresholding to obtain binary image

    Size s(900,900);
    resize(binary_output,square,s);
    return square;
}

void scanner :: getDigits ()
{
    Mat square = preprocessing(img);

    for ( int x = 0 ; x < 9 ; x++ )
    {
        for ( int y = 0 ; y < 9 ; y++ )
        {
            grid[x][y] = 0;

            Mat elm( square.rows*0.13 , square.cols*0.13, CV_8UC1 , Scalar(0) );                      //Making an image containing a square of the grif

            for ( int i = 0 ; i < elm.rows ; i++ )        
            {
                for ( int j = 0 ; j < elm.cols ; j++ )
                {
                    if ( i+int(square.rows*x/9) < square.rows && j+int(square.cols*y/9) < square.cols )               //Checking the pixel is valid
                        elm.at<uchar>(i,j) = square.at<uchar>(i+int(square.rows*x/9),j+int(square.cols*y/9));                //Extracting a square
                    else
                        elm.at<uchar>(i,j) = 0;                
                }
            }

            vector<vector<Point>> num;
            findContours( elm , num , RETR_EXTERNAL , CHAIN_APPROX_SIMPLE );

            double area = 0;
            int idx = 0;

            for ( int  i = 0 ; i < num.size() ; i++ )
            {
                if ( contourArea(num[i]) >= area )
                {
                    area = contourArea(num[i]);                       //Finding contour of maximum area
                    idx = i;
                }
            }

            Rect n = boundingRect(num[idx]);

            
            Mat number = elm(n);                                              //Cropping out the number from the cell
            Mat fin (number.rows-10,number.cols-10, CV_8UC1 , Scalar(0) );
                    
            for ( int i = 5 ; i < number.rows-5 ; i++ )
            {
                for ( int j = 5 ; j < number.cols-5 ; j++ )
                {
                    fin.at<uchar>(i-5,j-5) = number.at<uchar>(i,j);
                }
            }

            resize(fin,fin,Size(20,20));
            grid[x][y] = getNum ( fin );                                     //Assigning the corresponding numbber to the array
        }
    }
}

int main(int argc , char *argv[] )
{
    KNearestNeighbors knn (1);

    scanner s (argv[1],knn);

    s.getDigits();

    sudoku x;

    x.initiate();

    return 0;
}
