#ifndef LUXFRAME_H
#define LUXFRAME_H
#include <opencv2/highgui/highgui.hpp>

class LuxFrame
{
public:
    cv::Mat image;
    cv::Mat depth_map;

    LuxFrame();

    cv::Point3d getPoint3D(int x, int y) const
    {
        cv::Point3d point;
        point.x = depth_map.at<cv::Vec3f>(y, x)[0];
        point.y = depth_map.at<cv::Vec3f>(y, x)[1];
        point.z = depth_map.at<cv::Vec3f>(y, x)[2];

        return point;
    }
    void setPoint3D(int x, int y, cv::Point3d point)
    {
        depth_map.at<cv::Vec3f>(y, x)[0] = point.x;
        depth_map.at<cv::Vec3f>(y, x)[1] = point.y;
        depth_map.at<cv::Vec3f>(y, x)[2] = point.z;

    }

    // copy constructor
    LuxFrame(const LuxFrame & lf)
    {
        lf.image.copyTo(this->image);
        lf.depth_map.copyTo(this->depth_map);
    }

    void Transform(cv::Mat R, cv::Point3d t);
};

#endif // LUXFRAME_H
