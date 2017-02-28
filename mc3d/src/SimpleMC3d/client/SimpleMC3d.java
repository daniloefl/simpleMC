package SimpleMC3d.client;

import com.googlecode.gchart.client.GChart;

import com.google.gwt.core.client.EntryPoint;
import com.google.gwt.core.client.GWT;
import com.google.gwt.event.dom.client.ClickEvent;
import com.google.gwt.event.dom.client.ClickHandler;
import com.google.gwt.event.dom.client.KeyCodes;
import com.google.gwt.event.dom.client.KeyUpEvent;
import com.google.gwt.event.dom.client.KeyUpHandler;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Button;
import com.google.gwt.user.client.ui.DialogBox;
import com.google.gwt.user.client.ui.HTML;
import com.google.gwt.user.client.ui.Label;
import com.google.gwt.user.client.ui.RootPanel;
import com.google.gwt.user.client.ui.TextBox;
import com.google.gwt.user.client.ui.CheckBox;
import com.google.gwt.user.client.ui.VerticalPanel;

/**
 * This is the entry point for the web application.
 * It shows a 3d representation of e+ e- scattering.
 * The code in C++, in the main simpleMC repository actually simulates the
 * scattering.
 * @author Danilo Ferreira de Lima <daniloefl@gmail.com>
 */
public class SimpleMC3d implements EntryPoint {

  /**
   * Instance of the class used to draw results.
   */
  MassChart m_massChart;

  /**
   * Updates the plot and the contents of all controls if needed.
   */
  public void updatePlot() {
    double [] x = new double [10];
    double [] y = new double [10];
    m_massChart.getMassData(x, y);
    m_massChart.update();
  }

  /**
   * This is the entry point method.
   */
  public void onModuleLoad() {
    GChart.setCanvasFactory(new GWTCanvasBasedCanvasFactory());

    m_massChart = new MassChart();
    RootPanel.get("plot").add(m_massChart);
    
    updatePlot();
  }

}

