#include "Helper.h"

#include <valarray>
#include <complex>
#include <vector>

#include "FourVector.h"

using namespace std;


std::vector<std::vector<int> > generatePermutations(int njets, int tags, int start) {
  std::vector<std::vector<int> > perm;
  std::vector<std::vector<int> > temp_perm;
  if(tags==0) {
    vector<bool> tags(njets, 0);
    perm.push_back(tags);
  } else {
    for(int i=start; i<njets;i++){
      temp_perm = generatePermutations(njets,tags-1,i+1);
      for(unsigned int j=0; j<temp_perm.size(); j++) {
        temp_perm.at(j).at(i) = 1;
        perm.push_back(temp_perm.at(j));
      }
    }
  }
  return perm;
}
void perm(vector<vector<int> > &s, int n = 4, int start = 0) {
  for (int k = 0; k <= 4; k++) {
    vector<vector<int> > p = generatePermutations(n, k, 0);
    for (int l = 0; l < p.size(); ++l) s.push_back(p[l]);
  }
}

struct FElement {
    std::vector<int> p;
    //std::string (*element)(vector<string> &p, vector<string> &s);
    std::string element;
    FElement();
};

int main(int argc, char *argv[]) {
  FourVector vb1;
  FourVector vb2;
  FourVector vo1;
  FourVector vo2;

  std::vector<FourVector> in;
  std::vector<FourVector> out;
  in.push_back(vb1);
  in.push_back(vb2);
  out.push_back(vo1);
  out.push_back(vo2);

  std::map<std::string, int> partid;
  std::map<int, std::string> partname;
  partid["e+"] = -11;
  partid["e-"] = +11;
  partid["a"] = 22;
  for (std::map<std::string, int>::const_iterator it = partid.begin(); it != partid.end(); ++it) {
    partname[it->second] = it->first;
  }
  std::vector<FElement> fd_impl;
  fd_impl.push_back(FElement());
  fd_impl[0].p.push_back(partid["e+"]);
  fd_impl[0].p.push_back(partid["e-"]);
  fd_impl[0].p.push_back(partid["a"]);
  fd_impl[0].element = "fdm_QEDVertex((p[0]+p[1]).M2(), -1, #mu#)"
  fd_impl.push_back(FElement());
  fd_impl[1].p.push_back(partid["a"]);
  fd_impl[1].p.push_back(partid["a"]);
  fd_impl[1].element = "fdm_QEDPropagatorA((p[0]+p[1]).M2(), #mu#, #nu#)"
  fd_impl.push_back(FElement());
  fd_impl[2].p.push_back(partid["e+"]);
  fd_impl[2].element = "fdm_Fermion(#f#, true, #s#, p[0])"
  fd_impl.push_back(FElement());
  fd_impl[3].p.push_back(partid["e-"]);
  fd_impl[3].element = "fdm_Fermion(#f#, false, #s#, p[0])"

  // for this process
  int orderQED = 2;
  std::vector<std::string> inpart;
  inpart.push_back("e+");
  inpart.push_back("e-");
  std::vector<std::string> outpart;
  outpart.push_back("e+");
  outpart.push_back("e-");

  std::vector<std::string> part;
  for (int k = 0; k < inpart.size(); ++k)  part.push_back(inpart[k]);
  for (int k = 0; k < outpart.size(); ++k) part.push_back(outpart[k]);

  ofstream o("autogen.cxx");
  o << "#include <iostream>" << endl;
  o << "#include <vector>" << endl;
  o << "#include \"Helper.h\"" << endl;
  o << "#include \"FeynmanElements.h\"" << endl;
  o << "#include \"TLorentzVector.h\"" << endl;
  o << "using namespace std;" << endl;
  o << "number m2(vector<FourVector> &in, vector<FourVector> &out) {" << endl;
  o << "  number r(0,0);" << endl;
  o << "  vector<FourVector> p;" << endl;
  vector<vector<int> > s;
  o << "  for (int k = 0; k < in.size(); ++k)  p.push_back(in[k]);" << endl;
  o << "  for (int k = 0; k < out.size(); ++k) p.push_back(out[k]);" << endl;
  int n_in = inpart.size();
  int n_out = outpart.size();
  // find diagrams and multiply them out
  std::vector<std::vector<FElements> > terms;
  int nconn = 0;
  map<int, int> conn;
  vector<string> ppart = part;
  map<pair<int, int>, FElement > out_blocks;
  for (int ki = 0; ki < part.size(); ++ki) {
    int iid = partid[part[ki]];
    for (int kj = ki+1; kj < part.size(); ++kj) {
      int jid = partid[part[kj]];
      for (int ke = 0; ke < fd_impl.size(); ++ke) {
        bool matchI = false;
        bool matchJ = false;

        for (int ll = 0; ll < fd_impl[ke].p.size(); ++ll) {
          if (fd_impl[ke].p[ll] == iid) matchI = true;
          if (fd_impl[ke].p[ll] == jid) matchJ = true;
        }
        if (matchI && matchJ) {
          out_blocks.insert(make_pair(pair<int,int>(ki, kj), fd_impl[ke]));
          conn.insert(make_pair(ki, kj));
          ++nconn;
        }
      }
    }
  }

  // find connections
  int ibefore = 0;
  int iafter = 0;
  do {
    ibefore = iafter;
    int this_nconn = nconn;
    for (map<int, int>::const_iterator ll = conn.begin(); ll != conn.end(); ++ll) {
      if (
    }
    vector<FElements> my_term;
    int order_EW = 2;
    while (order_EW > 0) {
    }
  } while (ibefore < iafter);
  // generate spin permutations
  perm(s, 4);
  for (int ks = 0; ks < s.size(); ++ks) {
    vector<int> &sp = s[ks];
    // print out M^2 terms substituting the spin inputs by sp[i] (TODO)
  }
  o << "  r *= " << pow(2, -n_in) << endl; // average initial spin
  o << "  return r;" << endl;
  o << "}" << endl;
  int s1 = 0; // initial state spins
  int s2 = 0;
  int r1 = 0; // final state spins
  int r2 = 0;
  number M2 = 0;
  matrix v[4];
  matrix vz[4];
  matrix vz2[4];
  for (int k = 0; k < 4; ++k) {
    v[k] = fd_QEDVertex(scale, charge, k);
  }
  for (s1 = 0; s1 < 2; ++s1) { // average over initial state spins (factor of 1/4 below)
    matrix fi0 = fd_Fermion(false, true, s1, in[0]);
    for (s2 = 0; s2 < 2; ++s2) { // average over initial state spins (factor of 1/4 below)
      matrix fi1 = fd_Fermion(false, false, s2, in[1]);
      for (r1 = 0; r1 < 2; ++r1) { // sum over final state spins
        matrix fo1 = fd_Fermion(true, true, r1, out[0]);
        for (r2 = 0; r2 < 2; ++r2) { // sum over final state spins
          matrix fo0 = fd_Fermion(true, false, r2, out[1]);
          // include photon interactions
          number M = 0;
          for (int mu = 0; mu < 4; mu++) {
            for (int nu = 0; nu < 4; nu++) {
              M += mul14_41(mul14_44(fi0, v[mu]), fi1)*fd_QEDPropagatorA(p.M2(), mu, nu)*(mul14_41(mul14_44(fo0, v[nu]), fo1));
              M += mul14_41(mul14_44(fi0, v[mu]), fo1)*fd_QEDPropagatorA(p2.M2(), mu, nu)*(mul14_41(mul14_44(fo0, v[nu]), fi1));
            }
          }

          // include Z interactions
          if (s1 == s2 && r1 == r2) {
            for (int k = 0; k < 4; ++k) {
              vz[k] = fd_QEDVertexZ(scale, charge, s1, k);
              vz2[k] = fd_QEDVertexZ(scale, charge, r2, k);
            }
            for (int mu = 0; mu < 4; mu++) {
              for (int nu = 0; nu < 4; nu++) {
                M += mul14_41(mul14_44(fi0, vz[mu]), fi1)*fd_QEDPropagatorZ(p, mu, nu)*(mul14_41(mul14_44(fo0, vz2[nu]), fo1));//(i1->term()*vz->term()*i2->term())*(propz->term())*(o2->term()*vz2->term()*o1->term());
              }
            }
          }
          if (s1 == r1 && s2 == r2) {
            for (int k = 0; k < 4; ++k) {
              vz[k] = fd_QEDVertexZ(scale, charge, s1, k);
              vz2[k] = fd_QEDVertexZ(scale, charge, r2, k);
            }
            for (int mu = 0; mu < 4; mu++) {
              for (int nu = 0; nu < 4; nu++) {
                M += mul14_41(mul14_44(fi0, vz[mu]), fo1)*fd_QEDPropagatorZ(p2, mu, nu)*(mul14_41(mul14_44(fo0, vz2[nu]), fi1));//(i1->term()*vz->term()*o1->term())*(propz2->term())*(o2->term()*vz2->term()*i2->term());
              }
            }
          }

          M2 += norm(M);
        }
      }
    }
  }
  M2 *= 0.25; // average over spins in initial state

  double amp = M2.real();

  return amp;
}

