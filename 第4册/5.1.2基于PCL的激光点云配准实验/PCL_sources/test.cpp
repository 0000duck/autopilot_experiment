#include<pcl/visualization/pcl_visualizer.h>
#include<iostream>//��׼C++���е�������������ͷ�ļ���
#include<pcl/io/io.h>
#include<pcl/io/pcd_io.h>//pcd ��д����ص�ͷ�ļ���

using namespace std;
using namespace pcl;
typedef pcl::PointXYZRGB PointIoType;//�Լ���������ͽ��к궨��

int main(int argc, char** argv){
	pcl::PointCloud<PointIoType>::Ptr cloud(new pcl::PointCloud<PointIoType>());
	if (pcl::io::loadPCDFile("H:/pro_software_workplace/VisualStudio/Project1/Project1/128.pcd", *cloud) == -1)
		return (-1);
	boost::shared_ptr<pcl::visualization::PCLVisualizer> viewer1(new pcl::visualization::PCLVisualizer("3D Viewer"));
	viewer1->addPointCloud<PointIoType>(cloud, "128.pcb");
	viewer1->setBackgroundColor(0, 0, 0);//���ÿ��ӻ����ڵı�����ɫΪ��ɫ
	while (!viewer1->wasStopped())//�־û���ʾ�������ͼ��
	{
		viewer1->spinOnce(100);
		boost::this_thread::sleep(boost::posix_time::microseconds(100000));
	}

	return 0;
}
