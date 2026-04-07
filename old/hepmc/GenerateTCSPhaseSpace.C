#include <TFile.h>
#include <TTree.h>
#include <TRandom3.h>
#include <cmath>
#include <iostream>

const double mp = 0.9382720813;

// Kaellen
double lambdaK(double a, double b, double c)
{
    return a*a + b*b + c*c - 2*(a*b + a*c + b*c);
}

// --------------------------------------------------------------------
// t from costh in 2?2 kinematics
// --------------------------------------------------------------------
double t_from_costh(double s, double Q2, double costh)
{
    double Ecm = sqrt(s);

    double p  = (s - mp*mp) / (2*Ecm);
    double pf = sqrt(lambdaK(s, Q2, mp*mp)) / (2*Ecm);

    double Ep  = (s + mp*mp)/(2*Ecm);
    double Epf = (s + mp*mp - Q2)/(2*Ecm);

    return mp*mp + mp*mp - 2*(Ep*Epf - p*pf*costh);
}

void GenerateTCSPhaseSpace(int N = 20000,
			   double Ee = 18.0,
			   double Ep = 275.0)
{
    TRandom3 rng(0);

    double smin = (mp + 2*0.000511)*(mp + 2*0.000511);
    double smax = 4.0 * Ee * Ep;

    double Q2min = 2.0;
    double Q2max = 20.0;

    // OUTPUT
    TFile *f = new TFile("TCSPhaseSpace.root","RECREATE");
    TTree *T = new TTree("tcs","Correct TCS Phase Space");

    double s, Q2, t, thetaL, phiL;
    T->Branch("s",&s,"s/D");
    T->Branch("Q2",&Q2,"Q2/D");
    T->Branch("t",&t,"t/D");
    T->Branch("thetaL",&thetaL,"thetaL/D");
    T->Branch("phiL",&phiL,"phiL/D");
    
    int acc = 0;

    while (acc < N)
    {
        // 1) Generate s with correct marginal PDF
        double s_trial;
        do {
            s_trial = rng.Uniform(smin, smax);
            double w = sqrt(lambdaK(s_trial, mp*mp, 0)) / s_trial;
            if (rng.Uniform(0,1) < w) break;
        } while (true);
        s = s_trial;

        // Q2 max depends on s
        double Q2max_phys = (sqrt(s)-mp)*(sqrt(s)-mp);
        if (Q2max_phys < Q2min) continue;

        double Q2_upper = std::min(Q2max, Q2max_phys);

        // 2) Generate Q2 with correct conditional PDF
        double Q2_trial;
        double wmax_Q = sqrt(lambdaK(s, Q2min, mp*mp));

        do {
            Q2_trial = rng.Uniform(Q2min, Q2_upper);
            double wQ = sqrt(lambdaK(s, Q2_trial, mp*mp));
            if (rng.Uniform(0, wmax_Q) < wQ) break;
        } while (true);

        Q2 = Q2_trial;

        // 3) Generate t via uniform cos?*
        double costh = rng.Uniform(-1,1);
        t = t_from_costh(s, Q2, costh);

        // 4) decay angles uniform
        double c = rng.Uniform(-1,1);
        thetaL = acos(c);
        phiL   = rng.Uniform(0,2*M_PI);

        T->Fill();
        acc++;
    }

    f->Write();
    f->Close();

    std::cout<<"Generated "<<acc<<" events.\n";
}
