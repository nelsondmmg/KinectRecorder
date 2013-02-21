#include "luxframe.hpp"

LuxFrame::LuxFrame()
{

}

void LuxFrame::Transform(cv::Mat R, cv::Point3d t)
{
    for (int x=0;x<640;x++)
        for (int y=0;y<480;y++)
        {
            cv::Point3d curr_point = this->getPoint3D(x,y);
            if (!curr_point.z) continue;

            curr_point.x=   R.at<float>(0,0)*curr_point.x+
                            R.at<float>(0,1)*curr_point.y+
                            R.at<float>(0,2)*curr_point.z+ t.x;

            curr_point.y=   R.at<float>(1,0)*curr_point.x+
                            R.at<float>(1,1)*curr_point.y+
                            R.at<float>(1,2)*curr_point.z+ t.y;

            curr_point.z=   R.at<float>(2,0)*curr_point.x+
                            R.at<float>(2,1)*curr_point.y+
                            R.at<float>(2,2)*curr_point.z+ t.z;

            setPoint3D(x,y,curr_point);
        }
}
