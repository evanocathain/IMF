#include <stdio.h>
#include <stdlib.h>
#include <math.h> 

int strings_equal(char *string1, char *string2);

int main (int argc, char* argv[])
{
  double k1, k2, k3, k4;                           // constants
  double m_max, m1, m2, m3, m4;                    // transition pts
  double alpha1, alpha2, alpha3, alpha4;           // slopes
  double mean=0.0, normal=0.0, frac_ccsn=0.0, 
    frac_ns=0.0, frac_bh=0.0, rate=0.0;
  double sfr=4.0;                                  // in Msun/year
  /* This is Stahler & Palla value and that of Mckee & Williams
   * Stahler & Palla suggest a 50% error but Mckee & Williams don't
   * suggest an error estimate.*/

  /* The IMF is a 4-piece power-law of the form
   * Phi(m)=k_i*m^{alpha_i} where the k's and alpha's are given by
   * (Kroupa 2001, MNRAS, 322, 231) ...*/

  m_max=120.0;         // no stars heavier than m_max
  m1=1.0;              // above 1M_sun the true slope is Scalo imf
  m2=0.5;              // between 0.5 and 1M_sun the slope is Salpeter imf
  m3=0.08;
  m4=0.01;

  alpha1=-2.3;//\pm0.3
  alpha2=-2.3;//\pm0.3
  alpha3=-1.3;//\pm0.5
  alpha4=-0.3;//\pm0.7

  if (argc>1){
    if (strings_equal(argv[1],"-imf4")) {
      alpha1=-2.7;
    }
    if (strings_equal(argv[1],"-salpeter")) {
      alpha1=alpha2=alpha3=alpha4=-2.35;
    }
  }

  // from continuity we can get the k values wrt k1
  k1=1;
  k2=(pow(m1,(alpha1-alpha2)))*k1;
  k3=(pow(m2,(alpha2-alpha3)))*k2;
  k4=(pow(m3,(alpha3-alpha4)))*k3; 
  printf("From continuity constants are: k2 = %.2lf*k1, k3 = %.2lf*k1, k4 = %.2lf*k1\n", k2, k3, k4);

  // integrate over all mass -> get value*k1
  normal=((k4/(alpha4+1))*(pow(m3,(alpha4+1))-pow(m4,(alpha4+1))))+((k3/(alpha3+1))*(pow(m2,(alpha3+1))-pow(m3,(alpha3+1))))+((k2/(alpha2+1))*(pow(m1,(alpha2+1))-pow(m2,(alpha2+1))))+((k1/(alpha1+1))*(pow(m_max,(alpha1+1))-pow(m1,(alpha1+1))));

  printf("Integrating over the whole mass range gives %lf*k1 which \nshould equal 1 for normalised distribution so we know k1...\n", normal);

  k1=1/normal;
  printf("The normalisation k1 is %lf\n", k1);
  // now normalise the constants
  k2=(pow(m1,(alpha1-alpha2)))*k1;
  k3=(pow(m2,(alpha2-alpha3)))*k2;
  k4=(pow(m3,(alpha3-alpha4)))*k3;
  printf("Other constants are k2 = %lf, k3 = %lf, k4 = %lf\n", k2, k3, k4);

  mean=((k4/(alpha4+2))*(pow(m3,(alpha4+2))-pow(m4,(alpha4+2))))+((k3/(alpha3+2))*(pow(m2,(alpha3+2))-pow(m3,(alpha3+2))))+((k2/(alpha2+2))*(pow(m1,(alpha2+2))-pow(m2,(alpha2+2))))+((k1/(alpha1+2))*(pow(m_max,(alpha1+2))-pow(m1,(alpha1+2))));
  
  printf("mean is %lf\n", mean);

  frac_ccsn=((k1/(alpha1+1))*(pow(120.0,(alpha1+1))-pow(11.0,(alpha1+1)))) ;

  printf("The fraction in Type II range in total is %lf\n", frac_ccsn);

  // rate = (frac in range/<m>)*SFR
  rate=(frac_ccsn/mean)*(sfr*100);       // core-collapse SNe per century

  printf("So the Type II SN rate is %1.2lf plus or minus %1.2lf SN per century\n", rate, (rate*0.5)); 

  frac_ns=((k1/(alpha1+1))*(pow(25,(alpha1+1))-pow(11,(alpha1+1)))) ;
  frac_bh=frac_ccsn-frac_ns;

  printf("The fraction in Type II range which will be NSs is %lf\n", frac_ns);
  rate=(frac_ns/mean)*(sfr*100);       // NSs born per century
  printf("So the Type II SN rate for just NSs is %1.2lf plus or minus %1.2lf SN per century\n", rate, (rate*0.5));    // used 50% error

  printf("The fraction in Type II range which will be BHs is %lf\n", frac_bh);
  rate=(frac_bh/mean)*(sfr*100);       // BHs born per century
  printf("So the Type II SN rate for just BHs is %1.2lf plus or minus %1.2lf SN per century\n", rate, (rate*0.5));    // used 50% error
    
  return(0);
}

int strings_equal(char *string1, char *string2)
{
  if (!strcmp(string1,string2)){
    return 1;
  } else {
    return 0;
  }
}
