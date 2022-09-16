/****************************************************************
 *
 * Copyright (c) 2011
 * All rights reserved.
 *
 * Hochschule Bonn-Rhein-Sieg
 * University of Applied Sciences
 * Computer Science Department
 *
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 *
 * Author:
 * Jan Paulus, Nico Hochgeschwender, Michael Reckhaus, Azamat Shakhimardanov
 * Supervised by:
 * Gerhard K. Kraetzschmar
 *
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 *
 * This sofware is published under a dual-license: GNU Lesser General Public
 * License LGPL 2.1 and BSD license. The dual-license implies that users of this
 * code may choose which terms they prefer.
 *
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Hochschule Bonn-Rhein-Sieg nor the names of its
 *       contributors may be used to endorse or promote products derived from
 *       this software without specific prior written permission.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License LGPL as
 * published by the Free Software Foundation, either version 2.1 of the
 * License, or (at your option) any later version or the BSD license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License LGPL and the BSD license for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License LGPL and BSD license along with this program.
 *
 ****************************************************************/

#include <unistd.h>
#include <cmath>
#include <limits>
#include <iostream>
#include <fstream>
#include <vector>
#include <signal.h>
#include "youbot_driver/youbot/YouBotManipulator.hpp"
#include "youbot_driver/youbot/YouBotBase.hpp"
#include "JointConfigurator.hpp"


using namespace std;
using namespace youbot;


void menu() {

  //  if (ArmOrBase == ARM) {
  //    std::cout << std::endl << "Manipulator Joint " << jointNumber << " | " << jointName  << std::endl;
  //    std::cout << "Controller Type: " << controllerType << " Firmware version: " << version << std::endl;
  //  } else if (ArmOrBase == BASE) {
  //    std::cout << std::endl << "Base Joint " << jointNumber << " | " << jointName  << std::endl;
  //    std::cout << "Controller Type: " << controllerType << " Firmware version: " << version << std::endl;
  //  }

  std::cout << "===========================================================" << std::endl;
  std::cout << "1 = show parameters from joint and config file" << std::endl;
  std::cout << "2 = set config file values to joint" << std::endl;
  std::cout << "3 = store config file values to joint" << std::endl;
  std::cout << "===========================================================" << std::endl;
  std::cout << "4 = show password protected parameters" << std::endl;
  std::cout << "5 = enter password" << std::endl;
  std::cout << "6 = set password protected parameters" << std::endl;
  std::cout << "7 = store password protected parameters" << std::endl;
  std::cout << "===========================================================" << std::endl;
  std::cout << "8 = show read only parameters" << std::endl;
  std::cout << "0 = quit" << std::endl;
  std::cout << "===========================================================" << std::endl;
  std::cout << ": " << std::flush;
}

bool running = true;

void sigintHandler(int signal) {
  running = false;
  std::cout << std::endl << " Interrupt!" << std::endl;
}

