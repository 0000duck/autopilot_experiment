#pragma once
#ifndef _PROCESS_
#define _PROCESS_
#include <math.h>
#include<stdio.h>
#include<pcl/io/io.h>
#include<pcl/io/pcd_io.h>//pcd ��д����ص�ͷ�ļ���
#include<pcl/io/ply_io.h>
#include<pcl/point_types.h> //PCL��֧�ֵĵ�����ͷ�ļ���
#include <boost/thread/thread.hpp>
#include <vector>
#include <pcl/kdtree/kdtree_flann.h>
#include<pcl/visualization/pcl_visualizer.h>
#include <Eigen/Core>

#define SIZE_OF_FEATURES 28 //�ںϵ���Ƭ����feature����
using namespace std;
using namespace pcl;
typedef pcl::PointXYZRGB PointIoType;
typedef pcl::PointCloud<pcl::PointXYZRGB>::Ptr PointPtr;

typedef struct feature_box
{
	double x;  //center_x
	double y;  //center_y
	double z;  //center_z
	double length;
	double width;
	double height;
	double dir_x;
	double dir_y;
	double dir_z;
	int type;
	int index;
}feature;

class Process //����һ��Process���ƴ�����
{
public:
	Process();
	~Process();
	void execute();//�������еĵ��õ��ں�ִ�к���
	void mix(PointPtr *sour, PointPtr *dist);//�ںϺ���
	Eigen::Vector3d Kdsearch();//Kdtree��������
	void txt_Reader();//feature.txt�ļ���ȡ����
private:
	std::vector<feature> sour_pole;//Kdtree������Դ����
	std::vector<feature> dist_pole;//Kdtree��������������
	feature features[SIZE_OF_FEATURES];//��Χ�нṹ����������
};

#endif
