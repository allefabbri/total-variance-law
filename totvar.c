// Copyright 2015, Alessandro Fabbri
// for any question, please mail rommacc@yahoo.it

/************************************************************************
* This program is free software: you can redistribute it and/or modify  *
* it under the terms of the GNU General Public License as published by  *
* the Free Software Foundation, either version 3 of the License, or     *
* (at your option) any later version.                                   *
*                                                                       *
* This program is distributed in the hope that it will be useful,       *
* but WITHOUT ANY WARRANTY; without even the implied warranty of        *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
* GNU General Public License for more details.                          *
*                                                                       *
* You should have received a copy of the GNU General Public License     *
* along with this program.  If not, see <http://www.gnu.org/licenses/>. *
************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define SAMPLES     200

double mean(double * x, int size){
  double m=0;
  int i;
  for(i=0; i<size; i++) m+=x[i];
  m /= size;
  return m;
}

double var(double * x, int size){
  double v=0, m=mean(x, size);
  int i;
  for(i=0; i<size; i++) v+=(x[i]-m)*(x[i]-m);
  v /= size;
  return v;
}



int main(){
  int i,j;
  int indices[] = {10,24,76,109,145,180,198};

  double *x, *means, *vars, *fracs;
  x = (double *) malloc(SAMPLES*sizeof(double));

// Global mean and var
  for(i=0; i<SAMPLES; i++){
    x[i] = (double) rand()/RAND_MAX;
  }
  printf("Ave  global : %f\nVar  global : %f\nSize global : %d\n", mean(x,SAMPLES), var(x,SAMPLES), SAMPLES);

// Partitioned mean and var
  int partitions_n = sizeof(indices)/sizeof(*indices)+1;
  int *sizes, *firsts, *lasts;
  sizes  = (int *) malloc(partitions_n*sizeof(int));
  firsts = (int *) malloc(partitions_n*sizeof(int));
  lasts  = (int *) malloc(partitions_n*sizeof(int));
  double ** partitions;
  partitions = (double **) malloc(partitions_n*sizeof(double *));
  means = (double *) malloc(partitions_n*sizeof(double));
  vars  = (double *) malloc(partitions_n*sizeof(double));
  fracs = (double *) malloc(partitions_n*sizeof(double));


  sizes[0]  = indices[0];
  firsts[0] = 0;
  lasts[0]  = indices[0]-1;
  for(i=1; i<partitions_n-1; i++){
    sizes[i]  = indices[i]-indices[i-1];
    firsts[i] = indices[i-1];
    lasts[i]  = indices[i]-1;
  }
  sizes[partitions_n-1]  = SAMPLES - indices[partitions_n-2];
  firsts[partitions_n-1] = indices[partitions_n-2];
  lasts[partitions_n-1]  = SAMPLES-1;

  for(i=0; i<partitions_n; i++){
    partitions[i] = (double *) malloc(sizes[i]*sizeof(double));
    for(j=0; j<sizes[i]; j++){
//      printf("%d\n",j+((i==0)?0:indices[i-1]));
      partitions[i][j] = x[j+((i==0)?0:indices[i-1])];
    }
    means[i] = mean(partitions[i], sizes[i]);
    vars[i]  = var(partitions[i], sizes[i]);
    fracs[i] = (double) sizes[i]/SAMPLES;
  }
  
  printf("\n\nPartitions number : %d\n",partitions_n);
  for(i=0; i<partitions_n; i++){
    printf("%2d    %2d   %3d - %3d   %7.6f   %f    %4.3f\n",
      i,sizes[i],firsts[i],lasts[i],means[i],vars[i],fracs[i]);
  }

// Law of total variance
  double tot_var=0;
  for(i=0; i<partitions_n; i++){
    tot_var += vars[i]*fracs[i] + means[i]*means[i]*(1-fracs[i])*fracs[i];
    for(j=0; j<=i-1 && i>0; j++){
      tot_var -= 2*means[i]*fracs[i]*means[j]*fracs[j];
    }
  }
  printf("\nCompare %f vs %f diff %f\n", var(x,SAMPLES), tot_var, fabs(var(x,SAMPLES)-tot_var));

// Printing debug info
  FILE * debug = fopen("pd.txt","w");
  fprintf(debug, "full buffer\n");
  for(i=0; i<SAMPLES; i++) 
    fprintf(debug, "%3.3f%c", x[i], ((i+1)%10)?' ':'\n');
  fprintf(debug, "\n");
  for(i=0; i<partitions_n; i++){
    fprintf(debug, "\npartitions # %d size %d\n", i+1, sizes[i]);
    for(j=0; j<sizes[i]; j++){
      fprintf(debug, "%3.3f ",partitions[i][j]);
    }
    fprintf(debug, "\n");
  }
  fclose( debug );

  return 0;
}


