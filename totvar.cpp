#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>
#include <cstdlib>

#define SAMPLES     200

template<class T>
double mean(std::vector<T> & x){
  double m=0;
  for(auto xi : x) m+=xi;
    m /= x.size();
  return m;
}

template<class T>
double var(std::vector<T> & x){
  double v=0, m=mean(x);
  for(auto xi : x) v += (xi-m)*(xi-m);
    v /= x.size();
  return v;
}



int main(){
  std::vector<double> x, means, vars, fracs;
  std::vector<std::vector<double> > partitions;

  std::vector<int> index = {10,24,76,109,145,180,198};

// Global mean and var
  for(int i=0; i<SAMPLES; i++){
  	x.push_back( (double) rand()/RAND_MAX );
  }
  std::cout << "Ave  global : " << mean(x) << std::endl;
  std::cout << "Var  global : " << var(x) << std::endl;
  std::cout << "Size global : " << x.size() << std::endl;

// Partitioned mean and va 
  partitions.push_back( std::vector<double>(x.begin(),x.begin()+index[0]) );
  for(int i=0; i<index.size()-1; i++){
    partitions.push_back( std::vector<double>(x.begin()+index[i], x.begin()+index[i+1]) );
  }
  partitions.push_back( std::vector<double>(x.begin()+index.back(), x.end()) );

  std::cout << std::endl << "Partitions number : " << partitions.size() 
  << "\n #   size      mean       var    frac" << std::endl;
  for( auto xn : partitions ){
    means.push_back( mean(xn) );
    vars.push_back( var(xn) );
    fracs.push_back( xn.size()/((double) x.size()) );
  }

  for(size_t i=0; i<partitions.size(); i++){
    std::cout << std::setw(2) << i << "    " << std::setw(3) << partitions[i].size() << "  " 
              << std::fixed << std::setprecision(6) << means[i] << "  " 
              << std::fixed << std::setprecision(6) << vars[i] << "   " 
              << std::fixed << std::setprecision(3) << fracs[i] << std::endl;
  }

// Law of total variance
  double tot_var=0;
  for(size_t i=0; i<partitions.size(); i++){
    tot_var += vars[i]*fracs[i] + means[i]*means[i]*(1-fracs[i])*fracs[i];
    for(size_t j=0; j<=i-1 && i>0; j++){
      tot_var -= 2*means[i]*fracs[i]*means[j]*fracs[j];
    }
  }
  std::cout.unsetf(std::ios_base::fixed);
  std::cout << std::endl << "Compare " << var(x) << " vs " << tot_var << " diff " << fabs(var(x)-tot_var) << std::endl;


  return 0;
}