#include "Process.h"


Process::Process()
{

}
Process::~Process()
{

}

void Process::execute(){
	pcl::PCDReader reader;
	//�����������Ʊ������ڽ��������ںϵ���Ƭ��������
	pcl::PointCloud<PointIoType>::Ptr cloud(new pcl::PointCloud<PointIoType>())
									, cloud2(new pcl::PointCloud<PointIoType>());
	//��ȡ�����ļ�����
	reader.read("H:/pro_software_workplace/VisualStudio/Project1/Project1/288.pcd", *cloud);
	reader.read("H:/pro_software_workplace/VisualStudio/Project1/Project1/128.pcd", *cloud2);
	mix(&cloud, &cloud2);//�����ں�
}
void Process::mix(PointPtr *sour, PointPtr *dist){
	Eigen::Vector3d D_value_vector = Kdsearch();//��������ֵ����
	//�����������Ʊ������մ���ĵ���ָ��
	pcl::PointCloud<PointIoType>::Ptr p_sour(new pcl::PointCloud<PointIoType>())
									, p_dist(new pcl::PointCloud<PointIoType>());
	p_sour = *sour;
	p_dist = *dist;
	//ͨ��forѭ��������Դ�����е����е㣬
	//��ÿ����x��y���궼��ȥ��Ƭ����֮�������ֵ
	for (size_t i = 0; i < p_sour->points.size(); i++){
		p_sour->points[i].x = p_sour->points[i].x - D_value_vector.x();
		p_sour->points[i].y = p_sour->points[i].y - D_value_vector.y();
	}
	std::string path = "./288_1.pcd";
	//�����ȥ�����Դ����
	pcl::io::savePCDFileBinaryCompressed<PointIoType>(path, *p_sour);
}

Eigen::Vector3d Process::Kdsearch(){
	// ����һ�����Ʊ��������ڴ洢Դ���������������ĸ˰�Χ������
	pcl::PointCloud<pcl::PointXYZL>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZL>);
	int size_of_sour_pole = sour_pole.size();
	int size_of_dist_pole = dist_pole.size();
	cloud->width = size_of_dist_pole;
	cloud->height = 1;
	cloud->points.resize(cloud->width * cloud->height);
	for (int i = 0; i < size_of_dist_pole; ++i)
	{
		cloud->points[i].x = dist_pole.at(i).x;
		cloud->points[i].y = dist_pole.at(i).y;
		cloud->points[i].z = dist_pole.at(i).z;
		cloud->points[i].label = 0;
	}
	float radius = 1.0f;
	int match_size = 0;
	pcl::KdTreeFLANN<pcl::PointXYZL> kdtree;
	pcl::PointXYZL searchPoint;
	std::vector<int> pointIdxRadiusSearch;
	std::vector<float> pointRadiusSquaredDistance;
	Eigen::Vector3d meanVector(0, 0, 0);
	kdtree.setInputCloud(cloud);
	//����Դ���Ƹ˵����ĵ㣬��������dist_pole��������ĵ�
	for (int sour_index = 0; sour_index < size_of_sour_pole; sour_index++){
		searchPoint.x = sour_pole.at(sour_index).x;
		searchPoint.y = sour_pole.at(sour_index).y;
		searchPoint.z = sour_pole.at(sour_index).z;
		std::cout << "������Ϊ" << searchPoint.x << " " << searchPoint.y << " " << searchPoint.z << std::endl;
		//ɸѡ�����������ĵ���
		if (kdtree.radiusSearch(searchPoint, radius, pointIdxRadiusSearch, pointRadiusSquaredDistance) > 0){
			//ɸѡ��û��ƥ����ĵ�
			if (cloud->points[pointIdxRadiusSearch[0]].label == 0){
				//ɸѡ������С��1m�ĵ���
				if (pointRadiusSquaredDistance[0]<1){
					std::cout << "ƥ���Ϊ" << 
					cloud->points[pointIdxRadiusSearch[0]].x << " " <<
					cloud->points[pointIdxRadiusSearch[0]].y << " " <<
					cloud->points[pointIdxRadiusSearch[0]].z << " (����: " <<
					pointRadiusSquaredDistance[0] << ")" << std::endl;
					//���õ���Ϊ��ƥ��
					cloud->points[pointIdxRadiusSearch[0]].label = 1;
					//�����������
					meanVector.x() = meanVector.x() + searchPoint.x - cloud->points[pointIdxRadiusSearch[0]].x;
					meanVector.y() = meanVector.y() + searchPoint.y - cloud->points[pointIdxRadiusSearch[0]].y;
					match_size += 1;
				}
				else{
					std::cout << "û��ƥ����ڽ���" << std::endl;
				}
			}
			else{
				std::cout << "û��ƥ����ڽ���" << std::endl;
			}

		}
	}
	//���ؼ���õ�����ֵ����
	return meanVector / match_size;
}

void Process::txt_Reader(){
	FILE *fp_1;
	fp_1 = fopen("H:/pro_software_workplace/VisualStudio/Project1/Project1/feature.txt", "r");
	if (fp_1){
		for (int i = 0; i < SIZE_OF_FEATURES; ++i){
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
	for (int i = 0; i < SIZE_OF_FEATURES; i++){
		//ɸѡ���˵İ�Χ�����ĵ�������
		if (features[i].type == 206){
			//����index������������Ƭ���ư�Χ�����ݣ�������
			if (features[i].index == 1){
				sour_pole.push_back(features[i]);
			}
			else{
				dist_pole.push_back(features[i]);
			}
		}
	}
}



