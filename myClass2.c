#include <stdlib.h>
#include <iostream>
using namespace std;
 
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
		          //#pragma acc update device(nData)
			  #pragma acc update device(this)
			      // note that the pointer is to host memory, so we overwrite with a
			          // pointer to memory allocated on the device.
			          #pragma acc enter data create(data[0:nData])
			            }
			             
			               ~myClass() {
			                   delete [] data;
			                   #pragma acc exit data delete(data[0:nData])
			                   #pragma acc exit data delete(this)
			                     }
			                      
			                        void set_series(int start) {
			                            for(int i=0; i < nData; i++)
			                                  data[i] = i + start;
			                                  #pragma acc update device(data[0:nData])
			                                    }
			                                     
			                                       void acc_mult(int factor) {
			                                           // We assign nData to cnt so the compiler can determine
			                                               // that it is free to parallelize the loop.
			                                                   int cnt= nData;
			                                                   #pragma omp parallel for
			                                                   #pragma acc parallel loop
			                                                       for(int i=0; i < cnt; ++i) {
			                                                             data[i] *= factor;
			                                                                 }
			                                                                 #pragma acc update host(data[0:nData])
			                                                                   }
			                                                                     
			                                                                       const int * get_data() {return data; }
			                                                                         int get_nData() {return nData; }
			                                                                         };
			                                                                          
			                                                                          int main(int argc, char *argv[])
			                                                                          {
			                                                                            if(argc != 4) {
			                                                                                cerr << "Use: nData start factor" << endl;
			                                                                                    return 1;
			                                                                                      }
			                                                                                        int nData=atoi(argv[1]);
			                                                                                          if(nData <=0) {
			                                                                                              cerr << "nData must be greater than zero!" << endl;
			                                                                                                  return 1;
			                                                                                                    }
			                                                                                                     
			                                                                                                       int start=atoi(argv[2]);
			                                                                                                         int factor=atoi(argv[3]);
			                                                                                                          
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
			                                                                                                                              	             	            myClass foo(nData);
			                                                                                                                              	             	             
			                                                                                                                              	             	               // step 2: set the series values beginning at start
			                                                                                                                              	             	                 foo.set_series(start);
			                                                                                                                              	             	                  
			                                                                                                                              	             	                    // step 3: multiply on the device by some factor 
			                                                                                                                              	             	                      foo.acc_mult(factor);
			                                                                                                                              	             	                        
			                                                                                                                              	             	                          // step 4: print out the scaled results
			                                                                                                                              	             	                            for(int i=0; i < foo.get_nData(); i++)
			                                                                                                                              	             	                                if( foo.get_data()[i] != ((i+start)*factor) ) {
			                                                                                                                              	             	                                      cerr << "error at position " << i << " " << foo.get_data()[i] << endl;
			                                                                                                                              	             	                                            return 1;
			                                                                                                                              	             	                                                }
			                                                                                                                              	             	                                                  
			                                                                                                                              	             	                                                    return 0;
			                                                                                                                              	             	                                                    }
