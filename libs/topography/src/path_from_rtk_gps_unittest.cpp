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


#include <mrpt/topography.h>
#include <mrpt/poses/CPose3DInterpolator.h>
#include <mrpt/system/filesystem.h>
#include <gtest/gtest.h>

using namespace mrpt;
using namespace mrpt::utils;
using namespace mrpt::math;
using namespace mrpt::poses;
using namespace mrpt::topography;
using namespace std;

// Defined in run_unittests.cpp
namespace mrpt { namespace utils {
	extern std::string MRPT_GLOBAL_UNITTEST_SRC_DIR;
  }
}


TEST(TopographyReconstructPathFrom3RTK, sampleDataset )
{
	mrpt::poses::CPose3DInterpolator robot_path;

	mrpt::slam::CRawlog  rawlog;

	const string dataset_fil = MRPT_GLOBAL_UNITTEST_SRC_DIR + string("/share/mrpt/datasets/test_rtk_path.rawlog");
	if (!mrpt::system::fileExists(dataset_fil))
	{
		cerr << "WARNING: Skipping test due to missing file: " << dataset_fil << "\n";
		return;
	}
	if (!rawlog.loadFromRawLogFile(dataset_fil))
	{
		cerr << "WARNING: Skipping test due to error loading file: " << dataset_fil << "\n";
	}
	else
	{
		mrpt::topography::TPathFromRTKInfo	rtk_path_info;

		// -------------------------------------------
		// Run path reconstruction:
		// -------------------------------------------
		mrpt::topography::path_from_rtk_gps(
			robot_path,
			rawlog,
			0, // first entry
			rawlog.size()-1, // last entry
			false, // Isn't a GUI
			false, // disableGPSInterp
			1, // path_smooth_filter_size
			&rtk_path_info );

		EXPECT_EQ(robot_path.size(),75u);

		// Expected values:
		//1226225355.000000 279.705647 216.651473 8.517821 0.194222 -0.083873 -0.045293
		//1226225380.000000 377.095830 233.343569 9.724171 0.177037 -0.073565 -0.019024
		const mrpt::system::TTimeStamp t1 = mrpt::system::time_tToTimestamp( 1226225355.000000 );
		const mrpt::system::TTimeStamp t2 = mrpt::system::time_tToTimestamp( 1226225380.000000 );
		const CPose3D pose_GT_1(279.705647,216.651473,8.517821,0.194222,-0.083873,-0.045293);
		const CPose3D pose_GT_2(377.095830,233.343569,9.724171,0.177037,-0.073565,-0.019024);

		CPose3D pose1,pose2;
		bool    valid;
		robot_path.interpolate(t1,pose1,valid);
		EXPECT_TRUE(valid);

		robot_path.interpolate(t2,pose2,valid);
		EXPECT_TRUE(valid);


		vector_double p1vec(12), p2vec(12);
		pose1.getAs12Vector(p1vec);
		pose2.getAs12Vector(p2vec);

		vector_double p1vec_gt(12), p2vec_gt(12);
		pose_GT_1.getAs12Vector(p1vec_gt);
		pose_GT_2.getAs12Vector(p2vec_gt);

		EXPECT_NEAR( (p1vec - p1vec_gt).array().abs().sum(), 0, 1e-3);
		EXPECT_NEAR( (p2vec - p2vec_gt).array().abs().sum(), 0, 1e-3);
	}
}
