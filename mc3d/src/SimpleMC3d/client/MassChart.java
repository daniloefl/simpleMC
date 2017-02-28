package SimpleMC3d.client;

import com.googlecode.gchart.client.GChart;

import java.util.*;

/**
 * Uses GChart to draw the results in the web page.
 * @author Danilo Ferreira de Lima <daniloefl@gmail.com>
 */
public class MassChart extends GChart {

  // Constructor
  PsiChart() {
    setChartTitle("");
    setChartSize(800, 600);
    setLegendYShift(-150);

    addCurve();
    getCurve().setLegendLabel("Mass of the e<sup>+</sup> e<sup>-</sup> system");
    getCurve().setYAxis(Y_AXIS);
    getCurve().getSymbol().setSymbolType(SymbolType.BOX_CENTER); 
    getCurve().getSymbol().setWidth(5);
    getCurve().getSymbol().setHeight(5);
    getCurve().getSymbol().setBorderWidth(0);
    getCurve().getSymbol().setBackgroundColor("navy");
    getCurve().getSymbol().setFillThickness(2);
    getCurve().getSymbol().setFillSpacing(5);

    getXAxis().setAxisLabel("Mass [GeV]");
    getXAxis().setAxisLabelThickness(20);
    getXAxis().setTickCount(13);
    getXAxis().setTicksPerLabel(2);
    getXAxis().setHasGridlines(true);

    getYAxis().setAxisLabel("Count");
    getYAxis().setAxisLabelThickness(20);
    getYAxis().setTickCount(13);
    getYAxis().setTicksPerLabel(2);
    getYAxis().setHasGridlines(true);

  }

  /**
   * Get histogram data and draw it.
   */
  public void getMassData(double [] x, double [] y) {
    getCurve(0).clearPoints();
    for (int k = 0; k < calc.N; ++k) {
      getCurve(0).addPoint(x[k], y[k]);
    }
  }

}

