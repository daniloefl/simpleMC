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
import com.google.gwt.user.client.ui.RootPanel;
import com.google.gwt.user.client.ui.RootLayoutPanel;

import thothbot.parallax.core.client.RenderingPanel;

import com.google.gwt.user.client.Window;
import com.google.gwt.event.logical.shared.ResizeHandler;
import com.google.gwt.core.client.Scheduler;
import com.google.gwt.event.logical.shared.ResizeEvent;

import com.google.gwt.user.client.ui.ToggleButton;

import com.google.gwt.user.client.Timer;

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

  // Contents of the plot
  double [] x;
  double [] y;

  /**
   * Adds an entry in the plot.
   * @param e Event description
   */
  public void addInPlot(MCEvent e) {
    double m = Math.sqrt(e.s());
    for (int k = 0; k < x.length-1; ++k) {
      if (x[k+1] > m) {
        y[k] += 1;
        break;
      }
    }
  }

  /**
   * Updates the plot and the contents of all controls if needed.
   */
  public void updatePlot() {
    m_massChart.getMassData(x, y);
    m_massChart.update();
  }

  // Panel to draw on
  private RenderingPanel renderingPanel;
  private SimpleMC3dScene scene;

  // Object that reads and format input file
  private Reader m_read;

  // Timer
  private Timer timer;

  /**
   * This is the entry point method.
   */
  public void onModuleLoad() {
    int N = 10;
    double xmin = 70;
    double dx = 5;
    x = new double[N];
    y = new double[N];
    for (int k = 0; k < N; ++k) {
      x[k] = xmin + k*dx;
      y[k] = 0;
    }

    m_read = new Reader("simulations.txt");

    renderingPanel = new RenderingPanel();
    RootPanel.get("scene").add(renderingPanel);

    Window.addResizeHandler(new ResizeHandler() {
      @Override
      public void onResize(ResizeEvent event) {
        Scheduler.get().scheduleDeferred(new Scheduler.ScheduledCommand() {
          @Override
          public void execute() {
            renderingPanel.onResize();
          }
        });
      }
    });

    // timer
    timer = new Timer() {
      @Override
      public void run() {
        MCEvent e = m_read.getNext();
        scene.addInScene(e);
        addInPlot(e);
        updatePlot();
        timer.schedule(5000);
      }
    };
    timer.schedule(1);

    // Background color
    renderingPanel.setBackground(0x111111);
    scene = new SimpleMC3dScene();
    renderingPanel.setAnimatedScene(scene);

    GChart.setCanvasFactory(new GWTCanvasBasedCanvasFactory());
    m_massChart = new MassChart();
    RootPanel.get("plot").add(m_massChart);
    updatePlot();
  }

}

