//===================================================
// Name        : inclinometre.cpp
// Author      : Antoine & Adrien
// Version     :
// Copyright   : 
// Description : calcul de l'inclinaison, publication de l'inclinaison, la Vitesse angulaire et de l'acceleration linéaire
//===================================================

#include "GestionMPU6050.h"

using namespace std;

// Objets utilisés
    GestionMPU6050 mpu6050(ADDRESS_AD0_HIGH);

// *************************************
// Fonction de fin déclenchée par CTRL-C
// *************************************
void fin(int sig)
{
	exit(0);
}

// ******************
// Fonction pricipale
// ******************
int main(int argc, char **argv) {


	ros::init(argc, argv, "publish_velocity");
	ros::NodeHandle nh;

	ros::Publisher pub_acc = nh.advertise<geometry_msgs::Vector3>("/mpu_Lacc", 1000);	// topic de l'acceleration linéaire
	ros::Publisher pub_vit = nh.advertise<geometry_msgs::Vector3>("/mpu_Avit", 1000);	//topic de la vitesse angulaire
	ros::Publisher pub_pos = nh.advertise<geometry_msgs::Vector3>("/mpu_Apos", 1000);	//topic de la position angulaire
	
	ros::Rate rate(F);
	// Ecoute du CTRL-C avec fonction à lancer
	signal(SIGINT, fin);

	cout << "Test du MPU6050" << endl;
	mpu6050.defaultInitialize();
	cout << "Who am I: " << hex << (int)mpu6050.whoAmI() << dec << endl;
	if(mpu6050.isConnectionOK()) {
		cout << "Connection: OK" << endl;
	} else {
		cout << "Connection: KO" << endl;
		exit(-1);
	}
	
	mpu6050.setXGyroOffset(89);
	mpu6050.setYGyroOffset(-8);
	mpu6050.setZGyroOffset(104);	

	mpu6050.setXAccelOffset(-1041);
	mpu6050.setYAccelOffset(-11);
	mpu6050.setZAccelOffset(1333);		
	
	delay(1000);
	
	geometry_msgs::Vector3 lvAccL;
	geometry_msgs::Vector3 lvVitA;
	geometry_msgs::Vector3 lvPosA;
		
	
	while(ros::ok) 
	{			
		lvAccL.x = (double)mpu6050.getAccelXMS();
		lvAccL.y = (double)mpu6050.getAccelYMS();
		lvAccL.z = (double)mpu6050.getAccelZMS();
		
		lvPosA.y = atan2(lvAccL.x, lvAccL.z) * 180.0 / M_PI;
		lvPosA.x = atan2(lvAccL.y, lvAccL.z) * 180.0 / M_PI;
								
		lvVitA.x = mpu6050.getGyroXDegS();
		lvVitA.y = mpu6050.getGyroYDegS();
		lvVitA.z = mpu6050.getGyroZDegS();
			
		pub_acc.publish(lvAccL);
		pub_vit.publish(lvVitA);
		pub_pos.publish(lvPosA);
		
		rate.sleep();		
	}
	rate.sleep();
	ros::spinOnce();
}