int main(int argc, char *argv[]) {

  //  signal(SIGINT, sigintHandler);

  try {

    if (argc < 3 || argc > 5) {
      std::cout << "Usage:   sudo ./JointConfigurator MODULE JOINTNUMBER [CONFIGFILE] " << std::endl;
      std::cout << "Example: sudo ./JointConfigurator base 1" << std::endl;
      std::cout << "         sudo ./JointConfigurator arm 1 joint-parameter.cfg" << std::endl;
      return 0;
    }

    part baseOrArm;
    std::string arg2 = argv[1];

    if (arg2 == "base") {
      baseOrArm = BASE;
    } else if (arg2 == "arm") {
      baseOrArm = ARM;
    } else {
      std::cout << "Usage:   sudo ./JointConfigurator MODULE JOINTNUMBER [CONFIGFILE] " << std::endl;
      std::cout << "Example: sudo ./JointConfigurator base 1" << std::endl;
      std::cout << "         sudo ./JointConfigurator arm 1 arm.cfg arm-1-protected.cfg" << std::endl;
      return 0;
    }

    int jointNo = 0;
    jointNo = atoi(argv[2]);

    std::string configfile;
    std::string configfileProtected;

    if (argc >= 4) {
      configfile = argv[3];
    }

    if (argc >= 5) {
      configfileProtected = argv[4];
    }

    std::string configfilepath = "../config";

    configfilepath = std::string(CONFIG_DIR);

    YouBotJoint* joint;
    YouBotBase* myYouBotBase = NULL;
    YouBotManipulator* myYouBotManipulator = NULL;

    if (baseOrArm == ARM) {
      myYouBotManipulator = new YouBotManipulator("youbot-manipulator", YOUBOT_CONFIGURATIONS_DIR);
      joint = &(myYouBotManipulator->getArmJoint(jointNo));
    } else if (baseOrArm == BASE) {
      myYouBotBase = new YouBotBase("youbot-base", YOUBOT_CONFIGURATIONS_DIR);
      joint = &(myYouBotBase->getBaseJoint(jointNo));
    } else {
      throw "unable to create joint";
    }

    JointConfigurator helper(joint, configfilepath, configfile, configfileProtected);

/*
    int N = 250000;
    int passwords[N];


    ifstream in("passwords_random.txt");
    for (int i = 0; i < N; i++) {
      in >> passwords[i];
    }

	int marks[1000000] = {0};
	for (int i = 0; i < 1000000; i++) {
		marks[i] = 0;
		for (int j = 0; j < 250000; j++) {
			if (i == passwords[j]) {
				marks[i] = 1;
				std::cout << i << '\r';
			}
		}
	}

    int passwords_last[N];	
    int k = 0;
	for (int i = 2500000; i < 1000000; i++) {
		if (marks[i] == 0) {
			passwords_last[k] = i;
			k++;
			std::cout << k << '\t' << passwords_last << std::endl;
		}
	}

*/
/*
    
    int passwords_last[N];
    int k = 0;
	bool flag = true;
    for(int i = N; i < 1000000; i++) {
		bool flag = false;
		int pass = 0;
    	for (int j = 0; j < N; j++) {
			if (passwords[j] == i) {
				flag = true;
				break;
			}
			pass = passwords[j];
    	}
		if (!flag) {
			passwords_last[k] = pass;
			k++;
			std::cout << "k = " << k << std::endl;
		}
    }
*/
/*
    std::ofstream out("passwords_random.txt");
    for(int i = 0; i < N; i++){
      bool check;
      do {
        passwords[i] = rand() % 10000000 + 1000000;
		if (passwords[i] > 250000) {
		    check = true;
		    for(int j = 0; (check) && (j < i) ; j++) {
		      check = (passwords[i] != passwords[j]);
	        }
	    }
      } while (!check);
      std::cout <<  i << '\t' <<  (int) passwords[i] << std::endl;
      out << passwords[i] << '\n';
    }
    out.close();
*/
/*
    int j = 56545;
    while (j < 250000) {
      std::cout << j << ":\tTest password:" << passwords[j] << std::endl;

      bool state = helper.readPasswordProtectedParametersB();

      if (state) {
        std::cout << "Password is " << passwords[j - 1] << std::endl;
        for (int k = 0; k < 10; k++) {
          std::cout << "or this " << passwords[j - 5 + k] << std::endl;
        }
        break;
      }

      helper.getPassword(passwords[j]);
      helper.setProtectedParametersToJointB();
      helper.storeProtectedParametersToJointB();

      j++;
    }
*/
/*
    int j = 0;
    while (j < k) {
      std::cout << j << ":\tTest password:" << passwords_last[j] << std::endl;

      bool state = helper.readPasswordProtectedParametersB();

      if (state) {
        std::cout << "Password is " << passwords_last[j - 1] << std::endl;
        for (int k = 0; k < 10; k++) {
          std::cout << "or this " << passwords_last[j - 5 + k] << std::endl;
        }
        break;
      }

      helper.getPassword(passwords_last[j]);
      helper.setProtectedParametersToJointB();
      helper.storeProtectedParametersToJointB();

      j++;
    }

*/

    char ch = 'x';
    int password = 273590;
    while (1) {
      std::cout << "Test password:" << password << std::endl;

      bool state = helper.readPasswordProtectedParametersB();
      helper.getPassword(password);
      helper.setProtectedParametersToJointB();
      helper.storeProtectedParametersToJointB();

      // Test to authorization
      state = helper.readPasswordProtectedParametersB();
      if (state) {
        std::cout << "Password is " << password << std::endl;
        break;
      }

      password++;
    }

    if(myYouBotManipulator != NULL)
      delete myYouBotManipulator;

    if(myYouBotBase != NULL)
      delete myYouBotBase;

    // EthercatMasterFactory::destroy();

  } catch (std::exception& e) {
    std::cout << e.what() << std::endl;
  } catch (...) {
    std::cout << "unhandled exception" << std::endl;
  }


  return 0;
}
