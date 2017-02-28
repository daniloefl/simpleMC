package SimpleMC3d.client;

class MCEvent {

  public double [] px;
  public double [] py;
  public double [] pz;
  public double [] e;

  MCEvent() {
    px = new double[4];
    py = new double[4];
    pz = new double[4];
    e  = new double[4];
  }

  void set(int k, double p0, double p1, double p2, double p3) {
    px[k] = p0;
    py[k] = p1;
    pz[k] = p2;
    e[k] = p3;
  }

  double s() {
    return Math.pow(e[2] + e[3], 2) - Math.pow(px[2] + px[3], 2) - Math.pow(py[2] + py[3], 2) - Math.pow(pz[2] + pz[3], 2);
  }
}

