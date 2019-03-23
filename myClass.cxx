#include <stdlib.h>
#include <iostream>
using namespace std;

/////////////////////////////////////////
//// A simple class
//////////////////////////////////////////
class myClass {
	private:
		int nData;
		int *data;

		myClass() {};
	public:
		myClass(int _nData) {
			nData = _nData;
			data = new int[nData];

#pragma acc enter data create(this)
			// The following pragma copies the all the data in the class to the device
#pragma acc update device(this)
			// Alternatively, the following pragma copies just nData
			//#pragma acc update device(nData)
#pragma acc enter data create(data[0:nData])
		}

		~myClass() {
			delete [] data;
#pragma acc exit data delete(data[0:nData])
#pragma acc exit data delete(this)
		}
};

/////////////////////////////////////////
// Test the class for trivial copyability
////////////////////////////////////////
int main(int argc, char *argv[])
{
	// Verify that the class myClass will work with OpenACC.
	// for more discussion about why the C++ classes MUST be trivially
	// copyable, see
	// http://www.drdobbs.com/parallel/cuda-unifying-hostdevice-interactions-wi/240161436
	if( __has_trivial_copy(myClass) == true) {
		cout <<
			"Congrat! myClass will work with OpenACC copy operations " 
			<<  endl;
	} else {
		cout <<
			"OOPS! myClass will NOT work with OpenACC copy operations " 
			<<  endl;
		return 1;
	}

	// step1: create the mirrored class
	myClass foo(10);

	return 0;
}
