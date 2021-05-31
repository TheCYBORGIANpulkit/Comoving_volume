// sample.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include<iostream>
#include<cmath>
#include<cstdlib>
#include<fstream>
#include <gsl/gsl_integration.h>
#include <gsl/gsl_sf_gamma.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_interp.h>
#define rho_c 2.8e11 // h^2 M_sun Mpc^-3
using namespace std;


double* mbin, * log10mbin, * log10phi;
double* log10rho;
int nm;

struct my_f_params
{
    double a;
    double b;
    double c;
};


double model(double x, void* p)
{
    struct my_f_params* params = (struct my_f_params*)p;
    double iphi = (params->a);
    double ims = (params->b);
    double ialpha = (params->c);
    double w;
    //w = 0.4 * log(10) * iphi * pow(pow(10,0.4*(ims-x)),ialpha+1) * exp(-pow(10,0.4*(ims-x)));
    w = (5.65 - 0.21 * x) * 0.4 * log(10) * iphi * pow(pow(10, 0.4 * (ims - x)), ialpha + 1) * exp(-pow(10, 0.4 * (ims - x)));
    return(w);
}



int linesinfile(char* infname) {
    int nlines;
    char* c, buf[1000];

    printf("Opening %s \n", infname);

    FILE* infile;

    if ((infile = fopen(infname, "r")) == NULL) {
        printf("File %s does not exist ... aborting \n", infname);
        perror("FILE DOES NOT EXIST ... ABORTING \n");
        fflush(stderr);
        exit(EXIT_FAILURE);
    }//if

    nlines = 0;
    while ((c = fgets(buf, 1000, infile)) != NULL)
        nlines++;
    rewind(infile);
    fclose(infile);

    printf("nlines=%d in %s \n", nlines, infname);

    return(nlines);

}//linesinfile




int main(int argc, char** argv)
{
    if (argc != 5) {
        printf("\nCommand line args\n");
        printf("./a.out [phistar] [mstar] [alfa]  [h]\n");
        printf("./a.out  0.0048     9.96  -1.33   0.7\n");
        return(-1);
    }//if

    double phistar, mstar, alpha;
    double up_bound, low_bound, h;
    double vol;
    int i;
    double p, E, d1, d2, d3;;

    phistar = atof(argv[1]);
    mstar = atof(argv[2]);
    alpha = atof(argv[3]);
    h = atof(argv[4]);

    ///////////////////////////////////////////////////////////////

    double result, error;
    gsl_integration_workspace* w1 = gsl_integration_workspace_alloc(1000);
    gsl_function F1;
    struct my_f_params  params = { phistar, mstar, alpha };
    F1.function = &model;
    F1.params = &params;
    //gsl_integration_qagil ( &F1 , -17.37 , 0 , 1e-12 , 1000 , w1 , &result , &error );
    gsl_integration_qagil(&F1, 0, 0, 1e-12, 1000, w1, &result, &error);
    //gsl_integration_qagi ( &F1 , 0 , 1e-12 , 1000 , w1 , &result , &error );
    //gsl_integration_qag ( &F1 , -25, 0, 0 , 1e-12 , 1000 , 1, w1 , &result , &error );
    gsl_integration_workspace_free(w1);

    printf("result=%g \n", result);
    printf("rho_HI=%g \n", result * pow(10, 9.76305));
    printf("omega_HI=%g \n", result * 4.93e09 / rho_c / 1e-4 / h / h);
    printf("omega_HI=%g \n", result * pow(10, 9.76305) / rho_c / 1e-4 / h / h);
    return(0);
}
binned_omega_HI.C
Displaying binned_omega_HI.C.