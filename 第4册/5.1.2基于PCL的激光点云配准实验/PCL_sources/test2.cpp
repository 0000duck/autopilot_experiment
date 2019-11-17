#include<pcl/visualization/pcl_visualizer.h>
#include<iostream>//��׼C++���е�������������ͷ�ļ���
#include<pcl/io/io.h>
#include<pcl/io/pcd_io.h>//pcd ��д����ص�ͷ�ļ���

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

void BoundingBox(PointPtr *c_sour){
	FILE *fp_1;//����һ���ļ�����ָ��
	feature features[20];//����һ������20���ṹ�����������
	fp_1 = fopen("H:/pro_software_workplace/VisualStudio/Project1/Project1/feature_128.txt", "r");//���ļ�feature_128.txt
	if (fp_1){
		for (int i = 0; i < 20; ++i){
			//ѭ����20������д��feature�ṹ��������
			fscanf(fp_1, "%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%ld,%ld\n",
				&features[i].x, &features[i].y, &features[i].z,
				&features[i].length, &features[i].width, &features[i].height,
				&features[i].dir_x, &features[i].dir_y, &features[i].dir_z,
				&features[i].type, &features[i].index);
		}
	}
	else
		std::cout << "���ݼ���ʧ��" << std::endl;
	fclose(fp_1);

	PointPtr sour(new pcl::PointCloud<PointIoType>());
	sour = *c_sour;//���մ���ĵ���
	boost::shared_ptr<pcl::visualization::PCLVisualizer> viewer(new pcl::visualization::PCLVisualizer("3D Viewer"));//ʵ����һ�����ƿ��ӻ�����
	viewer->setBackgroundColor(0, 0, 0);//���ñ���Ϊ��ɫ
	viewer->addPointCloud<PointIoType>(sour, "sample sour");//�����Ҫ��ʾ�ĵ���
	for (int i = 0; i<20; i++){
		//ѭ������feature�ṹ�����ݣ����ɰ�Χ��
		Eigen::Vector3d noml(features[i].dir_x, features[i].dir_y, features[i].dir_z);//��Vector3d���ʹ洢������
		//noml.normalize();//��������λ��
		Eigen::Matrix3f mt;//������ת����
		mt << noml.x(), -noml.y(), noml.z(),
			noml.y(), noml.x(), noml.z(),
			0, 0, 1.0;
		Eigen::Quaternionf q(mt);//������Ԫ��
		noml.normalize();
		Eigen::Vector3f translation(features[i].x, features[i].y, features[i].z);//����һ���������Ͱ�Χ��������������
		viewer->addCube(translation, q, features[i].length, features[i].width, features[i].height, "bbox" + to_string(i));//��viewer����Ӱ�Χ��
	}
	while (!viewer->wasStopped())//����ͼ��־û�
	{
		viewer->spinOnce(100);
		boost::this_thread::sleep(boost::posix_time::microseconds(100000));
	}
}

int main(int argc, char** argv){
	pcl::PointCloud<PointIoType>::Ptr cloud(new pcl::PointCloud<PointIoType>());
	//��ȡҪ��ʾ�ĵ����ļ�
	if (pcl::io::loadPCDFile("H:/pro_software_workplace/VisualStudio/Project1/Project1/128.pcd", *cloud) == -1)
		return (-1);
	BoundingBox(&cloud);//���ð�Χ����Ӻ���
	return 0;
}
