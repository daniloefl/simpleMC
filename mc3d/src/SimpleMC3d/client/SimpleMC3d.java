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

import com.google.gwt.resources.client.ClientBundle;
import com.google.gwt.resources.client.ImageResource;

import com.google.gwt.user.client.ui.ToggleButton;
import com.google.gwt.user.client.ui.FlowPanel;

import thothbot.parallax.core.client.events.AnimationReadyEvent;
import thothbot.parallax.core.client.events.AnimationReadyHandler;
import thothbot.parallax.core.client.events.Context3dErrorEvent;
import thothbot.parallax.core.client.events.Context3dErrorHandler;
import thothbot.parallax.core.client.events.SceneLoadingEvent;
import thothbot.parallax.core.client.events.SceneLoadingHandler;
import com.google.gwt.user.client.ui.Image;
import com.google.gwt.user.client.ui.Label;

import  com.google.gwt.user.client.ui.RequiresResize;

/**
 * This is the entry point for the web application.
 * It shows a 3d representation of e+ e- scattering.
 * The code in C++, in the main simpleMC repository actually simulates the
 * scattering.
 * @author Danilo Ferreira de Lima <daniloefl@gmail.com>
 */
public class SimpleMC3d implements EntryPoint, ResizeHandler, Context3dErrorHandler {

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

  // Panel to draw on
  private RenderingPanel renderingPanel;

  // Resize handler
  @Override
  public void onResize(ResizeEvent event) {
    Scheduler.get().scheduleDeferred(new Scheduler.ScheduledCommand() {
      @Override
      public void execute() {
        renderingPanel.onResize();
      }
    });
  }

  // In case it is not supported
  @Override
  public void onContextError(Context3dErrorEvent event) {
    RootPanel.get("scene").remove(renderingPanel);
    final Label lbl = new Label();
    lbl.setText("Your browser does not support WebGL.");
    RootPanel.get("scene").add(lbl);
  }

  /**
   * This is the entry point method.
   */
  public void onModuleLoad() {
    renderingPanel = new RenderingPanel();
    RootPanel.get("scene").add(renderingPanel);

    // Background color
    renderingPanel.setBackground(0x111111);
    renderingPanel.addCanvas3dErrorHandler(this);

    renderingPanel.setAnimatedScene(new SimpleMC3dScene());

    GChart.setCanvasFactory(new GWTCanvasBasedCanvasFactory());
    m_massChart = new MassChart();
    RootPanel.get("plot").add(m_massChart);

    RootLayoutPanel.get().add(renderingPanel);
    //RootLayoutPanel.get().add(m_massChart);
    updatePlot();
  }

}

