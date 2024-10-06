 #include "windmill.hpp"
#include <ceres/ceres.h>
#include <ceres/rotation.h>
#include <chrono>
#include <cmath>
#include <iostream>
#include <numeric>
#include <opencv2/core.hpp>
#include <opencv2/core/operations.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <thread>
#include <vector>
//#include <algorithm>
using namespace cv;
using namespace std;
using namespace ceres;
using namespace chrono;



struct Costfunctors
{
Costfunctors(double the,double time):
_the(the),_time(time){}
template<typename T>
bool operator()(const T *const D, T *residual) const {
    T B = D[0];
    T A = D[1];
    T w = D[2];
    T phi = D[3];



    residual[0] = cos(B * T(_time) - (A / w) * (ceres::sin(phi )-ceres::sin(w * (T(_time)) + phi ))) - T(_the);
    return true;
  }

private:
const double _the, _time;
};
const double true_A = 0.785;
    const double true_w = 1.884;
    const double true_phi = 0.24;
    const double true_B = 1.305;

int main()
{std::vector<double> timings;
  int times = 1;
   while (times <= 10) {std::chrono::milliseconds t = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
    WINDMILL::WindMill wm(t.count());
    double every_start = (double)t.count();
    cv::Mat src;
    cout << "外层循环 " << times << endl;  // 输出外层循环
    
     

    
Mat targetR = imread("../image/R.png", IMREAD_GRAYSCALE);
Mat targetHammer = imread("../image/target.png", IMREAD_GRAYSCALE);
double D[4]={0.305,1.785,0.884,1.24};
int oringin = 0;ceres::Problem problem;
    while(1)
    {   
        t = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
        src = wm.getMat((double)t.count());
        
        //==========================代码区========================//
       
       oringin++;
         cout << "当前迭代" << oringin << endl;  // 输出当前迭代次数
         cout << "外层循环 " <<  times << endl;
        auto start = std::chrono::high_resolution_clock::now();
       Mat gray;cvtColor(src,gray,COLOR_BGR2GRAY);
        Mat Rresult;matchTemplate(gray, targetR, Rresult, 5);//??????????
        
        double minVal_R, maxVal_R;Point minLoc_R, maxLoc_R;
      minMaxLoc(Rresult, &minVal_R, &maxVal_R, &minLoc_R, &maxLoc_R);//??????????
      Point2d center;
       center.x=maxLoc_R.x + targetR.cols/2;
       center.y=maxLoc_R.y + targetR.rows/2;

       Mat hammer, yuan;
      threshold(targetHammer, hammer, 64, 255, THRESH_BINARY);
      threshold(gray, yuan, 64, 255, THRESH_BINARY);

      vector<vector<Point>>chammer,cyuan;
       findContours(hammer,chammer,RETR_EXTERNAL,CHAIN_APPROX_NONE);
       findContours(yuan,cyuan,RETR_EXTERNAL,CHAIN_APPROX_NONE);
       double area=contourArea(chammer[0]);
       Point2d center1;

//?????
       for (size_t i = 0; i < cyuan.size(); i++) {
        double tarea = contourArea(cyuan[i]);
        if (fabs(tarea - area) < 0.09 * area) {
          drawContours(src, cyuan, (int)i, Scalar(0, 255, 255), 3);
          Moments m = moments(cyuan[i]);
          Point2d center2 = Point2f(m.m10 / m.m00, m.m01 / m.m00);
          center1 = center2 - center;
          circle(src, center, 10, Scalar(255, 0, 0), -1);break;}}

          Point2d normcenter1 = center1 / norm(center1);
          double x = normcenter1.x;


          double time=(double)t.count();
          double dt=(time-every_start)/1000;
          problem.AddResidualBlock(new ceres::AutoDiffCostFunction<Costfunctors,1,4>(new Costfunctors(x,dt)),nullptr,D);
          ceres::Solver::Options options;
      options.max_num_iterations = 1000;
      options.linear_solver_type = ceres::DENSE_QR;
        problem.SetParameterLowerBound(D, 0, 1);
        problem.SetParameterUpperBound(D, 0, 1.5);
        problem.SetParameterLowerBound(D, 1, 0.5);
        problem.SetParameterUpperBound(D, 1, 1);
        problem.SetParameterLowerBound(D, 2, 1.5);
        problem.SetParameterUpperBound(D, 2, 2);
        problem.SetParameterLowerBound(D, 3, 0.2);
        problem.SetParameterUpperBound(D, 3, 0.5);
        ceres::Solver::Summary summary;
        ceres::Solve(options,&problem,&summary);


        cout << "A0: " << D[0] << ", true_A0: " << true_B << endl;
      cout << "A: " << D[1] << ", true_A: " << true_A << endl;
      cout << "omega: " << D[2] << ", true_w: " << true_w << endl;
      cout << "phi: " << D[3] << ", true_phi: " << true_phi<< endl;

           bool converged = std::abs(D[0] - true_B) < 0.05 * true_B &&
                            std::abs(D[1] - true_A) < 0.05 * true_A &&
                            std::abs(D[2] - true_w) < 0.05 * true_w &&
                            std::abs(D[3] - true_phi) < 0.05 * true_phi;
          if (converged) {
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        timings.push_back(elapsed.count());
        times++;
        break;
                         } 
         




    imshow("windmill", src);


        //=======================================================//
        
        waitKey(1);}}
        double average_time =
      std::accumulate(timings.begin(), timings.end(), 0.0) * 10;
  cout << "平均时间 " << average_time << "s" << endl;
  return 0;}
