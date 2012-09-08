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
#ifndef CMatrixTemplateNumeric_H
#define CMatrixTemplateNumeric_H

#include <mrpt/math/CMatrixTemplate.h>
#include <mrpt/utils/CSerializable.h>

#include <mrpt/system/os.h>
#include <cmath>
#include <limits>

namespace mrpt
{
	namespace poses
	{
		class CPose2D;
		class CPose3D;
		class CPoint2D;
		class CPoint3D;
	}

	namespace math
	{
		using namespace mrpt::system;

		/**  A matrix of dynamic size.
		  *   Basically, this class is a wrapper on Eigen::Matrix<T,Dynamic,Dynamic>, but
		  *   with a RowMajor element memory layout (except for column vectors).
		  *
		  * \note This class exists for backward compatibility of ancient times when MRPT didn't rely on Eigen, feel free to directly use Eigen::Matrix<> types instead.
		  *
		  * \sa CMatrixTemplate (a non Eigen lib-based  class, which can hold arbitrary objects, not only numerical types).
		 * \note For a complete introduction to Matrices and vectors in MRPT, see: http://www.mrpt.org/Matrices_vectors_arrays_and_Linear_Algebra_MRPT_and_Eigen_classes
		 * \ingroup mrpt_base_grp
		  */
		template <class T>
		class CMatrixTemplateNumeric : 
			public Eigen::Matrix<
				T,
				Eigen::Dynamic,
				Eigen::Dynamic,
				// Use row major storage for backward compatibility with MRPT matrices in all cases (even in column vectors!)
				Eigen::AutoAlign | Eigen::RowMajor
				>
		{
		public:
			typedef Eigen::Matrix<T,Eigen::Dynamic,Eigen::Dynamic,Eigen::AutoAlign|Eigen::RowMajor> Base;
			typedef CMatrixTemplateNumeric<T> mrpt_autotype;

			MRPT_MATRIX_CONSTRUCTORS_FROM_POSES(CMatrixTemplateNumeric)

			/** Default constructor, builds a 1x1 matrix */
			inline CMatrixTemplateNumeric() : Base(1,1) { Base::setZero(); }

			/** Constructor that builds a 0x0 matrix (that is, uninitialized), for usage in places where efficiency is a priority.
			  *  Use as:
			  *   \code
			  *     CMatrixTemplateNumeric<double>  M( UNINITIALIZED_MATRIX);
			  *   \endcode
			  */
			inline CMatrixTemplateNumeric(TConstructorFlags_Matrices constructor_flag) : Base( 0,0 ) { }

			/** Constructor, creates a matrix of the given size, filled with zeros. */
			inline CMatrixTemplateNumeric(size_t row, size_t col) : Base(row,col) { Base::setZero(); }

			MRPT_EIGEN_DERIVED_CLASS_CTOR_OPERATOR_EQUAL(CMatrixTemplateNumeric) // Implements ctor and "operator =" for any other Eigen class

			/** Assignment operator of other types
			*/
			template <class R>
			inline CMatrixTemplateNumeric<T>& operator = (const CMatrixTemplate<R>& m)
			{
				Base::resize( m.getRowCount(), m.getColCount() );

				for (size_t i=0; i < CMatrixTemplate<T>::getRowCount(); i++)
					for (size_t j=0; j < CMatrixTemplate<T>::getColCount(); j++)
						Base::coeffRef(i,j) = static_cast<T>(m.get_unsafe(i,j));
				return *this;
			}

			/** Constructor from a given size and a C array. The array length must match cols x row.
			  * \code
			  *  const double numbers[] = {
			  *    1,2,3,
			  *    4,5,6 };
			  *	 CMatrixDouble   M(3,2, numbers);
			  * \endcode
			  */
			template <typename V, size_t N>
			inline CMatrixTemplateNumeric(size_t row, size_t col, V (&theArray)[N] ) : Base(row,col)
			{
				ASSERT_EQUAL_(row*col,N)
				ASSERT_EQUAL_(sizeof(theArray[0]),sizeof(T))
				::memcpy(Base::data(),&theArray[0],sizeof(T)*N); // Remember, row-major order!
			}

			/** Destructor
			  */
			inline ~CMatrixTemplateNumeric() { }

			/** == comparison of two matrices; it differs from default Eigen operator in that returns false if matrices are of different sizes instead of raising an assert. */
			template <typename Derived>
			inline bool operator ==(const Eigen::MatrixBase<Derived>& m2) const
			{
				return Base::cols()==m2.cols() && 
					   Base::rows()==m2.rows() && 
					   Base::cwiseEqual(m2).all();
			}
			/** != comparison of two matrices; it differs from default Eigen operator in that returns true if matrices are of different sizes instead of raising an assert. */
			template <typename Derived>
			inline bool operator !=(const Eigen::MatrixBase<Derived>& m2)  const{ return !((*this)==m2); }

		}; // end of class definition


		/** Declares a matrix of float numbers (non serializable).
		  *  For a serializable version, use math::CMatrix
		  *  \sa CMatrixDouble, CMatrix, CMatrixD
		  */
		typedef CMatrixTemplateNumeric<float> CMatrixFloat;

		/** Declares a matrix of double numbers (non serializable).
		  *  For a serializable version, use math::CMatrixD
		  *  \sa CMatrixFloat, CMatrix, CMatrixD
		  */
		typedef CMatrixTemplateNumeric<double> CMatrixDouble;

		/** Declares a matrix of unsigned ints (non serializable).
		  *  \sa CMatrixDouble, CMatrixFloat
		  */
		typedef CMatrixTemplateNumeric<unsigned int> CMatrixUInt;

		/** Declares a matrix of booleans (non serializable).
		  *  \sa CMatrixDouble, CMatrixFloat, CMatrixB
		  */
		typedef CMatrixTemplate<bool> CMatrixBool;

#ifdef HAVE_LONG_DOUBLE
		/** Declares a matrix of "long doubles" (non serializable), or of "doubles" if the compiler does not support "long double".
		  *  \sa CMatrixDouble, CMatrixFloat
		  */
		typedef CMatrixTemplateNumeric<long double> CMatrixLongDouble;
#else
		/** Declares a matrix of "long doubles" (non serializable), or of "doubles" if the compiler does not support "long double".
		  *  \sa CMatrixDouble, CMatrixFloat
		  */
		typedef CMatrixTemplateNumeric<double> CMatrixLongDouble;
#endif


		namespace detail	
		{
			/**
			  * Vicinity traits class specialization for fixed size matrices.
			  */
			template<typename T> class VicinityTraits<CMatrixTemplateNumeric<T> >	{
			public:
				inline static void initialize(CMatrixTemplateNumeric<T> &mat,size_t N)	{
					mat.setSize(N,N);
					mat.fill(0);
				}
				inline static void insertInContainer(CMatrixTemplateNumeric<T> &mat,size_t r,size_t c,const T &t)	{
					mat.get_unsafe(r,c)=t;
				}
			};
		}	//End of detail namespace.

	} // End of namespace


	namespace utils
	{
		// Extensions to mrpt::utils::TTypeName for matrices:
		template<typename T> struct TTypeName <mrpt::math::CMatrixTemplateNumeric<T> > {
			static std::string get() { return std::string("CMatrixTemplateNumeric<")+ std::string( TTypeName<T>::get() )+std::string(">"); }
		};
	}

} // End of namespace


#endif
