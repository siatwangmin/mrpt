/* +---------------------------------------------------------------------------+
   |                 The Mobile Robot Programming Toolkit (MRPT)               |
   |                                                                           |
   |                          http://www.mrpt.org/                             |
   |                                                                           |
   | Copyright (c) 2005-2012, Individual contributors, see AUTHORS file        |
   | Copyright (c) 2005-2012, MAPIR group, University of Malaga                |
   | Copyright (c) 2012, University of Almeria                                 |
   | All rights reserved.                                                      |
   |                                                                           |
   | Redistribution and use in source and binary forms, with or without        |
   | modification, are permitted provided that the following conditions are    |
   | met:                                                                      |
   |    * Redistributions of source code must retain the above copyright       |
   |      notice, this list of conditions and the following disclaimer.        |
   |    * Redistributions in binary form must reproduce the above copyright    |
   |      notice, this list of conditions and the following disclaimer in the  |
   |      documentation and/or other materials provided with the distribution. |
   |    * Neither the name of the copyright holders nor the                    |
   |      names of its contributors may be used to endorse or promote products |
   |      derived from this software without specific prior written permission.|
   |                                                                           |
   | THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS       |
   | 'AS IS' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED |
   | TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR|
   | PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS BE LIABLE |
   | FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL|
   | DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR|
   |  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)       |
   | HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,       |
   | STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN  |
   | ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE           |
   | POSSIBILITY OF SUCH DAMAGE.                                               |
   +---------------------------------------------------------------------------+ */

#include <mrpt/base.h>

using namespace mrpt::utils;
using namespace mrpt::poses;
using namespace std;

int main()
{
	try
	{
		// The landmark (global) position: 3D (x,y,z)
		CPoint3D L(0,4,2);

		// Robot pose: 2D (x,y,phi)
		CPose2D  R(2,1, DEG2RAD(45.0f) );  

		// Camera pose relative to the robot: 6D (x,y,z,yaw,pitch,roll).
		CPose3D  C(0.5f,0.5f,1.5f ,DEG2RAD(-90.0f),DEG2RAD(0),DEG2RAD(-90.0f)  );  

		// TEST 1. Relative position L' of the landmark wrt the camera
		// --------------------------------------------------------------
		cout << "L: " << L << endl;
		cout << "R: " << R << endl;
		cout << "C: " << C << endl;
		cout << "R+C:" << (R+C) << endl;
		//cout << (R+C).getHomogeneousMatrix();

		CPoint3D L2;
		CTicTac tictac;
		tictac.Tic();
		size_t i,N = 10000;
		for (i=0;i<N;i++)
			L2 = L - (R+C);
		cout << "Computation in: " << 1e6 * tictac.Tac()/((double)N) << " us" << endl;

		cout << "L': " << L2 << endl;

		// TEST 2. Reconstruct the landmark position:
		// --------------------------------------------------------------
		CPoint3D L3 = R + C + L2;
		cout << "R(+)C(+)L' = " << L3 << endl;
		cout << "Should be equal to L = " << L << endl;

		// TEST 3. Distance from the camera to the landmark
		// --------------------------------------------------------------
		cout << "|(R(+)C)-L|= " << (R+C).distanceTo(L) << endl;
		cout << "|L-(R(+)C)|= " << (R+C).distanceTo(L) << endl;

		return 0;
	} catch (exception &e)
	{
		cerr << "EXCEPCTION: " << e.what() << endl;
		return -1;
	}
	catch (...)
	{
		cerr << "Untyped excepcion!!";
		return -1;
	}
}

